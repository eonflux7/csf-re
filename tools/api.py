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
HOOK = re.compile(r"HOOK(?:_KEEP_ECX_CDECL|_KEEP_ECX|_REGS|_TAIL)?\(\s*0[xX]([0-9A-Fa-f]+)\s*,\s*&?(\w+)")
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
    if not h or h.group(0).startswith(("HOOK_TAIL", "HOOK_REGS")):  # a tail thunk's impl has the helper's signature, not its own
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
    """Does the function read `reg` (ecx/edx) before writing it? True = it takes an argument there. Scans from the
    entry; a call or ret ends a path without a read. Branches are followed (a few levels, inside the function) and
    every path must read the register first: 0x46BE40 tests a stack argument, branches, and reads ecx on both
    sides; its caller bound it __stdcall and passed garbage (2026-10-11)."""
    from packet import disasm, callable_shared_chunk
    f = db.F.get(ea) or callable_shared_chunk(ea, db)
    if not f:
        return False
    ins = disasm(ea, min(f["size"], 400))
    at = {i.address: k for k, i in enumerate(ins)}

    aliases = {reg: 0xFFFFFFFF, reg[1:]: 0xFFFF}
    if reg in ("eax", "ebx", "ecx", "edx"):
        aliases.update({reg[1] + "l": 0xFF, reg[1] + "h": 0xFF00})

    def read_mask(operand):
        return sum(mask for name, mask in aliases.items() if re.search(rf"\b{name}\b", operand))

    def pushed_mask(k):
        """The low bits consumed by a typed stack argument, despite a whole-register push. An untyped or
        interrupted push sequence stays a full-width read (partial writes must not erase a live pointer).
        """
        index = 0
        for j in ins[k:k + 8]:
            if j.mnemonic == "push":
                index += 1
            elif j.mnemonic == "call" and re.fullmatch(r"0x[0-9a-f]+", j.op_str):
                a = ida_abi().get(int(j.op_str, 16), {})
                args = [t for t, _, loc in a.get("args", []) if loc == "stack"]
                if index < len(args):
                    t = c_type(args[index])
                    if t in ("int16_t", "uint16_t", "short", "unsigned short"):
                        return 0xFFFF
                    if t in ("int8_t", "uint8_t", "char", "unsigned char", "bool"):
                        return 0xFF
                break
            elif j.mnemonic.startswith("j") or j.mnemonic in ("call", "ret", "pop") or \
                    j.op_str.startswith("esp,"):
                break
        return 0xFFFFFFFF

    def walk(k, prologue, depth, live=0xFFFFFFFF):
        # prologue: MSVC frame/SEH setup and register saves: a `push ecx` there reserves a stack slot
        while k < len(ins):
            i = ins[k]
            k += 1
            ops = i.op_str
            m = i.mnemonic
            if m in ("call", "ret"):
                return False
            if m.startswith("j"):
                t = int(ops, 16) if re.fullmatch(r"0x[0-9a-f]+", ops) else None
                if t not in at or depth <= 0:
                    return False
                if m == "jmp":
                    k, depth, prologue = at[t], depth - 1, False
                    continue
                return walk(k, False, depth - 1, live) and walk(at[t], False, depth - 1, live)
            parts = [x.strip() for x in ops.split(",")]
            if not parts or not parts[0]:
                continue
            dst, srcs = parts[0], parts[1:]
            written = dst in aliases and m in ("mov", "lea", "pop", "movzx", "movsx")
            # Constant-producing arithmetic does not consume the old destination bits.
            constant = len(parts) == 2 and dst in aliases and (
                m in ("xor", "sub") and dst == parts[1] or
                m == "or" and int_literal(parts[1]) == aliases[dst] or
                m == "and" and int_literal(parts[1]) == 0)
            written |= constant
            if m == "push" and prologue and (dst in ("ebp", "ebx", "esi", "edi", "eax", "ecx") or dst.startswith(("0x", "-"))):
                continue  # `push ecx` in the prologue = `sub esp, 4`; after it (0x431CF0, 0x544390) it passes `this` on
            if prologue and (m == "mov" and ops in ("ebp, esp", "eax, dword ptr fs:[0]", "dword ptr fs:[0], esp")
                             or m == "sub" and dst == "esp"):
                continue
            if m == "push" and dst == reg:
                # `push ecx; ...; fstp dword ptr [esp]`: room for a float argument, the pushed value is overwritten
                # (0x439B20: both callers escalated on "reads ecx" against "pops nothing")
                # `push ecx; ..; mov eax, esp; push ..; mov dword ptr [eax], 0`: a temporary whose address goes to
                # the callee, written through the copy of esp (0x64D6B0: its callers were bound __fastcall without
                # their two stack arguments)
                room, alias = False, None
                for j in ins[k:k + 6]:
                    jd = j.op_str.split(",")[0].strip()
                    if alias and j.mnemonic == "mov" and jd == f"dword ptr [{alias}]":
                        room = True
                        break
                    if j.mnemonic in ("call", "pop", "ret") or j.mnemonic.startswith("j") or (j.mnemonic == "push" and not alias):
                        break
                    if j.mnemonic == "mov" and j.op_str.replace(" ", "").endswith(",esp") and alias is None:
                        alias = jd
                        continue
                    if alias and re.search(rf"\b{alias}\b", jd) and j.mnemonic != "push":
                        break  # the copy is overwritten
                    if j.mnemonic in ("fstp", "fst", "mov") and jd == "dword ptr [esp]" and not alias:
                        room = True
                        break
                if room:
                    prologue = False
                    continue
            prologue = False
            if m == "push":
                if read_mask(dst) & live & pushed_mask(k):
                    return True
                continue
            if m == "lea" and srcs and srcs[0] == f"[{dst}]":  # `lea ecx, [ecx]`: alignment padding
                continue
            read = 0 if constant else (read_mask(", ".join(srcs)) | (read_mask(dst) if not written else 0))
            if read & live:
                return True
            if written:
                live &= ~aliases[dst]
                if not live:
                    return False
        return False

    return walk(0, True, 3)


