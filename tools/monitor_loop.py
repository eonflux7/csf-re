#!/usr/bin/env python3
"""Readers and Rich renderables for the monitor's first three tabs (tools/monitor_app.py):

  Loop    where the RE loop is (tools/grunt.py wave: types -> produce -> verify -> review -> report -> next wave),
          what runs now and what comes next, the stop rule live, past waves, the side loops (verifier, diff tiers
          1-3, smoke pool, tools)
  Game    whole-game progress: functions / modules / classes / globals, verification and diff tiers, linktest,
          pace and ETA, per-area table, live coverage
  Agents  orchestrator sessions (Claude jobs: tokens, API-equivalent $), captain, grunts per model, logins and
          billing, and the forecast: what finishing the game costs at the measured rates

The loop stage is read from the processes (ps: grunt.py wave/lanes, verifier.py, review.py, linktest.py, captain.py,
ida/propagate.sh) and the phase grunt writes to work/grunt_state.json; nothing here starts or stops anything.
API-equivalent $ uses list prices (PRICES; Claude 5.5 family, cache write assumed 1.25x input). Codex runs on a
ChatGPT plan: tokens and window % only. Subscription forecast: the monitor samples every account's 5h/7d usage into
work/usage_history.jsonl (every 10 min) next to the game KB done, so "% of the weekly window per KB" is measured;
monthly plan prices from ~/.config/csf-re/billing.yaml ({account label: USD per month}) turn it into $.
"""
import collections, glob, json, math, os, re, subprocess, time
from datetime import datetime

from rich import box
from rich.console import Group
from rich.panel import Panel
from rich.table import Table
from rich.text import Text

import monitor as M

# ---------------------------------------------------------------- processes -> loop stage

STAGES = [
    ("types", "Types", "captain names classes, then the module; field consensus, rename, IDB sync"),
    ("produce", "Produce", "grunt lanes S/M/L/X, gate, link, commit pending"),
    ("verify", "Verify", "verifier in game: baseline + pending groups, diff"),
    ("review", "Review", "Opus reads ~5% against the original"),
    ("report", "Report", "linktest, wave report, stop rule"),
    ("between", "Next wave", "regression smoke, next module by rank"),
]
STOP_DEFAULTS = {"--stop-quarantine": 0.03, "--stop-review": 0.10, "--stop-pending": 0.25}


def tool_procs():
    """Running repo tools: [dict(tool, argv, et, pid)] (tool = path under tools/ without extension)."""
    out = subprocess.run(["ps", "-eo", "pid,etimes,args", "--no-headers"], capture_output=True, text=True).stdout
    rows = []
    for l in out.splitlines():
        pid, et, args = l.strip().split(None, 2)
        if args.startswith(("/bin/bash -c", "bash -c", "/bin/sh -c", "sh -c")):
            continue
        m = re.search(r"tools/((?:ida/)?\w+)\.(?:py|sh)\b\s*(.*)", args)
        if m and not m.group(1).startswith("monitor"):
            rows.append(dict(tool=m.group(1), argv=m.group(2).split(), et=int(et), pid=int(pid)))
    return rows


def _opt(argv, name, default=None):
    for i, a in enumerate(argv):
        if a == name and i + 1 < len(argv):
            return argv[i + 1]
        if a.startswith(name + "="):
            return a.split("=", 1)[1]
    return default


def ranked(n=8):
    import grunt
    return grunt.rank_modules(grunt.DB(), n)


def mod_names():
    mt = M.cached("modules", 60, M.module_table)
    return {m["id"]: m.get("name") or m["id"] for m in mt} if isinstance(mt, list) else {}


def wave_ids(spec):
    mt = M.cached("modules", 60, M.module_table)
    keys = [k.strip() for k in (spec or "").split(",") if k.strip()]
    return {m["id"] for m in mt if m["id"] in keys or m.get("name") in keys} if isinstance(mt, list) else set()


def loop_state():
    """Which stage of the RE loop runs now, from the processes and grunt's live phase."""
    P = M.cached("tool_procs", 2, tool_procs)
    P = P if isinstance(P, list) else []
    find = lambda tool, pred=lambda p: True: [p for p in P if p["tool"] == tool and pred(p)]  # noqa: E731
    wave_cmd = lambda p: p["argv"][:1] == ["wave"]  # noqa: E731
    parent = next(iter(find("grunt", lambda p: wave_cmd(p) and "--waves" in " ".join(p["argv"]))), None)
    wave = next(iter(sorted(find("grunt", lambda p: wave_cmd(p) and "--waves" not in " ".join(p["argv"])
                                 and "--next" not in p["argv"] and len(p["argv"]) > 1), key=lambda p: p["et"])), None)
    lanes = next(iter(find("grunt", lambda p: p["argv"][:1] == ["lanes"])), None)
    ver = next(iter(find("verifier")), None)
    rev = next(iter(find("review")), None)
    g = M.grunt_snapshot()
    st, alive = g["st"] or {}, g["alive"]
    phase = st.get("phase", "") if alive else ""
    producing = alive and phase not in ("finished", "stopped", "")
    spec = (wave["argv"][1] if wave else None) or ((st.get("config") or {}).get("wave") if producing else None) \
        or (_opt(ver["argv"], "--module") if ver else None)
    mods = wave_ids(spec)
    stage, mode = None, "idle"
    if wave:
        mode = "waves" if parent else "wave"
        if rev:
            stage = "review"
        elif find("linktest"):
            stage = "report"
        elif phase.startswith(("captain: naming", "ida:")):
            stage = "types"
        elif phase == "captain: placeholder names":
            stage = "report"
        elif producing:
            stage = "produce"
        elif ver:
            stage = "verify"
        else:
            stage = "produce" if not st.get("counts") else "report"
    elif parent:
        mode, stage = "waves", "between"
    elif producing or lanes:
        mode, stage = "lanes", "produce"
    elif ver:
        mode, stage = "verifier", "verify"
    elif rev:
        mode, stage = "review", "review"
    thresholds = {k: float(_opt((wave or parent or {"argv": []})["argv"], k, v)) for k, v in STOP_DEFAULTS.items()}
    n_waves = int(_opt(parent["argv"], "--waves", "0")) if parent else 0
    return dict(P=P, find=find, parent=parent, wave=wave, lanes=lanes, ver=ver, rev=rev, g=g, phase=phase,
                stage=stage, mode=mode, mods=mods, spec=spec, thresholds=thresholds,
                n_waves=n_waves, t0=time.time() - (wave or parent or lanes or {"et": 0})["et"])


