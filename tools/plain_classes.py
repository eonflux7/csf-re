#!/usr/bin/env python3
"""Classes without a vtable (plain structs with methods) -> entries in db/classes.json marked "plain": true.

db/classes.json (tools/classes.py) only knows polymorphic classes: its ids are vtables. About half the methods the
game calls with ecx = this belong to classes without one (no vtable store anywhere), so no packet, no header and no
Hex-Rays type ever told a grunt what `this` is. Here, among the __thiscall functions no class claims (Hex-Rays'
calling convention, export/abi.json; db/types/this_class.json after the propagation loop):

  same object   a method that calls another on its own `this` (classes.py's linear this-tracking, and Hex-Rays'
                arg0_src ["p", 0]) shares its class: union-find over those edges;
  allocation    `push size; call new; mov ecx, eax; call f` (classes.py alloc_sites) gives the size;
  fields        [this+disp] accesses of every method in the cluster (classes.py this_scan), as for vtable classes.

A cluster becomes a class when it has 2+ methods or a known size (a lone method gains nothing over offsets).
Id = its lowest method's address (code addresses never collide with vtables, which live in .rdata); header name
S_<id> (gen_classes_h.py), renamable like any class in db/types/classes.yaml. Re-run after classes.py (which
rewrites the file) and after the propagation loop; tools/ida/propagate.sh then types them in the IDB.

Usage: plain_classes.py [--dry]
"""
import collections, json, os, sys
sys.path.insert(0, os.path.dirname(__file__))

ROOT = os.path.dirname(os.path.dirname(os.path.abspath(__file__)))
CLASSES = os.path.join(ROOT, "db", "classes.json")


def main(dry=False):
    import api, this_class
    import classes as cl  # capstone this-tracking + allocation sites (loads the exe image)
    from packet import DB
    db = DB()
    abi = api.ida_abi()
    cur = json.load(open(CLASSES))
    keep = [c for c in cur if not c.get("plain")]
    owned = {int(m, 16) for c in keep for m in c["methods"] + c["ctors"] + c["dtors"]}
    owned |= set(this_class.load())
    # STL instances (db/stl.json, module stl) are VC7.1 container layouts, written by hand from a replica (PLAN):
    # left out, else every vector<T> helper glues the classes holding one into a single giant "class"
    stl = {int(x["ea"], 16) for x in json.load(open(os.path.join(ROOT, "db", "stl.json")))}
    meth = {ea for ea, a in abi.items() if a.get("cc") == "thiscall" and ea in db.rows
            and db.rows[ea]["region"] == "game" and db.rows[ea]["module"] != "stl" and ea not in stl
            and ea not in owned and ea in cl.F and not cl.F[ea]["lib"]}
    par = {m: m for m in meth}

    def find(x):
        while par[x] != x:
            par[x] = par[par[x]]
            x = par[x]
        return x
    scans = {}
    for m in meth:
        scans[m] = cl.this_scan(m)
        for t in scans[m][2]:  # called with ecx = this
            if t in meth:
                par[find(m)] = find(t)
        for callee, srcs in (abi[m].get("arg0_src") or {}).items():
            t = int(callee, 16)
            if t in meth and any(s[0] == "p" and s[1] == 0 for s in srcs):
                par[find(m)] = find(t)
    sizes = cl.alloc_sites()
    comp = collections.defaultdict(set)
    for m in meth:
        comp[find(m)].add(m)
    out = []
    for ms in comp.values():
        sz = set().union(*(sizes.get(m, set()) for m in ms))
        if len(ms) < 2 and not sz:
            continue
        fields = collections.defaultdict(lambda: [0, "", 0])
        for m in ms:
            for off, s, rw in scans[m][1]:
                e = fields[off]
                e[0] = max(e[0], s)
                e[1] += "".join(ch for ch in rw if ch not in e[1])
                e[2] += 1
        cid = f"{min(ms):08x}"
        out.append(dict(vtable=cid, plain=True, base=None, size=min(sz) if len(sz) == 1 else None,
                        sizes=sorted(sz), ctors=[], dtors=[], methods=[f"{x:08x}" for x in sorted(ms)],
                        fields={f"{o:04x}": dict(size=s, rw=rw, n=n) for o, (s, rw, n) in sorted(fields.items())}))
    n_m = sum(len(c["methods"]) for c in out)
    print(f"plain classes: {len(out)} classes, {n_m} methods (of {len(meth)} unclaimed __thiscall functions), "
          f"{sum(1 for c in out if c['size'])} with a size, {sum(len(c['fields']) for c in out)} fields")
    if not dry:
        json.dump(keep + out, open(CLASSES, "w"), indent=1)
    return out


if __name__ == "__main__":
    main("--dry" in sys.argv)