def int_literal(s):
    try:
        return int(s, 0) & 0xFFFFFFFF
    except ValueError:
        return None


SUBREG = {"eax": r"e?ax|al|ah", "ebx": r"e?bx|bl|bh", "esi": r"e?si", "edi": r"e?di"}


def reg_args(ea, db):
    """Registers among eax/ebx/esi/edi the function reads before it writes them: arguments no calling convention
    names (LTCG usercall; zlib's 0x6A46A0 takes its state in esi, 0x6A33A0 in eax with the tree in edi). A push is a
    save, not a read; a call or ret ends a path; every path must read the register (as reads_before_write)."""
    from packet import disasm, callable_shared_chunk
    f = db.F.get(ea) or callable_shared_chunk(ea, db)
    if not f:
        return set()
    ins = disasm(ea, min(f["size"], 400))
    at = {i.address: k for k, i in enumerate(ins)}

    def walk(reg, k, depth):
        rx = re.compile(rf"\b(?:{SUBREG[reg]})\b")
        while k < len(ins):
            i = ins[k]
            k += 1
            m, ops = i.mnemonic, i.op_str
            if m in ("call", "ret", "push", "pop", "nop") or m.startswith("rep"):
                if m in ("call", "ret") or (m == "pop" and rx.search(ops)):
                    return False
                if m.startswith("rep"):  # string instructions: movs reads esi and edi, stos/scas edi and eax, lods esi
                    uses = {"movs": ("esi", "edi"), "cmps": ("esi", "edi"), "stos": ("edi", "eax"),
                            "scas": ("edi", "eax"), "lods": ("esi",)}
                    if any(k in m + " " + ops for k, v in uses.items() if reg in v):
                        return True
                continue
            if m.startswith("j"):
                t = int(ops, 16) if re.fullmatch(r"0x[0-9a-f]+", ops) else None
                if t not in at or depth <= 0:
                    return False
                if m == "jmp":
                    k, depth = at[t], depth - 1
                    continue
                return walk(reg, k, depth - 1) and walk(reg, at[t], depth - 1)
            parts = [x.strip() for x in ops.split(",")]
            if not parts or not parts[0]:
                continue
            dst, srcs = parts[0], parts[1:]
            if m in ("xor", "sub", "sbb") and len(parts) == 2 and parts[0] == parts[1]:
                if rx.fullmatch(dst):
                    return False
                continue
            if m == "lea" and srcs and srcs[0] == f"[{dst}]" or m == "mov" and srcs == [dst]:  # padding
                continue
            if m[:4] in ("stos", "scas", "movs", "cmps", "lods") and len(m) == 5 and m[4] in "bwd":
                if reg in {"movs": ("esi", "edi"), "cmps": ("esi", "edi"), "stos": ("edi", "eax"),
                           "scas": ("edi", "eax"), "lods": ("esi",)}[m[:4]]:
                    return True
                continue
            if any(rx.search(x) for x in srcs) or ("[" in dst and rx.search(dst)):
                return True
            if rx.fullmatch(dst):
                # a write of the register or of a part of it (mov al, ..; fnstsw ax; or eax, -1) ends the path:
                # what the function reads afterwards is its own value
                if m in ("mov", "lea", "movzx", "movsx", "fnstsw", "lahf", "imul") and len(parts) != 2 or \
                        m in ("mov", "lea", "movzx", "movsx") or m.startswith("set") or m == "fnstsw" or \
                        (m == "or" and srcs == ["0xffffffff"]) or (m == "and" and srcs == ["0"]):
                    return False
                return True  # add/and/cmp/test/inc ...: reads it
        return False

    return {r for r in SUBREG if walk(r, 0, 3)}


