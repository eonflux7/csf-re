"""Hex-Rays' view of every function's ABI -> export/abi.json (read-only on the IDB).

Per function, from the decompiler's own type object (not the pseudocode text):
  cc        cdecl / stdcall / thiscall / fastcall / usercall / userpurge / ellipsis (variadic cdecl)
  ret       return type
  args      [[type, name, location]]  location = register name (ecx, edx, eax, ...) or "stack"
  stk       stack bytes the arguments take
Per call site, from each caller's ctree (Hex-Rays counts what the caller actually pushes, including arguments
the callee never reads, which its own prototype drops):
  calls     {callee ea: [argument count at each call site]}
  cleanup   {callee ea: [stack bytes removed right after each call: add esp, N / pop ecx; 0 = none there]}
  this_args {callee ea: [struct name the first argument points to, per call site]} (typed callers only)
  arg0_src  {callee ea: [["p", i] = the caller's i-th parameter | ["f", struct, offset] = a struct member |
            ["g", global, "inst"|"ptr"] = a global (the object / a pointer to it) | ["r", callee] = a call's result |
            ["l", stack offset, room] = an object on the caller's stack]}:
            what the first argument is made of, so a callee's class types that parameter / member (this_class.py)
  order     per block, the direct callees of its call statements in order; consts: the numbers it computes with
            (literals, case values, float literals / .rdata floats; not pointer offsets): semcheck.behaviour

Usage: export_abi.py [--only ea,ea,...]     (default: every function in db/functions.csv; ~2 min)
"""
import os, sys, csv, json, time
sys.path.insert(0, os.path.dirname(__file__))
from common import ROOT, open_db, close_db
import ida_hexrays, ida_typeinf, ida_idp, ida_funcs, ida_ua, idaapi, idautils, idc

CC = {ida_typeinf.CM_CC_CDECL: "cdecl", ida_typeinf.CM_CC_STDCALL: "stdcall",
      ida_typeinf.CM_CC_THISCALL: "thiscall", ida_typeinf.CM_CC_FASTCALL: "fastcall",
      ida_typeinf.CM_CC_SPECIAL: "usercall", ida_typeinf.CM_CC_SPECIALP: "userpurge",
      ida_typeinf.CM_CC_SPECIALE: "usercall", ida_typeinf.CM_CC_ELLIPSIS: "ellipsis",
      ida_typeinf.CM_CC_PASCAL: "pascal"}


def proto(tif):
    ftd = ida_typeinf.func_type_data_t()
    if not tif.get_func_details(ftd):
        return None
    args = []
    for a in ftd:
        loc = a.argloc
        if loc.is_reg1():
            where = ida_idp.get_reg_name(loc.reg1(), 4)
        elif loc.is_stkoff():
            where = "stack"
        else:
            where = "other"
        args.append([a.type.dstr(), a.name, where])
    cc = tif.get_cc()
    return dict(cc=CC.get(cc & ida_typeinf.CM_CC_MASK, f"cc{cc:x}"), ret=ftd.rettype.dstr(), args=args,
                stk=ftd.stkargs)


def cleanups(ea):
    """{callee: [stack bytes the caller removes right after each `call callee`]}: `add esp, N` = N, `pop ecx` = 4,
    anything else = 0 (no cleanup there, or a deferred one). IDA's decoded instructions, not a re-disassembly."""
    out = {}
    f = ida_funcs.get_func(ea)
    if not f:
        return out
    for x in idautils.FuncItems(ea):
        insn = ida_ua.insn_t()
        if not ida_ua.decode_insn(insn, x) or insn.itype not in (idaapi.NN_call,):
            continue
        tgt = idc.get_operand_value(x, 0)
        if insn.Op1.type not in (ida_ua.o_near, ida_ua.o_far) or not ida_funcs.get_func(tgt):
            continue
        nx = x + insn.size
        m = idc.print_insn_mnem(nx)
        if m == "add" and idc.print_operand(nx, 0) == "esp":
            n = idc.get_operand_value(nx, 1)
        elif m == "pop" and idc.print_operand(nx, 0) == "ecx":
            n = 4
        else:
            n = 0
        out.setdefault(f"{tgt:08x}", []).append(n)
    return out


