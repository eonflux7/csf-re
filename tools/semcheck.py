#!/usr/bin/env python3
"""Semantic cross-check of a compiled reimplementation against the original function (part of the gate).

Compares absolute addresses referenced by our object code (CSF_FUNC / CSF_GLOBAL / CSF_STR bind by address, so
they appear as immediates/displacements) with the original's references:
  - every direct callee of the original must be referenced           (dropped calls)
  - every string, writable global (.data/.bss) and vtable it references must be referenced
  - we must not reference game addresses the original doesn't (wrong callee / wrong global)
  - .rdata constants (floats etc.) are soft: a literal in our code is fine
  - field displacements we use on non-stack bases must occur in the original (wrong field offset)

Usage: semcheck.py --all [--behaviour]   run over every src/auto file, report failures (false-positive survey);
       --behaviour: only the constant / call-order checks
"""
import concurrent.futures as cf, glob, json, os, re, subprocess, sys, tempfile
sys.path.insert(0, os.path.dirname(__file__))
from packet import DB, disasm

ROOT = os.path.dirname(os.path.dirname(os.path.abspath(__file__)))
IMG_LO, IMG_HI = 0x401000, 0x900000
_sect = None
_vt = None


def section_of(addr):
    global _sect
    if _sect is None:
        import pefile
        pe = pefile.PE(os.path.join(ROOT, "work", "pc", "CommXPC.exe"), fast_load=True)
        _sect = [(0x400000 + s.VirtualAddress, 0x400000 + s.VirtualAddress + max(s.Misc_VirtualSize, s.SizeOfRawData),
                  s.Name.rstrip(b"\0").decode()) for s in pe.sections]
    for lo, hi, n in _sect:
        if lo <= addr < hi:
            return n
    return None


def vtables():
    global _vt
    if _vt is None:
        _vt = {int(v["addr"], 16)
               for v in json.load(open(os.path.join(ROOT, "db", "vtables.json")))}
    return _vt


HEX = re.compile(r"0x[0-9a-f]+")
DISP = re.compile(r"\[(e[a-z]{2})\s*([+-])\s*(0x[0-9a-f]+|\d+)\]")


def our_refs(obj):
    """(absolute game addresses, displacements on non-stack bases) used anywhere in the object's code."""
    dis = subprocess.run(["llvm-objdump-21", "-d", "--no-show-raw-insn", "--x86-asm-syntax=intel", obj],
                         capture_output=True, text=True).stdout
    addrs, disps = set(), set()
    skip = False
    for line in dis.splitlines():
        m = re.match(r"^[0-9a-f]+ <(.+)>:$", line)
        if m:  # compiler-generated initializers / hook registrars: their field offsets aren't the function's
            skip = bool(re.search(r"\?\?__E|_GLOBAL__|__dyn_tls|_Init_thread|\?\$TSS|@hook@@", m.group(1)))
            continue
        if "\t" not in line:
            continue
        ins = line.split("\t", 1)[1]
        if ins.startswith(("call", "jmp")) and "<" in ins and "[" not in ins:  # relative: own helpers / imports
            continue
        for h in HEX.findall(ins):
            v = int(h, 16)
            if IMG_LO <= v < IMG_HI:
                addrs.add(v)
        for base, sign, off in ([] if skip else DISP.findall(ins)):  # addresses: all code (CSF_GLOBAL binds in init)
            if base not in ("esp", "ebp") and sign == "+":
                disps.add(int(off, 0))
    # Constant-initialized bindings (CSF_GLOBAL references) are 4-byte pointer variables in .rdata/.data sections.
    # Only whole 4-byte sections and .data are scanned: other .rdata holds strings, whose bytes look like addresses.
    raw = subprocess.run(["llvm-objdump-21", "-s", "-j", ".data", "-j", ".rdata", obj], capture_output=True,
                         text=True).stdout
    secs, cur = [], None
    for line in raw.splitlines():
        if line.startswith("Contents of section"):
            cur = [line.split()[-1].rstrip(":"), bytearray()]
            secs.append(cur)
            continue
        m = re.match(r"^ [0-9a-f]{4,} ((?:[0-9a-f]{2,8} ?){1,4})", line)
        if m and cur:
            cur[1] += bytes.fromhex(m.group(1).replace(" ", ""))
    for name, blob in secs:
        if name == ".rdata" and len(blob) != 4:
            continue
        for i in range(0, len(blob) - 3, 4):
            v = int.from_bytes(blob[i:i + 4], "little")
            if IMG_LO <= v < IMG_HI:
                addrs.add(v)
    return addrs, disps


