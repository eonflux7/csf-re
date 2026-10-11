#!/usr/bin/env python3
"""Gate for one reimplemented function file.

Checks: allowed includes only, no inline asm, exactly one HOOK for the target address, compiles with the real
build flags (static_asserts included), referenced addresses/field offsets match the original (semcheck.py),
the original's constants appear and its consecutive calls keep their order (semcheck.behaviour, Hex-Rays' view),
and the hooked function's `ret N` matches the original.

Usage: gate.py <ea-hex> <file.cpp>      exit 0 = pass; errors on stdout
"""
import json, os, re, shlex, subprocess, sys, tempfile
sys.path.insert(0, os.path.dirname(__file__))
from packet import ret_bytes, disasm, DB
import semcheck

ROOT = os.path.dirname(os.path.dirname(os.path.abspath(__file__)))
ALLOWED_INC = {"cstdint", "cstddef", "cstring", "cmath", "climits", "cfloat", "globals.h", "hook.h",
               "core/memory.h", "types/auto_classes.h", "legacy/string.h"}


def compile_cmd():
    cc = json.load(open(os.path.join(ROOT, "build", "compile_commands.json")))
    cmd = next(c["command"] for c in cc if c["file"].endswith("hook/main.cpp"))
    args = shlex.split(cmd)
    i = args.index("-c")
    return [a for a in args[:i] if not a.startswith(("/Fo", "/Fd"))]


def our_rets(obj, fn_name):
    """Find `ret` immediates in the compiled function whose (mangled) symbol contains fn_name."""
    dis = subprocess.run(["llvm-objdump-21", "-d", "--no-show-raw-insn", "--x86-asm-syntax=intel", obj],
                         capture_output=True, text=True).stdout
    rets, cur, found = set(), None, False
    for line in dis.splitlines():
        m = re.match(r"^[0-9a-f]+ <(.+)>:$", line)
        if m:
            if not m.group(1).startswith("$"):  # $ehgcr_* etc.: labels inside the current function
                cur = m.group(1)
            continue
        if cur and cur.startswith(("?catch$", "?dtor$", "?fin$", "___ehhandler")):  # EH funclets: own rets
            continue
        if cur and re.search(rf"\b{re.escape(fn_name)}\b|\?{re.escape(fn_name)}@", cur):
            found = True
            m2 = re.search(r"\tret\s*(\S*)", line)
            if m2:
                rets.add(int(m2.group(1), 16) if m2.group(1) else 0)
    return found, sorted(rets)


def stack_bytes(conv, params):
    """Bytes a callee pops for a CSF_FUNC declaration (all params 4-byte except double/int64/explicit 8)."""
    ps = [p.strip() for p in params.split(",") if p.strip() and p.strip() != "void"]
    if conv == "__fastcall":
        ps = ps[2:]  # ecx, edx
    size = sum(8 if re.search(r"\b(double|int64_t|uint64_t|long long)\b(?!\s*\*)", p) else 4 for p in ps)
    return size


def effective_rets(ea, db, depth=3):
    """`ret N` bytes of a function; one that ends in a direct tail jump (`jmp 0xT`, no ret of its own) inherits T's:
    the jump hands T the caller's stack frame, so T's ret pops the caller's args."""
    rets = ret_bytes(ea, db.F[ea]["size"])
    if rets or depth == 0:
        return rets
    f = db.F[ea]
    tails = {int(i.op_str, 16) for i in disasm(ea, f["size"]) if i.mnemonic == "jmp" and i.op_str.startswith("0x")}
    tails = {t for t in tails if t in db.F and not f["ea"] <= t < f["end"]}
    out = set()
    for t in tails:
        out |= set(effective_rets(t, db, depth - 1))
    return sorted(out)


def check_bindings(src, db):
    """Every CSF_FUNC binding must match the callee's real `ret N` (cdecl pops 0)."""
    errs = []
    for m in re.finditer(r"CSF_FUNC\(\s*(.+?)\s*,\s*(__\w+)\s*,\s*(\w+)\s*,\s*0x([0-9A-Fa-f]+)\s*,\s*\((.*?)\)\s*\)",
                         src, re.S):
        conv, name, addr, params = m.group(2), m.group(3), int(m.group(4), 16), m.group(5)
        if addr not in db.F:
            errs.append(f"CSF_FUNC {name}: 0x{addr:X} is not a function start")
            continue
        rets = effective_rets(addr, db)
        if not rets:
            continue  # no ret (tail jump / noreturn): can't check
        want = 0 if conv == "__cdecl" else stack_bytes(conv, params)
        if conv == "__cdecl" and rets != [0]:
            errs.append(f"CSF_FUNC {name}: declared __cdecl but 0x{addr:X} pops {rets} bytes (use __stdcall, or "
                        f"__fastcall with ecx/edx for __thiscall)")
        elif conv != "__cdecl" and want not in rets:
            errs.append(f"CSF_FUNC {name}: declared {conv} popping {want} bytes but 0x{addr:X} pops {rets}")
    return errs


