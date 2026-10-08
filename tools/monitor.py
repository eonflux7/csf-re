#!/usr/bin/env python3
"""Live terminal dashboard for the RE workflow (portrait-friendly: panels stacked vertically).

  .venv/bin/python tools/monitor.py [--interval 2]

Reads only files, never the IDB:
  Claude      ~/.claude/jobs/<id>/state.json (newest job for this repo): state, current activity, background
              shells, session tokens; timeline.jsonl for recent activity
  Grunts      work/grunt_state.json (written live by tools/grunt.py: in-flight model calls, token totals,
              run counters), work/grunt_log.jsonl (finished functions)
  Processes   ps: grunt / smoke / rwname / codex exec; the game via tasklist.exe (every 10 s)
  Plan        docs/PLAN.md (current phase, next open items)
  Progress    db/functions.csv; git log since the Claude session started
"""
import argparse, collections, csv, glob, json, os, re, subprocess, time, urllib.request
from datetime import datetime, timezone
import sys
sys.path.insert(0, os.path.dirname(os.path.abspath(__file__)))
from limits import codex_usage, deepseek_balance  # noqa: E402

from rich.console import Group
from rich.live import Live
from rich.panel import Panel
from rich.table import Table
from rich.text import Text

ROOT = os.path.dirname(os.path.dirname(os.path.abspath(__file__)))
JOBS = os.path.expanduser("~/.claude/jobs")
LIVE = os.path.join(ROOT, "work", "grunt_state.json")
LOG = os.path.join(ROOT, "work", "grunt_log.jsonl")
BACKEND_STYLE = {"codex": "magenta", "deepseek": "cyan", "claude": "yellow"}
_cache = {}


def cached(key, ttl, fn):
    """fn() result, recomputed at most every ttl seconds."""
    t, v = _cache.get(key, (0, None))
    if time.time() - t > ttl:
        try:
            v = fn()
        except Exception as e:  # a half-written file etc.: keep the panel alive
            v = v if v is not None else e
        _cache[key] = (time.time(), v)
    return v


def ago(ts):
    s = max(0, int(time.time() - ts))
    return f"{s}s" if s < 60 else f"{s // 60}m{s % 60:02d}s" if s < 3600 else f"{s // 3600}h{s % 3600 // 60:02d}m"


def iso(s):
    return datetime.fromisoformat(s.replace("Z", "+00:00")).timestamp()


def kfmt(n):
    return f"{n / 1e6:.2f}M" if n >= 1e6 else f"{n / 1e3:.1f}k" if n >= 1e3 else str(n)


def pid_alive(pid):
    try:
        os.kill(pid, 0)
        return True
    except (OSError, TypeError):
        return False


# ---------------------------------------------------------------- Claude

def repo_root(p):
    """Main checkout path for a path inside it or inside one of its .claude/worktrees/."""
    return (p or "").split("/.claude/worktrees/")[0].rstrip("/")


def claude_jobs():
    """Claude jobs for this repo (worktree jobs included), newest first: every one not done or touched in the
    last 10 min; the newest job when none is active."""
    jobs = []
    for p in glob.glob(os.path.join(JOBS, "*", "state.json")):
        try:
            d = json.load(open(p))
        except (OSError, ValueError):
            continue
        if repo_root(d.get("originCwd") or d.get("cwd")) != repo_root(ROOT):
            continue
        d["_dir"] = os.path.dirname(p)
        jobs.append(d)
    jobs.sort(key=lambda d: d.get("updatedAt", ""), reverse=True)
    active = [d for d in jobs if d.get("state") != "done" or time.time() - iso(d["updatedAt"]) < 600]
    return active or jobs[:1]


def context_tokens(path):
    """Context size at the session's last main-chain turn: input + cache read + cache write of its usage."""
    with open(path, "rb") as f:
        f.seek(0, 2)
        f.seek(max(0, f.tell() - 400_000))
        lines = f.read().decode(errors="replace").splitlines()[1:]
    for l in reversed(lines):
        if '"usage"' not in l:
            continue
        try:
            e = json.loads(l)
        except ValueError:
            continue
        u = (e.get("message") or {}).get("usage")
        if e.get("type") == "assistant" and not e.get("isSidechain") and u:
            return sum(u.get(k) or 0 for k in ("input_tokens", "cache_read_input_tokens",
                                                "cache_creation_input_tokens"))
    return None