def code_of(ea, db):
    """Original instructions, following tail jumps into shared code blocks that aren't function starts."""
    f = db.F[ea]
    out = list(disasm(ea, f["size"]))
    for i in list(out):
        if i.mnemonic == "jmp" and i.op_str.startswith("0x"):
            t = int(i.op_str, 16)
            if not (f["ea"] <= t < f["end"]) and t not in db.F:
                for j in disasm(t, 256, linear=True):
                    out.append(j)
                    if j.mnemonic in ("ret", "jmp", "int3"):
                        break
    return out


def loaded_as_pointer(obj):
    """Offsets N where our code loads a dword from [x + N] and then uses the loaded register as a memory base."""
    lines = [l.split("\t", 1)[1] for l in subprocess.run(
        ["llvm-objdump-21", "-d", "--no-show-raw-insn", "--x86-asm-syntax=intel", obj],
        capture_output=True, text=True).stdout.splitlines() if "\t" in l]
    out = set()
    for k, l in enumerate(lines):
        m = re.match(r"mov\s+(e[a-z]{2}), dword ptr \[(?!esp|ebp)\w+ \+ (0x[0-9a-f]+|\d+)\]", l)
        if not m:
            continue
        regs, slots = {m.group(1)}, set()  # follow the loaded value through register copies and stack spills (-O0)
        for nxt in lines[k + 1:k + 12]:
            if any(re.search(rf"\[{r}\b", nxt) for r in regs):
                out.add(int(m.group(2), 0))
                break
            sp = re.match(r"mov\s+dword ptr \[(ebp - 0x[0-9a-f]+|esp(?: \+ 0x[0-9a-f]+)?)\], (e[a-z]{2})$", nxt)
            if sp and sp.group(2) in regs:
                slots.add(sp.group(1))
                continue
            ld = re.match(r"mov\s+(e[a-z]{2}), (?:dword ptr \[(.+)\]|(e[a-z]{2}))$", nxt)
            if ld:
                src_in = (ld.group(2) in slots) if ld.group(2) else (ld.group(3) in regs)
                (regs.add if src_in else regs.discard)(ld.group(1))
                if not regs and not slots:
                    break
                continue
            if nxt.startswith(("ret", "jmp")):
                break
    return out


def our_addr_ofs(obj):
    dis = subprocess.run(["llvm-objdump-21", "-d", "--no-show-raw-insn", "--x86-asm-syntax=intel", obj],
                         capture_output=True, text=True).stdout
    return {int(m.group(1), 0) for m in re.finditer(
        r"\t(?:add\s+(?!esp|ebp)\w+, |lea\s+\w+, \[(?!esp|ebp)\w+ \+ )(0x[0-9a-f]+|\d+)", dis)}


def orig_refs(ea, db):
    imms, disps = set(), set()
    for i in code_of(ea, db):
        for h in HEX.findall(i.op_str):
            v = int(h, 16)
            imms.add(v)
        for base, sign, off in DISP.findall(i.op_str):
            disps.add(int(off, 0))
        for m in re.finditer(r"[+-] (0x[0-9a-f]+|\d+)\]", i.op_str):
            disps.add(int(m.group(1), 0))
    return imms, disps


