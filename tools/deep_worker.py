#!/usr/bin/env python3
"""Deep worker: one agentic GPT session (Codex CLI, gpt-6.1-sol@high) on a task the orchestrator hands out.

The orchestrator starts a task, fires up workers for the parts that need long reading or trial and error (a queue
item's root cause, a class of emudiff differences, a container replica), and reviews what comes back. A worker
never touches the repo: it gets its own detached worktree (work/deep/NAME/tree, derived dirs linked in, its own
build dir) and may edit and run the offline tools there (gate, semcheck, emudiff, single-file compiles). No game,
no smokes, no verifier, no commits, no IDB writes, no network.

  deep_worker.py run NAME --task FILE|-  [--model gpt-6.1-sol] [--effort high] [--timeout 3600] [--fresh]
  deep_worker.py list                     workers and their state
  deep_worker.py drop NAME                remove a worker's tree and files

Comes back in work/deep/NAME/: report.md (the worker's last message: findings, what it checked and how, what it
did not check), patch.diff (everything it changed in its tree, new files included; apply with `git apply` after
review), events.jsonl (the session), state.json (for tools/monitor.py). Several workers run side by side; the
Codex login is picked like the lanes pick theirs.
"""
import argparse, json, os, shutil, subprocess, sys, time

ROOT = os.path.dirname(os.path.dirname(os.path.abspath(__file__)))
sys.path.insert(0, os.path.dirname(os.path.abspath(__file__)))
DEEP = os.path.join(ROOT, "work", "deep")

BRIEF = """You are a deep worker on CSF-RE: a C++ reimplementation of CommXPC.exe (Commandos: Strike Force, PC,
MSVC 7.1), installed function by function as hooks over the original. Read CLAUDE.md and the parts of docs/PLAN.md
and docs/JOURNAL.md your task touches before you start. The orchestrator reviews what you return and decides.

Your working directory is your own worktree of the repo. You may edit files in it and run the offline tools with
`.venv/bin/python tools/<tool>.py` (gate.py, semcheck.py, emudiff.py: `emudiff.py EA --src src/auto/EA.cpp` after
an edit, the object in build/ is stale otherwise). Typed pseudocode and other IDA exports are under export/
(read-only), the original image is work/pc/CommXPC.exe. Not available to you, do not try: the game, smokes,
tools/verifier.py, tools/grunt.py, anything in tools/ida/ that writes, git commit, the network. Leave docs/PLAN.md,
docs/JOURNAL.md, queue/ and db/ as they are: what belongs there goes in your report.

Rules of the repo that bind you: an original address is never a bare number in code (CSF_FUNC / CSF_GLOBAL /
CSF_STR / CSF_VTBL / CSF_ADDR_*, see src/globals.h); library regions (rw, d3dx9, netz, crt, png) are not
reimplemented; no RenderWare SDK source; export/ is never edited by hand; large dumps go to files, not your answer.

Your last message is the report the orchestrator reads. Lead with the finding. Then, apart from each other: what
you verified and with which command, what you concluded without verifying, what you changed (files), what is
still open. Do not claim a fix works unless a tool run showed it.
"""


def sh(*cmd, cwd=None):
    r = subprocess.run(cmd, capture_output=True, text=True, cwd=cwd)
    return r.returncode, r.stdout + r.stderr


def prepare(name, fresh):
    """Detached worktree of HEAD for this worker (like the verify tree: derived dirs linked, own work/ and build/)."""
    d = os.path.join(DEEP, name)
    tree = os.path.join(d, "tree")
    head = sh("git", "-C", ROOT, "rev-parse", "HEAD")[1].strip()
    if fresh and os.path.exists(tree):
        sh("git", "-C", ROOT, "worktree", "remove", "--force", tree)
    os.makedirs(d, exist_ok=True)
    if not os.path.exists(os.path.join(tree, ".git")):
        sh("git", "-C", ROOT, "worktree", "prune")
        rc, out = sh("git", "-C", ROOT, "worktree", "add", "--detach", tree, head)
        if rc:
            raise SystemExit("deep worker tree: " + out)
    for link in ("export", "ida", ".venv"):
        if not os.path.lexists(os.path.join(tree, link)):
            os.symlink(os.path.join(ROOT, link), os.path.join(tree, link))
    os.makedirs(os.path.join(tree, "work"), exist_ok=True)
    for link in ("pc", "tools"):  # the exe image; third-party tools
        src = os.path.join(ROOT, "work", link)
        if os.path.exists(src) and not os.path.lexists(os.path.join(tree, "work", link)):
            os.symlink(src, os.path.join(tree, "work", link))
    if not os.path.exists(os.path.join(tree, "build", "build.ninja")):
        rc, out = sh("cmake", "-B", "build", "-G", "Ninja", "-DCMAKE_TOOLCHAIN_FILE=cmake/clang-cl-x86.cmake", cwd=tree)
        if rc:
            raise SystemExit("deep worker cmake: " + out[-2000:])
    return d, tree, sh("git", "-C", tree, "rev-parse", "HEAD")[1].strip()


