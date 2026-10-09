#!/usr/bin/env python3
"""Smoke pool: several play copies, so independent smokes run side by side (bisect parts, the verifier's starts).

The first copy is the play copy (CSF_PLAY, /mnt/e/dev/csf-play): deploys and ini edits go there as before. The
others mirror it (`sync`: hook DLL + ini without bisect lines) and only ever run smokes. Each copy keeps its own
csf_hook.log / hits / diff files; savegames are shared (Documents\\CSF Data\\Games). Several instances can run at
once because the hook DLL renames the game's single-instance mutex (src/core/devcmd.cpp, MultiInstance).

  CSF_POOL="dir dir ..."   the copies (default: the play copy + /mnt/e/dev/csf-play-p1..p3 that exist);
                           CSF_POOL_SIZE=N uses only the first N (1 = no parallel smokes)
  pool.py make N           create copies p1..pN-1 next to the play copy (tools/playcopy.py: data hard-linked)
  pool.py list             the copies in use
"""
import concurrent.futures as cf, os, shutil, subprocess, sys

ROOT = os.path.dirname(os.path.dirname(os.path.abspath(__file__)))
sys.path.insert(0, os.path.dirname(__file__))
PLAY = os.environ.get("CSF_PLAY", "/mnt/e/dev/csf-play")
BISECT_TAG = "# grunt-bisect"


def copies():
    if os.environ.get("CSF_POOL"):
        out = os.environ["CSF_POOL"].split()
    else:
        out = [PLAY] + [d for d in (f"{PLAY}-p{i}" for i in range(1, 8)) if os.path.isdir(d)]
    n = int(os.environ.get("CSF_POOL_SIZE", "0") or 0)
    return out[:n] if n > 0 else out


def size():
    return len(copies())


def sync():
    """Mirror the play copy's hook DLL and ini (bisect lines dropped) into the other copies."""
    import playexe
    dll = os.path.join(PLAY, playexe.ensure(PLAY))
    ini = [l for l in open(os.path.join(PLAY, "csf_hook.ini")).read().splitlines() if not l.endswith(BISECT_TAG)]
    for d in copies()[1:]:
        shutil.copy(dll, os.path.join(d, playexe.ensure(d)))
        open(os.path.join(d, "csf_hook.ini"), "w").write("\n".join(ini) + "\n")


def smoke(play, secs, env=None):
    """One smoke on one copy: (ok, stdout)."""
    r = subprocess.run([os.path.join(ROOT, "tools", "smoke.sh"), str(secs)], capture_output=True, text=True,
                       env={**os.environ, **(env or {}), "CSF_PLAY": play})
    return r.returncode == 0, r.stdout


def run(jobs):
    """jobs: [(secs, env, prepare)] with prepare(play) called first (e.g. writing that copy's bisect lines), run
    on distinct copies at once, at most size() at a time. Returns [(ok, out)] in job order."""
    free = list(copies())
    res = [None] * len(jobs)
    with cf.ThreadPoolExecutor(len(free)) as ex:
        todo = list(enumerate(jobs))
        running = {}
        while todo or running:
            while todo and free:
                i, (secs, env, prep) = todo.pop(0)
                play = free.pop(0)
                if prep:
                    prep(play)
                running[ex.submit(smoke, play, secs, env)] = (i, play)
            done, _ = cf.wait(running, return_when=cf.FIRST_COMPLETED)
            for f in done:
                i, play = running.pop(f)
                res[i] = f.result()
                free.append(play)
    return res


def make(n):
    for i in range(1, n):
        d = f"{PLAY}-p{i}"
        subprocess.run([sys.executable, os.path.join(ROOT, "tools", "playcopy.py"), d, "--src", PLAY], check=True)


if __name__ == "__main__":
    if len(sys.argv) > 2 and sys.argv[1] == "make":
        make(int(sys.argv[2]))
    else:
        print("\n".join(copies()))
