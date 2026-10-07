#!/usr/bin/env python3
"""Gate for one reimplemented function file.

Checks: allowed includes only, no inline asm, exactly one HOOK for the target address, compiles with the real
build flags (static_asserts included), referenced addresses/field offsets match the original (semcheck.py),
and the hooked function's `ret N` matches the original.

Usage: gate.py <ea-hex> <file.cpp>      exit 0 = pass; errors on stdout
"""
import json, os, re, shlex, subprocess, sys, tempfile
sys.path.insert(0, os.path.dirname(__file__))
from packet import ret_bytes, DB
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


def check_bindings(src, db):
    """Every CSF_FUNC binding must match the callee's real `ret N` (cdecl pops 0)."""
    errs = []
    for m in re.finditer(r"CSF_FUNC\(\s*(.+?)\s*,\s*(__\w+)\s*,\s*(\w+)\s*,\s*0x([0-9A-Fa-f]+)\s*,\s*\((.*?)\)\s*\)",
                         src, re.S):
        conv, name, addr, params = m.group(2), m.group(3), int(m.group(4), 16), m.group(5)
        if addr not in db.F:
            errs.append(f"CSF_FUNC {name}: 0x{addr:X} is not a function start")
            continue
        rets = ret_bytes(addr, db.F[addr]["size"])
        if not rets:
            continue  # no ret (tail jump / noreturn): can't check
        want = 0 if conv == "__cdecl" else stack_bytes(conv, params)
        if conv == "__cdecl" and rets != [0]:
            errs.append(f"CSF_FUNC {name}: declared __cdecl but 0x{addr:X} pops {rets} bytes (use __stdcall, or "
                        f"__fastcall with ecx/edx for __thiscall)")
        elif conv != "__cdecl" and want not in rets:
            errs.append(f"CSF_FUNC {name}: declared {conv} popping {want} bytes but 0x{addr:X} pops {rets}")
    return errs


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
    # the only allowed asm: a naked tail-jump thunk — at most 8 register moves/tests/branches around one indirect jmp
    for blk in re.findall(r"__asm\s*\{([^}]*)\}", src):
        ins = [l.strip() for l in re.split(r"[\n;]", blk) if l.strip() and not re.fullmatch(r"\w+:", l.strip())]
        bad = [x for x in ins if not re.match(r"(mov|lea|add|sub|test|cmp|je|jne|jz|jnz|ret|call\s+e[a-z]x\b|jmp)\b", x)]
        if bad or len(ins) > 8 or not any(re.match(r"jmp\s+(dword\s+ptr\s+(ds:)?\[|e[a-z]x\b|0x)", x) for x in ins):
            errs.append("inline asm allowed only as a naked tail-jump thunk (<= 8 of mov/lea/add/sub/test/cmp/je/jne/"
                        "ret/call reg, ending in an indirect jmp); offending: " + "; ".join(bad[:3] or ins[:3]))
    if re.search(r"\b(__asm|asm)\b", re.sub(r"__asm\s*\{[^}]*\}", "", src)):
        errs.append("inline asm only as `__asm { ... }` blocks in a naked thunk")
    hooks = re.findall(r"HOOK(?:_METHOD)?\(\s*0x([0-9A-Fa-f]+)\s*,\s*&?([\w:]+)", src)
    if len(hooks) != 1 or int(hooks[0][0], 16) != ea:
        errs.append(f"need exactly one HOOK(0x{ea:X}, Fn, \"module\") line; found {hooks}")
    if re.search(r"\b(malloc|free|calloc|realloc)\s*\(|\bnew\s+[A-Za-z_]|\bdelete\s", src):
        errs.append("use game::Mem_Alloc/Mem_Free (core/memory.h), not malloc/free/new/delete")
    code_only = re.sub(r"//[^\n]*|/\*.*?\*/", "", src, flags=re.S)
    for line in code_only.splitlines():
        if re.search(r'"', line) and not re.search(r"^\s*(HOOK|#\s*include)", line):
            stripped = re.sub(r'CSF_STR\(\s*0x[0-9A-Fa-f]+\s*,\s*"(?:[^"\\]|\\.)*"\s*\)', "", line)
            if '"' in stripped:
                errs.append(f"bare string literal (use CSF_STR(0xADDR, \"text\")): {line.strip()[:120]}")
    db = db or DB()
    errs += check_bindings(src, db)
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
        fn = hooks[0][1].split("::")[-1]
        found, rets = our_rets(obj, fn)
        orig = ret_bytes(ea, db.F[ea]["size"])
        if not found:
            return False, f"could not find compiled function `{fn}` in object file"
        sem = semcheck.check(ea, obj, db)
        if sem:
            return False, "semantic check (references must match the original's):\n" + "\n".join(sem)
        if rets and orig and set(rets) != set(orig):
            return False, (f"calling convention mismatch: original pops {orig} bytes (ret N), yours pops {rets}. "
                           "Fix the calling convention / parameter list (__thiscall → __fastcall with dummy edx; "
                           "__stdcall pops args; __cdecl pops 0).")
    return True, "ok"


if __name__ == "__main__":
    ok, msg = gate(int(sys.argv[1], 16), sys.argv[2])
    print(msg)
    sys.exit(0 if ok else 1)
