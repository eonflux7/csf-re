"""Serialized git commits in the main checkout: grunt lanes (produce) and tools/verifier.py (verify) both commit
there, concurrently. One flock (work/git.lock) around add+commit keeps them from tripping over index.lock."""
import contextlib, fcntl, os, subprocess

ROOT = os.path.dirname(os.path.dirname(os.path.abspath(__file__)))
TRAILER = "Co-Authored-By: Claude Opus 5.5 <noreply@anthropic.com>"


@contextlib.contextmanager
def lock(name="git.lock"):
    os.makedirs(os.path.join(ROOT, "work"), exist_ok=True)
    with open(os.path.join(ROOT, "work", name), "w") as lk:
        fcntl.flock(lk, fcntl.LOCK_EX)
        yield


def _clear_stale_index_lock():
    """A git process that died left .git/index.lock: every later commit fails until it is gone (2026-10-13: 20
    minutes of lanes output stayed uncommitted). Ours are serialized by the flock we hold, so a lock file older
    than 30 s with no git process alive belongs to nobody."""
    import time
    p = os.path.join(ROOT, ".git", "index.lock")
    try:
        old = time.time() - os.path.getmtime(p) > 30
    except OSError:
        return
    if old and not subprocess.run(["pgrep", "-x", "git"], capture_output=True, text=True).stdout.strip():
        try:
            os.remove(p)
            print("repo: stale .git/index.lock removed", flush=True)
        except OSError:
            pass


def commit(files, title, body=""):
    """git add -A + commit exactly `files` (paths relative to ROOT; deletions included)."""
    msg = f"{title}\n\n{body + chr(10) + chr(10) if body else ''}{TRAILER}"
    with lock():
        _clear_stale_index_lock()
        subprocess.run(["git", "-C", ROOT, "add", "-A", "--", *files], check=False)
        r = subprocess.run(["git", "-C", ROOT, "commit", "-q", "-m", msg, "--", *files], capture_output=True, text=True)
    return r.returncode == 0