# Compiler EH/SEH runtime reached from the function's unwind thunk; our compiler emits its own.
EH_HELPER = re.compile(r"CxxFrameHandler|_EH_prolog|_SEH_prolog|_SEH_epilog|_except_handler|__security_check"
                       # MSVC's code-gen helpers for float->int casts, 64-bit arithmetic and stack probes: a plain
                       # cast / int64 op / large local is the faithful source; clang lowers it its own way
                       r"|^__ftol2?(_sse)?$|^__a(u)?ll(mul|div|rem|shl|shr)$|^__alloca_probe|^__chkstk$|^_chkstk$")


def ida_switches(ea):
    """{jump_ea: targets} of switch jump tables (export/disasm): those indirect jmps are not tail calls."""
    from packet import ida_view
    v = ida_view(ea)
    return {int(k): t for k, t in (v or {}).get("switches", {}).items()}


def dead_calls(ea, db):
    """Callees reached only behind an always-false unsigned check (`cmp r, -1; jbe skip; call T`: VC7.1 STL's
    max_size() is -1). MSVC keeps the call, any modern compiler folds it away, so it can't be required."""
    ins = code_of(ea, db)
    dead, live = set(), set()
    for k, i in enumerate(ins):
        if i.mnemonic != "call" or not i.op_str.startswith("0x"):
            continue
        t = int(i.op_str, 16)
        if (k >= 2 and ins[k - 1].mnemonic == "jbe" and ins[k - 2].mnemonic == "cmp"
                and ins[k - 2].op_str.endswith(", -1") and ins[k - 1].op_str == hex(i.address + i.size)):
            dead.add(t)
        else:
            live.add(t)
    return dead - live


VSLOT_ERR = "the original calls virtual slots"


def vslot_missing(ea, db, disps):
    """Vtable slots the original calls (`call dword ptr [reg + N]`) whose offset N never appears in `disps`."""
    vslots = sorted({int(m.group(1), 0) for i in code_of(ea, db) if i.mnemonic == "call"
                     for m in [re.match(r"dword ptr \[(?!esp|ebp)\w+ \+ (0x[0-9a-f]+|\d+)\]$", i.op_str)] if m})
    return [n for n in vslots if n not in disps]


