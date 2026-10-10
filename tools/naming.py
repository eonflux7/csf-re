#!/usr/bin/env python3
"""The naming pass: everything that changes names in db/ and src/, in one command, run at milestones (between wave
runs, next to tools/ida/propagate.sh). Waves never rename: names are readability, the structure the grunts need
(classes, vtables, sizes, parameter types) comes from the binary through propagate.sh.

  1. captain: name the unnamed classes (family), modules and placeholder functions of the modules that have done
     functions; tools/fields.py apply pools the grunts' field names (consensus). --no-captain: no model calls,
     only what db/ already holds.
  2. tools/rename_classes.py + tools/fields.py fixbuild rewrite src/ and regenerate src/types/auto_classes.h.
  3. Invariant: a rename changes no code. Every object's code hash (verify.code_hash: disassembly without symbol
     names) is taken before and after; if the build breaks or any hash differs, src/ and the names are put back as
     they were and the pass exits 1 with the list (work/naming_changed.txt). --allow EA,EA accepts functions whose
     change was looked at (their in-game evidence goes stale by itself: the ledger is keyed by the same hash).

Usage: naming.py [module ...] [--no-captain] [--allow EA,EA] [--propagate]
"""
import argparse, concurrent.futures as cf, glob, os, subprocess, sys

ROOT = os.path.dirname(os.path.dirname(os.path.abspath(__file__)))
sys.path.insert(0, os.path.dirname(os.path.abspath(__file__)))
import repo  # noqa: E402
from verify import code_hash  # noqa: E402

BUILD = os.path.join(ROOT, "build")
OBJ = os.path.join(BUILD, "CMakeFiles", "csf_hook.dir", "src")
NAMED = ["src", "db/types", "db/functions.csv"]  # what a rename pass writes
CHANGED = os.path.join(ROOT, "work", "naming_changed.txt")


def git(*a):
    return subprocess.run(["git", "-C", ROOT, *a], capture_output=True, text=True)


def tool(*a):
    r = subprocess.run([sys.executable, os.path.join(ROOT, "tools", a[0]), *a[1:]], capture_output=True, text=True)
    out = (r.stdout + r.stderr).strip().splitlines()
    print("\n".join(f"  [{a[0][:-3]}] {l}" for l in out[-6:]), flush=True)
    return r.returncode == 0


def build():
    r = subprocess.run(["ninja", "-C", BUILD], capture_output=True, text=True)
    return r.returncode == 0, (r.stdout + r.stderr)[-3000:]


def hashes():
    """{object path relative to src/: code hash} for every built object (src/auto, src/core, ...)."""
    objs = sorted(glob.glob(os.path.join(OBJ, "**", "*.obj"), recursive=True))
    with cf.ThreadPoolExecutor(16) as ex:
        return dict(zip((os.path.relpath(o, OBJ) for o in objs), ex.map(code_hash, objs)))


def done_modules():
    from triage import load_csv
    return sorted({r["module"] for r in load_csv().values() if r["status"] == "done" and r["region"] == "game"})


def captain_steps(mods):
    import modules
    unnamed = sorted(m["id"] for m in modules.read_yaml() if m["id"] in mods and not m.get("name"))
    print(f"naming: captain family over {len(mods)} modules", flush=True)
    tool("captain.py", "family", *mods)
    if unnamed:
        print(f"naming: captain modules ({len(unnamed)} unnamed)", flush=True)
        tool("captain.py", "modules", "--only", ",".join(unnamed))
    print("naming: captain functions (placeholder names)", flush=True)
    tool("captain.py", "functions", "--module", ",".join(mods))


def put_back(start, why):
    git("checkout", start, "--", *NAMED)
    if git("status", "--porcelain", "--", *NAMED).stdout.strip():  # the captain committed names since `start`
        repo.commit(NAMED, "Naming pass put back", why)
    build()
    sys.exit(f"naming: {why}; src/ and the names are as before")


def main():
    ap = argparse.ArgumentParser(description=__doc__, formatter_class=argparse.RawDescriptionHelpFormatter)
    ap.add_argument("modules", nargs="*", help="module ids/names for the captain steps (default: all with done functions)")
    ap.add_argument("--no-captain", action="store_true", help="no model calls: apply the names db/ already holds")
    ap.add_argument("--allow", default="", help="comma list of functions (EA) whose code may change")
    ap.add_argument("--propagate", action="store_true", help="then tools/ida/propagate.sh (IDB, ~45 min)")
    a = ap.parse_args()
    if git("status", "--porcelain", "--", *NAMED, "db/modules.yaml").stdout.strip():
        sys.exit("naming: src/ or db/ has uncommitted changes; commit or stash them first")
    start = git("rev-parse", "HEAD").stdout.strip()
    ok, err = build()
    if not ok:
        sys.exit("naming: the build is broken before the pass:\n" + err)
    before = hashes()
    print(f"naming: {len(before)} objects hashed", flush=True)

    if not a.no_captain:
        if a.modules:
            from grunt import module_ids
            mods = sorted(module_ids(",".join(a.modules)))
        else:
            mods = done_modules()
        captain_steps(mods)
    tool("fields.py", "apply")
    tool("rename_classes.py")
    tool("fields.py", "fixbuild")

    ok, err = build()
    if not ok:
        put_back(start, "the build breaks after the rename:\n" + err)
    after = hashes()
    allow = {f"{int(x, 16):08x}" for x in a.allow.split(",") if x}
    changed = sorted(o for o in before if after.get(o) != before[o])
    refused = [o for o in changed if os.path.basename(o)[:8] not in allow]
    if refused:
        with open(CHANGED, "w") as f:
            f.write("\n".join(refused) + "\n")
        put_back(start, f"{len(refused)} object(s) changed code under a rename ({os.path.relpath(CHANGED, ROOT)}): "
                        + ", ".join(refused[:8]) + (" ..." if len(refused) > 8 else ""))
    n = len(git("status", "--porcelain", "--", "src").stdout.splitlines())
    if repo.commit(NAMED, f"Naming pass: {n} source files renamed, code unchanged",
                   f"{len(before)} objects hash-identical" + (f"; allowed to change: {', '.join(changed)}" if changed else "")):
        print(f"naming: committed ({n} source files, {len(before) - len(changed)} objects hash-identical)", flush=True)
    else:
        print("naming: nothing to rename", flush=True)
    if a.propagate:
        subprocess.run([os.path.join(ROOT, "tools", "ida", "propagate.sh")])
    else:
        print("naming: next tools/ida/propagate.sh so the IDB and the packets show the names", flush=True)


if __name__ == "__main__":
    main()
