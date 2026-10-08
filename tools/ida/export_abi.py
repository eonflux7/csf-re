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
  arg0_src  {callee ea: [["p", i] = the caller's i-th parameter | ["f", struct, offset] = a struct member]}:
            what the first argument is made of, so a callee's class types that parameter / member (this_class.py)

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
        self.args = list(cf.argidx)

    def arg0_source(self, x):
        """Where the first argument comes from, when that can type it: ["p", i] the caller's i-th parameter,
        ["f", struct, offset] a member of a typed struct (x->m / x.m). Casts are looked through."""
        while x.op == ida_hexrays.cot_cast:
            x = x.x
        if x.op == ida_hexrays.cot_var and x.v.idx in self.args:
            return ["p", self.args.index(x.v.idx)]
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

    def visit_expr(self, e):
        if e.op == ida_hexrays.cot_call and e.x.op == ida_hexrays.cot_obj:
            k = f"{e.x.obj_ea:08x}"
            self.out.setdefault(k, []).append(len(e.a))
            if len(e.a):  # the first argument's type (thiscall: the object in ecx), for this_class.py
                t = e.a[0].type
                if t.is_ptr() and t.get_pointed_object().is_struct():
                    self.objs.setdefault(k, []).append(t.get_pointed_object().get_type_name() or "")
                src = self.arg0_source(e.a[0])
                if src and (src[0] == "p" or src[1]):
                    self.src.setdefault(k, []).append(src)
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
    rec["calls"] = v.out
    if v.objs:
        rec["this_args"] = v.objs
    if v.src:
        rec["arg0_src"] = v.src
    rec["cleanup"] = cleanups(ea)
    out[f"{ea:08x}"] = rec
    if i % 2000 == 0:
        print(f"{i}/{len(targets)} {time.time() - t0:.0f}s", flush=True)
json.dump(out, open(OUT, "w"), separators=(",", ":"))
print(f"done {len(out)} functions, {fails} failed, {time.time() - t0:.0f}s")
close_db(save=False)