def jsonl(p):
    out = []
    try:
        for l in open(p):
            try:
                out.append(json.loads(l))
            except ValueError:
                pass
    except OSError:
        pass
    return out


def module_sums(S, mods):
    z = collections.Counter()
    for m in mods:
        p_ = S["per"].get(m) if isinstance(S, dict) else None
        if p_:
            for k in ("fns", "bytes", "done", "done_b", "ok", "rev", "rej", "classes", "cls_open", "cls_held", "esc"):
                z[k] += p_[k]
    return z


def stop_gauges(L, Z):
    """The wave's stop rule, live: quarantined / in-game outcomes since the wave started, review rejects, pending."""
    v = q = 0
    for r in jsonl(os.path.join(M.ROOT, "work", "verifier_log.jsonl")):
        if r.get("ts", 0) >= L["t0"] and r.get("event") in ("group", "baseline"):
            v += r.get("verified", 0)
            q += len(r.get("quarantined") or [])
    th = L["thresholds"]
    pend = Z["done"] - Z["ok"]
    return [("quarantined", q, v + q, th["--stop-quarantine"], q >= 2),
            ("review rejects", Z["rej"], Z["rev"], th["--stop-review"], Z["rej"] >= 2),
            # judged at wave end (wave_report, after the verify step): mid-production nearly all are pending
            ("unverified", pend, Z["done"], th["--stop-pending"], Z["done"] >= 10 and
             L["stage"] in ("review", "report", "between"))]


def gauge_text(name, a, b, lim, armed):
    r = a / b if b else 0
    over = armed and r > lim
    t = Text(f"{name:<15}", style="bold")
    t.append_text(M.bar(min(100, r / max(lim, 1e-9) * 100), 10))
    t.append(f" {a}/{b} = {r:.0%}", style="bold red" if over else "green" if a == 0 else "yellow")
    t.append(f"  limit {lim:.0%}" + ("  STOP" if over else ""), style="bold red" if over else "dim")
    return t


# ---------------------------------------------------------------- Loop tab renderables

def _stage_details(L, S, Z, rank):
    """Per stage: a few short lines of numbers."""
    g, st = L["g"], (L["g"]["st"] or {})
    c = st.get("counts", {})
    D = {k: [] for k, *_ in STAGES}
    cap = M.cached("captain", 1, lambda: json.load(open(M.CAPTAIN)) if os.path.exists(M.CAPTAIN) else None)
    if L["stage"] == "types" and isinstance(cap, dict) and M.pid_alive(cap.get("pid")):
        cc = cap.get("counts", {})
        D["types"].append(Text(f"captain {cap.get('job', '')} {cc.get('done', 0)}/{cc.get('total', 0)}", style="cyan"))
    if L["phase"].startswith("ida:"):
        D["types"].append(Text("IDB sync + re-decompile", style="cyan"))
    if L["mods"]:
        nm = mod_names()
        named = [nm[m] for m in sorted(L["mods"]) if nm.get(m, m) != m]
        unnamed = len(L["mods"]) - len(named)
        D["types"].append(Text(("module " + ",".join(named))[:30] if not unnamed else
                               f"module unnamed ({unnamed})", style="green" if not unnamed else "yellow",
                               no_wrap=True, overflow="ellipsis"))
    if Z["classes"]:
        D["types"].append(Text(f"classes {Z['classes'] - Z['cls_open']}/{Z['classes']} named"
                               + (f", {Z['cls_held']} held" if Z["cls_held"] else ""),
                               style="green" if not Z["cls_open"] else "yellow"))
    if Z["fns"]:
        pct = Z["done_b"] / max(1, Z["bytes"]) * 100
        t = M.bar(pct, 10)
        t.append(f" {pct:.0f}%", style="bold")
        D["produce"] += [t, Text(f"{Z['done']}/{Z['fns']} fns  {Z['bytes'] / 1024:.0f} KB")]
    if L["stage"] == "produce" and g["alive"]:
        rate = (c.get("ok", 0) + c.get("esc", 0)) / max(1, time.time() - g["since"]) * 3600
        t = Text(f"{rate:.0f} fn/h  ", style="bold")
        t.append(f"ok {c.get('ok', 0)} ", style="green")
        t.append(f"esc {c.get('esc', 0)}", style="red" if c.get("esc") else "dim")
        D["produce"].append(t)
        left = max(0, Z["fns"] - Z["done"] - Z["esc"])
        if rate > 1 and left:
            D["produce"].append(Text(f"{len(g['calls'])} in flight  ETA {M.dur(left / rate * 3600)}", style="dim"))
    V = M.cached("verifier", 15, M.verifier)
    if Z["done"]:
        D["verify"].append(Text(f"verified {Z['ok']}/{Z['done']}", style="green" if Z["ok"] == Z["done"] else "yellow"))
    if isinstance(V, dict) and V["alive"]:
        D["verify"].append(Text(V["phase"] or "smoking", style="bold cyan", no_wrap=True, overflow="ellipsis"))
    if isinstance(V, dict) and V["pending"] is not None:
        D["verify"].append(Text(f"pending (all) {V['pending']}", style="yellow" if V["pending"] else "dim"))
    if Z["done"]:
        want = max(1, round(Z["done"] * 0.05))
        D["review"].append(Text(f"reviewed {Z['rev']} (~{want} wanted)", style="cyan" if Z["rev"] else "dim"))
        D["review"].append(Text(f"rejected {Z['rej']}", style="bold red" if Z["rej"] else "green"))
    for name, a, b, lim, armed in (stop_gauges(L, Z) if L["mods"] else []):
        r = a / b if b else 0
        D["report"].append(Text(f"{name.split()[-1][:10]:<10} {a}/{b} {r:4.0%} /{lim:.0%}",
                                style="bold red" if armed and r > lim else "green" if not a else "yellow"))
    if L["parent"]:
        done_w = sum(1 for w in jsonl(os.path.join(M.ROOT, "work", "waves.jsonl")) if w.get("ts", 0) >= L["t0"])
        D["between"].append(Text(f"wave {done_w + (1 if L['wave'] else 0)}"
                                 + (f"/{L['n_waves']}" if L["n_waves"] > 0 else " (until stop)"), style="bold"))
    if L["stage"] == "between" and L["ver"]:
        D["between"].append(Text("regression smoke (full)", style="bold cyan"))
    nxt = [r for r in rank if r[1] not in L["mods"]]
    if nxt:
        D["between"].append(Text(f"next: {nxt[0][2]}", no_wrap=True, overflow="ellipsis"))
        D["between"].append(Text(f"{nxt[0][3] / 1024:.0f} KB todo", style="dim"))
    return D