def norm_name(n):
    return n if re.fullmatch(r"[A-Za-z_]\w*", n or "") and not ADDR_NAME.fullmatch(n) else None


ADDR_NAME = re.compile(r"sub_[0-9A-Fa-f]{6,8}")  # the placeholder of an unnamed address (name_src addr)

# Hex-Rays type -> C type for a binding. Pointers to anything but scalars become void* (classes are declared in
# types/auto_classes.h; a binding must not depend on header order).
SCALAR = {
    "int": "int", "signed int": "int", "BOOL": "int", "_BOOL4": "int", "LONG": "int", "HRESULT": "int",
    "unsigned int": "unsigned int", "_DWORD": "uint32_t", "DWORD": "uint32_t", "unsigned __int32": "uint32_t",
    "__int32": "int", "size_t": "uint32_t", "UINT": "unsigned int",
    "__int16": "int16_t", "short": "int16_t", "signed __int16": "int16_t",
    "unsigned __int16": "uint16_t", "_WORD": "uint16_t", "WORD": "uint16_t", "unsigned short": "uint16_t",
    "char": "char", "signed __int8": "int8_t", "__int8": "int8_t",
    "unsigned __int8": "uint8_t", "_BYTE": "uint8_t", "BYTE": "uint8_t", "unsigned char": "uint8_t",
    "bool": "bool", "_BOOL1": "bool", "float": "float", "double": "double", "long double": "double",
    "__int64": "int64_t", "unsigned __int64": "uint64_t", "_QWORD": "uint64_t", "void": "void",
    "wchar_t": "wchar_t",
}
WIDE = {"double", "int64_t", "uint64_t"}  # 8 stack bytes


def c_type(t):
    """C spelling of a Hex-Rays type, or None (by-value struct, function pointer, unknown scalar)."""
    t = re.sub(r"\b(struct|union|enum)\s+", "", t.replace("const ", "").strip())
    stars = t.count("*")
    base = t.replace("*", "").strip()
    if "(" in t or "[" in t or "@" in t:
        return None
    if stars:
        b = SCALAR.get(base)
        if b and b != "void" and stars == 1:
            return b + "*"
        return "void" + "*" * stars
    return SCALAR.get(base)


_abi = None


def ida_abi():
    """export/abi.json (tools/ida/export_abi.py): Hex-Rays' prototype of every function (calling convention,
    argument types and locations) and the argument count at each call site it decompiled."""
    global _abi
    if _abi is None:
        p = os.path.join(ROOT, "export", "abi.json")
        _abi = {int(k, 16): v for k, v in json.load(open(p)).items()} if os.path.exists(p) else {}
        callers = collections.defaultdict(list)
        for v in _abi.values():
            for c, ns in (v.get("calls") or {}).items():
                callers[int(c, 16)] += ns
        for c, ns in callers.items():
            if c in _abi:
                _abi[c]["site_nargs"] = ns
    return _abi


def thunk_target(ea, db):
    """A lone `jmp target` thunk has its target's ABI: follow the chain. IDA may type the thunk itself from a
    library signature (NetZ 0x7A3190 `thiscall` -> 0x753410 `xor al, al; ret 4`, no `this`)."""
    from packet import disasm
    for _ in range(4):
        ins = disasm(ea, db.F[ea]["size"]) if ea in db.F else []
        if len(ins) == 1 and ins[0].mnemonic == "jmp" and ins[0].op_str.startswith("0x"):
            ea = int(ins[0].op_str, 16)
        else:
            break
    return ea


