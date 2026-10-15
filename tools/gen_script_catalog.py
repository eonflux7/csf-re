#!/usr/bin/env python3
"""Generate src/auto/0076a540.cpp: the constructor of the mission-script catalog (types and commands).

The original is 7300 instructions of straight-line code: ~420 registration calls whose arguments are constants,
addresses of parameter descriptors built in the frame just before, or a 4-byte value built in place on the stack.
A model writing it by hand stops after a dozen entries; this walks the disassembly once with a symbolic stack and
emits one statement per store and per call, in the original's order.

  gen_script_catalog.py [--out FILE] [--table FILE]    (--table: the calls as a readable list)
"""
import json, os, re, sys

ROOT = os.path.dirname(os.path.dirname(os.path.abspath(__file__)))
sys.path.insert(0, os.path.join(ROOT, "tools"))
import api  # noqa: E402
import packet  # noqa: E402
from packet import DB, disasm, ret_bytes  # noqa: E402

EA = 0x76A540
SORT, PRED = 0x769C70, 0x768E50
REG32 = ("eax", "ebx", "ecx", "edx", "esi", "edi")


def main():
    out = sys.argv[sys.argv.index("--out") + 1] if "--out" in sys.argv else os.path.join(ROOT, "src", "auto", "0076a540.cpp")
    db = DB()
    ins = disasm(EA, db.F[EA]["size"])
    strings = {int(k, 16): v for k, v in json.load(open(os.path.join(ROOT, "export", "strings.json"))).items()}
    frame = next(int(i.op_str.split(",")[1], 0) for i in ins if i.mnemonic == "sub" and i.op_str.startswith("esp"))

    def addr(v):
        """An image address as the right macro."""
        s = strings.get(v)
        if s is not None and s and all(32 <= ord(c) < 127 for c in s) and "??" not in s:
            return 'CSF_STR(0x%08X, "%s")' % (v, s.replace("\\", "\\\\").replace('"', '\\"'))
        if v in db.F:
            return "CSF_ADDR_FN(0x%08X)" % v
        return "CSF_ADDR_DATA(0x%08X)" % v

    def is_addr(v):
        return isinstance(v, int) and 0x401000 <= v < 0x900000

    def as_type(v, t):
        """Value v (int constant, ('frame', off) or ('slot', k) resolved before) as parameter type t."""
        t = t.strip()
        ptr = "*" in t
        if isinstance(v, tuple) and v[0] == "frame":
            e = "frame + 0x%02X" % v[1]
            return "reinterpret_cast<%s>(%s)" % (t, e) if ptr else "reinterpret_cast<%s>(%s)" % (t, e)
        if is_addr(v):
            a = addr(v)
            if a.startswith("CSF_STR"):
                return "reinterpret_cast<%s>(const_cast<char*>(%s))" % (t, a) if ptr else \
                    "reinterpret_cast<%s>(%s)" % (t, a)
            return "reinterpret_cast<%s>(%s)" % (t, a) if ptr else "static_cast<%s>(%s)" % (t, a)
        if ptr:
            return "nullptr" if v == 0 else "reinterpret_cast<%s>(static_cast<uintptr_t>(0x%X))" % (t, v)
        if v >= 0x80000000:
            v -= 1 << 32
        return str(v) if -1 <= v < 10 else ("0x%X" % v if v > 0 else str(v))

    callee = {}
    for i in ins:
        if i.mnemonic == "call":
            t = int(i.op_str, 16)
            if t not in callee:
                a = api.load()[t]
                params = [p.strip() for p in re.split(r",(?![^<(]*[>)])", a["params"]) if p.strip()]
                stack = params[2:] if a["conv"] == "__fastcall" else params
                types = [re.sub(r"\s*\w*$", "", p) if re.search(r"[\s*&]\w+$", p) else p for p in stack]
                callee[t] = dict(name=a["name"], conv=a["conv"], types=types, decl=api.decl_line(t),
                                 n=len(stack), pops=(ret_bytes(t, db.F[t]["size"]) or [0])[0])
                if t == 0x769FC0:
                    # Its current source uses a local Vector alias, unavailable
                    # in this TU. Keep the same pointer ABI without that alias.
                    callee[t].update(name='sub_769FC0', decl='CSF_FUNC(unsigned int, __fastcall, sub_769FC0, 0x00769FC0, (void* self, void* edx, unsigned int a2));')

    reg = {r: None for r in REG32}  # None = unknown, int = constant, ('frame', off), 'this', ('esp', depth)
    reg["ecx"] = "this"
    stack = []  # values, top last
    body, table = [], []
    saved = 0   # callee-save pushes of the prologue

    def val(op):
        op = op.strip()
        if op in reg:
            return reg[op]
        if op == "bl":
            return reg["ebx"] & 0xFF if isinstance(reg["ebx"], int) else None
        if re.fullmatch(r"-?(0x[0-9a-f]+|\d+)", op):
            return int(op, 0) & 0xFFFFFFFF
        raise SystemExit("operand? " + op)

    k = 0
    while k < len(ins):
        i = ins[k]
        k += 1
        m, ops = i.mnemonic, i.op_str
        parts = [x.strip() for x in ops.split(",")]
        at = "0x%08X" % i.address
        if m == "push":
            if ops in ("ebp",) or (ops in ("ebx", "esi", "edi") and len(body) == 0 and saved < 4):
                saved += 1
                continue
            v = val(ops)
            stack.append(("slot", None) if v is None else v)
        elif m == "pop" or m == "ret" or (m == "mov" and ops in ("ebp, esp", "esp, ebp")) or (m == "sub" and parts[0] == "esp"):
            continue
        elif m == "xor" and parts[0] == parts[1]:
            reg[parts[0]] = 0
        elif m == "lea":
            mm = re.fullmatch(r"\[ebp - (0x[0-9a-f]+|\d+)\]", parts[1])
            if not mm:
                raise SystemExit(f"{at}: lea {ops}")
            reg[parts[0]] = ("frame", frame - int(mm.group(1), 0))
        elif m == "mov":
            dst, src = parts
            mm = re.fullmatch(r"(dword|byte) ptr \[ebp - (0x[0-9a-f]+|\d+)\]", dst)
            me = re.fullmatch(r"dword ptr \[(e[a-ds][xi])( \+ (0x[0-9a-f]+|\d+))?\]", dst)
            if mm:
                v = val(src)
                if v is None or isinstance(v, (tuple, str)):
                    raise SystemExit(f"{at}: store of {v!r}")
                off = frame - int(mm.group(2), 0)
                if mm.group(1) == "byte":
                    body.append("    CSF_AT(uint8_t, frame, 0x%02X) = %d;" % (off, v & 0xFF))
                elif is_addr(v):
                    a = addr(v)
                    body.append("    CSF_AT(%s, frame, 0x%02X) = %s;" % (
                        "const char*" if a.startswith("CSF_STR") else "uintptr_t", off, a))
                else:
                    body.append("    CSF_AT(uint32_t, frame, 0x%02X) = %s;" % (off, as_type(v, "uint32_t")))
            elif me and isinstance(reg[me.group(1)], tuple) and reg[me.group(1)][0] == "esp":
                # a 4-byte argument built in place: `push ecx; mov eax, esp; ...; mov [eax], K`
                idx = reg[me.group(1)][1]
                stack[idx] = val(src)
            elif me and reg[me.group(1)] == "this" or (me and me.group(1) == "esi"):
                body.append("    CSF_AT(uint32_t, self, 0x%X) = %s;" % (int(me.group(3) or "0", 0), as_type(val(src), "uint32_t")))
            elif dst == "eax" and src == "esp":
                reg["eax"] = ("esp", len(stack) - 1)
            elif dst in reg and src == "esi":
                reg[dst] = "this"
            elif dst == "esi" and src == "ecx":
                pass  # esi = this for the whole function
            elif dst in reg and re.fullmatch(r"dword ptr \[esi \+ (0x[0-9a-f]+|\d+)\]", src):
                break  # the tail: sort the entries (written by hand below)
            elif dst in reg and (src in reg or re.fullmatch(r"-?(0x[0-9a-f]+|\d+)", src)):
                reg[dst] = val(src)
            else:
                raise SystemExit(f"{at}: mov {ops}")
        elif m == "call":
            t = int(ops, 16)
            c = callee[t]
            if len(stack) < c["n"]:
                raise SystemExit(f"{at}: call {c['name']} with {len(stack)} on the stack")
            args = [stack.pop() for _ in range(c["n"])]
            # Registration argument 4 is a command name, even when IDA treated
            # its three letters as an offset (VAR\0 == 0x00524156, ANM\0 ==
            # 0x004D4E41). Emit CSF_STR, not a relocated data-image address:
            # datagen cannot distinguish these isolated words from pointers.
            if t not in (0x769FC0, SORT):
                name_addr = args[3]
                raw = packet._img[name_addr - 0x400000:].split(b'\0', 1)[0]
                assert raw and all(32 <= b < 127 for b in raw), (at, name_addr)
                strings[name_addr] = raw.decode('ascii')
            if any(isinstance(a, tuple) and a[0] == "slot" for a in args):
                raise SystemExit(f"{at}: unfilled stack slot in {args}")
            if c["conv"] == "__fastcall" and reg["ecx"] != "this":
                raise SystemExit(f"{at}: ecx is {reg['ecx']!r}")
            body.append("    %s(%s);" % (c["name"], ", ".join(
                (["self", "nullptr"] if c["conv"] == "__fastcall" else []) +
                [as_type(a, ty) for a, ty in zip(args, c["types"])])))
            table.append((c["name"], args))
            reg["eax"] = reg["ecx"] = reg["edx"] = None
        else:
            raise SystemExit(f"{at}: {m} {ops}")
    if stack:
        raise SystemExit(f"stack not empty before the tail: {stack}")

    decls = [c["decl"] for t, c in sorted(callee.items()) if t != SORT]
    src = f"""// GENERATED by tools/gen_script_catalog.py from the disassembly of 0x{EA:08X}: do not edit, regenerate.
// The constructor of the mission-script catalog: reserves 450 entries, registers every script type and command
// (id, kind, name, parameter descriptors) and sorts the entries. One statement per store / call of the original,
// in its order; `frame` is the original's 0x{frame:X} bytes of locals (descriptors of {{int type; const char* label;
// uint8_t flag}} that several consecutive calls share).
#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

typedef unsigned char (__cdecl *SortPredicate)(int, int);

{chr(10).join(decls)}
CSF_FUNC(int, __cdecl, {callee[SORT]['name']}, 0x{SORT:08X}, (int* first, int* last, int depth, SortPredicate pred));

static S_0076a140* __fastcall MissionScriptCatalog_Construct(S_0076a140* self, void* /*edx*/)
{{
    alignas(4) uint8_t frame[0x{frame:X}];

{chr(10).join(body)}

    int* const first = CSF_AT(int*, self, 0x4);
    int* const last = CSF_AT(int*, self, 0x8);
    {callee[SORT]['name']}(first, last, static_cast<int>(last - first),
        reinterpret_cast<SortPredicate>(CSF_ADDR_FN(0x{PRED:08X})));
    return self;
}}

HOOK(0x{EA:08X}, MissionScriptCatalog_Construct, "auto");
"""
    open(out, "w").write(src)
    print(f"{out}: {len(table)} registration calls, {len(body)} statements")
    if "--table" in sys.argv:
        with open(sys.argv[sys.argv.index("--table") + 1], "w") as f:
            for name, args in table:
                f.write(name + " " + " ".join(
                    (repr(strings[a]) if a in strings else "0x%X" % a) if isinstance(a, int) else "&frame+0x%X" % a[1]
                    for a in args) + "\n")


if __name__ == "__main__":
    main()