def job_ctx(job):
    p = job.get("linkScanPath")
    v = cached("ctx:" + job["_dir"], 5, lambda: context_tokens(p) if p else None)
    return v if isinstance(v, int) else None


def timeline(job, n=6):
    p = os.path.join(job["_dir"], "timeline.jsonl")
    if not os.path.exists(p):
        return []
    with open(p, "rb") as f:
        f.seek(0, 2)
        f.seek(max(0, f.tell() - 60000))
        lines = f.read().decode(errors="replace").splitlines()[1:]
    out = []
    for l in lines:
        try:
            e = json.loads(l)
        except ValueError:
            continue
        if e.get("detail") and (not out or out[-1]["detail"] != e["detail"]):
            out.append(e)
    return out[-n:]


def claude_usage():
    """5h / 7d windows of the Claude plan, from the endpoint Claude Code's /usage reads (own OAuth token)."""
    tok = json.load(open(os.path.expanduser("~/.claude/.credentials.json")))["claudeAiOauth"]["accessToken"]
    req = urllib.request.Request("https://api.anthropic.com/api/oauth/usage", headers={
        "Authorization": f"Bearer {tok}", "anthropic-beta": "oauth-2025-04-20", "User-Agent": "csf-re-monitor"})
    d = json.load(urllib.request.urlopen(req, timeout=8))
    return {k: (d[w]["utilization"], iso(d[w]["resets_at"]) - time.time()) for k, w in
            (("5h", "five_hour"), ("7d", "seven_day")) if d.get(w)}


def usage_line(label, u, style):
    t = Text(f"{label} ", style=style)
    if not isinstance(u, dict):
        t.append(f"n/a: {str(u)[:40]}", style="dim")
        return t
    for k, (pct, reset) in u.items():
        t.append(f" {k} ")
        t.append_text(bar(pct, 10))
        t.append(f" {pct:>3.0f}% ↻{dur(reset)} ", style="dim" if pct < 70 else "bold")
    return t


STATE_STYLE = {"working": "bold green", "waiting": "bold yellow", "blocked": "bold red", "done": "dim"}
AGENT_COLORS = ["bright_cyan", "bright_magenta", "bright_yellow", "bright_blue", "orange1", "spring_green1"]


def ctx_text(n):
    style = "dim" if n is None else "red" if n >= 160_000 else "yellow" if n >= 100_000 else "cyan"
    return Text(f"ctx {kfmt(n) if n is not None else '?':>6}", style=style)


