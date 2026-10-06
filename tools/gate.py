#!/usr/bin/env python3
"""Gate for one reimplemented function file.

Checks: allowed includes only, no inline asm, exactly one HOOK for the target address, compiles with the real
build flags (static_asserts included), and the hooked function's `ret N` matches the original.

Usage: gate.py <ea-hex> <file.cpp>      exit 0 = pass; errors on stdout
"""
import json, os, re, shlex, subprocess, sys, tempfile
sys.path.insert(0, os.path.dirname(__file__))
from packet import ret_bytes, DB

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
            cur = m.group(1)
            continue
        if cur and re.search(rf"\b{re.escape(fn_name)}\b|\?{re.escape(fn_name)}@", cur):
            found = True
            m2 = re.search(r"\tret\s*(\S*)", line)
            if m2:
                rets.add(int(m2.group(1), 16) if m2.group(1) else 0)
    return found, sorted(rets)


def gate(ea, path, db=None):
    path = os.path.abspath(path)
    src = open(path).read()
    errs = []
    for inc in re.findall(r'#\s*include\s*[<"]([^>"]+)[>"]', src):
        if inc not in ALLOWED_INC:
            errs.append(f"include not allowed: {inc}")
    if re.search(r"\b(__asm|asm)\b", src):
        errs.append("inline asm not allowed")
    hooks = re.findall(r"HOOK(?:_METHOD)?\(\s*0x([0-9A-Fa-f]+)\s*,\s*&?([\w:]+)", src)
    if len(hooks) != 1 or int(hooks[0][0], 16) != ea:
        errs.append(f"need exactly one HOOK(0x{ea:X}, Fn, \"module\") line; found {hooks}")
    if re.search(r"\b(malloc|free|calloc|realloc)\s*\(|\bnew\s+[A-Za-z_]|\bdelete\s", src):
        errs.append("use game::Mem_Alloc/Mem_Free (core/memory.h), not malloc/free/new/delete")
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
        db = db or DB()
        orig = ret_bytes(ea, db.F[ea]["size"])
        if not found:
            return False, f"could not find compiled function `{fn}` in object file"
        if rets and orig and set(rets) != set(orig):
            return False, (f"calling convention mismatch: original pops {orig} bytes (ret N), yours pops {rets}. "
                           "Fix the calling convention / parameter list (__thiscall → __fastcall with dummy edx; "
                           "__stdcall pops args; __cdecl pops 0).")
    return True, "ok"


if __name__ == "__main__":
    ok, msg = gate(int(sys.argv[1], 16), sys.argv[2])
    print(msg)
    sys.exit(0 if ok else 1)
