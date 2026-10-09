#!/usr/bin/env python3
"""Which of Hex-Rays' register arguments in ebx/esi/edi are real inputs (exact, from the binary).

Hex-Rays gives MSVC functions `a1@<edi>` (or esi/ebx) when edi is pushed before it is written: usually that push is
only the callee-save of a shrink-wrapped prologue (`push edi` after an early-exit test, `pop edi` before `ret`), and
the phantom argument then spreads to every caller whose edi reaches the call untouched (0x42F4F0: 22 callers push 2
stack args, Hex-Rays gave it 3; grunts bridged the phantom with __regcall / naked asm and passed garbage).

R is an input of f iff some path from the entry reads R before writing it, where
  - `push R` is a save when f also has a `pop R` (callee-save), else a read (an argument push);
  - a direct call / tail jump to g reads R iff R is an input of g (recursively; cycles assume no); other calls keep
    ebx/esi/edi (callee-saved in every MSVC convention);
  - an indirect call / tail jump (virtual, function pointer) reaches an address-taken function, which MSVC compiles
    to a standard convention: it reads no ebx/esi/edi; switch tables are followed;
  - anything not understood (unknown jump table, budget, function not in the export) counts as a read: "spurious"
    is only claimed when proven.

Usage: regargs.py report [--done]     Hex-Rays register args (ebx/esi/edi): real vs spurious; --done: only
                                      done functions (their impls take no register argument)
       regargs.py write               db/regargs.json {ea: {"spurious": [regs], "real": [regs]}} (packet, api)
"""
import json, os, re, sys
sys.path.insert(0, os.path.dirname(__file__))

ROOT = os.path.dirname(os.path.dirname(os.path.abspath(__file__)))
OUT = os.path.join(ROOT, "db", "regargs.json")
REGS = ("ebx", "esi", "edi")
SUB = {"ebx": r"\b(ebx|bx|bl|bh)\b", "esi": r"\b(esi|si)\b", "edi": r"\b(edi|di)\b"}
_memo, _ins = {}, {}


def _code(db, ea):
    if ea not in _ins:
        from packet import disasm
        ins = disasm(ea, db.F[ea]["size"]) if ea in db.F else []
        _ins[ea] = (ins, {i.address: k for k, i in enumerate(ins)})
    return _ins[ea]


def _jump_table(addr, idx, limit=256):
    """Instruction indexes of a switch table's targets (dwords at addr while they land on this function's code)."""
    import semcheck
    out = []
    for k in range(limit):
        t = semcheck.image_dword(addr + 4 * k)
        if t is None or t not in idx:
            break
        out.append(idx[t])
    return out


