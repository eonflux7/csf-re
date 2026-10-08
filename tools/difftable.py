#!/usr/bin/env python3
"""Generate the differential-test table for hooked functions (build/diff_table.inc, included by hook/diff.cpp).

A function qualifies when running the original and ours back to back is safe and comparable: no calls except to
pure functions (tier 2: no writes outside their own frame, transitively - running them twice changes nothing; no
tail jumps out), and every memory write goes through `this` (ecx, never reassigned) or to an absolute global. The harness snapshots `this` (bytes the function can touch) and the written globals, runs the
original, restores, runs ours, and compares return value + memory.

Usage: difftable.py [--out build/diff_table.inc]
"""
import glob, os, re, sys
sys.path.insert(0, os.path.dirname(__file__))
from packet import DB, disasm, ret_bytes, pseudo, proto_of

ROOT = os.path.dirname(os.path.dirname(os.path.abspath(__file__)))
WRITE_OPS = {"mov", "add", "sub", "or", "and", "xor", "inc", "dec", "fstp", "fst", "fistp", "fist", "neg", "not",
             "shl", "shr", "sar", "xchg", "movss", "movsd", "adc", "sbb", "setne", "sete", "movsx", "movzx"}
SIZE = {"byte": 1, "word": 2, "dword": 4, "qword": 8}
CONV = {"__cdecl": 0, "__stdcall": 1, "__thiscall": 2, "__fastcall": 3}


PURE_CACHE = os.path.join(ROOT, "work", "pure_cache.json")  # the binary never changes: purity is computed once
_pure = None


def _pure_load():
    global _pure
    if _pure is None:
        import json
        try:
            _pure = {int(k, 16): v for k, v in json.load(open(PURE_CACHE)).items()}
        except (OSError, ValueError):
            _pure = {}
    return _pure


def _pure_save():
    import json
    os.makedirs(os.path.dirname(PURE_CACHE), exist_ok=True)
    json.dump({f"{k:08x}": v for k, v in _pure.items()}, open(PURE_CACHE, "w"))


def is_pure(db, ea, depth=0):
    """A function the diff may run twice (tier 2: callees of a diffed function run once for the original, once for
    ours): no memory writes except its own stack frame (esp/ebp-relative), no indirect calls/jumps, no rep/int, and
    every direct callee pure too. Getters, math, string compares. Cached in work/pure_cache.json."""
    pure = _pure_load()
    if ea in pure:
        return pure[ea]
    f = db.F.get(ea)
    if f is None or depth > 12:
        return False
    pure[ea] = False  # recursion guard (a cycle is not proven pure)
    ok = True
    for i in disasm(ea, min(f["size"], 4000)):
        m = i.mnemonic
        if m.startswith("rep") or m in ("int", "int3", "cpuid", "stosd", "stosb", "movsd", "movsb"):
            ok = False
        elif m == "call" or (m == "jmp" and not (i.op_str.startswith("0x") and f["ea"] <= int(i.op_str, 16) < f["end"])):
            ok = i.op_str.startswith("0x") and is_pure(db, int(i.op_str, 16), depth + 1)
        else:
            mw = re.match(r"(byte|word|dword|qword|tbyte) ptr (?:\w+:)?\[(.*?)\]", i.op_str.split(",")[0])
            if mw and (m in WRITE_OPS or m in ("fstp", "fst", "fistp", "fist", "inc", "dec")) \
                    and m not in ("movsx", "movzx") and not re.match(r"(esp|ebp)\b", mw.group(2)):
                ok = False
        if not ok:
            break
    pure[ea] = ok
    return ok


ALIAS_MOV = re.compile(r"mov (e[a-z]{2}), (e[a-z]{2})$")
REG_WRITE = re.compile(r"(?:mov|lea|xchg|pop|xor|sub|add|and|or|inc|dec|shl|shr|sar|imul|movzx|movsx|neg|not|"
                       r"cmov\w+|set\w+) (e[a-z]{2}|[a-d][lhx]|[sd]il?|bpl?)\b")
SUBREG = {"al": "eax", "ah": "eax", "ax": "eax", "bl": "ebx", "bh": "ebx", "bx": "ebx", "cl": "ecx", "ch": "ecx",
          "cx": "ecx", "dl": "edx", "dh": "edx", "dx": "edx", "si": "esi", "di": "edi", "bp": "ebp"}
CLOBBERED = ("eax", "ecx", "edx")  # by any call


