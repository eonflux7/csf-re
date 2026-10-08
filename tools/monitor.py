#!/usr/bin/env python3
"""Live terminal dashboard for the RE workflow (Textual app in tools/monitor_app.py; this module holds the data
readers and Rich renderables, and `--once` prints one Rich frame): Claude | Progress, grunt lanes, captain,
game/tools, subscription limits with login + billing, plan, commits, escalation queue.

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
from rich.panel import Panel
from rich.rule import Rule
from rich.table import Table
from rich.text import Text

ROOT = os.path.dirname(os.path.dirname(os.path.abspath(__file__)))
JOBS = os.path.expanduser("~/.claude/jobs")
LIVE = os.path.join(ROOT, "work", "grunt_state.json")
LOG = os.path.join(ROOT, "work", "grunt_log.jsonl")
CAPTAIN = os.path.join(ROOT, "work", "captain_state.json")
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


USAGE_CACHE = os.path.join(ROOT, "work", "claude_usage.json")


def claude_usage():
    """5h / 7d windows of the Claude plan, from the endpoint Claude Code's /usage reads (own OAuth token).
    Rate-limited (429) when polled often: asked at most every 10 min (across monitor restarts: work/
    claude_usage.json), 30 min after an error; meanwhile the last good answer is shown with its age."""
    try:
        c = json.load(open(USAGE_CACHE))
    except (OSError, ValueError):
        c = {}
    wait = 1800 if c.get("error") else 600
    if time.time() - c.get("asked", 0) >= wait:
        c["asked"] = time.time()
        try:
            tok = json.load(open(os.path.expanduser("~/.claude/.credentials.json")))["claudeAiOauth"]["accessToken"]
            req = urllib.request.Request("https://api.anthropic.com/api/oauth/usage", headers={
                "Authorization": f"Bearer {tok}", "anthropic-beta": "oauth-2025-04-20", "User-Agent": "csf-re-monitor"})
            d = json.load(urllib.request.urlopen(req, timeout=8))
            c.update(error=None, at=time.time(), u={k: (d[w]["utilization"], iso(d[w]["resets_at"])) for k, w in
                                                     (("5h", "five_hour"), ("7d", "seven_day")) if d.get(w)})
        except Exception as e:
            c["error"] = str(e)[:60]
        json.dump(c, open(USAGE_CACHE, "w"))
    if not c.get("u"):
        raise RuntimeError(c.get("error") or "no data yet")
    out = {k: (pct, at - time.time()) for k, (pct, at) in c["u"].items()}
    out["_age"] = time.time() - c.get("at", 0)
    return out


def usage_line(label, u, style):
    t = Text(f"{label} ", style=style)
    if not isinstance(u, dict):
        t.append(f"n/a: {str(u)[:40]}", style="dim")
        return t
    for k, v in u.items():
        if k == "_age":
            continue
        pct, reset = v
        t.append(f" {k} ")
        t.append_text(bar(pct, 10))
        t.append(f" {pct:>3.0f}% ↻{dur(max(0, reset))} ", style="dim" if pct < 70 else "bold")
    if u.get("_age", 0) > 900:
        t.append(f"({dur(u['_age'])} old)", style="dim")
    return t


STATE_STYLE = {"working": "bold green", "waiting": "bold yellow", "blocked": "bold red", "done": "dim"}
AGENT_COLORS = ["bright_cyan", "bright_magenta", "bright_yellow", "bright_blue", "orange1", "spring_green1"]


def ctx_text(n):
    style = "dim" if n is None else "red" if n >= 160_000 else "yellow" if n >= 100_000 else "cyan"
    return Text(f"ctx {kfmt(n) if n is not None else '?':>6}", style=style)


def accounts():
    """Login email, plan and next billing date of both subscriptions, from the local login files only (no calls).
    Codex: the id_token claims (chatgpt_subscription_active_until = paid through). Claude: ~/.claude.json
    oauthAccount; it has no renewal date, so the next monthly anniversary of subscriptionCreatedAt (estimate)."""
    import base64, datetime as dt
    out = {}
    try:
        tok = json.load(open(os.path.expanduser("~/.codex/auth.json")))["tokens"]["id_token"].split(".")[1]
        c = json.loads(base64.urlsafe_b64decode(tok + "=" * (-len(tok) % 4)))
        a = c.get("https://api.openai.com/auth", {})
        until = a.get("chatgpt_subscription_active_until")
        out["codex"] = (c.get("email", "?"), a.get("chatgpt_plan_type", "?"),
                        dt.datetime.fromisoformat(until).date() if until else None, False)
    except Exception:
        pass
    try:
        oa = json.load(open(os.path.expanduser("~/.claude.json")))["oauthAccount"]
        nxt = None
        if oa.get("subscriptionCreatedAt"):
            start = dt.datetime.fromisoformat(oa["subscriptionCreatedAt"].replace("Z", "+00:00")).date()
            today, y, m = dt.date.today(), start.year, start.month
            while True:
                last = (dt.date(y + m // 12, m % 12 + 1, 1) - dt.timedelta(days=1)).day
                d = dt.date(y, m, min(start.day, last))
                if d > today:
                    nxt = d
                    break
                y, m = (y + 1, 1) if m == 12 else (y, m + 1)
        plan = (oa.get("organizationType") or "?").replace("claude_", "")
        out["claude"] = (oa.get("emailAddress", "?"), plan, nxt, True)
    except Exception:
        pass
    return out


def account_line(which):
    a = cached("accounts", 300, accounts)
    if not isinstance(a, dict) or which not in a:
        return Text(" account n/a", style="dim")
    email, plan, nxt, est = a[which]
    t = Text(f" {email}", style="white")
    t.append(f"  {plan}", style="bold")
    if nxt:
        days = (nxt - __import__("datetime").date.today()).days
        t.append(f"  bills {'~' if est else ''}{nxt:%b %d} ({days}d)", style="bold yellow" if days <= 3 else "dim")
    return t


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


# ---------------------------------------------------------------- layout: Claude | Progress, then one Work panel

def panel_claude():
    """Compact: state, plan usage + account, what it is doing, the last few events."""
    jobs = cached("jobs", 1, claude_jobs)
    if not jobs or isinstance(jobs, Exception):
        return Panel(Group(usage_line("plan", cached("claude_usage", 30, claude_usage), "bold green"),
                           account_line("claude")), title="Claude · idle", border_style="dim")
    multi = len(jobs) > 1
    color = {j["_dir"]: AGENT_COLORS[i % len(AGENT_COLORS)]
             for i, j in enumerate(sorted(jobs, key=lambda d: d.get("createdAt", "")))}
    body = []
    for j in jobs:
        state = j.get("state", "?")
        t = Text(no_wrap=True, overflow="ellipsis")
        if multi:
            t.append("● ", style=color[j["_dir"]])
        t.append(f"{state.upper():<8}", style=STATE_STYLE.get(state, "bold"))
        t.append_text(ctx_text(job_ctx(j)))
        t.append(f"  up {ago(iso(j['createdAt']))}", style="dim")
        if j.get("fan"):
            t.append(f"  shells {len(j['fan'])}", style="cyan")
        body.append(t)
        body.append(Text("  " + (j.get("detail") or "").replace("\n", " "), style="bold white",
                         no_wrap=True, overflow="ellipsis"))
    body += [usage_line("plan", cached("claude_usage", 30, claude_usage), "bold green"), account_line("claude")]
    ev = [(e, j) for j in jobs for e in timeline(j)[:-1]]
    ev.sort(key=lambda x: x[0]["at"], reverse=True)
    for e, j in ev[:3]:
        t = Text(f"{ago(iso(e['at'])):>6} ", style="dim", no_wrap=True, overflow="ellipsis")
        if multi:
            t.append("● ", style=color[j["_dir"]])
        t.append(e["detail"].splitlines()[0][:200], style="grey62")
        body.append(t)
    title = f"Claude · {len(jobs)} agents" if multi else f"Claude · {jobs[0].get('name', '')}"
    return Panel(Group(*body), title=title, border_style="green")


def module_table():
    import yaml
    return yaml.safe_load(open(os.path.join(ROOT, "db", "modules.yaml")))


def wave_progress(spec):
    """Bytes/functions done in the wave's modules (grunt --module: ids or names), from functions.csv."""
    mods = cached("modules", 60, module_table)
    if not spec or not isinstance(mods, list):
        return None
    keys = [k.strip() for k in spec.split(",") if k.strip()]
    ids = {m["id"] for m in mods if m["id"] in keys or m.get("name") in keys}
    rows = cached("rows", 20, lambda: list(csv.DictReader(open(os.path.join(ROOT, "db", "functions.csv")))))
    if not isinstance(rows, list):
        return None
    w = [r for r in rows if r["module"] in ids]
    d = [r for r in w if r["status"] in ("done", "verified")]
    pre = os.path.commonprefix(keys)
    label = (pre.rstrip("_") + "_*" if len(keys) > 2 and len(pre) >= 3 else ",".join(keys)[:40])
    return dict(label=f"{label} ({len(ids)} modules)", n=len(w), done=len(d),
                bytes=sum(int(r["size"]) for r in w), done_bytes=sum(int(r["size"]) for r in d))


