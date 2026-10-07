#!/usr/bin/env python3
"""Build the minimal context packet for reimplementing one function.

Usage: packet.py <ea-hex>     (prints the packet)
"""
import csv, json, os, re, sys, threading
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
        from gen_classes_h import load_names
        self.class_names = load_names()  # db/types/classes.yaml, conf >= 0.7
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
        return self.class_names[vt]["name"] if vt in self.class_names else f"C_{vt}"


_md = None
_init_lock = threading.Lock()


def ida_view(ea):
    """IDA's code view of a function (export/disasm/<ea>.json from tools/ida/export_disasm.py), or None."""
    p = os.path.join(ROOT, "export", "disasm", f"{ea:08x}.json")
    return json.load(open(p)) if os.path.exists(p) else None


def disasm(ea, size, linear=False):
    """Instructions of the function at ea. With an IDA export: exactly the code heads IDA knows (all chunks, jump
    tables and embedded data excluded), each decoded by capstone. Otherwise (or linear=True, e.g. for a shared
    block that isn't a function): linear decoding of `size` bytes."""
    global _pe, _md
    if _md is None:
        with _init_lock:  # grunt runs this from worker threads: publish _md last, after _pe is ready
            if _md is None:
                _pe = pefile.PE(os.path.join(ROOT, "work", "pc", "CommXPC.exe"), fast_load=True)
                _md = Cs(CS_ARCH_X86, CS_MODE_32)
    img = _pe.get_memory_mapped_image()
    view = None if linear else ida_view(ea)
    if view:
        out = []
        for h in view["insns"]:
            ins = next(_md.disasm(img[h - 0x400000: h - 0x400000 + 16], h, 1), None)
            if ins:
                out.append(ins)
        return out
    return list(_md.disasm(img[ea - 0x400000: ea - 0x400000 + size], ea))


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
    text = re.sub(r"\bsub_([0-9A-F]+)\b", rep, text)

    def grep_(m):
        g = db.globals.get(int(m.group(2), 16))
        return g["name"] if g else m.group(0)
    return re.sub(r"\b(dword|word|byte|flt|dbl|qword|off|unk|stru)_([0-9A-F]+)\b", grep_, text)


def struct_text(db, vt):
    hdr = open(os.path.join(ROOT, "src", "types", "auto_classes.h")).read()
    m = re.search(rf"(?://[^\n]*\n)*struct {re.escape(db.cls_name(vt))}\b.*?\n}};\n(?:static_assert[^\n]*\n)*", hdr, re.S)
    return m.group(0) if m else None


_hdr_fields = None


def class_fields(db, vt):
    """{offset: (class name, type, member)} declared for vt and its bases in auto_classes.h (offsets from field_XXXX
    names and the offsetof asserts of named fields)."""
    global _hdr_fields
    if _hdr_fields is None:
        hdr = open(os.path.join(ROOT, "src", "types", "auto_classes.h")).read()
        _hdr_fields = {}
        for m in re.finditer(r"^struct (\w+)[^{]*\{\n(.*?)\n\};\n((?:static_assert[^\n]*\n)*)", hdr, re.S | re.M):
            offs = {n: int(o, 16) for n, o in re.findall(r"offsetof\(\w+, (\w+)\) == 0x([0-9a-f]+)", m.group(3))}
            fl = {}
            for t, n in re.findall(r"^\s+([\w:]+\*?) (\w+)(?:\[\w+\])?;", m.group(2), re.M):
                if n.startswith("pad_") or n == "vtbl":
                    continue
                o = int(n[6:], 16) if re.fullmatch(r"field_[0-9a-f]{4}", n) else offs.get(n)
                if o is not None:
                    fl[o] = (t, n)
            _hdr_fields[m.group(1)] = fl
    out, x = {}, vt
    while x:
        for o, (t, n) in _hdr_fields.get(db.cls_name(x), {}).items():
            out.setdefault(o, (db.cls_name(x), t, n))
        x = db.classes.get(x, {}).get("base")
    return out


