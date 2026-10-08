#!/usr/bin/env python3
"""Push repo knowledge into the IDB (the only script that writes it; one batch per run, idempotent):

  functions   db/functions.csv names (any region); duplicates get an _<EA> suffix in the IDB only
  comments    one-line summary from db/proposals.jsonl as the function comment (repeatable)
  globals     db/globals.yaml names
  vtables     ??_7<Name>@@6B@ (shows as `const Name::`vftable'`) for classes named in db/types/classes.yaml
  types       src/types/auto_classes.h structs as local types (C_<vtable> and named), typed vtables
              (<Class>_vtbl + __vftable: tools/vtbl_types.py), db/globals.yaml types on the globals
  --protos    prototypes the callers' cleanup proves longer than Hex-Rays' (proven_protos)
  --this      `this` typed as the class (db/types/this_class.json) on __thiscall methods (this_protos)

Usage: .venv/bin/python tools/ida/sync.py [--dry] [--no-types]
"""
import argparse, csv, json, os, re, sys
sys.path.insert(0, os.path.dirname(__file__))
from common import ROOT, open_db, close_db  # noqa: E402  (imports idapro first)
import ida_funcs, ida_name, ida_typeinf, idc  # noqa: E402
import yaml  # noqa: E402

sys.path.insert(0, os.path.join(ROOT, "tools"))
from gen_classes_h import load_names  # noqa: E402

AUTO = re.compile(r"^(sub|nullsub|j_sub|unknown_libname|loc|off|dword|byte|word|unk|flt|dbl|qword|stru)_[0-9A-Fa-f]+(_\d+)?$")


def set_name(ea, name, stats, key):
    cur = ida_name.get_name(ea)
    if cur == name:
        return
    owner = ida_name.get_name_ea(idc.BADADDR, name)
    if owner != idc.BADADDR and owner != ea:
        name = f"{name}_{ea:X}"
        if cur == name:
            return
    if ida_name.set_name(ea, name, ida_name.SN_NOWARN | ida_name.SN_NOCHECK):
        stats[key] += 1
    else:
        stats[key + "_fail"] += 1


def header_parts():
    """auto_classes.h as plain C for the IDA parser (no includes/pragmas/asserts/inline wrapper methods, stdint
    typedefs), and the typed vtables (tools/vtbl_types.py: <Class>_vtbl structs; a root class's `void** vtbl`
    becomes `<Class>_vtbl *__vftable`, IDA's convention). Two texts, parsed in order: IDA's parser gives up on a
    long text after errors, so the vtables go on their own; every struct is forward-declared in both."""
    src = open(os.path.join(ROOT, "src", "types", "auto_classes.h")).read()
    keep = [l for l in src.splitlines() if not l.startswith(("#", "static_assert"))
            and not re.match(r"^\s+[^/].*\)\s*(const\s*)?\{.*\}\s*(//.*)?$", l)]  # inline wrappers (C++ only)
    pre = ("typedef unsigned char uint8_t; typedef unsigned short uint16_t; typedef unsigned int uint32_t;"
           " typedef unsigned long long uint64_t; typedef signed char int8_t; typedef short int16_t;"
           " typedef int int32_t; typedef long long int64_t;\n"
           # src/legacy/string.h as C (VC7.1 std::string / std::wstring, 28 bytes)
           "struct legacy_string { uint32_t alloc; union { char buf[16]; char *ptr; } bx; uint32_t size; uint32_t res; };\n"
           "struct legacy_wstring { uint32_t alloc; union { unsigned short buf[8]; unsigned short *ptr; } bx;"
           " uint32_t size; uint32_t res; };\n")
    body = "\n".join(keep).replace("legacy::string", "legacy_string").replace("legacy::wstring", "legacy_wstring")
    import vtbl_types
    structs = set(re.findall(r"^struct (\w+)", body, re.M))
    fwd, vt, which = vtbl_types.decls(structs)
    fwd += "\n" + "\n".join(f"struct {v};" for v in sorted(which.values()))

    def retype(m):
        name = m.group(1)
        return m.group(0) if name not in which else m.group(0).replace("void** vtbl;", f"{which[name]} *__vftable;", 1)
    body = re.sub(r"^struct (\w+) \{\n\s+void\*\* vtbl;", retype, body, flags=re.M)
    return pre + fwd + "\n" + body + "\n", pre + fwd + "\n" + vt + "\n"


def header_decls():
    return "\n".join(header_parts())


def global_types():
    """db/globals.yaml types IDA can parse (scalars, pointers to scalars or to class structs): {addr: decl}."""
    import vtbl_types
    structs = set(re.findall(r"^struct (\w+)", header_decls(), re.M))
    out = {}
    for g in yaml.safe_load(open(os.path.join(ROOT, "db", "globals.yaml"))) or []:
        t = (g.get("type") or "").strip()
        if not t or "(" in t:
            continue
        it = vtbl_types.ida_type(t, structs)
        if it == "int" and t not in ("int", "int32_t", "std::int32_t", "long"):
            continue  # unknown by-value type (a table typedef): leave IDA's own
        out[int(g["addr"])] = f"{it} {g['name']};"
    return out


