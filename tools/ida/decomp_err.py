"""Why Hex-Rays can't decompile some functions: export_abi.py decomp_err.py ea [ea...] (read-only)."""
import os, sys
sys.path.insert(0, os.path.dirname(__file__))
from common import open_db, close_db
import ida_hexrays

open_db()
assert ida_hexrays.init_hexrays_plugin()
for a in sys.argv[1:]:
    ea = int(a, 16)
    try:
        cf = ida_hexrays.decompile(ea, flags=ida_hexrays.DECOMP_NO_WAIT | ida_hexrays.DECOMP_NO_CACHE)
        print(f"{ea:08x} ok" if cf else f"{ea:08x} None")
    except ida_hexrays.DecompilationFailure as e:
        print(f"{ea:08x} {e}")
close_db(save=False)
