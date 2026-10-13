#!/usr/bin/env python3
"""Gate for one reimplemented function file.

Checks: allowed includes only, no inline asm, exactly one HOOK for the target address, compiles with the real
build flags (static_asserts included), referenced addresses/field offsets match the original (semcheck.py),
the original's constants appear and its consecutive calls keep their order (semcheck.behaviour, Hex-Rays' view),
and the hooked function's `ret N` matches the original.

Usage: gate.py <ea-hex> <file.cpp>      exit 0 = pass; errors on stdout
"""
import glob, json, os, re, shlex, subprocess, sys, tempfile
sys.path.insert(0, os.path.dirname(__file__))
from packet import ret_bytes, disasm, DB
import semcheck

ROOT = os.path.dirname(os.path.dirname(os.path.abspath(__file__)))
ALLOWED_INC = {"cstdint", "cstddef", "cstring", "cmath", "climits", "cfloat", "globals.h", "hook.h",
               "core/memory.h", "types/auto_classes.h", "legacy/string.h", "types/vc71_vector.h"}


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
        # a definition: `type name(params) {`. Not `if (std::sqrt(x) < y) {`: the CRT's sqrt was reported as the
        # file's own function (0x4B6A80 went through the whole ladder on it)
        if re.fullmatch(r"\w+", plain) and re.search(
                rf"(?:[\w>)]\s+|[*&]\s*)(?<!return )(?<!::)\b{re.escape(plain)}\s*\([^;{{}}()]*(?:\([^()]*\)[^;{{}}()]*)*\)\s*\{{",
                src, re.S):
            bad.append(plain)
    return sorted(set(bad))


def emudiff_mode():
    """db/lanes.yaml `emudiff_gate`: off | log | on. log: run it, append the verdict to work/emudiff_gate.jsonl,
    reject nothing."""
    try:
        import yaml
        v = (yaml.safe_load(open(os.path.join(ROOT, "db", "lanes.yaml"))) or {}).get("emudiff_gate", "off")
    except Exception:
        return "off"
    return {True: "on", False: "off"}.get(v, str(v))


EMU_ERR = "emulated run differs from the original"


def emulated_diff(ea, obj, path, db):
    """The last gate step: tools/emudiff.py runs the original's bytes and this object side by side from the same
    state. -> error text for a DIFF (mode on), else None. inconclusive / error verdicts never reject."""
    mode = emudiff_mode()
    if mode == "off":
        return None
    try:
        import emudiff
        verdict, detail = emudiff.check(ea, obj_path=obj, src=path)
    except Exception as e:  # the emulator is a check, not a dependency of the gate
        verdict, detail = "error", f"{type(e).__name__}: {e}"
    try:
        with open(os.path.join(ROOT, "work", "emudiff_gate.jsonl"), "a") as f:
            f.write(json.dumps({"ea": f"{ea:08x}", "verdict": verdict, "detail": detail[:300], "mode": mode}) + "\n")
    except OSError:
        pass
    if verdict != "DIFF" or mode != "on":
        return None
    names = lambda m: f"0x{int(m.group(0), 16):X} ({db.name(int(m.group(0), 16))})" \
        if int(m.group(0), 16) in db.F and db.name(int(m.group(0), 16)) else m.group(0)
    detail = re.sub(r"\b00[4-8][0-9a-f]{5}\b", names, detail + "\n" + getattr(emudiff._tls, "trace", ""))
    return (f"{EMU_ERR} (both run in an emulator from the same registers, arguments and memory; calls leaving the "
            f"function are recorded and answered alike; `X vs Y` is the original's value vs yours):\n{detail}\n"
            "Read it as: `N calls vs M` = the two took different paths or you call something the original does not "
            "(or twice); `call K: target A vs B` / `arguments` / `ecx` = the K-th call (from 0) differs in what is "
            "called or passed (check which call an early `push` belongs to, and `this` = the address of an embedded "
            "object vs a pointer loaded from it); `returns` = eax at the end; `memory` = a store the other side does "
            "not make or makes with another value.")


def strip_cpp_comments(src):
    return re.sub(r"//[^\n]*|/\*.*?\*/", "", src, flags=re.S)


_LOCAL_RX = re.compile(r"^[ \t]+((?:S|C)_[0-9a-f]{8}\w*)\s+(\w+)\s*(?:\[[^\]]*\]\s*)?(?:=\s*\{[^}]*\}|\{[^}]*\})?\s*;", re.M)


