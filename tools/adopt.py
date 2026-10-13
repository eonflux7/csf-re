#!/usr/bin/env python3
"""Take the functions a deep-worker job wrote into the main tree, after checking each one here again.

  adopt.py JOB [--dry] [--allow-inconclusive]

For every new or changed src/auto/EA.cpp in work/deep/JOB/tree: copy it in, run gate.py and emudiff.py --src on it
from the main tree (the job's own claims are not used). A file that passes both (gate ok; emudiff `same`) stays
and its function is marked done in db/functions.csv (pending in-game verification, like a lane's output); any
other is taken out again and listed. Other files of the job's patch (tools, headers, docs) are the reviewer's to
apply by hand. Does not commit.
"""
import argparse, concurrent.futures, os, re, shutil, subprocess, sys

ROOT = os.path.dirname(os.path.dirname(os.path.abspath(__file__)))
sys.path.insert(0, os.path.join(ROOT, "tools"))
PY = os.path.join(ROOT, ".venv", "bin", "python")


def check(ea, path, allow_inconclusive):
    g = subprocess.run([PY, "tools/gate.py", ea, path], capture_output=True, text=True, cwd=ROOT)
    gl = (g.stdout + g.stderr).strip().splitlines()[-1:] or ["(no output)"]
    if g.returncode or not re.search(r"\bok\b", gl[0]):
        return ea, False, "gate: " + gl[0][:160]
    e = subprocess.run([PY, "tools/emudiff.py", ea, "--src", path], capture_output=True, text=True, cwd=ROOT)
    el = (e.stdout + e.stderr).strip().splitlines()[-1:] or ["(no output)"]
    good = re.search(r"\bsame\b", el[0]) or (allow_inconclusive and "inconclusive" in el[0])
    return ea, bool(good), "emudiff: " + el[0][:160]


def main():
    ap = argparse.ArgumentParser(description=__doc__, formatter_class=argparse.RawDescriptionHelpFormatter)
    ap.add_argument("job")
    ap.add_argument("--dry", action="store_true", help="check only: leave the main tree as it was")
    ap.add_argument("--allow-inconclusive", action="store_true")
    a = ap.parse_args()
    tree = os.path.join(ROOT, "work", "deep", a.job, "tree")
    out = subprocess.run(["git", "-C", tree, "status", "--porcelain", "--", "src/auto"], capture_output=True, text=True).stdout
    files = sorted(l[3:].strip() for l in out.splitlines() if re.fullmatch(r"src/auto/[0-9a-f]{8}\.cpp", l[3:].strip()))
    if not files:
        raise SystemExit(f"job {a.job}: no src/auto/EA.cpp files changed in its tree")
    saved = {}
    for f in files:  # what the main tree had, to put back on failure or --dry
        p = os.path.join(ROOT, f)
        saved[f] = open(p, "rb").read() if os.path.exists(p) else None
        shutil.copyfile(os.path.join(tree, f), p)
    with concurrent.futures.ThreadPoolExecutor(8) as ex:
        res = list(ex.map(lambda f: check(os.path.basename(f)[:8], f, a.allow_inconclusive), files))
    bad = [(ea, why) for ea, ok, why in res if not ok]
    for f in files:
        ea = os.path.basename(f)[:8]
        if a.dry or any(b[0] == ea for b in bad):
            p = os.path.join(ROOT, f)
            if saved[f] is None:
                os.remove(p)
            else:
                open(p, "wb").write(saved[f])
    for ea, why in bad:
        print(f"  not taken {ea}: {why}")
    good = [ea for ea, ok, _ in res if ok]
    if not a.dry and good:
        import grunt
        rows = grunt.load_csv()
        for ea in good:
            row = rows[int(ea, 16)]
            row["status"], row["module"] = "done", row["module"] or "auto"
        grunt.save_csv(rows)
        subprocess.run([PY, os.path.join(ROOT, "tools", "globals.py")], check=False)
    print(f"adopt {a.job}: {len(good)} of {len(files)} pass gate and emudiff here"
          + (" (dry run, nothing kept)" if a.dry else "; marked done, not committed"))
    return 1 if bad else 0


if __name__ == "__main__":
    sys.exit(main())
