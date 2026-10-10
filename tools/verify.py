#!/usr/bin/env python3
"""Verification ledger: how strongly each reimplemented function has been checked, tied to its compiled code.

db/verify.json, per src/auto function: {"code": hash of the compiled code, "hits": max calls in one smoke,
"runs": smokes that executed it, "diffed": differential comparisons, "mismatched": mismatching ones,
"since": date this code was first seen, "last": date of the last evidence}.
The code hash covers the function's own object code (instruction bytes + relocation types; symbol names, the HOOK
registration stub and the file's other metadata are left out), so renames don't invalidate evidence but any change
to what the function compiles to does — e.g. a class layout change in types/auto_classes.h. Evidence recorded for
another hash is STALE: the code that was exercised is not the code that runs now.

Tiers of a done function (strongest first): diffed (compared in-game against the original, 0 mismatches) >
executed (ran in a smoke without crashing) > gate (static checks only; never reached in a smoke) ; plus stale and
mismatched.

Pending: a done function whose current code was never *installed* in a passing smoke ("ok" = passing smokes with
its hook installed, read from csf_hook.log). Grunt lanes commit gated functions without smoking; every deploy
disables the pending ones (`pending --ini`), so the game always runs the verified baseline, and tools/verifier.py
enables them a module group at a time, smokes, bisects, and quarantines what breaks.

Usage:
  verify.py ingest          read the play copy's csf_hook_hits.txt + csf_diff_summary.txt (smoke.sh runs this after
                            SMOKE OK; CSF_NO_LEDGER=1 skips it) — the build dir must be what was deployed
  verify.py report [--list TIER]   tier counts (functions and bytes); --list prints the addresses of one tier
  verify.py stale           rebuild, list functions whose code changed since their evidence (exit 1 if any)
  verify.py pending [--ini PATH] [--except FILE]   list pending functions / write them as `disable` lines tagged
                            `# pending` into the play copy's ini (except the addresses in FILE: a verify group)
  verify.py reset WHY EA...  drop the diff evidence of unchanged code whose diff was wrong (harness / table fix)
  verify.py bootstrap OLD_BUILD_DIR   mark functions verified whose compiled code is identical in OLD_BUILD_DIR
                            (the build the last passing lanes smoke ran) - once, when the ok field was introduced

CSF_TREE=<checkout> reads that checkout's build objects and sources (tools/verifier.py builds a detached tree).
"""
import argparse, concurrent.futures as cf, csv, datetime, glob, hashlib, json, os, re, subprocess, sys

ROOT = os.path.dirname(os.path.dirname(os.path.abspath(__file__)))
LEDGER = os.path.join(ROOT, "db", "verify.json")
CACHE = os.path.join(ROOT, "work", "codehash_cache.json")
TREE = os.environ.get("CSF_TREE", ROOT)  # the checkout whose build was deployed (verifier: its own detached tree)
OBJ_DIR = os.path.join(TREE, "build", "CMakeFiles", "csf_hook.dir", "src", "auto")
PLAY = os.environ.get("CSF_PLAY", "/mnt/e/dev/csf-play")
TIERS = ["diffed", "executed", "gate", "stale", "mismatched"]


def _normalize(dis):
    out, skip = [], False
    for line in dis.splitlines()[3:]:  # header: file format, blank, "Disassembly of section"
        m = re.match(r"^[0-9a-f]+ <(.+)>:$", line)
        if m:
            skip = m.group(1).startswith("??__E")  # HOOK registration stub (carries the name and a counter)
            out.append("<fn>")
            continue
        if skip or not line.strip():
            continue
        if line.startswith("Disassembly of section"):
            out.append(line)
            continue
        if "IMAGE_REL_" in line:
            out.append(re.search(r"IMAGE_REL_\w+", line).group(0))  # relocation kind, not the symbol name
            continue
        out.append(re.sub(r"\s*<[^>]*>", "", line.split(":", 1)[1] if ":" in line else line).strip())
    return "\n".join(out)


def code_hash(obj):
    dis = subprocess.run(["llvm-objdump-21", "-d", "-r", "--x86-asm-syntax=intel", obj],
                         capture_output=True, text=True).stdout
    return hashlib.sha1(_normalize(dis).encode()).hexdigest()[:16]


def code_hashes(obj_dir=None, tree=None):
    """{ea: hash} for every src/auto function with a built object (cached by object mtime+size)."""
    obj_dir, tree = obj_dir or OBJ_DIR, tree or TREE
    try:
        cache = json.load(open(CACHE))
    except (OSError, ValueError):
        cache = {}
    objs = sorted(glob.glob(os.path.join(obj_dir, "*.cpp.obj")))

    def one(obj):
        st = os.stat(obj)
        key = f"{st.st_mtime_ns}:{st.st_size}"
        c = cache.get(obj)
        return obj, key, c[1] if c and c[0] == key else code_hash(obj)

    out = {}
    with cf.ThreadPoolExecutor(16) as ex:
        for obj, key, h in ex.map(one, objs):
            cache[obj] = [key, h]
            src = os.path.join(tree, "src", "auto", os.path.basename(obj)[:-4])
            if os.path.exists(src):
                out[f"{os.path.basename(obj)[:8]}"] = h
    os.makedirs(os.path.dirname(CACHE), exist_ok=True)
    json.dump(cache, open(CACHE, "w"))
    return out


