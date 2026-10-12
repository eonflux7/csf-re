#!/usr/bin/env python3
"""Does a game function return in st0? Decided from the binary: the x87 stack depth at every `ret` (db/fpret.json).

Hex-Rays' prototype is a guess here: it typed 0x40FC50 `long double` although no path leaves anything on the x87
stack; the gate then made the reimplementation return float, and every call left one x87 entry behind (the caller
never pops it). After a few calls the original's own float code overflows the stack and computes NaN
(0x401FA0: -nan(ind) for a zero vector, 2026-10-12). api.returns_fpu reads this file first.

Rules: nothing is live on the x87 stack across a call (MSVC), so every call starts from depth 0; a callee's result
is its own decided depth, else the old fact (Hex-Rays / last instruction); an indirect call's result is decided by
what follows it (a pop or use on an empty stack = it returned in st0; an expression that completes without it =
it didn't). A path that ends right after an undecided call is decided by the function's other paths. Functions
whose paths disagree, or with no decided path, are left out.

Usage: tools/fpret.py [--check]   (--check: list where the decided fact differs from Hex-Rays, write nothing)
"""
import json, os, re, sys
ROOT = os.path.dirname(os.path.dirname(os.path.abspath(__file__)))
sys.path.insert(0, os.path.join(ROOT, "tools"))
import api
from packet import DB, disasm
OUT = os.path.join(ROOT, "db", "fpret.json")
db = DB()
abi = api.ida_abi()
PUSH = re.compile(r'f(i?ld|ld1|ldz|ldpi|ldl2e|ldl2t|ldlg2|ldln2|ptan|sincos|xtract)$')
POP1 = re.compile(r'f(i?stp|isttp|addp|subp|subrp|mulp|divp|divrp|i?comp|ucomp|comip|ucomip|patan|yl2x|yl2xp1)$')
POP2 = re.compile(r'f(compp|ucompp)$')
USE = re.compile(r'f(st|ist|add|sub|subr|mul|div|divr|iadd|isub|isubr|imul|idiv|idivr|chs|abs|sqrt|sin|cos|xch|com|icom|ucom|comi|ucomi|tst|xam|rndint|scale|prem|prem1)$')
HELPER = re.compile(r'__?(ftol|CI)')
def hexrays_fp(ea):
    a = abi.get(ea)
    return bool(a and api.FLOAT_RET.match(api.c_type(a.get('ret') or '') or ''))
memo = {}
def sim(ea):
    """-> (set of (depth, ambiguous) at ret, ok)"""
    if ea in memo: return memo[ea]
    memo[ea] = (None, False)
    f = db.F[ea]
    ins = disasm(ea, f['size'])
    idx = {i.address: k for k, i in enumerate(ins)}
    seen, work, rets, ok = {}, [(0, 0, False)], set(), True
    def callee(t, d, amb):
        t = api.thunk_target(t, db)
        if t not in db.F: return None
        n = db.F[t].get('name') or ''
        if db.F[t].get('lib'):
            if re.match(r'__?ftol', n): return d - 1, amb
            if re.match(r'__?CI(fmod|pow|atan2)', n): return d - 1, amb
            if re.match(r'__?CI', n): return d, amb
            return (1 if hexrays_fp(t) else 0), False
        r, c = sim(t)
        depths = {x for x, a in r or () if x > 0 or not a}
        if not c or len(depths) != 1:
            return (1 if api.returns_fpu(t, db, binary=False) else 0), False   # undecided callee: the old fact
        return depths.pop(), False
    while work:
        k, d, amb = work.pop()
        while 0 <= k < len(ins):
            if k in seen:
                if seen[k][0] != d: ok = False
                break
            seen[k] = (d, amb)
            i = ins[k]; m = i.mnemonic; op = i.op_str
            need = 1 if (USE.match(m) or POP1.match(m)) else 2 if POP2.match(m) else 0
            if m in ('faddp','fsubp','fsubrp','fmulp','fdivp','fdivrp','fcompp','fucompp','fpatan','fyl2x','fprem','fscale') : need = 2
            if need > d and amb and need - d == 1: d += 1; amb = False
            elif need > d: ok = False; d = need
            d0 = d
            if PUSH.match(m): d += 1
            elif POP2.match(m): d -= 2
            elif POP1.match(m): d -= 1
            if d == 0 and d0 > 0: amb = False   # an expression completed without the call's result
            elif m in ('fninit', 'finit', 'emms'): d = 0; amb = False
            elif m == 'call':
                if op.startswith('0x'):
                    r = callee(int(op, 16), d, amb)
                    if r is None: d, amb = 0, True      # unknown direct callee: decided by what follows
                    else: d, amb = r
                else: d, amb = 0, True
            if m in ('ret', 'retn'):
                rets.add((d, amb)); break
            if m == 'jmp':
                if op.startswith('0x') and int(op, 16) in idx: k = idx[int(op, 16)]; continue
                if op.startswith('0x'):
                    r = callee(int(op, 16), d, amb)
                    rets.add(r if r is not None else (0, True))
                elif re.match(r'dword ptr \[.*\*4', op): ok = False   # switch: targets not followed
                else: rets.add((0, True))                              # indirect tail call
                break
            if m.startswith('j') and op.startswith('0x') and int(op, 16) in idx:
                work.append((idx[int(op, 16)], d, amb))
            k += 1
    memo[ea] = (rets, ok)
    return memo[ea]


def main():
    sys.setrecursionlimit(50000)
    res, diff = {}, []
    for ea, f in sorted(db.F.items()):
        if f.get("lib"):
            continue
        r, ok = sim(ea)
        # a path that ends right after an undecided call says nothing by itself: the function's other paths decide
        depths = {d for d, a in r or () if d > 0 or not a}
        if not ok or len(depths) != 1 or max(depths) > 1:
            continue
        fp = depths.pop()
        res[f"{ea:08X}"] = fp
        if bool(fp) != bool(api.returns_fpu(ea, db, binary=False)):
            diff.append(f"  0x{ea:08X} binary: {'st0' if fp else 'nothing in st0'}; Hex-Rays says "
                        f"{'float' if hexrays_fp(ea) else 'not float'}  {f.get('name') or ''}")
    print(f"fpret: {len(res)} functions decided, {sum(res.values())} return in st0; {len(diff)} differ from the old fact")
    print("\n".join(diff))
    if "--check" not in sys.argv:
        with open(OUT, "w") as f:
            json.dump(res, f, indent=0, sort_keys=True)
            f.write("\n")


if __name__ == "__main__":
    main()
