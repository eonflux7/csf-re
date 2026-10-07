#!/usr/bin/env python3
"""Recover class skeletons (no RTTI) from the x86 code. Writes db/classes.json.

class id = primary vtable address (hex). For each class:
  ctors      functions that store this vtable to [this+0] (last vtable store wins = the class itself)
  base       vtable stored earlier in the same ctor, or primary vtable of a ctor it calls on `this`
  size       from allocation sites: push imm / call operator new / ... mov ecx,eax / call ctor
  methods    vtable slots + non-virtual functions called with ecx=this from the class's own methods
  fields     {offset: {size, r, w}} aggregated from [this+disp] accesses in methods (linear this-tracking)
"""
import collections, json, os, struct, sys
import pefile
from capstone import Cs, CS_ARCH_X86, CS_MODE_32
from capstone.x86 import X86_OP_REG, X86_OP_MEM, X86_OP_IMM, X86_REG_ECX, X86_REG_EAX, X86_REG_ESP, X86_REG_EBP

ROOT = os.path.dirname(os.path.dirname(os.path.abspath(__file__)))
OP_NEW = {0x6910F0}
FLOAT_OPS = {"fld", "fst", "fstp", "fadd", "fsub", "fsubr", "fmul", "fdiv", "fdivr", "fcom", "fcomp",
             "movss", "addss", "subss", "mulss", "divss", "comiss", "ucomiss"}
pe = pefile.PE(os.path.join(ROOT, "work", "pc", "CommXPC.exe"), fast_load=True)
base = pe.OPTIONAL_HEADER.ImageBase
img = pe.get_memory_mapped_image()
md = Cs(CS_ARCH_X86, CS_MODE_32)
md.detail = True

F = {}
for l in open(os.path.join(ROOT, "export", "functions.jsonl")):
    f = json.loads(l)
    F[f["ea"]] = f
VT = {int(v["addr"], 16): v for v in json.load(open(os.path.join(ROOT, "db", "vtables.json")))}
_cache = {}


def insns(ea):
    if ea not in _cache:
        f = F[ea]
        _cache[ea] = list(md.disasm(img[ea - base: ea - base + f["size"]], ea))
    return _cache[ea]


def this_scan(ea):
    """Linear scan tracking registers that hold `this` (ecx at entry).
    Returns (vtable stores at offset 0 in order, [(off,size,rw)], [callees called with ecx=this], [(vt, off)] secondary vtables)."""
    regs = {X86_REG_ECX}
    vt_stores, fields, this_calls, sec = [], [], [], []
    for ins in insns(ea):
        ops = ins.operands
        m = ins.mnemonic
        # memory accesses relative to this
        for k, op in enumerate(ops):
            if op.type == X86_OP_MEM and op.mem.base in regs and op.mem.index == 0:
                write = (k == 0 and m.startswith(("mov", "add", "sub", "inc", "dec", "and", "or", "xor", "set"))) \
                    or m in ("fst", "fstp", "fistp", "fist")
                off = op.mem.disp
                if m == "lea":
                    continue
                if write and m == "mov" and len(ops) == 2 and ops[1].type == X86_OP_IMM and ops[1].imm in VT:
                    (vt_stores if off == 0 else sec).append((ops[1].imm, off) if off else ops[1].imm)
                    continue
                if 0 <= off < 0x10000:
                    kind = "f" if m in FLOAT_OPS else ""
                    fields.append((off, op.size, ("w" if write else "r") + kind))
        if m == "call":
            if ops and ops[0].type == X86_OP_IMM and X86_REG_ECX in regs and ops[0].imm in F:
                this_calls.append(ops[0].imm)
            regs.discard(X86_REG_EAX)
            regs.discard(X86_REG_ECX)  # ecx is caller-saved
            regs.discard(0x15)  # edx
            continue
        # register propagation
        if m == "mov" and len(ops) == 2 and ops[0].type == X86_OP_REG:
            dst = ops[0].reg
            if ops[1].type == X86_OP_REG and ops[1].reg in regs:
                regs.add(dst)
            else:
                regs.discard(dst)
        elif len(ops) >= 1 and ops[0].type == X86_OP_REG and m not in ("cmp", "test", "push"):
            if m in ("pop",) or m not in ("cmp", "test"):
                regs.discard(ops[0].reg)
        if m == "ret":
            break
    return vt_stores, fields, this_calls, sec


