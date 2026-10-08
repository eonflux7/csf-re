#!/usr/bin/env python3
"""Generate the differential-test table for hooked functions (build/diff_table.inc, included by hook/diff.cpp).

A function qualifies when running the original and ours back to back is safe and comparable: no calls at all
(leaf, incl. no tail jumps out), and every memory write goes through `this` (ecx, never reassigned) or to an
absolute global. The harness snapshots `this` (bytes the function can touch) and the written globals, runs the
original, restores, runs ours, and compares return value + memory.

Usage: difftable.py [--out build/diff_table.inc]
"""
import glob, os, re, sys
sys.path.insert(0, os.path.dirname(__file__))
from packet import DB, disasm, ret_bytes, pseudo

ROOT = os.path.dirname(os.path.dirname(os.path.abspath(__file__)))
WRITE_OPS = {"mov", "add", "sub", "or", "and", "xor", "inc", "dec", "fstp", "fst", "fistp", "fist", "neg", "not",
             "shl", "shr", "sar", "xchg", "movss", "movsd", "adc", "sbb", "setne", "sete", "movsx", "movzx"}
SIZE = {"byte": 1, "word": 2, "dword": 4, "qword": 8}
CONV = {"__cdecl": 0, "__stdcall": 1, "__thiscall": 2, "__fastcall": 3}


def analyze(db, ea):
    f = db.F[ea]
    ins = list(disasm(ea, f["size"]))
    for i in ins:
        if i.mnemonic == "call" or i.mnemonic.startswith("rep") or i.mnemonic in ("int3", "int"):
            return None
        if i.mnemonic == "jmp" or i.mnemonic.startswith("j"):
            if not i.op_str.startswith("0x") or not (f["ea"] <= int(i.op_str, 16) < f["end"]):
                return None
    if any(re.match(r"(mov|lea|xchg|pop|xor|sub|add|and|or) ecx\b", f"{i.mnemonic} {i.op_str}") for i in ins):
        uses_ecx_as_this = False
    else:
        uses_ecx_as_this = True
    this_max, globs = 0, {}
    fl_this, fl_glob = {}, {}  # FPU/SSE stores: offset -> 4 (float) / 8 (double); compared with a tolerance
    for i in ins:
        for m in re.finditer(r"(byte|word|dword|qword) ptr \[ecx(?: \+ (0x[0-9a-f]+|\d+))?\]", i.op_str):
            this_max = max(this_max, int(m.group(2) or "0", 0) + SIZE[m.group(1)])
        first = i.op_str.split(",")[0]
        mw = re.match(r"(byte|word|dword|qword) ptr \[(.*?)\]", first)
        is_write = i.mnemonic in WRITE_OPS and mw and (i.mnemonic not in ("movsx", "movzx"))
        if i.mnemonic in ("fstp", "fst", "fistp", "fist") and mw:
            is_write = True
        if not is_write:
            continue
        inner = mw.group(2)
        if i.mnemonic in ("fstp", "fst", "movss", "movsd") and mw.group(1) in ("dword", "qword"):
            n = 4 if mw.group(1) == "dword" else 8
            if re.fullmatch(r"0x[0-9a-f]+", inner):
                fl_glob[int(inner, 16)] = n
            else:
                mo = re.fullmatch(r"ecx(?: \+ (0x[0-9a-f]+|\d+))?", inner)
                if mo:
                    fl_this[int(mo.group(1) or "0", 0)] = n
        if re.fullmatch(r"0x[0-9a-f]+", inner):
            a = int(inner, 16)
            globs[a] = max(globs.get(a, 0), SIZE[mw.group(1)])
        elif re.fullmatch(r"ecx(?: \+ (0x[0-9a-f]+|\d+))?", inner) and uses_ecx_as_this:
            pass
        else:
            return None  # writes through another pointer / the stack args: not snapshot-able
    src = pseudo(ea) or ""
    proto = src.splitlines()[0] if src else ""
    conv = next((v for k, v in CONV.items() if k in proto), 0)
    rets = ret_bytes(ea, f["size"])
    argbytes = rets[0] if rets and rets[0] else 0
    if conv == 0:  # cdecl: count parameters in the prototype
        m = re.search(r"\((.*)\)\s*$", proto)
        params = [p for p in (m.group(1).split(",") if m else []) if p.strip() and p.strip() != "void"]
        argbytes = sum(8 if re.search(r"\b(double|__int64)\b(?!\s*\*)", p) else 4 for p in params)
        if rets and rets[0]:
            conv, argbytes = 1, rets[0]  # pops: actually stdcall
    if conv in (1, 2) and rets and rets[0] != argbytes:
        argbytes = rets[0]
    ret = 3 if re.match(r"(void)\b", proto) and "*" not in proto.split("(")[0] else \
        1 if re.match(r"float\b", proto) else 2 if re.match(r"double\b", proto) else \
        4 if re.match(r"(unsigned )?__int64\b", proto) else \
        5 if re.match(r"(unsigned )?(char|bool|__int8|_BYTE)\b", proto) and "*" not in proto.split("(")[0] else \
        6 if re.match(r"(unsigned )?(__int16|short|_WORD|wchar_t)\b", proto) and "*" not in proto.split("(")[0] else 0
    if conv not in (2, 3):
        this_max = 0
        fl_this = {}
    gl = sorted(globs.items())[:4]
    # float positions in the snapshot (this bytes first, then the globals in table order); 8-byte = double (+0x8000)
    fl, pos = [], this_max
    fl += [o | (0x8000 if n == 8 else 0) for o, n in sorted(fl_this.items()) if o + n <= this_max]
    for a, size in gl:
        for fa, n in fl_glob.items():
            if a <= fa and fa + n <= a + size:
                fl.append((pos + fa - a) | (0x8000 if n == 8 else 0))
        pos += size
    return dict(conv=conv, argbytes=argbytes, ret=ret, this_size=this_max, globs=gl, fl=sorted(fl)[:8])


def main():
    out = sys.argv[sys.argv.index("--out") + 1] if "--out" in sys.argv else os.path.join(ROOT, "build", "diff_table.inc")
    db = DB()
    rows = []
    for p in sorted(glob.glob(os.path.join(ROOT, "src", "auto", "*.cpp"))):
        ea = int(os.path.basename(p)[:8], 16)
        if ea not in db.F:
            continue
        d = analyze(db, ea)
        if d:
            g = ", ".join(f"{{0x{a:X}, {n}}}" for a, n in d["globs"])
            fl = ", ".join(f"0x{x:X}" for x in d["fl"])
            rows.append(f"    {{0x{ea:08X}, {d['conv']}, {d['argbytes']}, {d['ret']}, {d['this_size']}, "
                        f"{len(d['globs'])}, {{{g}}}, {len(d['fl'])}, {{{fl}}}}},")
    os.makedirs(os.path.dirname(out), exist_ok=True)
    with open(out, "w") as f:
        f.write("// generated by tools/difftable.py: {addr, conv(0 cdecl 1 stdcall 2 thiscall 3 fastcall), argbytes,\n"
                "// ret(0 int32 1 float 2 double 3 void 4 int64 5 int8 6 int16), this_size, nglobs, {{addr, size}...},\n"
                "// nfl, {snapshot offset of a float store (+0x8000: double)...}}\n")
        f.write("\n".join(rows) + "\n")
    print(f"{len(rows)} diffable functions -> {os.path.relpath(out, ROOT)}")


if __name__ == "__main__":
    main()