def takes_reg(ea, db, reg):
    """Does the function take an argument in ECX/EDX? Retain Hex-Rays locations unless
    bit-level original control flow proves that no incoming bit is consumed. An
    ambiguous proof is NOT a spurious argument. Also retain exact entry reads
    missing from Hex-Rays (notably forwarding thunks' receiver adjustments).
    """
    ea = thunk_target(ea, db)
    a = ida_abi().get(ea)
    if a and a.get("cc") and any(loc == reg for _, _, loc in a.get("args", [])):
        from regargs import entry_register_reads
        if entry_register_reads(db, ea, reg) != 0:
            return True
    # an exact read in the entry block (no branch before it) is a fact even where Hex-Rays says no: thunks like
    # `mov ecx, [ecx+0x30]; jmp target` get the target's prototype from Hex-Rays and lose their `this`
    return reads_before_write(ea, db, reg)


_spurious = None


def spurious_regs(ea):
    """Registers Hex-Rays makes arguments of ea that the binary proves are not (db/regargs.json, tools/regargs.py:
    a callee-save `push edi` it took for a read, spread to callers)."""
    global _spurious
    if _spurious is None:
        p = os.path.join(ROOT, "db", "regargs.json")
        _spurious = {int(k, 16): v["spurious"] for k, v in json.load(open(p)).items()} if os.path.exists(p) else {}
    return _spurious.get(ea, [])


def real_regs(ea, db=None):
    """Registers besides ecx/edx the original takes an argument in: ebx/esi/edi proven by tools/regargs.py
    (db/regargs.json "real"), eax by reg_args' read-before-write scan. Such a function needs HOOK_REGS (or
    regparm when it is eax alone); a plain convention reads garbage (zlib's deflate half, 2026-10-11)."""
    global _real
    if _real is None:
        p = os.path.join(ROOT, "db", "regargs.json")
        _real = {int(k, 16): v.get("real", []) for k, v in json.load(open(p)).items()} if os.path.exists(p) else {}
    regs = set(_real.get(ea, []))
    if db is not None and ea not in db.F:
        # Shared chunks have no CSV/regargs row. A stack-balanced entry can still require LTCG registers;
        # admitting its address must not bypass the gate's call_regs requirement.
        regs.update(reg_args(ea, db))
    # game code only: CRT / library entry points are asm with their own rules (___CxxFrameHandler takes eax)
    if db is not None and db.rows.get(ea, {}).get("region") == "game" and "eax" in reg_args(ea, db):
        regs.add("eax")
    return sorted(regs)


_real = None


def without_spurious(ea, a):
    """Hex-Rays' prototype record minus its proven-spurious register arguments; a usercall / userpurge left with
    ecx only becomes thiscall, with none cdecl / stdcall (the `ret N` check in abi_sig decides which is right)."""
    sp = spurious_regs(ea)
    if not a or not sp:
        return a
    args = [x for x in a.get("args", []) if x[2] not in sp]
    a = dict(a, args=args)
    if a.get("cc") in ("usercall", "userpurge"):
        regs = [loc for _, _, loc in args if loc != "stack"]
        if not regs:
            a["cc"] = "stdcall" if a["cc"] == "userpurge" and args else "cdecl"
        elif regs == ["ecx"] and args[0][2] == "ecx":
            a["cc"] = "thiscall"
        elif regs == ["ecx", "edx"] and [x[2] for x in args[:2]] == ["ecx", "edx"]:
            a["cc"] = "fastcall"
    return a