def panel_progress(width_hint=None):
    """Right of Claude: whole-game progress, phases, current wave, recent commits."""
    pg = cached("progress", 20, progress)
    phases = cached("phases", 10, all_phases)
    body = []
    if isinstance(pg, dict):
        pct = pg["done_bytes"] / max(1, pg["bytes"]) * 100
        t = Text("game  ", style="bold")
        t.append_text(bar(pct, 14))
        t.append(f" {pct:.2f}% bytes", style="bold green")
        body.append(t)
        body.append(Text(f"      {pg['done']:,}/{pg['game']:,} functions", style="green"))
    if isinstance(phases, list):
        t = Text()
        for name, d, n in phases:
            t.append(name.replace("Phase ", "P") + " ", style="bold")
            t.append(f"{d}/{n}  ", style="green" if d == n else "yellow")
        body.append(t)
    st = cached("live", 1, lambda: json.load(open(LIVE)) if os.path.exists(LIVE) else None)
    wp = wave_progress(((st or {}).get("config") or {}).get("wave")) if isinstance(st, dict) else None
    if wp:
        pct = wp["done_bytes"] / max(1, wp["bytes"]) * 100
        body.append(Text(f"wave  {wp['label']}", style="bold magenta", no_wrap=True, overflow="ellipsis"))
        t = Text("      ")
        t.append_text(bar(pct, 14))
        t.append(f" {pct:.0f}%  {wp['done']}/{wp['n']} fns")
        body.append(t)
    jobs = cached("jobs", 1, claude_jobs)
    since = min(iso(j["createdAt"]) for j in jobs) if isinstance(jobs, list) and jobs else time.time() - 86400
    cm = cached("commits", 10, lambda: commits(since))
    if isinstance(cm, list):
        body.append(Text(f"commits this session {len(cm)}", style="bold"))
        for ct, h, sub in cm[:4]:
            t = Text(f"{ago(int(ct)):>6} ", style="dim", no_wrap=True, overflow="ellipsis")
            t.append(sub)
            body.append(t)
    return Panel(Group(*body), title="Progress", border_style="yellow")


