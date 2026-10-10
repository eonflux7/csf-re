"""Export what each call instruction of the game code pops: export/calls.json {call_ea: bytes} (read-only).

IDA's stack-pointer analysis gives, for every call site, the bytes the callee removes from the stack (0 for cdecl
callees, 4*args for stdcall/thiscall ones); for indirect and virtual calls it is the only static source. Consumer:
tools/emudiff.py (the mock of a callee has to pop what the original's call site expects).

Usage: export_calls.py
"""
import csv, json, os, sys, time
sys.path.insert(0, os.path.dirname(__file__))
from common import ROOT, open_db, close_db
import ida_bytes, ida_funcs, ida_frame, ida_idp, ida_ua, idautils

rows = {int(r["ea"], 16) for r in csv.DictReader(open(os.path.join(ROOT, "db", "functions.csv")))
        if r["region"] == "game"}
open_db()
out, t0 = {}, time.time()
for ea in idautils.Functions():
    if ea not in rows:
        continue
    f = ida_funcs.get_func(ea)
    for start, end in idautils.Chunks(ea):
        h = start
        while h < end and h != 0xFFFFFFFF:
            if ida_bytes.is_code(ida_bytes.get_flags(h)):
                insn = ida_ua.insn_t()
                if ida_ua.decode_insn(insn, h) and ida_idp.is_call_insn(insn):
                    out[f"{h:08x}"] = ida_frame.get_sp_delta(f, h + insn.size)
            h = ida_bytes.next_head(h, end)
json.dump(out, open(os.path.join(ROOT, "export", "calls.json"), "w"), separators=(",", ":"))
print(f"{len(out)} call sites in {time.time() - t0:.0f}s")
close_db(save=False)
