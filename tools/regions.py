#!/usr/bin/env python3
"""Classify aligned code (game vs rw vs netz) by call-graph closure, then print address runs.

Principle: library code never calls game code directly.
  - callers of a game function are game (transitively)
  - callees of a library function are library (same lib, or crt/d3dx/png)
Seeds come from strings. Unlabeled functions take the label of their address neighbours (object files are
contiguous). Fixed regions (crt/d3dx9/png) come from db/regions.yaml entries with confidence: high.

Usage: regions.py [min_run]   -> prints suggested runs; edit db/regions.yaml by hand from it.
"""
import collections, json, os, re, sys
import yaml

ROOT = os.path.dirname(os.path.dirname(os.path.abspath(__file__)))
F = sorted((json.loads(l) for l in open(os.path.join(ROOT, "export", "functions.jsonl"))), key=lambda f: f["ea"])
by = {f["ea"]: f for f in F}
RWI = 0x8b68c0

callers = collections.defaultdict(set)
for f in F:
    for c in f["callees"]:
        callers[c].add(f["ea"])

GAME_STR = re.compile(r"(\.PANEL|Boton|Menus?[/\\]|Maps[/\\]|Models[/\\]|Texts[/\\]|\.fbs|\.fli|\.cfg|"
                      r"Juego|Mision|Soldado|Enemigo|Jugador|Partida|Arma|Proyectil|Commandos|CSF|"
                      r"^T[A-Z][a-z]+$|STATE_|\.dff|CErrorLog|OUT OF MEMORY|Desconocido|fichero)")
NETZ_STR = re.compile(r"(^Job\w+::|Extension$|SystemComponent|\bStation|PRUDP|Quazal|NetZ|"
                      r"(DOCore|ObjDup|DuplicatedObject|Transport|EndPoint|Socket|Session\w*|Result|"
                      r"Scheduler|EventHandler|StepSequenceJob|CallContext|HighLevelStream|BackEndServices|"
                      r"Authentication\w*|ProductFacade|VoiceChannel\w*|PeerManager|DOClassesTable|"
                      r"BundlingPolicy|PseudoGlobalVariable|SelectionIteratorTemplate)\.(cpp|h)$)")
RW_STR = re.compile(r"(rwRASTER|Raster|RwEngine|Only rendering sub|projection type|combination type|NVPerfHUD|"
                    r"video memory|GenuineIntel|AuthenticAMD)")

fixed = {}
for r in yaml.safe_load(open(os.path.join(ROOT, "db", "regions.yaml"))):
    if r["confidence"] == "high":
        for f in F:
            if r["start"] <= f["ea"] < r["end"]:
                fixed[f["ea"]] = r["lib"]
for f in F:
    if f["lib"]:
        fixed[f["ea"]] = "crt"

label = dict(fixed)
for f in F:
    if f["ea"] in label:
        continue
    s = "\n".join(f["strings"])
    if any(NETZ_STR.search(x) for x in f["strings"]):
        label[f["ea"]] = "netz"
    elif RW_STR.search(s):
        label[f["ea"]] = "rw"
    elif any(GAME_STR.search(x) for x in f["strings"]):
        label[f["ea"]] = "game"

# RW seed: RwEngineInstance users inside the RW-dense core/driver ranges
for f in F:
    if f["ea"] not in label and RWI in f["drefs"] and (0x566000 <= f["ea"] < 0x646000):
        label[f["ea"]] = "rw"

seeds = dict(label)
conflicts = collections.Counter()
# closure: game upward through callers; libs downward through callees
for _ in range(50):
    changed = False
    for ea, l in list(label.items()):
        if l == "game":
            for c in callers[ea]:
                if c not in label:
                    label[c] = "game"; changed = True
                elif label[c] not in ("game",) and c not in fixed:
                    conflicts[(label[c], "game")] += 1
        elif l in ("rw", "netz"):
            for c in by[ea]["callees"]:
                if c not in label:
                    label[c] = l; changed = True
    if not changed:
        break

# neighbour fill by address
eas = [f["ea"] for f in F]
for _ in range(3):
    for i, ea in enumerate(eas):
        if ea in label:
            continue
        lo = next((label[eas[j]] for j in range(i - 1, max(i - 8, -1), -1) if eas[j] in label), None)
        hi = next((label[eas[j]] for j in range(i + 1, min(i + 8, len(eas))) if eas[j] in label), None)
        if lo and lo == hi:
            label[ea] = lo

runs = []
for f in F:
    l = label.get(f["ea"], "?")
    if runs and runs[-1][2] == l:
        runs[-1][1] = f["end"]; runs[-1][3] += 1; runs[-1][4] += f["size"]
    else:
        runs.append([f["ea"], f["end"], l, 1, f["size"]])

min_n = int(sys.argv[1]) if len(sys.argv) > 1 else 20
# merge short runs into surroundings for display
print(f"labeled {len(label)}/{len(F)}  seeds {len(seeds)}  conflicts {dict(conflicts)}")
tot = collections.Counter()
for a, b, l, n, sz in runs:
    tot[l] += sz
    if n >= min_n:
        print(f"{a:08x}-{b:08x} {l:5s} n={n:5d} bytes={sz}")
print({k: v for k, v in tot.items()})
json.dump({f"{k:08x}": v for k, v in label.items()}, open(os.path.join(ROOT, "export", "labels.json"), "w"))
