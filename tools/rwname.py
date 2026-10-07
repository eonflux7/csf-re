#!/usr/bin/env python3
"""Name the RenderWare boundary API (RW functions called by game code) with an LLM, bottom-up.

Usage:
  rwname.py run [--n N] [--jobs 8] [--only ea,ea] [--redo] [--backend codex|deepseek|claude] [--model M]
  rwname.py dedupe                 re-ask name collisions jointly (one prompt per group)
  rwname.py report                 conflicts / low confidence
  rwname.py apply [--min 0.6]      write accepted names into db/functions.csv (name_src=rwname)

Results accumulate in db/rw_names.jsonl (one line per attempt; the last line per ea wins).
Packet: own pseudocode (RwEngineInstance substituted), callee names/snippets, address neighbours' names
(RW compiles one source file's functions contiguously), strings, and game-side call sites.
"""
import argparse, collections, concurrent.futures as cf, json, os, re, sys
sys.path.insert(0, os.path.dirname(__file__))
from grunt import load_key, ask
from triage import load_csv, save_csv

ROOT = os.path.dirname(os.path.dirname(os.path.abspath(__file__)))
OUT = os.path.join(ROOT, "db", "rw_names.jsonl")

SYSTEM = """You identify functions of a statically linked RenderWare Graphics 3.7 (Criterion, 2004-2005; D3D9
driver, release build, no asserts) inside a 2006 PC game. Answer with the exact public RenderWare C API name
(e.g. RwFrameCreate, RpClumpForAllAtomics, RwMatrixMultiply, RtAnimInterpolatorSetCurrentAnim,
RpHAnimHierarchyUpdateMatrices, RpSkinAtomicSetHAnimHierarchy, RwD3D9SetRenderState, RpMatFXMaterialSetEffects,
RtQuatRotate, RwStreamFindChunk, RtPNGImageRead, RpCollisionWorldForAllIntersections ...). Use the toolkit and
plugin names of RW 3.7 (Rt*, Rp*, Rw*, _rw* for internal ones). If it is an internal helper with no public name,
answer a descriptive name prefixed with an underscore and the module (e.g. _rpGeometryResize) and say so.

Facts about this binary:
- dword_8B68C0 is RwEngineInstance (RwGlobals*). RwEngineInstance+0x108 = RwMalloc(size, hint),
  +0x10C = RwFree(p), +0x110 = RwRealloc, +0x114 = RwCalloc, +0x118 = RwFreeListAlloc(freelist, hint),
  +0x11C = RwFreeListFree(freelist, p) (RW 3.7 memoryAlloc/memoryFree). Plugin/global offsets (e.g. dword_8B0924) are
  per-module static offsets into the engine instance or into objects.
- Functions are ordered as in the original source files: neighbours by address usually come from the same
  RW source module (e.g. baframe.c, baclump.c, rtanim.c), so neighbour names are a strong hint.
- Hex-Rays argument counts are reliable for __cdecl; RW API functions are __cdecl.

Output exactly one fenced ```json block:
{"name": "...", "confidence": 0.0-1.0, "kind": "api|internal", "prototype": "C prototype with RW types",
 "why": "one or two sentences of evidence"}"""

ENG = re.compile(r"\bdword_8B68C0\b")


def load():
    F = {}
    for l in open(os.path.join(ROOT, "export", "functions.jsonl")):
        d = json.loads(l)
        F[d["ea"]] = d
    return F


def results():
    res = {}
    if os.path.exists(OUT):
        for l in open(OUT):
            d = json.loads(l)
            res[int(d["ea"], 16)] = d
    return res


def pseudo(ea, limit=7000):
    p = os.path.join(ROOT, "export", "funcs", f"{ea:08x}.c")
    if not os.path.exists(p):
        return None
    t = ENG.sub("RwEngineInstance", open(p).read())
    return t if len(t) <= limit else t[:limit] + "\n... (truncated)\n"


def call_sites(ea, callers, k=3):
    pat = re.compile(rf"\bsub_{ea:X}\b")
    out = []
    for c in sorted(callers)[:12]:
        t = pseudo(c, 10 ** 6)
        if not t:
            continue
        lines = t.splitlines()
        for i, ln in enumerate(lines):
            if pat.search(ln):
                out.append(f"// in game function sub_{c:X}\n" + "\n".join(lines[max(0, i - 4): i + 3]))
                break
        if len(out) >= k:
            break
    return out


