#!/usr/bin/env python3
"""Negative controls for the safety nets: known-bad reimplementations must be rejected, for the right reason.

Three kinds of check (static ones run the real gate, nothing is deployed):
  fixtures   tests/controls/<ea>_<kind>.cpp — real bugs from history (git) or hand-written replicas of crashes.
             CAUGHT = gate rejected it with the kind's expected message; MISSED = the gate passed it (a hole);
             INVALID = rejected for another reason (e.g. the fixture no longer compiles: it rotted, fix the fixture).
  mutants    generated from done src/auto files by mutation operators (wrong offset/callee/string/convention, dropped
             call, ...). Mutants that don't compile are skipped. Survivors are listed: either a gate gap or an
             equivalent mutant (e.g. the shifted offset is one the original also uses).
  positive   the current src/auto file of every fixture address must still pass (the gate isn't just rejecting all).
Kinds marked live-only (constant changes, ...) are invisible to static checks by design; `--live` checks that the
in-game differential harness catches them: mutants are swapped into src/auto, built, deployed and smoked, MISMATCH
expected for each; the original files are restored and redeployed afterwards.

Usage: controls.py [--n 25] [--jobs 12] [--only fixtures|mutants|positive] [--all] [-v]
       controls.py --live            (needs the play copy; ~1 min)
Exit 1 if any fixture is MISSED/INVALID, a positive control fails, or (with --live) a live mutant goes undetected.
"""
import argparse, concurrent.futures as cf, glob, os, re, shutil, subprocess, sys, tempfile
sys.path.insert(0, os.path.dirname(__file__))
from packet import DB
from gate import gate
from gen_classes_h import load_names

ROOT = os.path.dirname(os.path.dirname(os.path.abspath(__file__)))
FIX_DIR = os.path.join(ROOT, "tests", "controls")
AUTO = os.path.join(ROOT, "src", "auto")

REF = r"never does|which the original does not"
# kind -> regex the gate error must match (None: live-only, static checks can't see it)
KINDS = {
    "tailcall": r"TAIL-JUMPS",
    "embedded": r"ADDRESS this\+0x",
    "boolret": r"BOOL",
    "offset": r"field offsets the original never uses|ADDRESS this\+0x",
    "callee": REF + r"|is not a function start|declared \w+ popping",
    "dropcall": REF,
    "global": REF,
    "string": REF,
    "convention": r"calling convention mismatch",
    "const": None,
}


def _hooked(src):
    m = re.search(r"HOOK(?:_METHOD|_KEEP_ECX)?\(\s*0x[0-9A-Fa-f]+\s*,\s*&?([\w:]+)", src)
    return m.group(1).split("::")[-1] if m else None


def _bump(m, delta, group=1):
    s = m.group(0)
    a, b = m.span(group)
    a, b = a - m.start(), b - m.start()
    return s[:a] + f"0x{int(m.group(group), 16) + delta:X}" + s[b:]


def mut_offset(src):
    m = re.search(r"CSF_AT\(([^,]+),\s*[^,]+,\s*(0x[0-9A-Fa-f]+)\)", src)
    return src[:m.start()] + _bump(m, 4, 2) + src[m.end():] if m else None


def mut_callee(src):
    m = re.search(r"CSF_FUNC\([^;]*?,\s*(0x[0-9A-Fa-f]+)\s*,\s*\(", src)
    return src[:m.start()] + _bump(m, 0x10) + src[m.end():] if m else None


def mut_global(src):
    m = re.search(r"CSF_GLOBAL\([^;]*?,\s*(0x[0-9A-Fa-f]+)\s*\)", src)
    return src[:m.start()] + _bump(m, 0x40) + src[m.end():] if m else None


def mut_string(src):
    m = re.search(r"CSF_STR\(\s*(0x[0-9A-Fa-f]+)", src)
    return src[:m.start()] + _bump(m, 1) + src[m.end():] if m else None


def mut_dropcall(src):
    names = re.findall(r"CSF_FUNC\([^,]+,\s*\w+,\s*(\w+)\s*,", src)
    for n in names:
        calls = list(re.finditer(rf"^[ \t]*{n}\([^;\n]*\);[ \t]*\n", src, re.M))
        if len(calls) == 1 and len(re.findall(rf"\b{n}\(", src)) == 1:
            return src[:calls[0].start()] + src[calls[0].end():]
    return None