def captain_lines():
    st = cached("captain", 1, lambda: json.load(open(CAPTAIN)) if os.path.exists(CAPTAIN) else None)
    if not isinstance(st, dict):
        return []
    alive = pid_alive(st.get("pid"))
    if not alive and time.time() - st.get("updated", 0) > 3 * 3600:
        return []
    c, tk = st.get("counts", {}), st.get("tokens", {})
    tot = c.get("total", 0)
    h = Text("captain ", style="bold bright_blue")
    h.append("RUNNING " if alive else "idle    ", style="bold green" if alive else "dim")
    h.append(f"{st.get('job', '')} ", style="bold")
    h.append_text(bar(c.get("done", 0) / max(1, tot) * 100, 10))
    h.append(f" {c.get('done', 0)}/{tot}  ")
    h.append(f"✓{c.get('applied', 0)}", style="green")
    h.append(f" held {c.get('held', 0)}", style="yellow")
    if c.get("rejected"):
        h.append(f" ✗{c['rejected']}", style="red")
    if c.get("errors"):
        h.append(f" err {c['errors']}", style="bold red")
    h.append(f"  {st.get('model', '')} ${tk.get('cost_usd', 0):.2f}", style="dim")
    h.append("" if alive else f"  {ago(st.get('updated', 0))} ago", style="dim")
    h.no_wrap, h.overflow = True, "ellipsis"
    out = [h]
    cur = st.get("current")
    if cur and alive:
        out.append(Text(f"        calling #{cur.get('batch')} ({len(cur.get('addrs', []))} items) "
                        f"{ago(cur.get('since', time.time()))}", style="cyan"))
    rec = st.get("recent", [])[:3] if alive else []
    for r in rec:
        a, n, ty, conf = (list(r) + [None] * 4)[:4]
        t = Text(f"        {n}", style="green" if ty else "yellow", no_wrap=True, overflow="ellipsis")
        t.append(f" {conf}{'' if ty else ' held'}", style="dim")
        out.append(t)
    return out