def panel_claude():
    jobs = cached("jobs", 1, claude_jobs)
    if not jobs or isinstance(jobs, Exception):
        return Panel("no Claude job for this repo", title="Claude", border_style="dim")
    multi = len(jobs) > 1
    # one colour per agent, by start order, so it doesn't jump around as agents take turns being the newest
    color = {j["_dir"]: AGENT_COLORS[i % len(AGENT_COLORS)]
             for i, j in enumerate(sorted(jobs, key=lambda d: d.get("createdAt", "")))}
    plan = usage_line("plan", cached("claude_usage", 60, claude_usage), "bold green")
    if not multi:
        job = jobs[0]
        state = job.get("state", "?")
        head = Text()
        head.append(f"{state.upper()}", style=STATE_STYLE.get(state, "bold"))
        head.append(f"  {job.get('tempo', '')}  ", style="dim")
        head.append_text(ctx_text(job_ctx(job)))
        head.append(f"  tokens {kfmt(job.get('tokens') or 0)}  ", style="cyan")
        head.append(f"up {ago(iso(job['createdAt']))}  upd {ago(iso(job['updatedAt']))} ago", style="dim")
        body = [head, plan, Text(job.get("detail") or "", style="bold white", overflow="ellipsis", no_wrap=True)]
    else:
        body = [plan]
        for j in jobs:  # one Text per row: a grid collapses the short columns when the detail is long
            state, c = j.get("state", "?"), color[j["_dir"]]
            name = j.get("name", "")
            t = Text("● ", style=c, no_wrap=True, overflow="ellipsis")
            t.append(f"{name[:23] + '…' if len(name) > 24 else name:<24} ", style="bold " + c)
            t.append(f"{state.upper():<8}", style=STATE_STYLE.get(state, "bold"))
            t.append_text(ctx_text(job_ctx(j)))
            t.append(f"  up {ago(iso(j['createdAt'])):>6}  ", style="dim")
            t.append((j.get("detail") or "").replace("\n", " "))
            body.append(t)
    # events of all agents in one newest-first list, each marked with its agent's colour when there are several
    ev = []
    for j in jobs:
        tl = timeline(j)
        ev += [(e, j) for e in (tl if multi else tl[:-1])]  # single agent: its newest event is the headline
    ev.sort(key=lambda x: x[0]["at"], reverse=True)
    for e, j in ev[:8 if multi else 5]:
        t = Text(f"  {ago(iso(e['at'])):>6} ", style="dim")
        if multi:
            t.append("● ", style=color[j["_dir"]])
        t.append(e["detail"].splitlines()[0][:200], style="grey62")
        t.no_wrap, t.overflow = True, "ellipsis"
        body.append(t)
    fan = [(s, j) for j in jobs for s in (j.get("fan") or [])]
    if fan:
        tb = Table.grid(padding=(0, 1))
        tb.add_column(no_wrap=True, min_width=9)
        tb.add_column(style="dim", no_wrap=True, justify="right", min_width=6)
        tb.add_column(no_wrap=True, overflow="ellipsis", ratio=1)
        for s, j in fan:
            tb.add_row(Text(("● " if multi else "") + s.get("id", "?"), style=color[j["_dir"]] if multi else "cyan"),
                       ago(s.get("startedAt", 0) / 1000), s.get("label", "").replace("\n", " "))
        body += [Text(f"background shells ({len(fan)})", style="bold"), tb]
    title = f"Claude · {len(jobs)} agents" if multi else f"Claude · {jobs[0].get('name', '')}"
    return Panel(Group(*body), title=title, border_style="green")


# ---------------------------------------------------------------- Grunts

def read_log_since(ts):
    out = []
    if not os.path.exists(LOG):
        return out
    with open(LOG, "rb") as f:
        f.seek(0, 2)
        f.seek(max(0, f.tell() - 3_000_000))
        for l in f.read().decode(errors="replace").splitlines()[1:]:
            try:
                r = json.loads(l)
            except ValueError:
                continue
            if r.get("ts", 0) >= ts:
                out.append(r)
    return out


def names():
    return {int(r["ea"], 16): r["name"] for r in csv.DictReader(open(os.path.join(ROOT, "db", "functions.csv")))}