def effects(db, ea, depth=0, _memo={}):
    """What running `ea` once writes, when everything it writes can be snapshotted: (this_max, {global: size},
    {this float offset: n}, {global float addr: n}) — `this` = ecx at entry and every register copied from it (MSVC's
    `mov esi, ecx`); callees are pure, or contained themselves when called with ecx = this (their writes merge in).
    None when it writes through any other pointer, calls anything else, or jumps out (running it twice in the
    diff would then repeat side effects the snapshot can't undo)."""
    if ea in _memo:
        return _memo[ea]
    f = db.F.get(ea)
    if f is None or depth > 8 or f["size"] > 4000:
        return None
    _memo[ea] = None  # recursion guard
    ins = list(disasm(ea, f["size"]))
    alias = {"ecx"}
    this_max, globs, fl_this, fl_glob = 0, {}, {}, {}
    this_re = None

    def this_off(inner):
        mo = re.fullmatch(r"(e[a-z]{2})(?: \+ (0x[0-9a-f]+|\d+))?", inner)
        return int(mo.group(2) or "0", 0) if mo and mo.group(1) in alias else None

    for i in ins:
        m, ops = i.mnemonic, i.op_str
        if m.startswith(("rep", "stos", "movs", "lods", "scas", "cmps")) and "ptr" in ops or m.startswith("rep") \
                or m in ("int3", "int", "into"):
            return None
        if m == "jmp" or m.startswith("j"):
            if not ops.startswith("0x") or not (f["ea"] <= int(ops, 16) < f["end"]):
                return None
            continue
        if m == "call":
            if not ops.startswith("0x"):
                return None
            t = int(ops, 16)
            if not is_pure(db, t):
                sub = effects(db, t, depth + 1) if "ecx" in alias else None
                if sub is None:
                    return None
                this_max = max(this_max, sub[0])
                for a2, n in sub[1].items():
                    globs[a2] = max(globs.get(a2, 0), n)
                fl_this.update(sub[2])
                fl_glob.update(sub[3])
            alias -= set(CLOBBERED)
            continue
        for mm in re.finditer(r"(byte|word|dword|qword) ptr \[(e[a-z]{2})(?: \+ (0x[0-9a-f]+|\d+))?\]", ops):
            if mm.group(2) in alias:  # reads count too: the snapshot must hold every byte the function looks at
                this_max = max(this_max, int(mm.group(3) or "0", 0) + SIZE[mm.group(1)])
        mw = re.match(r"(byte|word|dword|qword) ptr (?:\w+:)?\[(.*?)\]", ops.split(",")[0])
        is_write = mw and ((m in WRITE_OPS and m not in ("movsx", "movzx")) or m in ("fstp", "fst", "fistp", "fist"))
        if is_write:
            inner = mw.group(2)
            n = SIZE[mw.group(1)]
            fpw = m in ("fstp", "fst", "movss", "movsd") and n in (4, 8)
            if re.fullmatch(r"0x[0-9a-f]+", inner):
                a2 = int(inner, 16)
                globs[a2] = max(globs.get(a2, 0), n)
                if fpw:
                    fl_glob[a2] = n
            elif this_off(inner) is not None:
                if fpw:
                    fl_this[this_off(inner)] = n
            elif not re.match(r"(esp|ebp)\b", inner):
                return None  # another pointer: not snapshot-able
            continue
        # register effects: copies of `this` stay `this`, anything else written to a register drops it
        cp = ALIAS_MOV.match(f"{m} {ops}")
        if cp:
            (alias.add if cp.group(2) in alias else alias.discard)(cp.group(1))
            continue
        rw = REG_WRITE.match(f"{m} {ops}")
        if rw:
            alias.discard(SUBREG.get(rw.group(1), rw.group(1)))
    _memo[ea] = (this_max, globs, fl_this, fl_glob)
    return _memo[ea]


def analyze(db, ea):
    f = db.F[ea]
    eff = effects(db, ea)
    if eff is None:
        return None
    this_max, globs, fl_this, fl_glob = eff
    if len(globs) > 8 or this_max > 12288:  # hook/diff.cpp: Snap buf 16 KiB, globs[8]
        return None
    src = pseudo(ea) or ""
    proto = proto_of(src)
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
    gl = sorted(globs.items())
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
    if _pure is not None:
        _pure_save()
    print(f"{len(rows)} diffable functions -> {os.path.relpath(out, ROOT)}")


if __name__ == "__main__":
    main()