def tool_line():
    """The game and the short-lived tools grunt/captain start (smoke, deploy, bisect, IDA), one line."""
    pr = cached("procs", 2, procs)
    rows, codex = pr if isinstance(pr, tuple) else ([], 0)
    g = cached("game", 10, game)
    t = Text("game ", style="bold")
    t.append(f"running {g[1]}" if isinstance(g, tuple) else "off", style="green" if isinstance(g, tuple) else "dim")
    for et, pid, args in rows:
        m = re.search(r"tools/((?:ida/)?\w+)\S*\s*(\S*)", args)
        if not m or m.group(1) in ("grunt", "captain"):
            continue
        t.append(f"  {m.group(1)} {m.group(2)[:14]} {ago(time.time() - et)}".rstrip(), style="cyan")
    if codex:
        t.append(f"  codex exec ×{codex}", style="magenta")
    t.no_wrap, t.overflow = True, "ellipsis"
    return t


def limits_grid(st, recent):
    """Subscriptions/balances with account + billing, and this run's token use per backend."""
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

    def run(b):
        t = tot.get(b)
        if not t:
            return Text(" run: no calls", style="dim")
        return Text(f" run: {t['calls']} calls in {kfmt(t['prompt'])}"
                    + (f" ({t['cached'] / max(1, t['prompt']):.0%}c)" if t["cached"] else "")
                    + f" out {kfmt(t['completion'])}", style="dim", no_wrap=True, overflow="ellipsis")

    cx = [Text("Codex", style="bold magenta"), account_line("codex")]
    cu = cached("codex_usage", 60, codex_usage)
    if isinstance(cu, dict):
        for k, (pct, reset) in cu.items():
            t = Text(f" {k} ")
            t.append_text(bar(pct, 10))
            t.append(f" {pct:>3.0f}% ↻{dur(reset)}", style="dim" if pct < 70 else "bold")
            cx.append(t)
    else:
        cx.append(Text(f" n/a: {str(cu)[:40]}", style="dim"))
    cx.append(run("codex"))
    ds = [Text("DeepSeek", style="bold cyan")]
    db_ = cached("ds_balance", 120, deepseek_balance)
    if isinstance(db_, tuple):
        ds.append(Text(" balance " + ("  ".join(f"{amt} {cur}" for cur, amt in db_[0]) or "?"),
                       style="green" if db_[1] else "bold red"))
    else:
        ds.append(Text(f" n/a: {str(db_)[:30]}", style="dim"))
    ds.append(run("deepseek"))
    ds += efficiency(recent)
    g = Table.grid(expand=True, padding=(0, 2))
    g.add_column(ratio=1)
    g.add_column(ratio=1)
    g.add_row(Group(*cx), Group(*ds))
    return g


def efficiency(recent):
    """Quality/cost levers of this run in two short lines."""
    fns = [r for r in recent if not r.get("deferred")]
    if not fns:
        return []
    cx = [u for r in fns for u in r.get("usage", []) if u.get("backend") == "codex"]
    cout = sum(u.get("completion_tokens") or 0 for u in cx)
    ok = [r for r in fns if r.get("ok")]
    first = sum(r.get("attempts", 1) == 1 for r in ok)
    bat = [r for r in fns if r.get("batch")]
    t = Text(f" 1st try {first / max(1, len(ok)):.0%}", style="green" if first >= 0.75 * len(ok) else "yellow")
    t.append(f"  batched {len(bat) / len(fns):.0%}  codex out/fn {kfmt(cout // len(fns))}", style="dim")
    t.no_wrap, t.overflow = True, "ellipsis"
    return [t]


def grunt_snapshot():
    """One consistent read of grunt's live state + this run's finished functions."""
    st = cached("live", 1, lambda: json.load(open(LIVE)) if os.path.exists(LIVE) else None)
    st = st if isinstance(st, dict) else None
    alive = bool(st) and pid_alive(st.get("pid"))
    since = st.get("started", time.time()) if st else time.time() - 3600
    recent = cached("recent", 3, lambda: read_log_since(since))
    nm = cached("names", 30, names)
    calls = sorted((st or {}).get("calls", {}).values(), key=lambda x: x["since"]) if alive else []
    return dict(st=st, alive=alive, since=since, recent=recent if isinstance(recent, list) else [],
                names=nm if isinstance(nm, dict) else {}, calls=calls)


