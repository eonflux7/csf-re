#!/usr/bin/env python3
"""Generate src/types/auto_classes.h from db/classes.json + db/types/classes.yaml (names, field names, notes).

Each class becomes `struct C_<vtable>` (or its assigned name) with field_XXXX members typed by access size,
float when accessed by FPU/SSE ops; overlapping/odd accesses fall back to byte padding. Bases are embedded as
the first member when the base's size is known. sizeof static_asserts where the allocation size is known.
Named classes (conf >= 0.7 in db/types/classes.yaml) get their name, a comment with the evidence, named fields and
offsetof static_asserts for them. Regenerate after any classes.json / classes.yaml change (tools/rename_classes.py
does). Hand-designed types go in src/types/*.h and win over these.
"""
import json, os, re
import yaml

ROOT = os.path.dirname(os.path.dirname(os.path.abspath(__file__)))
C = {c["vtable"]: c for c in json.load(open(os.path.join(ROOT, "db", "classes.json")))}
MIN_CONF = 0.7


def load_names():
    """{vtable: entry} for the classes whose names are applied (conf >= MIN_CONF)."""
    p = os.path.join(ROOT, "db", "types", "classes.yaml")
    d = yaml.safe_load(open(p)) if os.path.exists(p) else {}
    out = {}
    for k, v in (d or {}).items():
        k = f"{k:08x}" if isinstance(k, int) else str(k).lower()
        if v.get("conf", 0) >= MIN_CONF:
            out[k] = v
    # field names by consensus of the grunts' proposals (tools/fields.py apply), under the curated ones
    cp = os.path.join(ROOT, "db", "types", "field_consensus.yaml")
    cons = (yaml.safe_load(open(cp)) or {}) if os.path.exists(cp) else {}
    for k, fs in cons.items():
        k = f"{k:08x}" if isinstance(k, int) else str(k).lower()
        if k in out:
            own = {int(str(o), 16) for o in (out[k].get("fields") or {})}
            merged = dict(out[k].get("fields") or {})
            merged.update({o: n for o, n in fs.items() if int(str(o), 16) not in own})
            out[k] = dict(out[k], fields=merged)
    return out


NAMES = load_names()


def name(vt):
    """C_<vtable>, or S_<first method> for a class without vtable (plain_classes.py), until classes.yaml names it."""
    return NAMES[vt]["name"] if vt in NAMES else (f"S_{vt}" if C.get(vt, {}).get("plain") else f"C_{vt}")


def field_names(vt):
    """{offset: name} for plain names (type and size taken from the observed access)."""
    return {o: n for o, (t, n, k) in field_specs(vt).items() if t is None}


TSIZE = {"legacy::string": 28, "legacy::wstring": 28, "float": 4, "double": 8, "int32_t": 4, "uint32_t": 4, "int16_t": 2, "uint16_t": 2,
         "int8_t": 1, "uint8_t": 1, "bool": 1, "char": 1}


def field_specs(vt):
    """{offset: (type or None, name, count)}. classes.yaml `fields` values are a plain name, or "type name" /
    "type name[N]" for fields whose type the access scan cannot see (strings, arrays, pointers: `T*`)."""
    out = {}
    for o, v in (NAMES.get(vt, {}).get("fields") or {}).items():
        o = int(o, 16) if isinstance(o, str) else int(o)
        v = str(v).strip()
        if " " not in v:
            out[o] = (None, v, 1)
            continue
        t, n = v.rsplit(" ", 1)
        k = 1
        if n.endswith("]"):
            n, k = n[:-1].split("[")
            k = int(k, 0)
        out[o] = (t.strip(), n, k)
    return out


def tsize(t):
    return 4 if t.endswith("*") else TSIZE[t]


def ctype(size, rw):
    if "f" in rw and size == 4:
        return "float"
    if "f" in rw and size == 8:
        return "double"
    return {1: "uint8_t", 2: "uint16_t", 4: "uint32_t", 8: "uint64_t"}.get(size)