def _now_next(L, Z, rank):
    """(now, next) sentences."""
    names = mod_names()
    label = ",".join(names.get(m, m) for m in sorted(L["mods"]))[:60] or (L["spec"] or "")
    nxt_mod = next((r for r in rank if r[1] not in L["mods"]), None)
    nxt_wave = f"wave {nxt_mod[2]} ({nxt_mod[1]}, {nxt_mod[3] / 1024:.0f} KB todo)" if nxt_mod else "ranking exhausted"
    pend = Z["done"] - Z["ok"]
    s, ph = L["stage"], L["phase"]
    if s is None:
        last = jsonl(os.path.join(M.ROOT, "work", "waves.jsonl"))[-1:]
        now = "Loop idle." + (f" Last wave {', '.join(names.get(m, m) for m in last[0]['modules'])} ended "
                               f"{M.ago(last[0]['ts'])} ago" + (f" (STOP: {'; '.join(last[0]['stop'])})"
                                                                 if last[0].get("stop") else "") if last else "")
        return now, (f"start: tools/grunt.py wave {nxt_mod[1]}   ({nxt_mod[2]}; or wave --waves -1)"
                     if nxt_mod else "nothing ranked")
    total_left = max(0, Z["fns"] - Z["done"] - Z["esc"])
    if s == "types":
        now = f"Types for {label}: " + ("captain names the module from its named classes"
                                        if ph == "captain: naming modules" else
                                        "captain names the module's classes (agentic Opus session)"
                                        if ph.startswith("captain") else "IDB sync + re-decompile of open functions")
        nxt = f"produce: grunt lanes over {total_left} open functions of {label}"
    elif s == "produce":
        now = f"Producing {label}: grunt lanes ({ph or 'starting'}), {Z['done']}/{Z['fns']} functions done"
        nxt = (f"verify: verifier.py --module {L['spec']} (baseline, then the module's pending functions in game)"
               if L["mode"] in ("wave", "waves") else "lanes end at --total; then run verifier.py")
    elif s == "verify":
        V = M.cached("verifier", 15, M.verifier)
        vph = V.get("phase") if isinstance(V, dict) and V.get("alive") else ""
        now = f"Verifying {label} in game: {vph or 'starting'} ({pend} of the module pending)"
        nxt = (f"review: Opus samples ~{max(1, round(Z['done'] * 0.05))} of {Z['done']} done functions"
               if L["mode"] in ("wave", "waves") else "verifier ends when no pending group is left")
    elif s == "review":
        now = f"Reviewing {label}: Opus reads a ~5% sample against the original"
        nxt = "report: linktest + wave report; stop rule decides whether the loop goes on"
    elif s == "report":
        now = f"Wave end for {label}: " + ("captain names placeholder functions" if ph.startswith("captain")
                                           else "linktest + wave report")
        nxt = (f"{nxt_wave}" + (" after a regression smoke" if L["parent"] and "--regress-every" in
                                " ".join(L["parent"]["argv"]) else "")) if L["parent"] else "loop ends (single wave)"
    else:
        now = "Between waves: " + ("regression smoke of the verified baseline (full coverage)" if L["ver"]
                                   else "picking the next module by rank")
        nxt = nxt_wave
    return now, nxt


def pipeline(L=None):
    """The loop as boxes with arrows; the running stage highlighted; NOW / NEXT / QUEUE below."""
    L = L or loop_state()
    S = M.cached("stats", 30, M.stats)
    Z = module_sums(S, L["mods"])
    rk = M.cached("rank", 120, ranked)
    rk = rk if isinstance(rk, list) else []
    D = _stage_details(L, S, Z, rk)
    keys = [k for k, *_ in STAGES]
    cur = keys.index(L["stage"]) if L["stage"] in keys else -1
    tb = Table.grid(expand=True)
    for i in range(len(STAGES)):
        if i:
            tb.add_column(width=2, vertical="middle")
        tb.add_column(ratio=1, min_width=18)
    cells = []
    for i, (k, title, what) in enumerate(STAGES):
        if cur < 0:
            st = "idle"
        elif k == "between":
            st = "active" if i == cur else "todo" if L["parent"] else "off"
        else:
            st = "active" if i == cur else "done" if i < cur and cur != keys.index("between") else "todo"
            if cur == keys.index("between"):
                st = "done"
        style = {"active": "bold yellow", "done": "green", "todo": "grey50", "idle": "grey42", "off": "grey23"}[st]
        icon = {"active": "▶", "done": "✓", "todo": "○", "idle": "·", "off": "·"}[st]
        if i:
            cells.append(Text("━▶" if i == cur else "─▶", style="bold yellow" if i == cur else "grey50"))
        body = [Text(what, style="italic grey62" if st != "active" else "italic white")] + D[k]
        cells.append(Panel(Group(*body), title=f"{icon} {i + 1 if k != 'between' else '↻'} {title}",
                           title_align="left", border_style=style, height=8,
                           box=box.HEAVY if st == "active" else box.ROUNDED, padding=(0, 1)))
    tb.add_row(*cells)
    now, nxt = _now_next(L, Z, rk)
    out = [tb]
    t = Text(" NOW  ", style="bold black on yellow" if L["stage"] else "bold black on grey50")
    t.append(" " + now, style="bold")
    out.append(t)
    t = Text(" NEXT ", style="bold black on cyan")
    t.append(" " + nxt)
    out.append(t)
    q = [r for r in rk if r[1] not in L["mods"]][1 if L["parent"] or L["stage"] is None else 0:][:5]
    if q:
        t = Text(" THEN ", style="bold black on grey62")
        t.append(" " + "  ›  ".join(f"{r[2]} ({r[3] / 1024:.0f} KB)" for r in q), style="dim")
        t.no_wrap, t.overflow = True, "ellipsis"
        out.append(t)
    return Group(*out), now


