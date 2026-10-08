#!/usr/bin/env python3
"""One name and one signature per original address (db/api.json), so the files of src/auto read as one codebase.

Every file binds its callees with CSF_FUNC(ret, conv, Name, 0xADDR, (params)). Grunts used to invent Name per
file (0x6B1380 was GetGlobalInt in one file, GetGlobalContext in another) and sometimes the signature too. Here:

  name       functions.csv name (done functions, IDA/RW/Opus names); else the most common name grunts gave it
             ("votes"), written back to functions.csv as name_src=vote while the row has no name
  signature  the implementing file's own definition (its ret N passed the gate); else the most common
             declaration whose stack purge matches the original
  abi        conflicts between declarations and the binary: a callee that reads ecx (this) or edx before writing
             it must be bound __fastcall (ecx, edx first); __cdecl/__stdcall callers would pass garbage

Commands:
  api.py build            rebuild db/api.json (+ votes into functions.csv)
  api.py fix FILE...       rename local CSF_FUNC names to the canonical ones (grunt's normalize() does this too)
  api.py check            report ABI conflicts (declarations vs the binary)
  api.py decl 0xADDR       the canonical CSF_FUNC line (packets show it for each callee)
"""
import collections, csv, glob, json, os, re, sys
sys.path.insert(0, os.path.dirname(__file__))

ROOT = os.path.dirname(os.path.dirname(os.path.abspath(__file__)))
API = os.path.join(ROOT, "db", "api.json")
FUNCS = os.path.join(ROOT, "db", "functions.csv")
DECL = re.compile(r"CSF_FUNC\(\s*([^;]+?)\s*,\s*(__\w+)\s*,\s*(\w+)\s*,\s*0[xX]([0-9A-Fa-f]+)\s*,\s*\((.*?)\)\s*\)\s*;", re.S)
HOOK = re.compile(r"HOOK\(\s*0[xX]([0-9A-Fa-f]+)\s*,\s*&?(\w+)")
CONVS = ("__cdecl", "__stdcall", "__fastcall", "__thiscall")


def ws(s):
    return re.sub(r"\s+", " ", s).strip()


def params_of(src, start):
    """Balanced (...) starting at src[start] == '('; returns (inner text, end index)."""
    depth, j = 0, start
    while j < len(src):
        depth += {"(": 1, ")": -1}.get(src[j], 0)
        j += 1
        if depth == 0:
            return src[start + 1:j - 1], j
    return None, start


def strip_comments(s):
    return re.sub(r"//[^\n]*|/\*.*?\*/", "", s, flags=re.S)


def impl_signature(src):
    """(ea, ret, conv, name, params) of the hooked function's definition, or None."""
    h = HOOK.search(src)
    if not h:
        return None
    ea, fn = int(h.group(1), 16), h.group(2)
    code = strip_comments(src)
    for m in re.finditer(rf"(?:^|[;}}\n])\s*((?:static\s+|inline\s+)*)([\w:<>\s\*&]+?)\s*\b{re.escape(fn)}\s*\(", code):
        head = ws(m.group(2))
        p, end = params_of(code, m.end() - 1)
        if p is None or not re.match(r"\s*\{", code[end:end + 40]):
            continue  # a declaration or a call, not the definition
        conv = next((c for c in CONVS if re.search(rf"\b{c}\b", head)), "__cdecl")
        ret = ws(re.sub(r"\b(__\w+|__declspec\([^)]*\))\b", "", head))
        if not ret or "naked" in head:
            return None
        return ea, ret, "__fastcall" if conv == "__thiscall" else conv, fn, ws(p)
    return None


def scan():
    """impls {ea: (ret, conv, name, params, file)}, decls {ea: [(ret, conv, name, params, file)]}"""
    impls, decls = {}, collections.defaultdict(list)
    for p in sorted(glob.glob(os.path.join(ROOT, "src", "auto", "*.cpp"))):
        src = open(p).read()
        rel = os.path.relpath(p, ROOT)
        s = impl_signature(src)
        if s:
            impls[s[0]] = (*s[1:], rel)
        for m in DECL.finditer(strip_comments(src)):
            decls[int(m.group(4), 16)].append((ws(m.group(1)), m.group(2), m.group(3), ws(m.group(5)), rel))
    return impls, decls