def panel_grunts():
    st = cached("live", 1, lambda: json.load(open(LIVE)) if os.path.exists(LIVE) else None)
    nm = cached("names", 30, names)
    nm = nm if isinstance(nm, dict) else {}
    alive = isinstance(st, dict) and pid_alive(st.get("pid"))
    body = []
    if not isinstance(st, dict):
        body.append(Text("no grunt state yet (work/grunt_state.json is written by grunt.py run/lanes)", style="dim"))
        since = time.time() - 3600
    else:
        since = st.get("started", time.time())
        c = st.get("counts", {})
        h = Text()
        h.append("RUNNING " if alive else "stopped ", style="bold green" if alive else "bold red")
        h.append(f"{st.get('mode', '')} pid {st.get('pid')}  ", style="dim")
        ph = st.get("phase", "")
        h.append(ph, style="bold red" if "FAIL" in ph else "bold yellow" if ph == "smoke" else "bold")
        h.append(f"  up {ago(since)}", style="dim")
        body.append(h)
        cfg = st.get("config")
        if cfg:
            body += config_lines(cfg)
        if st.get("codex_paused"):
            body.append(Text(f"Codex PAUSED by the quota guard (5h at {st.get('codex_5h', 0):.0f}%): main lane idle, "
                             "Codex work deferred", style="bold red"))
        done = c.get("ok", 0) + c.get("esc", 0)
        el = max(1, time.time() - since)
        line = Text()
        line.append(f"ok {c.get('ok', 0)}", style="green")
        line.append(f"  esc {c.get('esc', 0)}", style="red")
        if c.get("deferred"):
            line.append(f"  deferred {c['deferred']}", style="yellow")
        line.append(f"  pending {c.get('pending', 0)}/{c.get('batch', '-')}", style="yellow")
        line.append(f"  committed {c.get('committed', 0)}", style="cyan")
        line.append(f"  submitted {c.get('submitted', 0)}/{c.get('total', '-')}", style="dim")
        line.append(f"  {done / el * 3600:.0f} fn/h", style="bold")
        body.append(line)
        calls = sorted((st.get("calls") or {}).values(), key=lambda x: x["since"])
        if calls and alive:
            tb = Table(box=None, padding=(0, 1), show_header=True, header_style="dim")
            for col, kw in (("ea", {"min_width": 11}), ("backend", {"min_width": 8}), ("eff", {"min_width": 5}),
                            ("kind", {"min_width": 8}), ("age", {"justify": "right", "min_width": 5}),
                            ("name", {"overflow": "ellipsis", "ratio": 1})):
                tb.add_column(col, no_wrap=True, **kw)
            for x in calls:
                b = x["backend"]
                n = x.get("n", 1)
                kind = f"batch×{n}" if x["kind"] == "batch" else x["kind"]
                lbl = b + ("·cli" if x.get("transport") == "cli" else "")
                tb.add_row(f"{x['ea']:08x}" + (f"+{n - 1}" if n > 1 else ""), Text(lbl, style=BACKEND_STYLE.get(b, "")),
                           x["effort"], Text(kind, style="bold" if n > 1 else ""), ago(x["since"]),
                           nm.get(x["ea"], "") or "")
            fns = sum(x.get("n", 1) for x in calls if x["kind"] != "review")
            body.append(Text(f"in flight ({len(calls)} calls, {fns} functions)", style="bold"))
            body.append(tb)
    recent = cached("recent", 3, lambda: read_log_since(since))
    recent = recent if isinstance(recent, list) else []
    if recent:
        lines = []
        for r in recent[-10:][::-1]:
            ok = r.get("ok")
            what = r.get("meta", {}).get("name", "") if ok else (r.get("err", "").splitlines() or [""])[0]
            by = (r.get("by") or "").split(":")[0] or {"deep": "ds-lane", "main": "gpt-lane"}.get(r.get("lane"), "")
            t = Text(f"{ago(r.get('ts', 0)):>6} ", style="dim", no_wrap=True, overflow="ellipsis")
            t.append(f"{by:<8} ", style=BACKEND_STYLE.get(by, "dim"))
            t.append(f"{r['ea']:08x} ")
            t.append("OK  " if ok else "DEF " if r.get("deferred") else "ESC ",
                     style="green" if ok else "yellow" if r.get("deferred") else "red")
            tags = []
            if r.get("batch"):
                tags.append(f"b{r['batch']}")
            if ok and r.get("attempts", 1) > 1:
                tags.append(f"try{r['attempts']}")
            if r.get("reviews"):
                tags.append("rev✗" if any(r["reviews"]) else "rev✓")
            t.append(f"{' '.join(tags):<10} ", style="dim")
            t.append(what, style="" if ok else "grey62")
            lines.append(t)
        body.append(Text("recent results", style="bold"))
        body += lines
    return Panel(Group(*body), title="Grunts", border_style="magenta"), (st if isinstance(st, dict) else None), recent