def check(ea, obj, db):
    f = db.F[ea]
    ours, our_disps = our_refs(obj)
    imms, orig_disps = orig_refs(ea, db)
    own = range(f["ea"], f["end"])
    errs = []
    required = {}
    for c in f["callees"]:
        if c not in own and c not in dead_calls(ea, db) and not EH_HELPER.search(db.name(c) or ""):
            required[c] = f"call to 0x{c:X} ({db.name(c) or 'sub_%X' % c})"
    for s in f.get("srefs", []):
        required[s] = f"string at 0x{s:X}"
    for d in f["drefs"]:
        if d in own or d in required:
            continue
        sec = section_of(d)
        if sec in (".data", "_rwdseg") or d in vtables():
            required[d] = f"{'vtable' if d in vtables() else 'global'} 0x{d:X}"
    for a, what in sorted(required.items()):
        if a not in ours:
            errs.append(f"the original references {what}, your code never does (dropped call/access, or wrong address)")
    known = set(f["callees"]) | set(f.get("srefs", [])) | set(f["drefs"]) | imms
    for a in sorted(ours - known - {ea}):  # own address: the HOOK registration
        if section_of(a) in (".text", ".data", ".rdata", "_rwdseg", "_rwcseg"):
            n = db.name(a) if a in db.F else None
            errs.append(f"your code references 0x{a:X}{' (' + n + ')' if n else ''}, which the original does not")
    # address-of an embedded sub-object (add ecx, N / lea r, [ecx + N]) must stay an address computation in ours
    addr_of = {int(m.group(1), 16) for i in code_of(ea, db)
               for m in [re.match(r"(?:add ecx, |lea \w+, \[ecx \+ )(0x[0-9a-f]+)", f"{i.mnemonic} {i.op_str}")] if m}
    for n in sorted(addr_of):
        # flag only a dereference at +N with no address computation (pointer loaded from +N); field accesses the
        # compiler folded to [this + N + d] never compute this+N and are fine
        if n not in our_addr_ofs(obj) and n in loaded_as_pointer(obj):
            errs.append(f"the original computes the ADDRESS this+0x{n:X} (add/lea: an embedded object), your code "
                        f"doesn't — did you load a pointer from +0x{n:X} instead of taking its address?")
    # indirect tail jumps (jmp [abs] / jmp [reg+N] / jmp reg) forward the caller's stack args and registers to a
    # target whose signature isn't known here: ours must end in an indirect jmp too, never a call
    sw = ida_switches(ea)
    tails = [i for i in code_of(ea, db) if i.mnemonic == "jmp" and not i.op_str.startswith("0x")
             and i.address not in sw]
    if tails:
        dis = subprocess.run(["llvm-objdump-21", "-d", "--no-show-raw-insn", "--x86-asm-syntax=intel", obj],
                             capture_output=True, text=True).stdout
        if not re.search(r"\tjmp\s+(?:dword ptr (?:ds:)?\[[^\]]+\]|e[a-z]{2})\s*$", dis, re.M):
            t = tails[0].op_str
            errs.append(f"the original TAIL-JUMPS indirectly (`jmp {t}`), forwarding the caller's stack args and "
                        "registers untouched to a target whose signature is unknown; a call drops/shifts them. Write a "
                        "naked thunk: `static __declspec(naked) void F() { __asm { mov ecx, [ecx+4] ... jmp dword ptr "
                        "[eax+0x20] } }` (only mov/lea/add/sub, ending in the jmp)")
    # virtual calls: every vtable slot the original calls (`call [reg + N]`) must be a constant displacement in ours.
    # A generic helper taking the slot as a runtime argument hides it from every check here — three clones did that
    # with byte offsets as indices (slot 0x30 -> 0xC0) and crashed the menu tour (2026-10-08, 0x52F950 & co).
    miss = vslot_missing(ea, db, our_disps)
    if miss:
        errs.append(VSLOT_ERR + " " + ", ".join(f"[vtbl + 0x{n:X}]" for n in miss) + " but your "
                    "code never uses that constant offset: call each one directly with its byte offset as a constant, "
                    "e.g. `(*reinterpret_cast<R(__fastcall***)(void*, void*)>(obj))[0x30 / 4](obj, nullptr)` — no "
                    "helper that takes the slot as a runtime argument")
    # pointer adjusted first (add r, K / lea r, [x + K]) then dereferenced at +d: the compiler may fold to [x + K + d]
    adds = {int(m.group(1), 0) for i in code_of(ea, db)
            for m in [re.match(r"(?:add (?!esp|ebp)\w+, |lea \w+, \[(?!esp|ebp)\w+ \+ )(0x[0-9a-f]+|\d+)\]?$",
                               f"{i.mnemonic} {i.op_str}")] if m}
    folded = {k + d for k in adds for d in orig_disps | {0}}
    bad = sorted(d for d in our_disps if d >= 8 and d not in orig_disps and d not in imms and d not in folded)
    if bad:
        errs.append("your code accesses field offsets the original never uses: " + ", ".join(f"0x{d:X}" for d in bad))
    return errs


