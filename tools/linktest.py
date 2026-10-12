#!/usr/bin/env python3
"""Cutover link test: compile every src/ file as the standalone build (-DCSF_STANDALONE) and resolve symbols by
original address (see src/globals.h). Nothing runs; it answers "what is left before the exe links".

  - functions: csf::fn_slot<A> defined by HOOK(A, ...) vs referenced by CSF_FUNC(.., A, ..); unresolved ones grouped
    by region (game = remaining work; rw/crt/... = library boundary to provide at cutover)
  - duplicate definitions (two files implement A: a real link error)
  - data: csf::gv<A> (globals) and csf::vt<A> (vtables) the generated data must provide
  - raw image addresses left in code (a literal 0x4xxxxx..0x8xxxxx outside the CSF_* macros): wrong standalone
  - files that don't compile standalone

Usage: linktest.py [--list game|rw|...|raw|dup|fail] [-j N]
Writes work/linktest.json (status.py shows the summary).
"""
import argparse, collections, concurrent.futures as cf, csv, json, os, re, shlex, subprocess, sys
sys.path.insert(0, os.path.dirname(__file__))

ROOT = os.path.dirname(os.path.dirname(os.path.abspath(__file__)))
OBJ = os.path.join(ROOT, "build", "standalone")
OUT = os.path.join(ROOT, "work", "linktest.json")
HEADERS = ["src/globals.h", "hook/hook.h", "src/types/auto_classes.h", "src/core/memory.h", "src/legacy/string.h"]
SLOT = re.compile(r"csf::(fn_slot|gv|vt)<(\d+)>")


def flags():
    cc = json.load(open(os.path.join(ROOT, "build", "compile_commands.json")))
    cmd = next(c["command"] for c in cc if c["file"].endswith("hook/main.cpp"))
    a = shlex.split(cmd)
    return [x for x in a[:a.index("-c")] if not x.startswith(("/Fo", "/Fd"))] + ["-DCSF_STANDALONE"]


def sources():
    out = []
    for d, _, fs in os.walk(os.path.join(ROOT, "src")):
        out += [os.path.join(d, f) for f in fs if f.endswith(".cpp")]
    return sorted(out)


def compile_one(fl, src, newest_header):
    obj = os.path.join(OBJ, os.path.relpath(src, ROOT).replace("/", "_") + ".obj")
    if os.path.exists(obj) and os.path.getmtime(obj) >= max(os.path.getmtime(src), newest_header):
        return src, obj, None
    r = subprocess.run(fl + [f"/Fo{obj}", "-c", "--", src], capture_output=True, text=True,
                       cwd=os.path.join(ROOT, "build"))
    if r.returncode:
        if os.path.exists(obj):
            os.remove(obj)
        err = [l for l in (r.stdout + r.stderr).splitlines() if "error" in l]
        return src, None, "\n".join(err[:5])
    return src, obj, None


_undname = {}


def demangle(syms):
    """MSVC-mangled names -> demangled, one llvm-undname call for the lot."""
    todo = [s for s in syms if s not in _undname]
    if todo:
        r = subprocess.run(["llvm-undname-21"], input="\n".join(todo) + "\n", capture_output=True, text=True).stdout
        outs = [l for l in r.splitlines() if l.strip()]
        # undname echoes each input line followed by its demangled form
        for i in range(0, len(outs) - 1, 2):
            _undname[outs[i].strip()] = outs[i + 1].strip()
    return {s: _undname.get(s, s) for s in syms}


def symbols(obj):
    out = subprocess.run(["llvm-nm-21", obj], capture_output=True, text=True).stdout
    defined, undefined = set(), set()
    for l in out.splitlines():
        parts = l.split()
        if len(parts) == 2 and parts[0] == "U":
            undefined.add(parts[1])
        elif len(parts) == 3 and parts[1] in "RDBTrdbt":
            defined.add(parts[2])
    return defined, undefined


def raw_addresses(src, known=None):
    from addrfix import raw_hits
    return sorted({v for _, _, v, _ in raw_hits(open(src).read())})