def wave_panel(L=None):
    """The stop rule (live) and the waves so far (work/waves.jsonl)."""
    L = L or loop_state()
    S = M.cached("stats", 30, M.stats)
    Z = module_sums(S, L["mods"])
    body = []
    if L["mods"]:
        body.append(Text("stop rule (this wave)", style="bold"))
        body += [gauge_text(*x) for x in stop_gauges(L, Z)]
    names = mod_names()
    W = jsonl(os.path.join(M.ROOT, "work", "waves.jsonl"))[-8:][::-1]
    if W:
        tb = Table(box=None, padding=(0, 1), header_style="dim", show_edge=False, title="past waves",
                   title_justify="left", title_style="bold")
        for c in ("ended", "module", "time", "done", "inst.", "quar.", "never ran", "pend.", "stop"):
            tb.add_column(c, no_wrap=True, justify="left" if c in ("module", "stop") else "right")
        for w in W:
            tb.add_row(M.ago(w["ts"]), ",".join(names.get(m, m) for m in w["modules"])[:24], M.dur(w.get("secs", 0)),
                       str(w.get("done", 0)), Text(str(w.get("installed", w.get("verified", 0))), style="green"),
                       Text(str(w.get("quarantined", 0)), style="red" if w.get("quarantined") else "dim"),
                       str(w.get("never_ran", "")),
                       Text(str(w.get("pending", 0)), style="yellow" if w.get("pending") else "dim"),
                       Text("; ".join(w.get("stop") or []) or "—", style="bold red" if w.get("stop") else "dim"))
        body.append(tb)
    if not body:
        body.append(Text("no wave yet (tools/grunt.py wave <module>)", style="dim"))
    return Group(*body)


# ---------------------------------------------------------------- side loops: diff tiers, pool, game instances

def _table_eas(path, block=None):
    try:
        txt = open(path).read()
    except OSError:
        return None
    if block:
        m = re.search(rf"#ifdef {block}\n(.*?)#endif", txt, re.S)
        txt = m.group(1) if m else ""
    return {m.group(1).lower() for m in re.finditer(r"^\s*\{0x([0-9A-Fa-f]{8})", txt, re.M)}


def diff_tiers():
    """Diff coverage per tier: functions in the generated tables (verify tree's build, else build/) and how many of
    them the ledger has diffed; the tier-3 target = executed but never diffed."""
    out = {}
    for d in (os.path.join(M.ROOT, "work", "verify-tree", "build"), os.path.join(M.ROOT, "build")):
        t12 = _table_eas(os.path.join(d, "diff_table.inc"))
        t3 = _table_eas(os.path.join(d, "replay_table.inc"), "CSF_T3_FUNCS")
        if t12 or t3:
            out.update(t12=t12 or set(), t3=t3 or set(), src=os.path.relpath(d, M.ROOT))
            break
    try:
        led = json.load(open(os.path.join(M.ROOT, "db", "verify.json")))
    except (OSError, ValueError):
        led = {}
    diffed = {ea for ea, e in led.items() if e.get("diffed")}
    out["diffed"] = len(diffed)
    out["mism"] = sum(1 for e in led.values() if e.get("mismatched"))
    out["exec_nodiff"] = sum(1 for e in led.values() if e.get("hits") and not e.get("diffed"))
    for k in ("t12", "t3"):
        if k in out:
            out[k + "_diffed"] = len(out[k] & diffed)
    try:
        from grunt import PLAY
        txt = open(os.path.join(PLAY, "csf_hook.log"), errors="replace").read()
        out["t3_inconclusive"] = len(set(re.findall(r"^T3 ([0-9A-F]{8}) .*inconclusive", txt, re.M)))
    except Exception:
        pass
    return out


def diff_line(D):
    t = Text("diff      ", style="bold", no_wrap=True, overflow="ellipsis")
    if not isinstance(D, dict):
        t.append("n/a", style="dim")
        return t
    t.append(f"diffed {D['diffed']}", style="bold cyan")
    if "t12" in D:
        t.append(f"  tier 1-2 table {D['t12_diffed']}/{len(D['t12'])}", style="cyan")
        t.append(f"  tier 3 replay {D.get('t3_diffed', 0)}/{len(D.get('t3', ()))}",
                 style="bold magenta" if D.get("t3") else "dim")
        if D.get("t3_inconclusive"):
            t.append(f" ({D['t3_inconclusive']} inconclusive)", style="yellow")
    t.append(f"  executed, not diffed {D['exec_nodiff']}", style="yellow")
    t.append(f"  mismatched {D['mism']}", style="bold red" if D["mism"] else "dim")
    return t


def games():
    """Every running CommXPC.exe (the smoke pool runs several): [(pid, mem)]."""
    r = subprocess.run(["tasklist.exe", "/FI", "IMAGENAME eq CommXPC.exe", "/FO", "CSV", "/NH"],
                       capture_output=True, text=True, timeout=5).stdout
    return re.findall(r'"CommXPC\.exe","(\d+)".*?"([\d,.\s]+K)"', r)


