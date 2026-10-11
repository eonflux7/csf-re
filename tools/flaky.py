#!/usr/bin/env python3
"""flaky.py N [file with addresses to disable]: the savegame start N times on the smoke pool at once (load makes
rare crashes frequent: 13/16 where single smokes failed 1 in 4); prints the failure count and crash signatures,
logs in work/flaky_*.log. Compare with every src/auto function disabled to tell ours from the game's."""
import sys, os, re, json, time
ROOT = os.path.dirname(os.path.dirname(os.path.abspath(__file__)))
sys.path.insert(0, os.path.join(ROOT, "tools")); os.chdir(ROOT)
import pool, grunt
START = (60, {"CSF_START": "loadsave 1.sav"})
def trial(disabled, n, tag=""):
    pool.sync()
    res = pool.run([(START[0], START[1], (lambda play, d=disabled: grunt.set_disabled(d, play))) for _ in range(n)])
    fails = [out for ok, out in res if not ok]
    for k, out in enumerate(fails):
        open(f"work/flaky_{tag}_{int(time.time())}_{k}.log", "w").write(out)
    sig = []
    for out in fails:
        m = re.search(r"CRASH code=(\w+) eip=(\w+) addr=(\w+)", out) or re.search(r"exit=(-?\d+)", out)
        sig.append(m.group(0) if m else "?")
    return len(fails), sig
if __name__ == "__main__":
    n = int(sys.argv[1]); dis = [int(x, 16) for x in open(sys.argv[2]).read().split()] if len(sys.argv) > 2 else []
    f, sig = trial(dis, n, "base")
    print(f"{f}/{n} failed with {len(dis)} disabled"); [print("  ", s) for s in sig]
    for d in pool.copies(): grunt.set_disabled([], d)
