#!/usr/bin/env python3
"""Local web monitor for the current stage: game code is nearly all written, the orchestrator hands jobs to agents
(tools/deep_worker.py) and oversees them. Replaced the terminal dashboard of the wave stage (deleted 2026-10-13).

  .venv/bin/python tools/webmon.py [--port 8777]        then http://127.0.0.1:8777

Shows: what is left of the game code (by kind), the evidence behind the done functions, agent jobs (state, agent,
login, age, patch size, the orchestrator's verdict; task / report / patch one click away), the escalation queue
by failure class with the job that holds each item, quota of every login, background processes (verifier, waves),
open backlog and decisions, recent commits.

Reads files only (db/, queue/, docs/PLAN.md, work/*.json, work/deep/), never the IDB. One collector thread builds
the state every few seconds, slow parts (evidence tiers, quota endpoints) on their own longer period; requests are
served from the last finished state, so a slow part never blanks the page.
"""
import argparse, collections, csv, html, json, os, re, subprocess, sys, threading, time
from http.server import BaseHTTPRequestHandler, ThreadingHTTPServer

ROOT = os.path.dirname(os.path.dirname(os.path.abspath(__file__)))
sys.path.insert(0, os.path.dirname(os.path.abspath(__file__)))
import deep_worker, limits  # noqa: E402

STATE = {"at": 0}
SLOW = {}  # name -> (at, value)
MIN_HOOK = 6  # the lanes' min_size: a smaller function has no room for a hook


def slow(name, period, fn):
    """Value of a slow reader, recomputed at most every `period` s; its last good value until then (or on error)."""
    at, val = SLOW.get(name, (0, None))
    if time.time() - at >= period:
        try:
            val = fn()
        except Exception as e:  # shown instead of hiding the panel
            val = val if val is not None else {"error": str(e)[:200]}
        SLOW[name] = (time.time(), val)
    return val


def read_json(*p):
    try:
        return json.load(open(os.path.join(ROOT, *p)))
    except (OSError, ValueError):
        return {}


def alive(pid):
    return bool(pid) and os.path.exists(f"/proc/{pid}")


def queue_items(rows):
    out = []
    qd = os.path.join(ROOT, "queue")
    for f in sorted(os.listdir(qd)):
        if not f.endswith(".md"):
            continue
        text = open(os.path.join(qd, f), errors="replace").read(3000)
        head = text.splitlines()[0] if text else ""
        why = head.split(":", 1)[1].strip() if ":" in head else ""
        if not why:  # a gate rejection: the reason is the first line of the fenced block
            m = re.search(r"```\n(.+)", text)
            why = m.group(1).strip() if m else "?"
        detail = ""
        if why.startswith("emulated run differs"):
            m = re.search(r"\n(\d+/\d+ profiles; .*)", text)
            detail = m.group(1)[:160] if m else ""
        elif m := re.search(r"\n(MISMATCH .*|.*eip=[0-9a-f]{8}.*)", text):
            detail = m.group(1)[:160]
        ea = f[:8]
        r = rows.get(ea, {})
        out.append(dict(ea=ea, name=r.get("name", ""), size=int(r.get("size") or 0), cls=queue_class(why),
                        why=why[:160], detail=detail))
    return out


def queue_class(why):
    w = why.lower()
    for key, cls in (("emulated run differs", "emudiff: differs from the original"),
                     ("differential test mismatch", "in-game diff mismatch"),
                     ("breaks the", "breaks the installed baseline"), ("smoke failure", "in-game smoke failure"),
                     ("semantic check", "semantic check"), ("compile error", "compile error"),
                     ("csf_func", "wrong ABI at a call (CSF_FUNC)"), ("reviewer", "reviewer rejected"),
                     ("duplicate hook", "duplicate hook"), ("parked", "parked")):
        if key in w:
            return cls
    return "other"


def evidence():
    import verify
    hashes = verify.code_hashes()
    tiers = verify.tiers(hashes)
    size = {r["ea"]: int(r["size"]) for r in csv.DictReader(open(os.path.join(ROOT, "db", "functions.csv")))}
    n, b = collections.Counter(tiers.values()), collections.Counter()
    for ea, t in tiers.items():
        b[t] += size.get(ea, 0)
    pend = verify.pending(hashes)
    return dict(tiers=[dict(tier=t, n=n[t], kb=round(b[t] / 1024, 1)) for t in
                       ("diffed", "executed", "gate", "stale", "mismatched")],
                pending=len(pend), pending_kb=round(sum(size.get(e, 0) for e in pend) / 1024, 1))


