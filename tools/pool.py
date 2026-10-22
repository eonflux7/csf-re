#!/usr/bin/env python3
"""Smoke pool: several play copies, so independent smokes run side by side (bisect parts, the verifier's starts).

The first copy is the play copy (CSF_PLAY, tools/paths.py): deploys and ini edits go there as before. The
others mirror it (`sync`: hook DLL + ini without bisect lines) and only ever run smokes. Each copy keeps its own
csf_hook.log / hits / diff files; savegames are shared (Documents\\CSF Data\\Games). Several instances can run at
once because the hook DLL renames the game's single-instance mutex (src/core/devcmd.cpp, MultiInstance).

  CSF_POOL="dir dir ..."   the copies (default: the play copy + its siblings csf-play-p1..p3 that exist);
                           CSF_POOL_SIZE=N uses only the first N (1 = no parallel smokes)
  pool.py make N           create copies p1..pN-1 next to the play copy (tools/playcopy.py: data hard-linked)
  pool.py list             the copies in use
"""
import concurrent.futures as cf, os, re, shutil, signal, subprocess, sys, threading

ROOT = os.path.dirname(os.path.dirname(os.path.abspath(__file__)))
sys.path.insert(0, os.path.dirname(__file__))
from paths import PLAY
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


def kill(plays=None):
    """Stop game instances running from these copies (default: the whole pool). Killing smoke.sh / its powershell
    leaves the game itself running (2026-10-09: an interrupted bisect left 3 instances and its ini lines)."""
    wins = [subprocess.run(["wslpath", "-w", d], capture_output=True, text=True).stdout.strip() + "\\CommXPC.exe"
            for d in (plays or copies())]
    pat = "|".join(re.escape(w) for w in wins)
    subprocess.run(["powershell.exe", "-NoProfile", "-Command",
                    "Get-Process CommXPC -ErrorAction SilentlyContinue | Where-Object { $_.Path -match '^(" + pat +
                    ")$' } | Stop-Process -Force"], capture_output=True)


def _sigterm(*_):
    raise SystemExit(143)  # a plain `kill` still runs the finally blocks (bisect ini cleanup, kill())


def run(jobs):
    """jobs: [(secs, env, prepare)] with prepare(play) called first (e.g. writing that copy's bisect lines), run
    on distinct copies at once, at most size() at a time. Returns [(ok, out)] in job order. When interrupted
    (Ctrl-C, SIGTERM, an exception) the game instances of the pool are stopped."""
    if threading.current_thread() is threading.main_thread():
        signal.signal(signal.SIGTERM, _sigterm)
    try:
        return _run(jobs)
    except BaseException:
        kill()
        raise


def _run(jobs):
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