def load():
    try:
        return json.load(open(LEDGER))
    except (OSError, ValueError):
        return {}


def save(led):
    with open(LEDGER, "w") as f:
        json.dump(dict(sorted(led.items())), f, indent=0, separators=(",", ":"))
        f.write("\n")


def _read(name):
    p = os.path.join(PLAY, name)
    return open(p, errors="replace").read().replace("\r", "") if os.path.exists(p) else ""


def ingest():
    hashes = code_hashes()
    with ledger_lock():  # smoke pool: several smokes may end at once
        _ingest(hashes)


def ledger_lock():
    """Exclusive lock around a load() .. save() of db/verify.json (released when the returned file closes)."""
    import fcntl
    lk = open(os.path.join(ROOT, "work", "ledger.lock"), "w")
    fcntl.flock(lk, fcntl.LOCK_EX)
    return lk


def _ingest(hashes):
    led = load()
    today = datetime.date.today().isoformat()
    hits = {}
    for line in _read("csf_hook_hits.txt").splitlines():
        f = line.split()
        if len(f) >= 2 and re.fullmatch(r"[0-9a-fA-F]{8}", f[0]):
            hits[f[0].lower()] = int(f[1])
    diffs = {}
    for line in _read("csf_diff_summary.txt").splitlines():
        f = line.split()
        if len(f) >= 4 and re.fullmatch(r"[0-9a-fA-F]{8}", f[0]):
            # "<ea> N compared M mismatched [K inconclusive] name": inconclusive tier-3 runs are no evidence
            inc = int(f[5]) if len(f) > 6 and f[6] == "inconclusive" else 0
            diffs[f[0].lower()] = (int(f[1]), int(f[3]), inc)
    # hooks installed in this (passing) smoke: "hook 00543e10 auto Name" / "diff 00543e10 auto Name"
    installed = {m.group(1).lower() for m in re.finditer(r"^(?:\S+ )?(?:hook|diff) ([0-9a-fA-F]{8}) ",
                                                         _read("csf_hook.log"), re.M)}
    n_new = n_ev = n_ok = 0
    for ea, h in hashes.items():
        e = led.get(ea)
        if e is None or e["code"] != h:  # new code: earlier evidence was for other code
            e = led[ea] = dict(code=h, hits=0, runs=0, diffed=0, mismatched=0, since=today, last=None)
            n_new += 1
        if ea in installed:
            e["ok"] = e.get("ok", 0) + 1
            n_ok += 1
        if hits.get(ea, 0) > 0:
            e["hits"] = max(e["hits"], hits[ea])
            e["runs"] += 1
            e["last"] = today
            n_ev += 1
        if ea in diffs:
            e["diffed"] += diffs[ea][0]
            e["mismatched"] += diffs[ea][1]
            if diffs[ea][2]:
                e["inconclusive"] = e.get("inconclusive", 0) + diffs[ea][2]
    save(led)
    print(f"verify: {n_ok} hooks installed, {n_ev} functions executed in this smoke, "
          f"{sum(1 for d in diffs.values() if d[0])} diffed, {sum(1 for d in diffs.values() if d[2])} with "
          f"inconclusive tier-3 runs; "
          f"{n_new} new code hashes")


def reset(eas, why):
    """Drop the diff evidence (diffed / mismatched / inconclusive) of functions whose code is unchanged but whose
    diff was wrong: a harness or diff-table fix (e.g. a return the original leaves undefined). The next smokes
    re-record it. Logged in the entry (`reset`)."""
    today = datetime.date.today().isoformat()
    with ledger_lock():
        led = load()
        for ea in eas:
            ea = ea.lower().replace("0x", "").zfill(8)
            e = led.get(ea)
            if not e:
                print(f"{ea}: not in the ledger")
                continue
            print(f"{ea}: diffed {e['diffed']} mismatched {e['mismatched']} -> 0")
            e["diffed"] = e["mismatched"] = 0
            e.pop("inconclusive", None)
            e["reset"] = f"{today}: {why}"
        save(led)


def done_rows():
    with open(os.path.join(ROOT, "db", "functions.csv")) as f:
        return {r["ea"].lower().replace("0x", "").zfill(8): r for r in csv.DictReader(f) if r["status"] == "done"}


def tiers(hashes=None):
    hashes = hashes if hashes is not None else code_hashes()
    led = load()
    out = {}
    for ea in hashes:
        e = led.get(ea)
        if e is None:
            t = "gate"
        elif e["code"] != hashes[ea]:
            t = "stale" if e["runs"] or e["diffed"] else "gate"
        elif e["mismatched"]:
            t = "mismatched"
        elif e["diffed"]:
            t = "diffed"
        elif e["runs"]:
            t = "executed"
        else:
            t = "gate"
        out[ea] = t
    return out


PENDING_TAG = "# pending"


