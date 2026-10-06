#!/usr/bin/env python3
"""Seed-label functions by referenced strings, then print run-length address regions.

Used to draft db/regions.yaml (library vs game address ranges). Exploratory; re-runnable.
"""
import json, os, re, sys
ROOT = os.path.dirname(os.path.dirname(os.path.abspath(__file__)))
F = [json.loads(l) for l in open(os.path.join(ROOT, "export", "functions.jsonl"))]
F.sort(key=lambda f: f["ea"])

NETZ_CPP = re.compile(r"(DDL|Job|Extension|Transport|Session|Station|EndPoint|Socket|DOCore|NetZ|Duplicat|Facade|"
                      r"Discovery|Voice|Peer|BackEnd|Authentication|ObjDup|Core|Result|Platform|Scheduler|"
                      r"MemoryManager|String|EventHandler|StepSequence|CallContext|SystemComponent|Network|"
                      r"Emulation|Packet|HighLevelStream|RC4|LZo|Message|Thread|Bundling|PseudoGlobal|DOClasses)\w*\.(cpp|h)$")
RULES = [
    ("crt",  lambda f, s: f["lib"]),
    ("d3dx", lambda f, s: re.search(r"D3DX|shader|ps_[123]|vs_[123]|internal error|syntax error|texm|swizzl|register|semantic|predicate|oPos|oT\d|dp3 |dcl_", s)),
    ("rw",   lambda f, s: re.search(r"rwRASTER|[Rr]aster|RwEngine|rwID|Only rendering sub|projection type|combination type|NVPerfHUD|video memory|rwFILE|RpWorld|RwFrame", s)),
    ("png",  lambda f, s: re.search(r"libpng|png_|PNG|IHDR|IDAT|bit depth|gamma|color type|palette|chunk", s)),
    ("zlib", lambda f, s: re.search(r"zlib|deflate|inflate|distance code|invalid (block|literal|code|stored)|incorrect header|1\.2\.\d", s)),
    ("netz", lambda f, s: re.search(r"Job\w+::|Unknown$|Extension$|SystemComponent|prudp|Station", s) or any(NETZ_CPP.search(x) for x in f["strings"])),
]


def label(f):
    s = "\n".join(f["strings"])
    for name, rule in RULES:
        if rule(f, s):
            return name
    if f["strings"]:
        return "game?"
    return None


labels = [label(f) for f in F]
# run-length over labeled functions only
runs = []
for f, l in zip(F, labels):
    if l is None:
        continue
    if runs and runs[-1][2] == l:
        runs[-1][1] = f["ea"]; runs[-1][3] += 1
    else:
        runs.append([f["ea"], f["ea"], l, 1])
min_n = int(sys.argv[1]) if len(sys.argv) > 1 else 1
for a, b, l, n in runs:
    if n >= min_n:
        print(f"{a:08x}-{b:08x} {l:6s} n={n}")