class Calls(ida_hexrays.ctree_visitor_t):
    def __init__(self, cf):
        super().__init__(ida_hexrays.CV_FAST)
        self.out, self.objs, self.src = {}, {}, {}  # not self.this: SWIG keeps the C++ pointer there
        self.assigned = {}  # local idx -> [callee | None per assignment]
        self.lvars = cf.get_lvars()
        self.stk = sorted({v.location.stkoff() for v in self.lvars if v.is_stk_var()})
        self.args = list(cf.argidx)

    def arg0_source(self, x):
        """Where the first argument comes from, when that can type it: ["p", i] the caller's i-th parameter,
        ["f", struct, offset] a member of a typed struct (x->m / x.m), ["g", ea, "inst"|"ptr"] a global that is the
        object (&g) or holds a pointer to it (g), ["r", callee] what a call returned (directly, or through a local
        assigned once from that call: resolved in resolve()). Casts are looked through."""
        while x.op == ida_hexrays.cot_cast:
            x = x.x
        if x.op == ida_hexrays.cot_var and x.v.idx in self.args:
            return ["p", self.args.index(x.v.idx)]
        if x.op == ida_hexrays.cot_var:
            return ["v", x.v.idx]
        if x.op == ida_hexrays.cot_ref and x.x.op == ida_hexrays.cot_var and self.lvars[x.x.v.idx].is_stk_var():
            lv = self.lvars[x.x.v.idx]  # an object on the stack: ["l", stack offset, room up to the next variable]
            off = lv.location.stkoff()
            nxt = min((o for o in self.stk if o > off), default=None)
            return ["l", off, (nxt - off) if nxt is not None else 0]
        if x.op == ida_hexrays.cot_ref and x.x.op == ida_hexrays.cot_obj and not ida_funcs.get_func(x.x.obj_ea):
            return ["g", f"{x.x.obj_ea:08x}", "inst"]
        if x.op == ida_hexrays.cot_obj and not ida_funcs.get_func(x.obj_ea):
            return ["g", f"{x.obj_ea:08x}", "ptr"]
        if x.op == ida_hexrays.cot_call and x.x.op == ida_hexrays.cot_obj and ida_funcs.get_func(x.x.obj_ea):
            return ["r", f"{x.x.obj_ea:08x}"]
        if x.op == ida_hexrays.cot_memptr and x.x.type.is_ptr():
            return self.member(x.x.type.get_pointed_object(), x.m)
        if x.op == ida_hexrays.cot_memref:
            return self.member(x.x.type, x.m)
        return None

    @staticmethod
    def member(st, off):
        """["f", struct, offset, member name] of the 4-byte member at `off` declared in `st` itself."""
        udm = ida_typeinf.udm_t()
        udm.offset = off * 8
        if st.find_udm(udm, ida_typeinf.STRMEM_OFFSET) < 0 or udm.is_baseclass() or udm.size != 32:
            return None
        return ["f", st.get_type_name() or "", off, udm.name]

    def resolve(self):
        """["v", idx] -> ["r", callee] when the local is assigned exactly once, from a direct call; else dropped."""
        for k, srcs in list(self.src.items()):
            out = []
            for s in srcs:
                if s[0] != "v":
                    out.append(s)
                elif len(self.assigned.get(s[1], ())) == 1 and self.assigned[s[1]][0]:
                    out.append(["r", self.assigned[s[1]][0]])
            if out:
                self.src[k] = out
            else:
                del self.src[k]

    def visit_expr(self, e):
        if e.op == ida_hexrays.cot_asg and e.x.op == ida_hexrays.cot_var:
            y = e.y
            while y.op == ida_hexrays.cot_cast:
                y = y.x
            callee = (f"{y.x.obj_ea:08x}" if y.op == ida_hexrays.cot_call and y.x.op == ida_hexrays.cot_obj
                      and ida_funcs.get_func(y.x.obj_ea) else None)
            self.assigned.setdefault(e.x.v.idx, []).append(callee)
        if e.op == ida_hexrays.cot_call and e.x.op == ida_hexrays.cot_obj:
            k = f"{e.x.obj_ea:08x}"
            self.out.setdefault(k, []).append(len(e.a))
            if len(e.a):  # the first argument's type (thiscall: the object in ecx), for this_class.py
                t = e.a[0].type
                if t.is_ptr() and t.get_pointed_object().is_struct():
                    self.objs.setdefault(k, []).append(t.get_pointed_object().get_type_name() or "")
                src = self.arg0_source(e.a[0])
                if src and (src[0] in ("p", "v", "g", "r", "l") or src[1]):
                    self.src.setdefault(k, []).append(src)
        return 0


def callee_of(s):
    """Direct callee of a statement that is a call (`f(...);`, `x = f(...);`), else None."""
    if s.op != ida_hexrays.cit_expr:
        return None
    e = s.cexpr
    if e.op == ida_hexrays.cot_asg:
        e = e.y
    while e.op == ida_hexrays.cot_cast:
        e = e.x
    if e.op == ida_hexrays.cot_call and e.x.op == ida_hexrays.cot_obj and ida_funcs.get_func(e.x.obj_ea):
        return f"{e.x.obj_ea:08x}"
    return None


def s32(v, size):
    v &= (1 << (8 * size)) - 1 if size in (1, 2, 4, 8) else 0xFFFFFFFF
    return v - (1 << (8 * size)) if size in (1, 2, 4, 8) and v >> (8 * size - 1) else v