def self_refs(obj, src):
    """Undefined symbols of the object that name a function this file defines itself: inline asm referring to a
    decorated function by its plain name (links alone in no case; the lanes batch link failed on 0x564B80, 0x53CC70)."""
    out = subprocess.run(["llvm-nm-21", "--undefined-only", obj], capture_output=True, text=True).stdout
    bad = []
    for sym in re.findall(r"^\s*U\s+(\S+)", out, re.M):
        plain = re.sub(r"^[_@]|@\d+$", "", sym)
        if re.fullmatch(r"\w+", plain) and re.search(rf"\b{re.escape(plain)}\s*\([^;]*\)\s*\{{", src, re.S):
            bad.append(plain)
    return sorted(set(bad))


def strip_cpp_comments(src):
    return re.sub(r"//[^\n]*|/\*.*?\*/", "", src, flags=re.S)


def gate(ea, path, db=None):
    path = os.path.abspath(path)
    src = open(path).read()
    errs = []
    for inc in re.findall(r'#\s*include\s*[<"]([^>"]+)[>"]', src):
        if inc not in ALLOWED_INC:
            errs.append(f"include not allowed: {inc}")
    if re.search(r"\bCSF_(GLOBAL|FUNC|AT|STR)\b", src) and not re.search(r'#\s*include\s*"globals.h"', src):
        errs.append('CSF_GLOBAL/CSF_FUNC/CSF_AT/CSF_STR come from #include "globals.h" (add it)')
    if re.search(r"#\s*define\s+CSF_|#\s*undef\s+CSF_", src):
        errs.append('do not (re)define CSF_* macros: #include "globals.h" and use them as given')
    if re.search(r"\bvolatile\b", strip_cpp_comments(src)):  # fidelity: dead loads / garbage args are not behavior
        errs.append("volatile is not allowed: don't reproduce a load whose value the original never uses (skip it) or "
                    "an uninitialized value it passes (pass 0); for globals the game changes, a plain CSF_GLOBAL is read "
                    "fresh at every use already")
    import semcheck as _sc  # a vtable bound as data: its value is the first slot, not the table (0x547CB0 crashed)
    for m in re.finditer(r"CSF_GLOBAL\([^,]+,\s*(\w+),\s*(0x[0-9A-Fa-f]+)\)", src):
        if int(m.group(2), 16) in _sc.vtables():
            errs.append(f"{m.group(1)}: 0x{int(m.group(2), 16):X} is a vtable; never bind it with CSF_GLOBAL (its value is "
                        "the first slot, not the table): use CSF_VTBL(0x..) to store / call it, CSF_ADDR_VT(0x..) for "
                        "its address as a number")
    if re.search(r"\bHOOK\(", src) and "HOOK_KEEP_ECX" not in src:
        import regargs
        from packet import DB as _DB
        _db = db if db is not None else _DB()
        if ea in _db.F and regargs.needs_keep_ecx(_db, ea):
            errs.append(f"callers of 0x{ea:X} keep a value in ecx across the call and the original never writes ecx: "
                        f"register with HOOK_KEEP_ECX(0x{ea:X}, Fn, <stack args>, \"module\") instead of HOOK")
    if re.search(r"\b__regcall\b", src):  # grunts' bridge for Hex-Rays' phantom @<edi>/@<esi> args (tools/regargs.py)
        errs.append("__regcall is not allowed: register parameters Hex-Rays shows in ebx/esi/edi are saved registers, "
                    "not arguments (see the packet's NOTE); bind and implement with a plain convention")
    # the only allowed asm: a naked tail-jump thunk — at most 8 register moves/tests/branches around one indirect jmp
    for blk in re.findall(r"__asm\s*\{([^}]*)\}", src):
        ins = [l.strip() for l in re.split(r"[\n;]", blk) if l.strip() and not re.fullmatch(r"\w+:", l.strip())]
        bad = [x for x in ins if not re.match(r"(mov|lea|add|sub|test|cmp|je|jne|jz|jnz|ret|call\s+e[a-z]x\b|jmp)\b", x)]
        if bad or len(ins) > 8 or not any(re.match(r"jmp\s+(dword\s+ptr\s+(ds:)?\[|e[a-z]x\b|0x)", x) for x in ins):
            errs.append("inline asm allowed only as a naked tail-jump thunk (<= 8 of mov/lea/add/sub/test/cmp/je/jne/"
                        "ret/call reg, ending in an indirect jmp); offending: " + "; ".join(bad[:3] or ins[:3]))
    if re.search(r"\b(__asm|asm)\b", re.sub(r"__asm\s*\{[^}]*\}", "", src)):
        errs.append("inline asm only as `__asm { ... }` blocks in a naked thunk")
    # CSF_AT(T, p, off) already IS the T stored at p+off; a `*` in front dereferences that value again. 2026-10-09:
    # 38 files did this (spread through clone examples) and crashed the game; the gate's offset checks can't see it
    for m in re.finditer(r"\*\s*CSF_AT\s*\(", re.sub(r"//[^\n]*|/\*.*?\*/", "", src, flags=re.S)):
        before = re.sub(r"//[^\n]*|/\*.*?\*/", "", src, flags=re.S)[:m.start()].rstrip()
        if not re.search(r"[\w)\]]$", before) or re.search(r"\b(return|case|else|do)$", before):  # unary `*`
            errs.append("double dereference: `*CSF_AT(T*, p, off)` reads the pointer at p+off and then dereferences "
                        "it, because CSF_AT(T, p, off) already IS the T at p+off. For the value at p+off write "
                        "CSF_AT(T, p, off). If the original really loads a pointer there and dereferences it, write "
                        "two steps: `T* q = CSF_AT(T*, p, off);` then `*q`")
            break
    hooks = re.findall(r"HOOK(?:_METHOD|_KEEP_ECX|_TAIL)?\(\s*0x([0-9A-Fa-f]+)\s*,\s*&?([\w:]+)", src)
    if len(hooks) != 1 or int(hooks[0][0], 16) != ea:
        errs.append(f"need exactly one HOOK(0x{ea:X}, Fn, \"module\") line; found {hooks}")
    code_only = re.sub(r"//[^\n]*|/\*.*?\*/", "", src, flags=re.S)
    bare = re.sub(r'"(?:[^"\\\n]|\\.)*"', '""', code_only)  # comments ("the new block") and strings don't count
    if re.search(r"\b(malloc|free|calloc|realloc)\s*\(|\bnew\s+[A-Za-z_]|\bdelete\s", bare):
        errs.append("use game::Mem_Alloc/Mem_Free (core/memory.h), not malloc/free/new/delete")
    for line in code_only.splitlines():
        if re.search(r'"', line) and not re.search(r"^\s*(HOOK|#\s*include)", line):
            stripped = re.sub(r'CSF_STR\(\s*0x[0-9A-Fa-f]+\s*,\s*"(?:[^"\\]|\\.)*"\s*\)', "", line)
            if '"' in stripped:
                errs.append(f"bare string literal (use CSF_STR(0xADDR, \"text\")): {line.strip()[:120]}")
    db = db or DB()
    errs += check_bindings(src, db)
    import addrfix, api
    errs += api.binding_errors(src, db)
    raw = sorted({v for _, _, v, _ in addrfix.raw_hits(src)})
    if raw:  # the hook build would work, the standalone build can't link them (src/globals.h)
        errs.append("raw original address(es) " + ", ".join(f"0x{v:X}" for v in raw) + " in code: use CSF_VTBL(0x..) "
                    "for a vtable, CSF_ADDR_FN / CSF_ADDR_DATA / CSF_ADDR_VT(0x..) for an address used as a value, "
                    "CSF_GLOBAL / CSF_STR / CSF_FUNC otherwise")
    if errs:
        return False, "\n".join(errs)
    with tempfile.TemporaryDirectory() as td:
        obj = os.path.join(td, "t.obj")
        r = subprocess.run(compile_cmd() + [f"/Fo{obj}", "-c", "--", path], capture_output=True, text=True,
                           cwd=os.path.join(ROOT, "build"))
        if r.returncode != 0:
            out = (r.stdout + r.stderr).strip()
            out = "\n".join(l for l in out.splitlines() if "warning" not in l or "error" in l)
            return False, "compile error:\n" + out[:4000]
        unres = self_refs(obj, src)
        if unres:  # the gate compiles alone; the batch link would fail on these
            return False, ("link error: " + ", ".join(unres) + " referenced by its plain name (inline asm `offset Fn` / "
                           "`call Fn`), but the compiler decorates the symbol (@Fn@8 for __fastcall, ?Fn@@... for "
                           "static/C++) so it never resolves. Take the address in C++ instead (e.g. a static function "
                           "pointer variable loaded by the asm: `mov eax, [s_fn]`) or drop the asm.")
        fn = hooks[0][1].split("::")[-1]
        found, rets = our_rets(obj, fn)
        orig = effective_rets(ea, db)
        if not found:
            return False, f"could not find compiled function `{fn}` in object file"
        sem = (semcheck.check(ea, obj, db) + semcheck.byte_return_of_bool(ea, obj, fn, db)
               + semcheck.behaviour(ea, obj, fn, src, db))
        if any(e.startswith(semcheck.VSLOT_ERR) for e in sem):
            # the build is /Od /Ob0: a helper taking the slot as an argument hides the constant. Optimized, a correct
            # helper folds back to [vtbl + N]; a wrong one (byte offset used as index) shows [vtbl + 4N] and fails
            opt = os.path.join(td, "o2.obj")
            flags = [a for a in compile_cmd() if a not in ("/Od", "/Ob0")] + ["/O2", "/Ob2"]
            r2 = subprocess.run(flags + [f"/Fo{opt}", "-c", "--", path], capture_output=True, text=True,
                                cwd=os.path.join(ROOT, "build"))
            if r2.returncode == 0 and not semcheck.vslot_missing(ea, db, semcheck.our_refs(opt)[1]):
                sem = [e for e in sem if not e.startswith(semcheck.VSLOT_ERR)]
        if rets and orig and set(rets) != set(orig):
            sem.append(f"calling convention mismatch: original pops {orig} bytes (ret N), yours pops {rets}. "
                       "Fix the calling convention / parameter list (__thiscall → __fastcall with dummy edx; "
                       "__stdcall pops args; __cdecl pops 0).")
        sem, waived = apply_waivers(ea, obj, sem)
        if sem:
            conv = [e for e in sem if e.startswith("calling convention mismatch")]
            rest = [e for e in sem if e not in conv]
            if rest:
                return False, "semantic check (references must match the original's):\n" + "\n".join(rest)
            return False, conv[0]
    return True, "ok" + (f" (waived: {'; '.join(waived)})" if waived else "")