def byte_return_of_bool(ea, obj, fn, db):
    """The original returns a full 32-bit BOOL (setcc al on a zeroed eax: `xor eax,eax` before it or `movzx eax,al`
    after it) but our hooked function is declared bool/char: the upper bytes of eax are then undefined (callers test
    eax). Read from the MSVC mangled return type (`YI_N` = __fastcall returning bool, `YIH` = returning int)."""
    code = [f"{i.mnemonic} {i.op_str}".strip() for i in code_of(ea, db)]
    widened = False
    for k, c in enumerate(code):
        if re.match(r"set\w+ al$", c):
            if "xor eax, eax" in code[:k] or any(x.startswith("movzx eax, al") for x in code[k + 1:k + 4]):
                widened = True
    if not widened:
        return []
    syms = subprocess.run(["llvm-nm-21", obj], capture_output=True, text=True).stdout
    m = re.search(rf"\?{re.escape(fn)}@@Y[AGI](_N|[CDE])", syms)
    if m:
        t = "bool" if m.group(1) == "_N" else "char"
        return [f"the original returns a full 32-bit value (setcc al on a zeroed eax): declare `{fn}` returning int "
                f"(BOOL), not {t} — a {t} return leaves eax's upper bytes undefined and callers test eax"]
    return []


_abi = None


def abi(ea):
    """Hex-Rays' view of the original (export/abi.json, tools/ida/export_abi.py): `consts`, `order`, `calls`."""
    global _abi
    if _abi is None:
        try:
            _abi = json.load(open(os.path.join(ROOT, "export", "abi.json")))
        except OSError:
            _abi = {}
    return _abi.get(f"{ea:08x}", {})


def our_functions(obj):
    """{symbol: [instruction text]} of the object's code, relocation targets inlined as `<reloc SYM>` lines."""
    dis = subprocess.run(["llvm-objdump-21", "-dr", "--no-show-raw-insn", "--x86-asm-syntax=intel", obj],
                         capture_output=True, text=True).stdout
    out, cur = {}, None
    for line in dis.splitlines():
        m = re.match(r"^[0-9a-f]+ <(.+)>:$", line)
        if m:
            cur = out.setdefault(m.group(1), [])
        elif cur is not None and "IMAGE_REL_I386" in line:
            cur.append("<reloc " + line.split()[-1] + ">")
        elif cur is not None and "\t" in line:
            cur.append(line.split("\t", 1)[1].strip())
    return out


def call_seq(funcs, sym, seen=()):
    """Original functions our `sym` calls, in code order (/Od keeps source order): `mov r, ADDR` .. `call r`, direct
    `call ADDR`, and calls into the file's own helpers expanded in place."""
    out, regs = [], {}
    ins = funcs.get(sym, [])
    for k, i in enumerate(ins):
        m = re.match(r"mov\s+(e[a-z]{2}), (0x[0-9a-f]+)$", i)
        if m:
            regs[m.group(1)] = int(m.group(2), 16)
            continue
        m = re.match(r"mov\s+(e[a-z]{2}), dword ptr \[0x0\]$", i)
        if m and k + 1 < len(ins) and ins[k + 1].startswith("<reloc "):
            regs[m.group(1)] = None  # a pointer variable of the file's own: target unknown here
            continue
        m = re.match(r"call\s+(\S+)", i)
        if not m:
            continue
        t = m.group(1)
        if t in regs and regs[t] is None:
            raise LookupError(t)
        if t in regs:
            out.append(regs[t])
        elif t.startswith("0x") and IMG_LO <= int(t, 16) < IMG_HI:
            out.append(int(t, 16))
        elif k + 1 < len(ins) and ins[k + 1].startswith("<reloc "):
            callee = ins[k + 1][7:-1]
            if callee in funcs and callee not in seen and len(seen) < 6:
                out += call_seq(funcs, callee, seen + (sym,))
    return out


REAL = re.compile(r"__real@([0-9a-f]{8}|[0-9a-f]{16})\b")
NUM_LIT = re.compile(r"(?<![\w.])(0x[0-9a-fA-F]+|\d+\.\d*(?:[eE][-+]?\d+)?|\d*\.\d+(?:[eE][-+]?\d+)?|\d+[eE][-+]?\d+|\d+)"
                     r"[uUlLfF]*")


