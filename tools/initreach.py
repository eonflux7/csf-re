#!/usr/bin/env python3
"""Trap slots of the standalone build reachable from the C++ initialisers (static: direct calls in src/auto).

  initreach.py            list, one line per trap slot with its initialiser and call path (docs/INIT_REACH.txt)
  initreach.py --json     counts only (webmon)
  initreach.py --root 0x509250    the same walk from one function (WinMain) instead of the initialisers

A trap slot is an address build/cutover/stubs.json still fills with a trap. From each xc entry the walk follows the
callee addresses a source declares (CSF_FUNC, CSF_ADDR_FN). An initialiser hands its destructor to atexit as a
CSF_ADDR_FN: what is reached only through that runs at exit ("exit"), the rest during start-up ("startup").
Calls through vtables or pointers are not seen.
"""
import collections, json, os, re, sys

ROOT = os.path.dirname(os.path.dirname(os.path.abspath(__file__)))
FUNC = re.compile(r"CSF_FUNC\([^;]*?,\s*(0x[0-9A-Fa-f]+)")
ADDR = re.compile(r"CSF_ADDR_FN\(\s*(0x[0-9A-Fa-f]+)")


def refs(ea):
    p = os.path.join(ROOT, "src", "auto", f"{ea:08x}.cpp")
    if not os.path.exists(p):
        return None, None
    s = open(p, errors="replace").read()
    return {int(a, 16) for a in FUNC.findall(s)} - {ea}, {int(a, 16) for a in ADDR.findall(s)} - {ea}


def reach():
    """{trap address: dict(index, path, kind, when)} for every trap slot reachable from an xc initialiser."""
    stubs = {int(s["address"], 16): s for s in json.load(open(os.path.join(ROOT, "build/cutover/stubs.json")))["slots"]}
    ini = json.load(open(os.path.join(ROOT, "build/cutover/initializers.json")))["entries"]
    found = {}
    for e in ini:
        if e["table"] != "xc":
            continue
        root = int(e["target"], 16)
        calls, addrs = refs(root)
        if calls is None:
            continue
        for when, starts in (("startup", calls), ("exit", addrs)):
            seen, todo = set(starts), [(c, (root, c)) for c in starts]
            while todo:
                ea, path = todo.pop()
                if ea in stubs:
                    old = found.get(ea)
                    if not old or (old["when"] == "exit" and when == "startup"):
                        found[ea] = dict(index=e["index"], path=path, kind=stubs[ea]["kind"],
                                         name=stubs[ea].get("name", ""), when=when)
                    continue
                c, a = refs(ea)
                for n in (c or set()) | (a or set()):
                    if n not in seen:
                        seen.add(n)
                        todo.append((n, path + (n,)))
    return found


def reach_from(root):
    """{trap address: dict(path, kind, name)} reachable from one function through direct calls."""
    stubs = {int(s["address"], 16): s for s in json.load(open(os.path.join(ROOT, "build/cutover/stubs.json")))["slots"]}
    found, seen, todo = {}, {root}, [(root, (root,))]
    while todo:
        ea, path = todo.pop()
        c, a = refs(ea)
        for n in (c or set()) | (a or set()):
            if n in seen:
                continue
            seen.add(n)
            if n in stubs:
                found[n] = dict(path=path + (n,), kind=stubs[n]["kind"], name=stubs[n].get("name", ""))
            else:
                todo.append((n, path + (n,)))
    return found, len(seen)


def summary(found):
    out = {}
    for when in ("startup", "exit"):
        out[when] = dict(collections.Counter(f["kind"] for f in found.values() if f["when"] == when))
    return out


def main():
    if "--root" in sys.argv:
        root = int(sys.argv[sys.argv.index("--root") + 1], 16)
        found, n = reach_from(root)
        print(f"from 0x{root:08X}: {n} functions reached; trap slots",
              dict(collections.Counter(f["kind"] for f in found.values())))
        for t, f in sorted(found.items(), key=lambda x: (x[1]["kind"], len(x[1]["path"]))):
            print(f"{f['kind']:14} 0x{t:08X} {f['name'][:40]:40} via " + " > ".join("%X" % p for p in f["path"]))
        return
    found = reach()
    if "--json" in sys.argv:
        print(json.dumps(summary(found)))
        return
    s = summary(found)
    print("trap slots reachable from initialisers: start-up", s["startup"], "| exit only", s["exit"])
    for t, f in sorted(found.items(), key=lambda x: (x[1]["when"] != "startup", x[1]["index"])):
        print(f"{f['when']:7} xc[{f['index']}] {f['kind']:14} 0x{t:08X} {f['name'][:40]:40} via "
              + " > ".join("%X" % p for p in f["path"]))


if __name__ == "__main__":
    main()