def abi_sig(ea, db):
    """Signature of an address from Hex-Rays' prototype (export/abi.json), refined by exact binary facts:
      - cdecl argument count: the callers' cleanup (`add esp, N` right after the call) when the call sites agree,
        else the most arguments Hex-Rays saw pushed at any call site (its own prototype drops unused ones);
      - callee-popped conventions: the argument bytes must equal the original's `ret N`.
    __thiscall becomes __fastcall (self, edx dummy, ...) as CSF_FUNC binds it. None when it can't be bound from C
    (other register arguments, by-value structs) or Hex-Rays disagrees with the binary. Register arguments the binary
    proves spurious (db/regargs.json) are dropped first."""
    from packet import ret_bytes
    ea = thunk_target(ea, db)
    a = without_spurious(ea, ida_abi().get(ea))
    if not a or not a.get("cc") or ea not in db.F:
        return None
    # Revalidate ECX/EDX locations too: regargs.json covers only the nonstandard
    # callee-saved arguments, not stale Hex-Rays fastcall register locations.
    from regargs import entry_register_reads
    dropped = {loc for _, _, loc in a.get('args', []) if loc in ('ecx', 'edx')
               and entry_register_reads(db, ea, loc) == 0}
    if dropped:
        a = dict(a, args=[x for x in a['args'] if x[2] not in dropped])
        regs = [loc for _, _, loc in a['args'] if loc != 'stack']
        if not regs and a['cc'] in ('thiscall', 'fastcall'):
            a['cc'] = 'stdcall' if a.get('stk') else 'cdecl'
    cc = a["cc"]
    if cc not in ("cdecl", "stdcall", "thiscall", "fastcall", "ellipsis"):
        return None
    ret = c_type(a["ret"])
    if ret is None:
        return None
    # Hex-Rays types a forwarder void when its only statement is a call (0x6C2040: `push [ecx]; call; add esp, 4;
    # ret`), yet a caller compares against the eax it leaves (0x6E9D00 wrote the key as 0: 200/200 in-game
    # mismatches). A caller that provably reads eax after the call makes it an int return.
    if ret == "void" and db is not None and ret_unused(ea, db) is False:
        ret = "int"
    want = {"thiscall": ["ecx"], "fastcall": ["ecx", "edx"]}.get(cc, [])
    params, stack, regs = [], 0, []
    for t0, n, loc in a["args"]:
        t = c_type(t0)
        if t is None or t == "void":
            return None
        n = "self" if n == "this" else (n or f"a{len(params) + 1}")
        if loc == "stack":
            stack += 8 if t in WIDE else 4
        elif loc in ("ecx", "edx"):
            regs.append(loc)
        else:
            return None
        params.append((t, n, loc))
    if regs != want[:len(regs)]:
        return None  # register arguments out of the convention's order: a usercall in disguise
    rets = ret_bytes(ea, db.F[ea]["size"])
    callee_pops = cc in ("stdcall", "thiscall", "fastcall")
    if rets and (not callee_pops and set(rets) != {0} or callee_pops and set(rets) != {stack}):
        return None
    if cc == "cdecl" and all(t not in WIDE for t, _, _ in params):
        n = caller_cleanup(db).get(ea)
        if n is not None:
            n //= 4
        else:  # Hex-Rays' count at the call sites: the majority, never one site (a misread caller says 7 for 0)
            sites = collections.Counter(a.get("site_nargs", []))
            top = sites.most_common(1)
            n = top[0][0] if top and top[0][1] * 2 > sum(sites.values()) else len(params)
        if n > len(params):
            params += [("int", f"a{i + 1}", "stack") for i in range(len(params), n)]
    out = [f"{t} {n}" for t, n, _ in params]
    if cc == "thiscall":
        out = ["void* self", "void* edx"] + out[1:]
        cc = "fastcall"
    elif cc == "fastcall" and len(regs) == 1 and len(out) > 1:
        out = out[:1] + ["void* edx"] + out[1:]  # one register argument: the edx slot is unused
    elif cc == "ellipsis":
        out.append("...")
        cc = "cdecl"
    elif cc == "stdcall" and not params:
        cc = "cdecl"  # nothing to pop: the same ABI, spelled as the grunts and the gate expect
    if cc in ("stdcall", "cdecl") and (cc == "stdcall" or not params) and reads_before_write(ea, db, "ecx"):
        out = ["void* self", "void* edx"] + out  # see takes_reg: a thunk reading `this` before its jump
        cc = "fastcall"
    return dict(ret=ret, conv="__" + cc, params=", ".join(out), sig_src="abi")


_cleanup = None