def grunt_header(g):
    st, alive, since = g["st"], g["alive"], g["since"]
    h = Text("grunt   ", style="bold magenta")
    if not st:
        h.append("no run yet", style="dim")
        return [h]
    c = st.get("counts", {})
    ph = st.get("phase", "")
    h.append("RUNNING " if alive else "stopped ", style="bold green" if alive else "bold red")
    h.append(ph, style="bold red" if "FAIL" in ph else "bold yellow" if ph in ("smoke", "stopping") or
             ph.startswith("captain") else "bold")
    end = time.time() if alive else st.get("updated", time.time())
    h.append(f"  up {ago(since)}" if alive else f"  ran {dur(end - since)}, ended {ago(end)} ago", style="dim")
    h.append(f"  {(c.get('ok', 0) + c.get('esc', 0)) / max(1, end - since) * 3600:.0f} fn/h", style="bold")
    t = Text("        ")
    t.append(f"ok {c.get('ok', 0)}", style="green")
    t.append(f"  esc {c.get('esc', 0)}", style="red" if c.get("esc") else "dim")
    if c.get("deferred"):
        t.append(f"  def {c['deferred']}", style="yellow")
    t.append(f"  committed {c.get('committed', 0)}", style="cyan")
    t.append(f"  next smoke {c.get('pending', 0)}/{c.get('batch', '-')}", style="dim")
    t.append(f"  run {c.get('submitted', 0)}/{c.get('total', '-')}", style="dim")
    out = [h, t]
    if g["calls"]:
        per = collections.Counter()
        for x in g["calls"]:
            per[x["backend"]] += x.get("n", 1) if x["kind"] != "review" else 0
        f = Text("        in flight ", style="bold")
        f.append("  ".join(f"{b} {n}" for b, n in per.items()), style="dim")
        out.append(f)
    if st.get("codex_paused") and alive:
        out.append(Text(f"        Codex paused by the quota guard (5h {st.get('codex_5h', 0):.0f}%): "
                        "DeepSeek lane only", style="bold red"))
    return out


def call_rows(g):
    """In-flight model calls, oldest first: (ea, backend, age, kind, name)."""
    rows = []
    for x in g["calls"]:
        n = x.get("n", 1)
        rows.append((f"{x['ea']:08x}" + (f"+{n - 1}" if n > 1 else ""),
                     Text(x["backend"], style=BACKEND_STYLE.get(x["backend"], "")),
                     Text(ago(x["since"]), style="red" if time.time() - x["since"] > 600 else "dim"),
                     Text(f"batch×{n}" if x["kind"] == "batch" else x["kind"], style="dim"),
                     g["names"].get(x["ea"], "") or ""))
    return rows


def result_rows(g, n=None):
    """Finished functions of the run, newest first: (age, result, by, tags, ea, name or error)."""
    rows = []
    for r in g["recent"][::-1][:n]:
        ok = r.get("ok")
        what = r.get("meta", {}).get("name", "") if ok else (r.get("err", "").splitlines() or [""])[0]
        by = (r.get("by") or "").split(":")[0] or {"deep": "deepseek", "main": "codex"}.get(r.get("lane"), "")
        tags = ([f"b{r['batch']}"] if r.get("batch") else []) + \
               ([f"try{r['attempts']}"] if ok and r.get("attempts", 1) > 1 else [])
        rows.append((Text(ago(r.get("ts", 0)), style="dim"),
                     Text("OK" if ok else "DEF" if r.get("deferred") else "ESC",
                          style="green" if ok else "yellow" if r.get("deferred") else "red"),
                     Text({"deepseek": "ds"}.get(by, by)[:5], style=BACKEND_STYLE.get(by, "dim")),
                     Text(" ".join(tags), style="dim"), f"{r['ea']:08x}",
                     Text(what, style="" if ok else "grey62")))
    return rows


def panel_work(n_results=5):
    """Rich version (--once): grunt, in flight, results, captain, tools, limits."""
    g = grunt_snapshot()
    body = grunt_header(g)
    for rows, cap in ((call_rows(g)[:6], 5), (result_rows(g, n_results) if n_results > 0 else [], 6)):
        if rows:
            tb = Table.grid(padding=(0, 1))
            for _ in range(len(rows[0])):
                tb.add_column(no_wrap=True, overflow="ellipsis")
            for r in rows:
                tb.add_row(*r)
            body.append(tb)
    cap = captain_lines()
    if cap:
        body += [Rule(style="grey30")] + cap
    body += [Rule(style="grey30"), tool_line(), limits_grid(g["st"], g["recent"])]
    return Panel(Group(*body), title="Work · grunt · captain · limits", border_style="magenta")


