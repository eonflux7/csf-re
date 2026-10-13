"""Export the pointers IDA knows inside data: export/dataptrs.json [{source, target}] (hex) (read-only).

The exe has no relocation table, so which data words are addresses is not in the file. IDA marks them as offsets
(items with the offset flag: vtables, pointer tables, initialised pointer globals) and keeps a data reference from
the item. Per data item with that flag every 4-byte element whose value lies inside the image is taken; items that
only carry data references without the flag (struct instances with pointer members) are taken element-wise when
the element's value is one of the item's reference targets. Consumer: tools/datagen.py (pointer fix-ups of the
standalone build's data image).

Usage: export_dataptrs.py
"""
import json, os, sys, time
sys.path.insert(0, os.path.dirname(__file__))
from common import ROOT, open_db, close_db
import ida_bytes, ida_segment, idautils, ida_ida

open_db()
lo, hi = ida_ida.inf_get_min_ea(), ida_ida.inf_get_max_ea()
out, t0, n_flag, n_ref = {}, time.time(), 0, 0
for s in idautils.Segments():
    seg = ida_segment.getseg(s)
    if seg.perm & ida_segment.SEGPERM_EXEC:
        continue
    h = seg.start_ea
    while h < seg.end_ea and h != 0xFFFFFFFF:
        fl = ida_bytes.get_flags(h)
        if ida_bytes.is_data(fl) and ida_bytes.is_loaded(h):
            size = ida_bytes.get_item_size(h)
            if ida_bytes.is_off0(fl) and size % 4 == 0:
                for a in range(h, h + size, 4):
                    v = ida_bytes.get_dword(a)
                    if lo <= v < hi:
                        out[f"{a:08x}"] = v
                        n_flag += 1
            else:
                targets = set(idautils.DataRefsFrom(h))
                if targets:
                    for a in range(h, h + size - 3):  # struct members need not be aligned to the item start
                        if ida_bytes.is_loaded(a) and ida_bytes.get_dword(a) in targets and f"{a:08x}" not in out:
                            out[f"{a:08x}"] = ida_bytes.get_dword(a)
                            n_ref += 1
        h = ida_bytes.next_head(h, seg.end_ea)
rows = [dict(source=a, target=f"{v:08x}") for a, v in sorted(out.items())]  # the form datagen.py --data-refs reads
json.dump(rows, open(os.path.join(ROOT, "export", "dataptrs.json"), "w"), separators=(",", ":"))
print(f"{len(out)} pointers in data ({n_flag} in offset items, {n_ref} by reference) in {time.time() - t0:.0f}s")
close_db(save=False)
