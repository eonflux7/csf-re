#!/usr/bin/env python3
"""Which class each method belongs to (`this` type) -> db/types/this_class.json, for packets and the IDB.

Seeds (certain): vtable slots that take ecx (the most-base class listing them), constructors and destructors
(db/classes.json). Propagation (through Hex-Rays): once the IDB types the seeds' `this` (ida/sync.py --this),
Hex-Rays types the object every caller passes in ecx; export/abi.json `this_args` holds the struct each call site
passes. A method with no seed gets the class its call sites agree on: all of them, or one inheritance chain
(then their nearest common ancestor: a base method called with derived objects). Repeat sync --this, export_abi.py,
this_class.py until nothing new is found.

Usage: this_class.py [--report]
"""
import collections, json, os, re, sys
sys.path.insert(0, os.path.dirname(__file__))

ROOT = os.path.dirname(os.path.dirname(os.path.abspath(__file__)))
OUT = os.path.join(ROOT, "db", "types", "this_class.json")


def load():
    try:
        return {int(k, 16): v for k, v in json.load(open(OUT)).items()}
    except (OSError, ValueError):
        return {}


def build():
    import api
    from packet import DB
    db = DB()
    abi = api.ida_abi()
    by_name = {db.cls_name(vt): vt for vt in db.classes}

    def ancestors(vt):
        out = []
        while vt:
            out.append(vt)
            vt = db.classes.get(vt, {}).get("base")
        return out

    res, old = {}, load()
    for vt, c in db.classes.items():
        for m in c["ctors"]:
            res.setdefault(int(m, 16), dict(vt=vt, src="ctor"))
        for m in c["dtors"]:
            res.setdefault(int(m, 16), dict(vt=vt, src="dtor"))
    for ea in db.owner:
        if ea not in res and api.takes_reg(ea, db, "ecx"):
            vt = db.class_of(ea)
            if vt:
                res[ea] = dict(vt=vt, src="vtable")
    sites = collections.defaultdict(list)
    for caller, a in abi.items():
        for callee, names in (a.get("this_args") or {}).items():
            sites[int(callee, 16)] += [by_name[n] for n in names if n in by_name]
    n_prop = 0
    for ea, vts in sites.items():
        if ea in res or not vts or ea not in db.F or not api.takes_reg(ea, db, "ecx"):
            continue
        chains = [ancestors(v) for v in set(vts)]
        # nearest common ancestor of every class seen at the call sites (itself when only one is seen)
        base = next((v for v in chains[0] if all(v in ch for ch in chains[1:])), None)
        if base:
            res[ea] = dict(vt=base, src="calls", sites=len(vts))
            n_prop += 1
    json.dump({f"{ea:08x}": v for ea, v in sorted(res.items())}, open(OUT, "w"), indent=0)
    new = len(set(res) - set(old))
    srcs = collections.Counter(v["src"] for v in res.values())
    print(f"this_class: {len(res)} methods with a class ({dict(srcs)}); {new} new since the last run")
    return res, new


def report():
    from packet import DB
    import api
    db = DB()
    tc = load()
    todo = [ea for ea, r in db.rows.items() if r["region"] == "game" and r["status"] not in ("done", "verified", "lib")
            and ea in db.F]
    meth = [ea for ea in todo if api.takes_reg(ea, db, "ecx")]
    print(f"undone methods (take ecx): {len(meth)}; with a class: {sum(ea in tc for ea in meth)} "
          f"({sum(ea in tc for ea in meth) / max(1, len(meth)):.0%})")


if __name__ == "__main__":
    report() if "--report" in sys.argv else build()
