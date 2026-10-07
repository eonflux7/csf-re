"""Decompile functions to export/funcs/<ea>.c. Default: all non-lib (status != lib) from db/functions.csv.

Usage: export_pseudo.py [--all] [--only ea,ea,...]
Writes export/pseudo_failures.txt for functions Hex-Rays could not decompile.
"""
import os, sys, csv, time
sys.path.insert(0, os.path.dirname(__file__))
from common import ROOT, open_db, close_db
import ida_hexrays, ida_funcs

args = sys.argv[1:]
rows = list(csv.DictReader(open(os.path.join(ROOT, "db", "functions.csv"))))
if "--only" in args:
    targets = [int(x, 16) for x in args[args.index("--only") + 1].split(",")]
else:
    targets = [int(r["ea"], 16) for r in rows if "--all" in args or r["status"] != "lib"]

open_db()
assert ida_hexrays.init_hexrays_plugin()
outdir = os.path.join(ROOT, "export", "funcs")
os.makedirs(outdir, exist_ok=True)
fails, t0 = [], time.time()
for i, ea in enumerate(targets):
    try:
        cf = ida_hexrays.decompile(ea, flags=ida_hexrays.DECOMP_NO_WAIT | ida_hexrays.DECOMP_NO_CACHE)
        text = str(cf) if cf else None
    except ida_hexrays.DecompilationFailure as e:
        text, err = None, str(e)
    if text is None:
        fails.append(f"{ea:08x}")
        continue
    with open(os.path.join(outdir, f"{ea:08x}.c"), "w") as f:
        f.write(text)
    if i % 1000 == 0:
        print(f"{i}/{len(targets)} {time.time() - t0:.0f}s", flush=True)
open(os.path.join(ROOT, "export", "pseudo_failures.txt"), "w").write("\n".join(fails) + "\n")
print(f"done {len(targets) - len(fails)} ok, {len(fails)} failed, {time.time() - t0:.0f}s")
close_db(save=False)