def stack_bytes(conv, params):
    from gate import stack_bytes as sb
    return sb(conv, params)


def nparams(params):
    p = [x for x in params.split(",") if x.strip() and x.strip() != "void"]
    return len(p)


def reads_before_write(ea, db, reg):
    """Does the function read `reg` (ecx/edx) before writing it? (straight-line scan from the entry; stops at the
    first branch/call). True = it takes an argument there."""
    from packet import disasm
    if ea not in db.F:
        return False
    for i in disasm(ea, min(db.F[ea]["size"], 400)):
        ops = i.op_str
        m = i.mnemonic
        if m.startswith("j") or m in ("call", "ret"):
            return False
        parts = [x.strip() for x in ops.split(",")]
        if not parts or not parts[0]:
            continue
        dst, srcs = parts[0], parts[1:]
        written = dst == reg and m in ("mov", "lea", "pop", "movzx", "movsx") or \
            (m == "xor" and len(parts) == 2 and parts[0] == parts[1] == reg)
        if m == "push":  # `push ecx` at entry is MSVC reserving a stack slot, not a use of the value
            continue
        if m == "lea" and srcs and srcs[0] == f"[{dst}]":  # `lea ecx, [ecx]`: alignment padding
            continue
        if m in ("xor", "sub") and len(parts) == 2 and parts[0] == parts[1] == reg:  # zeroing: a write
            return False
        read = any(re.search(rf"\b{reg}\b", s) for s in srcs) or (re.search(rf"\b{reg}\b", dst) and not written)
        if read:
            return True
        if written:
            return False
    return False


def norm_name(n):
    return n if re.fullmatch(r"[A-Za-z_]\w*", n or "") else None


def build(write_votes=True):
    from packet import DB, ret_bytes
    db = DB()
    impls, decls = scan()
    rows = list(csv.DictReader(open(FUNCS)))
    byea = {int(r["ea"], 16): r for r in rows}
    api, taken = {}, collections.Counter()
    votes_written = 0
    for ea in sorted(set(impls) | set(decls)):
        r = byea.get(ea)
        votes = collections.Counter(d[2] for d in decls.get(ea, []))
        name, src = (r["name"], r["name_src"] or "csv") if r and norm_name(r["name"]) else (None, None)
        if not name and votes:
            name, src = votes.most_common(1)[0][0], "vote"
            if write_votes and r is not None and not r["name"]:
                r["name"], r["name_src"] = name, "vote"
                votes_written += 1
        if not name and ea in impls:
            name, src = impls[ea][2], "impl"
        if not name:
            continue
        if ea in impls:
            ret, conv, _, params, f = impls[ea]
            sig = dict(ret=ret, conv=conv, params=params, sig_src="impl", file=f)
        else:
            rets = ret_bytes(ea, db.F[ea]["size"]) if ea in db.F else []
            ok = [d for d in decls[ea] if not rets or
                  (d[1] == "__cdecl" and rets == [0]) or (d[1] != "__cdecl" and stack_bytes(d[1], d[3]) in rets)]
            pool = ok or decls[ea]
            best = collections.Counter((d[0], d[1], d[3]) for d in pool).most_common(1)[0][0]
            sig = dict(ret=best[0], conv=best[1], params=best[2], sig_src="vote")
        api[ea] = dict(name=name, name_src=src, votes=dict(votes), **sig)
        taken[name] += 1
    # one name per address: a name two addresses share gets the address appended (both, so neither looks canonical)
    for ea, a in api.items():
        if taken[a["name"]] > 1:
            a["name"] = f"{a['name']}_{ea:X}"
    json.dump({f"{ea:08x}": a for ea, a in sorted(api.items())}, open(API, "w"), indent=1)
    if write_votes and votes_written:
        import fcntl  # work/db.lock: the captain writes functions.csv concurrently (as in globals.py)
        with open(os.path.join(ROOT, "work", "db.lock"), "w") as lk:
            fcntl.flock(lk, fcntl.LOCK_EX)
            cur = list(csv.DictReader(open(FUNCS)))
            for c in cur:
                e = int(c["ea"], 16)
                if not c["name"] and byea.get(e, {}).get("name_src") == "vote":
                    c["name"], c["name_src"] = byea[e]["name"], "vote"
            w = csv.DictWriter(open(FUNCS, "w", newline=""), fieldnames=list(cur[0].keys()), lineterminator="\n")
            w.writeheader()
            w.writerows(cur)
    return api, votes_written


