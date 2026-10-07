"""Export per-function metadata to export/functions.jsonl (no decompilation).

Fields: ea, end, size, name, flags (lib/thunk), callees, data refs (globals), strings.
"""
import os, sys, json
sys.path.insert(0, os.path.dirname(__file__))
from common import ROOT, open_db, close_db
import ida_funcs, ida_name, idautils, ida_bytes, ida_nalt, ida_segment, ida_xref, idc

open_db()
os.makedirs(os.path.join(ROOT, "export"), exist_ok=True)
strs = {}
for s in idautils.Strings():
    strs[s.ea] = str(s)

out = open(os.path.join(ROOT, "export", "functions.jsonl"), "w")
for ea in idautils.Functions():
    f = ida_funcs.get_func(ea)
    callees, drefs, srefs = set(), set(), set()
    for item in idautils.FuncItems(ea):
        for x in idautils.XrefsFrom(item, 0):
            if x.type in (ida_xref.fl_CN, ida_xref.fl_CF, ida_xref.fl_JN, ida_xref.fl_JF):
                g = ida_funcs.get_func(x.to)
                if g and g.start_ea != ea and g.start_ea == x.to:
                    callees.add(x.to)
            elif x.iscode == 0:
                if x.to in strs:
                    srefs.add(x.to)
                else:
                    seg = ida_segment.getseg(x.to)
                    if seg and ida_segment.get_segm_name(seg) in (".data", ".rdata", "_rwdseg"):
                        drefs.add(x.to)
                        # pointer to string (e.g. table of char*)
    out.write(json.dumps(dict(
        ea=ea, end=f.end_ea, size=f.end_ea - f.start_ea,
        name=ida_funcs.get_func_name(ea),
        lib=bool(f.flags & ida_funcs.FUNC_LIB), thunk=bool(f.flags & ida_funcs.FUNC_THUNK),
        callees=sorted(callees), drefs=sorted(drefs),
        strings=[strs[s] for s in sorted(srefs)],
        srefs=sorted(srefs),
    )) + "\n")
out.close()
json.dump({hex(k): v for k, v in strs.items()}, open(os.path.join(ROOT, "export", "strings.json"), "w"))
close_db(save=False)