def packet(ea, F, rows, names, callers, eas_sorted, idx):
    known = lambda e: names.get(e) or (rows[e]["name"] if e in rows and rows[e]["name"] else None)
    parts = [f"## Function 0x{ea:08X} (size {F[ea]['size']} bytes)\n```c\n{pseudo(ea)}\n```"]
    cs = []
    for c in F[ea]["callees"]:
        n = known(c)
        if n:
            cs.append(f"- sub_{c:X} = {n}")
        elif rows.get(c, {}).get("region") == "rw":
            t = pseudo(c, 900)
            cs.append(f"- sub_{c:X} (unnamed, {F[c]['size']} B):\n```c\n{t}\n```" if t else f"- sub_{c:X} (unnamed)")
        else:
            cs.append(f"- sub_{c:X} ({rows.get(c, {}).get('region', '?')} code)")
    if cs:
        parts.append("## Callees\n" + "\n".join(cs[:25]))
    i = idx[ea]
    nb = []
    for j in range(max(0, i - 8), min(len(eas_sorted), i + 9)):
        e = eas_sorted[j]
        if e != ea and rows.get(e, {}).get("region") == "rw":
            nb.append(f"- 0x{e:08X} {known(e) or '?'}{'  <-- this function' if e == ea else ''}")
        elif e == ea:
            nb.append(f"- 0x{e:08X} <-- THIS FUNCTION")
    parts.append("## Address neighbours (same source file likely)\n" + "\n".join(nb))
    if F[ea]["strings"]:
        parts.append("## Strings\n" + "\n".join(f"- {s!r}" for s in F[ea]["strings"][:10]))
    sites = call_sites(ea, callers.get(ea, ()))
    if sites:
        parts.append(f"## Game call sites ({len(callers.get(ea, ()))} game callers)\n```c\n" + "\n\n".join(sites) + "\n```")
    return "\n\n".join(parts)


def order(targets, F):
    """Leaf-first among targets (callees within the target set first), then by address."""
    tset, done, out = set(targets), set(), []
    pending = sorted(targets)
    while pending:
        ready = [e for e in pending if all(c in done or c not in tset or c == e for c in F[e]["callees"])]
        if not ready:
            ready = pending[:1]
        for e in ready:
            done.add(e)
            out.append(e)
        pending = [e for e in pending if e not in done]
    return out


def one(ea, msg, backend, model, key):
    text, usage = ask(backend, model, [dict(role="system", content=SYSTEM), dict(role="user", content=msg)], key)
    m = re.search(r"```json\s*\n(.*?)```", text, re.S)
    try:
        d = json.loads(m.group(1)) if m else {}
    except json.JSONDecodeError:
        d = {}
    return dict(ea=f"{ea:08x}", backend=backend, model=model, usage=usage, **{k: d.get(k) for k in
                                                                              ("name", "confidence", "kind", "prototype", "why")})


def run(a):
    F, rows = load(), load_csv()
    api = json.load(open(os.path.join(ROOT, "export", "boundary_api.json")))["rw"]
    callers = {int(k, 16): [int(x, 16) for x in v] for k, v in
               json.load(open(os.path.join(ROOT, "export", "boundary_callers.json"))).items()}
    res = results()
    targets = [int(x, 16) for x in a.only.split(",")] if a.only else \
        [int(e, 16) for e in api if a.redo or int(e, 16) not in res]
    targets = order(targets, F)[: a.n]
    eas_sorted = sorted(F)
    idx = {e: i for i, e in enumerate(eas_sorted)}
    key = load_key() if a.backend == "deepseek" else None
    # dependency queue (no waves): a function starts once its callees within the target set have answers
    tset, done, inflight = set(targets), set(), {}
    pending = list(targets)
    with cf.ThreadPoolExecutor(a.jobs) as ex:
        while pending or inflight:
            ready = [e for e in pending if all(c in done or c not in tset or c == e for c in F[e]["callees"])]
            if not ready and not inflight and pending:
                ready = pending[:1]  # cycle: break it
            names = {e: d["name"] for e, d in results().items() if d.get("name") and (d.get("confidence") or 0) >= 0.5}
            for e in ready[: max(0, a.jobs * 2 - len(inflight))]:
                pending.remove(e)
                inflight[ex.submit(one, e, packet(e, F, rows, names, callers, eas_sorted, idx), a.backend, a.model,
                                   key)] = e
            fin, _ = cf.wait(list(inflight), return_when=cf.FIRST_COMPLETED)
            for fu in fin:
                e = inflight.pop(fu)
                done.add(e)
                try:
                    r = fu.result()
                except Exception as err:
                    print(f"  {e:08x} ERROR {err}", flush=True)
                    continue
                with open(OUT, "a") as f:
                    f.write(json.dumps(r) + "\n")
                print(f"  {r['ea']} {r['confidence']!s:5} {r['name']}", flush=True)


DEDUPE_SYSTEM = SYSTEM.split("Output exactly")[0] + """These functions were each given the SAME name by an earlier pass,
which cannot be right (one RW function has one address). Compare them and give each its correct, distinct RW 3.7
name (e.g. a Set/Get pair, Width/Height, Int16/Int32/Real variants, Atomic/Clump/Light variants).
Output exactly one ```json block: {"0xADDR": {"name": "...", "confidence": 0.0-1.0, "why": "..."}, ...}"""


