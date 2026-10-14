#!/usr/bin/env python3
"""Rewrite raw original-binary addresses in plain code into the address macros of src/globals.h, so the file
links standalone (keyed by address) and behaves the same in the hook phase:

  reinterpret_cast<void**>(0x0082D410)  -> CSF_VTBL(0x0082D410)
  0x830030 (a vtable)                   -> CSF_ADDR_VT(0x830030)
  0x506BD0 (a function start)           -> CSF_ADDR_FN(0x506BD0)
  0x82EB98 (a global / string / data)   -> CSF_ADDR_DATA(0x82EB98)

Only literals that certainly are addresses (function starts, vtables, globals, strings) are touched; macro
arguments, comments and string literals are left alone. Also replaces a CSF_FUNC binding of the file's own
address (self-recursion through the original entry) by a forward declaration of the hooked function.

Usage: addrfix.py [--dry] FILE...      (prints what changed)
"""
import csv, json, os, re, sys

ROOT = os.path.dirname(os.path.dirname(os.path.abspath(__file__)))
IMAGE = (0x401000, 0x8C5000)
_kinds = None


def kinds():
    """address -> 'VT' | 'FN' | 'DATA'"""
    global _kinds
    if _kinds is None:
        k = {}
        p = os.path.join(ROOT, "export", "strings.json")
        if os.path.exists(p):
            k.update({int(a, 16): "DATA" for a in json.load(open(p))})
        try:
            import yaml
            k.update({int(g["addr"]): "DATA" for g in yaml.safe_load(open(os.path.join(ROOT, "db", "globals.yaml"))) or []})
        except OSError:
            pass
        k.update({int(r["ea"], 16): "FN" for r in csv.DictReader(open(os.path.join(ROOT, "db", "functions.csv")))})
        k.update({int(v["addr"], 16): "VT" for v in json.load(open(os.path.join(ROOT, "db", "vtables.json")))})
        _kinds = k
    return _kinds


ASM = re.compile(r"__asm\s*\{[^}]*\}")
MACRO = re.compile(r"\b(CSF_FUNC|CSF_GLOBAL|CSF_STR|CSF_VTBL|CSF_ADDR_\w+|HOOK\w*)\s*\(")


def code_spans(src):
    """(start, end) ranges of plain code: outside comments, string/char literals and CSF_*/HOOK invocations."""
    skip = []
    for m in re.finditer(r"//[^\n]*|/\*.*?\*/|\"(?:[^\"\\\n]|\\.)*\"|'(?:[^'\\\n]|\\.)*'", src, re.S):
        skip.append((m.start(), m.end()))
    # __asm blocks take no C++ expressions: their addresses stay (hook phase only; asm_hits() counts them)
    for m in ASM.finditer(src):
        skip.append((m.start(), m.end()))
    for m in MACRO.finditer(src):
        if any(a <= m.start() < b for a, b in skip):
            continue
        depth, j = 0, m.end() - 1
        while j < len(src):
            depth += {"(": 1, ")": -1}.get(src[j], 0)
            j += 1
            if depth == 0:
                break
        skip.append((m.start(), j))
    skip.sort()
    spans, pos = [], 0
    for a, b in skip:
        if a > pos:
            spans.append((pos, a))
        pos = max(pos, b)
    spans.append((pos, len(src)))
    return spans


LIT = re.compile(r"\b0[xX]([0-9A-Fa-f]{6,8})[uU]?[lL]?\b")


_image = None
DATA_START = 0x82D000  # first address of the data image (.rdata onward)


def binary_kind(v):
    """'VT' | 'DATA' for an address in the data image that no db list names, read from the exe: a word that points
    into .text starts a vtable (secondary vtables such as 0x841E90 are in no list), anything else is data."""
    global _image
    if not DATA_START <= v < IMAGE[1]:
        return None
    if _image is None:
        try:
            import pefile
            pe = pefile.PE(os.path.join(ROOT, "work", "pc", "CommXPC.exe"), fast_load=True)
            _image = pe.get_memory_mapped_image()
        except (OSError, ImportError):
            _image = b""
    if not _image:
        return None
    o = v - 0x400000
    w = int.from_bytes(_image[o:o + 4], "little")
    return "VT" if IMAGE[0] <= w < DATA_START else "DATA"