def pool_line():
    t = Text("pool      ", style="bold", no_wrap=True, overflow="ellipsis")
    try:
        import pool
        cps = pool.copies()
    except Exception:
        cps = []
    G = M.cached("games", 10, games)
    G = G if isinstance(G, list) else []
    t.append(f"{len(cps)} play copies", style="cyan")
    t.append(f"  games running {len(G)}", style="bold green" if G else "dim")
    if G:
        t.append("  " + "  ".join(m.replace(" ", "") for _, m in G[:8]), style="dim")
    return t


def side_panel(L=None):
    """The loops beside production: verifier, diff tiers, smoke pool, orchestrator, tools, captain."""
    L = L or loop_state()
    body = []
    V = M.cached("verifier", 15, M.verifier)
    if isinstance(V, dict):
        body.append(M.verifier_line(V))
    body.append(diff_line(M.cached("diff_tiers", 30, diff_tiers)))
    body.append(pool_line())
    cov = M.cached("coverage", 3, M.live_coverage)
    if isinstance(cov, dict):
        body.append(M.coverage_line(cov))
    jobs = M.cached("jobs", 1, M.claude_jobs)
    if isinstance(jobs, list):
        for j in jobs[:3]:
            t = Text("orch.     ", style="bold green", no_wrap=True, overflow="ellipsis")
            st = j.get("state", "?")
            t.append(f"{st:<8}", style=M.STATE_STYLE.get(st, "bold"))
            t.append((j.get("detail") or j.get("name") or "").replace("\n", " "), style="white")
            body.append(t)
    other = [p for p in L["P"] if p["tool"] not in ("grunt", "verifier", "review", "captain")]
    if other:
        t = Text("tools     ", style="bold", no_wrap=True, overflow="ellipsis")
        for p in other[:6]:
            t.append(f"{p['tool']} {' '.join(p['argv'][:2])[:18]} {M.ago(time.time() - p['et'])}  ", style="cyan")
        body.append(t)
    body += M.captain_lines()
    return Group(*body)


# ---------------------------------------------------------------- Game tab

def tier_counts():
    """verify.py tiers: {tier: (functions, bytes)} over done functions."""
    import verify
    t = verify.tiers()
    rows = M.cached("rows", 20, lambda: list(__import__("csv").DictReader(open(os.path.join(M.ROOT, "db", "functions.csv")))))
    size = {r["ea"]: int(r["size"]) for r in rows} if isinstance(rows, list) else {}
    out = collections.defaultdict(lambda: [0, 0])
    for ea, v in t.items():
        out[v][0] += 1
        out[v][1] += size.get(ea, 0)
    return dict(out)


def pace():
    """Game KB produced per day (grunt log, ok results; last 7 days) -> ETA."""
    now = time.time()
    rows = [r for r in M.read_log_since(now - 7 * 86400) if r.get("ok")]
    if not rows:
        return None
    span = max(1.0, (now - min(r["ts"] for r in rows)) / 86400)
    kb = sum(r.get("size") or 0 for r in rows) / 1024
    day = sum(r.get("size") or 0 for r in rows if r["ts"] >= now - 86400) / 1024
    return dict(kb=kb, days=span, per_day=kb / span, today=day, fns=len(rows))


def linktest_line():
    t = Text("linktest  ", style="bold", no_wrap=True, overflow="ellipsis")
    try:
        lt = json.load(open(os.path.join(M.ROOT, "work", "linktest.json")))
    except (OSError, ValueError):
        t.append("n/a (tools/linktest.py)", style="dim")
        return t
    un = lt.get("unresolved_functions") or {}
    t.append(f"{lt.get('compiled', 0)}/{lt.get('files', 0)} files compile", style="green")
    t.append(f"  failed {len(lt.get('failed') or {})}", style="bold red" if lt.get("failed") else "dim")
    for k, v in un.items():
        t.append(f"  unresolved {k} {len(v)}", style="yellow" if k == "game" else "dim")
    return t


def game_view():
    S = M.cached("stats", 30, M.stats)
    if not isinstance(S, dict):
        return Text(f"stats n/a: {S}", style="dim")
    body = [M.panel_progress(areas=False).renderable]
    G = S["game"]
    TC = M.cached("tiers", 60, tier_counts)
    if isinstance(TC, dict):
        t = Text("evidence  ", style="bold", no_wrap=True, overflow="ellipsis")
        for k, st in (("diffed", "bold cyan"), ("executed", "cyan"), ("gate", "yellow"), ("stale", "dim"),
                      ("mismatched", "bold red")):
            n, b = TC.get(k, (0, 0))
            t.append(f"{k} {n} ({b / 1024:.0f} KB)  ", style=st if n else "dim")
        body.append(t)
    body.append(diff_line(M.cached("diff_tiers", 30, diff_tiers)))
    body.append(linktest_line())
    pc = M.cached("pace", 60, pace)
    left = (G["bytes"] - G["done_b"]) / 1024
    t = Text("pace      ", style="bold", no_wrap=True, overflow="ellipsis")
    if isinstance(pc, dict) and pc["per_day"] > 0:
        t.append(f"{pc['per_day']:.0f} KB/day", style="bold green")
        t.append(f" (last {pc['days']:.1f} d: {pc['kb']:.0f} KB, {pc['fns']} fns; today {pc['today']:.0f} KB)", style="dim")
        t.append(f"  {left:,.0f} KB left: ~{left / pc['per_day']:.0f} days", style="bold")
        q = G["bytes"] / 1024 * 0.25 - G["done_b"] / 1024
        if q > 0:
            t.append(f"  25% playtest mark in ~{q / pc['per_day']:.0f} days", style="yellow")
    else:
        t.append("no finished functions in the last 7 days", style="dim")
    body.append(t)
    body += [Text(""), Text("areas", style="bold"), M.area_table(S)]
    L = M.cached("coverage", 3, M.live_coverage)
    if isinstance(L, dict):
        body += [Text(""), M.coverage_table(L)]
    return Group(*body)