def alloc_sites():
    """Yield (size, ctor_ea) for `push imm; call new; ... mov ecx,eax; call ctor` sequences."""
    out = collections.defaultdict(set)
    for ea, f in F.items():
        if OP_NEW.isdisjoint(f["callees"]):
            continue
        ins = insns(ea)
        for i, x in enumerate(ins):
            if x.mnemonic == "call" and x.operands and x.operands[0].type == X86_OP_IMM and x.operands[0].imm in OP_NEW:
                size = None
                for j in range(i - 1, max(i - 4, -1), -1):
                    p = ins[j]
                    if p.mnemonic == "push" and p.operands[0].type == X86_OP_IMM:
                        size = p.operands[0].imm
                        break
                if size is None:
                    continue
                ecx_new = False
                for y in ins[i + 1: i + 14]:
                    if y.mnemonic == "mov" and len(y.operands) == 2 and y.operands[0].type == X86_OP_REG \
                            and y.operands[0].reg == X86_REG_ECX and y.operands[1].type == X86_OP_REG \
                            and y.operands[1].reg == X86_REG_EAX:
                        ecx_new = True
                    elif y.mnemonic == "call":
                        if ecx_new and y.operands[0].type == X86_OP_IMM:
                            out[y.operands[0].imm].add(size)
                        break
    return out