def pending(hashes=None):
    """Done functions (built in TREE) whose current code was never installed in a passing smoke."""
    hashes = hashes if hashes is not None else code_hashes()
    led, rows = load(), done_rows()
    auto = {ea for ea in rows if os.path.exists(os.path.join(TREE, "src", "auto", f"{ea}.cpp"))}  # not src/core
    return sorted(ea for ea in auto if ea not in hashes or led.get(ea, {}).get("code") != hashes[ea]
                  or not led[ea].get("ok"))


def write_pending_ini(ini, keep_on=()):
    """Replace the `# pending` disable lines of the play copy's ini; addresses in keep_on stay enabled."""
    keep = {k.lower().replace("0x", "").zfill(8) for k in keep_on}
    lines = [l for l in (open(ini).read().splitlines() if os.path.exists(ini) else []) if not l.endswith(PENDING_TAG)]
    pend = [ea for ea in pending() if ea not in keep]
    lines += [f"disable 0x{ea.upper()} {PENDING_TAG}" for ea in pend]
    open(ini, "w").write("\n".join(lines) + "\n")
    return pend


def bootstrap(old_build):
    """One-time: the ok field is new. Every function committed by lanes so far passed a smoke installed; its code
    still counts as smoked if it compiles to the same code as in the build that smoke ran (old_build)."""
    old = code_hashes(os.path.join(old_build, "CMakeFiles", "csf_hook.dir", "src", "auto"), TREE)
    cur = code_hashes()
    led = load()
    today = datetime.date.today().isoformat()
    n = changed = 0
    for ea in done_rows():
        if ea not in cur:
            continue
        if old.get(ea) != cur[ea]:
            changed += 1
            continue
        e = led.get(ea)
        if e is None or e["code"] != cur[ea]:
            e = led[ea] = dict(code=cur[ea], hits=0, runs=0, diffed=0, mismatched=0, since=today, last=None)
        if not e.get("ok"):
            e["ok"] = 1
            n += 1
    save(led)
    print(f"bootstrap: {n} functions marked smoked (same code as {old_build}); {changed} changed since -> pending")


def summary_lines(hashes=None):
    """Short table for status.py: tier, functions, bytes."""
    t = tiers(hashes)
    rows = done_rows()
    size = {ea: int(r["size"]) for ea, r in rows.items()}
    lines = []
    what = {"diffed": "results compared with the original's in game", "executed": "ran in a smoke without crashing",
            "gate": "never ran: static checks only (installed or not)", "stale": "code changed since its evidence",
            "mismatched": "differs from the original"}
    for name in TIERS:
        eas = [ea for ea, v in t.items() if v == name]
        if eas or name in ("diffed", "executed", "gate"):
            lines.append(f"  {name:11} {len(eas):6} fns {sum(size.get(e, 0) for e in eas) / 1024:7.1f} KB   {what[name]}")
    pend = pending(hashes)
    lines.append(f"  {'pending':11} {len(pend):6} fns {sum(size.get(e, 0) for e in pend) / 1024:7.1f} KB"
                 "   (never installed in a passing smoke; disabled in deploys until tools/verifier.py)")
    return lines


def main():
    ap = argparse.ArgumentParser()
    sub = ap.add_subparsers(dest="cmd", required=True)
    sub.add_parser("ingest")
    r = sub.add_parser("report")
    r.add_argument("--list", choices=TIERS)
    sub.add_parser("stale")
    p = sub.add_parser("pending")
    p.add_argument("--ini")
    p.add_argument("--except", dest="exc", help="file of addresses to keep enabled (a verify group)")
    rs = sub.add_parser("reset")
    rs.add_argument("why")
    rs.add_argument("eas", nargs="+")
    b = sub.add_parser("bootstrap")
    b.add_argument("old_build")
    a = ap.parse_args()
    if a.cmd == "pending":
        if a.ini:
            keep = open(a.exc).read().split() if a.exc and os.path.exists(a.exc) else []
            pend = write_pending_ini(a.ini, keep)
            print(f"{len(pend)} pending functions disabled in {a.ini} ({len(keep)} kept on)")
        else:
            print("\n".join(pending()))
        return
    if a.cmd == "reset":
        reset(a.eas, a.why)
        return
    if a.cmd == "bootstrap":
        bootstrap(a.old_build)
        return
    if a.cmd == "ingest":
        ingest()
    elif a.cmd == "report":
        if a.list:
            rows = done_rows()
            for ea, v in sorted(tiers().items()):
                if v == a.list:
                    r = rows.get(ea, {})
                    print(f"{ea} {r.get('size', '?'):>5} {r.get('name', '')}")
        else:
            print("\n".join(summary_lines()))
    elif a.cmd == "stale":
        r = subprocess.run(["ninja", "-C", os.path.join(ROOT, "build")], capture_output=True, text=True)
        if r.returncode:
            sys.exit("build failed:\n" + (r.stdout + r.stderr)[-2000:])
        st = [ea for ea, v in sorted(tiers().items()) if v == "stale"]
        for ea in st:
            print(f"stale {ea}")
        print(f"{len(st)} functions changed since their evidence: re-smoke (smoke.sh), the next ingest re-records them")
        sys.exit(1 if st else 0)


if __name__ == "__main__":
    main()