def compact_class(db, vt, ea, size):
    """The fields of the class chain whose offsets this function's memory operands use (cheap: the full struct
    dump was ~half of every GUI packet). Displacements are matched on any base register, so it may over-include."""
    fields = class_fields(db, vt)
    disp = set()
    for i in disasm(ea, size):
        for m in re.finditer(r"\[(e(?!bp|sp)\w\w) \+ (0x[0-9a-f]+|\d+)\]", i.op_str):  # not stack slots
            disp.add(int(m.group(2), 0))
        if re.search(r"\[e(?!bp|sp)\w\w\]", i.op_str):
            disp.add(0)
    used = sorted(o for o in disp if o in fields)
    lines = [f"- +0x{o:03X} {fields[o][0]}::{fields[o][2]}  ({fields[o][1]})" for o in used]
    return ("Fields at the offsets this function uses (declared in types/auto_classes.h, DO NOT redefine; "
            "access through the struct, other offsets via CSF_AT):\n" + ("\n".join(lines) if lines else "(none declared)"))


STL_LAYOUT = {
    "vector": "allocator@0 (4 bytes, empty), _Myfirst@4, _Mylast@8, _Myend@12 (16 bytes)",
    "vector<bool>": "_Myvec (vector<unsigned>, 16 bytes)@0, _Mysize@16",
    "string": "allocator@0, _Bx@4 (char[16] inline buffer, or char* when _Myres >= 16), _Mysize@20, _Myres@24 (28 bytes)",
    "list": "allocator@0, _Myhead@4 (sentinel node: _Next@0, _Prev@4, value@8), _Mysize@8 (12 bytes)",
    "tree": "comparator/allocator@0, _Myhead@4 (sentinel: _Left@0, _Parent@4, _Right@8, _Color@12, _Isnil@13, "
            "value after), _Mysize@8 (12 bytes)",
}
_stl = None


def stl_note(ea):
    global _stl
    if _stl is None:
        _stl = {int(x["ea"], 16): x for x in json.load(open(os.path.join(ROOT, "db", "stl.json")))}
    x = _stl.get(ea)
    if not x:
        return None
    k = x["kind"]
    return (f"\n## STL instantiation\nThis is a VC7.1 (MSVC 2003) `std::{k}` template member (or internal helper), instantiated for a game "
            f"element type. Layout: {STL_LAYOUT.get(k, '?')}. Reproduce its exact behavior on raw memory at these "
            "offsets (CSF_AT / casts); do NOT use our compiler's std:: containers (different layout). Element copies, "
            "constructors and destructors appear inlined or as callees: keep them exactly (same callees, same element "
            "size and order). Allocate/free with exactly the functions the original calls.")


def build(db, ea, with_asm_limit=400):
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
        out.append(f"\n## `this` class: {db.cls_name(vt)} (vtable 0x{vt.upper()}, size {c['size'] or 'unknown'}, "
                   f"base {db.cls_name(c['base']) if c['base'] else 'none'})")
        if vt in db.class_names:
            out.append(f"Role: {db.class_names[vt].get('why', '')}. Name your function {db.cls_name(vt)}_<Verb...>.")
        if os.environ.get("CSF_CLASS_VIEW", "compact") == "compact":
            chain, x = [], c["base"]
            while x:
                chain.append(db.cls_name(x))
                x = db.classes.get(x, {}).get("base")
            if chain:
                out.append("Bases: " + " : ".join(chain))
            out.append(compact_class(db, vt, ea, f["size"]))
        else:
            st = struct_text(db, vt)
            if st:
                out.append("Declared in types/auto_classes.h (DO NOT redefine; use as-is):\n```cpp\n" + st + "```")
            if c["base"] and struct_text(db, c["base"]):
                out.append("Base:\n```cpp\n" + struct_text(db, c["base"]) + "```")
    note = stl_note(ea)
    if note:
        out.append(note)
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
                       f"proto: `{cproto}`" + ("  NORETURN (throws/aborts): still call it on exactly the same "
                                               "condition, even if the branch looks unreachable" if "__noreturn" in cproto else ""))
    gl = [d for d in f["drefs"]]
    if gl:
        out.append("\n## Data referenced\n" + "\n".join(
            f"- 0x{d:08X}" + (f" `{db.globals[d]['name']}`: {db.globals[d]['type']}" if d in db.globals else "")
            for d in gl))
    if f.get("srefs"):
        out.append("\n## Strings referenced (use CSF_STR(addr, \"text\"), never a bare literal)\n" + "\n".join(
            f"- 0x{a:08X}: {s!r}" for a, s in list(zip(f["srefs"], f["strings"]))[:30]))
    return "\n".join(out)


if __name__ == "__main__":
    db = DB()
    print(build(db, int(sys.argv[1], 16)))