def proven_protos():
    """Prototypes the binary proves Hex-Rays wrong on (its own prototype is built from the body alone): cdecl
    functions whose callers remove more stack bytes than Hex-Rays' arguments take (`add esp, N` right after every
    call; tools/api.py caller_cleanup). The unused trailing arguments are added as int. Applying them makes
    Hex-Rays read the callers right too (it trusts a callee's prototype: with 3 args instead of 5 it dropped the
    pushed `this` of 0x431CF0 and called it stdcall). Re-run export_abi.py after; repeat until nothing is left."""
    sys.path.insert(0, os.path.join(ROOT, "tools"))
    import api
    from packet import DB
    db = DB()
    abi, clean = api.ida_abi(), api.caller_cleanup(db)
    out = {}
    for ea, a in abi.items():
        if a.get("cc") != "cdecl" or ea not in clean:
            continue
        n = clean[ea] // 4
        if n <= len(a["args"]) or any(t in ("double", "__int64", "unsigned __int64") for t, _, _ in a["args"]):
            continue
        args = [f"{t} {nm or f'a{i + 1}'}" for i, (t, nm, _) in enumerate(a["args"])]
        args += [f"int a{i + 1}" for i in range(len(args), n)]
        out[ea] = f"{a['ret']} __cdecl f({', '.join(args)});"
    return out


def this_protos():
    """Methods with a known class (db/types/this_class.json) whose Hex-Rays prototype is __thiscall: the same
    prototype with `this` typed as a pointer to the class struct (src/types/auto_classes.h, loaded by the types
    stage). Hex-Rays then shows this->field and types the objects callers pass, which this_class.py propagates."""
    sys.path.insert(0, os.path.join(ROOT, "tools"))
    import api, this_class
    from packet import DB
    db = DB()
    abi = api.ida_abi()
    out = {}
    for ea, c in this_class.load().items():
        a = abi.get(ea)
        if not a or a.get("cc") != "thiscall" or not a.get("args"):
            continue
        name = db.cls_name(c["vt"])
        args = [f"{name} *this"] + [f"{t} {n or f'a{i + 2}'}" for i, (t, n, _) in enumerate(a["args"][1:])]
        if a["args"][0][0] == f"{name} *":
            continue
        out[ea] = f"{a['ret']} __thiscall f({', '.join(args)});"
    return out


def main():
    ap = argparse.ArgumentParser()
    ap.add_argument("--dry", action="store_true")
    ap.add_argument("--no-types", action="store_true")
    ap.add_argument("--protos", action="store_true", help="only the prototypes stage (proven_protos)")
    ap.add_argument("--this", action="store_true", help="only the `this` types stage (this_protos)")
    a = ap.parse_args()
    if a.protos or a.this:
        protos = proven_protos() if a.protos else this_protos()
        open_db()
        n = bad = 0
        try:
            for ea, decl in protos.items():
                if a.dry:
                    continue
                if idc.SetType(ea, decl):
                    n += 1
                else:
                    bad += 1
        finally:
            close_db(save=not a.dry)
        print(f"prototypes ({'caller cleanup' if a.protos else 'this types'}): {len(protos)}, {n} applied, "
              f"{bad} rejected by IDA")
        return
    rows = list(csv.DictReader(open(os.path.join(ROOT, "db", "functions.csv"))))
    summ = {}
    for l in open(os.path.join(ROOT, "db", "proposals.jsonl")):
        r = json.loads(l)
        if r.get("summary"):
            summ[int(r["ea"], 16)] = r["summary"]
    globs = yaml.safe_load(open(os.path.join(ROOT, "db", "globals.yaml"))) or []
    names = load_names()
    open_db()
    stats = dict(fn=0, fn_fail=0, cmt=0, glob=0, glob_fail=0, vt=0, vt_fail=0, types=0)
    try:
        for r in rows:
            ea = int(r["ea"], 16)
            if r["name"] and ida_funcs.get_func(ea) and not a.dry:
                set_name(ea, r["name"], stats, "fn")
            if ea in summ and r["status"] == "done" and not a.dry:
                f = ida_funcs.get_func(ea)
                if f and ida_funcs.get_func_cmt(f, True) != summ[ea]:
                    ida_funcs.set_func_cmt(f, summ[ea], True)
                    stats["cmt"] += 1
        for g in globs:
            if not a.dry:
                set_name(int(g["addr"]), g["name"], stats, "glob")
        for vt, e in names.items():
            if not a.dry:
                set_name(int(vt, 16), f"??_7{e['name']}@@6B@", stats, "vt")
        if not a.no_types and not a.dry:
            til = ida_typeinf.get_idati()
            main_, vtbls = header_parts()
            errs = ida_typeinf.parse_decls(til, main_, None, ida_typeinf.HTI_DCL | ida_typeinf.HTI_PAK1)
            stats["vtbl_errors"] = ida_typeinf.parse_decls(til, vtbls, None, ida_typeinf.HTI_DCL | ida_typeinf.HTI_PAK1)
            stats["types"] = sum(1 for _ in re.finditer(r"^struct \w+", header_decls(), re.M))
            stats["type_errors"] = errs
            stats["gtypes"] = stats["gtypes_fail"] = 0
            for addr, decl in global_types().items():
                stats["gtypes" if idc.SetType(addr, decl) else "gtypes_fail"] += 1
    finally:
        close_db(save=not a.dry)
    print(" ".join(f"{k}={v}" for k, v in stats.items()))


if __name__ == "__main__":
    main()