def quota():
    out = []
    for h in limits.codex_homes():
        row = dict(login=limits.codex_label(h), kind="Codex (sol-hi jobs, lanes)")
        try:
            u = limits.codex_usage(h)
            row.update({k: dict(used=v[0], reset_min=round(v[1] / 60)) for k, v in u.items()})
        except Exception as e:
            row["error"] = str(e)[:80]
        out.append(row)
    import grunt
    for which, kind in (("main", "Claude main (orchestrator, opus-me jobs)"), ("grunts", "Claude grunts (Haiku lanes)")):
        row = dict(login="claude " + which, kind=kind)
        try:
            u = grunt.claude_usage(which)  # {window: (percent, seconds until reset)}
            for k in ("5h", "7d"):
                if k in u:
                    row[k] = dict(used=round(u[k][0]), reset_min=max(0, round(u[k][1] / 60)))
        except Exception as e:
            row["error"] = str(e)[:80]
        out.append(row)
    return out


def plan():
    text = open(os.path.join(ROOT, "docs", "PLAN.md")).read()
    sect = text.split("## Backlog", 1)[1].split("\n## ", 1)[0]
    areas, cur = [], None
    for line in sect.splitlines():
        if line.startswith("### "):
            cur = dict(area=line[4:].strip(), open=[], done=0)
            areas.append(cur)
        elif cur and (m := re.match(r"- \[( |x)\] (.*)", line.strip())):
            if m.group(1) == "x":
                cur["done"] += 1
            else:
                cur["open"].append(m.group(2)[:200])
    dec = text.split("## Open decisions", 1)[1].split("\n## ", 1)[0] if "## Open decisions" in text else ""
    decisions = [re.sub(r"\s+", " ", d.replace("**", "")).strip()[:700] for d in re.split(r"\n(?=- )", dec) if d.strip().startswith("- ")]
    focus = text.split("## Current focus", 1)[1].split("\n## ", 1)[0] if "## Current focus" in text else ""
    focus = [re.sub(r"\s+", " ", d).strip()[:260] for d in re.split(r"\n(?=\d+\. )", focus) if re.match(r"\d+\. ", d.strip())]
    return dict(areas=areas, decisions=decisions, focus=focus)


def linktest():
    d = read_json("work", "linktest.json")
    if not d:
        return {}
    u = d.get("unresolved_functions") or {}
    unres = ", ".join(f"{region} {len(v)}" for region, v in sorted(u.items() if isinstance(u, dict) else u))
    return dict(summary=f"Cutover link test: {d.get('compiled')}/{d.get('files')} files compile standalone; "
                        f"functions still unresolved: {unres}; {len(d.get('globals') or [])} globals and "
                        f"{len(d.get('vtables') or [])} vtables to generate")


def background():
    out = []
    for label, f in (("verifier", "verifier_state.json"), ("lanes / waves", "grunt_state.json")):
        st = read_json("work", f)
        if st:
            out.append(dict(what=label, running=alive(st.get("pid")), phase=st.get("phase", ""),
                            age_min=round((time.time() - st.get("updated", time.time())) / 60),
                            counts=st.get("counts") or {}))
    try:
        ps = subprocess.run(["pgrep", "-af", r"tools/(grunt|verifier|pairbisect|flakybisect|linktest|naming)\.py"],
                            capture_output=True, text=True).stdout
        procs = [re.sub(r"^\d+ \S*python\S* ", "", l)[:110] for l in ps.splitlines() if "pgrep" not in l and "bash -c" not in l]
    except OSError:
        procs = []
    return dict(states=out, procs=sorted(set(procs)))


