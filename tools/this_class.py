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
ARGS = os.path.join(ROOT, "db", "types", "arg_types.json")  # parameters / struct members that point to a class


def load_args():
    try:
        return json.load(open(ARGS))
    except (OSError, ValueError):
        return dict(params={}, fields={}, globals={}, returns={}, locals={})


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
                res[ea] = dict(vt=vt, src="plain" if db.classes[vt].get("plain") else "vtable")
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
    # backwards: what a function passes as `this` to a classed method (export/abi.json arg0_src).
    #   its own `this` (param 0 of a thiscall): the function is a method of that class (or of a derived one: the
    #   nearest common ancestor of everything it passes is the safe answer);
    #   another parameter: that parameter points to the class (sync.py --this types it);
    #   a 4-byte struct member: the member points to the class (sync.py types it in the IDB's struct).
    this_h, par_h, fld_h = collections.defaultdict(list), collections.defaultdict(list), collections.defaultdict(list)
    glob_h, ret_h = collections.defaultdict(list), collections.defaultdict(list)
    stk_h = collections.defaultdict(list)  # (caller, stack offset) -> [(class, room)]
    for caller, a in abi.items():
        thiscall = a.get("cc") == "thiscall"
        for callee, srcs in (a.get("arg0_src") or {}).items():
            c = res.get(int(callee, 16))
            if not c or not api.takes_reg(int(callee, 16), db, "ecx"):
                continue
            for s in srcs:
                if s[0] == "p" and s[1] == 0 and thiscall:
                    this_h[caller].append(c["vt"])
                elif s[0] == "p":
                    par_h[(caller, s[1])].append(c["vt"])
                elif s[0] == "f" and len(s) == 4:
                    fld_h[(s[1], s[3])].append(c["vt"])
                elif s[0] == "g" and 0x401000 <= int(s[1], 16) < 0x900000:  # Hex-Rays objects at small
                    glob_h[(s[1], s[2])].append(c["vt"])                   # "addresses" are constants
                elif s[0] == "r":
                    ret_h[s[1]].append((caller, c["vt"]))
                elif s[0] == "l":
                    stk_h[(caller, s[1])].append((c["vt"], s[2]))

    def nca(vts):
        chains = [ancestors(v) for v in set(vts)]
        return next((v for v in chains[0] if all(v in ch for ch in chains[1:])), None)
    n_back = 0
    for ea, vts in this_h.items():
        if ea not in res and ea in db.F:
            base = nca(vts)
            if base:
                res[ea] = dict(vt=base, src="callees", sites=len(vts))
                n_back += 1
    params = collections.defaultdict(dict)
    for (ea, i), vts in par_h.items():
        base = nca(vts)
        if base:
            params[f"{ea:08x}"][str(i)] = base
    fields = collections.defaultdict(dict)
    for (st, name), vts in fld_h.items():
        base = nca(vts)
        if base and st:
            fields[st][name] = base
    # a global passed as `this`: the object itself (&g_x) or a pointer to it (g_p); both kinds seen = a conflict
    globs = {}
    for (g, kind), vts in glob_h.items():
        base = nca(vts)
        if base and g not in globs:
            globs[g] = dict(vt=base, kind=kind)
        elif g in globs:
            globs[g] = None
    globs = {g: v for g, v in globs.items() if v}
    # a call's result passed as `this`: the function returns a pointer to that class (not when it returns
    # nothing / a float, Hex-Rays' own prototype disagreeing)
    # Only when >= 2 callers agree and at least half of the function's call sites use the result that way: one
    # caller passing an allocator's result to a ctor typed the allocator 0x6910F0 as returning that class, and
    # Hex-Rays spread it to every allocation (2026-10-09).
    nsites = collections.Counter()
    for caller, a in abi.items():
        for callee, counts in (a.get("calls") or {}).items():
            nsites[callee] += len(counts)
    rets = {}
    for ea, votes in ret_h.items():
        a = abi.get(int(ea, 16)) or {}
        callers = {c for c, _ in votes}
        if len(callers) < 2 or len(votes) < 0.5 * nsites[ea]:
            continue
        base = nca([v for _, v in votes])
        if base and a.get("ret") and a["ret"] not in ("void", "float", "double", "char", "bool", "__int16", "__int8"):
            rets[ea] = base
    # an object on a caller's stack (&local passed as `this`): typed when the class's size is known and fits
    # before the next stack variable (a smaller guess would overlap its neighbours in Hex-Rays)
    locs = collections.defaultdict(dict)
    for (caller, off), seen in stk_h.items():
        base = nca([v for v, _ in seen])
        size = db.classes.get(base, {}).get("size") or 0 if base else 0
        room = min(r for _, r in seen)
        if base and 0 < size <= room:
            locs[f"{caller:08x}"][str(off)] = base
    new_args = dict(params=params, fields=fields, globals=globs, returns=rets, locals=locs)
    old_args = json.loads(json.dumps(load_args(), sort_keys=True))
    args_changed = json.loads(json.dumps(new_args, sort_keys=True)) != old_args
    json.dump(new_args, open(ARGS, "w"), indent=0, sort_keys=True)
    print(f"arg types: {sum(len(v) for v in params.values())} parameters, {sum(len(v) for v in fields.values())} "
          f"struct members, {len(globs)} globals, {len(rets)} return values point to a class, "
          f"{sum(len(v) for v in locs.values())} stack objects typed; {n_back} methods "
          f"classed from what they pass to classed methods")
    json.dump({f"{ea:08x}": v for ea, v in sorted(res.items())}, open(OUT, "w"), indent=0)
    # progress = new methods, or any parameter / member / global / return / stack type not yet pushed to the IDB
    # (propagate.sh stops on "0 new"; types derived in a round are applied by the next round's sync)
    new = len(set(res) - set(old)) + (1 if args_changed else 0)
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
