#!/usr/bin/env python3
"""Build the minimal context packet for reimplementing one function.

Usage: packet.py <ea-hex>     (prints the packet)
"""
import csv, json, os, re, sys
import pefile
from capstone import Cs, CS_ARCH_X86, CS_MODE_32

ROOT = os.path.dirname(os.path.dirname(os.path.abspath(__file__)))
_pe = None


class DB:
    def __init__(self):
        self.rows = {int(r["ea"], 16): r for r in csv.DictReader(open(os.path.join(ROOT, "db", "functions.csv")))}
        self.F = {}
        for l in open(os.path.join(ROOT, "export", "functions.jsonl")):
            f = json.loads(l)
            self.F[f["ea"]] = f
        self.classes = {c["vtable"]: c for c in json.load(open(os.path.join(ROOT, "db", "classes.json")))}
        self.owner = {}
        for vt, c in self.classes.items():
            for m in c["methods"]:
                self.owner.setdefault(int(m, 16), []).append(vt)
        self.globals = {}
        gp = os.path.join(ROOT, "db", "globals.yaml")
        if os.path.exists(gp):
            import yaml
            for g in yaml.safe_load(open(gp)) or []:
                self.globals[int(g["addr"])] = g

    def name(self, ea):
        r = self.rows.get(ea)
        return (r["name"] if r and r["name"] else None)

    def class_of(self, ea):
        """Most-base class among owners (implementation lives in the class that introduced it)."""
        owners = self.owner.get(ea, [])
        if not owners:
            return None
        def ancestors(vt):
            seen = []
            while vt and vt in self.classes and vt not in seen:
                seen.append(vt)
                vt = self.classes[vt]["base"]
            return seen
        for o in owners:
            if all(o in ancestors(p) for p in owners):
                return o
        return sorted(owners, key=lambda o: self.classes[o]["size"] or 1 << 30)[0]

    def cls_name(self, vt):
        return f"C_{vt}"


def disasm(ea, size):
    global _pe
    if _pe is None:
        _pe = pefile.PE(os.path.join(ROOT, "work", "pc", "CommXPC.exe"), fast_load=True)
    img = _pe.get_memory_mapped_image()
    md = Cs(CS_ARCH_X86, CS_MODE_32)
    return list(md.disasm(img[ea - 0x400000: ea - 0x400000 + size], ea))


def ret_bytes(ea, size):
    rets = {int(i.op_str, 16) if i.op_str else 0 for i in disasm(ea, size) if i.mnemonic == "ret"}
    return sorted(rets)


def pseudo(ea):
    p = os.path.join(ROOT, "export", "funcs", f"{ea:08x}.c")
    return open(p).read() if os.path.exists(p) else None


def substitute_names(db, text):
    def rep(m):
        ea = int(m.group(1), 16)
        n = db.name(ea)
        return n if n else m.group(0)
    return re.sub(r"\bsub_([0-9A-F]+)\b", rep, text)


def struct_text(db, vt):
    hdr = open(os.path.join(ROOT, "src", "types", "auto_classes.h")).read()
    m = re.search(rf"struct C_{vt}\b.*?\n}};\n(static_assert[^\n]*\n)?", hdr, re.S)
    return m.group(0) if m else None


def build(db, ea, with_asm_limit=160):
    f = db.F[ea]
    row = db.rows[ea]
    src = pseudo(ea)
    if src is None:
        raise SystemExit(f"no pseudocode for {ea:08x}")
    out = []
    rets = ret_bytes(ea, f["size"])
    proto = src.splitlines()[0]
    out.append(f"## Target function 0x{ea:08X} (size {f['size']} bytes, region {row['region']})")
    out.append(f"Hex-Rays prototype: `{proto}`")
    out.append(f"Original `ret` immediate(s): {rets}  (stack bytes the callee pops; must match exactly)")
    vt = db.class_of(ea)
    if vt:
        c = db.classes[vt]
        out.append(f"\n## `this` class: C_{vt} (vtable 0x{vt.upper()}, size {c['size'] or 'unknown'}, "
                   f"base {('C_' + c['base']) if c['base'] else 'none'})")
        st = struct_text(db, vt)
        if st:
            out.append("Declared in types/auto_classes.h (DO NOT redefine; use as-is):\n```cpp\n" + st + "```")
        if c["base"] and struct_text(db, c["base"]):
            out.append("Base:\n```cpp\n" + struct_text(db, c["base"]) + "```")
    out.append("\n## Hex-Rays pseudocode (names substituted where known)\n```c\n" + substitute_names(db, src) + "\n```")
    if f["size"] <= with_asm_limit:
        out.append("\n## Disassembly (authoritative for signedness / exact semantics)\n```asm\n" +
                   "\n".join(f"{i.address:08x}  {i.mnemonic} {i.op_str}" for i in disasm(ea, f["size"])) + "\n```")
    if f["callees"]:
        out.append("\n## Callees")
        for c in f["callees"]:
            cr = db.rows.get(c)
            cs = pseudo(c)
            cproto = cs.splitlines()[0] if cs else "?"
            status = cr["status"] if cr else "?"
            nm = db.name(c) or f"sub_{c:X}"
            out.append(f"- 0x{c:08X} `{nm}` [{status}] rets={ret_bytes(c, db.F[c]['size']) if c in db.F else '?'} "
                       f"proto: `{cproto}`")
    gl = [d for d in f["drefs"]]
    if gl:
        out.append("\n## Data referenced\n" + "\n".join(
            f"- 0x{d:08X}" + (f" `{db.globals[d]['name']}`: {db.globals[d]['type']}" if d in db.globals else "")
            for d in gl))
    if f["strings"]:
        out.append("\n## Strings referenced\n" + "\n".join(f"- {s!r}" for s in f["strings"][:20]))
    return "\n".join(out)


if __name__ == "__main__":
    db = DB()
    print(build(db, int(sys.argv[1], 16)))