def collect():
    rows = {r["ea"]: r for r in csv.DictReader(open(os.path.join(ROOT, "db", "functions.csv")))}
    game = [r for r in rows.values() if r["region"] == "game"]
    queue = queue_items(rows)
    qset = {q["ea"] for q in queue}
    jobs = deep_worker.jobs()
    holder = {}
    for j in jobs:
        if not j.get("review"):  # a reviewed job no longer holds its items
            for ea in j.get("eas", []):
                holder[ea] = j["name"]
    for q in queue:
        q["job"] = holder.get(q["ea"], "")
    for j in jobs:  # a task also names callees and neighbours: count the queue items only
        j["eas"] = [e for e in j.get("eas", []) if e in qset]
    left = collections.OrderedDict((k, [0, 0]) for k in ("stl", "queue", "tiny", "other"))
    for r in game:
        if r["status"] == "done":
            continue
        size = int(r["size"])
        k = "queue" if r["ea"] in qset else "stl" if r["module"] == "stl" else "tiny" if size < MIN_HOOK else "other"
        left[k][0] += 1
        left[k][1] += size
    total_b = sum(int(r["size"]) for r in game)
    done = [r for r in game if r["status"] == "done"]
    labels = dict(stl="STL template instances (replicas + generator)", queue="in the escalation queue",
                  tiny=f"under {MIN_HOOK} bytes (no room for a hook)", other="not attempted / returned to the lanes")
    for j in jobs:
        j["age_min"] = round(((j.get("finished") or time.time()) - j["started"]) / 60)
        j["tokens"] = slow("tok" + j["name"], 20 if j["phase"] == "running" else 600, lambda j=j: deep_worker.tokens(j))
        j["shells"] = len(deep_worker.shells(j))
        j.pop("usage", None)
    commits = subprocess.run(["git", "-C", ROOT, "log", "-14", "--format=%h\t%ar\t%s"], capture_output=True,
                             text=True).stdout.splitlines()
    return dict(
        at=time.time(),
        progress=dict(total=len(game), done=len(done), total_kb=round(total_b / 1024),
                      done_kb=round(sum(int(r["size"]) for r in done) / 1024),
                      pct=round(100 * sum(int(r["size"]) for r in done) / max(total_b, 1), 1),
                      left=[dict(kind=k, label=labels[k], n=v[0], kb=round(v[1] / 1024, 1)) for k, v in left.items()]),
        evidence=slow("evidence", 90, evidence), quota=slow("quota", 60, quota), plan=slow("plan", 30, plan),
        jobs=sorted(jobs, key=lambda j: (j["phase"] != "running", bool(j.get("review")), -j["started"])),
        queue=queue, background=background(), linktest=slow("linktest", 60, linktest),
        commits=[dict(zip(("hash", "when", "subject"), c.split("\t", 2))) for c in commits])


def collector(period):
    while True:
        t0 = time.time()
        try:
            st = collect()
            st["took"] = round(time.time() - t0, 2)
            STATE.clear()
            STATE.update(st)
        except Exception as e:
            STATE["error"] = f"{type(e).__name__}: {e}"
        time.sleep(max(1.0, period - (time.time() - t0)))


