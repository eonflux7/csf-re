#!/usr/bin/env python3
"""Extract MSVC x86 RTTI from the PE: classes, base hierarchy, vtables and their slots.

Writes db/rtti.json. Pure PE parsing, no IDA needed.
"""
import json, os, struct, subprocess, sys
import pefile

ROOT = os.path.dirname(os.path.dirname(os.path.dirname(os.path.abspath(__file__))))
EXE = os.path.join(ROOT, "work", "pc", "CommXPC.exe")
OUT = os.path.join(ROOT, "db", "rtti.json")

pe = pefile.PE(EXE, fast_load=True)
base = pe.OPTIONAL_HEADER.ImageBase
img = pe.get_memory_mapped_image()
secs = {s.Name.rstrip(b"\0").decode(): (base + s.VirtualAddress, base + s.VirtualAddress + s.Misc_VirtualSize)
        for s in pe.sections}
text = secs[".text"]


def u32(va):
    return struct.unpack_from("<I", img, va - base)[0]


def valid(va):
    return base <= va < base + len(img) - 4


def in_text(va):
    return text[0] <= va < text[1]


def cstr(va):
    off = va - base
    end = img.index(b"\0", off)
    return img[off:end].decode("latin-1")


def demangle_all(raw_names):
    # ".?AVFoo@Bar@@" -> vftable symbol "??_7Foo@Bar@@6B@" -> "const Bar::Foo::`vftable'"
    syms = ["??_7" + n[4:] + "6B@" for n in raw_names]
    out = subprocess.run(["llvm-undname-21"], input="\n".join(syms), capture_output=True, text=True).stdout
    res = {}
    lines = [l for l in out.splitlines() if l.strip()]
    # llvm-undname echoes "input\noutput\n\n" per symbol
    it = iter(lines)
    for raw, sym in zip(raw_names, syms):
        for l in it:
            if l == sym:
                d = next(it)
                d = d.removeprefix("const ").removesuffix("::`vftable'")
                res[raw] = d
                break
    return res


# 1. type descriptors: "\.?AV" / ".?AU" names; TD = name_va - 8
tds = {}
for kind in (b".?AV", b".?AU"):
    i = 0
    while (i := img.find(kind, i)) != -1:
        va = base + i
        name = cstr(va)
        if name.endswith("@@"):
            tds[va - 8] = name
        i += 1

# 2. complete object locators: scan .rdata for sig==0 && pTD in tds
cols = {}
rd = secs[".rdata"]
for va in range(rd[0], rd[1] - 20, 4):
    sig, off, cdoff, ptd, pchd = struct.unpack_from("<5I", img, va - base)
    if sig == 0 and ptd in tds and valid(pchd):
        cols[va] = dict(td=ptd, offset=off, cdoffset=cdoff, chd=pchd)


def hierarchy(chd):
    sig, attr, nbases, parr = struct.unpack_from("<4I", img, chd - base)
    bases = []
    for k in range(nbases):
        bcd = u32(parr + 4 * k)
        ptd, ncont, mdisp, pdisp, vdisp, battr = struct.unpack_from("<IIiiiI", img, bcd - base)
        bases.append(dict(td=ptd, mdisp=mdisp, pdisp=pdisp, vdisp=vdisp, ncont=ncont))
    return attr, bases


# 3. vtables: a pointer to a COL in .rdata, followed by code pointers
col_refs = {}
for va in range(rd[0], rd[1] - 4, 4):
    p = u32(va)
    if p in cols:
        col_refs.setdefault(p, []).append(va + 4)

all_vt_starts = set(v for vs in col_refs.values() for v in vs)
classes = {}
for col_va, col in cols.items():
    raw = tds[col["td"]]
    attr, bases = hierarchy(col["chd"])
    c = classes.setdefault(raw, dict(raw=raw, td=col["td"], attributes=attr,
                                     bases=[tds.get(b["td"], hex(b["td"])) for b in bases[1:]],
                                     base_offsets=[b["mdisp"] for b in bases[1:]],
                                     vtables=[]))
    for vt in col_refs.get(col_va, []):
        slots = []
        a = vt
        while True:
            p = u32(a)
            if not in_text(p):
                break
            if a != vt and a in all_vt_starts:  # ran into the next vtable
                break
            slots.append(p)
            a += 4
        c["vtables"].append(dict(addr=vt, offset=col["offset"], slots=slots))

names = demangle_all(sorted(classes))
for raw, c in classes.items():
    c["name"] = names.get(raw, raw)
    c["bases"] = [names.get(b, b) for b in c["bases"]]
    c["vtables"].sort(key=lambda v: v["offset"])

os.makedirs(os.path.dirname(OUT), exist_ok=True)
out = sorted(classes.values(), key=lambda c: c["name"])
with open(OUT, "w") as f:
    json.dump(out, f, indent=1)
nv = sum(len(c["vtables"]) for c in out)
ns = sum(len(v["slots"]) for c in out for v in c["vtables"])
print(f"type descriptors {len(tds)}  classes w/ COL {len(out)}  vtables {nv}  slots {ns}")
