#!/usr/bin/env python3
"""Deep worker: one agent session on a job the orchestrator hands out and oversees (2026-10-13, user: the
orchestrator no longer does the jobs itself; it spawns agents, reviews what they return, decides).

Agents (--agent): sol-hi = gpt-6.1-sol@high through the Codex CLI, sandboxed to the job's tree;
opus-me = claude-opus-5-5@medium through `claude -p` on the main account (the grunts' account is Haiku only),
edits and a fixed list of commands allowed, everything else denied.

The orchestrator starts a task, fires up workers for the parts that need long reading or trial and error (a queue
item's root cause, a class of emudiff differences, a container replica), and reviews what comes back. A worker
never touches the repo: it gets its own detached worktree (work/deep/NAME/tree, derived dirs linked in, its own
build dir) and may edit and run the offline tools there (gate, semcheck, emudiff, single-file compiles). No game,
no smokes, no verifier, no commits, no IDB writes, no network.

  deep_worker.py start NAME --task FILE|- [--agent sol-hi|opus-me] [--timeout 3600] [--fresh]   in the background
  deep_worker.py run   NAME ...           the same, in the foreground
  deep_worker.py list                     jobs and their state
  deep_worker.py review NAME applied|partial|rejected [--note TEXT]    the orchestrator's verdict on a finished job
  deep_worker.py drop NAME                remove a job's tree and files

Comes back in work/deep/NAME/: report.md (the worker's last message: findings, what it checked and how, what it
did not check), patch.diff (everything it changed in its tree, new files included; apply with `git apply` after
review), events.jsonl (the session), state.json (shown by tools/webmon.py), run.log. Several jobs run side by
side; a Codex job takes the login with the most room in its 5h window. A job stopped by quota or timeout keeps
its tree: start it again without --fresh and it goes on from there.
"""
import argparse, json, os, re, shutil, subprocess, sys, time

ROOT = os.path.dirname(os.path.dirname(os.path.abspath(__file__)))
sys.path.insert(0, os.path.dirname(os.path.abspath(__file__)))
DEEP = os.path.join(ROOT, "work", "deep")
AGENTS = {"sol-hi": ("codex", "gpt-6.1-sol", "high"), "opus-me": ("claude", "claude-opus-5-5", "medium")}
# opus-me: what `claude -p` may do without asking (dontAsk denies the rest)
CLAUDE_ALLOW = ["Read", "Grep", "Glob", "Edit", "Write", "Bash(.venv/bin/python:*)", "Bash(git diff:*)",
                "Bash(git status:*)", "Bash(git log:*)", "Bash(git show:*)", "Bash(ls:*)", "Bash(cat:*)", "Bash(rg:*)",
                "Bash(grep:*)", "Bash(head:*)", "Bash(tail:*)", "Bash(wc:*)", "Bash(find:*)", "Bash(sed -n:*)",
                "Bash(mkdir:*)", "Bash(cp:*)", "Bash(ninja:*)", "Bash(llvm-objdump:*)"]

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
    backend, model, effort = AGENTS[a.agent]
    st = state(d)
    state(d, name=a.name, pid=os.getpid(), started=time.time(), finished=None, phase="running", head=head,
          agent=a.agent, model=model, effort=effort, task=task.strip().splitlines()[0][:160], error=None,
          review=None, note=None, runs=st.get("runs", 0) + 1,
          eas=sorted(set(re.findall(r"\b00[4-7][0-9a-f]{5}\b", task))))
    prompt = BRIEF + "\n=== TASK ===\n" + task
    if os.path.exists(os.path.join(d, "patch.diff")) and os.path.getsize(os.path.join(d, "patch.diff")):
        prompt += ("\n\n(This job was started before and stopped by quota or timeout. Your tree still holds what "
                   "was changed then: look at `git status` and `git diff HEAD` first and go on from there.)\n")
    t0, err, usage = time.time(), None, {}
    try:
        if backend == "codex":
            cmd = ["codex", "exec", "--ephemeral", "--ignore-user-config", "-s", "workspace-write", "-C", tree,
                   "-m", model, "-c", f'model_reasoning_effort="{effort}"', "-c", "web_search=disabled",
                   "-o", report, "--json", "-"]
            home = login()
            state(d, login=os.path.basename(home).lstrip("."))
            with open(events, "w") as ev:
                r = subprocess.run(cmd, input=prompt, stdout=ev, stderr=subprocess.PIPE, text=True,
                                   timeout=a.timeout, env=dict(os.environ, CODEX_HOME=home))
            tail = open(events, errors="replace").read()[-600:]
            if r.returncode or not os.path.exists(report):
                err = f"codex exec rc={r.returncode}: {r.stderr[-400:]}"
                if "usage limit" in tail:
                    err = "quota: " + tail[tail.find("usage limit"):][:120].split('"')[0]
            for line in open(events, errors="replace"):
                if '"turn.completed"' in line:
                    usage = json.loads(line).get("usage", {})
        else:
            cmd = ["claude", "-p", "--model", model, "--effort", effort, "--permission-mode", "dontAsk",
                   "--allowedTools", *CLAUDE_ALLOW, "--no-session-persistence", "--strict-mcp-config",
                   "--disable-slash-commands", "--output-format", "json"]
            state(d, login="claude main")
            r = subprocess.run(cmd, input=prompt, capture_output=True, text=True, cwd=tree, timeout=a.timeout)
            open(events, "w").write(r.stdout)
            try:
                res = json.loads(r.stdout)
            except ValueError:
                res = {"is_error": True, "result": (r.stderr or r.stdout)[-400:]}
            if res.get("is_error") or not res.get("result"):
                err = f"claude -p rc={r.returncode}: {str(res.get('result'))[:400]}"
                if "limit" in err.lower():
                    err = "quota: " + err
            else:
                open(report, "w").write(res["result"])
            u = res.get("usage", {})
            usage = dict(input_tokens=u.get("input_tokens", 0) + u.get("cache_read_input_tokens", 0) +
                         u.get("cache_creation_input_tokens", 0), cached_input_tokens=u.get("cache_read_input_tokens", 0),
                         output_tokens=u.get("output_tokens", 0), cost_usd=res.get("total_cost_usd", 0),
                         turns=res.get("num_turns", 0))
    except subprocess.TimeoutExpired:
        err = f"timeout after {a.timeout} s (its tree keeps what it changed)"
    sh("git", "-C", tree, "add", "-A", "-N")  # new files show in the diff
    patch = subprocess.run(["git", "-C", tree, "diff", "HEAD", "--", ".", ":!build"], capture_output=True, text=True).stdout
    open(os.path.join(d, "patch.diff"), "w").write(patch)
    state(d, finished=time.time(), phase=("quota" if err.startswith("quota") else "failed") if err else "done",
          error=err, usage=usage,
          patch_lines=patch.count("\n"))
    print(f"deep worker {a.name}: {'FAILED: ' + err if err else 'done'} in {time.time() - t0:.0f} s; "
          f"report {os.path.relpath(report, ROOT)}, patch {patch.count(chr(10))} lines", flush=True)
    return 1 if err else 0