# Failure kinds of a gate (or reviewer) error, for the run log (grunt.py `fails`, `grunt.py cost`): what the
# retries are spent on. First match per error line; an error carries every kind it shows.
FAIL_KINDS = [
    ("backend", r"^(codex|claude|go|deepseek|opencode) error: "),
    ("abi", r"calling convention mismatch|declared __cdecl but|popping \d+ bytes but|reads ecx|reads edx|"
            r"returns a full 32-bit value|returns its value in st0"),
    ("binding", r"is not a function start|CSF_FUNC .*own address|referenced by its plain name"),
    ("sem_missing_ref", r"the original references .*your code never does"),
    ("sem_extra_ref", r"your code references 0x[0-9A-F]+.*which the original does not"),
    ("sem_vslot", r"virtual call|\[vtbl \+ 0x"),
    ("sem_field", r"field offsets the original never uses|computes the ADDRESS this\+"),
    ("sem_const", r"computes with constants your code never uses"),
    ("sem_order", r"the original calls in this order"),
    ("tail_jump", r"TAIL-JUMPS"),
    ("compile", r"compile error|error:|could not find compiled function"),
    ("raw_address", r"raw original address"),
    ("string", r"bare string literal"),
    ("hook", r"need exactly one HOOK"),
    ("asm", r"inline asm"),
    ("deref", r"double dereference"),
    ("include", r"include not allowed|come from #include \"globals.h\""),
    ("alloc", r"Mem_Alloc"),
    ("review", r"reviewer compared your file"),
    ("no_code", r"no ```cpp block"),
]
_FAIL_RE = [(k, re.compile(p, re.I)) for k, p in FAIL_KINDS]