BUILTIN = re.compile(r"^(const\s+)?(unsigned\s+|signed\s+)?(void|bool|char|short|int|long|float|double|"
                     r"(std::)?u?int(8|16|32|64)_t|(std::)?size_t|uintptr_t|wchar_t)(\s+long)?(\s*\*)*$")


def _api():
    p = os.path.join(ROOT, "db", "api.json")
    return {int(k, 16): v for k, v in json.load(open(p)).items()} if os.path.exists(p) else {}


def _vtables():
    return {v["addr"]: v for v in json.load(open(os.path.join(ROOT, "db", "vtables.json")))}


def _param(p, i):
    """(type, name) of one parameter; class-typed pointers become void* (the header declares classes in base
    order, a wrapper may name a class declared later). None = can't express it here (function pointer, by-value
    class)."""
    p = re.sub(r"/\*.*?\*/", "", p).strip()
    if "(" in p or "[" in p or not p:
        return None
    m = re.match(r"^(.*?[\s\*&])(\w+)$", p)
    t, n = (m.group(1).strip(), m.group(2)) if m and not BUILTIN.match(p) else (p, f"a{i}")
    if not BUILTIN.match(t):
        if t.endswith("*"):
            t = "void*"
        else:
            return None
    return t, n


def slot_name(a, slot):
    """Name of a vtable slot: `v` + the target's canonical name without its class prefix (the C++ wrapper here,
    the IDB's <Class>_vtbl member in ida/sync.py: pseudocode and headers say the same), or vSlot<offset>."""
    if not a or a.get("name_src") == "addr":  # unnamed target (signature from the binary): named after its slot
        return f"vSlot{slot * 4:X}"
    short = re.sub(r"^(C_[0-9a-f]{8}|C[A-Z]\w*?|[A-Z]\w*?)_(?=[A-Z])", "", a["name"], count=1)
    short = re.sub(r"_[0-9A-F]{5,8}$", "", short) or a["name"]
    return "v" + short[0].upper() + short[1:]


def vmethods(vt, base, used):
    """Non-virtual inline members that call the vtable slots this class introduces (slots past its base's count):
    `slider->vGetValue()` instead of `reinterpret_cast<int (__fastcall*)(void*, void*)>(vtbl[0x244 / 4])(...)`.
    Named after the slot target's canonical name (db/api.json), class prefix dropped; only slots whose target has
    a known __fastcall (this, edx, ...) signature. No layout change; the gate's slot check sees the displacement."""
    VT, API = _VT, _API
    v = VT.get(vt)
    if not v:
        return []
    first = VT[base]["nslots"] if base and base in VT else 0
    out = []
    for slot in range(first, v["nslots"]):
        a = API.get(int(v["slots"][slot], 16))
        if not a or a["conv"] != "__fastcall":
            continue
        ps = [x for x in a["params"].split(",")] if a["params"].strip() not in ("", "void") else []
        if len(ps) < 2:
            continue
        args = [_param(x, i) for i, x in enumerate(ps[2:], 1)]
        if any(x is None for x in args):
            continue
        ret = a["ret"].strip()
        if not BUILTIN.match(ret):
            if ret.endswith("*"):
                ret = "void*"
            else:
                continue
        mname = slot_name(a, slot)
        if mname in used:
            mname = f"{mname}_{slot:x}"
        used.add(mname)
        names = []
        for i, (t, n) in enumerate(args, 1):
            k = 0
            while n in names or n in ("this", "self") or n in [x[1] for x in args[i:]]:
                k += 1
                n = f"p{i}_{k}"
            names.append(n)
        sig = ", ".join(f"{t} {n}" for (t, _), n in zip(args, names))
        fptr = ", ".join(["void*", "void*"] + [t for t, _ in args])
        call = ", ".join(["this", "nullptr"] + names)
        out.append(f"    {ret} {mname}({sig}) {{ return reinterpret_cast<{ret}(__fastcall*)({fptr})>"
                   f"(vtbl[0x{slot * 4:x} / 4])({call}); }}  // slot +0x{slot * 4:X} -> 0x{v['slots'][slot].upper()}")
    return out


_VT, _API = {}, {}


