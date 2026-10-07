#!/usr/bin/env python3
"""Evidence sheet for naming a class (Phase 2): hierarchy, size, ctors/dtors and who calls them (with the strings
those callers reference, two levels up), named library/boundary callees of its own methods (RW, CRT, ...), strings
of its own methods, vtable slot map from db/vtables.json (inherited/overridden/new, done/named), field names proposed by grunts, and data tables that point at
its methods.

Usage: classinfo.py <vtable> [<vtable> ...]  [--slots]
"""
import argparse, collections, csv, json, os

ROOT = os.path.dirname(os.path.dirname(os.path.abspath(__file__)))
C = {c["vtable"]: c for c in json.load(open(os.path.join(ROOT, "db", "classes.json")))}
ROWS = {int(r["ea"], 16): r for r in csv.DictReader(open(os.path.join(ROOT, "db", "functions.csv")))}
S = json.load(open(os.path.join(ROOT, "export", "strings.json")))
F, CALLERS = {}, collections.defaultdict(set)
for l in open(os.path.join(ROOT, "export", "functions.jsonl")):
    f = json.loads(l)
    F[f["ea"]] = f
    for c in f["callees"]:
        CALLERS[c].add(f["ea"])
VT = {v["addr"]: v for v in json.load(open(os.path.join(ROOT, "db", "vtables.json")))}
KIDS = collections.defaultdict(list)
for vt, c in C.items():
    if c["base"]:
        KIDS[c["base"]].append(vt)
NAMES_PATH = os.path.join(ROOT, "db", "class_names.yaml")


def class_names():
    if not os.path.exists(NAMES_PATH):
        return {}
    import yaml
    d = yaml.safe_load(open(NAMES_PATH)) or {}
    return {f"{k:08x}" if isinstance(k, int) else str(k).lower(): v for k, v in d.items()}


def strs(ea):
    f = F.get(ea) or {}
    return list(f.get("strings") or []) + [S[hex(r)] for r in f.get("srefs") or [] if hex(r) in S]


def fname(ea):
    r = ROWS.get(ea, {})
    return r.get("name") or f"sub_{ea:X}"


def own(vt):
    b = C[vt]["base"]
    inh = set(C[b]["methods"]) if b in C else set()
    return [int(m, 16) for m in C[vt]["methods"] if m not in inh]


def chain(vt):
    out, seen = [], set()
    while vt and vt not in seen:
        seen.add(vt)
        out.append(vt)
        vt = C.get(vt, {}).get("base")
    return out


def fields_from_proposals(vt):
    acc = collections.defaultdict(collections.Counter)
    for l in open(os.path.join(ROOT, "db", "proposals.jsonl")):
        r = json.loads(l)
        for k, v in (r.get("field_names") or {}).items():
            if isinstance(v, str) and "." in k and k.split(".", 1)[0].lower().replace("c_", "") == vt:
                acc[k.split(".", 1)[1]][v] += 1
    return acc


def sheet(vt, slots=False):
    names = class_names()
    c = C[vt]
    nm = lambda v: f"{v}({names[v]})" if v in names else v
    print(f"######## {nm(vt)}  size={c['size']}  sizes={c.get('sizes')}  methods={len(c['methods'])} own={len(own(vt))}")
    print("chain:", " -> ".join(nm(v) for v in chain(vt)))
    if KIDS[vt]:
        print("kids:", " ".join(f"{nm(k)}[{C[k]['size']}]" for k in sorted(KIDS[vt])))
    for kind in ("ctors", "dtors"):
        for x in c[kind]:
            x = int(x, 16)
            print(f"{kind[:-1]} {x:08x} {fname(x)} ({F.get(x, {}).get('size')}B)")
            for y in sorted(CALLERS[x])[:12]:
                s = strs(y)
                up = sorted(CALLERS[y])[:4]
                s2 = [t for u in up for t in strs(u)][:8]
                print(f"   <- {y:08x} {fname(y)} ({F[y]['size']}B) {' | '.join(t[:28] for t in s[:10])}"
                      + (f"  <<- {' '.join(f'{u:x}' for u in up)}: {' | '.join(t[:24] for t in s2)}" if s2 else ""))
    lib = collections.Counter()
    ms = collections.Counter()
    for m in own(vt) + [int(x, 16) for x in c["ctors"] + c["dtors"]]:
        for s in strs(m):
            ms[s] += 1
        for cal in F.get(m, {}).get("callees", []):
            r = ROWS.get(cal, {})
            if r.get("region") not in (None, "game") and r.get("name"):
                lib[f"{r['region']}:{r['name']}"] += 1
    if lib:
        print("lib callees:", ", ".join(f"{k}x{n}" for k, n in lib.most_common(25)))
    if ms:
        print("strings:", " | ".join(s[:40] for s, _ in ms.most_common(30)))
    fp = fields_from_proposals(vt)
    if fp:
        print("field names (grunts):", "; ".join(f"{f}={'/'.join(n for n, _ in v.most_common(2))}"
                                                 for f, v in sorted(fp.items())))
    fl = c["fields"]
    print("fields:", " ".join(f"+{o}:{f['size']}{f['rw']}" for o, f in sorted(fl.items())[:60]))
    if slots:
        base_slots = VT.get(c["base"], {}).get("slots", []) if c["base"] else []
        for i, m in enumerate(VT.get(vt, {}).get("slots", [])):
            ea = int(m, 16)
            r = ROWS.get(ea, {})
            tag = "inh " if i < len(base_slots) and base_slots[i] == m else "ovr " if i < len(base_slots) else "new "
            print(f"  slot {i:3} +{i * 4:03x} {m} {r.get('status', '?'):5} {F.get(ea, {}).get('size', '?'):>5}B "
                  f"{tag}{r.get('name', '')}")


if __name__ == "__main__":
    ap = argparse.ArgumentParser()
    ap.add_argument("vtables", nargs="+")
    ap.add_argument("--slots", action="store_true")
    a = ap.parse_args()
    for v in a.vtables:
        sheet(v.lower().replace("0x", "").zfill(8), a.slots)
        print()