VIEW = {"plan": False, "focus": False, "help": False}
KEYS = {"p": ("plan", "open plan items of every phase"), "f": ("focus", "current focus (PLAN.md)"),
        "h": ("help", "this help")}


def focus():
    txt = open(os.path.join(ROOT, "docs", "PLAN.md")).read()
    m = re.search(r"## Current focus.*?\n(.*?)\n## ", txt, re.S)
    items = re.findall(r"^(\d+)\. (.+?)(?=\n\d+\. |\Z)", m.group(1).strip(), re.S | re.M) if m else []
    return [(n, re.sub(r"\s+", " ", t).replace("`", "").replace("**", "")) for n, t in items]


def open_items():
    txt = open(os.path.join(ROOT, "docs", "PLAN.md")).read()
    out = []
    for ph in re.split(r"\n(?=### )", txt):
        m = re.match(r"### (.+)", ph)
        if not m:
            continue
        its = re.findall(r"^- \[ \] (.+)$", ph, re.M)
        if its:
            out.append((m.group(1).split("—")[0].strip(), [i.replace("`", "") for i in its]))
    return out


def panel_plan():
    """Bottom: collapsed = the next open items of the current phase; `p` all open items, `f` current focus."""
    body = []
    if VIEW["focus"]:
        fo = cached("focus", 10, focus)
        for n, t in (fo if isinstance(fo, list) else []):
            body.append(Text(f"{n}. {t}", style="white", no_wrap=True, overflow="ellipsis"))
    if VIEW["plan"]:
        oi = cached("open_items", 10, open_items)
        for ph, its in (oi if isinstance(oi, list) else []):
            body.append(Text(ph, style="bold yellow"))
            body += [Text("  ☐ " + it, no_wrap=True, overflow="ellipsis") for it in its]
    if not body:
        p = cached("plan", 10, plan)
        if isinstance(p, tuple):
            name, d, n, open_ = p
            body.append(Text(f"{name.split('—')[0].strip()} ({d}/{n}) next:", style="bold"))
            body += [Text("  ☐ " + re.sub(r"`", "", it), no_wrap=True, overflow="ellipsis") for it in open_[:3]]
    hint = Text()
    for k, (name, _) in list(KEYS.items()) + [("q", ("quit", ""))]:
        hint.append(f" {k}", style="bold yellow")
        hint.append(f" {name}" + (" ✓" if VIEW.get(name) else "") + " ", style="bold" if VIEW.get(name) else "dim")
    return Panel(Group(*body), title="Plan", subtitle=hint, subtitle_align="right", border_style="yellow")


def render(console=None):
    """Everything sized to the terminal: the free rows go to the grunt results list."""
    clock = Text(datetime.now().strftime("%H:%M:%S") + "  csf-re monitor", style="dim", justify="right")
    top = Table.grid(expand=True)
    top.add_column(ratio=3)
    top.add_column(ratio=2, min_width=36)
    top.add_row(panel_claude(), panel_progress())
    plan_p = panel_plan()
    parts = [clock, top]
    if VIEW["help"]:
        parts.append(Panel(Group(*[Text(f"{k}  {v[1]}") for k, v in KEYS.items()], Text("q  quit")),
                           title="Keys", border_style="dim"))
    if console is None:
        return Group(*parts, panel_work(), plan_p)
    opts = console.options
    used = sum(len(console.render_lines(x, opts, pad=False)) for x in parts + [plan_p, panel_work(0)])
    free = console.size.height - used - 1
    return Group(*parts, panel_work(max(3, free)), plan_p)


def main():
    ap = argparse.ArgumentParser()
    ap.add_argument("--interval", type=float, default=2)
    ap.add_argument("--once", action="store_true", help="print one Rich frame and exit (no Textual)")
    a = ap.parse_args()
    if a.once:
        from rich.console import Console
        con = Console()
        con.print(render(con))
        return
    import monitor_app
    monitor_app.run(a.interval)

if __name__ == "__main__":
    try:
        main()
    except KeyboardInterrupt:
        pass
