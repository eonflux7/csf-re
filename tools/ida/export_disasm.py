"""Export IDA's view of each function's code to export/disasm/<ea>.json (read-only; no IDB writes).

Per function:
  chunks   [[start, end], ...]          function chunks (incl. tails shared/moved by the compiler)
  insns    [ea, ...]                    heads IDA marks as code (jump tables / embedded data excluded)
  switches {jump_ea: [target, ...]}     switch jump tables
  crefs    [[from, to, type], ...]      code xrefs leaving an instruction (call/jmp; type: call|jump)
  drefs    [[from, to, type], ...]      data xrefs (type: r|w|o = read/write/offset)
Consumers (tools/packet.py disasm(), semcheck, difftable) decode `insns` with capstone, so operand text keeps the
capstone format; without an export they fall back to linear decoding.

Usage: export_disasm.py [--regions game,rw,...]   (default: game)
"""
import csv, json, os, sys, time
sys.path.insert(0, os.path.dirname(__file__))
from common import ROOT, open_db, close_db
import ida_funcs, ida_bytes, ida_nalt, ida_xref, idautils

args = sys.argv[1:]
regions = set((args[args.index("--regions") + 1] if "--regions" in args else "game").split(","))
rows = {int(r["ea"], 16) for r in csv.DictReader(open(os.path.join(ROOT, "db", "functions.csv")))
        if r["region"] in regions}

open_db()
out_dir = os.path.join(ROOT, "export", "disasm")
os.makedirs(out_dir, exist_ok=True)
CODE_TYPES = {ida_xref.fl_CN: "call", ida_xref.fl_CF: "call", ida_xref.fl_JN: "jump", ida_xref.fl_JF: "jump"}
DATA_TYPES = {ida_xref.dr_R: "r", ida_xref.dr_W: "w", ida_xref.dr_O: "o"}
t0, n = time.time(), 0
for ea in idautils.Functions():
    if ea not in rows:
        continue
    f = ida_funcs.get_func(ea)
    chunks = [[s, e] for s, e in idautils.Chunks(ea)]
    insns, switches, crefs, drefs = [], {}, [], []
    for start, end in chunks:
        h = start
        while h < end and h != 0xFFFFFFFF:
            fl = ida_bytes.get_flags(h)
            if ida_bytes.is_code(fl):
                insns.append(h)
                si = ida_nalt.get_switch_info(h)
                if si:
                    switches[h] = sorted({x.to for x in idautils.XrefsFrom(h, 0) if x.iscode})
                for x in idautils.XrefsFrom(h, 0):
                    if x.iscode and x.type in CODE_TYPES and not (start <= x.to < end):
                        crefs.append([h, x.to, CODE_TYPES[x.type]])
                    elif not x.iscode and x.type in DATA_TYPES:
                        drefs.append([h, x.to, DATA_TYPES[x.type]])
            h = ida_bytes.next_head(h, end)
    json.dump(dict(ea=ea, chunks=chunks, insns=insns, switches=switches, crefs=crefs, drefs=drefs),
              open(os.path.join(out_dir, f"{ea:08x}.json"), "w"), separators=(",", ":"))
    n += 1
    if n % 2000 == 0:
        print(f"{n} {time.time() - t0:.0f}s", flush=True)
print(f"exported {n} functions in {time.time() - t0:.0f}s")
close_db(save=False)
