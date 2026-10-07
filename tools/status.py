#!/usr/bin/env python3
"""Project dashboard: phase checklist + function progress by region/module (count and bytes).

Usage: tools/status.py [--modules]
"""
import collections, csv, os, re, subprocess, sys

ROOT = os.path.dirname(os.path.dirname(os.path.abspath(__file__)))


def checklist():
    text = open(os.path.join(ROOT, "docs", "PLAN.md")).read()
    sect = text.split("## Phase checklist", 1)[1].split("\n## ", 1)[0]
    phase, out, nxt = None, [], []
    for line in sect.splitlines():
        if line.startswith("### "):
            phase = line[4:].strip()
            out.append([phase, 0, 0])
        elif m := re.match(r"- \[( |x)\] (.*)", line.strip()):
            out[-1][2] += 1
            if m.group(1) == "x":
                out[-1][1] += 1
            elif len(nxt) < 5:
                nxt.append(f"{out[-1][0].split('—')[0].strip()}: {m.group(2)}")
    return out, nxt


def functions(by_module):
    path = os.path.join(ROOT, "db", "functions.csv")
    if not os.path.exists(path):
        return None
    rows = list(csv.DictReader(open(path)))
    names = {}
    if by_module:  # db/modules.yaml (tools/modules.py): show reviewed names, in address order
        import yaml
        mp = os.path.join(ROOT, "db", "modules.yaml")
        for m in (yaml.safe_load(open(mp)) if os.path.exists(mp) else []):
            names[m["id"]] = f"{m['id']} {m['name']}" if m.get("name") else m["id"]
    agg = collections.defaultdict(lambda: collections.Counter())
    for r in rows:
        key = (names.get(r["module"], r["module"]) or "(unassigned)") if (by_module and r["region"] == "game") \
            else r["region"]
        a = agg[key]
        a["n"] += 1
        a["bytes"] += int(r["size"])
        a["named"] += bool(r["name"])
        a["n_" + r["status"]] += 1
        a["b_" + r["status"]] += int(r["size"])
    return agg


def main():
    by_module = "--modules" in sys.argv
    phases, nxt = checklist()
    print("== Phases")
    for name, done, total in phases:
        mark = "✅" if "✅" in name or (total and done == total) else f"{done}/{total}" if total else "—"
        print(f"  {mark:>6}  {name}")
    print("\n== Next open items")
    for n in nxt:
        print("  -", n)
    agg = functions(by_module)
    if agg:
        print(f"\n== Functions by {'module (game only)' if by_module else 'region'}")
        print(f"  {'':32s} {'funcs':>6s} {'KB':>6s} {'named':>6s} {'done':>6s} {'verif':>6s} {'esc':>4s}  done%bytes")
        order = (lambda x: x[0].split()[0]) if by_module else (lambda x: -x[1]["bytes"])
        for k, a in sorted(agg.items(), key=order):
            done_b = a["b_done"] + a["b_verified"]
            pct = f"{100 * done_b / a['bytes']:.1f}%" if k == "game" or by_module else "-"
            print(f"  {k:32s} {a['n']:6d} {a['bytes'] // 1024:6d} {a['named']:6d} {a['n_done']:6d} "
                  f"{a['n_verified']:6d} {a['n_escalated']:4d}  {pct}")
    try:  # verification depth of src/auto (db/verify.json, filled by every SMOKE OK)
        sys.path.insert(0, os.path.dirname(__file__))
        import verify
        print("\n== Verification depth (src/auto; `verify.py report --list gate`)")
        print("\n".join(verify.summary_lines()))
    except Exception as e:  # no build dir yet etc.
        print(f"\n== Verification depth: unavailable ({e})")
    q = os.path.join(ROOT, "queue")
    if os.path.isdir(q):
        items = [f for f in os.listdir(q) if not f.startswith(".")]
        print(f"\n== Escalation queue: {len(items)} item(s)")
    log = subprocess.run(["git", "-C", ROOT, "log", "--oneline", "-5"], capture_output=True, text=True).stdout
    if log:
        print("\n== Recent commits\n" + "".join("  " + l + "\n" for l in log.splitlines()))
    open_dec = open(os.path.join(ROOT, "docs", "PLAN.md")).read().split("## Open decisions", 1)
    if len(open_dec) > 1:
        n = len(re.findall(r"^\d+\. ", open_dec[1], re.M))
        print(f"== Open decisions for the user: {n} (docs/PLAN.md)")


if __name__ == "__main__":
    main()