def our_consts(funcs, src):
    """(ints as unsigned 32-bit, floats) our code computes with: immediates and displacements, float bit patterns,
    the compiler's float pool (__real@...), and the source's own literals."""
    import struct
    ints, floats = set(), set()
    for sym, ins in funcs.items():
        if re.search(r"\?\?__E|@hook@@", sym):
            continue
        for i in ins:
            for h in re.findall(r"(?<![\w@])(-?0x[0-9a-f]+|-?\d+)(?![\w@])", i):
                ints.add(int(h, 16 if "0x" in h else 10) & 0xFFFFFFFF)
            for r in REAL.findall(i):
                floats.add(struct.unpack("<f" if len(r) == 8 else "<d", bytes.fromhex(r)[::-1])[0])
    for v in list(ints):
        floats.add(struct.unpack("<f", v.to_bytes(4, "little"))[0])
    code = re.sub(r"//[^\n]*|/\*.*?\*/|\"(?:[^\"\\\n]|\\.)*\"", "", src, flags=re.S)
    for lit in NUM_LIT.findall(code):
        if re.fullmatch(r"0x[0-9a-fA-F]+|\d+", lit):
            ints.add(int(lit, 16 if lit[:2] in ("0x", "0X") else 10) & 0xFFFFFFFF)
        else:
            floats.add(float(lit))
    for g in list(floats):  # a float literal is also its bit pattern (the original may store 0x3F800000)
        try:
            ints.add(struct.unpack("<I", struct.pack("<f", g))[0])
        except OverflowError:
            pass
    return ints, floats


_pe = None


def rdata_floats(addrs):
    """Float/double values stored at the .rdata addresses given (the original's float constant pool)."""
    import struct
    global _pe
    if _pe is None:
        import pefile
        _pe = pefile.PE(os.path.join(ROOT, "work", "pc", "CommXPC.exe"), fast_load=True)
    out = set()
    for a in addrs:
        if section_of(a) != ".rdata":
            continue
        raw = _pe.get_data(a - 0x400000, 8)
        if len(raw) == 8:
            out.add(struct.unpack("<f", raw[:4])[0])
            out.add(struct.unpack("<d", raw)[0])
    return out


TRIVIAL = {-1, 0, 1, 2, -0x80000000, 0x80000000, 0x7FFFFFFF}  # sign-bit tests are `< 0` in C


