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

def claude_job():
    best = None
    for p in glob.glob(os.path.join(JOBS, "*", "state.json")):
        try:
            d = json.load(open(p))
        except (OSError, ValueError):
            continue
        if d.get("cwd") != ROOT:
            continue
        d["_dir"] = os.path.dirname(p)
        if best is None or d.get("updatedAt", "") > best.get("updatedAt", ""):
            best = d
    return best


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


def panel_claude():
    job = cached("job", 1, claude_job)
    if not job or isinstance(job, Exception):
        return Panel("no Claude job for this repo", title="Claude", border_style="dim")
    state = job.get("state", "?")
    style = {"working": "bold green", "waiting": "bold yellow", "blocked": "bold red", "done": "dim"}.get(state, "bold")
    head = Text()
    head.append(f"{state.upper()}", style=style)
    head.append(f"  {job.get('tempo', '')}  ", style="dim")
    head.append(f"tokens {kfmt(job.get('tokens') or 0)}  ", style="cyan")
    head.append(f"up {ago(iso(job['createdAt']))}  upd {ago(iso(job['updatedAt']))} ago", style="dim")
    body = [head, Text(job.get("detail") or "", style="bold white", overflow="ellipsis", no_wrap=True)]
    tl = timeline(job)
    for e in tl[:-1][::-1][:5]:
        t = Text(f"  {ago(iso(e['at'])):>6} ", style="dim")
        t.append(e["detail"].splitlines()[0][:200], style="grey62")
        t.no_wrap, t.overflow = True, "ellipsis"
        body.append(t)
    fan = job.get("fan") or []
    if fan:
        tb = Table.grid(padding=(0, 1))
        tb.add_column(style="cyan", no_wrap=True, min_width=9)
        tb.add_column(style="dim", no_wrap=True, justify="right", min_width=6)
        tb.add_column(no_wrap=True, overflow="ellipsis", ratio=1)
        for s in fan:
            tb.add_row(s.get("id", "?"), ago(s.get("startedAt", 0) / 1000), s.get("label", "").replace("\n", " "))
        body += [Text(f"background shells ({len(fan)})", style="bold"), tb]
    return Panel(Group(*body), title=f"Claude · {job.get('name', '')}", border_style="green")


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
        done = c.get("ok", 0) + c.get("esc", 0)
        el = max(1, time.time() - since)
        line = Text()
        line.append(f"ok {c.get('ok', 0)}", style="green")
        line.append(f"  esc {c.get('esc', 0)}", style="red")
        line.append(f"  pending {c.get('pending', 0)}/{c.get('batch', '-')}", style="yellow")
        line.append(f"  committed {c.get('committed', 0)}", style="cyan")
        line.append(f"  submitted {c.get('submitted', 0)}/{c.get('total', '-')}", style="dim")
        line.append(f"  {done / el * 3600:.0f} fn/h", style="bold")
        body.append(line)
        calls = sorted((st.get("calls") or {}).values(), key=lambda x: x["since"])
        if calls and alive:
            tb = Table(box=None, padding=(0, 1), show_header=True, header_style="dim")
            for col, kw in (("ea", {}), ("backend", {}), ("eff", {}), ("kind", {}), ("age", {"justify": "right"}),
                            ("name", {"overflow": "ellipsis", "no_wrap": True})):
                tb.add_column(col, **kw)
            for x in calls:
                b = x["backend"]
                tb.add_row(f"{x['ea']:08x}", Text(b, style=BACKEND_STYLE.get(b, "")), x["effort"], x["kind"],
                           ago(x["since"]), nm.get(x["ea"], "") or "")
            body.append(Text(f"in flight ({len(calls)})", style="bold"))
            body.append(tb)
    recent = cached("recent", 3, lambda: read_log_since(since))
    recent = recent if isinstance(recent, list) else []
    if recent:
        tb = Table.grid(padding=(0, 1))
        for w in (6, 4, 8, 3):
            tb.add_column(no_wrap=True, min_width=w)
        tb.add_column(no_wrap=True, overflow="ellipsis", ratio=1)
        for r in recent[-10:][::-1]:
            ok = r.get("ok")
            what = r.get("meta", {}).get("name", "") if ok else (r.get("err", "").splitlines() or [""])[0]
            tb.add_row(Text(ago(r.get("ts", 0)), style="dim"), Text(r.get("lane", "") or "", style="dim"),
                       f"{r['ea']:08x}", Text("OK " if ok else "ESC", style="green" if ok else "red"),
                       Text(what, style="" if ok else "grey62"))
        body.append(Text("recent results", style="bold"))
        body.append(tb)
    return Panel(Group(*body), title="Grunts", border_style="magenta"), (st if isinstance(st, dict) else None), recent


def codex_usage():
    """5h / 7d windows of the ChatGPT subscription, from the endpoint the Codex CLI's /status uses (own token)."""
    a = json.load(open(os.path.expanduser("~/.codex/auth.json")))["tokens"]
    req = urllib.request.Request("https://chatgpt.com/backend-api/wham/usage", headers={
        "Authorization": f"Bearer {a['access_token']}", "ChatGPT-Account-Id": a.get("account_id") or "",
        "User-Agent": "codex_cli_rs"})
    d = json.load(urllib.request.urlopen(req, timeout=8))["rate_limit"]
    return {k: (d[w]["used_percent"], d[w]["reset_after_seconds"]) for k, w in
            (("5h", "primary_window"), ("7d", "secondary_window")) if d.get(w)}


def deepseek_balance():
    key = next(l.split("=", 1)[1].strip() for l in open(os.path.expanduser("~/.config/csf-re/env"))
               if l.startswith("DEEPSEEK_API_KEY="))
    req = urllib.request.Request("https://api.deepseek.com/user/balance",
                                 headers={"Authorization": f"Bearer {key}", "Accept": "application/json"})
    d = json.load(urllib.request.urlopen(req, timeout=8))
    return [(b["currency"], b["total_balance"]) for b in d.get("balance_infos", [])], d.get("is_available")


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
        body.append(Text(f" in  {kfmt(t['prompt'])}" + (f" ({kfmt(t['cached'])} cached)" if t["cached"] else "")))
        body.append(Text(f" out {kfmt(t['completion'])}  {kfmt(rate[b] // 10)}/min"))
    if not tot:
        body.append(Text("no calls yet", style="dim"))
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
    head.append(f"   codex exec: {codex}", style="magenta" if codex else "dim")
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
    job = cached("job", 1, claude_job)
    since = iso(job["createdAt"]) if isinstance(job, dict) else time.time() - 86400
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