# ---------------------------------------------------------------- Agents tab: spend, forecast

# $ per million tokens: input, output, cache read, cache write (list prices; cache write = 1.25x input, assumed)
PRICES = {"opus": (4.0, 20.0, 0.20, 5.0), "sonnet": (2.0, 10.0, 0.20, 2.5), "haiku": (0.10, 0.50, 0.01, 0.125)}


def price_of(model):
    m = (model or "").lower()
    return next((v for k, v in PRICES.items() if k in m), None)


def usd(model, inp, out, cr=0, cw=0):
    p = price_of(model)
    return None if p is None else (inp * p[0] + out * p[1] + cr * p[2] + cw * p[3]) / 1e6


_tally = {}  # transcript path -> dict(off, seen, rows)


def transcript_usage(path):
    """Usage of one Claude Code transcript, read incrementally: [(ts, model, in, out, cache_read, cache_write)],
    one row per assistant message (the transcript repeats usage on every content block of a message)."""
    T = _tally.setdefault(path, dict(off=0, seen=set(), rows=[]))
    try:
        with open(path, "rb") as f:
            f.seek(T["off"])
            data = f.read()
    except OSError:
        return T["rows"]
    cut = data.rfind(b"\n") + 1
    T["off"] += cut
    for l in data[:cut].decode(errors="replace").splitlines():
        if '"usage"' not in l:
            continue
        try:
            e = json.loads(l)
        except ValueError:
            continue
        msg = e.get("message") or {}
        u, mid = msg.get("usage"), msg.get("id")
        if e.get("type") != "assistant" or not u or mid in T["seen"]:
            continue
        T["seen"].add(mid)
        T["rows"].append((M.iso(e["timestamp"]) if e.get("timestamp") else 0, msg.get("model", ""),
                          u.get("input_tokens") or 0, u.get("output_tokens") or 0,
                          u.get("cache_read_input_tokens") or 0, u.get("cache_creation_input_tokens") or 0))
    return T["rows"]


def all_jobs():
    """Every Claude job of this repo (done ones too): the orchestrator's sessions."""
    out = []
    for p in glob.glob(os.path.join(M.JOBS, "*", "state.json")):
        try:
            d = json.load(open(p))
        except (OSError, ValueError):
            continue
        if M.repo_root(d.get("originCwd") or d.get("cwd")) == M.repo_root(M.ROOT):
            d["_dir"] = os.path.dirname(p)
            out.append(d)
    return out


def orchestrator_spend():
    """Per job and in total: tokens and API-equivalent $ today / last 7 days / all time."""
    now, t_day = time.time(), datetime.now().replace(hour=0, minute=0, second=0).timestamp()
    tot = {k: collections.Counter() for k in ("today", "7d", "all")}
    per = {}
    for j in all_jobs():
        p = j.get("linkScanPath")
        if not p:
            continue
        c = collections.Counter()
        for ts, model, i, o, cr, cw in transcript_usage(p):
            cost = usd(model, i, o, cr, cw) or 0
            row = dict(inp=i + cr + cw, out=o, usd=cost, msgs=1)
            for k, ok in (("today", ts >= t_day), ("7d", ts >= now - 7 * 86400), ("all", True)):
                if ok:
                    tot[k].update(row)
            c.update(row)
        per[j["_dir"]] = c
    return tot, per


def grunt_spend():
    """Grunt model calls (work/grunt_log.jsonl) per backend:model, today / 7 days / all: calls, tokens, $.
    $: the logged cost where the backend reports one (go), else API-equivalent for Claude models, None for Codex."""
    now, t_day = time.time(), datetime.now().replace(hour=0, minute=0, second=0).timestamp()
    out = collections.defaultdict(lambda: {k: collections.Counter() for k in ("today", "7d", "all")})
    kb = {k: 0.0 for k in ("today", "7d", "all")}
    for r in M.read_log_since(0):
        ts = r.get("ts", 0)
        wins = [k for k, ok in (("today", ts >= t_day), ("7d", ts >= now - 7 * 86400), ("all", True)) if ok]
        if r.get("ok"):
            for k in wins:
                kb[k] += (r.get("size") or 0) / 1024
        for u in r.get("usage") or []:
            be = u.get("backend") or ("deepseek" if "prompt_cache_hit_tokens" in u else
                                      {"flash": "deepseek", "deep": "codex"}.get(r.get("lane"), "?"))
            model = u.get("model") or ""
            p, o, ca = u.get("prompt_tokens") or 0, u.get("completion_tokens") or 0, u.get("cached_tokens") or 0
            cw = u.get("cache_write_tokens") or 0
            cost = u.get("cost")
            if cost is None:
                cost = usd(model, max(0, p - ca), o, ca, cw)
            row = dict(calls=1, inp=p, out=o, cached=ca)
            if cost is not None:
                row["usd"] = cost
            for k in wins:
                out[M.who(be, model) + f" ({be})"][k].update(row)
    return dict(out), kb


