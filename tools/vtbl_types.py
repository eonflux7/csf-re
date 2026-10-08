#!/usr/bin/env python3
"""Typed vtables for the IDB (ida/sync.py types stage): one `<Class>_vtbl` struct per class with a vtable, a
typed function pointer per slot, and the class's vtable pointer renamed `__vftable`. That is IDA's convention:
Hex-Rays then shows `this->__vftable->vDraw(this, x)` instead of `(*(int (__thiscall **)(int, int))(*(_DWORD *)this
+ 0x44))(this, x)`, and uses the derived class's table on derived objects. Slot names are the C++ wrappers' names
(gen_classes_h.slot_name: `vDraw` here is `obj->vDraw()` in src/types/auto_classes.h). Signatures from
db/api.json (implementations, votes, the binary-checked Hex-Rays ones); slots without one stay `void *`.

Usage: vtbl_types.py  (prints the declarations: what sync.py feeds IDA's parser)
"""
import json, os, re, sys
sys.path.insert(0, os.path.dirname(__file__))

ROOT = os.path.dirname(os.path.dirname(os.path.abspath(__file__)))
SCALARS = {"void", "bool", "char", "short", "int", "long", "float", "double", "int8_t", "uint8_t", "int16_t",
           "uint16_t", "int32_t", "uint32_t", "int64_t", "uint64_t", "unsigned int", "unsigned char",
           "unsigned short", "unsigned long", "signed char", "wchar_t", "size_t", "uintptr_t"}


def ida_type(t, structs):
    """A binding's C/C++ type as IDA's parser takes it: std:: and const dropped, references as pointers,
    pointers to unknown types as void*, unknown scalars (enums, legacy::string by value) as int."""
    t = re.sub(r"\bstd::|\bconst\b|\bvolatile\b", "", t).replace("&", "*").strip()
    stars = t.count("*")
    base = re.sub(r"\s+", " ", t.replace("*", "")).strip()
    if base.startswith("unsigned ") and base[9:] in SCALARS or base in SCALARS:
        return base + " " + "*" * stars if stars else base
    if stars and base in structs:
        return f"{base} " + "*" * stars
    return ("void " + "*" * stars) if stars else "int"


def split_param(p, i):
    p = re.sub(r"/\*.*?\*/", "", p).strip()
    if p == "...":
        return "...", ""
    m = re.match(r"^(.*?[\s\*&])(\w+)$", p)
    if m and not re.fullmatch(r"(unsigned|signed|const|std::\w+|\w+_t|int|char|short|long|float|double|bool|void)",
                             m.group(2)):
        return m.group(1).strip(), m.group(2)
    return p, f"a{i}"


def fn_ptr(a, cls, member, structs):
    """`ret (__conv *member)(params)` from an api.json entry; CSF_FUNC's __fastcall(self, edx, ...) is the
    thiscall it binds, so it goes back to __thiscall(Class *this, ...)."""
    if not a:
        return f"void *{member}"
    ps = [x for x in a["params"].split(",") if x.strip() and x.strip() != "void"]
    ps = [split_param(p, i + 1) for i, p in enumerate(ps)]
    conv = a["conv"]
    if conv == "__fastcall" and len(ps) >= 2:
        conv, ps = "__thiscall", [(f"{cls} *", "this")] + ps[2:]
    elif conv == "__fastcall" and len(ps) == 1:
        conv, ps = "__thiscall", [(f"{cls} *", "this")]
    if "(" in "".join(t for t, _ in ps):
        return f"void *{member}"
    args = ", ".join("..." if t == "..." else (t if t.endswith("*") and n == "this" else ida_type(t, structs))
                     + ("" if t == "..." else f" {n}") for t, n in ps) or "void"
    return f"{ida_type(a['ret'], structs)} ({conv} *{member})({args})"


def decls(structs=None):
    """(forward declarations, vtbl structs, {class struct name: vtbl struct name})"""
    from gen_classes_h import load_names, slot_name
    import api
    names = load_names()
    A = api.load()
    vts = {v["addr"]: v for v in json.load(open(os.path.join(ROOT, "db", "vtables.json")))}
    cls = {c["vtable"] for c in json.load(open(os.path.join(ROOT, "db", "classes.json")))}
    cname = lambda vt: names[vt]["name"] if vt in names else f"C_{vt}"  # noqa: E731
    structs = structs if structs is not None else {cname(vt) for vt in cls}
    out, which = [], {}
    for vt in sorted(cls):
        v = vts.get(vt)
        if not v or not v["nslots"]:
            continue
        cn = cname(vt)
        if cn not in structs:
            continue
        used, mem = set(), []
        for slot, tgt in enumerate(v["slots"]):
            a = A.get(int(tgt, 16))
            m = slot_name(a, slot)
            if m in used:
                m = f"{m}_{slot * 4:x}"
            used.add(m)
            mem.append(f"    {fn_ptr(a, cn, m, structs)};  // +0x{slot * 4:X} -> 0x{tgt.upper()}")
        out.append(f"struct {cn}_vtbl {{\n" + "\n".join(mem) + "\n};")
        which[cn] = f"{cn}_vtbl"
    fwd = "\n".join(f"struct {s};" for s in sorted(structs))
    return fwd, "\n".join(out), which


if __name__ == "__main__":
    f, d, w = decls()
    print(f"// {len(w)} vtbl structs\n{d}")
