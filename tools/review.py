#!/usr/bin/env python3
"""Opus review sampling: an independent read of committed functions against the original, per module.

The gate checks references and the ABI, the verifier checks that the game survives and (for leaf/pure-callee
functions) that results match. Neither reads the logic of a large function with calls. This samples a share of
each module's done functions (default 5%, weighted to big ones without diff evidence), has a strong model review
each against its packet (disassembly + pseudocode + callees, the same packet a grunt gets) with grunt's
REVIEW_SYSTEM, and records the verdicts:

  db/reviews.jsonl   {ea, module, code (compiled-code hash), ok, issues, model, date}
  queue/<ea>.md      for a rejected function (the file stays: it may be verified in game; this is a fix request)

The per-module rejection rate is the quality estimate for the module's unreviewed code; recurring issue kinds
are what the grunt prompt should learn (`report`).

Usage:
  review.py sample [--module M,..] [--pct 5] [--min 3] [--max 20] [--model claude-opus-5-5] [--effort high] [--dry]
  review.py report [--module M]
Runs on the grunts' Claude account (grunt.ask backend claude; never the main account).
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


def pick(mods, pct, lo, hi, seed=None):
    """{module: [ea...]} sample: done src/auto functions not reviewed with their current code, >= 40 bytes,
    weighted by size, diffed ones last (they already have the strongest evidence)."""
    import verify
    rows = list(csv.DictReader(open(os.path.join(ROOT, "db", "functions.csv"))))
    names = module_names()
    hashes = verify.code_hashes()
    tiers = verify.tiers(hashes)
    reviewed = {(r["ea"], r.get("code")) for r in load()}
    by = collections.defaultdict(list)
    for r in rows:
        ea = r["ea"]
        if r["status"] != "done" or not os.path.exists(os.path.join(ROOT, "src", "auto", f"{ea}.cpp")):
            continue
        m = r["module"] or "?"
        if mods and m not in mods and names.get(m) not in mods:
            continue
        by[m].append(r)
    rnd = random.Random(seed)
    out = {}
    for m, rs in by.items():
        n = max(lo, min(hi, round(len(rs) * pct / 100)))
        pool = [r for r in rs if int(r["size"]) >= 40 and (r["ea"], hashes.get(r["ea"])) not in reviewed]
        weight = lambda r: int(r["size"]) * (0.2 if tiers.get(r["ea"]) == "diffed" else 1.0)
        chosen = []
        while pool and len(chosen) < n:
            tot = sum(weight(r) for r in pool)
            x, acc = rnd.uniform(0, tot), 0
            for r in pool:
                acc += weight(r)
                if acc >= x:
                    chosen.append(r["ea"])
                    pool.remove(r)
                    break
        if chosen:
            out[m] = chosen
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
    chosen, hashes = pick(mods, a.pct, a.min, a.max, a.seed)
    total = sum(len(v) for v in chosen.values())
    names = module_names()
    print(f"review sample: {total} functions in {len(chosen)} modules ({a.model} @ {a.effort})", flush=True)
    if a.dry:
        for m, eas in sorted(chosen.items()):
            print(f"  {names.get(m, m)}: {' '.join(eas)}")
        return
    db = DB()
    import concurrent.futures as cf
    jobs = [(m, ea) for m, eas in chosen.items() for ea in eas]
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
            rec = dict(ea=ea, module=m, code=hashes.get(ea), ok=ok, issues=issues, model=a.model, date=today)
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
        by[m][0] += 1
        if not r["ok"]:
            by[m][1] += 1
            issues += [(r["ea"], i) for i in r["issues"]]
    n, b = sum(x[0] for x in by.values()), sum(x[1] for x in by.values())
    print(f"reviewed {n}, rejected {b} ({100 * b / max(n, 1):.0f}%): estimated defect rate of unreviewed code")
    for m, (k, bad) in sorted(by.items(), key=lambda x: -x[1][1]):
        print(f"  {names.get(m, m):32s} {bad}/{k} rejected")
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
    s.add_argument("--pct", type=float, default=5)
    s.add_argument("--min", type=int, default=3)
    s.add_argument("--max", type=int, default=20)
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