def spend_table():
    """One row per agent role / model: today, 7 days, all time (tokens in/out, $)."""
    tb = Table(box=box.SIMPLE_HEAD, padding=(0, 1), header_style="bold", title="spend (API-equivalent $ for "
               "Claude, logged $ where billed; Codex: tokens only)", title_justify="left", title_style="bold")
    for c in ("agent", "model", "calls 7d", "in 7d", "out 7d", "$ today", "$ 7d", "$ all"):
        tb.add_column(c, justify="left" if c in ("agent", "model") else "right", no_wrap=True)
    f = lambda c, k: f"${c[k]['usd']:.2f}" if "usd" in c[k] else "—"  # noqa: E731
    O = M.cached("orch_spend", 30, orchestrator_spend)
    if isinstance(O, tuple):
        tot = O[0]
        tb.add_row(Text("orchestrator", style="bold green"), "Claude jobs", str(tot["7d"]["msgs"]),
                   M.kfmt(tot["7d"]["inp"]), M.kfmt(tot["7d"]["out"]), f"${tot['today']['usd']:.2f}",
                   f"${tot['7d']['usd']:.2f}", f"${tot['all']['usd']:.2f}")
    cc = M.cached("captain_costs", 30, M.captain_costs)
    if isinstance(cc, tuple):
        all_, today, per = cc
        R7 = [r for r in jsonl(os.path.join(M.ROOT, "work", "captain_log.jsonl")) if r.get("ts", 0) >= time.time() - 7 * 86400]
        u7 = [r.get("usage") or {} for r in R7]
        tb.add_row(Text("captain", style="bold bright_blue"), " ".join(per)[:24] or "—", str(len(R7)),
                   M.kfmt(sum((u.get("input") or 0) + (u.get("cache_read") or 0) + (u.get("cache_write") or 0) for u in u7)),
                   M.kfmt(sum(u.get("output") or 0 for u in u7)), f"${today:.2f}",
                   f"${sum(u.get('cost_usd') or 0 for u in u7):.2f}", f"${all_:.2f}")
    GS = M.cached("grunt_spend", 60, grunt_spend)
    if isinstance(GS, tuple):
        for name, c in sorted(GS[0].items(), key=lambda x: -x[1]["7d"]["calls"]):
            if not c["all"]["calls"]:
                continue
            tb.add_row(Text("grunts", style="bold magenta"),
                       Text(name, style=M.MODEL_STYLE.get(name.split("·")[0].split(" ")[0], "dim")),
                       str(c["7d"]["calls"]), M.kfmt(c["7d"]["inp"]), M.kfmt(c["7d"]["out"]), f(c, "today"),
                       f(c, "7d"), f(c, "all"))
    return tb


# ---- usage history -> subscription forecast

USAGE_HIST = os.path.join(M.ROOT, "work", "usage_history.jsonl")
BILLING = os.path.expanduser("~/.config/csf-re/billing.yaml")


def account_usages():
    """{label: {"5h": (pct, secs to reset), "7d": ...}} of every subscription (cached reads, as panel_costs)."""
    out = {"claude": M.cached("claude_usage", 30, M.claude_usage),
           "claude-grunts": M.cached("claude_grunts_usage", 30, lambda: M.claude_usage("grunts"))}
    for home in M.codex_homes():
        lab = M.codex_label(home)
        out[lab] = M.cached(f"usage_{lab}", 60, lambda h=home: M.codex_usage(h))
    return {k: v for k, v in out.items() if isinstance(v, dict) and "7d" in v}


def sample_usage(done_kb):
    """Append {ts, kb, acct: {label: [5h %, 7d %, 7d reset ts]}} at most every 10 min."""
    last = 0
    try:
        with open(USAGE_HIST, "rb") as f:
            f.seek(0, 2)
            f.seek(max(0, f.tell() - 4000))
            tail = f.read().decode(errors="replace").splitlines()
        last = json.loads(tail[-1])["ts"] if tail else 0
    except (OSError, ValueError, KeyError, IndexError):
        pass
    if time.time() - last < 600:
        return
    U = account_usages()
    if not U:
        return
    now = time.time()
    rec = dict(ts=now, kb=round(done_kb, 2),
               acct={k: [v.get("5h", (0, 0))[0], v["7d"][0], round(now + v["7d"][1])] for k, v in U.items()
                     if not v.get("_age", 0) > 1800})
    os.makedirs(os.path.dirname(USAGE_HIST), exist_ok=True)
    with open(USAGE_HIST, "a") as f:
        f.write(json.dumps(rec) + "\n")


def burn_rates(days=14):
    """Per account: % of its weekly window per game KB, from consecutive samples inside the same 7d window."""
    H = [h for h in jsonl(USAGE_HIST) if h.get("ts", 0) >= time.time() - days * 86400]
    acc = collections.defaultdict(lambda: [0.0, 0.0])  # label -> d% , dKB
    for a, b in zip(H, H[1:]):
        dkb = b["kb"] - a["kb"]
        if dkb < 0:
            continue
        for k, (_, p7b, rb) in b.get("acct", {}).items():
            if k not in a.get("acct", {}):
                continue
            _, p7a, ra = a["acct"][k]
            if abs(rb - ra) > 3600 or p7b < p7a:  # window reset in between
                continue
            acc[k][0] += p7b - p7a
            acc[k][1] += dkb
    return {k: (dp / dkb if dkb >= 5 else None, dkb) for k, (dp, dkb) in acc.items()}


def billing_prices():
    try:
        import yaml
        d = yaml.safe_load(open(BILLING)) or {}
        return {str(k): float(v) for k, v in d.items()}
    except Exception:
        return {}


