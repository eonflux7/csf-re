#!/usr/bin/env python3
"""Rename pass: apply db/types/classes.yaml (conf >= 0.7) to src/ and db/functions.csv.

- Type tokens `C_<vtable>` become the class name (src/types/auto_classes.h is regenerated with the same names).
- Method identifiers `C_<vtable>_<Rest>` bound to an address (HOOK / CSF_FUNC) get the prefix of the class that
  owns that address: the most-base class whose vtable/methods contain it (grunts often used the first derived class
  they saw, e.g. TCharTask methods named C_0082fbe8_*). Unbound ones keep their class and get its name.
  Functions shared by unrelated hierarchies (COMDAT-folded stubs) keep their prefix.
- Named scalar fields (`fields:` in classes.yaml) replace field_XXXX in files whose only class type is that class
  or one derived from it.
- db/functions.csv names of renamed functions follow; names stay unique (an _<EA> suffix on collision). Done
  functions with an empty csv name get their HOOK name.

Idempotent. Usage: rename_classes.py [--dry]
"""
import argparse, collections, csv, glob, json, os, re, subprocess, sys

ROOT = os.path.dirname(os.path.dirname(os.path.abspath(__file__)))
sys.path.insert(0, os.path.dirname(os.path.abspath(__file__)))
from gen_classes_h import load_names  # noqa: E402
from triage import load_csv, save_csv  # noqa: E402

C = {c["vtable"]: c for c in json.load(open(os.path.join(ROOT, "db", "classes.json")))}
NAMES = load_names()
OWNERS = collections.defaultdict(list)
for vt, c in C.items():
    for m in c["methods"]:
        OWNERS[int(m, 16)].append(vt)


def ancestors(vt):
    out = []
    while vt and vt in C and vt not in out:
        out.append(vt)
        vt = C[vt]["base"]
    return out


def owner(ea):
    """Most-base owner if all owners lie on one inheritance chain, else None (shared/folded function)."""
    os_ = OWNERS.get(ea, [])
    for o in os_:
        if all(o in ancestors(p) for p in os_):
            return o
    return None


def cname(vt):
    return NAMES[vt]["name"] if vt in NAMES else (f"S_{vt}" if C.get(vt, {}).get("plain") else f"C_{vt}")


def derived_or_self(vt):
    return {v for v in C if vt in ancestors(v)}


BIND = re.compile(r"\bHOOK(?:_METHOD|_KEEP_ECX)?\(\s*(0x[0-9A-Fa-f]+)\s*,\s*(\w+)|"
                  r"\bCSF_FUNC\(\s*[^,]+,\s*\w+\s*,\s*(\w+)\s*,\s*(0x[0-9A-Fa-f]+)")
METHOD = re.compile(r"\bC_([0-9a-f]{8})_(\w+)")
TYPE = re.compile(r"\bC_([0-9a-f]{8})\b")


def rewrite(text):
    bound = {}
    for m in BIND.finditer(text):
        if m.group(1):
            bound[m.group(2)] = int(m.group(1), 16)
        else:
            bound[m.group(3)] = int(m.group(4), 16)
    renames = {}

    def meth(m):
        ident, vt, rest = m.group(0), m.group(1), m.group(2)
        ea = bound.get(ident)
        o = owner(ea) if ea is not None else None
        if ea is not None and OWNERS.get(ea) and o is None:
            new = ident if vt not in NAMES else f"{cname(vt)}_{rest}"  # folded: keep the class the grunt used
        else:
            new = f"{cname(o or vt)}_{rest}"
        if new != ident and ea is not None:
            renames[ident] = (new, ea)
        return new

    out = METHOD.sub(meth, text)
    types = set(TYPE.findall(out))
    out = TYPE.sub(lambda m: cname(m.group(1)), out)
    # named fields: only when the file uses exactly one class type
    used = types | {vt for vt, e in NAMES.items() if re.search(rf"\b{re.escape(e['name'])}\b", out)}
    used |= set(re.findall(r"\bS_([0-9a-f]{8})\b", out))  # plain classes (S_<ea>) count as a second type too
    if len(used) == 1:
        (vt,) = used
        for base in ancestors(vt):
            for o, n in (NAMES.get(base, {}).get("fields") or {}).items():
                o = int(o, 16) if isinstance(o, str) else int(o)  # classes.yaml: hex strings or ints
                out = re.sub(rf"\bfield_{o:04x}\b", n.split()[-1], out)  # "T* m_x" -> m_x
    return out, renames


def main():
    ap = argparse.ArgumentParser()
    ap.add_argument("--dry", action="store_true")
    a = ap.parse_args()
    files = sorted(glob.glob(os.path.join(ROOT, "src", "auto", "*.cpp")) + glob.glob(os.path.join(ROOT, "src", "core", "*.cpp")))
    all_ren, changed = {}, 0
    for p in files:
        t = open(p).read()
        n, ren = rewrite(t)
        all_ren.update({ea: (old, new) for old, (new, ea) in ren.items()})
        if n != t:
            changed += 1
            if not a.dry:
                open(p, "w").write(n)
    rows = load_csv()
    # done functions whose name never reached the csv (missing grunt metadata): take the HOOK name
    filled = 0
    for p in glob.glob(os.path.join(ROOT, "src", "auto", "*.cpp")):
        for m in BIND.finditer(open(p).read()):
            if m.group(1):
                r = rows.get(int(m.group(1), 16))
                if r and not r["name"] and r["status"] == "done":
                    r["name"], r["name_src"] = m.group(2), "agent"
                    filled += 1
    taken = collections.Counter(r["name"] for r in rows.values() if r["name"])
    csv_ren = 0
    for ea, (old, new) in sorted(all_ren.items()):
        r = rows.get(ea)
        if not r or r["name"] != old:
            continue
        if taken[new]:
            new = f"{new}_{ea:X}"
        taken[old] -= 1
        taken[new] += 1
        r["name"] = new
        csv_ren += 1
        if new != all_ren[ea][1]:  # keep the source in step with the csv
            p = os.path.join(ROOT, "src", "auto", f"{ea:08x}.cpp")
            if os.path.exists(p) and not a.dry:
                t = open(p).read()
                open(p, "w").write(re.sub(rf"\b{all_ren[ea][1]}\b", new, t))
    if not a.dry:
        save_csv(rows)
        subprocess.run([sys.executable, os.path.join(ROOT, "tools", "gen_classes_h.py")], check=True)
    print(f"{len(NAMES)} named classes; {changed} files changed; {len(all_ren)} functions renamed, {csv_ren} in functions.csv"
          + f"; {filled} empty csv names filled from HOOK" + (" (dry run)" if a.dry else ""))


if __name__ == "__main__":
    main()
