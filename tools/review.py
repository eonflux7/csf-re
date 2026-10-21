#!/usr/bin/env python3
"""Opus review sampling: an independent read of committed functions against the original.

The gate checks references and the ABI, the verifier checks that the game survives and (for leaf/pure-callee
functions) that results match. Neither reads the logic of a large function with calls. This is one rolling sample
over all done functions, aimed where there is no other evidence: functions that never ran in a smoke (tier
`gate`) weigh 5x, executed ones 1x, diffed ones 0.2x, each times its size. A strong model reviews each against
its packet (disassembly + pseudocode + callees, the same packet a grunt gets) with grunt's REVIEW_SYSTEM:

  db/reviews.jsonl   {ea, module, code (compiled-code hash), ok, issues, model, date}
  queue/<ea>.md      for a rejected function (the file stays: it may be verified in game; this is a fix request)

The rejection rate per evidence tier (`report`) is the quality estimate for unreviewed code of that tier; a
module's handful of reviews says nothing about the module. Recurring issue kinds are what the grunt prompt
should learn.

Usage:
  review.py sample [--n 20] [--module M,..] [--model claude-opus-5-5] [--effort high] [--dry]
  review.py report [--module M]
Runs through grunt.ask backend claude (the agent config, CLAUDE_GRUNTS).
"""
import argparse, collections, csv, datetime, json, os, random, re, sys

ROOT = os.path.dirname(os.path.dirname(os.path.abspath(__file__)))
sys.path.insert(0, os.path.dirname(__file__))
REVIEWS = os.path.join(ROOT, "db", "reviews.jsonl")


def load():
    if not os.path.exists(REVIEWS):
        return []
    return [json.loads(l) for l in open(REVIEWS) if l.strip()]


def module_names():
    import yaml
    p = os.path.join(ROOT, "db", "modules.yaml")
    return {m["id"]: m.get("name") or m["id"] for m in (yaml.safe_load(open(p)) or [])} if os.path.exists(p) else {}


TIER_WEIGHT = {"gate": 5.0, "executed": 1.0, "stale": 1.0, "diffed": 0.2}


def pick(mods, n, seed=None):
    """[(module, ea, tier)]: n done src/auto functions not reviewed with their current code, >= 40 bytes, drawn
    without replacement with weight size x TIER_WEIGHT (the least in-game evidence first)."""
    import verify
    rows = list(csv.DictReader(open(os.path.join(ROOT, "db", "functions.csv"))))
    names = module_names()
    hashes = verify.code_hashes()
    tiers = verify.tiers(hashes)
    reviewed = {(r["ea"], r.get("code")) for r in load()}
    pool = []
    for r in rows:
        ea, m = r["ea"], r["module"] or "?"
        if r["status"] != "done" or ea not in hashes or int(r["size"]) < 40 or (ea, hashes[ea]) in reviewed:
            continue
        if mods and m not in mods and names.get(m) not in mods:
            continue
        w = int(r["size"]) * TIER_WEIGHT.get(tiers.get(ea), 1.0)
        pool.append((w, m, ea, tiers.get(ea, "gate")))
    rnd = random.Random(seed)
    out = []
    while pool and len(out) < n:
        x, acc = rnd.uniform(0, sum(p[0] for p in pool)), 0
        for p in pool:
            acc += p[0]
            if acc >= x:
                out.append(p[1:])
                pool.remove(p)
                break
    return out, hashes


def review_one(db, ea, model, effort):
    import grunt, packet
    grunt._tl.ea = int(ea, 16)  # transcripts land in work/calls/<ea>/
    pk = packet.build(db, int(ea, 16))
    code = open(os.path.join(ROOT, "src", "auto", f"{ea}.cpp")).read()
    msgs = [{"role": "system", "content": grunt.REVIEW_SYSTEM},
            {"role": "user", "content": f"{pk}\n\n## Reimplementation under review\n```cpp\n{code}\n```"}]
    text, usage = grunt.ask("claude", model, msgs, None, effort)
    m = re.search(r"```json\s*\n(.*?)```", text, re.S)
    try:
        d = json.loads(m.group(1)) if m else {}
    except json.JSONDecodeError:
        d = {}
    if "ok" not in d:
        return None, text, usage
    return d, text, usage