def forecast():
    """What finishing the game costs at the measured rates: KB left, days at the current pace, tokens and $ per KB
    (7 days) scaled to the rest, and the subscription view (weekly windows needed per account)."""
    S = M.cached("stats", 30, M.stats)
    if not isinstance(S, dict):
        return Text("stats n/a", style="dim")
    G = S["game"]
    left = (G["bytes"] - G["done_b"]) / 1024
    try:
        sample_usage(G["done_b"] / 1024)
    except Exception:
        pass
    body = []
    pc = M.cached("pace", 60, pace)
    GS = M.cached("grunt_spend", 60, grunt_spend)
    O = M.cached("orch_spend", 30, orchestrator_spend)
    cc = M.cached("captain_costs", 30, M.captain_costs)
    t = Text("game left ", style="bold")
    t.append(f"{left:,.0f} KB of {G['bytes'] / 1024:,.0f} KB", style="bold yellow")
    t.append(f"  ({G['fns'] - G['done']:,} functions)", style="dim")
    if isinstance(pc, dict) and pc["per_day"] > 0:
        t.append(f"  at {pc['per_day']:.0f} KB/day: ~{left / pc['per_day']:.0f} days", style="bold")
    body.append(t)
    wave = None
    L = M.cached("loop", 2, loop_state)
    if isinstance(L, dict) and L["mods"]:
        Z = module_sums(S, L["mods"])
        wave = (Z["bytes"] - Z["done_b"]) / 1024
    if isinstance(GS, tuple) and GS[1]["7d"] > 1:
        kb7 = GS[1]["7d"]
        tb = Table(box=box.SIMPLE_HEAD, padding=(0, 1), header_style="bold", title=f"per KB (last 7 days: "
                   f"{kb7:.0f} KB done) -> rest of the game", title_justify="left", title_style="bold")
        for c in ("model", "tokens/KB", "$/KB", "this wave", "rest: tokens", "rest: $"):
            tb.add_column(c, justify="left" if c == "model" else "right", no_wrap=True)
        tot_usd = 0.0
        for name, c in sorted(GS[0].items(), key=lambda x: -x[1]["7d"]["calls"]):
            w = c["7d"]
            if not w["calls"]:
                continue
            tpk = (w["inp"] + w["out"]) / kb7
            upk = w.get("usd", 0) / kb7 if "usd" in w else None
            tot_usd += (upk or 0) * left
            tb.add_row(name, M.kfmt(int(tpk)), f"${upk:.3f}" if upk is not None else "—",
                       (f"${upk * wave:.2f}" if upk is not None else M.kfmt(int(tpk * wave))) if wave else "",
                       M.kfmt(int(tpk * left)), f"${upk * left:,.0f}" if upk is not None else "—")
        other = []
        if isinstance(O, tuple) and O[0]["7d"]["usd"]:
            upk = O[0]["7d"]["usd"] / kb7
            other.append(("orchestrator", upk))
        if isinstance(cc, tuple):
            t7 = sum(r.get("usage", {}).get("cost_usd") or 0 for r in jsonl(os.path.join(M.ROOT, "work", "captain_log.jsonl"))
                     if r.get("ts", 0) >= time.time() - 7 * 86400)
            if t7:
                other.append(("captain", t7 / kb7))
        for name, upk in other:
            tot_usd += upk * left
            tb.add_row(Text(name, style="bold"), "", f"${upk:.3f}", f"${upk * wave:.2f}" if wave else "", "",
                       f"${upk * left:,.0f}")
        body.append(tb)
        t = Text("API-equivalent to finish ", style="bold")
        t.append(f"~${tot_usd:,.0f}", style="bold yellow")
        t.append("  (what the Claude + billed calls would cost at list prices; Codex on its plan not included)",
                 style="dim")
        body.append(t)
    else:
        body.append(Text("no grunt output in the last 7 days: no per-KB rates yet", style="dim"))
    # subscription windows
    R = burn_rates()
    U = account_usages()
    prices = billing_prices()
    tb = Table(box=box.SIMPLE_HEAD, padding=(0, 1), header_style="bold", title="subscriptions: weekly window per KB "
               "(work/usage_history.jsonl, last 14 days)", title_justify="left", title_style="bold")
    for c in ("account", "7d now", "% of week / KB", "KB per week", "weeks for the rest", "$/month"):
        tb.add_column(c, justify="left" if c == "account" else "right", no_wrap=True)
    weeks_max, months_usd = 0.0, 0.0
    for k in sorted(set(U) | set(R)):
        pk, dkb = R.get(k, (None, 0))
        now7 = U.get(k, {}).get("7d", (None,))[0]
        price = prices.get(k)
        if price:
            months_usd += price
        if pk:
            weeks = pk * left / 100
            weeks_max = max(weeks_max, weeks)
        tb.add_row(k, f"{now7:.0f}%" if now7 is not None else "?",
                   f"{pk:.3f}" if pk else Text(f"measuring ({dkb:.0f} KB seen)", style="dim"),
                   f"{100 / pk:,.0f}" if pk else "", Text(f"{pk * left / 100:.1f}", style="bold") if pk else "",
                   f"${price:.0f}" if price else "—")
    body.append(tb)
    if weeks_max:
        months = weeks_max * 7 / 30.4
        t = Text("subscriptions to finish ", style="bold")
        t.append(f"~{weeks_max:.1f} weeks of the busiest account's window (~{months:.1f} months)", style="bold yellow")
        if months_usd:
            t.append(f"  ≈ ${months * months_usd:,.0f} in plan fees at ${months_usd:.0f}/month", style="bold")
        body.append(t)
    if not prices:
        body.append(Text(f"plan prices: add {BILLING} as {{account: USD per month}} "
                         f"(accounts: {', '.join(sorted(U)) or '?'}) to turn weeks into $", style="dim"))
    return Group(*body)


def orchestrator_view():
    """Claude jobs of this repo: state, context, tokens and $, current activity, recent events."""
    jobs = M.cached("jobs", 1, M.claude_jobs)
    O = M.cached("orch_spend", 30, orchestrator_spend)
    per = O[1] if isinstance(O, tuple) else {}
    body = []
    if not isinstance(jobs, list) or not jobs:
        return Text("no Claude job for this repo", style="dim")
    for i, j in enumerate(jobs):
        col = M.AGENT_COLORS[i % len(M.AGENT_COLORS)]
        st = j.get("state", "?")
        t = Text("● ", style=col, no_wrap=True, overflow="ellipsis")
        t.append(f"{j.get('name', '')[:40]}  ", style="bold")
        t.append(f"{st.upper():<8}", style=M.STATE_STYLE.get(st, "bold"))
        t.append_text(M.ctx_text(M.job_ctx(j)))
        t.append(f"  up {M.ago(M.iso(j['createdAt']))}", style="dim")
        c = per.get(j["_dir"])
        if c:
            t.append(f"  {c['msgs']} turns  in {M.kfmt(c['inp'])} out {M.kfmt(c['out'])}  ${c['usd']:.2f}", style="cyan")
        body.append(t)
        body.append(Text("  " + (j.get("detail") or "").replace("\n", " "), style="white", no_wrap=True,
                         overflow="ellipsis"))
        for e in M.timeline(j, 5)[:-1][::-1]:
            body.append(Text(f"  {M.ago(M.iso(e['at'])):>6} {e['detail'].splitlines()[0][:200]}", style="grey62",
                             no_wrap=True, overflow="ellipsis"))
    return Group(*body)