def raw_hits(src, wanted=None):
    """[(start, end, value, kind)] of raw address literals in plain code (wanted: restrict to these values)."""
    k = kinds()
    out = []
    for a, b in code_spans(src):
        for m in LIT.finditer(src, a, b):
            if m.end() > b:
                continue
            v = int(m.group(1), 16)
            kind = k.get(v) or binary_kind(v) if IMAGE[0] <= v < IMAGE[1] else None
            if kind and (wanted is None or v in wanted):
                out.append((m.start(), m.end(), v, kind))
    return out


def asm_hits(src):
    """Known addresses inside __asm blocks (naked thunks): to rewrite before cutover."""
    k = kinds()
    return sorted({int(m.group(1), 16) for b in ASM.finditer(src) for m in LIT.finditer(b.group(0))
                   if int(m.group(1), 16) in k})


SELF = r"CSF_FUNC\(\s*([^;]+?)\s*,\s*(__\w+)\s*,\s*(\w+)\s*,\s*0[xX]0*{ea}\s*,\s*\((.*?)\)\s*\)\s*;"


def fix_self_binding(src, ea):
    """CSF_FUNC(ret, conv, Name, <own ea>, (args)) -> calls go to the hooked function itself (declared ahead)."""
    m = re.search(SELF.format(ea=f"{ea:X}"), src, re.I | re.S)
    if not m:
        return src
    h = re.search(r"HOOK\(\s*0[xX][0-9A-Fa-f]+\s*,\s*&?(\w+)", src)
    if not h:
        return src
    ret, conv, name, args = m.group(1), m.group(2), m.group(3), m.group(4)
    fn = h.group(1)
    decl = f"static {ret} {conv} {fn}({args});  // self-recursion: call the reimplementation directly"
    src = src[:m.start()] + decl + src[m.end():]
    if name != fn:
        src = re.sub(rf"\b{re.escape(name)}\b", fn, src)
    return src


def fix(src, ea=None):
    if ea is not None:
        src = fix_self_binding(src, ea)
    k = kinds()

    def vt_cast(m):
        v = int(m.group(1), 16)
        return f"CSF_VTBL({m.group(1)})" if k.get(v) == "VT" else m.group(0)
    # whole cast first (reads better), then any remaining literal in place (keeps the expression's integer type)
    spans = code_spans(src)
    out, pos = [], 0
    for a, b in spans:
        out.append(src[pos:a])
        out.append(re.sub(r"reinterpret_cast<\s*void\s*\*\s*\*\s*>\(\s*(0[xX][0-9A-Fa-f]{6,8})[uU]?\s*\)", vt_cast,
                          src[a:b]))
        pos = b
    out.append(src[pos:])
    src = "".join(out)
    hits = raw_hits(src)
    for s, e, v, kind in reversed(hits):
        lit = re.sub(r"[uUlL]+$", "", src[s:e])
        src = src[:s] + f"CSF_ADDR_{kind}({lit})" + src[e:]
    # an address from a symbol (standalone) is not a constant expression: constexpr -> const
    src = re.sub(r"\bconstexpr\b(?=[^;{]*=\s*[^;]*\bCSF_(ADDR_\w+|VTBL)\s*\()", "const", src)
    if re.search(r"\bCSF_", src) and not re.search(r'#\s*include\s*"globals.h"', src):
        m = re.search(r"^#\s*include[^\n]*\n", src, re.M)
        at = m.end() if m else 0
        src = src[:at] + '#include "globals.h"\n' + src[at:]
    return src


if __name__ == "__main__":
    args = sys.argv[1:]
    dry = "--dry" in args
    n = 0
    for p in [a for a in args if a != "--dry"]:
        src = open(p).read()
        m = re.match(r"([0-9a-fA-F]{8})\.cpp$", os.path.basename(p))
        new = fix(src, int(m.group(1), 16) if m else None)
        if new != src:
            n += 1
            print(p)
            if not dry:
                open(p, "w").write(new)
    print(f"{n} file(s) {'would change' if dry else 'changed'}")