def caller_cleanup(db=None):
    """ea -> stack bytes its callers remove right after `call ea` (export/abi.json `cleanup`: `add esp, N`, a
    `pop ecx` = 4); the smallest seen, since MSVC may merge the cleanup of consecutive calls into one larger add.
    Sites without a cleanup right after the call (deferred, or the epilogue's mov esp, ebp) say nothing; trusted
    when at least half the sites clean up explicitly. Only cdecl callees use it."""
    global _cleanup
    if _cleanup is None:
        seen = collections.defaultdict(list)
        for v in ida_abi().values():
            for c, ns in (v.get("cleanup") or {}).items():
                seen[int(c, 16)] += ns
        _cleanup = {t: min(x for x in v if x) for t, v in seen.items() if sum(1 for x in v if x) * 2 >= len(v)}
    return _cleanup


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
        votes = collections.Counter(d[2] for d in decls.get(ea, []) if norm_name(d[2]))
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
    # every other function: its binary-checked Hex-Rays signature (abi_sig), so each callee in a packet comes with
    # a `bind as` line; unnamed ones carry the sub_ placeholder (name_src addr: never forced onto a file's own name)
    n_abi = 0
    for ea in sorted(db.F):
        if ea in api:
            continue
        sig = abi_sig(ea, db)
        if not sig:
            continue
        r = byea.get(ea)
        name, src = (r["name"], r["name_src"] or "csv") if r and norm_name(r["name"]) else (f"sub_{ea:X}", "addr")
        api[ea] = dict(name=name, name_src=src, votes={}, **sig)
        taken[name] += 1
        n_abi += 1
    # one name per address: a name two addresses share gets the address appended (both, so neither looks canonical)
    for ea, a in api.items():
        if taken[a["name"]] > 1 and a["name_src"] != "addr":
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
    print(f"api: {len(api)} addresses, {n_abi} with a signature from the binary only (sig_src abi)")
    return api, votes_written


_api = None


def load():
    global _api
    if _api is None:
        p = API
        _api = {int(k, 16): v for k, v in json.load(open(p)).items()} if os.path.exists(p) else {}
    return _api


def decl_line(ea):
    a = load().get(ea)
    if not a:
        return None
    return f"CSF_FUNC({a['ret']}, {a['conv']}, {a['name']}, 0x{ea:08X}, ({a['params']}));"


# x87 instructions that leave a result in st0 (a `ret` right after one returns it)
X87_RESULT = re.compile(r"f(i?ld\w*|add|sub|subr|mul|div|divr|i?addp?|i?subr?p?|i?mulp?|i?divr?p?|chs|abs|sqrt|"
                        r"st|xch|rndint|scale|sin|cos|patan|yl2x)$")
FLOAT_RET = re.compile(r"^(float|double|long double)$")


_fpret = None


def fpret():
    global _fpret
    if _fpret is None:
        p = os.path.join(ROOT, "db", "fpret.json")
        _fpret = json.load(open(p)) if os.path.exists(p) else {}
    return _fpret


def returns_fpu(ea, db, depth=3, binary=True):
    """Does the function return its value in st0? Hex-Rays' own prototype of it (not a call site's view) says
    float/double, or a `ret` path ends in an x87 result or in a call / tail jump to such a function (only stack
    cleanup in between). 0x42D3C0 returns 0x70DDF0's `fld [0x82D4CC]; ret` (the shared `return 0.0f` stub): Hex-Rays
    typed that call `int` there, the grunt returned eax, and the diff saw 200/200 mismatches on a verified function."""
    from packet import disasm
    ea = thunk_target(ea, db)
    if binary and f"{ea:08X}" in fpret():  # decided from the x87 stack depth at every ret (tools/fpret.py)
        return bool(fpret()[f"{ea:08X}"])
    a = ida_abi().get(ea)
    if a and FLOAT_RET.match(c_type(a.get("ret") or "") or ""):
        return True
    if depth == 0 or ea not in db.F:
        return False
    ins = disasm(ea, db.F[ea]["size"])
    for k, i in enumerate(ins):
        if i.mnemonic != "ret" and not (i.mnemonic == "jmp" and i.op_str.startswith("0x")
                                        and not db.F[ea]["ea"] <= int(i.op_str, 16) < db.F[ea]["end"]):
            continue
        if i.mnemonic == "jmp":
            if returns_fpu(int(i.op_str, 16), db, depth - 1, binary):
                return True
            continue
        j = k - 1
        while j >= 0 and (ins[j].mnemonic in ("pop", "leave") or re.match(r"(add|sub) esp,|mov esp, ebp",
                                                                            f"{ins[j].mnemonic} {ins[j].op_str}")):
            j -= 1
        if j < 0:
            continue
        p = ins[j]
        if X87_RESULT.match(p.mnemonic):
            return True
        if p.mnemonic == "call" and p.op_str.startswith("0x") and returns_fpu(int(p.op_str, 16), db, depth - 1, binary):
            return True
    return False


_callers = None
EAX_RE = re.compile(r"\b(eax|ax|al|ah)\b")


