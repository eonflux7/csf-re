#!/usr/bin/env python3
"""Bisect a crash of the installed baseline that only shows now and then.

tools/verifier.py gives up on a baseline failure that passes on re-run ("flaky crash, not bisectable"). This runs
each step often enough to tell: the candidates are split, one half is switched off in the hook ini (no rebuild),
and the smoke pool repeats the short start until the crash signature shows (the culprit is among the half still
on) or --runs smokes passed without it (it is among the half switched off).

  flakybisect.py --sig 'eip=006326e4' [--since 2026-10-13] [--runs 40]

Candidates: functions the ledger (db/verify.json) says were installed on or after --since. First the two controls:
all candidates on must show the crash within --runs, all off must not (otherwise the culprit is older code or not
a hook at all, and the search is pointless). Holds work/verify.lock like the verifier. Prints the culprit; changes
nothing in the repo (quarantine it by hand: its crash report names something else).
"""
import argparse, fcntl, json, os, re, sys, time

ROOT = os.path.dirname(os.path.dirname(os.path.abspath(__file__)))
sys.path.insert(0, os.path.dirname(os.path.abspath(__file__)))
import grunt, pool, verifier, verify  # noqa: E402


def shows(sig, off, runs, secs, env, label):
    """Run up to `runs` smokes with `off` disabled; True as soon as one output matches sig."""
    k, n, t0 = pool.size(), 0, time.time()
    while n < runs:
        jobs = [(secs, env, lambda play: grunt.set_disabled(off, play))] * min(k, runs - n)
        outs = pool.run(jobs)
        n += len(jobs)
        other = sum(1 for ok, out in outs if not ok and not re.search(sig, out))
        if any(re.search(sig, out) for _, out in outs):
            print(f"  {label}: crash after {n} smokes ({time.time() - t0:.0f} s)", flush=True)
            return True
        if other:
            print(f"  {label}: {other} smoke(s) failed another way (ignored)", flush=True)
    print(f"  {label}: no crash in {n} smokes ({time.time() - t0:.0f} s)", flush=True)
    return False


def main():
    ap = argparse.ArgumentParser(description=__doc__, formatter_class=argparse.RawDescriptionHelpFormatter)
    ap.add_argument("--sig", required=True, help="regex of the crash line, e.g. 'eip=006326e4'")
    ap.add_argument("--since", default="", help="candidates: installed on or after this date (ledger `since`)")
    ap.add_argument("--runs", type=int, default=40, help="smokes without the crash that clear a half")
    ap.add_argument("--among", default="", help="resume an interrupted run: comma-separated addresses still "
                    "suspect (the controls are skipped, everything else stays on)")
    a = ap.parse_args()
    lk = open(os.path.join(ROOT, "work", "verify.lock"), "w")
    fcntl.flock(lk, fcntl.LOCK_EX)
    head = verifier.prepare_tree()
    ok, out = verifier.build_tree()
    if not ok:
        print("verify tree build FAILED at", head[:8], "\n", out[-1500:])
        return 1
    verifier.deploy([])
    pool.sync()
    led = json.load(open(os.path.join(ROOT, "db", "verify.json")))
    pend = set(verify.pending())
    cands = sorted(int(ea, 16) for ea, r in led.items()
                   if r.get("ok") and (r.get("since") or "") >= a.since and ea not in pend
                   and os.path.exists(os.path.join(verifier.TREE, "src", "auto", f"{ea}.cpp")))
    secs, env = verifier.SHORT[0]
    print(f"{len(cands)} candidates at {head[:8]}; pool of {pool.size()}; signature /{a.sig}/", flush=True)
    try:
        cur = cands
        if a.among:
            cur = sorted(int(x, 16) for x in a.among.split(",") if x.strip())
            print(f"resuming among {len(cur)}: {cur[0]:08x}..{cur[-1]:08x}", flush=True)
        elif not shows(a.sig, [], a.runs, secs, env, "control, all on"):
            print("the crash did not show with everything on: nothing to bisect (raise --runs?)")
            return 1
        elif shows(a.sig, cands, a.runs, secs, env, "control, all candidates off"):
            print("the crash shows with every candidate off: older code, or not a hook")
            return 1
        while len(cur) > 1:
            half = cur[:len(cur) // 2]
            # half off, the rest of cur on (everything else stays on: cleared already)
            cur = cur[len(cur) // 2:] if shows(a.sig, half, a.runs, secs, env, f"{len(half)} of {len(cur)} off") else half
        c = cur[0]
        print(f"culprit candidate: {c:08x}; confirming", flush=True)
        on = shows(a.sig, [x for x in cands if x != c], a.runs, secs, env, "only it on")
        off = shows(a.sig, [c], a.runs, secs, env, "only it off")
        print(f"RESULT {c:08x}: alone on -> {'crash' if on else 'no crash'}; alone off -> "
              f"{'crash' if off else 'no crash'}" + ("" if on and not off else "  (not confirmed: more than one "
                                                     "culprit, or the rate is too low for --runs)"), flush=True)
        return 0
    finally:
        for play in pool.copies():
            grunt.set_disabled([], play)
        verify.write_pending_ini(verifier.INI)


if __name__ == "__main__":
    sys.exit(main())
