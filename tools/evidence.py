#!/usr/bin/env python3
"""Read-only evidence a grunt may request while reimplementing a function (bounded; see grunt.work()).

Tools (target = hex address, or a class name / vtable for `class`):
  pseudo  <ea>     Hex-Rays pseudocode of another function, current names substituted (+ our reimplementation if done)
  asm     <ea>     its disassembly (IDA code heads)
  callers <ea>     functions that call it, with names
  class   <vt|name> the class's struct from types/auto_classes.h + its vtable slots with names
Usage: evidence.py <tool> <target>      (prints what a grunt would get)
"""
import json, os, re, sys
sys.path.insert(0, os.path.dirname(__file__))
from packet import DB, disasm, pseudo, struct_text, substitute_names

ROOT = os.path.dirname(os.path.dirname(os.path.abspath(__file__)))
LIMIT = 6000
TOOLS = ("pseudo", "asm", "callers", "class")

PROMPT = """
Evidence requests (optional): if a concrete uncertainty blocks you (what a callee returns or expects, the layout of a
class, how callers use the result), you may answer with ONLY one ```json block instead of the file:
  {"actions": [{"tool": "pseudo", "target": "0x4A1230"}, ...]}
tools: pseudo <ea> (another function's pseudocode, and our reimplementation if one exists), asm <ea>, callers <ea>,
class <vtable or class name> (struct + vtable slots). At most %d requests for this function in total; then answer
with the file as usual. Don't request what the packet already shows."""

_vtables = None


def _bound(text):
    return text if len(text) <= LIMIT else text[:LIMIT] + f"\n... [truncated, {len(text) - LIMIT} more chars]"


def _ea(target):
    return int(str(target).lower().replace("sub_", "0x"), 16)


def run(db, tool, target):
    global _vtables
    try:
        if tool == "pseudo":
            ea = _ea(target)
            p = pseudo(ea)
            out = substitute_names(db, p) if p else "no pseudocode exported for this address"
            ours = os.path.join(ROOT, "src", "auto", f"{ea:08x}.cpp")
            if os.path.exists(ours):
                out += f"\n\n// already reimplemented (src/auto/{ea:08x}.cpp):\n" + open(ours).read()
            return _bound(out)
        if tool == "asm":
            ea = _ea(target)
            if ea not in db.F:
                return "not a function start"
            return _bound("\n".join(f"{i.address:08X}  {i.mnemonic} {i.op_str}" for i in disasm(ea, db.F[ea]["size"])))
        if tool == "callers":
            ea = _ea(target)
            cs = sorted(f["ea"] for f in db.F.values() if ea in f["callees"])
            if not cs:
                return "no direct callers (only called through pointers / vtables, or unused)"
            lines = [f"0x{c:X} {db.name(c) or ''}" for c in cs[:40]]
            return "\n".join(lines) + (f"\n... {len(cs) - 40} more" if len(cs) > 40 else "")
        if tool == "class":
            t = str(target)
            vt = next((v for v, e in db.class_names.items() if e["name"] == t), None)
            vt = vt or f"{_ea(t.replace('C_', '0x')):08x}"
            if vt not in db.classes:
                return "unknown class"
            out = [struct_text(db, vt) or f"// no struct for {db.cls_name(vt)}"]
            if _vtables is None:
                _vtables = {v["addr"]: v for v in json.load(open(os.path.join(ROOT, "db", "vtables.json")))}
            v = _vtables.get(vt)
            if v:
                out.append(f"// vtable 0x{vt.upper()} ({db.cls_name(vt)}): slot (byte offset) -> function")
                out += [f"//   {i:2d} (+0x{4 * i:X}) 0x{s.upper()} {db.name(int(s, 16)) or ''}"
                        for i, s in enumerate(v["slots"])]
            return _bound("\n".join(out))
    except (ValueError, KeyError) as e:
        return f"bad target {target!r}: {e}"
    return f"unknown tool {tool!r} (tools: {', '.join(TOOLS)})"


def answer(db, actions, budget):
    """Run up to `budget` actions. Returns (message for the model, number used)."""
    parts, used = [], 0
    for a in actions:
        if used >= budget:
            parts.append("(request budget exhausted: answer with the file now)")
            break
        if not isinstance(a, dict):
            continue
        tool, target = str(a.get("tool", "")), str(a.get("target", ""))
        parts.append(f"## {tool} {target}\n```\n{run(db, tool, target)}\n```")
        used += 1
    left = budget - used
    tail = (f"\n\n{left} request(s) left. Answer with the file (```cpp + ```json metadata), or request more evidence."
            if left else "\n\nNo requests left: answer with the file now (```cpp + ```json metadata).")
    return "Evidence you requested:\n\n" + "\n\n".join(parts) + tail, used


if __name__ == "__main__":
    if len(sys.argv) != 3:
        sys.exit(__doc__)
    print(run(DB(), sys.argv[1], sys.argv[2]))
