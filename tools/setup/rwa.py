#!/usr/bin/env python3
"""RenderWare Audio is interleaved with RW toolkits and libpng in 0x7C4FA0-0x818000. Take the call-graph
component (calls within that range, undirected) containing the DirectSound/COM/winmm users and write it to
db/region_overrides.json as region `rwa` (lib; no librw counterpart -> needs its own replacement at cutover)."""
import collections, json, os

ROOT = os.path.dirname(os.path.dirname(os.path.dirname(os.path.abspath(__file__))))
LO, HI = 0x7C4FA0, 0x818000
SEEDS = {0x7F0120, 0x7F0670, 0x7F2420, 0x7F2EA0, 0x7E0D10, 0x7E25E0, 0x7F78E0}  # DSOUND/ole32/winmm/GetForegroundWindow users


def main():
    F = {}
    for l in open(os.path.join(ROOT, "export", "functions.jsonl")):
        d = json.loads(l)
        F[d["ea"]] = d
    V = {e for e in F if LO <= e < HI}
    adj = collections.defaultdict(set)
    for e in V:
        for c in F[e]["callees"]:
            if c in V:
                adj[e].add(c)
                adj[c].add(e)
    comp, st = set(SEEDS), list(SEEDS)
    while st:
        for y in adj[st.pop()]:
            if y not in comp:
                comp.add(y)
                st.append(y)
    p = os.path.join(ROOT, "db", "region_overrides.json")
    ov = json.load(open(p)) if os.path.exists(p) else {}
    ov = {k: v for k, v in ov.items() if v.get("by") != "rwa.py"}
    for e in sorted(comp):
        ov[f"{e:08x}"] = dict(region="rwa", by="rwa.py")
    json.dump(dict(sorted(ov.items())), open(p, "w"), indent=0)
    print(f"rwa: {len(comp)} functions, {sum(F[e]['size'] for e in comp) / 1024:.0f} KB, "
          f"{min(comp):#x}-{max(comp):#x}")


if __name__ == "__main__":
    main()
