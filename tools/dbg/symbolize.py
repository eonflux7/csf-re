#!/usr/bin/env python3
"""Annotate addresses in a dbg report (stdin): game functions via db/functions.csv, our DLL via build/dinput8.map."""
import bisect, csv, os, re, sys
ROOT = os.path.dirname(os.path.dirname(os.path.dirname(os.path.abspath(__file__))))
rows = sorted((int(r["ea"], 16), int(r["size"]), r["name"] or f"sub_{r['ea'].upper()}", r["region"])
              for r in csv.DictReader(open(os.path.join(ROOT, "db", "functions.csv"))))
starts = [r[0] for r in rows]
dll = []
for l in open(os.path.join(ROOT, "build", "dinput8.map"), errors="ignore"):
    m = re.match(r"\s*0001:([0-9a-f]{8})\s+(\S+)\s+([0-9a-f]{8,16})", l, re.I)
    if m and int(m.group(3), 16) >= 0x10001000:  # skip absolute/zero symbols
        dll.append((int(m.group(3), 16) - 0x10000000, m.group(2)))
dll.sort()
dstarts = [d[0] for d in dll]
base = None


def sym(v):
    if 0x401000 <= v < 0x900000:
        i = bisect.bisect_right(starts, v) - 1
        if i >= 0 and v < rows[i][0] + rows[i][1]:
            return f"{rows[i][2]}+{v - rows[i][0]:X} [{rows[i][3]}]"
    if base and base <= v < base + 0x200000:
        i = bisect.bisect_right(dstarts, v - base) - 1
        if i >= 0:
            return f"dinput8!{dll[i][1][:60]}+{v - base - dll[i][0]:X}"
    return None


for line in sys.stdin:
    line = line.rstrip("\r\n")
    m = re.match(r"DLLBASE ([0-9A-F]{8})", line)
    if m:
        base = int(m.group(1), 16)
    notes = [s for s in (sym(int(h, 16)) for h in re.findall(r"\b[0-9A-F]{8}\b", line)) if s]
    print(line + (("    ; " + ", ".join(notes)) if notes else ""))