def mut_convention(src):
    fn = _hooked(src)
    if not fn:
        return None
    m = re.search(rf"\b(__stdcall|__cdecl)(\s+{re.escape(fn)}\s*\(([^)]*)\))", src)
    if not m or m.group(3).strip() in ("", "void"):
        return None
    other = "__cdecl" if m.group(1) == "__stdcall" else "__stdcall"
    return src[:m.start(1)] + other + src[m.end(1):]


def mut_const(src):
    m = re.search(r"\breturn\s+(0x[0-9A-Fa-f]+|[1-9]\d*)\s*;", src)
    if not m:
        return None
    v = int(m.group(1), 0)
    return src[:m.start(1)] + str(v + 1) + src[m.end(1):]


OPERATORS = {"offset": mut_offset, "callee": mut_callee, "global": mut_global, "string": mut_string,
             "dropcall": mut_dropcall, "convention": mut_convention}


def judge(kind, ok, err):
    if ok:
        return "MISSED"
    if err.startswith("compile error"):
        return "INVALID"
    return "CAUGHT" if re.search(KINDS[kind], err) else "INVALID"


def current_names(src):
    """Apply today's class names (db/types/classes.yaml) to a fixture written before a rename pass."""
    names = load_names()
    return re.sub(r"\bC_([0-9a-f]{8})\b", lambda m: names[m.group(1)]["name"] if m.group(1) in names else m.group(0), src)


def fixtures():
    out = []
    for p in sorted(glob.glob(os.path.join(FIX_DIR, "*.cpp"))):
        m = re.match(r"([0-9a-f]{8})_(\w+)\.cpp$", os.path.basename(p))
        if m and m.group(2) in KINDS:
            out.append((int(m.group(1), 16), m.group(2), p))
    return out


def run_fixtures(db, jobs, verbose):
    def one(t):
        ea, kind, p = t
        if KINDS[kind] is None:
            return ea, kind, p, "LIVE", ""
        with tempfile.TemporaryDirectory() as td:
            tp = os.path.join(td, os.path.basename(p))
            open(tp, "w").write(current_names(open(p).read()))
            ok, err = gate(ea, tp, db)
        return ea, kind, p, judge(kind, ok, err), err
    bad = 0
    with cf.ThreadPoolExecutor(jobs) as ex:
        res = list(ex.map(one, fixtures()))
    counts = {}
    for ea, kind, p, verdict, err in res:
        counts.setdefault(kind, {}).setdefault(verdict, 0)
        counts[kind][verdict] += 1
        if verdict in ("MISSED", "INVALID"):
            bad += 1
        if verdict != "CAUGHT" or verbose:
            first = err.splitlines()[1 if err.startswith(("compile", "semantic")) else 0][:150] if err else ""
            print(f"  {verdict:8} {os.path.relpath(p, ROOT)}  {first}")
    for kind, c in sorted(counts.items()):
        print(f"fixtures {kind:10} " + "  ".join(f"{k} {v}" for k, v in sorted(c.items())))
    return bad


def run_positive(db, jobs, everything=False):
    eas = sorted({ea for ea, _, _ in fixtures()} if not everything else
                 {int(os.path.basename(p)[:8], 16) for p in glob.glob(os.path.join(AUTO, "*.cpp"))})
    def one(ea):
        p = os.path.join(AUTO, f"{ea:08x}.cpp")
        return (ea, *gate(ea, p, db)) if os.path.exists(p) else (ea, True, "no file")
    bad = 0
    with cf.ThreadPoolExecutor(jobs) as ex:
        for ea, ok, err in ex.map(one, eas):
            if not ok:
                bad += 1
                print(f"  POSITIVE FAIL 0x{ea:X}: {err.splitlines()[-1][:150]}")
    print(f"positive  {len(eas) - bad}/{len(eas)} current files pass the gate")
    return bad


