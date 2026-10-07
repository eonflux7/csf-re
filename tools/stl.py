#!/usr/bin/env python3
"""Find MSVC 7.1 STL template instantiations compiled into game code; mark them module=stl.

STL instances are not grunted one by one: a template impl (legacy::vector<T> etc.) will cover all element types.
Evidence, applied to a fixpoint:
  seed     references a VC7.1 STL error string ("vector<T> too long", "map/set<T> too long", ...)
  xcall    calls a seed throw-helper (vector _Xlen, std::_String_base::_Xlen/_Xran in the CRT)
  clone    same normalized shape as a marked function (export/clones.json) -> other element type
  callee   game function called only by marked functions, and itself shaped like a template (clone cluster
           with >= 2 members, all of whose members are only called by marked functions)
Writes db/stl.json ([{ea, kind, evidence, cluster}]) and sets module=stl in db/functions.csv
(only where module is empty).
"""
import collections, json, os, re, sys
sys.path.insert(0, os.path.dirname(__file__))
from triage import load_csv, save_csv

ROOT = os.path.dirname(os.path.dirname(os.path.abspath(__file__)))

STRINGS = {  # string -> container kind of the function referencing it
    "vector<T> too long": "vector", "vector<bool> too long": "vector<bool>",
    "map/set<T> too long": "tree", "invalid map/set<T> iterator": "tree",
    "list<T> too long": "list", "deque<T> too long": "deque",
}
CRT_HELPERS = {"?_Xlen@_String_base@std@@QBEXXZ": "string", "?_Xran@_String_base@std@@QBEXXZ": "string"}


def main():
    F = {}
    for l in open(os.path.join(ROOT, "export", "functions.jsonl")):
        d = json.loads(l)
        F[d["ea"]] = d
    rows = load_csv()
    game = {ea for ea, r in rows.items() if r["region"] == "game"}
    strings = json.load(open(os.path.join(ROOT, "export", "strings.json")))
    callers = collections.defaultdict(set)
    for d in F.values():
        for c in d["callees"]:
            callers[c].add(d["ea"])
    clusters = json.load(open(os.path.join(ROOT, "export", "clones.json")))
    cid = {int(m, 16): i for i, c in enumerate(clusters) for m in c["members"]}

    kind, evidence = {}, {}

    def mark(ea, k, ev):
        if ea in game and ea not in kind:
            kind[ea], evidence[ea] = k, ev
            return True
        return False

    str_kind = {int(a, 16): STRINGS[t] for a, t in strings.items() if t in STRINGS}
    seeds = set()
    for d in F.values():
        for s in d["srefs"] + d["drefs"]:
            if s in str_kind and mark(d["ea"], str_kind[s], "seed"):
                seeds.add(d["ea"])
    # small throw helpers (vector _Xlen etc.): their callers are container members
    helpers = {ea: kind[ea] for ea in seeds if F[ea]["size"] < 128}
    for d in F.values():
        if d["name"] in CRT_HELPERS:
            helpers[d["ea"]] = CRT_HELPERS[d["name"]]
    for h, k in helpers.items():
        for c in callers[h]:
            mark(c, k, "xcall")

    changed = True
    while changed:
        changed = False
        for ea in list(kind):  # clone expansion
            if ea in cid:
                for m in clusters[cid[ea]]["members"]:
                    changed |= mark(int(m, 16), kind[ea], "clone")
        for ea in list(kind):  # template-shaped exclusive callees
            for c in F[ea]["callees"]:
                if c in kind or c not in game or c not in cid:
                    continue
                mem = [int(m, 16) for m in clusters[cid[c]]["members"]]
                if all(callers[m] and callers[m] <= set(kind) for m in mem):
                    for m in mem:
                        changed |= mark(m, kind[ea], "callee")

    out = [dict(ea=f"{ea:08x}", size=F[ea]["size"], kind=kind[ea], evidence=evidence[ea], cluster=cid.get(ea))
           for ea in sorted(kind)]
    json.dump(out, open(os.path.join(ROOT, "db", "stl.json"), "w"), indent=0)
    n = 0
    for ea in kind:
        if not rows[ea]["module"] or re.fullmatch(r"m[0-9A-F]{6}", rows[ea]["module"]):  # empty or a carved module
            rows[ea]["module"] = "stl"  # (re-run tools/modules.py carve afterwards)
            n += 1
    save_csv(rows)
    by = collections.Counter((kind[e], evidence[e]) for e in kind)
    print(f"{len(kind)} STL functions, {sum(F[e]['size'] for e in kind) / 1024:.0f} KB; module=stl set on {n}")
    for (k, ev), c in sorted(by.items()):
        print(f"  {k:14} {ev:7} {c}")


if __name__ == "__main__":
    main()