def login():
    """The Codex login with the most room in its 5h window (the lanes' round robin does not look)."""
    import grunt
    best = None
    for h in grunt.codex_homes():
        try:
            used = grunt.codex_usage(h)["5h"][0]
        except Exception:
            used = 50
        if best is None or used < best[0]:
            best = (used, h)
    if best[0] >= 99:
        raise SystemExit(f"deep worker: every Codex login is at its 5h limit ({best[0]}%)")
    return best[1]


def state(d, **kw):
    p = os.path.join(d, "state.json")
    st = json.load(open(p)) if os.path.exists(p) else {}
    st.update(kw, updated=time.time())
    tmp = p + ".tmp"
    json.dump(st, open(tmp, "w"), indent=1)
    os.replace(tmp, p)
    return st


def run(a):
    task = sys.stdin.read() if a.task == "-" else open(a.task).read()
    d, tree, head = prepare(a.name, a.fresh)
    open(os.path.join(d, "task.md"), "w").write(task)
    report, events = os.path.join(d, "report.md"), os.path.join(d, "events.jsonl")
    if os.path.exists(report):
        os.remove(report)
    state(d, name=a.name, pid=os.getpid(), started=time.time(), finished=None, phase="running", head=head,
          model=a.model, effort=a.effort, task=task.strip().splitlines()[0][:120], error=None)
    cmd = ["codex", "exec", "--ephemeral", "--ignore-user-config", "-s", "workspace-write", "-C", tree,
           "-m", a.model, "-c", f'model_reasoning_effort="{a.effort}"', "-c", "web_search=disabled",
           "-o", report, "--json", "-"]
    t0, err = time.time(), None
    try:
        with open(events, "w") as ev:
            r = subprocess.run(cmd, input=BRIEF + "\n=== TASK ===\n" + task, stdout=ev, stderr=subprocess.PIPE,
                               text=True, timeout=a.timeout, env=dict(os.environ, CODEX_HOME=login()))
        if r.returncode or not os.path.exists(report):
            tail = open(events, errors="replace").read()[-600:]
            err = f"codex exec rc={r.returncode}: {r.stderr[-400:]}"
            if "usage limit" in tail:  # run again without --fresh after the reset: the tree keeps its changes
                err = "quota: " + tail[tail.find("usage limit"):][:120].split('"')[0]
    except subprocess.TimeoutExpired:
        err = f"timeout after {a.timeout} s (its tree keeps what it changed)"
    sh("git", "-C", tree, "add", "-A", "-N")  # new files show in the diff
    patch = subprocess.run(["git", "-C", tree, "diff", "HEAD", "--", ".", ":!build"], capture_output=True, text=True).stdout
    open(os.path.join(d, "patch.diff"), "w").write(patch)
    usage = {}
    for line in open(events, errors="replace"):
        if '"turn.completed"' in line:
            usage = json.loads(line).get("usage", {})
    state(d, finished=time.time(), phase="failed" if err else "done", error=err, usage=usage,
          patch_lines=patch.count("\n"))
    print(f"deep worker {a.name}: {'FAILED: ' + err if err else 'done'} in {time.time() - t0:.0f} s; "
          f"report {os.path.relpath(report, ROOT)}, patch {patch.count(chr(10))} lines", flush=True)
    return 1 if err else 0


def ls(_):
    for name in sorted(os.listdir(DEEP)) if os.path.isdir(DEEP) else []:
        p = os.path.join(DEEP, name, "state.json")
        if not os.path.exists(p):
            continue
        st = json.load(open(p))
        phase = st["phase"]
        if phase == "running" and not os.path.exists(f"/proc/{st['pid']}"):
            phase = "dead"
        secs = (st.get("finished") or time.time()) - st["started"]
        print(f"{name:24} {phase:8} {secs / 60:5.0f} min  patch {st.get('patch_lines', '-'):>5}  {st.get('task', '')}")
    return 0


def drop(a):
    d = os.path.join(DEEP, a.name)
    sh("git", "-C", ROOT, "worktree", "remove", "--force", os.path.join(d, "tree"))
    shutil.rmtree(d, ignore_errors=True)
    sh("git", "-C", ROOT, "worktree", "prune")
    return 0


def main():
    ap = argparse.ArgumentParser(description=__doc__, formatter_class=argparse.RawDescriptionHelpFormatter)
    sub = ap.add_subparsers(dest="cmd", required=True)
    r = sub.add_parser("run")
    r.add_argument("name")
    r.add_argument("--task", required=True, help="file with the task, or - for stdin")
    r.add_argument("--model", default="gpt-6.1-sol")
    r.add_argument("--effort", default="high")
    r.add_argument("--timeout", type=int, default=3600)
    r.add_argument("--fresh", action="store_true", help="throw the worker's old tree away first")
    r.set_defaults(fn=run)
    sub.add_parser("list").set_defaults(fn=ls)
    dp = sub.add_parser("drop")
    dp.add_argument("name")
    dp.set_defaults(fn=drop)
    a = ap.parse_args()
    return a.fn(a)


if __name__ == "__main__":
    sys.exit(main())