def callers_of(ea, db):
    """Functions with a direct call to ea (export/functions.jsonl callees)."""
    global _callers
    if _callers is None:
        _callers = collections.defaultdict(list)
        for c, f in db.F.items():
            for t in f.get("callees") or []:
                _callers[t].append(c)
    return _callers.get(ea, [])


_image = None


def address_taken(ea):
    """The function's absolute address appears anywhere in the exe image (vtables, command / callback tables,
    `push offset`, `mov [x], offset`): it has callers the direct-call list doesn't show."""
    global _image
    if _image is None:
        import pefile
        from packet import ROOT as _R
        _image = pefile.PE(os.path.join(_R, "work", "pc", "CommXPC.exe"), fast_load=True).get_memory_mapped_image()
    return _image.find(ea.to_bytes(4, "little")) != -1


def _eax_dead_after(fn, ins, idx, start, db, depth, budget=200):
    """Every path from instruction index `start` writes eax (or calls) before reading it. A `ret` hands eax to the
    function's own callers (ret_unused of it, depth-limited); a jump out of the function is a tail call: unknown."""
    seen, work = set(), [start]
    while work:
        k = work.pop()
        while True:
            if k in seen:
                break
            seen.add(k)
            budget -= 1
            if budget < 0 or k >= len(ins):
                return False
            i = ins[k]
            m, ops = i.mnemonic, i.op_str
            dst, _, srcop = (x.strip() for x in ops.partition(","))
            if m == "call":
                break
            if m == "ret" or m == "jmp" and ops.startswith("0x") and int(ops, 16) not in idx:
                if not _returns_nothing(fn, db, depth):
                    return False  # (a tail jump: eax is the target's, or ours passed through a void target)
                break
            if m.startswith("j"):
                if EAX_RE.search(ops) or not ops.startswith("0x"):
                    return False  # jmp eax / jump table
                t = int(ops, 16)
                if t not in idx:
                    return False  # conditional tail jump
                work.append(idx[t])
                if m == "jmp":
                    break
                k += 1
                continue
            if m in ("xor", "sub") and dst == srcop and EAX_RE.fullmatch(dst):
                break
            if m in ("mov", "lea", "movzx", "movsx", "pop", "setne", "sete") and EAX_RE.fullmatch(dst) \
                    and not EAX_RE.search(srcop):
                break  # (a write to al/ax: the caller only ever looks at that part)
            if m.startswith("set") and EAX_RE.fullmatch(dst):
                break
            if EAX_RE.search(ops) or m in ("cdq", "cwde", "stosd", "stosb", "stosw", "cmpxchg"):
                return False  # reads eax (implicitly for the string/convert ops)
            k += 1
    return True


def eax_undefined_on_return(ea, db, budget=2000):
    """Some path from the entry reaches a `ret` without writing eax (no eax/ax/al destination, no call): the
    original's return value is then whatever the caller left in eax, so a diff can't compare it (MSVC void
    std::string _Tidy 0x544310: `jb` past the free straight to `ret`, Hex-Rays still says int)."""
    from packet import disasm
    if ea not in db.F:
        return False
    ins = disasm(ea, db.F[ea]["size"])
    if not ins:
        return False
    idx = {i.address: k for k, i in enumerate(ins)}
    seen, work = set(), [0]
    while work:
        k = work.pop()
        while k < len(ins) and k not in seen:
            seen.add(k)
            budget -= 1
            if budget < 0:
                return False
            i = ins[k]
            m, ops = i.mnemonic, i.op_str
            dst = ops.partition(",")[0].strip()
            if m == "call" or EAX_RE.fullmatch(dst) and m not in ("cmp", "test", "push") \
                    or m in ("cdq", "cwde", "lodsd", "lodsb", "lodsw"):
                break  # eax defined on this path
            if m == "ret":
                return True
            if m.startswith("j"):
                if not ops.startswith("0x") or int(ops, 16) not in idx:
                    break  # tail jump: the target defines the result
                work.append(idx[int(ops, 16)])
                if m == "jmp":
                    break
            k += 1
    return False


def _returns_nothing(fn, db, depth):
    """fn's eax is never looked at by its callers: Hex-Rays' own prototype of fn is void (virtual methods: callers
    unknown), else ret_unused of fn (depth-limited)."""
    a = ida_abi().get(fn)
    if a and c_type(a.get("ret") or "") == "void":
        return True
    return depth > 0 and bool(ret_unused(fn, db, depth - 1))