def sample(a):
    from packet import DB
    mods = set(a.module.split(",")) if a.module else None
    chosen, hashes = pick(mods, a.n, a.seed)
    total = len(chosen)
    names = module_names()
    by_tier = collections.Counter(t for _, _, t in chosen)
    print(f"review sample: {total} functions ({', '.join(f'{k} {v}' for k, v in by_tier.most_common())}; "
          f"{a.model} @ {a.effort})", flush=True)
    if a.dry:
        for m, ea, tier in sorted(chosen):
            print(f"  {ea} {tier:8s} {names.get(m, m)}")
        return
    db = DB()
    import concurrent.futures as cf
    tier_of = {ea: tier for _, ea, tier in chosen}
    jobs = [(m, ea) for m, ea, _ in chosen]
    today = datetime.date.today().isoformat()
    bad = []
    with cf.ThreadPoolExecutor(a.jobs) as ex:
        futs = {ex.submit(review_one, db, ea, a.model, a.effort): (m, ea) for m, ea in jobs}
        for f in cf.as_completed(futs):
            m, ea = futs[f]
            try:
                d, text, usage = f.result()
            except Exception as e:
                print(f"  {ea}: review failed ({e})", flush=True)
                continue
            if d is None:
                print(f"  {ea}: no verdict", flush=True)
                continue
            issues = [str(i) for i in d.get("issues") or []]
            ok = bool(d.get("ok", True)) or not issues
            rec = dict(ea=ea, module=m, code=hashes.get(ea), tier=tier_of[ea], ok=ok, issues=issues, model=a.model,
                       date=today)
            with open(REVIEWS, "a") as fh:
                fh.write(json.dumps(rec) + "\n")
            print(f"  {ea} {names.get(m, m)}: {'ok' if ok else 'ISSUES: ' + ' | '.join(issues)[:200]}", flush=True)
            if not ok:
                bad.append(ea)
                code = open(os.path.join(ROOT, "src", "auto", f"{ea}.cpp")).read()
                with open(os.path.join(ROOT, "queue", f"{ea}.md"), "w") as fh:
                    fh.write(f"# Escalation 0x{ea.upper()}: Opus review found behavior differences (file kept)\n\n"
                             + "\n".join(f"- {i}" for i in issues) + f"\n\n## Current file\n```cpp\n{code}\n```\n")
    import repo
    files = ["db/reviews.jsonl"] + [f"queue/{ea}.md" for ea in bad]
    repo.commit(files, f"review sample: {total} functions reviewed, {len(bad)} with issues")
    report(mods)


def report(mods=None):
    names = module_names()
    recs = load()
    by = collections.defaultdict(lambda: [0, 0])
    issues = []
    for r in recs:
        m = r.get("module") or "?"
        if mods and m not in mods and names.get(m) not in mods:
            continue
        t = r.get("tier") or "(tier not recorded)"
        by[t][0] += 1
        if not r["ok"]:
            by[t][1] += 1
            issues += [(r["ea"], i) for i in r["issues"]]
    n, b = sum(x[0] for x in by.values()), sum(x[1] for x in by.values())
    print(f"reviewed {n}, rejected {b} ({100 * b / max(n, 1):.0f}%): estimated defect rate of unreviewed code")
    for t, (k, bad) in sorted(by.items(), key=lambda x: -x[1][0]):
        print(f"  {t:20s} {bad}/{k} rejected")
    # recurring kinds: crude keyword buckets, for the prompt
    kinds = collections.Counter()
    for _, i in issues:
        for k, pat in [("signedness", r"signed|unsigned|jl|jg|jb|ja"), ("offset", r"offset|\+0x|field"),
                       ("call order/missing call", r"call|order"), ("width/truncation", r"width|byte|word|trunc"),
                       ("float/double", r"float|double|fpu|x87"), ("reload/caching", r"reload|cache|re-read"),
                       ("loop/bounds", r"loop|bound|index"), ("constant", r"constant|immediate")]:
            if re.search(pat, i, re.I):
                kinds[k] += 1
    if kinds:
        print("recurring issue kinds: " + ", ".join(f"{k} {n}" for k, n in kinds.most_common()))


def main():
    ap = argparse.ArgumentParser()
    sub = ap.add_subparsers(dest="cmd", required=True)
    s = sub.add_parser("sample")
    s.add_argument("--module")
    s.add_argument("--n", type=int, default=20, help="functions to review")
    s.add_argument("--model", default="claude-opus-5-5")
    s.add_argument("--effort", default="high")
    s.add_argument("--jobs", type=int, default=4)
    s.add_argument("--seed", type=int)
    s.add_argument("--dry", action="store_true")
    r = sub.add_parser("report")
    r.add_argument("--module")
    a = ap.parse_args()
    if a.cmd == "sample":
        sample(a)
    else:
        report(set(a.module.split(",")) if a.module else None)


if __name__ == "__main__":
    main()