def main():
    global _VT, _API
    _VT, _API = _vtables(), _api()
    order, seen = [], set()
    nwrap = 0


    def visit(vt):
        if vt in seen or vt not in C:
            return
        seen.add(vt)
        if C[vt]["base"]:
            visit(C[vt]["base"])
        order.append(vt)


    for vt in sorted(C):
        visit(vt)

    out = ["// GENERATED by tools/gen_classes_h.py from db/classes.json — do not edit by hand.",
           "// Rename classes and fields in db/types/classes.yaml; hand-designed layouts go in src/types/.",
           "#pragma once", "#include <cstddef>", "#include <cstdint>", '#include "legacy/string.h"', "",
           "#pragma pack(push, 1)"]
    emitted, extent = set(), {}


    def field_end(c):
        return max([int(o, 16) + f["size"] for o, f in c["fields"].items()] + [4])


    for vt in order:
        c = C[vt]
        b = c["base"]
        # embed base only if already declared (no cycles) and it fits inside our known size
        if not (b in emitted and (not c["size"] or extent[b] <= c["size"])):
            b = None
        root = 0 if c.get("plain") else 4  # a class without vtable starts with its first field
        extent[vt] = c["size"] or max(field_end(c), extent[b] if b else root)
        emitted.add(vt)
        start = extent[b] if b else root
        lines = []
        if vt in NAMES:
            e = NAMES[vt]
            lines.append(f"// {name(vt)} ({'no vtable, methods from 0x' if c.get('plain') else 'vtable 0x'}"
                         f"{vt.upper()}): {e.get('why', '')}")
            if e.get("note"):
                lines.append(f"// {e['note']}")
        lines.append(f"struct {name(vt)}" + (f" : {name(b)}" if b else "") + " {")
        fn = field_names(vt)
        specs = field_specs(vt)
        typed = {o: (t, n, k) for o, (t, n, k) in specs.items() if t is not None}
        if not b and not c.get("plain"):
            lines.append("    void** vtbl;")
        cur = start
        end = extent[vt]
        items = [(o, ("typed", typed[o])) for o in typed] + \
                [(int(o, 16), ("scan", f)) for o, f in c["fields"].items() if int(o, 16) not in typed]
        for o, (kind, f) in sorted(items, key=lambda x: (x[0], x[1][0] == "scan")):
            if kind == "typed":
                t, n, k = f
                size = tsize(t) * k
                decl = f"    {t} {n}" + (f"[{k}]" if k > 1 else "") + ";"
            else:
                size = f["size"]
                t = ctype(f["size"], f["rw"])
                if not t or o % min(f["size"], 4):
                    continue
                decl = f"    {t} {fn.get(o, f'field_{o:04x}')};"
            if o < cur or o + size > end:
                continue
            if o > cur:
                lines.append(f"    uint8_t pad_{cur:04x}[0x{o - cur:x}];")
            lines.append(decl)
            cur = o + size
        if end > cur:
            lines.append(f"    uint8_t pad_{cur:04x}[0x{end - cur:x}];")
        used = {re.search(r"(\w+)(\[\w+\])?;$", l).group(1) for l in lines if re.search(r"(\w+)(\[\w+\])?;$", l)}
        vm = vmethods(vt, C[vt]["base"], used)
        if vm:
            lines.append("    // virtual calls through this class's own vtable slots (tools/gen_classes_h.py vmethods)")
            lines += vm
            nwrap += len(vm)
        lines.append("};")
        if c["size"]:
            lines.append(f"static_assert(sizeof({name(vt)}) == 0x{c['size']:x});")
        for o, (t, n, k) in sorted(specs.items()):
            if any(re.search(rf" {re.escape(n)}(\[\d+\])?;$", l) for l in lines):
                lines.append(f"static_assert(offsetof({name(vt)}, {n}) == 0x{o:x});")
        out += lines + [""]
    out.append("#pragma pack(pop)")
    os.makedirs(os.path.join(ROOT, "src", "types"), exist_ok=True)
    open(os.path.join(ROOT, "src", "types", "auto_classes.h"), "w").write("\n".join(out) + "\n")
    print(f"wrote {len(order)} classes, {nwrap} virtual-call wrappers")


if __name__ == "__main__":
    main()