def run(jobs):
    os.makedirs(OBJ, exist_ok=True)
    fl = flags()
    newest = max(os.path.getmtime(os.path.join(ROOT, h)) for h in HEADERS if os.path.exists(os.path.join(ROOT, h)))
    srcs = sources()
    objs, fails = {}, {}
    with cf.ThreadPoolExecutor(jobs) as ex:
        for src, obj, err in ex.map(lambda s: compile_one(fl, s, newest), srcs):
            if obj:
                objs[src] = obj
            else:
                fails[os.path.relpath(src, ROOT)] = err
    defs, refs = collections.defaultdict(list), collections.defaultdict(set)
    other_undef = collections.Counter()
    per = {}
    with cf.ThreadPoolExecutor(jobs) as ex:  # one llvm-nm per object: 35 s for 4k objects one after another
        for src, du in zip(objs, ex.map(symbols, objs.values())):
            per[src] = du
    allsyms = set()
    for d, u in per.values():
        allsyms |= {s for s in d | u if "csf@@" in s}
    dm = demangle(sorted(allsyms))
    defined_names = set()
    for d, _ in per.values():
        defined_names |= d
    for src, (d, u) in per.items():
        rel = os.path.relpath(src, ROOT)
        for s in d:
            m = SLOT.search(dm.get(s, ""))
            if m and m.group(1) == "fn_slot" and rel not in defs[int(m.group(2))]:
                defs[int(m.group(2))].append(rel)
        for s in u:
            m = SLOT.search(dm.get(s, ""))
            if m:
                refs[(m.group(1), int(m.group(2)))].add(rel)
            elif s not in defined_names and not s.startswith(("__", "_imp_", "??_", "_fltused")):
                other_undef[s] += 1
    rows = {int(r["ea"], 16): r for r in csv.DictReader(open(os.path.join(ROOT, "db", "functions.csv")))}
    fn_unres = collections.defaultdict(dict)
    for (kind, a), users in refs.items():
        if kind == "fn_slot" and a not in defs:
            r = rows.get(a)
            region = r["region"] if r else "not-a-function"
            fn_unres[region][a] = len(users)
    from addrfix import asm_hits
    raw, asm = {}, {}
    for src in srcs:
        if not os.path.exists(src):  # quarantined by the verifier while this ran
            continue
        h = raw_addresses(src)
        if h:
            raw[os.path.relpath(src, ROOT)] = [f"0x{v:X}" for v in h]
        h = asm_hits(open(src).read())
        if h:
            asm[os.path.relpath(src, ROOT)] = [f"0x{v:X}" for v in h]
    res = dict(
        files=len(srcs), compiled=len(objs), failed=fails,
        implemented=len(defs),
        duplicates={f"0x{a:X}": fs for a, fs in defs.items() if len(fs) > 1},
        unresolved_functions={reg: {f"0x{a:X}": n for a, n in sorted(d.items(), key=lambda x: -x[1])}
                              for reg, d in sorted(fn_unres.items())},
        globals=sorted(f"0x{a:X}" for k, a in refs if k == "gv"),
        vtables=sorted(f"0x{a:X}" for k, a in refs if k == "vt"),
        raw_addresses=raw,
        asm_addresses=asm,
        other_undefined=dict(other_undef.most_common(50)),
    )
    os.makedirs(os.path.dirname(OUT), exist_ok=True)
    json.dump(res, open(OUT, "w"), indent=1)
    return res


def summary(res):
    un = res["unresolved_functions"]
    lines = [f"compiled {res['compiled']}/{res['files']} files standalone; {res['implemented']} functions implemented"
             + (f"; {len(res['duplicates'])} DUPLICATE" if res["duplicates"] else "")]
    lines.append("unresolved functions: " + ", ".join(f"{reg} {len(d)}" for reg, d in un.items()) if un else
                 "unresolved functions: none")
    lines.append(f"data to generate: {len(res['globals'])} globals, {len(res['vtables'])} vtables; "
                 f"raw image addresses in code: {sum(len(v) for v in res['raw_addresses'].values())} in "
                 f"{len(res['raw_addresses'])} files; asm thunks with original addresses: {len(res.get('asm_addresses', {}))} "
                 f"files; other undefined symbols: {len(res['other_undefined'])}")
    if res["failed"]:
        lines.append(f"FAILED to compile standalone: {len(res['failed'])} files (--list fail)")
    return "\n".join(lines)


if __name__ == "__main__":
    ap = argparse.ArgumentParser()
    ap.add_argument("--list", help="game|rw|crt|...|raw|asm|dup|fail|other")
    ap.add_argument("-j", type=int, default=os.cpu_count())
    ap.add_argument("--cached", action="store_true", help="report the last run (work/linktest.json)")
    a = ap.parse_args()
    res = json.load(open(OUT)) if a.cached else run(a.j)
    print(summary(res))
    if a.list in ("raw", "asm"):
        for f, v in res["raw_addresses" if a.list == "raw" else "asm_addresses"].items():
            print(f"  {f}: {' '.join(v)}")
    elif a.list == "dup":
        for k, v in res["duplicates"].items():
            print(f"  {k}: {' '.join(v)}")
    elif a.list == "fail":
        for f, e in res["failed"].items():
            print(f"  {f}\n    " + (e or "").replace("\n", "\n    "))
    elif a.list == "other":
        for k, v in res["other_undefined"].items():
            print(f"  {v:4d}  {k}")
    elif a.list:
        for k, n in res["unresolved_functions"].get(a.list, {}).items():
            print(f"  {k}  used by {n} file(s)")
    sys.exit(1 if res["failed"] or res["duplicates"] else 0)