def main():
    scans = {}
    for ea, f in F.items():
        if f["lib"]:
            continue
        refs_vt = [d for d in f["drefs"] if d in VT]
        if refs_vt or True:
            pass
    # ctors: functions that store a vtable at [this]
    ctor_of = {}           # function -> class vtable (last store)
    stored = {}            # function -> list of vtable stores
    for ea, f in F.items():
        if not any(d in VT for d in f["drefs"]):
            continue
        vs, fl, tc, sec = this_scan(ea)
        if vs:
            stored[ea] = vs
            scans[ea] = (vs, fl, tc, sec)
    classes = {}

    def cls(vt):
        return classes.setdefault(vt, dict(vtable=f"{vt:08x}", ctors=[], dtors=[], base=None, size=None, sizes=[],
                                           methods=set(), fields=collections.defaultdict(lambda: [0, "", 0])))

    slot_owner = collections.defaultdict(set)
    for vt, v in VT.items():
        for s in v["slots"]:
            slot_owner[int(s, 16)].add(vt)
    def called_from_slot0(ea, vt):
        """ea is the (deleting) dtor in slot 0 of vt, or the dtor body it calls."""
        s0 = VT[vt]["slots"][:1]
        return bool(s0) and (int(s0[0], 16) == ea or ea in F[int(s0[0], 16)]["callees"])

    dtor_base = {}
    for ea, vs in stored.items():
        last = vs[-1]
        # derived dtor with the base dtor inlined: stores [derived, ..., base] and is reached from the derived
        # deleting-dtor slot. It belongs to the derived class (else the base absorbs every derived field).
        # A derived vtable has at least as many slots as its base, so fewer slots in the last store means a dtor.
        if len(set(vs)) > 1 and (called_from_slot0(ea, vs[0]) or VT[vs[0]]["nslots"] > VT[last]["nslots"]):
            d = cls(vs[0])
            d["dtors"].append(ea)
            dtor_base.setdefault(vs[0], next(v for v in vs if v != vs[0]))  # fallback, may skip a level
            ctor_of[ea] = vs[0]
            continue
        c = cls(last)
        # dtor heuristic: the vtable store comes first and the function is reached from a deleting-dtor slot
        is_dtor = called_from_slot0(ea, last)
        (c["dtors"] if is_dtor else c["ctors"]).append(ea)
        ctor_of[ea] = last
        if len(set(vs)) > 1 and not is_dtor:
            prev = [v for v in vs if v != last]
            c["base"] = c["base"] or prev[-1]  # nearest earlier store: the direct base when base ctors are inlined
    # base via calling another ctor (or, from a dtor, the base dtor) on this
    # ctors first: a dtor may call a base dtor that restores a further ancestor's vtable
    for ea, (vs, fl, tc, sec) in sorted(scans.items(), key=lambda kv: kv[0] in classes[ctor_of[kv[0]]]["dtors"]):
        c = classes[ctor_of[ea]]
        if c["base"] is None and (ea in c["ctors"] or ea in c["dtors"]):
            for t in tc:
                if t in ctor_of and ctor_of[t] != ctor_of[ea]:
                    c["base"] = ctor_of[t]
                    break
    for vt, b in dtor_base.items():
        classes[vt]["base"] = classes[vt]["base"] or b
    # break base cycles (A.base=B, B.base=A): a dtor with an inlined base dtor stores [derived, base] and, when the
    # dtor heuristic misses it, reads as a ctor of the base whose "base" is the derived class. In each cycle the
    # vtable with the fewest slots is the real base: drop its base link.
    for vt in list(classes):
        path, x = [], vt
        while x is not None and x in classes and x not in path:
            path.append(x)
            x = classes[x]["base"]
        if x is not None and x in path:
            cyc = path[path.index(x):]
            root = min(cyc, key=lambda v: (len(VT[v]["slots"]), v))
            classes[root]["base"] = None
    # sizes
    for ctor, sizes in alloc_sites().items():
        if ctor in ctor_of:
            c = classes[ctor_of[ctor]]
            c["sizes"] = sorted(set(c["sizes"]) | sizes)
    for c in classes.values():
        c["size"] = c["sizes"][0] if len(c["sizes"]) == 1 else None
    # methods + fields
    for vt, c in classes.items():
        c["methods"] |= {int(s, 16) for s in VT[vt]["slots"]}
        c["methods"] |= set(c["ctors"]) | set(c["dtors"])
        todo = list(c["methods"])
        seen = set(todo)
        while todo:
            m = todo.pop()
            if m not in F or F[m]["lib"]:
                continue
            vs, fl, tc, sec = this_scan(m)
            for off, sz, rw in fl:
                e = c["fields"][off]
                e[0] = max(e[0], sz)
                for ch in rw:
                    if ch not in e[1]:
                        e[1] += ch
                e[2] += 1
            for t in tc:
                # non-virtual method: called on this, not a ctor of another class, not a slot of an unrelated vtable
                if t not in seen and t not in ctor_of and (not slot_owner[t] or vt in slot_owner[t]):
                    seen.add(t)
                    c["methods"].add(t)
                    todo.append(t)
    out = []
    for vt, c in sorted(classes.items()):
        out.append(dict(vtable=c["vtable"], base=f"{c['base']:08x}" if c["base"] else None, size=c["size"],
                        sizes=c["sizes"], ctors=[f"{x:08x}" for x in sorted(c["ctors"])],
                        dtors=[f"{x:08x}" for x in sorted(c["dtors"])],
                        methods=[f"{x:08x}" for x in sorted(c["methods"])],
                        fields={f"{o:04x}": dict(size=s, rw=rw, n=n) for o, (s, rw, n) in sorted(c["fields"].items())}))
    json.dump(out, open(os.path.join(ROOT, "db", "classes.json"), "w"), indent=1)
    n_size = sum(1 for c in out if c["size"])
    n_base = sum(1 for c in out if c["base"])
    n_meth = sum(len(c["methods"]) for c in out)
    print(f"classes {len(out)}  with size {n_size}  with base {n_base}  methods {n_meth}  "
          f"multi-size {sum(1 for c in out if len(c['sizes']) > 1)}")


if __name__ == "__main__":
    main()