def start(a):
    """Detached: the job outlives the caller; its last line lands in work/deep/NAME/run.log."""
    task = sys.stdin.read() if a.task == "-" else open(a.task).read()
    d = os.path.join(DEEP, a.name)
    os.makedirs(d, exist_ok=True)
    open(os.path.join(d, "task.md"), "w").write(task)
    cmd = [sys.executable, os.path.abspath(__file__), "run", a.name, "--task", os.path.join(d, "task.md"),
           "--agent", a.agent, "--timeout", str(a.timeout)] + (["--fresh"] if a.fresh else [])
    subprocess.Popen(cmd, stdout=open(os.path.join(d, "run.log"), "a"), stderr=subprocess.STDOUT,
                     stdin=subprocess.DEVNULL, start_new_session=True, cwd=ROOT)
    print(f"deep worker {a.name} ({a.agent}) started; work/deep/{a.name}/run.log")
    return 0


def review(a):
    d = os.path.join(DEEP, a.name)
    if not os.path.exists(os.path.join(d, "state.json")):
        raise SystemExit(f"no job {a.name}")
    state(d, review=a.verdict, note=a.note, reviewed=time.time())
    return 0


def jobs():
    """Every job's state.json, a running one whose process is gone marked dead (for list and tools/webmon.py)."""
    out = []
    for name in sorted(os.listdir(DEEP)) if os.path.isdir(DEEP) else []:
        p = os.path.join(DEEP, name, "state.json")
        try:
            st = json.load(open(p))
        except (OSError, ValueError):
            continue
        if "started" not in st:
            continue
        if st["phase"] == "running" and not os.path.exists(f"/proc/{st['pid']}"):
            st["phase"] = "dead"
        st["has_report"] = os.path.exists(os.path.join(DEEP, name, "report.md"))
        if "eas" not in st:  # started by an older version of this tool
            try:
                st["eas"] = sorted(set(re.findall(r"\b00[4-7][0-9a-f]{5}\b", open(os.path.join(DEEP, name, "task.md")).read())))
            except OSError:
                st["eas"] = []
        out.append(st)
    return out


def ls(_):
    for st in jobs():
        secs = (st.get("finished") or time.time()) - st["started"]
        print(f"{st['name']:20} {st.get('agent', 'sol-hi'):8} {st['phase']:8} {secs / 60:5.0f} min  patch "
              f"{st.get('patch_lines', '-'):>5}  {st.get('review') or '-':9} {st.get('task', '')[:70]}")
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
    for cmd, fn in (("run", run), ("start", start)):
        r = sub.add_parser(cmd)
        r.add_argument("name")
        r.add_argument("--task", required=True, help="file with the task, or - for stdin")
        r.add_argument("--agent", default="sol-hi", choices=sorted(AGENTS))
        r.add_argument("--timeout", type=int, default=3600)
        r.add_argument("--fresh", action="store_true", help="throw the job's old tree away first")
        r.set_defaults(fn=fn)
    rv = sub.add_parser("review")
    rv.add_argument("name")
    rv.add_argument("verdict", choices=["applied", "partial", "rejected"])
    rv.add_argument("--note", default="")
    rv.set_defaults(fn=review)
    sub.add_parser("list").set_defaults(fn=ls)
    dp = sub.add_parser("drop")
    dp.add_argument("name")
    dp.set_defaults(fn=drop)
    a = ap.parse_args()
    return a.fn(a)


if __name__ == "__main__":
    sys.exit(main())
