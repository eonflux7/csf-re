#!/usr/bin/env python3
"""Recover vtables and a class skeleton list without RTTI.

vtable candidate = .rdata address referenced (as data) by some function, holding a run of .text pointers.
For each vtable: slots, referencing functions (ctors/dtors), and the base vtable inferred from the
QueryInterface-style chain (slot whose body is `if (id == &TypeX) {...} else BaseQI(...)`) and from ctors
that store two vtables in sequence (base ctor inlined/called first).

Writes db/vtables.json (derived, but committed: it is the anchor list for type work).
"""
import json, os, re, struct, collections
import pefile

ROOT = os.path.dirname(os.path.dirname(os.path.dirname(os.path.abspath(__file__))))
pe = pefile.PE(os.path.join(ROOT, "work", "pc", "CommXPC.exe"), fast_load=True)
base = pe.OPTIONAL_HEADER.ImageBase
img = pe.get_memory_mapped_image()
secs = {s.Name.rstrip(b"\0").decode(): (base + s.VirtualAddress, base + s.VirtualAddress + s.Misc_VirtualSize)
        for s in pe.sections}
T, RD = secs[".text"], secs[".rdata"]
u32 = lambda va: struct.unpack_from("<I", img, va - base)[0]

F = [json.loads(l) for l in open(os.path.join(ROOT, "export", "functions.jsonl"))]
fstart = {f["ea"] for f in F}
refs = collections.defaultdict(set)
for f in F:
    for d in f["drefs"]:
        if RD[0] <= d < RD[1]:
            refs[d].add(f["ea"])

cands = sorted(d for d in refs if u32(d) in fstart)
vts = []
for i, va in enumerate(cands):
    nxt = cands[i + 1] if i + 1 < len(cands) else RD[1]
    slots, a = [], va
    while a < nxt and a + 4 <= RD[1]:
        p = u32(a)
        if p not in fstart:
            break
        slots.append(p)
        a += 4
    if len(slots) >= 1:
        vts.append(dict(addr=va, slots=slots, refs=sorted(refs[va])))

# QI chain: find slot functions matching the pattern in pseudocode, extract base QI callee
qi_re = re.compile(r"unknown_libname_\d+\(&(?:unk|dword|byte)_([0-9A-F]+)\)")
qi_of = {}  # qi function -> (typeid, base qi function)
for f in F:
    p = os.path.join(ROOT, "export", "funcs", f"{f['ea']:08x}.c")
    if f["size"] > 200 or not os.path.exists(p):
        continue
    src = open(p).read()
    m = qi_re.search(src)
    if m and "*a3 = " in src:
        callees = [c for c in f["callees"] if not str(c) in src[:0]]
        base_qi = [c for c in f["callees"] if f"sub_{c:X}" in src]
        qi_of[f["ea"]] = (int(m.group(1), 16), base_qi[0] if base_qi else None)

vt_by_qi = {}
for v in vts:
    for k, s in enumerate(v["slots"]):
        if s in qi_of:
            v["qi_slot"], v["typeid"] = k, qi_of[s][0]
            vt_by_qi[s] = v["addr"]
            break
for v in vts:
    if "qi_slot" in v:
        bq = qi_of[v["slots"][v["qi_slot"]]][1]
        v["base"] = vt_by_qi.get(bq)
        v["base_qi"] = bq
out = [dict(addr=f"{v['addr']:08x}", nslots=len(v["slots"]),
            slots=[f"{s:08x}" for s in v["slots"]], refs=[f"{r:08x}" for r in v["refs"]],
            typeid=f"{v['typeid']:08x}" if "typeid" in v else None,
            base=f"{v['base']:08x}" if v.get("base") else None) for v in vts]
json.dump(out, open(os.path.join(ROOT, "db", "vtables.json"), "w"), indent=1)
withqi = sum(1 for v in out if v["typeid"])
withbase = sum(1 for v in out if v["base"])
print(f"vtables {len(out)}  with QI/typeid {withqi}  with resolved base {withbase}  QI funcs {len(qi_of)}")