class Sem(ida_hexrays.ctree_visitor_t):
    """What the gate compares beyond references (semcheck.py):
      order   per block, the direct callees of its call statements in order (only blocks with >= 2 distinct):
              statements of one block run in that order, whatever branches surround them
      consts  numbers the code computes with: literals (not pointer offsets / array indices / addresses), switch
              case values, float literals and .rdata floats it loads"""

    def __init__(self):
        super().__init__(ida_hexrays.CV_PARENTS)
        self.order, self.consts = [], set()

    def visit_insn(self, i):
        if i.op == ida_hexrays.cit_block:
            seq = [c for c in map(callee_of, i.cblock) if c]
            if len(set(seq)) >= 2:
                self.order.append(seq)
        elif i.op == ida_hexrays.cit_switch:
            size = i.cswitch.expr.type.get_size()
            for case in i.cswitch.cases:
                for v in case.values:
                    self.consts.add(s32(v, size if size in (1, 2, 4, 8) else 4))
        return 0

    def is_offset(self, e):
        """Pointer arithmetic: `p + N` on a pointer, `(T *)(x + N)` on an untyped integer, or an array index."""
        n = len(self.parents)
        p = self.parents[n - 1].cexpr if n >= 1 and self.parents[n - 1].is_expr() else None
        if p is None:
            return False
        if p.op == ida_hexrays.cot_idx:
            return p.y == e
        k = n - 1  # up a chain of additions (`this + i + 14`): pointer-typed anywhere, or cast to a pointer
        while k >= 0 and self.parents[k].is_expr() and self.parents[k].cexpr.op in (ida_hexrays.cot_add,
                                                                                     ida_hexrays.cot_sub):
            if self.parents[k].cexpr.type.is_ptr():
                return True
            k -= 1
        if k == n - 1:
            return False
        g = self.parents[k].cexpr if k >= 0 and self.parents[k].is_expr() else None
        return g is not None and g.op == ida_hexrays.cot_cast and g.type.is_ptr()

    def visit_expr(self, e):
        if e.op == ida_hexrays.cot_num:
            if self.is_offset(e):
                return 0
            size = e.type.get_size()
            v = s32(e.numval(), size if size in (1, 2, 4, 8) else 4)
            if not 0x401000 <= v < 0x900000:
                self.consts.add(v)
        elif e.op == ida_hexrays.cot_fnum:
            import ida_lines
            txt = ida_lines.tag_remove(e.fpc._print()).rstrip("fFlL")
            try:
                self.consts.add(float(txt))
            except ValueError:
                pass
        elif e.op == ida_hexrays.cot_obj and e.type.is_floating() and idc.get_segm_name(e.obj_ea) == ".rdata":
            import struct
            n = e.type.get_size()
            if n in (4, 8):
                raw = idc.get_bytes(e.obj_ea, n)
                if raw:
                    self.consts.add(struct.unpack("<f" if n == 4 else "<d", raw)[0])
        return 0


args = sys.argv[1:]
rows = list(csv.DictReader(open(os.path.join(ROOT, "db", "functions.csv"))))
targets = ([int(x, 16) for x in args[args.index("--only") + 1].split(",")] if "--only" in args
           else [int(r["ea"], 16) for r in rows])
open_db()
assert ida_hexrays.init_hexrays_plugin()
OUT = os.path.join(ROOT, "export", "abi.json")
out = json.load(open(OUT)) if "--only" in args and os.path.exists(OUT) else {}  # --only updates in place
fails, t0 = 0, time.time()
for i, ea in enumerate(targets):
    try:
        cf = ida_hexrays.decompile(ea, flags=ida_hexrays.DECOMP_NO_WAIT | ida_hexrays.DECOMP_NO_CACHE)
    except ida_hexrays.DecompilationFailure:
        cf = None
    if not cf:
        fails += 1
        continue
    rec = proto(cf.type) or {}
    v = Calls(cf)
    v.apply_to(cf.body, None)
    v.resolve()
    rec["calls"] = v.out
    if v.objs:
        rec["this_args"] = v.objs
    if v.src:
        rec["arg0_src"] = v.src
    s = Sem()
    s.apply_to(cf.body, None)
    if s.order:
        rec["order"] = s.order
    if s.consts:
        rec["consts"] = sorted(s.consts, key=lambda c: (isinstance(c, float), c))
    rec["cleanup"] = cleanups(ea)
    out[f"{ea:08x}"] = rec
    if i % 2000 == 0:
        print(f"{i}/{len(targets)} {time.time() - t0:.0f}s", flush=True)
json.dump(out, open(OUT, "w"), separators=(",", ":"))
print(f"done {len(out)} functions, {fails} failed, {time.time() - t0:.0f}s")
close_db(save=False)