def fail_kinds(err):
    """Sorted kinds of one gate error text (several checks can fail at once); 'other' when nothing matches."""
    out = set()
    for line in (err or "").splitlines() or [""]:
        for k, rx in _FAIL_RE:
            if rx.search(line):
                out.add(k)
                break
    return sorted(out) or ["other"]


WAIVERS = os.path.join(ROOT, "db", "waivers.yaml")


def load_waivers():
    import yaml
    try:
        d = yaml.safe_load(open(WAIVERS)) or {}
    except OSError:
        return {}
    return {(int(k, 16) if isinstance(k, str) else k): v for k, v in d.items()}


def apply_waivers(ea, obj, errs):
    """Drop findings waived for exactly this compiled code (db/waivers.yaml, written by tools/waive.py)."""
    if not errs:
        return errs, []
    w = load_waivers().get(ea)
    if not w:
        return errs, []
    from verify import code_hash
    if w.get("code") != code_hash(obj):
        return errs, []  # the code changed since the review: the waiver no longer applies
    keep, waived = [], []
    for e in errs:
        (waived if any(re.search(p, e) for p in w.get("checks", [])) else keep).append(e)
    return keep, [w.get("why", "")] if waived else []


if __name__ == "__main__":
    ok, msg = gate(int(sys.argv[1], 16), sys.argv[2])
    print(msg)
    sys.exit(0 if ok else 1)