_api = None


def load():
    global _api
    if _api is None:
        _api = {int(k, 16): v for k, v in json.load(open(API)).items()} if os.path.exists(API) else {}
    return _api


def decl_line(ea):
    a = load().get(ea)
    if not a:
        return None
    return f"CSF_FUNC({a['ret']}, {a['conv']}, {a['name']}, 0x{ea:08X}, ({a['params']}));"


def canonicalize(src):
    """Rename each CSF_FUNC binding (and its uses) to the canonical name of its address. Signatures are left
    alone: a different parameter type may be the caller's legitimate view; ABI conflicts are check()'s job."""
    api = load()
    own = HOOK.search(src)
    for m in list(DECL.finditer(src)):
        ea, local = int(m.group(4), 16), m.group(3)
        a = api.get(ea)
        if not a or a["name"] == local:
            continue
        want = a["name"]
        if re.search(rf"\b{re.escape(want)}\b", src) or (own and own.group(2) == want):
            continue  # the canonical name is taken in this file (its own function, another binding): keep
        src = re.sub(rf"\b{re.escape(local)}\b", want, src)
    return src


def binding_errors(src, db):
    """Gate: a CSF_FUNC binding of the file's own address, or of a callee that takes ecx (this) bound without it."""
    errs = []
    own = HOOK.search(src)
    for m in DECL.finditer(strip_comments(src)):
        ea, conv, name = int(m.group(4), 16), m.group(2), m.group(3)
        if own and ea == int(own.group(1), 16):
            errs.append(f"CSF_FUNC {name} binds this file's own address 0x{ea:X}: for recursion call your function "
                        f"{own.group(2)} directly (declare it ahead)")
        elif conv != "__fastcall" and reads_before_write(ea, db, "ecx"):
            errs.append(f"CSF_FUNC {name}: 0x{ea:X} reads ecx (`this`) on entry, so bind it __fastcall "
                        f"(void* self, void* edx, ...stack args) and pass the right object, not {conv}")
    return errs


def check():
    from packet import DB
    db = DB()
    impls, decls = scan()
    out = []
    for ea, ds in sorted(decls.items()):
        need_ecx = reads_before_write(ea, db, "ecx")
        need_edx = reads_before_write(ea, db, "edx")
        for ret, conv, name, params, f in ds:
            n = nparams(params)
            if need_ecx and conv != "__fastcall":
                out.append((ea, f, f"{name}: 0x{ea:X} reads ecx (this) on entry but is bound {conv}"))
            elif need_edx and not (conv == "__fastcall" and n >= 2):
                out.append((ea, f, f"{name}: 0x{ea:X} reads edx on entry but the binding passes none"))
        if ea in impls:  # ABI-level only: what the callee pops
            iret, iconv, _, iparams, _ = impls[ea]
            ipop = 0 if iconv == "__cdecl" else stack_bytes(iconv, iparams)
            for ret, conv, name, params, f in ds:
                pop = 0 if conv == "__cdecl" else stack_bytes(conv, params)
                if pop != ipop:
                    out.append((ea, f, f"{name}: bound {conv} ({params}) but the implementation is {iconv} ({iparams})"))
    return out


if __name__ == "__main__":
    cmd = sys.argv[1] if len(sys.argv) > 1 else "build"
    if cmd == "build":
        api, n = build()
        srcs = collections.Counter(a["sig_src"] for a in api.values())
        print(f"db/api.json: {len(api)} addresses ({dict(srcs)}); {n} vote names written to functions.csv")
    elif cmd == "fix":
        n = 0
        for p in sys.argv[2:]:
            s = open(p).read()
            t = canonicalize(s)
            if t != s:
                open(p, "w").write(t)
                n += 1
        print(f"{n} file(s) renamed")
    elif cmd == "check":
        res = check()
        for ea, f, msg in res:
            print(f"{f}: {msg}")
        print(f"{len(res)} ABI conflict(s)")
        sys.exit(1 if res else 0)
    elif cmd == "decl":
        print(decl_line(int(sys.argv[2], 16)) or "unknown")