def config_lines(c):
    """The cost/speed settings the run started with (grunt.run_config)."""
    t = Text("codex    ", style="magenta")
    t.append(f"{c.get('transport')} · {c.get('effort')}/{c.get('retry_effort')} · ")
    if (c.get("batch_fns") or 1) > 1:
        t.append(f"batch ≤{c["batch_fns"]}" + (f"/{c["batch_bytes"]}B" if c.get("batch_bytes") else "")
                 + f" (fns ≤{c.get("batch_max")}B)")
    else:
        t.append("no batching", style="yellow")
    t.append(f" · class {c.get("class_view")}", style="" if c.get("class_view") == "compact" else "yellow")
    t.no_wrap, t.overflow = True, "ellipsis"
    d = Text("deepseek ", style="cyan")
    t, d = d, t
    t.append(f"<{c.get('tiny')} B" + (" + clones" if c.get("deep_clones") else ""))
    ro = c.get("review_over")
    t.append(f" · review >{ro} B" if ro is not None and ro >= 0 else " · no review", style="dim")
    if c.get("codex_cap"):
        t.append(f" · codex cap {c['codex_cap']:.0f}% of 5h", style="dim")
    t.no_wrap, t.overflow = True, "ellipsis"
    return [d, t]


def efficiency(recent):
    """Per-function cost and the savings levers, over this run's finished functions."""
    fns = [r for r in recent if not r.get("deferred")]
    if not fns:
        return []
    cx = [u for r in fns for u in r.get("usage", []) if u.get("backend") == "codex"]
    cin = sum((u.get("prompt_tokens") or 0) - (u.get("cached_tokens") or 0) for u in cx)
    cca = sum(u.get("cached_tokens") or 0 for u in cx)
    cout = sum(u.get("completion_tokens") or 0 for u in cx)
    n = len(fns)
    ok = [r for r in fns if r.get("ok")]
    first = sum(r.get("attempts", 1) == 1 for r in ok)
    bat = [r for r in fns if r.get("batch")]
    revs = [x for r in fns for x in (r.get("reviews") or [])]
    out = [Text("per function (this run)", style="bold")]
    out.append(Text(f" codex in {kfmt(cin // n)} out {kfmt(cout // n)}"
                    + (f"  cache {cca / max(1, cin + cca):.0%}" if cx else "")))
    t = Text(f" 1st try {first / max(1, len(ok)):.0%}", style="green" if first >= 0.75 * len(ok) else "yellow")
    t.append(f"  batched {len(bat) / n:.0%}", style="dim")
    if bat:
        t.append(f" (avg {sum(r['batch'] for r in bat) / len(bat):.1f})", style="dim")
    out.append(t)
    if revs:
        out.append(Text(f" reviews {len(revs)}, caught {sum(revs)}", style="dim"))
    return out


def bar(pct, width=12):
    n = round(pct / 100 * width)
    style = "red" if pct >= 90 else "yellow" if pct >= 70 else "green"
    t = Text("█" * n, style=style)
    t.append("░" * (width - n), style="dim")
    return t


def dur(s):
    s = int(s)
    return f"{s // 86400}d{s % 86400 // 3600}h" if s >= 86400 else f"{s // 3600}h{s % 3600 // 60:02d}m" if s >= 3600 \
        else f"{s // 60}m"


def panel_tokens(st, recent):
    """Compact (fits beside the grunt panel): per-backend tokens of the current run + subscription/balance limits."""
    tot = (st or {}).get("tokens") or {}
    if not tot:
        agg = collections.defaultdict(lambda: dict(calls=0, prompt=0, cached=0, completion=0))
        for r in recent:
            for u in r.get("usage", []):
                a = agg[u.get("backend", "deepseek")]
                a["calls"] += 1
                a["prompt"] += u.get("prompt_tokens", 0) or 0
                a["cached"] += u.get("cached_tokens", 0) or 0
                a["completion"] += u.get("completion_tokens", 0) or 0
        tot = agg
    rate = collections.Counter()
    for r in recent:
        if r.get("ts", 0) > time.time() - 600:
            for u in r.get("usage", []):
                rate[u.get("backend", "deepseek")] += u.get("completion_tokens", 0) or 0
    body = []
    for b, t in sorted(tot.items()):
        h = Text(f"{b} ", style="bold " + BACKEND_STYLE.get(b, ""))
        h.append(f"{t['calls']} calls", style="dim")
        body.append(h)
        body.append(Text(f" in  {kfmt(t['prompt'])}" + (f" ({t['cached'] / max(1, t['prompt']):.0%} cached)"
                                                         if t["cached"] else "")))
        body.append(Text(f" out {kfmt(t['completion'])}  {kfmt(rate[b] // 10)}/min"))
    if not tot:
        body.append(Text("no calls yet", style="dim"))
    body += efficiency(recent)
    c = (st or {}).get("counts") or {}
    if c.get("autofix"):
        body.append(Text(f" auto-fixed {c['autofix']} (retries saved)", style="dim"))
    if c.get("transport_retry"):
        body.append(Text(f" transport retries {c['transport_retry']}", style="yellow"))
    body.append(Text(""))
    cu = cached("codex_usage", 60, codex_usage)
    body.append(Text("Codex (ChatGPT plan)", style="bold magenta"))
    if isinstance(cu, dict):
        for k, (pct, reset) in cu.items():
            t = Text(f" {k} ")
            t.append_text(bar(pct))
            t.append(f" {pct:>3.0f}% ↻{dur(reset)}", style="dim" if pct < 70 else "bold")
            body.append(t)
    else:
        body.append(Text(f" n/a: {str(cu)[:40]}", style="dim"))
    db_ = cached("ds_balance", 120, deepseek_balance)
    t = Text("DeepSeek ", style="bold cyan")
    if isinstance(db_, tuple):
        t.append("  ".join(f"{amt} {cur}" for cur, amt in db_[0]) or "?",
                 style="green" if db_[1] else "bold red")
    else:
        t.append(f"n/a: {str(db_)[:30]}", style="dim")
    body.append(t)
    return Panel(Group(*body), title="Tokens · limits", border_style="cyan")