def const_found(c, ints, floats):
    if isinstance(c, float):
        if c.is_integer() and int(c) & 0xFFFFFFFF in ints:
            return True
        if c and abs(1.0 / c - round(1.0 / c)) < 1e-4 and int(round(1.0 / c)) & 0xFFFFFFFF in ints:
            return True  # x * (1/255) as x / 255
        tol = 1e-6 * max(1.0, abs(c))
        return any(abs(g - c) <= tol or abs(g + c) <= tol or (g and abs(1.0 / g - c) <= tol) for g in floats)
    alts = {c, -c, ~c, c - 1, c + 1}
    alts |= {c * k for k in (2, 4, 8)} | {c // k for k in (2, 4, 8) if c % k == 0}  # index scaled / count in dwords
    if c > 0 and c & (c - 1) == 0:
        alts.add(c.bit_length() - 1)  # x * 8 as x << 3
    if 0 < c < 32:
        alts |= {1 << c, (1 << c) - 1}  # x << 3 as x * 8; a mask of c bits
    if c >= 0x100 and any(c < v <= c + 0x40 for v in ints):
        return True  # an offset kept in an integer and folded into the next one (base + 0x21B8, then + 4)
    return any(a & 0xFFFFFFFF in ints for a in alts) or float(c) in floats


def behaviour(ea, obj, fn, src, db):
    """Beyond references: the original's constants must appear in ours, and calls Hex-Rays shows as consecutive
    statements of one block must come in the same order in ours (each callee called once in both, so branches and
    loops can't reorder them)."""
    a = abi(ea)
    if not a:
        return []
    funcs = our_functions(obj)
    errs = []
    ints, floats = our_consts(funcs, src)
    floats |= rdata_floats(our_refs(obj)[0])  # a .rdata float read by address (CSF_GLOBAL) is the literal itself
    miss = [c for c in a.get("consts", []) if c not in TRIVIAL and not (isinstance(c, float) and c in (0.0, 1.0, -1.0))
            and not const_found(c, ints, floats)]
    bases = [b for b in a.get("consts", []) if isinstance(b, int) and b >= 16 and b & 0xFFFFFFFF in ints]
    miss = [c for c in miss if not (isinstance(c, int) and any(c > b and c - b in ints for b in bases))]
    if miss:
        errs.append(CONST_ERR + ": " + ", ".join(f"{c:g}" if isinstance(c, float) else
                                                 (f"0x{c & 0xFFFFFFFF:X}" if abs(c) >= 0x100 else str(c))
                                                 for c in miss[:12]) +
                    " — a wrong value, a dropped case/branch, or a value derived another way (write the original's "
                    "literal so it can be checked)")
    sym = next((s for s in funcs if s.startswith(f"?{fn}@") or s in (fn, f"_{fn}", f"@{fn}")
                or re.match(rf"@{re.escape(fn)}@\d+$", s)), None)
    try:
        ours = call_seq(funcs, sym) if sym and a.get("order") else None
    except LookupError:  # calls through a function-pointer variable: our order can't be read from the code
        ours = None
    if ours is not None:
        once = {int(k, 16) for k, v in a.get("calls", {}).items() if len(v) == 1}
        pos = {c: ours.index(c) for c in set(ours) if ours.count(c) == 1}
        seen = set()
        for blk in a["order"]:
            seq = [int(c, 16) for c in blk]
            seq = [c for c in seq if c in once and c in pos]
            for x, y in zip(seq, seq[1:]):
                if pos[x] > pos[y] and (x, y) not in seen:
                    seen.add((x, y))
                    errs.append(f"{ORDER_ERR}: 0x{x:X} ({db.name(x) or 'sub_%X' % x}) then 0x{y:X} "
                                f"({db.name(y) or 'sub_%X' % y}) as consecutive statements; yours calls them the "
                                "other way round (side effects reordered)")
    return errs


CONST_ERR = "the original computes with constants your code never uses"
ORDER_ERR = "the original calls in this order"


def survey(only_behaviour=False):
    from gate import compile_cmd
    db = DB()
    files = sorted(glob.glob(os.path.join(ROOT, "src", "auto", "*.cpp")))

    def one(p):
        ea = int(os.path.basename(p)[:8], 16)
        with tempfile.TemporaryDirectory() as td:
            obj = os.path.join(td, "t.obj")
            r = subprocess.run(compile_cmd() + [f"/Fo{obj}", "-c", "--", p], capture_output=True, text=True,
                               cwd=os.path.join(ROOT, "build"))
            if r.returncode:
                return ea, ["compile failed"]
            m = re.search(r"HOOK(?:_METHOD)?\(\s*0x[0-9A-Fa-f]+\s*,\s*&?([\w:]+)", open(p).read())
            fn = m.group(1).split("::")[-1] if m else ""
            if only_behaviour:
                return ea, behaviour(ea, obj, fn, open(p).read(), db) if fn else []
            return ea, (check(ea, obj, db) + (byte_return_of_bool(ea, obj, fn, db) if fn else [])
                        + (behaviour(ea, obj, fn, open(p).read(), db) if fn else []))

    bad = 0
    with cf.ThreadPoolExecutor(16) as ex:
        for ea, errs in ex.map(one, files):
            if errs:
                bad += 1
                print(f"{ea:08x}: " + " | ".join(errs))
    print(f"{bad}/{len(files)} flagged")


if __name__ == "__main__":
    if "--all" in sys.argv:
        survey("--behaviour" in sys.argv)
