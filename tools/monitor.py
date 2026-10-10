#!/usr/bin/env python3
"""Live terminal dashboard for the RE workflow (Textual app in tools/monitor_app.py; this module and
tools/monitor_loop.py hold the data readers and Rich renderables, and `--once` prints one Rich frame): the RE loop
(stage pipeline, now/next, stop rule, side loops, grunt lanes), game progress, agents (orchestrator, captain,
grunts, logins + billing, spend, forecast), modules, plan, commits, escalation queue.

  .venv/bin/python tools/monitor.py [--interval 2] [--once]
  CSF_MONITOR_ROOT=<main checkout>   read another checkout's state (work/, db/, docs/), e.g. from a worktree

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
from limits import codex_usage, codex_homes, codex_label  # noqa: E402
from grunt import CLAUDE_GRUNTS, claude_usage  # noqa: E402

from rich.console import Group
from rich.panel import Panel
from rich.rule import Rule
from rich.table import Table
from rich.text import Text

ROOT = os.environ.get("CSF_MONITOR_ROOT") or os.path.dirname(os.path.dirname(os.path.abspath(__file__)))  # data root
JOBS = os.path.expanduser("~/.claude/jobs")
LIVE = os.path.join(ROOT, "work", "grunt_state.json")
LOG = os.path.join(ROOT, "work", "grunt_log.jsonl")
CAPTAIN = os.path.join(ROOT, "work", "captain_state.json")
BACKEND_STYLE = {"codex": "magenta", "deepseek": "cyan", "claude": "yellow"}
MODEL_STYLE = {"haiku": "yellow", "mimo": "bright_cyan", "sol": "magenta", "ds": "cyan", "sonnet": "green", "opus": "bright_red"}


def who(backend, model=""):
    """Short model label: claude:claude-haiku-5-5@low -> haiku·lo, codex:gpt-6.1-sol@xhigh -> sol·xh."""
    base, _, eff = (model or "").partition("@")
    name = ("haiku" if "haiku" in base else "opus" if "opus" in base else "sonnet" if "sonnet" in base else "mimo" + ("-fl" if "flash" in base else "") if "mimo" in base else
            "sol" if "sol" in base else "ds" + ("-pro" if "pro" in base else "")
            if backend == "deepseek" or "deepseek" in base else base.split("-")[0] or backend)
    return name + (f"·{eff[:2]}" if eff else "")


def who_text(backend, model=""):
    w = who(backend, model)
    return Text(w, style=MODEL_STYLE.get(w.split("·")[0], BACKEND_STYLE.get(backend, "dim")))
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
    """Main checkout path for a path inside it or inside one of its worktrees (.claude/worktrees/*, work/*)."""
    return re.split(r"/(?:\.claude/worktrees|work)/", (p or "") + "/")[0].rstrip("/")


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
    Codex: the id_token claims (chatgpt_subscription_active_until = paid through). Claude (captain's ~/.claude.json and
    the grunts' CLAUDE_GRUNTS/.claude.json): oauthAccount; it has no renewal date, so the next monthly anniversary of subscriptionCreatedAt (estimate)."""
    import base64, datetime as dt
    out = {}
    for home in codex_homes():  # every Codex login the grunts rotate over (limits.codex_homes)
        try:
            tok = json.load(open(os.path.join(home, "auth.json")))["tokens"]["id_token"].split(".")[1]
            c = json.loads(base64.urlsafe_b64decode(tok + "=" * (-len(tok) % 4)))
            a = c.get("https://api.openai.com/auth", {})
            until = a.get("chatgpt_subscription_active_until")
            out[codex_label(home)] = (c.get("email", "?"), a.get("chatgpt_plan_type", "?"),
                                      dt.datetime.fromisoformat(until).date() if until else None, False)
        except Exception:
            pass
    for which, path in (("claude", os.path.expanduser("~/.claude.json")),
                        ("claude-grunts", os.path.join(CLAUDE_GRUNTS, ".claude.json"))):
        try:
            oa = json.load(open(path))["oauthAccount"]
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
            out[which] = (oa.get("emailAddress", "?"), plan, nxt, True)
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


PROC_PAT = re.compile(r"tools/|codex exec|smoke\.sh|CommXPC")


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

def panel_claude(lines=6):
    """Compact: state, plan usage + account, what it is doing, then recent events up to `lines` lines in all
    (the app passes the Costs panel's height so the two panels line up)."""
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
    ev = [(e, j) for j in jobs for e in timeline(j)[:-1]]  # plan usage + login: Costs panel
    ev.sort(key=lambda x: x[0]["at"], reverse=True)
    for e, j in ev[:max(2, lines - len(body))]:
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


# Areas: modules grouped by meaning, from their reviewed names (first matching prefix wins; a module's own
# `area:` in db/modules.yaml overrides). Unnamed modules stay "unnamed" until captain modules names them.
AREAS = [
    ("AI", ("ai_",)),
    ("characters", ("char_", "character_", "commando_", "weapon_", "vehicle")),
    ("world & scenes", ("scene_", "campaign", "dynamic_objects", "game_scene_", "cheats")),
    ("mission scripts", ("mission_script", "script_")),
    ("game data", ("bdd_", "game_data_", "game_bdd_", "object_class_", "text_database", "type_registry")),
    ("HUD", ("hud", "minimap", "interface")),
    ("menus", ("menu_",)),
    ("GUI widgets", ("gui_",)),
    ("graphics", ("gfx_", "fx_")),
    ("input & audio", ("input_", "audio")),
    ("multiplayer", ("net_", "mp_")),
    ("core & app", ("core_", "app_", "dev_console", "options_file", "profiles_", "game_config")),
]


def area_of(m):
    if m.get("area"):
        return m["area"]
    name = m.get("name") or ""
    if not name:
        return "unnamed"
    return next((a for a, pre in AREAS if name.startswith(pre)), "other")


def area_rows(S):
    """Per area: modules (started / with code), bytes, bytes done, functions done / verified in game / diffed."""
    out = {a: dict(area=a, mods=0, started=0, bytes=0, done_b=0, done=0, ok=0, diff=0, rev=0, rej=0)
           for a, _ in AREAS + [("other", ()), ("unnamed", ())]}
    for p_ in S["per"].values():
        if not p_["fns"]:
            continue
        r = out[area_of(p_["m"])]
        r["mods"] += 1
        r["started"] += p_["done"] > 0
        for k in ("bytes", "done_b", "done", "ok", "diff", "rev", "rej"):
            r[k] += p_[k]
    return [r for r in out.values() if r["mods"]]


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
    names = [m.get("name") or m["id"] for m in mods if m["id"] in ids]  # `wave --waves` passes ids: show names
    pre = os.path.commonprefix(names)
    label = (pre.rstrip("_") + "_*" if len(names) > 2 and len(pre) >= 3 else ",".join(names)[:40])
    return dict(label=label + (f" ({len(ids)} modules)" if len(ids) > 1 else ""), n=len(w), done=len(d),
                bytes=sum(int(r["size"]) for r in w), done_bytes=sum(int(r["size"]) for r in d))


PLACEHOLDER = re.compile(
    r"^(?:C_[0-9a-f]{8}_|[A-Z]\w*?_)?(Return(MinusOne|Zero|One|True|False|\d+)|Get(Returned)?Const(ant)?\w*|"
    r"Is\w*Field\w*Null|\w*Field_?0?[0-9A-Fa-f]{2,4}\w*|\w*At0x\w*|\w*Offset[0-9A-F]+\w*|\w*_[0-9A-F]{6}|"
    r"\w*(Vtbl|Slot)\d+\w*|\w*Unk\w*|[Ss]ub_[0-9A-F]+|Nullsub\w*)$")  # = captain.FN_PLACEHOLDER


def reviews():
    """db/reviews.jsonl (tools/review.py): ea -> ok of its latest Opus review."""
    out = {}
    try:
        for l in open(os.path.join(ROOT, "db", "reviews.jsonl")):
            try:
                r = json.loads(l)
            except ValueError:
                continue
            out[r["ea"]] = bool(r.get("ok"))
    except OSError:
        pass
    return out


def verifier():
    """tools/verifier.py: live state (work/verifier_state.json), its last runs (work/verifier_log.jsonl) and the
    pending count (done functions never installed in a passing smoke: verify.pending)."""
    try:
        st = json.load(open(os.path.join(ROOT, "work", "verifier_state.json")))
    except (OSError, ValueError):
        st = {}
    ev = []
    try:
        for l in open(os.path.join(ROOT, "work", "verifier_log.jsonl")):
            try:
                ev.append(json.loads(l))
            except ValueError:
                pass
    except OSError:
        pass
    try:
        import verify
        pend = len(verify.pending())
    except Exception:
        pend = None
    return dict(alive=pid_alive(st.get("pid")), phase=st.get("phase", ""), started=st.get("started"),
                events=ev, pending=pend)


def live_coverage(max_age=60):
    """What the running game executes, from the hook's csf_hook_hits.txt in the play copy (rewritten every few
    seconds): installed hooks hit, and how many of them had never run in any smoke before (ledger hits 0) — the
    coverage a playtest adds. None when the file is older than 60 s (no game running)."""
    from grunt import PLAY
    p = os.path.join(PLAY, "csf_hook_hits.txt")
    try:
        age = time.time() - os.path.getmtime(p)
    except OSError:
        return None
    if age > max_age:
        return None
    hits = {}
    for l in open(p, errors="replace"):
        f = l.split()
        if len(f) >= 2 and re.fullmatch(r"[0-9a-fA-F]{8}", f[0]):
            hits[f[0].lower()] = int(f[1])
    try:
        led = json.load(open(os.path.join(ROOT, "db", "verify.json")))
    except (OSError, ValueError):
        led = {}
    rows = cached("rows", 20, lambda: list(csv.DictReader(open(os.path.join(ROOT, "db", "functions.csv")))))
    rows = {r["ea"]: r for r in rows} if isinstance(rows, list) else {}
    mt = cached("modules", 60, module_table)
    mods = {m["id"]: m for m in mt} if isinstance(mt, list) else {}
    size = lambda ea: int(rows.get(ea, {}).get("size") or 0)  # noqa: E731
    auto = cached("auto_files", 30, lambda: {os.path.basename(f)[:8] for f in glob.glob(os.path.join(ROOT, "src", "auto", "*.cpp"))})
    auto = auto if isinstance(auto, set) else set()
    fresh = lambda ea: ea in auto and not (led.get(ea) or {}).get("hits")  # noqa: E731  (the ledger tracks src/auto)
    hit = [ea for ea, c in hits.items() if c > 0]
    new = [ea for ea in hit if fresh(ea)]
    per = collections.defaultdict(lambda: [0, 0, 0])  # area -> installed, hit, new
    for ea, c in hits.items():
        m = mods.get(rows.get(ea, {}).get("module"))
        a = area_of(m) if m else "other"
        per[a][0] += 1
        per[a][1] += c > 0
        per[a][2] += c > 0 and fresh(ea)
    newest = sorted(new, key=lambda ea: -hits[ea])[:4]
    return dict(age=age, installed=len(hits), hit=len(hit), hit_kb=sum(map(size, hit)) / 1024,
                new=len(new), new_kb=sum(map(size, new)) / 1024, per=dict(per),
                sample=[rows.get(ea, {}).get("name") or ea for ea in newest])


def coverage_line(L):
    t = Text("game      ", style="bold bright_green")
    t.append(f"hit {L['hit']:,}/{L['installed']:,} ({L['hit'] / max(1, L['installed']):.0%}, {L['hit_kb']:.0f} KB)  ")
    t.append(f"never run before: {L['new']:,} ({L['new_kb']:.1f} KB)", style="bold yellow" if L["new"] else "dim")
    if L["sample"]:
        t.append("  " + ", ".join(L["sample"]), style="dim")
    t.append(f"  {L['age']:.0f}s ago", style="dim")
    t.no_wrap, t.overflow = True, "ellipsis"
    return t


def coverage_table(L):
    tb = Table(box=None, padding=(0, 2), title="live game coverage (csf_hook_hits.txt)", title_justify="left")
    for c in ("area", "installed", "hit", "never run before"):
        tb.add_column(c, justify="left" if c == "area" else "right")
    for a, (n, h, nw) in sorted(L["per"].items(), key=lambda x: -x[1][1]):
        tb.add_row(a, f"{n:,}", f"{h:,}", Text(f"{nw:,}", style="bold yellow" if nw else "dim"))
    return tb


def verifier_line(V):
    t = Text("verifier  ", style="bold", no_wrap=True, overflow="ellipsis")
    if V["alive"]:
        t.append("RUNNING ", style="bold green")
        t.append(V["phase"] or "-", style="bold yellow")
        if V["started"]:
            t.append(f"  up {ago(V['started'])}", style="dim")
    else:
        t.append("idle", style="dim")
    if V["pending"] is not None:
        t.append(f"  pending {V['pending']}", style="bold yellow" if V["pending"] else "green")
    groups = [e for e in V["events"] if e.get("event") == "group"]
    if groups:
        last = groups[-1]["ts"]
        run = [e for e in groups if last - e["ts"] < 6 * 3600]  # the latest run's groups
        ver = sum(e.get("verified", 0) for e in run)
        q = sum(len(e.get("quarantined") or []) for e in run)
        t.append(f"  last run {ago(last)} ago: ", style="dim")
        t.append(f"{ver} verified", style="green")
        t.append(f", {q} quarantined", style="bold red" if q else "dim")
        tot_q = sum(len(e.get("quarantined") or []) for e in groups)
        t.append(f"  (all runs: {sum(e.get('verified', 0) for e in groups)} verified, {tot_q} quarantined)",
                 style="dim")
    return t


def review_line(G):
    t = Text("reviews   ", style="bold", no_wrap=True, overflow="ellipsis")
    if not G["rev"]:
        t.append("none yet (tools/review.py sample --module M)", style="dim")
        return t
    t.append(f"{G['rev']} sampled by Opus", style="cyan")
    t.append(f"  {G['rej']} rejected", style="bold red" if G["rej"] else "green")
    t.append(f"  defect rate {G['rej'] / G['rev'] * 100:.0f}%", style="bold")
    t.append(f"  ({G['rev'] / max(1, G['done']) * 100:.1f}% of done functions reviewed)", style="dim")
    return t


def area_table(S):
    """Areas: bytes done (bar), modules started, functions done / verified in game / diffed, review rejections."""
    tb = Table(box=None, padding=(0, 1), show_edge=False, header_style="dim")
    for c, j in (("area", "left"), ("bytes done", "left"), ("", "right"), ("KB", "right"), ("modules", "right"),
                 ("done", "right"), ("verified", "right"), ("diffed", "right"), ("reviewed", "right")):
        tb.add_column(c, justify=j, no_wrap=True)
    for r in area_rows(S):
        pct = r["done_b"] / max(1, r["bytes"]) * 100
        rv = Text(f"{r['rev']}", style="cyan" if r["rev"] else "dim")
        if r["rej"]:
            rv.append(f" ✗{r['rej']}", style="bold red")
        tb.add_row(Text(r["area"], style="dim italic" if r["area"] in ("unnamed", "other") else "bold"),
                   bar(pct, 16), Text(f"{pct:5.1f}%", style="bold green" if pct >= 25 else "green" if pct else "dim"),
                   Text(f"{r['done_b'] / 1024:.0f}/{r['bytes'] / 1024:.0f}", style="dim"),
                   Text(f"{r['started']}/{r['mods']}"), Text(str(r["done"])),
                   Text(str(r["ok"]), style="green" if r["ok"] else "dim"),
                   Text(str(r["diff"]), style="cyan" if r["diff"] else "dim"), rv)
    return tb


def stats():
    """Whole-game and per-module progress: functions (named / done / executed / diffed / escalated), modules
    (named / classes prepped / started / complete), classes (named / held / with fields), globals."""
    import yaml
    rows = list(csv.DictReader(open(os.path.join(ROOT, "db", "functions.csv"))))
    ver = json.load(open(os.path.join(ROOT, "db", "verify.json")))
    queue = {os.path.basename(f)[:8] for f in glob.glob(os.path.join(ROOT, "queue", "*.md"))}
    cls = json.load(open(os.path.join(ROOT, "db", "classes.json")))
    cy = yaml.safe_load(open(os.path.join(ROOT, "db", "types", "classes.yaml"))) or {}
    cy = {(f"{k:08x}" if isinstance(k, int) else str(k).lower()): v for k, v in cy.items() if isinstance(v, dict)}
    named_c = {vt for vt, v in cy.items() if (v.get("conf") or 0) >= 0.7}
    held_c = set()
    for f in glob.glob(os.path.join(ROOT, "work", "captain", "held_*.jsonl")):
        for l in open(f):
            try:
                vt = json.loads(l).get("vtable")
            except ValueError:
                continue
            if vt:
                held_c.add(str(vt).lower().replace("0x", "").zfill(8))
    held_c |= {vt for vt in cy if vt not in named_c}
    held_c -= named_c
    mods = yaml.safe_load(open(os.path.join(ROOT, "db", "modules.yaml")))
    gl = yaml.safe_load(open(os.path.join(ROOT, "db", "globals.yaml"))) or []
    rev = reviews()
    zero = dict(fns=0, bytes=0, named=0, ph=0, done=0, done_b=0, exe=0, ok=0, diff=0, esc=0, rev=0, rej=0)
    per = {m["id"]: dict(m=m, **zero) for m in mods}
    G = dict(zero)
    for r in rows:
        if r["region"] != "game":
            continue
        sz, ea = int(r["size"]), r["ea"]
        d = r["status"] in ("done", "verified")
        v = ver.get(ea, {})
        inc = dict(fns=1, bytes=sz, named=bool(r["name"]), ph=bool(r["name"] and PLACEHOLDER.match(r["name"])),
                   done=d, done_b=sz if d else 0, exe=d and bool(v.get("hits")), ok=d and bool(v.get("ok")),
                   diff=d and bool(v.get("diffed")), esc=ea in queue,
                   rev=ea in rev, rej=ea in rev and not rev[ea])
        for tgt in (G, per.get(r["module"])):
            if tgt is not None:
                for k, x in inc.items():
                    tgt[k] += x
    M_ = dict(n=0, named=0, prepped=0, started=0, complete=0)
    for p_ in per.values():
        m = p_["m"]
        vts = [c[2:] for c in m.get("classes", []) if re.fullmatch(r"C_[0-9a-f]{8}", c)]
        p_["classes"] = len(m.get("classes", []))
        p_["cls_open"] = sum(vt not in named_c and vt not in held_c for vt in vts)
        p_["cls_held"] = sum(vt in held_c for vt in vts)
        p_["prepped"] = p_["cls_open"] == 0
        if not p_["fns"]:
            continue
        M_["n"] += 1
        M_["named"] += bool(m.get("name"))
        M_["prepped"] += p_["prepped"] and bool(m.get("name"))
        M_["started"] += p_["done"] > 0
        M_["complete"] += p_["done_b"] >= 0.9 * p_["bytes"]
    C = dict(n=len(cls), named=len(named_c & {c["vtable"] for c in cls}), held=len(held_c),
             fields=sum(1 for vt in named_c if cy[vt].get("fields")))
    gnamed = [g for g in gl if not re.match(r"g_(Dword|Word|Byte|Unk|Data|Ptr|Float|Value|GlobalValue|vtbl)_?[0-9A-F]{6}",
                                            g.get("name", ""))]
    GL = dict(n=len(gl), named=len(gnamed), pinned=sum(g.get("src") in ("opus", "user", "captain") for g in gl))
    return dict(game=G, modules=M_, classes=C, globals=GL, per=per)


def frac(a, b, style="green"):
    t = Text(f"{a:,}", style="bold " + style)
    t.append(f"/{b:,}", style="dim")
    return t


def wave_line():
    """The running wave's progress bar, or None."""
    st = cached("live", 1, lambda: json.load(open(LIVE)) if os.path.exists(LIVE) else None)
    wp = wave_progress(((st or {}).get("config") or {}).get("wave")) if isinstance(st, dict) else None
    if wp:
        pct = wp["done_bytes"] / max(1, wp["bytes"]) * 100
        t = Text("wave      ", style="bold magenta")
        t.append_text(bar(pct, 20))
        t.append(f" {pct:.0f}%  {wp['done']}/{wp['n']} fns  {wp['label']}")
        t.no_wrap, t.overflow = True, "ellipsis"
        return t


def panel_progress(areas=True):
    """Game / functions / modules / classes / globals in one grid, plus the current wave; the per-area table
    unless areas=False (the app shows it in its own tab)."""
    S = cached("stats", 30, stats)
    body = []
    if isinstance(S, dict):
        G, Mo, C, GL = S["game"], S["modules"], S["classes"], S["globals"]
        pct = G["done_b"] / max(1, G["bytes"]) * 100
        t = Text("game      ", style="bold")
        t.append_text(bar(pct, 20))
        t.append(f" {pct:.2f}% of {G['bytes'] / 1024:,.0f} KB", style="bold green")
        body.append(t)
        w = wave_line()
        if w:
            body.append(w)
        tb = Table.grid(padding=(0, 2))
        for _ in range(5):
            tb.add_column(no_wrap=True)
        lab = lambda s: Text(s, style="bold")  # noqa: E731
        def kv(k, a, b=None, st="green"):
            t = Text()
            t.append(f"{k} ", style="dim")
            t.append(f"{a:,}", style="bold " + st)
            if b is not None:
                t.append(f"/{b:,}", style="dim")
            return t
        tb.add_row(lab("functions"), kv("named", G["named"] - G["ph"], G["fns"]), kv("done", G["done"], G["fns"]),
                   kv("verified", G["ok"], G["done"]), kv("diffed", G["diff"], G["done"], "cyan"))
        tb.add_row("", kv("placeholders", G["ph"], st="yellow"), kv("escalated", G["esc"], st="red"),
                   kv("executed", G["exe"], G["done"], "cyan"), "")
        tb.add_row(lab("modules"), kv("named", Mo["named"], Mo["n"]), kv("prepped", Mo["prepped"], Mo["n"]),
                   kv("started", Mo["started"], Mo["n"]), kv("≥90% done", Mo["complete"], Mo["n"]))
        tb.add_row(lab("classes"), kv("named", C["named"], C["n"]), kv("held", C["held"], st="yellow"),
                   kv("with fields", C["fields"], C["named"]), "")
        tb.add_row(lab("globals"), kv("named", GL["named"], GL["n"]), kv("pinned", GL["pinned"], GL["n"]), "", "")
        body.append(tb)
        V = cached("verifier", 15, verifier)
        if isinstance(V, dict):
            body.append(verifier_line(V))
        body.append(review_line(G))
        L = cached("coverage", 3, live_coverage)
        if isinstance(L, dict):
            body.append(coverage_line(L))
        if areas:
            body += [Rule(style="dim"), area_table(S)]
    return Panel(Group(*body), title="Progress", border_style="yellow")


def module_rows():
    """Modules tab: one row per module with functions, its classes' prep state and done/verified progress."""
    S = cached("stats", 30, stats)
    if not isinstance(S, dict):
        return []
    out = []
    for mid, p_ in S["per"].items():
        if not p_["fns"]:
            continue
        m = p_["m"]
        pct = p_["done_b"] / max(1, p_["bytes"]) * 100
        out.append(dict(id=mid, area=area_of(m), name=m.get("name") or "", src=str(m.get("name_src") or ""),
                        fns=p_["fns"], ok=p_["ok"], rev=p_["rev"], rej=p_["rej"],
                        kb=p_["bytes"] / 1024, named=p_["named"] - p_["ph"], ph=p_["ph"], classes=p_["classes"],
                        cls_open=p_["cls_open"], cls_held=p_["cls_held"], prepped=p_["prepped"],
                        done=p_["done"], pct=pct, exe=p_["exe"], diff=p_["diff"], esc=p_["esc"]))
    return out


def captain_costs():
    """Captain spend from work/captain_log.jsonl: today and all time, per model."""
    tot, today, per = 0.0, 0.0, collections.Counter()
    t0 = datetime.now().replace(hour=0, minute=0, second=0).timestamp()
    if os.path.exists(os.path.join(ROOT, "work", "captain_log.jsonl")):
        for l in open(os.path.join(ROOT, "work", "captain_log.jsonl")):
            try:
                r = json.loads(l)
            except ValueError:
                continue
            u = r.get("usage") or {}
            c = u.get("cost_usd") or 0
            tot += c
            per[u.get("model", "?")] += c
            if r.get("ts", 0) >= t0:
                today += c
    return tot, today, per


def panel_costs(st=None, recent=()):
    """Every subscription / balance in one place, two lines per account (login, plan, billing date; 5h and 7d
    windows on one row), the grunts' account first; then run spend."""
    def usage_row(u):
        if not isinstance(u, dict):
            return Text(f" usage n/a: {str(u)[:44]}", style="dim")
        t = Text()
        for k, v in u.items():
            if k == "_age":
                continue
            pct, reset = v
            t.append(f" {k} ")
            t.append_text(bar(pct, 8))
            t.append(f" {pct:>3.0f}% {dur(max(0, reset))} ", style="dim" if pct < 70 else "bold")
        if u.get("_age", 0) > 900:
            t.append(f"({dur(u['_age'])} old)", style="dim")
        t.no_wrap, t.overflow = True, "ellipsis"
        return t

    def account(label, style, which, usage):
        t = Text(f"{label} ", style=style)
        t.append_text(account_line(which))
        t.no_wrap, t.overflow = True, "ellipsis"
        return [t, usage_row(usage)]

    body = account("grunts", "bold yellow", "claude-grunts", cached("claude_grunts_usage", 30, lambda: claude_usage("grunts")))
    body += account("Claude", "bold green", "claude", cached("claude_usage", 30, claude_usage))
    cc = cached("captain_costs", 30, captain_costs)
    if isinstance(cc, tuple):
        tot, today, per = cc
        body.append(Text(f" captain (claude -p) today ${today:.2f}  total ${tot:.2f}  "
                         + " ".join(f"{m} ${c:.2f}" for m, c in per.most_common()), style="dim",
                         no_wrap=True, overflow="ellipsis"))
    for home in codex_homes():
        lab = codex_label(home)
        body += account("Codex" if lab == "codex" else lab, "bold magenta", lab,
                        cached(f"usage_{lab}", 60, lambda h=home: codex_usage(h)))
    tot = (st or {}).get("tokens") or {}
    if tot:
        t = Text(" grunt run: ", style="dim")
        t.append("  ".join(f"{b} {v['calls']} calls in {kfmt(v['prompt'])} out {kfmt(v['completion'])}"
                           for b, v in sorted(tot.items())), style="dim")
        t.no_wrap, t.overflow = True, "ellipsis"
        body.append(t)
    body += efficiency(list(recent))
    return Panel(Group(*body), title="Costs · usage · logins", border_style="cyan")



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


def efficiency(recent):
    """Quality/cost levers of this run: per tier (one-lane runs) pass rate, first-try rate, $ per function and which
    model solved them; older runs: one line."""
    fns = [r for r in recent if not r.get("deferred")]
    if not fns:
        return []
    return efficiency_lines(fns) + fail_kinds_line(fns)


def fail_kinds_line(fns):
    """What this run's retries were spent on: gate.fail_kinds of every rejected attempt (grunt log `fails`)."""
    kinds = collections.Counter(k for r in fns for f in r.get("fails") or [] for k in f.get("kinds") or ["other"])
    if not kinds:
        return []
    t = Text(f" retries {sum(len(r.get('fails') or []) for r in fns)}: ", style="bold")
    for k, n in kinds.most_common(5):
        t.append(f"{k} {n}  ", style="yellow" if n >= 3 else "dim")
    t.no_wrap, t.overflow = True, "ellipsis"
    return [t]


def efficiency_lines(fns):
    if any(r.get("tier") for r in fns):
        out = []
        for tier in ("S", "M", "L", "X"):
            ts = [r for r in fns if r.get("tier") == tier]
            if not ts:
                continue
            ok = [r for r in ts if r.get("ok")]
            first = sum(r.get("attempts", 1) == 1 for r in ok)
            cl = sum(u.get("completion_tokens") or 0 for r in ts for u in r.get("usage", []) if u.get("backend") == "claude")
            cx = sum(u.get("completion_tokens") or 0 for r in ts for u in r.get("usage", []) if u.get("backend") == "codex")
            wins = collections.Counter(who(*(r.get("by") or ":").split(":", 1)) for r in ok)
            t = Text(f" {tier} {len(ok)}/{len(ts)} ", style="bold")
            t.append(f"1st {first / max(1, len(ok)):.0%}", style="green" if first >= 0.75 * len(ok) else "yellow")
            t.append(f"  claude out {kfmt(cl // max(1, len(ts)))}/fn  codex out {kfmt(cx // max(1, len(ts)))}/fn  ", style="dim")
            for w, n in wins.most_common(4):
                t.append(f"{w} {n} ", style=MODEL_STYLE.get(w.split("·")[0], "dim"))
            t.no_wrap, t.overflow = True, "ellipsis"
            out.append(t)
        return out
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
    t.append(f"  next commit {c.get('pending', 0)}/{c.get('batch', '-')}", style="dim")
    t.append(f"  run {c.get('submitted', 0)}/{c.get('total', '-')}", style="dim")
    out = [h, t]
    if g["calls"]:
        per = collections.Counter()
        for x in g["calls"]:
            per[who(x["backend"], f"{x.get('model', '')}@{x.get('effort', '')}" if x.get("effort") else
                    x.get("model", ""))] += x.get("n", 1)
        f = Text("        in flight ", style="bold")
        tiers = st.get("inflight_tiers") or {}
        if tiers:
            f.append("  ".join(f"{t} {tiers.get(t, 0)}" for t in ("S", "M", "L", "X")) + "  │  ", style="dim")
        for w, n in per.most_common():
            f.append(f"{w} {n}  ", style=MODEL_STYLE.get(w.split("·")[0], "dim"))
        out.append(f)
    sub = ((st.get("config") or {}).get("codex_sub") or "").split(":", 1)
    sub = who(*sub) if len(sub) == 2 else None
    if st.get("codex_paused") and alive:
        out.append(Text(f"        Codex paused by the quota guard (5h {st.get('codex_5h', 0):.0f}%): L tier waits, "
                        + (f"S/M Codex rungs on {sub}" if sub else "S/M without the Codex rung"), style="bold red"))
    elif st.get("codex_reserved") and alive and sub:
        out.append(Text(f"        Codex 5h {st.get('codex_5h', 0):.0f}% past the reserve: kept for L, S/M Codex rungs "
                        f"on {sub}", style="yellow"))
    if st.get("claude_paused") and alive:
        out.append(Text(f"        grunts' Claude account paused by the quota guard ({st.get('claude_pct', 0):.0f}%): "
                        "its rungs are skipped, functions deferred", style="bold red"))
    return out


def call_rows(g):
    """In-flight model calls, oldest first: (ea, backend, age, kind, name)."""
    rows = []
    for x in g["calls"]:
        n = x.get("n", 1)
        rows.append((f"{x['ea']:08x}" + (f"+{n - 1}" if n > 1 else ""),
                     who_text(x["backend"], f"{x.get('model', '')}@{x['effort']}" if x.get("effort") else
                              x.get("model", "")),
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
        backend, _, model = (r.get("by") or "").partition(":")
        backend = backend or {"flash": "deepseek", "deep": "codex"}.get(r.get("lane"), "")
        tags = ([r["tier"]] if r.get("tier") else []) + ([f"b{r['batch']}"] if r.get("batch") else []) + \
               ([f"try{r['attempts']}"] if ok and r.get("attempts", 1) > 1 else [])
        rows.append((Text(ago(r.get("ts", 0)), style="dim"),
                     Text("OK" if ok else "DEF" if r.get("deferred") else "ESC",
                          style="green" if ok else "yellow" if r.get("deferred") else "red"),
                     who_text(backend, model) if backend else Text(""),
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
    body += [Rule(style="grey30"), tool_line()]
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
    return Panel(Group(*body), title="Plan", border_style="yellow")


def render(console=None):
    """Rich frame (--once): Loop (pipeline, wave | side loops, grunt), Game, Agents, Plan."""
    import monitor_loop as ML
    clock = Text(datetime.now().strftime("%H:%M:%S") + "  csf-re monitor", style="dim", justify="right")
    L = ML.loop_state()
    g = L["g"]
    mid = Table.grid(expand=True)
    mid.add_column(ratio=1)
    mid.add_column(ratio=1)
    mid.add_row(Panel(ML.wave_panel(L), title="Wave · stop rule · history", border_style="yellow"),
                Panel(ML.side_panel(L), title="Side loops", border_style="cyan"))
    top = Table.grid(expand=True)
    top.add_column(ratio=3)
    top.add_column(ratio=2, min_width=44)
    top.add_row(Panel(ML.orchestrator_view(), title="Orchestrator", border_style="green"), panel_costs(g["st"], g["recent"]))
    return Group(clock, Panel(ML.pipeline(L)[0], title="RE loop", border_style="bright_yellow"), mid, panel_work(12),
                 Panel(ML.game_view(), title="Game", border_style="yellow"), top, ML.spend_table(),
                 Panel(ML.forecast(), title="Forecast", border_style="yellow"), panel_plan())


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
