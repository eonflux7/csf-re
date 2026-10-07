"""Create the IDB from the exe with full auto-analysis. Run once."""
import os, shutil, sys
sys.path.insert(0, os.path.dirname(__file__))
from common import ROOT, IDB, EXE, open_db, close_db
import ida_funcs, ida_hexrays, ida_loader

if os.path.exists(IDB):
    sys.exit("IDB exists; delete it to re-create")
open_db(EXE)
print("functions:", ida_funcs.get_func_qty())
print("hexrays:", ida_hexrays.init_hexrays_plugin())
ida_loader.save_database(IDB, 0)
close_db(save=True)
# idalib saves next to the input exe; move it into ida/
src = EXE + ".i64"
if os.path.exists(src) and not os.path.exists(IDB):
    shutil.move(src, IDB)
