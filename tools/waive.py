#!/usr/bin/env python3
"""Waive gate findings for ONE function's exact compiled code (Opus sign-off), instead of loosening semcheck for all.

The waiver stores the code hash (tools/verify.py) of the reviewed file: if the code changes, the waiver stops
applying and the gate re-checks normally. Only post-compile findings (semcheck, ret N) can be waived; file rules
(includes, asm, HOOK, literals) cannot.
Usage: waive.py <ea> <regex matching the finding> <why> [file]      (file default: src/auto/<ea>.cpp)
       waive.py --list
"""
import os, re, subprocess, sys, tempfile
sys.path.insert(0, os.path.dirname(__file__))
import yaml
from gate import WAIVERS, compile_cmd, gate, load_waivers
from verify import code_hash

ROOT = os.path.dirname(os.path.dirname(os.path.abspath(__file__)))


def main():
    if sys.argv[1:2] == ["--list"]:
        for ea, w in sorted(load_waivers().items()):
            print(f"{ea:08x} {w['code']} {w['checks']}  {w['why']}  ({w.get('by', '?')}, {w.get('date', '?')})")
        return
    if len(sys.argv) < 4:
        sys.exit(__doc__)
    ea = int(sys.argv[1], 16)
    pattern, why = sys.argv[2], sys.argv[3]
    path = os.path.abspath(sys.argv[4] if len(sys.argv) > 4 else os.path.join(ROOT, "src", "auto", f"{ea:08x}.cpp"))
    ok, err = gate(ea, path)
    if ok:
        sys.exit(f"gate already passes: {err}")
    if not re.search(pattern, err):
        sys.exit(f"pattern doesn't match the gate's finding:\n{err}")
    with tempfile.TemporaryDirectory() as td:
        obj = os.path.join(td, "t.obj")
        r = subprocess.run(compile_cmd() + [f"/Fo{obj}", "-c", "--", path], capture_output=True, text=True,
                           cwd=os.path.join(ROOT, "build"))
        if r.returncode:
            sys.exit("compile error: file rules and compile errors can't be waived")
        h = code_hash(obj)
    d = {}
    if os.path.exists(WAIVERS):
        d = yaml.safe_load(open(WAIVERS)) or {}
    import datetime
    e = d.setdefault(f"{ea:08x}", {})
    e.update(code=h, why=why, by=os.environ.get("CSF_WAIVER_BY", "opus"), date=datetime.date.today().isoformat())
    e["checks"] = sorted(set(e.get("checks", []) if e.get("code") == h else []) | {pattern})
    with open(WAIVERS, "w") as f:
        f.write("# Gate findings waived for one function's exact compiled code (tools/waive.py). Keyed by ea.\n")
        yaml.safe_dump(d, f, sort_keys=True, width=120)
    ok, err = gate(ea, path)
    print(("gate now passes: " if ok else "gate still fails (other findings):\n") + err)


if __name__ == "__main__":
    main()