PAGE = r"""<!doctype html><html lang="en"><head><meta charset="utf-8"><title>CSF-RE monitor</title>
<meta name="viewport" content="width=device-width, initial-scale=1">
<style>
:root{--bg:#f6f5f1;--card:#fff;--ink:#1c1b19;--dim:#6d6a63;--line:#e2dfd7;--acc:#2f5d8a;--ok:#2e7d4f;--warn:#b26a00;--bad:#b3261e;--chip:#eeece6}
@media (prefers-color-scheme:dark){:root{--bg:#15161a;--card:#1d1f24;--ink:#e7e5df;--dim:#9a978f;--line:#2e3038;--acc:#7fb0e0;--ok:#6cc08b;--warn:#e0a94a;--bad:#f0827a;--chip:#282a31}}
*{box-sizing:border-box}body{margin:0;background:var(--bg);color:var(--ink);font:14px/1.45 system-ui,-apple-system,"Segoe UI",sans-serif}
header{display:flex;gap:16px;align-items:baseline;padding:14px 20px 6px;flex-wrap:wrap}
h1{font-size:17px;margin:0;font-weight:650}h2{font-size:12px;letter-spacing:.06em;text-transform:uppercase;color:var(--dim);margin:0 0 8px;font-weight:600}
#stamp{color:var(--dim);font-size:12px;margin-left:auto}
main{display:grid;grid-template-columns:repeat(12,1fr);gap:12px;padding:8px 20px 28px}
section{background:var(--card);border:1px solid var(--line);border-radius:8px;padding:12px 14px;min-width:0}
.c4{grid-column:span 4}.c5{grid-column:span 5}.c6{grid-column:span 6}.c7{grid-column:span 7}.c8{grid-column:span 8}.c12{grid-column:span 12}
@media (max-width:1100px){.c4,.c5,.c6,.c7,.c8{grid-column:span 12}}
table{border-collapse:collapse;width:100%}td,th{padding:4px 8px 4px 0;text-align:left;vertical-align:top;border-top:1px solid var(--line)}
tr:first-child td,thead th{border-top:none}th{font-weight:600;color:var(--dim);font-size:12px}
td.n,th.n{text-align:right;font-variant-numeric:tabular-nums;white-space:nowrap}
.mono{font-family:ui-monospace,Menlo,Consolas,monospace;font-size:12.5px}.dim{color:var(--dim)}.small{font-size:12px}
.big{font-size:30px;font-weight:650;font-variant-numeric:tabular-nums}
.bar{height:8px;background:var(--chip);border-radius:4px;overflow:hidden;margin:6px 0 10px}.bar>i{display:block;height:100%;background:var(--acc)}
.chip{display:inline-block;padding:1px 8px;border-radius:10px;background:var(--chip);font-size:12px;white-space:nowrap}
.running{color:var(--acc)}.done,.applied{color:var(--ok)}.quota,.partial,.dead{color:var(--warn)}.failed,.rejected{color:var(--bad)}
a,button.link{color:var(--acc);text-decoration:none;background:none;border:0;padding:0;font:inherit;cursor:pointer}a:hover,button.link:hover{text-decoration:underline}
details{border-top:1px solid var(--line);padding:5px 0}details:first-of-type{border-top:none}summary{cursor:pointer;display:flex;gap:10px;align-items:baseline}
summary b{font-weight:600}.grow{flex:1}.wrap{overflow-x:auto}
ul{margin:0;padding-left:18px}li{margin:3px 0}
dialog{width:min(1000px,94vw);max-height:88vh;border:1px solid var(--line);border-radius:8px;background:var(--card);color:var(--ink);padding:0}
dialog::backdrop{background:rgba(0,0,0,.45)}dialog header{padding:10px 14px;border-bottom:1px solid var(--line);position:sticky;top:0;background:var(--card)}
dialog pre{margin:0;padding:14px;white-space:pre-wrap;word-break:break-word;font:12.5px/1.5 ui-monospace,Menlo,Consolas,monospace}
tr.job{cursor:pointer}tr.job:hover td{background:var(--chip)}tr.jd td{background:var(--bg);padding:12px 14px;border-top:none}
.jdw{display:grid;grid-template-columns:minmax(0,2fr) minmax(0,3fr);gap:20px}@media (max-width:1100px){.jdw{grid-template-columns:1fr}}
.hist{padding:3px 0;border-top:1px solid var(--line);overflow-wrap:anywhere}.hist:first-of-type{border-top:none}
.meter{display:inline-block;width:64px;height:7px;background:var(--chip);border-radius:4px;overflow:hidden;vertical-align:middle;margin-right:6px}.meter>i{display:block;height:100%}
</style></head><body>
<header><h1>CSF-RE</h1><span class="dim">agent jobs, queue and what is left of the game code</span><span id="stamp"></span></header>
<main id="m"><section class="c12">loading…</section></main>
<dialog id="dlg"><header><b id="dt"></b> <span class="dim small" id="ds"></span><button class="link" style="float:right" onclick="dlg.close()">close</button></header><pre id="dp"></pre></dialog>
<script>
const E=s=>String(s??'').replace(/[&<>"]/g,c=>({'&':'&amp;','<':'&lt;','>':'&gt;','"':'&quot;'}[c]));
const open_=new Set(),openJobs=new Set(),jd={};let last=null;
const k=n=>n>=1e6?(n/1e6).toFixed(1)+'M':n>=1e3?Math.round(n/1e3)+'k':String(n||0);
const tok=t=>!t||(!t.input&&!t.output)?'<span class="dim">–</span>':`${k(t.input)} / ${k(t.output)}`;
const dur=s=>s>=3600?(s/3600).toFixed(1)+' h':s>=60?Math.round(s/60)+' min':s+' s';
function jobDetail(n){const d=jd[n];if(!d)return '<span class="dim">loading…</span>';const t=d.tokens||{};
  return `<div class="jdw"><div><h2>Running shells</h2>${(d.shells||[]).length?d.shells.map(x=>`<div class="mono small"><span class="running">${dur(x.secs)}</span> ${E(x.cmd)}</div>`).join(''):`<span class="dim small">${d.phase=='running'?'none right now (the agent is thinking or reading)':'job is not running'}</span>`}
   <h2 style="margin-top:12px">Tokens</h2><div class="small">input ${k(t.input)} <span class="dim">(${k(t.cached)} from cache)</span> · output ${k(t.output)}${t.context?` · context now ${k(t.context)}${t.window?' of '+k(t.window):''}`:''}${t.cost_usd?` · $${t.cost_usd.toFixed(2)}`:''}</div>
   ${!t.input&&d.phase=='running'?'<div class="dim small">no running count for this job (started before the session log was kept); the total shows when it ends</div>':''}</div>
   <div><h2>Last steps</h2>${(d.history||[]).length?d.history.slice().reverse().map(x=>`<div class="hist"><span class="chip">${x.kind}</span> <span class="${x.kind=='say'?'':'mono small'}">${E(x.text)}</span></div>`).join(''):'<span class="dim small">nothing recorded yet</span>'}</div></div>`;}
async function loadJobs(){for(const n of openJobs){try{const r=await fetch('/api/job/'+n);jd[n]=await r.json();const el=document.getElementById('jd-'+n);if(el)el.innerHTML=jobDetail(n);}catch(e){}}}
async function show(title,url){dt.textContent=title;ds.textContent=url;dp.textContent='loading…';dlg.showModal();
  const r=await fetch(url);dp.textContent=r.ok?await r.text():'not there (yet)';}
function meter(u){if(!u)return '<span class="dim">–</span>';const c=u.used>=90?'var(--bad)':u.used>=70?'var(--warn)':'var(--ok)';
  const r=u.reset_min>=120?Math.round(u.reset_min/60)+' h':u.reset_min+' min';
  return `<span class="meter"><i style="width:${Math.min(100,u.used)}%;background:${c}"></i></span>${u.used}% <span class="dim small" title="until the window resets">· ${r}</span>`;}
function render(s){last=s;const p=s.progress,ev=s.evidence||{},pl=s.plan||{},bg=s.background||{};let h='';
  // progress
  h+=`<section class="c4"><h2>Game code written</h2><div class="big">${p.pct}%</div><div class="bar"><i style="width:${p.pct}%"></i></div>
   <div class="dim small">${p.done.toLocaleString()} of ${p.total.toLocaleString()} functions, ${p.done_kb} of ${p.total_kb} KB</div>
   <table style="margin-top:8px"><thead><tr><th>Left</th><th class="n">fns</th><th class="n">KB</th></tr></thead>${p.left.map(l=>`<tr><td>${E(l.label)}</td><td class="n">${l.n}</td><td class="n">${l.kb}</td></tr>`).join('')}</table></section>`;
  // evidence
  const tl={diffed:'compared with the original in game',executed:'ran in game without crashing',gate:'static checks only, never ran',stale:'changed since its evidence',mismatched:'differs from the original'};
  h+=`<section class="c4"><h2>Evidence behind the done functions</h2>${ev.error?`<div class="failed">${E(ev.error)}</div>`:ev.tiers?`<table>${ev.tiers.map(t=>`<tr><td>${tl[t.tier]}</td><td class="n">${t.n.toLocaleString()}</td><td class="n dim">${t.kb} KB</td></tr>`).join('')}
   <tr><td>pending first in-game install</td><td class="n">${ev.pending}</td><td class="n dim">${ev.pending_kb} KB</td></tr></table>`:'<span class="dim">collecting…</span>'}
   ${s.linktest&&s.linktest.summary?`<div class="dim small" style="margin-top:8px">${E(s.linktest.summary)}</div>`:''}</section>`;
  // quota
  h+=`<section class="c4"><h2>Quota</h2><table><thead><tr><th>Login</th><th>5 h window</th><th>7 d window</th></tr></thead>${(s.quota||[]).map(q=>`<tr><td title="${E(q.kind)}">${E(q.login)}</td>${q.error?`<td colspan=2 class="failed small">${E(q.error)}</td>`:`<td>${meter(q['5h'])}</td><td>${meter(q['7d'])}</td>`}</tr>`).join('')}</table>
   <h2 style="margin-top:14px">Background</h2>${(bg.states||[]).map(b=>`<div><span class="${b.running?'running':'dim'}">●</span> ${E(b.what)}: ${E(b.phase)||'–'} <span class="dim small">${b.running?'':'(not running, '+b.age_min+' min ago)'}</span></div>`).join('')}
   ${(bg.procs||[]).map(x=>`<div class="mono dim small">${E(x)}</div>`).join('')}</section>`;
  // jobs
  const J=s.jobs||[];const run=J.filter(j=>j.phase=='running').length,wait=J.filter(j=>j.phase!='running'&&!j.review).length;
  h+=`<section class="c12"><h2>Agent jobs <span class="chip">${run} running</span> <span class="chip">${wait} waiting for review</span> <span class="chip">${J.length-run-wait} reviewed</span></h2><div class="wrap"><table>
   <thead><tr><th>Job</th><th>Agent</th><th>State</th><th class="n">Time</th><th class="n">Tokens in / out</th><th class="n">Patch</th><th>Verdict</th><th>Task</th><th></th></tr></thead>
   ${J.map(j=>`<tr class="job" data-job="${E(j.name)}"><td class="mono"><span class="dim">${openJobs.has(j.name)?'▾':'▸'}</span> ${E(j.name)}</td><td>${E(j.agent||'sol-hi')}<div class="dim small">${E(j.login||'')}</div></td>
    <td class="${j.phase}">${j.phase=='done'&&!j.review?'done, to review':j.phase}${j.shells?` <span class="chip">${j.shells} shell${j.shells>1?'s':''}</span>`:''}${j.runs>1?` <span class="dim small">run ${j.runs}</span>`:''}${j.error?`<div class="small dim">${E(j.error.slice(0,90))}</div>`:''}</td>
    <td class="n">${j.age_min} min</td><td class="n">${tok(j.tokens)}</td><td class="n">${j.patch_lines??'–'}</td>
    <td class="${j.review||''}">${j.review||'–'}${j.note?`<div class="small dim">${E(j.note)}</div>`:''}</td>
    <td>${E(j.task)}${(j.eas||[]).length?` <span class="dim small">(${j.eas.length} addresses)</span>`:''}</td>
    <td style="white-space:nowrap" onclick="event.stopPropagation()"><button class="link" onclick="show('${E(j.name)}: task','/job/${E(j.name)}/task')">task</button> ·
     <button class="link" onclick="show('${E(j.name)}: report','/job/${E(j.name)}/report')">report</button> ·
     <button class="link" onclick="show('${E(j.name)}: patch','/job/${E(j.name)}/patch')">patch</button> ·
     <button class="link" onclick="show('${E(j.name)}: activity','/job/${E(j.name)}/activity')">full history</button></td></tr>
    ${openJobs.has(j.name)?`<tr class="jd"><td colspan=9 id="jd-${E(j.name)}">${jobDetail(j.name)}</td></tr>`:''}`).join('')||'<tr><td colspan=9 class="dim">no jobs yet</td></tr>'}</table></div></section>`;
  // queue
  const Q=s.queue||[],by={};Q.forEach(q=>(by[q.cls]=by[q.cls]||[]).push(q));const held=Q.filter(q=>q.job).length;
  h+=`<section class="c7"><h2>Escalation queue <span class="chip">${Q.length} items</span> <span class="chip">${held} held by a job</span></h2>
   ${Object.entries(by).sort((a,b)=>b[1].length-a[1].length).map(([c,v])=>`<details data-k="${E(c)}" ${open_.has(c)?'open':''}><summary><b>${v.length}</b><span class="grow">${E(c)}</span><span class="dim small">${v.filter(q=>q.job).length} held · ${(v.reduce((a,q)=>a+q.size,0)/1024).toFixed(1)} KB</span></summary>
    <div class="wrap"><table>${v.map(q=>`<tr><td class="mono"><button class="link" onclick="show('queue ${q.ea}','/queue/${q.ea}')">${q.ea}</button></td><td>${E(q.name)||'<span class=dim>unnamed</span>'}</td><td class="n">${q.size}</td><td class="small dim">${E(q.detail)}</td><td class="mono small running">${E(q.job)}</td></tr>`).join('')}</table></div></details>`).join('')}</section>`;
  // plan
  h+=`<section class="c5"><h2>Waiting on the user</h2>${(pl.decisions||[]).length?`<ul>${pl.decisions.map(d=>`<li>${E(d.replace(/^- /,''))}</li>`).join('')}</ul>`:'<span class="dim">nothing</span>'}
   <h2 style="margin-top:14px">Open backlog</h2>${(pl.areas||[]).map(a=>`<details data-k="pl${E(a.area)}" ${open_.has('pl'+a.area)?'open':''}><summary><b>${a.open.length}</b><span class="grow">${E(a.area)}</span><span class="dim small">${a.done} done</span></summary><ul class="small">${a.open.map(o=>`<li>${E(o)}</li>`).join('')}</ul></details>`).join('')}
   <h2 style="margin-top:14px">Recent commits</h2><table>${(s.commits||[]).map(c=>`<tr><td class="mono dim small">${c.hash}</td><td class="small">${E(c.subject)}</td><td class="dim small" style="white-space:nowrap">${E(c.when)}</td></tr>`).join('')}</table></section>`;
  m.innerHTML=h;
  m.querySelectorAll('tr.job').forEach(r=>r.addEventListener('click',()=>{const n=r.dataset.job;openJobs.has(n)?openJobs.delete(n):openJobs.add(n);render(last);loadJobs();}));
  m.querySelectorAll('details').forEach(d=>d.addEventListener('toggle',()=>d.open?open_.add(d.dataset.k):open_.delete(d.dataset.k)));
  stamp.textContent=(s.error?'collector error: '+s.error+' · ':'')+'state from '+new Date(s.at*1000).toLocaleTimeString();}
async function tick(){try{const r=await fetch('/api/state');const s=await r.json();if(s.progress&&!dlg.open){render(s);loadJobs();}else if(!s.progress)stamp.textContent='collecting…';}
  catch(e){stamp.textContent='monitor not reachable';}}
(location.hash.match(/job=([\w.,-]+)/)||['',''])[1].split(',').filter(Boolean).forEach(n=>openJobs.add(n));
tick();setInterval(tick,4000);
</script></body></html>"""


