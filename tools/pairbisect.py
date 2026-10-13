#!/usr/bin/env python3
"""Find the smallest set of installed functions that fails a smoke together (delta debugging).

tools/verifier.py's search switches parts on alone and gives up when none fails ("no single part of N fails alone:
an interaction between parts"). This one also tries each part switched off: a failure that needs two functions in
different parts survives until both are found. For a failure that shows every time; a rare one: flakybisect.py.

  pairbisect.py --sig 'exit=-1073740791' --start 'loadsave 1.sav' [--secs 60] [--diff 0]

Candidates: every installed function of the verify tree. Each round runs up to pool-size smokes at once, hooks
toggled in the ini (no rebuild). Holds work/verify.lock like the verifier; changes nothing in the repo.
"""
import argparse, fcntl, os, re, sys, time

ROOT = os.path.dirname(os.path.dirname(os.path.abspath(__file__)))
sys.path.insert(0, os.path.dirname(os.path.abspath(__file__)))
import grunt, pool, verifier, verify  # noqa: E402


def main():
    ap = argparse.ArgumentParser(description=__doc__, formatter_class=argparse.RawDescriptionHelpFormatter)
    ap.add_argument("--sig", required=True, help="regex of the failing smoke's output")
    ap.add_argument("--start", default="", help="CSF_START of the smoke (default: the FR01 scene)")
    ap.add_argument("--secs", type=int, default=60)
    ap.add_argument("--diff", default="1", help="CSF_DIFF (0: hooks only, no differential testing)")
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
    pend = {int(e, 16) for e in verify.pending()}
    cands = sorted(int(p[:8], 16) for p in os.listdir(os.path.join(verifier.TREE, "src", "auto")) if p.endswith(".cpp"))
    cands = [e for e in cands if e not in pend]
    env = {"CSF_SMOKE_ADAPTIVE": "0", "CSF_DIFF": a.diff, **({"CSF_START": a.start} if a.start else {})}
    k, n, t0 = pool.size(), 0, time.time()

    def fails(ons):
        """[does the smoke with exactly `on` of the candidates switched on show the signature] per on-set."""
        nonlocal n
        res = []
        for i in range(0, len(ons), k):
            part = ons[i:i + k]
            outs = pool.run([(a.secs, env, (lambda play, on=on: grunt.set_disabled(set(cands) - set(on), play)))
                             for on in part])
            n += len(part)
            for on, (ok, out) in zip(part, outs):
                if not ok and not re.search(a.sig, out):
                    print(f"    ({len(on)} on: failed another way, counted as a pass)", flush=True)
                res.append(bool(re.search(a.sig, out)))
        return res

    print(f"{len(cands)} candidates at {head[:8]}; pool of {k}; signature /{a.sig}/", flush=True)
    try:
        on_all, off_all = fails([cands, []])
        if not on_all or off_all:
            print(f"controls: all on -> {'fails' if on_all else 'passes'}, all off -> {'fails' if off_all else 'passes'}: "
                  "nothing to search")
            return 1
        cur, g = cands, 2
        while len(cur) > 1:
            g = min(g, len(cur))
            chunks = [cur[i * len(cur) // g:(i + 1) * len(cur) // g] for i in range(g)]
            rest = [[e for e in cur if e not in set(c)] for c in chunks]
            res = fails(chunks + rest) if g > 2 else fails(chunks)  # with two chunks each is the other's complement
            alone, without = res[:g], (res[g:] if g > 2 else res[::-1])
            if any(alone):
                cur, g = chunks[alone.index(True)], 2
            elif any(without):
                cur, g = rest[without.index(True)], max(g - 1, 2)
            elif g >= len(cur):
                break  # every single one is needed
            else:
                g = min(2 * g, len(cur))
            print(f"  {len(cur)} left, {g} chunks next ({n} smokes, {time.time() - t0:.0f} s)", flush=True)
        print("RESULT fails with only these on: " + " ".join(f"{e:08x}" for e in cur), flush=True)
        return 0
    finally:
        for play in pool.copies():
            grunt.set_disabled([], play)
        verify.write_pending_ini(verifier.INI)


if __name__ == "__main__":
    sys.exit(main())
