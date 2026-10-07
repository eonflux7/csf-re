"""Decompile functions (any region) to stdout: decomp.py <ea> [ea ...]. Read-only."""
import sys
from common import open_db, close_db
import ida_hexrays, ida_funcs, idc

open_db()
for a in sys.argv[1:]:
    ea = int(a, 16)
    try:
        print(f"// ==== {ea:08x} {idc.get_func_name(ea)}\n{ida_hexrays.decompile(ea)}\n")
    except Exception as e:
        print(f"// ==== {ea:08x} decompile failed: {e}\n")
close_db(False)
