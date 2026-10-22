#!/usr/bin/env python3
"""Name the addresses of a standalone run's dump (crash.py output or the CSF_HANG_SECONDS dump in winmain.out)
from the link map: each hex word in [MAINBASE, MAINBASE+image) becomes map symbol + offset, with the original
address when the symbol is a HOOK'd function (its source file is src_auto_<ea>.cpp) or a fn_slot trap.

  stdsym.py [dump-file]     (default $CSF_STANDALONE/winmain.out; stdin with -)
"""
import bisect, re, sys
from pathlib import Path

ROOT = Path(__file__).resolve().parents[2]
sys.path.insert(0, str(ROOT / "tools"))
from paths import STANDALONE
MAP = ROOT / "build/cutover/csf_standalone.map"
PREF = 0x400000


def load():
    syms = []
    for l in MAP.read_text(errors="replace").splitlines():
        m = re.match(r"\s*[0-9a-f]{4}:[0-9a-f]{8}\s+(\S+)\s+([0-9a-f]{16})\s+(?:f\s+)?(?:i\s+)?(\S+)?", l)
        if m:
            syms.append((int(m.group(2), 16), m.group(1), m.group(3) or ""))
    syms.sort()
    return syms


def main():
    src = sys.argv[1] if len(sys.argv) > 1 else str(Path(STANDALONE) / "winmain.out")
    text = sys.stdin.read() if src == "-" else open(src, errors="replace").read()
    m = re.search(r"MAINBASE ([0-9A-Fa-f]{8})", text)
    if not m:
        sys.exit("no MAINBASE line in the dump")
    base = int(m.group(1), 16)
    syms = load()
    keys = [s[0] for s in syms]

    def name(v):
        va = v - base + PREF
        i = bisect.bisect_right(keys, va) - 1
        if i < 0 or va - keys[i] > 0x100000:
            return None
        a, sym, obj = syms[i]
        if sym.startswith("?image@") and "data.cpp" in obj:  # the data image is the original's from 0x82D000 on
            return f"data {0x82D000 + va - a:08X}"
        ea = re.search(r"src_auto_([0-9a-f]{8})\.cpp", obj)
        return f"{sym}+{va - a:X}" + (f" [orig {ea.group(1).upper()}]" if ea else "") + (f" ({obj})" if obj else "")

    for l in text.splitlines():
        l = l.split("    ;")[0]  # crash.py's symbolize.py names addresses as the original's: wrong here
        if re.match(r"\s*MEM", l):
            head, _, rest = l.partition(": ")
            print(head + ": " + " ".join(f"{w}" + (f"<{n}>" if (n := name(int(w, 16))) and int(w, 16) >= base else "")
                                          for w in rest.split()))
            continue
        if not re.match(r"\s*(STK|EIP|ASM|CRASH)", l):
            if "MAINBASE" in l or "HANG" in l:
                print(l)
            continue
        ws = [int(w, 16) for w in re.findall(r"\b[0-9A-Fa-f]{8}\b", l)]
        hit = [n for n in (name(w) for w in ws[-1:] if w >= base) if n]
        print(l + ("    " + hit[0] if hit else ""))


if __name__ == "__main__":
    main()
