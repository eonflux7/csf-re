#!/usr/bin/env python3
"""Library boundary API: lib functions called directly from game code -> export/boundary_api.json
({lib: [ea hex, ...]}, plus export/boundary_callers.json {ea: [game caller eas]})."""
import collections, json, os, sys
sys.path.insert(0, os.path.dirname(__file__))
from triage import load_csv

ROOT = os.path.dirname(os.path.dirname(os.path.abspath(__file__)))


def main():
    rows = load_csv()
    region = {ea: r["region"] for ea, r in rows.items()}
    callers = collections.defaultdict(set)
    for l in open(os.path.join(ROOT, "export", "functions.jsonl")):
        f = json.loads(l)
        if region.get(f["ea"]) != "game":
            continue
        for c in f["callees"]:
            if region.get(c, "game") not in ("game", "unknown"):
                callers[c].add(f["ea"])
    api = collections.defaultdict(list)
    for ea in sorted(callers):
        api[region[ea]].append(f"{ea:08x}")
    json.dump(api, open(os.path.join(ROOT, "export", "boundary_api.json"), "w"), indent=0)
    json.dump({f"{ea:08x}": [f"{c:08x}" for c in sorted(cs)] for ea, cs in sorted(callers.items())},
              open(os.path.join(ROOT, "export", "boundary_callers.json"), "w"))
    print({k: len(v) for k, v in api.items()})


if __name__ == "__main__":
    main()