# ---------------------------------------------------------------- processes

PROC_PAT = re.compile(r"tools/(grunt|smoke|rwname|deploy|semcheck|difftable|dbg)|codex exec|tools/ida/")


def procs():
    out = subprocess.run(["ps", "-eo", "pid,etimes,args", "--no-headers"], capture_output=True, text=True).stdout
    rows, codex = [], 0
    for l in out.splitlines():
        pid, et, args = l.strip().split(None, 2)
        if "monitor.py" in args or not PROC_PAT.search(args) or args.startswith(("/bin/bash -c", "bash -c")):
            continue
        if "codex exec" in args:
            if not args.startswith("node"):
                codex += 1
            continue
        rows.append((int(et), pid, args))
    return sorted(rows), codex


def game():
    r = subprocess.run(["tasklist.exe", "/FI", "IMAGENAME eq CommXPC.exe", "/FO", "CSV", "/NH"],
                       capture_output=True, text=True, timeout=5).stdout
    m = re.search(r'"CommXPC\.exe","(\d+)".*?"([\d,.\s]+K)"', r)
    return (m.group(1), m.group(2)) if m else None


def panel_procs():
    pr = cached("procs", 2, procs)
    rows, codex = pr if isinstance(pr, tuple) else ([], 0)
    g = cached("game", 10, game)
    tb = Table.grid(padding=(0, 1))
    tb.add_column(style="dim", justify="right", no_wrap=True, min_width=6)
    tb.add_column(no_wrap=True, overflow="ellipsis", ratio=1)
    for et, pid, args in rows[-8:]:
        a = re.sub(r"^\S*python\S*\s+", "", args).replace(ROOT + "/", "")
        tb.add_row(ago(time.time() - et), a)
    head = Text()
    head.append("game ", style="bold")
    head.append(f"RUNNING pid {g[0]} {g[1]}" if isinstance(g, tuple) else "not running",
                style="green" if isinstance(g, tuple) else "dim")
    if codex:
        head.append(f"   codex exec (cli transport): {codex}", style="magenta")
    return Panel(Group(head, tb), title="Processes", border_style="blue")


# ---------------------------------------------------------------- plan / progress / session

def plan():
    txt = open(os.path.join(ROOT, "docs", "PLAN.md")).read()
    phases = re.split(r"\n(?=### )", txt)
    for ph in phases:
        m = re.match(r"### (.+)", ph)
        if not m:
            continue
        items = re.findall(r"^- \[([ x])\] (.+)$", ph, re.M)
        open_ = [t for x, t in items if x == " "]
        if items and open_:
            return m.group(1), sum(x == "x" for x, _ in items), len(items), open_
    return "-", 0, 0, []