def is_input(db, ea, reg, depth=0):
    key = (ea, reg)
    if key in _memo:
        return _memo[key]
    if ea not in db.F or depth > 12:
        return True
    if db.rows.get(ea, {}).get("region", "game") != "game":
        return False  # library code (CRT, RenderWare, ...): standard conventions only
    _memo[key] = False  # cycle: assume no (a recursive path alone doesn't make R an input)
    ins, idx = _code(db, ea)
    if not ins:
        _memo[key] = True
        return True
    pat = re.compile(SUB[reg])
    saves = any(i.mnemonic == "pop" and i.op_str == reg for i in ins)
    res, seen, work, budget = False, set(), [0], 3000
    while work and not res:
        k = work.pop()
        while k < len(ins) and k not in seen:
            seen.add(k)
            budget -= 1
            if budget < 0:
                res = True
                break
            i = ins[k]
            m, ops = i.mnemonic, i.op_str
            dst, _, src = (x.strip() for x in ops.partition(","))
            if m == "push" and ops == reg:
                if not saves:
                    res = True
                    break
                k += 1
                continue
            if m == "pop" and ops == reg:
                break  # restored: whatever follows sees the caller's value again only via the stack, not a read
            if m == "call":
                if ops.startswith("0x"):
                    t = int(ops, 16)
                    if t in db.F and not db.F[t].get("lib") and is_input(db, t, reg, depth + 1):
                        res = True
                        break
                elif pat.search(ops):
                    res = True
                    break
                k += 1
                continue
            if m == "ret":
                break
            if m.startswith("j"):
                if not ops.startswith("0x"):
                    tbl = re.fullmatch(r"dword ptr \[e[a-z]{2}\*4 \+ (0x[0-9a-f]+)\]", ops)
                    targets = _jump_table(int(tbl.group(1), 16), idx) if tbl and m == "jmp" else None
                    if pat.search(ops):
                        res = True
                        break
                    if not targets:
                        # indirect tail call (virtual / function pointer): an address-taken target has the standard
                        # convention, so it doesn't take ebx/esi/edi; an unknown jump table counts as a read
                        res = bool(tbl)
                        break
                    if pat.search(ops):
                        res = True
                        break
                    work += targets
                    break
                t = int(ops, 16)
                if t in idx:
                    work.append(idx[t])
                    if m == "jmp":
                        break
                    k += 1
                    continue
                # tail jump out of the function
                if t not in db.F or is_input(db, t, reg, depth + 1):
                    res = True
                break
            reads = pat.search(src) or (pat.search(dst) and (m not in ("mov", "lea", "movzx", "movsx", "pop",
                                                                        "setne", "sete") or "[" in dst))
            if m == "lea" and src == f"[{dst}]":
                k += 1
                continue  # `lea ebx, [ebx]`: alignment padding
            if m in ("xor", "sub") and dst == src and re.fullmatch(SUB[reg], dst) \
                    or m == "or" and dst == reg and src in ("0xffffffff", "-1"):
                break  # zeroing / all-ones: a write
            if reads or m in ("lodsd", "lodsb", "stosd", "stosb", "movsd", "movsb", "rep movsd", "rep stosd",
                              "cmpsb", "scasb") and reg in ("esi", "edi"):
                res = True
                break
            if re.fullmatch(SUB[reg], dst) and m in ("mov", "lea", "movzx", "movsx", "pop"):
                break  # written before any read on this path (a sub-register write: what follows reads only it)
            k += 1
    _memo[key] = res
    return res


def hexrays_regs(ea):
    import api
    a = api.ida_abi().get(ea) or {}
    return sorted({loc for _, _, loc in a.get("args", []) if loc in REGS})


def analyze(db, eas=None):
    import api
    out = {}
    for ea in sorted(eas if eas is not None else api.ida_abi()):
        if ea not in db.F or db.F[ea].get("lib"):
            continue
        hr = hexrays_regs(ea)
        if not hr:
            continue
        real = [r for r in hr if is_input(db, ea, r)]
        out[ea] = dict(spurious=[r for r in hr if r not in real], real=real)
    return out


def main():
    from packet import DB
    db = DB()
    cmd = sys.argv[1] if len(sys.argv) > 1 else "report"
    res = analyze(db)
    if cmd == "write":
        json.dump({f"{ea:08x}": v for ea, v in sorted(res.items())}, open(OUT, "w"), indent=0)
        print(f"{OUT}: {len(res)} functions with Hex-Rays ebx/esi/edi args")
    done = {ea for ea, r in db.rows.items() if r["status"] in ("done", "verified")}
    rows = [(ea, v) for ea, v in res.items() if "--done" not in sys.argv or ea in done]
    n_sp = sum(1 for _, v in rows if v["spurious"] and not v["real"])
    n_real = sum(1 for _, v in rows if v["real"])
    print(f"{len(rows)} functions with Hex-Rays ebx/esi/edi args: {n_sp} all spurious, {n_real} with a real one")
    if "--done" in sys.argv:
        for ea, v in rows:
            if v["real"]:
                print(f"  REAL {ea:08x} {db.name(ea) or ''}: {','.join(v['real'])}")


if __name__ == "__main__":
    main()
