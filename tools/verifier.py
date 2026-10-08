#!/usr/bin/env python3
"""Verify loop: the game-side half of the RE loop, decoupled from production (grunt lanes no longer smoke).

Grunt lanes commit gated functions as *pending* (tools/verify.py: never installed in a passing smoke); every deploy
disables pending hooks, so the game runs the verified baseline. This loop takes the pending functions a module group
at a time and:

  1. builds the committed HEAD in its own detached tree (work/verify-tree; in-flight lanes files never get in)
  2. deploys it with only this group's pending hooks enabled on top of the baseline
  3. smokes: FR01 scene (short), or with --full also the menu tour and a savegame
  4. a crash -> grunt's bisect (crash report -> linker map -> culprit first, else binary search over ini toggles,
     no rebuild); culprits are quarantined (file -> queue/, status todo) and the group re-smoked without them
  5. a differential-test MISMATCH in the group -> quarantined the same way
  6. a passing smoke records the group as installed-OK in db/verify.json (smoke.sh -> verify.py ingest)
  7. commits the ledger + quarantines (tools/repo.py lock: lanes commits concurrently)

Usage:
  verifier.py                  all pending, groups of --group (default 150), FR01 smoke per group
  verifier.py --full           + menu tour + savegame per group (~3 min per group)
  verifier.py --module m53A0F0,gui_listbox    only these modules' pending functions
  verifier.py --groups 1       one group and stop (the lanes canary runs this in the background)
  verifier.py --dry            show the groups, run nothing
First the verified baseline alone is smoked (--no-baseline skips it); its culprits are quarantined the same way.
One instance at a time (work/verify.lock); the play copy is the verifier's alone while it runs.
"""
import argparse, collections, csv, fcntl, json, os, re, shutil, subprocess, sys, time

ROOT = os.path.dirname(os.path.dirname(os.path.abspath(__file__)))
TREE = os.path.join(ROOT, "work", "verify-tree")
os.environ["CSF_TREE"] = TREE  # verify.py (and smoke.sh's ingest) hash the tree's build, not ROOT's
sys.path.insert(0, os.path.dirname(__file__))
import grunt, verify, repo  # noqa: E402

PLAY = grunt.PLAY
INI = os.path.join(PLAY, "csf_hook.ini")
GROUP_FILE = os.path.join(ROOT, "work", "verify_group.txt")
LOG = os.path.join(ROOT, "work", "verifier_log.jsonl")
grunt.LIVE = os.path.join(ROOT, "work", "verifier_state.json")  # grunt.bisect() reports through live()
grunt.DLL_MAP = os.path.join(TREE, "build", "dinput8.map")      # crash reports resolve against the deployed build
SHORT = [(grunt.SMOKE_SECS, {})]
FULL = SHORT + [(grunt.SMOKE_LONG_SECS, {"CSF_START": "menu", "CSF_INI_EXTRA": "menutour 1500"}),
                (grunt.SMOKE_LONG_SECS, {"CSF_START": "loadsave 1.sav"})]


def sh(*cmd, cwd=None):
    r = subprocess.run(cmd, cwd=cwd, capture_output=True, text=True)
    return r.returncode, (r.stdout + r.stderr)


def prepare_tree():
    """Detached worktree at ROOT's HEAD, with the derived dirs linked in and its own build dir. Returns HEAD."""
    head = sh("git", "-C", ROOT, "rev-parse", "HEAD")[1].strip()
    if not os.path.exists(os.path.join(TREE, ".git")):
        sh("git", "-C", ROOT, "worktree", "prune")
        rc, out = sh("git", "-C", ROOT, "worktree", "add", "--detach", TREE, head)
        if rc:
            raise SystemExit("verify tree: " + out)
    else:
        rc, out = sh("git", "-C", TREE, "checkout", "-q", "--detach", head)
        if rc:
            raise SystemExit("verify tree checkout: " + out)
    for d in ("export", "ida", ".venv"):
        link = os.path.join(TREE, d)
        if not os.path.lexists(link):
            os.symlink(os.path.join(ROOT, d), link)
    if not os.path.exists(os.path.join(TREE, "build", "build.ninja")):
        rc, out = sh("cmake", "-B", "build", "-G", "Ninja", "-DCMAKE_TOOLCHAIN_FILE=cmake/clang-cl-x86.cmake", cwd=TREE)
        if rc:
            raise SystemExit("verify tree cmake: " + out[-2000:])
    return head