def struct_locals(code):
    """By-value locals of generated struct types (S_/C_<ea>) in comment-free code: matches of _LOCAL_RX that are not
    member declarations of a struct/class/union defined in the file (a hand-laid-out frame is the author's sizing)."""
    out, kinds, pos = [], [], 0
    for m in re.finditer(r"[{}]|" + _LOCAL_RX.pattern, code, re.M):
        t = m.group(0)
        if t == "{":
            head = code[max(code.rfind(";", 0, m.start()), code.rfind("}", 0, m.start()), code.rfind("{", 0, m.start())) + 1:m.start()]
            kinds.append(bool(re.search(r"\b(struct|class|union)\b", head)) and "(" not in head)
        elif t == "}":
            if kinds:
                kinds.pop()
        elif not (kinds and kinds[-1]):
            out.append(m)
    return out


def tail_ret_pops(code):
    """Pop counts the file's hook::tail_ret calls name: 0 for tail_ret(eax), N for tail_ret(eax, N); -1 when the
    second argument is not a number."""
    out = set()
    for m in re.finditer(r"\btail_ret\s*\(", code):
        depth, args, cur = 1, [], ""
        for ch in code[m.end():]:
            if ch == "(":
                depth += 1
            elif ch == ")":
                depth -= 1
                if not depth:
                    break
            if ch == "," and depth == 1:
                args.append(cur)
                cur = ""
            else:
                cur += ch
        args.append(cur)
        if len(args) < 2:
            out.add(0)
        else:
            a = args[1].strip().rstrip("uU")
            out.add(int(a, 0) if re.fullmatch(r"0[xX][0-9a-fA-F]+|\d+", a) else -1)
    return out


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
                        f"register with HOOK_KEEP_ECX(0x{ea:X}, Fn, <stack args>, \"module\") instead of HOOK "
                        f"(HOOK_KEEP_ECX_CDECL(...) when the original is __cdecl: a plain `ret`, the caller pops)")
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
    hooks = re.findall(r"HOOK(?:_METHOD|_KEEP_ECX_CDECL|_KEEP_ECX|_REGS|_TAIL)?\(\s*0x([0-9A-Fa-f]+)\s*,\s*&?([\w:]+)", src)
    code_only = re.sub(r"//[^\n]*|/\*.*?\*/", "", src, flags=re.S)
    bare = re.sub(r'"(?:[^"\\\n]|\\.)*"', '""', code_only)  # comments ("the new block") and strings don't count
    if re.search(r"\b(malloc|free|calloc|realloc)\s*\(|\bnew\s+[A-Za-z_]|\bdelete\s", bare):
        errs.append("use game::Mem_Alloc/Mem_Free (core/memory.h), not malloc/free/new/delete")
    for line in code_only.splitlines():
        if re.search(r'"', line) and not re.search(r"^\s*(HOOK|#\s*include)", line):
            stripped = re.sub(r'CSF_STR\(\s*0x[0-9A-Fa-f]+\s*,\s*"(?:[^"\\]|\\.)*"\s*\)', "", line)
            if '"' in stripped:
                errs.append(f"bare string literal (use CSF_STR(0xADDR, \"text\")): {line.strip()[:120]}")
    # a by-value stack local of a generated struct type: auto_classes.h sizes are lower bounds (the fields seen so
    # far). 0x4223F0 built a 12-byte vector in a 4-byte S_00526d40 and overwrote its saved esi and the stack
    # cookie; 5 of 31 such locals in src/auto provably reached past sizeof (2026-10-11)
    for m in struct_locals(code_only):
        errs.append(f"stack local `{m.group(1)} {m.group(2)}`: struct sizes in types/auto_classes.h are lower bounds "
                    "(only the fields seen so far), so a by-value local can be smaller than the object the original "
                    "keeps on its stack, and callees / CSF_AT then write past it. Reserve the bytes the original's "
                    f"frame gives the object (from its `lea reg, [ebp-X]` up to the next local; when unsure, the whole "
                    f"`sub esp, N`): `alignas(8) uint8_t {m.group(2)}Bytes[N];` (add `= {{}}` only if the original zeroes "
                    f"it) and `{m.group(1)}& {m.group(2)} = *reinterpret_cast<{m.group(1)}*>({m.group(2)}Bytes);`")
    db = db or DB()
    keep = re.search(r"HOOK_KEEP_ECX(_CDECL)?\(\s*0x[0-9A-Fa-f]+\s*,\s*&?[\w:]+\s*,\s*(\d+)", src)
    if keep and ea in db.F:  # the wrapper's own ret must match the original's
        own = ret_bytes(ea, db.F[ea]["size"])
        if own == [0] and not keep.group(1) and int(keep.group(2)):
            errs.append(f"HOOK_KEEP_ECX pops its {keep.group(2)} stack argument(s) on return, but the original is __cdecl "
                        "(plain `ret`, the caller pops): register with HOOK_KEEP_ECX_CDECL(addr, Fn, <stack args>, "
                        "\"module\")")
        elif own and own != [0] and keep.group(1):
            errs.append(f"HOOK_KEEP_ECX_CDECL does not pop, but the original returns with ret {own}: use HOOK_KEEP_ECX")
    import addrfix, api
    regs = api.real_regs(ea, db) if ea in db.F else []
    if regs and "HOOK_REGS" not in src and not (regs == ["eax"] and "regparm" in src):
        errs.append(f"the original takes an argument in {', '.join(regs)} (it reads the register before writing it): no "
                    "plain calling convention delivers that. Write `static uint32_t __cdecl Fn(hook::Regs* regs, const "
                    "uint32_t* args)` (regs->eax / ebx / ecx / edx / esi / edi: the registers at entry; args[0] the "
                    "first stack argument; the result is eax) and register it with HOOK_REGS(0xADDR, Fn, N, "
                    "\"module\") where N is the original's `ret N` (0 for a plain `ret`)")
    for m in re.finditer(r"CSF_FUNC\(([^;]*?),\s*(\w+)\s*,\s*(0x[0-9A-Fa-f]+)\s*,", code_only):
        cregs = api.real_regs(int(m.group(3), 16), db)
        if cregs and not (cregs == ["eax"] and "regparm" in m.group(1)):
            errs.append(f"CSF_FUNC {m.group(2)}: {m.group(3)} takes arguments in {', '.join(cregs)}, which no declared "
                        "convention passes. Drop the CSF_FUNC and call it with `hook::Regs r{}; r.<reg> = ...; uint32_t "
                        f"stack[] = {{...}}; hook::call_regs(CSF_ADDR_FN({m.group(3)}), &r, stack, <count>);` (nullptr, 0 "
                        "without stack arguments)")
    # a literal null `this` for a callee that reads ecx: Hex-Rays shows such calls without the object when the
    # original simply leaves ecx as it is (the caller's own this, or a value loaded for something else); writers
    # then pass nullptr. 0x49BAF0, 0x4053E0, 0x4E72C0 (2026-10-11) crashed or sat unexecuted that way
    for m in re.finditer(r"CSF_FUNC\([^;]*?,\s*__fastcall\s*,\s*(\w+)\s*,\s*(0x[0-9A-Fa-f]+)\s*,", code_only):
        if re.search(rf"\b{m.group(1)}\s*\(\s*(?:nullptr|NULL|0)\s*,\s*(?:nullptr|NULL|0)\s*[,)]", code_only) \
                and api.takes_reg(int(m.group(2), 16), db, "ecx"):
            errs.append(f"{m.group(1)}(nullptr, ...): {m.group(2)} reads ecx (`this`), so null crashes it. The original "
                        "passes whatever ecx holds at that call: find the last instruction that wrote ecx before "
                        "the `call` (often none since entry: then it is this function's own `this`; or a `mov ecx, "
                        "[ebp+N]` that loaded an argument) and pass that")
    for p_ in glob.glob(os.path.join(ROOT, "src", "**", "*.cpp"), recursive=True):
        if os.sep + "auto" + os.sep not in p_ and re.search(rf"^HOOK(?:_METHOD|_TRAMP)?\(\s*0x0*{ea:X}\b", open(p_).read(), re.M | re.I):
            errs.append(f"0x{ea:X} is already hooked by {os.path.relpath(p_, ROOT)} (hand-written): a second hook would replace it")
    errs += check_bindings(src, db)
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
        if re.search(rf"CSF_ADDR_FN\(\s*0x0*{ea:x}\s*\)", strip_cpp_comments(src), re.I):
            # the standalone build defines this slot with HOOK below the use (3 files failed the link test on it)
            return False, (f"CSF_ADDR_FN(0x{ea:08X}) is this function's own address: pass your own function by name "
                           "instead (`&YourFunction`, cast as needed)")
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
        if "HOOK_TAIL" in src:
            # fn runs before the jump: it must leave the caller's stack arguments where they are (two register
            # parameters, plain ret); what the original pops on its own return paths is tail_ret's second argument
            own = set(ret_bytes(ea, db.F[ea]["size"]))
            said = tail_ret_pops(code_only)
            if set(rets) != {0}:
                sem.append(f"calling convention mismatch: a HOOK_TAIL function takes exactly (void* self, const uint32_t* "
                           f"args) and pops nothing (yours pops {rets}): its stack arguments still belong to the jump "
                           "target. For a path where the original returns with `ret N` write `return "
                           "hook::tail_ret(eax_value, N);`")
            elif said - own or (own - said and "tail_ret" in code_only):
                sem.append(f"calling convention mismatch: the original's own return paths pop {sorted(own)} bytes "
                           f"(ret N); your hook::tail_ret calls say {sorted(said)}. Write `hook::tail_ret(eax_value, N)` "
                           "for `ret N`, `hook::tail_ret(eax_value)` for a plain `ret`")
        elif "HOOK_REGS" in src:
            # fn is the helper's __cdecl (regs, args); what the original pops is the macro's third argument
            own = set(ret_bytes(ea, db.F[ea]["size"]))
            said = re.search(r"HOOK_REGS\(\s*0x[0-9A-Fa-f]+\s*,\s*&?[\w:]+\s*,\s*(\w+)", src)
            said = int(said.group(1), 0) if said and re.fullmatch(r"0[xX][0-9A-Fa-f]+|\d+", said.group(1)) else None
            if set(rets) != {0}:
                sem.append("calling convention mismatch: a HOOK_REGS function is `uint32_t __cdecl Fn(hook::Regs* regs, "
                           f"const uint32_t* args)` and pops nothing (yours pops {rets})")
            elif own and own != {said}:
                sem.append(f"calling convention mismatch: the original returns with ret {sorted(own)}; HOOK_REGS's third "
                           f"argument says {said} (write the N of `ret N`, 0 for a plain `ret`)")
        elif rets and orig and set(rets) != set(orig):
            sem.append(f"calling convention mismatch: original pops {orig} bytes (ret N), yours pops {rets}. "
                       "Fix the calling convention / parameter list (__thiscall → __fastcall with dummy edx; "
                       "__stdcall pops args; __cdecl pops 0).")
        if re.search(r"catch\s*\(\s*\.\.\.\s*\)", code_only):
            # a handler around code that cannot throw: the compiler drops it and the calls in it (0x47EBA0: a rethrow
            # after a dword copy loop; 0x6A6DD0: the node release after two stores). The original's handler is as
            # dead; the source has it. Callees named inside a catch (...) block are not required in the object.
            dead = set()
            for m in re.finditer(r"catch\s*\(\s*\.\.\.\s*\)\s*\{", code_only):
                k, depth = m.end(), 1
                while k < len(code_only) and depth:
                    depth += {"{": 1, "}": -1}.get(code_only[k], 0)
                    k += 1
                body = code_only[m.end():k]
                for d in re.finditer(r"CSF_FUNC\([^;]*?,\s*(\w+)\s*,\s*(0x[0-9A-Fa-f]+)\s*,", code_only):
                    if re.search(rf"\b{d.group(1)}\s*\(", body):
                        dead.add(int(d.group(2), 16))
            sem = [e for e in sem if not ("your code never does" in e and e.count("\n") == 0 and
                                          any(f"call to 0x{a:X} " in e for a in dead))]
        sem, waived = apply_waivers(ea, obj, sem)
        if sem:
            conv = [e for e in sem if e.startswith("calling convention mismatch")]
            rest = [e for e in sem if e not in conv]
            if rest:
                return False, "semantic check (references must match the original's):\n" + "\n".join(rest)
            return False, conv[0]
        emu = emulated_diff(ea, obj, path, db)
        if emu:
            return False, emu
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
    ("emudiff", r"emulated run differs from the original"),
    ("tail_jump", r"TAIL-JUMPS"),
    ("compile", r"compile error|error:|could not find compiled function"),
    ("raw_address", r"raw original address"),
    ("string", r"bare string literal"),
    ("hook", r"need exactly one HOOK"),
    ("asm", r"inline asm"),
    ("deref", r"double dereference"),
    ("local_struct", r"stack local `"),
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