def all_phases():
    txt = open(os.path.join(ROOT, "docs", "PLAN.md")).read()
    out = []
    for ph in re.split(r"\n(?=### )", txt):
        m = re.match(r"### (.+)", ph)
        items = re.findall(r"^- \[([ x])\] ", ph, re.M)
        if m and items:
            out.append((m.group(1).split("—")[0].strip(), sum(x == "x" for x in items), len(items)))
    return out


def progress():
    rows = list(csv.DictReader(open(os.path.join(ROOT, "db", "functions.csv"))))
    g = [r for r in rows if r["region"] == "game"]
    done = [r for r in g if r["status"] in ("done", "verified")]
    return dict(game=len(g), done=len(done), bytes=sum(int(r["size"]) for r in g),
                done_bytes=sum(int(r["size"]) for r in done),
                stl=sum(r["module"] == "stl" for r in done), stl_all=sum(r["module"] == "stl" for r in g),
                named=sum(bool(r["name"]) for r in rows if r["region"] == "rw"))


def commits(since):
    out = subprocess.run(["git", "-C", ROOT, "log", f"--since=@{int(since)}", "--format=%ct\t%h\t%s"],
                         capture_output=True, text=True).stdout
    return [l.split("\t", 2) for l in out.splitlines() if l.count("\t") == 2]


def panel_plan():
    p = cached("plan", 10, plan)
    phases = cached("phases", 10, all_phases)
    body = []
    if isinstance(phases, list):
        t = Text()
        for name, d, n in phases:
            t.append(f"{name.replace('Phase ', 'P')} ", style="bold")
            t.append(f"{d}/{n}  ", style="green" if d == n else "yellow")
        body.append(t)
    if isinstance(p, tuple):
        name, d, n, open_ = p
        body.append(Text(f"{name}  ({d}/{n})", style="bold"))
        for it in open_[:6]:
            body.append(Text("  ☐ " + re.sub(r"`", "", it), no_wrap=True, overflow="ellipsis"))
    return Panel(Group(*body), title="Plan", border_style="yellow")


def panel_session():
    pg = cached("progress", 20, progress)
    jobs = cached("jobs", 1, claude_jobs)
    since = min(iso(j["createdAt"]) for j in jobs) if isinstance(jobs, list) and jobs else time.time() - 86400
    cm = cached("commits", 10, lambda: commits(since))
    body = []
    if isinstance(pg, dict):
        pct = pg["done_bytes"] / max(1, pg["bytes"]) * 100
        t = Text()
        t.append(f"game {pg['done']}/{pg['game']} fns", style="bold green")
        t.append(f"  {pct:.2f}% bytes", style="green")
        t.append(f"  stl {pg['stl']}/{pg['stl_all']}", style="dim")
        t.append(f"  rw named {pg['named']}", style="dim")
        body.append(t)
    if isinstance(cm, list):
        body.append(Text(f"commits this session: {len(cm)}", style="bold"))
        for ct, h, s in cm[:10]:
            t = Text(f"  {ago(int(ct)):>6} ", style="dim")
            t.append(h + " ", style="yellow")
            t.append(s)
            t.no_wrap, t.overflow = True, "ellipsis"
            body.append(t)
    return Panel(Group(*body), title="Done this session", border_style="white")


def render():
    g, st, recent = panel_grunts()
    clock = Text(datetime.now().strftime("%H:%M:%S") + "  csf-re monitor", style="dim", justify="right")
    side = Table.grid(expand=True)
    side.add_column(ratio=3)
    side.add_column(ratio=2, min_width=34, max_width=44)
    side.add_row(g, panel_tokens(st, recent))
    return Group(clock, panel_claude(), side, panel_procs(), panel_plan(), panel_session())


def main():
    ap = argparse.ArgumentParser()
    ap.add_argument("--interval", type=float, default=2)
    ap.add_argument("--once", action="store_true", help="print one frame and exit")
    a = ap.parse_args()
    if a.once:
        from rich.console import Console
        Console().print(render())
        return
    with Live(render(), refresh_per_second=4, screen=True) as lv:
        while True:
            time.sleep(a.interval)
            lv.update(render())


if __name__ == "__main__":
    try:
        main()
    except KeyboardInterrupt:
        pass