def ret_unused(ea, db, depth=2):
    """True when no caller reads eax after calling ea: on every path from every direct call site eax is written (or
    clobbered by another call) before any read; a caller returning right after passes the question to its own
    callers. None when that can't be shown (no direct callers, address taken by a vtable, a tail jump, depth).
    Hex-Rays types a void function int when its last call's result happens to sit in eax (MSVC std::string _Tidy
    0x544310 / 0x727250: `return free(p)` on one path, leftover eax on the other) and the diff compares garbage."""
    from packet import disasm
    key = (ea, depth)
    if key in _ret_unused_memo:
        return _ret_unused_memo[key]
    _ret_unused_memo[key] = None  # recursion guard
    res = None
    cs = callers_of(ea, db) if ea not in db.owner and not address_taken(ea) else []
    if cs:
        res = True
        for c in cs:
            ins = disasm(c, db.F[c]["size"])
            idx = {i.address: k for k, i in enumerate(ins)}
            sites = [k for k, i in enumerate(ins) if i.mnemonic in ("call", "jmp") and i.op_str == f"0x{ea:x}"]
            for k in sites:
                if ins[k].mnemonic == "jmp":
                    ok = _returns_nothing(c, db, depth)
                else:
                    ok = _eax_dead_after(c, ins, idx, k + 1, db, depth)
                if not ok:
                    res = None
                    break
            if not sites or res is None:
                res = None
                break
    _ret_unused_memo[key] = res
    return res


_ret_unused_memo = {}


def float_ret_errors(src, db):
    """Gate: the file's own function and every CSF_FUNC binding must return float/double where the original returns
    in st0. An int there reads eax (garbage) and leaves the caller's x87 stack one short."""
    errs = []
    code = strip_comments(src)
    for m in DECL.finditer(code):
        ret, ea, name = ws(m.group(1)), int(m.group(4), 16), m.group(3)
        if ea in db.F and not FLOAT_RET.match(ret) and returns_fpu(ea, db):
            errs.append(f"CSF_FUNC {name}: 0x{ea:X} returns its value in st0 (x87), so bind it returning float "
                        f"(or double), not {ret}")
    # The other way round, where the binary decides it (db/fpret.json): a float return the original doesn't make
    # leaves one x87 entry behind per call, and the original's float code overflows the stack a few calls later.
    for m in DECL.finditer(code):
        ret, ea, name = ws(m.group(1)), int(m.group(4), 16), m.group(3)
        if FLOAT_RET.match(ret) and fpret().get(f"{thunk_target(ea, db):08X}") == 0:
            errs.append(f"CSF_FUNC {name}: 0x{ea:X} returns nothing in st0 (no path leaves a value on the x87 "
                        f"stack, whatever the decompiler's prototype says), so bind it returning void or int, not {ret}")
    sig = impl_signature(src)
    if sig and FLOAT_RET.match(sig[1]) and fpret().get(f"{sig[0]:08X}") == 0:
        errs.append(f"{sig[3]} must not return {sig[1]}: the original 0x{sig[0]:X} leaves nothing on the x87 stack "
                    f"on any path (the decompiler's float return type is wrong); return void, or int if eax is set")
    if sig and sig[0] in db.F and not FLOAT_RET.match(sig[1]) and returns_fpu(sig[0], db):
        errs.append(f"{sig[3]} must return float (or double): the original 0x{sig[0]:X} returns its value in st0 "
                    f"(x87), yours returns {sig[1]}")
    return errs


def canonicalize(src):
    """Rename each CSF_FUNC binding (and its uses) to the canonical name of its address. Signatures are left
    alone: a different parameter type may be the caller's legitimate view; ABI conflicts are check()'s job."""
    api = load()
    own = HOOK.search(src)
    for m in list(DECL.finditer(src)):
        ea, local = int(m.group(4), 16), m.group(3)
        a = api.get(ea)
        if not a or a["name"] == local or a["name_src"] == "addr":
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
        elif conv != "__fastcall" and takes_reg(ea, db, "ecx"):
            errs.append(f"CSF_FUNC {name}: 0x{ea:X} reads ecx (`this`) on entry, so bind it __fastcall "
                        f"(void* self, void* edx, ...stack args) and pass the right object, not {conv}")
    return errs + float_ret_errors(src, db)


def check():
    from packet import DB
    db = DB()
    impls, decls = scan()
    out = []
    for ea, ds in sorted(decls.items()):
        need_ecx = takes_reg(ea, db, "ecx")
        need_edx = takes_reg(ea, db, "edx")
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