def activity(name):
    """A job's longer history as text (the row's click shows the last few)."""
    return "\n\n".join(f"[{k}] {t}" for k, t in deep_worker.history(name, 80)) or "no activity recorded yet"


class H(BaseHTTPRequestHandler):
    def log_message(self, *a):
        pass

    def send(self, body, ctype="text/plain; charset=utf-8", code=200):
        data = body.encode() if isinstance(body, str) else body
        self.send_response(code)
        self.send_header("Content-Type", ctype)
        self.send_header("Content-Length", str(len(data)))
        self.send_header("Cache-Control", "no-store")
        self.end_headers()
        self.wfile.write(data)

    def do_GET(self):
        path = self.path.split("?")[0]
        try:
            if path == "/":
                return self.send(PAGE, "text/html; charset=utf-8")
            if path == "/api/state":
                return self.send(json.dumps(STATE), "application/json")
            if m := re.fullmatch(r"/api/job/([\w.-]+)", path):
                return self.send(json.dumps(deep_worker.detail(m.group(1))), "application/json")
            if m := re.fullmatch(r"/job/([\w.-]+)/(task|report|patch|activity)", path):
                name, what = m.groups()
                if what == "activity":
                    return self.send(activity(name))
                f = os.path.join(deep_worker.DEEP, name, dict(task="task.md", report="report.md", patch="patch.diff")[what])
                return self.send(open(f, errors="replace").read(400000) or "(empty)")
            if m := re.fullmatch(r"/queue/([0-9a-f]{8})", path):
                return self.send(open(os.path.join(ROOT, "queue", m.group(1) + ".md"), errors="replace").read(400000))
        except OSError:
            pass
        self.send("not found", code=404)


def main():
    ap = argparse.ArgumentParser(description=__doc__, formatter_class=argparse.RawDescriptionHelpFormatter)
    ap.add_argument("--port", type=int, default=8777)
    ap.add_argument("--interval", type=float, default=4.0)
    ap.add_argument("--once", action="store_true", help="print one collected state as JSON and exit")
    a = ap.parse_args()
    if a.once:
        print(json.dumps(collect(), indent=1))
        return 0
    threading.Thread(target=collector, args=(a.interval,), daemon=True).start()
    srv = ThreadingHTTPServer(("127.0.0.1", a.port), H)
    print(f"CSF-RE monitor: http://127.0.0.1:{a.port}", flush=True)
    try:
        srv.serve_forever()
    except KeyboardInterrupt:
        pass
    return 0


if __name__ == "__main__":
    sys.exit(main())
