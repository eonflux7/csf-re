#!/usr/bin/env python3
"""Push repo knowledge into the IDB (the only script that writes it; one batch per run, idempotent):

  functions   db/functions.csv names (any region); duplicates get an _<EA> suffix in the IDB only
  comments    one-line summary from db/proposals.jsonl as the function comment (repeatable)
  globals     db/globals.yaml names
  vtables     ??_7<Name>@@6B@ (shows as `const Name::`vftable'`) for classes named in db/types/classes.yaml
  types       src/types/auto_classes.h structs as local types (C_<vtable> and named)

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


def header_decls():
    """auto_classes.h as plain C for the IDA parser: no includes/pragmas/asserts, stdint typedefs."""
    src = open(os.path.join(ROOT, "src", "types", "auto_classes.h")).read()
    keep = [l for l in src.splitlines() if not l.startswith(("#", "static_assert"))]
    pre = ("typedef unsigned char uint8_t; typedef unsigned short uint16_t; typedef unsigned int uint32_t;"
           " typedef unsigned long long uint64_t;\n"
           # src/legacy/string.h as C (VC7.1 std::string / std::wstring, 28 bytes)
           "struct legacy_string { uint32_t alloc; union { char buf[16]; char *ptr; } bx; uint32_t size; uint32_t res; };\n"
           "struct legacy_wstring { uint32_t alloc; union { unsigned short buf[8]; unsigned short *ptr; } bx;"
           " uint32_t size; uint32_t res; };\n")
    return pre + "\n".join(keep).replace("legacy::string", "legacy_string").replace("legacy::wstring", "legacy_wstring")


def main():
    ap = argparse.ArgumentParser()
    ap.add_argument("--dry", action="store_true")
    ap.add_argument("--no-types", action="store_true")
    a = ap.parse_args()
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
            errs = ida_typeinf.parse_decls(til, header_decls(), None, ida_typeinf.HTI_DCL | ida_typeinf.HTI_PAK1)
            stats["types"] = sum(1 for _ in re.finditer(r"^struct \w+", header_decls(), re.M))
            stats["type_errors"] = errs
    finally:
        close_db(save=not a.dry)
    print(" ".join(f"{k}={v}" for k, v in stats.items()))


if __name__ == "__main__":
    main()