def dedupe(a):
    """Re-ask name collisions jointly: each group of functions sharing a name goes to the model in one prompt."""
    F, rows = load(), load_csv()
    res = results()
    api = {int(e, 16) for e in json.load(open(os.path.join(ROOT, "export", "boundary_api.json")))["rw"]}
    applied = {e: r["name"] for e, r in rows.items() if r["name_src"] == "rwname"}
    by = collections.defaultdict(set)
    for e, d in res.items():
        if e in api and d.get("name"):
            by[d["name"]].add(e)
    for e, n in applied.items():
        by[n].add(e)
    groups = [sorted(g) for g in by.values() if len(g) > 1]
    print(f"{len(groups)} collision groups")
    eas_sorted = sorted(F)
    idx = {e: i for i, e in enumerate(eas_sorted)}
    key = load_key() if a.backend == "deepseek" else None
    names = {e: d["name"] for e, d in res.items() if d.get("name")}

    def ask_group(g):
        parts = [packet(e, F, rows, names, {}, eas_sorted, idx) for e in g]
        msg = "\n\n---\n\n".join(parts)
        text, usage = ask(a.backend, a.model, [dict(role="system", content=DEDUPE_SYSTEM), dict(role="user", content=msg)], key)
        m = re.search(r"```json\s*\n(.*?)```", text, re.S)
        return g, json.loads(m.group(1)) if m else {}

    with cf.ThreadPoolExecutor(a.jobs) as ex:
        for fu in cf.as_completed([ex.submit(ask_group, g) for g in groups]):
            try:
                g, d = fu.result()
            except Exception as err:
                print("  ERROR", err)
                continue
            with open(OUT, "a") as f:
                for k, v in d.items():
                    e = int(k, 16)
                    if e in g and isinstance(v, dict):
                        f.write(json.dumps(dict(ea=f"{e:08x}", backend=a.backend, model=a.model, kind="dedupe",
                                                name=v.get("name"), confidence=v.get("confidence"), why=v.get("why"))) + "\n")
                        print(f"  {e:08x} {v.get('confidence')!s:5} {v.get('name')}")
                        if rows[e]["name_src"] == "rwname":  # re-evaluated by apply
                            rows[e]["name"], rows[e]["name_src"] = "", ""
    save_csv(rows)


def report(a):
    res = results()
    by = collections.defaultdict(list)
    for e, d in res.items():
        by[d.get("name")].append(e)
    print("== duplicate names")
    for n, es in sorted(by.items(), key=lambda x: str(x[0])):
        if len(es) > 1:
            print(f"  {n}: {' '.join(f'{e:08x}' for e in es)}")
    low = [(d.get("confidence") or 0, e, d.get("name")) for e, d in res.items() if (d.get("confidence") or 0) < a.min]
    print(f"== below {a.min}: {len(low)}")
    for c, e, n in sorted(low):
        print(f"  {e:08x} {c} {n}")
    cs = [d.get("confidence") or 0 for d in res.values()]
    print(f"== {len(res)} named; mean confidence {sum(cs) / max(1, len(cs)):.2f}")


def apply(a):
    res, rows = results(), load_csv()
    by = collections.Counter(d.get("name") for d in res.values())
    n = 0
    for e, d in res.items():
        if d.get("name") and (d.get("confidence") or 0) >= a.min and by[d["name"]] == 1 \
                and rows[e]["name_src"] in ("", "rwname", "ida"):
            rows[e]["name"], rows[e]["name_src"] = d["name"], "rwname"
            n += 1
    save_csv(rows)
    print(f"applied {n} names")


def main():
    ap = argparse.ArgumentParser()
    sp = ap.add_subparsers(dest="cmd", required=True)
    r = sp.add_parser("run")
    r.add_argument("--n", type=int, default=10 ** 6)
    r.add_argument("--jobs", type=int, default=8)
    r.add_argument("--only")
    r.add_argument("--redo", action="store_true")
    r.add_argument("--backend", choices=["codex", "deepseek", "claude"], default="codex")
    r.add_argument("--model", default="gpt-6.1-sol")
    d = sp.add_parser("dedupe")
    d.add_argument("--jobs", type=int, default=8)
    d.add_argument("--backend", choices=["codex", "deepseek", "claude"], default="codex")
    d.add_argument("--model", default="gpt-6.1-sol")
    d.set_defaults(fn=dedupe)
    for name, fn in (("report", report), ("apply", apply)):
        p = sp.add_parser(name)
        p.add_argument("--min", type=float, default=0.6)
        p.set_defaults(fn=fn)
    r.set_defaults(fn=run)
    a = ap.parse_args()
    a.fn(a)


if __name__ == "__main__":
    main()