def build_tree():
    py = os.path.join(ROOT, ".venv", "bin", "python")
    sh(py, os.path.join(TREE, "tools", "difftable.py"), "--out", os.path.join(TREE, "build", "diff_table.inc"),
       cwd=TREE)
    rc, out = sh("ninja", "-C", os.path.join(TREE, "build"))
    return rc == 0, out


def deploy(group):
    """Copy the tree's DLL; pending hooks off except this group's."""
    shutil.copy(os.path.join(TREE, "build", "dinput8.dll"), os.path.join(PLAY, "dinput8.dll"))
    if not os.path.exists(INI):
        shutil.copy(os.path.join(TREE, "hook", "csf_hook.ini"), INI)
    open(GROUP_FILE, "w").write("\n".join(group) + "\n")
    return verify.write_pending_ini(INI, group)


def rows():
    return {r["ea"]: r for r in csv.DictReader(open(os.path.join(ROOT, "db", "functions.csv")))}


def groups(pending, size, modules=None):
    """Pending functions grouped by module (a group never mixes more modules than needed to fill it)."""
    rs = rows()
    if modules:  # names (hud) or ids (m42CF50), as everywhere else
        modules = grunt.module_ids(",".join(sorted(modules)))
    by = collections.defaultdict(list)
    for ea in pending:
        m = rs.get(ea, {}).get("module") or "?"
        if not modules or m in modules:
            by[m].append(ea)
    out, cur = [], []
    for m in sorted(by, key=lambda k: -len(by[k])):
        for ea in by[m]:
            cur.append(ea)
            if len(cur) >= size:
                out.append(cur)
                cur = []
    if cur:
        out.append(cur)
    return out


def quarantine(eas, why, out):
    """Committed functions that broke a smoke / mismatched: file -> queue/, status todo, ledger entry dropped."""
    for ea in eas:
        grunt.quarantine(int(ea, 16), why, out)
    with open(os.path.join(ROOT, "work", "db.lock"), "w") as lk:  # the captain/api.py write functions.csv too
        fcntl.flock(lk, fcntl.LOCK_EX)
        allrows = list(csv.DictReader(open(os.path.join(ROOT, "db", "functions.csv"))))
        for r in allrows:
            if r["ea"] in eas:
                r["status"] = "todo"
        w = csv.DictWriter(open(os.path.join(ROOT, "db", "functions.csv"), "w", newline=""),
                           fieldnames=list(allrows[0].keys()), lineterminator="\n")
        w.writeheader()
        w.writerows(allrows)
    led = verify.load()
    for ea in eas:
        led.pop(ea, None)
    verify.save(led)


def record(**kw):
    with open(LOG, "a") as f:
        f.write(json.dumps(dict(ts=time.time(), **kw)) + "\n")


def baseline(starts):
    """The verified baseline alone (every pending hook off) must pass; a failure there is in verified code (a smoke
    that never covered the path: e.g. menu-tour-only lanes smokes broke the FR01 scene start). Culprits among the
    verified functions are found the same way (crash report first) and quarantined. Returns (ok, quarantined)."""
    verified = [int(os.path.basename(p)[:8], 16) for p in sorted(os.listdir(os.path.join(TREE, "src", "auto")))
                if p.endswith(".cpp")]
    pend = {int(e, 16) for e in verify.pending()}
    verified = [e for e in verified if e not in pend]
    bad = []
    for secs, env in starts:
        for attempt in range(8):
            grunt._last_smoke = (secs, env)
            grunt.set_disabled([])
            ok, out = grunt.smoke(deploy=False)
            if ok:
                break
            culprits, why = grunt.bisect([e for e in verified if f"{e:08x}" not in bad], max_culprits=8,
                                         first_out=out)
            if not culprits:
                print(f"baseline smoke fails, not attributable ({why})", flush=True)
                return False, bad
            cs = [f"{c:08x}" for c in culprits]
            print(f"baseline: culprits {' '.join(cs)} ({why})", flush=True)
            quarantine(cs, f"breaks the verified baseline (verifier bisect: {why})", out)
            bad += cs
            for c in cs:  # off in this deploy too, until the tree is rebuilt without them
                open(INI, "a").write(f"disable 0x{c.upper()} {verify.PENDING_TAG}\n")
        else:
            return False, bad
    return True, bad