def run_mutants(db, n, jobs, verbose):
    files = sorted(glob.glob(os.path.join(AUTO, "*.cpp")))
    tasks = []
    for kind, op in OPERATORS.items():
        cands = []
        for p in files:
            src = open(p).read()
            mutant = op(src)
            if mutant and mutant != src:
                cands.append((int(os.path.basename(p)[:8], 16), mutant))
        step = max(1, len(cands) // n)  # deterministic spread over the address space
        tasks += [(kind, ea, mutant) for ea, mutant in cands[::step][:n]]

    def one(t):
        kind, ea, mutant = t
        with tempfile.TemporaryDirectory() as td:
            p = os.path.join(td, f"{ea:08x}.cpp")
            open(p, "w").write(mutant)
            ok, err = gate(ea, p, db)
        return kind, ea, judge(kind, ok, err), err

    stats = {}
    with cf.ThreadPoolExecutor(jobs) as ex:
        for kind, ea, verdict, err in ex.map(one, tasks):
            s = stats.setdefault(kind, {"CAUGHT": 0, "MISSED": 0, "INVALID": 0, "missed": []})
            s[verdict] += 1
            if verdict == "MISSED":
                s["missed"].append(ea)
            if verbose and verdict == "INVALID":
                print(f"  invalid {kind} 0x{ea:X}: {err.splitlines()[-1][:150]}")
    for kind, s in stats.items():
        tried = s["CAUGHT"] + s["MISSED"]
        rate = f"{100 * s['CAUGHT'] / tried:5.1f}%" if tried else "    -"
        print(f"mutants  {kind:10} caught {s['CAUGHT']:3}/{tried:<3} {rate}   (skipped: {s['INVALID']} other error)"
              + (f"   survivors: {' '.join(f'{e:x}' for e in s['missed'][:8])}" if s["missed"] else ""))
    return stats


# ---- live: the differential harness must catch what static checks can't
LIVE = [  # (ea, kind, how to build the mutant from the current file)
    (0x47F9B0, "boolret", lambda src: open(os.path.join(FIX_DIR, "0047f9b0_boolret.cpp")).read()),
]


def live_candidates(db):
    """Add a const mutant for a few leaf functions the last smoke actually diffed (they're reached in-mission)."""
    summ = os.path.join(os.environ.get("CSF_PLAY", "/mnt/e/dev/csf-play"), "csf_diff_summary.txt")
    out = list(LIVE)
    if not os.path.exists(summ):
        return out
    for line in open(summ, errors="replace"):
        f = line.split()
        if len(f) < 5 or int(f[1]) < 5:
            continue
        ea = int(f[0], 16)
        p = os.path.join(AUTO, f"{ea:08x}.cpp")
        if os.path.exists(p) and mut_const(open(p).read()):
            out.append((ea, "const", mut_const))
        if len(out) >= 4:
            break
    return out


def run_live():
    db = DB()
    cands = live_candidates(db)
    saved = {}
    try:
        for ea, kind, mk in cands:
            p = os.path.join(AUTO, f"{ea:08x}.cpp")
            saved[p] = open(p).read()
            open(p, "w").write(mk(saved[p]))
            print(f"live mutant {kind:8} 0x{ea:X}")
        r = subprocess.run([os.path.join(ROOT, "tools", "deploy.sh")], capture_output=True, text=True)
        if r.returncode:
            print("build/deploy failed:\n" + (r.stdout + r.stderr)[-2000:])
            return 1
        out = subprocess.run([os.path.join(ROOT, "tools", "smoke.sh"), "15"], capture_output=True, text=True,
                             env={**os.environ, "CSF_NO_LEDGER": "1"}).stdout  # mutants must not enter the ledger
    finally:
        for p, src in saved.items():
            open(p, "w").write(src)
        subprocess.run([os.path.join(ROOT, "tools", "deploy.sh")], capture_output=True, text=True)
    open(os.path.join(ROOT, "work", "controls_live.txt"), "w").write(out)
    hit = {int(m.group(1), 16) for m in re.finditer(r"^MISMATCH ([0-9A-F]{8})", out, re.M)}
    missed = 0
    for ea, kind, _ in cands:
        v = "CAUGHT" if ea in hit else "MISSED"
        missed += v == "MISSED"
        print(f"  {v:7} {kind:8} 0x{ea:X}")
    print(f"live     {len(cands) - missed}/{len(cands)} mutants flagged by the diff harness (original files restored)")
    return 1 if missed else 0


def main():
    ap = argparse.ArgumentParser()
    ap.add_argument("--n", type=int, default=25, help="mutants per operator")
    ap.add_argument("--jobs", type=int, default=12)
    ap.add_argument("--only", choices=["fixtures", "mutants", "positive"])
    ap.add_argument("--live", action="store_true")
    ap.add_argument("--all", action="store_true", help="positive controls over every src/auto file, not just fixtures")
    ap.add_argument("-v", "--verbose", action="store_true")
    a = ap.parse_args()
    if a.live:
        sys.exit(run_live())
    db = DB()
    bad = 0
    if a.only in (None, "fixtures"):
        bad += run_fixtures(db, a.jobs, a.verbose)
    if a.only in (None, "positive"):
        bad += run_positive(db, a.jobs, a.all)
    if a.only in (None, "mutants"):
        run_mutants(db, a.n, a.jobs, a.verbose)
    sys.exit(1 if bad else 0)


if __name__ == "__main__":
    main()