def verify_group(group, starts):
    """Returns (verified, quarantined, note)."""
    on, bad, note = list(group), [], ""
    for secs, env in starts:
        for attempt in range(6):  # each failing run removes >= 1 culprit
            grunt._last_smoke = (secs, env)
            grunt.set_disabled([])
            ok, out = grunt.smoke(deploy=False)
            if ok:
                mism = [ea for ea in on if re.search(rf"^MISMATCH {ea.upper()}\b", out, re.M)]
                if mism:  # the ledger already took this run; the quarantine drops their entries
                    quarantine(mism, "differential test mismatch in game (verifier)", out)
                    bad += mism
                    on = [e for e in on if e not in mism]
                break
            culprits, why = grunt.bisect([int(e, 16) for e in on], first_out=out)
            if not culprits:
                note = f"smoke failed, not attributable to the group ({why}); group left pending"
                return [], bad, note
            cs = [f"{c:08x}" for c in culprits]
            quarantine(cs, f"in-game smoke failure (verifier bisect: {why})", out)
            bad += cs
            on = [e for e in on if e not in cs]
            verify.write_pending_ini(INI, on)
            if not on:
                break
        else:
            return [], bad, "too many failing runs; group left pending"
    return on, bad, note


def main():
    ap = argparse.ArgumentParser()
    ap.add_argument("--group", type=int, default=150)
    ap.add_argument("--groups", type=int, default=0, help="stop after this many groups (0 = all)")
    ap.add_argument("--module", help="comma-separated module ids/names")
    ap.add_argument("--full", action="store_true", help="menu tour + savegame too, per group")
    ap.add_argument("--dry", action="store_true")
    ap.add_argument("--no-baseline", action="store_true", help="skip the baseline smoke (all pending off) first")
    a = ap.parse_args()
    lk = open(os.path.join(ROOT, "work", "verify.lock"), "w")
    try:
        fcntl.flock(lk, fcntl.LOCK_EX | fcntl.LOCK_NB)
    except OSError:
        print("verifier already running")
        return
    mods = set(a.module.split(",")) if a.module else None
    done_groups, checked_baseline = 0, a.no_baseline
    while True:
        head = prepare_tree()
        ok, out = build_tree()
        if not ok:  # committed HEAD doesn't build: lanes' batch build check should have caught it
            print("verify tree build FAILED at", head[:8], "\n", out[-2500:])
            record(event="build_failed", head=head)
            return 1
        pend = verify.pending()
        gs = groups(pend, a.group, mods)
        print(f"{len(pend)} pending; {len(gs)} group(s) at {head[:8]}", flush=True)
        if a.dry:
            for g in gs:
                print(f"  {len(g)}: {' '.join(g[:12])}{' ...' if len(g) > 12 else ''}")
            return 0
        if not gs or (a.groups and done_groups >= a.groups):
            verify.write_pending_ini(INI)  # leave the play copy on the verified baseline
            return 0
        if not checked_baseline:
            deploy([])
            ok, bad = baseline(FULL if a.full else SHORT)
            if bad:
                repo.commit(["db/verify.json", "db/functions.csv"] + [f"src/auto/{e}.cpp" for e in bad] +
                            [f"queue/{e}.md" for e in bad],
                            f"verifier: {len(bad)} functions quarantined (they broke the verified baseline)")
            record(event="baseline", ok=ok, quarantined=bad, head=head)
            if not ok:
                return 1
            checked_baseline = True
            if bad:
                continue  # rebuild the tree without them
        g = gs[0]
        deploy(g)
        t0 = time.time()
        good, bad, note = verify_group(g, FULL if a.full else SHORT)
        dt = time.time() - t0
        print(f"group of {len(g)}: {len(good)} verified, {len(bad)} quarantined{', ' + note if note else ''} "
              f"({dt:.0f} s)", flush=True)
        record(event="group", n=len(g), verified=len(good), quarantined=bad, note=note, secs=round(dt), head=head)
        files = ["db/verify.json", "db/functions.csv"] + [f"src/auto/{ea}.cpp" for ea in bad] + \
                [f"queue/{ea}.md" for ea in bad]
        if good or bad:
            repo.commit(files, f"verifier: {len(good)} functions verified in game, {len(bad)} quarantined", note)
        done_groups += 1
        if note:  # not attributable: don't spin on the same group
            return 1


if __name__ == "__main__":
    sys.exit(main() or 0)
