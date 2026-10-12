#!/usr/bin/env python3
"""Build the minimal context packet for reimplementing one function.

Usage: packet.py <ea-hex>     (prints the packet)
"""
import csv, json, os, re, sys, threading
import pefile
from capstone import Cs, CS_ARCH_X86, CS_MODE_32

ROOT = os.path.dirname(os.path.dirname(os.path.abspath(__file__)))
_img = None


class DB:
    def __init__(self):
        self.rows = {int(r["ea"], 16): r for r in csv.DictReader(open(os.path.join(ROOT, "db", "functions.csv")))}
        self.F = {}
        for l in open(os.path.join(ROOT, "export", "functions.jsonl")):
            f = json.loads(l)
            self.F[f["ea"]] = f
        self.classes = {c["vtable"]: c for c in json.load(open(os.path.join(ROOT, "db", "classes.json")))}
        from gen_classes_h import load_names
        self.class_names = load_names()  # db/types/classes.yaml, conf >= 0.7
        self.owner = {}
        self._this = None  # db/types/this_class.json, loaded on first use
        for vt, c in self.classes.items():
            for m in c["methods"]:
                self.owner.setdefault(int(m, 16), []).append(vt)
        import yaml as _y
        mp = os.path.join(ROOT, "db", "modules.yaml")
        self.modules = {m["id"]: m for m in (_y.safe_load(open(mp)) or [])} if os.path.exists(mp) else {}
        self.globals = {}
        gp = os.path.join(ROOT, "db", "globals.yaml")
        if os.path.exists(gp):
            import yaml
            for g in yaml.safe_load(open(gp)) or []:
                self.globals[int(g["addr"])] = g

    def name(self, ea):
        r = self.rows.get(ea)
        return (r["name"] if r and r["name"] else None)

    def class_of(self, ea):
        """Most-base class among owners (implementation lives in the class that introduced it); else the class
        db/types/this_class.json gives it (constructors, destructors, non-virtual methods via Hex-Rays' typed
        call sites: tools/this_class.py)."""
        owners = self.owner.get(ea, [])
        if not owners:
            if self._this is None:
                import this_class
                self._this = this_class.load()
            c = self._this.get(ea)
            return c["vt"] if c and c["vt"] in self.classes else None
        def ancestors(vt):
            seen = []
            while vt and vt in self.classes and vt not in seen:
                seen.append(vt)
                vt = self.classes[vt]["base"]
            return seen
        for o in owners:
            if all(o in ancestors(p) for p in owners):
                return o
        return sorted(owners, key=lambda o: self.classes[o]["size"] or 1 << 30)[0]

    def cls_name(self, vt):
        if vt in self.class_names:
            return self.class_names[vt]["name"]
        return f"S_{vt}" if self.classes.get(vt, {}).get("plain") else f"C_{vt}"


_md = None
_init_lock = threading.Lock()


def ida_view(ea):
    """IDA's code view of a function (export/disasm/<ea>.json from tools/ida/export_disasm.py), or None."""
    p = os.path.join(ROOT, "export", "disasm", f"{ea:08x}.json")
    return json.load(open(p)) if os.path.exists(p) else None


def disasm(ea, size, linear=False):
    """Instructions of the function at ea. With an IDA export: exactly the code heads IDA knows (all chunks, jump
    tables and embedded data excluded), each decoded by capstone. Otherwise (or linear=True, e.g. for a shared
    block that isn't a function): linear decoding of `size` bytes."""
    global _img, _md
    if _md is None:
        with _init_lock:  # grunt runs this from worker threads: publish _md last, after _img is ready
            if _md is None:
                # the image once: pefile rebuilds it on every call (2 ms; api.py build asks 38k times)
                _img = pefile.PE(os.path.join(ROOT, "work", "pc", "CommXPC.exe"), fast_load=True).get_memory_mapped_image()
                _md = Cs(CS_ARCH_X86, CS_MODE_32)
    img = _img
    view = None if linear else ida_view(ea)
    if view:
        out = []
        for h in view["insns"]:
            ins = next(_md.disasm(img[h - 0x400000: h - 0x400000 + 16], h, 1), None)
            if ins:
                out.append(ins)
        return out
    return list(_md.disasm(img[ea - 0x400000: ea - 0x400000 + size], ea))


def ret_bytes(ea, size):
    """`ret N` bytes of the function's own returns. `mov eax, <address inside the function>; ret` is the end of a
    catch handler (it returns where execution continues to the EH runtime), not a return of the function."""
    ins, rets = disasm(ea, size), set()
    for k, i in enumerate(ins):
        if i.mnemonic != "ret":
            continue
        p = ins[k - 1] if k else None
        if p is not None and p.mnemonic == "mov" and re.fullmatch(r"eax, 0x[0-9a-f]+", p.op_str) \
                and ea <= int(p.op_str[5:], 16) < ea + size:
            continue
        rets.add(int(i.op_str, 16) if i.op_str else 0)
    return sorted(rets)


def pseudo(ea):
    p = os.path.join(ROOT, "export", "funcs", f"{ea:08x}.c")
    return open(p).read() if os.path.exists(p) else None


def proto_of(src):
    """The prototype line of exported pseudocode (export_pseudo.py writes the function's comment lines above it)."""
    return next((l for l in (src or "").splitlines() if l.strip() and not l.lstrip().startswith("//")), "")


def substitute_names(db, text):
    def rep(m):
        ea = int(m.group(1), 16)
        n = db.name(ea)
        return n if n else m.group(0)
    text = re.sub(r"\bsub_([0-9A-F]+)\b", rep, text)

    def grep_(m):
        g = db.globals.get(int(m.group(2), 16))
        return g["name"] if g else m.group(0)
    return re.sub(r"\b(dword|word|byte|flt|dbl|qword|off|unk|stru)_([0-9A-F]+)\b", grep_, text)


def struct_text(db, vt):
    hdr = open(os.path.join(ROOT, "src", "types", "auto_classes.h")).read()
    m = re.search(rf"(?://[^\n]*\n)*struct {re.escape(db.cls_name(vt))}\b.*?\n}};\n(?:static_assert[^\n]*\n)*", hdr, re.S)
    return m.group(0) if m else None


_hdr_fields = None


def class_fields(db, vt):
    """{offset: (class name, type, member)} declared for vt and its bases in auto_classes.h (offsets from field_XXXX
    names and the offsetof asserts of named fields)."""
    global _hdr_fields
    if _hdr_fields is None:
        hdr = open(os.path.join(ROOT, "src", "types", "auto_classes.h")).read()
        _hdr_fields = {}
        for m in re.finditer(r"^struct (\w+)[^{]*\{\n(.*?)\n\};\n((?:static_assert[^\n]*\n)*)", hdr, re.S | re.M):
            offs = {n: int(o, 16) for n, o in re.findall(r"offsetof\(\w+, (\w+)\) == 0x([0-9a-f]+)", m.group(3))}
            fl = {}
            for t, n in re.findall(r"^\s+([\w:]+\*?) (\w+)(?:\[\w+\])?;", m.group(2), re.M):
                if n.startswith("pad_") or n == "vtbl":
                    continue
                o = int(n[6:], 16) if re.fullmatch(r"field_[0-9a-f]{4}", n) else offs.get(n)
                if o is not None:
                    fl[o] = (t, n)
            _hdr_fields[m.group(1)] = fl
    out, x = {}, vt
    while x:
        for o, (t, n) in _hdr_fields.get(db.cls_name(x), {}).items():
            out.setdefault(o, (db.cls_name(x), t, n))
        x = db.classes.get(x, {}).get("base")
    return out


def compact_class(db, vt, ea, size):
    """The fields of the class chain whose offsets this function's memory operands use (cheap: the full struct
    dump was ~half of every GUI packet). Displacements are matched on any base register, so it may over-include."""
    fields = class_fields(db, vt)
    disp = set()
    for i in disasm(ea, size):
        for m in re.finditer(r"\[(e(?!bp|sp)\w\w) \+ (0x[0-9a-f]+|\d+)\]", i.op_str):  # not stack slots
            disp.add(int(m.group(2), 0))
        if re.search(r"\[e(?!bp|sp)\w\w\]", i.op_str):
            disp.add(0)
    used = sorted(o for o in disp if o in fields)
    lines = [f"- +0x{o:03X} {fields[o][0]}::{fields[o][2]}  ({fields[o][1]})" for o in used]
    text = ("Fields at the offsets this function uses (declared in types/auto_classes.h, DO NOT redefine; "
            "access through the struct, other offsets via CSF_AT):\n" + ("\n".join(lines) if lines else "(none declared)"))
    # virtual calls this function makes at slot offsets the class chain has a typed wrapper for
    slots = {int(m.group(1), 0) for i in disasm(ea, size) if i.mnemonic in ("call", "jmp")
             for m in [re.search(r"\[e\w\w \+ (0x[0-9a-f]+|\d+)\]", i.op_str)] if m}
    if 0 in disp:
        slots.add(0)
    vm = [(o, cls, l) for o, (cls, l) in class_vmethods(db, vt).items() if o in slots]
    if vm:
        text += ("\nVirtual calls with typed wrappers (when the object is this class or a base, call e.g. "
                 "`self->vName(args)` instead of casting vtbl[slot]):\n" +
                 "\n".join(f"- slot +0x{o:X}: {cls}::{l}" for o, cls, l in sorted(vm)))
    return text


_hdr_vm = None


def class_vmethods(db, vt):
    """{slot offset: (class, wrapper signature)} for vt and its bases (auto_classes.h vmethods)."""
    global _hdr_vm
    if _hdr_vm is None:
        hdr = open(os.path.join(ROOT, "src", "types", "auto_classes.h")).read()
        _hdr_vm = {}
        for m in re.finditer(r"^struct (\w+)[^{]*\{\n(.*?)\n\};", hdr, re.S | re.M):
            _hdr_vm[m.group(1)] = {int(o, 16): sig for sig, o in re.findall(
                r"^\s+(\S.*?\))\s*\{ return .*?// slot \+0x([0-9A-F]+) ->", m.group(2), re.M)}
    out, x = {}, vt
    while x:
        for o, sig in _hdr_vm.get(db.cls_name(x), {}).items():
            out.setdefault(o, (db.cls_name(x), sig))
        x = db.classes.get(x, {}).get("base")
    return out


STL_LAYOUT = {
    "vector": "allocator@0 (4 bytes, empty), _Myfirst@4, _Mylast@8, _Myend@12 (16 bytes)",
    "vector<bool>": "_Myvec (vector<unsigned>, 16 bytes)@0, _Mysize@16",
    "string": "allocator@0, _Bx@4 (char[16] inline buffer, or char* when _Myres >= 16), _Mysize@20, _Myres@24 (28 bytes)",
    "list": "allocator@0, _Myhead@4 (sentinel node: _Next@0, _Prev@4, value@8), _Mysize@8 (12 bytes)",
    "tree": "comparator/allocator@0, _Myhead@4 (sentinel: _Left@0, _Parent@4, _Right@8, _Color@12, _Isnil@13, "
            "value after), _Mysize@8 (12 bytes)",
}
_stl = None


def stl_note(ea):
    global _stl
    if _stl is None:
        _stl = {int(x["ea"], 16): x for x in json.load(open(os.path.join(ROOT, "db", "stl.json")))}
    x = _stl.get(ea)
    if not x:
        return None
    k = x["kind"]
    return (f"\n## STL instantiation\nThis is a VC7.1 (MSVC 2003) `std::{k}` template member (or internal helper), instantiated for a game "
            f"element type. Layout: {STL_LAYOUT.get(k, '?')}. Reproduce its exact behavior on raw memory at these "
            "offsets (CSF_AT / casts); do NOT use our compiler's std:: containers (different layout). Element copies, "
            "constructors and destructors appear inlined or as callees: keep them exactly (same callees, same element "
            "size and order). Allocate/free with exactly the functions the original calls.")


CODEGEN_HELPER = re.compile(r"^__ftol2?(_sse)?$|^__a(u)?ll(mul|div|rem|shl|shr)$|^__alloca_probe|^_?_chkstk$"
                            r"|^__CI(a?cos|a?sin|a?tan2?|pow|fmod|exp|log(10)?|sqrt|sinh|cosh|tanh)(_pentium4)?$")


VT_STORE = re.compile(r"^dword ptr \[(\w+)(?: \+ (0x[0-9a-f]+|\d+))?\], (0x[0-9a-f]+)$")


def vtable_refs(db, ea, size):
    """Every instruction referencing a known vtable, with the line that reproduces it: a store
    `mov [reg+off], vt` -> `CSF_AT(void*, obj, off) = CSF_VTBL(vt)` (off 0: `obj->vtbl` on a declared class)."""
    out = []
    for i in disasm(ea, size):
        for v in re.findall(r"0x[0-9a-f]{6,8}", i.op_str):
            vt = f"{int(v, 16):08x}"
            if vt not in db.classes:
                continue
            nm = db.cls_name(vt)
            m = VT_STORE.match(i.op_str) if i.mnemonic == "mov" else None
            if m:
                off = int(m.group(2), 0) if m.group(2) else 0
                how = (f"`CSF_AT(void*, obj, 0x{off:X}) = CSF_VTBL(0x{vt.upper()});`" if off else
                       f"`*(void***)obj = CSF_VTBL(0x{vt.upper()});` (or `obj->vtbl = ...` on a declared class)")
                out.append(f"- 0x{i.address:08X} `{i.mnemonic} {i.op_str}`: stores {nm}'s vtable into "
                           f"[{m.group(1)}+0x{off:X}] -> {how}")
            elif i.mnemonic in ("mov", "lea") and i.op_str.split(",")[0] in ("eax", "ebx", "ecx", "edx", "esi", "edi", "ebp"):
                out.append(f"- 0x{i.address:08X} `{i.mnemonic} {i.op_str}`: loads {nm}'s vtable (stored or compared "
                           f"next) -> `CSF_VTBL(0x{vt.upper()})`")
            elif i.mnemonic == "cmp":
                out.append(f"- 0x{i.address:08X} `{i.mnemonic} {i.op_str}`: type test against {nm}'s vtable -> "
                           f"compare with `CSF_VTBL(0x{vt.upper()})`")
            else:
                out.append(f"- 0x{i.address:08X} `{i.mnemonic} {i.op_str}`: references {nm}'s vtable -> "
                           f"`CSF_VTBL(0x{vt.upper()})`")
    return out


def stack_objects(ea, size):
    """[(X, room, [inner offsets])] for each ebp-X whose address (`lea r, [ebp - X]`) is pushed or put in ecx/edx
    before a call: room = X minus the next such object's offset closer to ebp (or X: up to the frame end)."""
    ins = disasm(ea, size)
    taken = set()
    for k, i in enumerate(ins):
        m = re.fullmatch(r"(e[a-z]{2}), \[ebp - (0x[0-9a-f]+|\d+)\]", i.op_str) if i.mnemonic == "lea" else None
        if not m:
            continue
        reg, x = m.group(1), int(m.group(2), 0)
        for j in ins[k + 1:k + 8]:
            if j.mnemonic == "push" and j.op_str == reg or reg in ("ecx", "edx") and j.mnemonic == "call":
                taken.add(x)
                break
            if j.op_str.startswith(reg + ",") and j.mnemonic != "push" or j.mnemonic in ("call", "ret") \
                    or j.mnemonic.startswith("j"):
                break
    if not taken:
        return []
    direct = {int(m.group(1), 0) for i in ins for m in re.finditer(r"\[ebp - (0x[0-9a-f]+|\d+)\]", i.op_str)}
    xs = sorted(taken, reverse=True)
    out = []
    for n, x in enumerate(xs):
        nxt = xs[n + 1] if n + 1 < len(xs) else 0
        out.append((x, x - nxt, sorted((y for y in direct if nxt < y < x), reverse=True)))
    return out


def build(db, ea, with_asm_limit=400):
    f = db.F[ea]
    row = db.rows[ea]
    src = pseudo(ea)
    if src is None:
        raise SystemExit(f"no pseudocode for {ea:08x}")
    out = []
    rets = ret_bytes(ea, f["size"])
    proto = proto_of(src)
    out.append(f"## Target function 0x{ea:08X} (size {f['size']} bytes, region {row['region']})")
    out.append(f"Hex-Rays prototype: `{proto}`")
    import api as _api
    import regargs as _ra
    if _ra.needs_keep_ecx(db, ea):
        out.append("NOTE: callers keep a value in ecx across this call (LTCG) and the original never writes ecx: register "
                   "it with HOOK_KEEP_ECX(0xADDR, Fn, <number of stack args>, \"module\") instead of HOOK "
                   "(HOOK_KEEP_ECX_CDECL when the original is __cdecl: it returns with a plain `ret`).")
    rr = _api.real_regs(ea, db)
    if rr:
        out.append(f"NOTE: it takes an argument in {', '.join(rr)} (exact: the register is read before it is written). "
                   "Write it as `static uint32_t __cdecl Fn(hook::Regs* regs, const uint32_t* args)`: regs->eax / ebx / "
                   "ecx / edx / esi / edi are the registers at entry, args[0] is the first stack argument, the result "
                   "is eax. Register with HOOK_REGS(0xADDR, Fn, N, \"module\") instead of HOOK, N = the original's "
                   "`ret N` (0 for a plain `ret`). No asm, no __regcall.")
    sp = _api.spurious_regs(ea)
    if sp:
        out.append(f"NOTE: its {', '.join('@<' + r + '>' for r in sp)} parameter is NOT an argument (exact: the register "
                   "is only saved and restored, e.g. `push edi` ... `pop edi`, or passed to a callee that does the same). "
                   "Implement and bind it without that parameter, as a plain C++ convention; never __regcall or asm.")
    out.append(f"Original `ret` immediate(s): {rets}  (stack bytes the callee pops; must match exactly)")
    mod = db.modules.get(row["module"])
    if mod and mod.get("name"):  # subsystem for Subsystem_Verb names of non-class functions
        out.append(f"Module: {mod['name']}" + (f" ({mod['note']})" if mod.get("note") else ""))
    vt = db.class_of(ea)
    if vt and "__thiscall" not in proto:
        import api
        if not api.takes_reg(ea, db, "ecx"):  # a shared stub in a vtable (`ret 0x10`): no `this` at all
            out.append(f"Listed in {db.cls_name(vt)}'s vtable but never reads ecx: write it as a plain function "
                       "(no `this` parameter), keeping the original's `ret` immediate.")
            vt = None
    if vt:
        c = db.classes[vt]
        out.append(f"\n## `this` class: {db.cls_name(vt)} (" + ("no vtable: a plain struct, its methods found by "
                   "what they call on the same `this`" if c.get("plain") else f"vtable 0x{vt.upper()}")
                   + f", size {c['size'] or 'unknown'}, base {db.cls_name(c['base']) if c['base'] else 'none'})")
        if vt in db.class_names:
            out.append(f"Role: {db.class_names[vt].get('why', '')}. Name your function {db.cls_name(vt)}_<Verb...>.")
        chain, x = [], c["base"]
        while x:
            chain.append(db.cls_name(x))
            x = db.classes.get(x, {}).get("base")
        if chain:
            out.append("Bases: " + " : ".join(chain))
        out.append(compact_class(db, vt, ea, f["size"]))
    note = stl_note(ea)
    if note:
        out.append(note)
    sw = {int(k) for k in ((ida_view(ea) or {}).get("switches") or {})}
    tails = [i for i in disasm(ea, f["size"]) if i.mnemonic == "jmp" and not i.op_str.startswith("0x")
             and i.address not in sw]
    ins = disasm(ea, f["size"])
    if any("fs:" in i.op_str for i in ins[:12]):
        out.append("\n## Exception frame\nThe prologue (`push -1; push 0x...; mov eax, fs:[0]`) is MSVC's C++ "
                   "exception frame, emitted for locals with destructors / try blocks. Don't reproduce it and don't "
                   "reference the pushed handler address (not a function); just write the C++ with those locals.")
    if tails:  # the gate rejects a call here: say so up front (was the most common real retry)
        out.append(f"\n## Indirect tail jump\nThe original ends in `jmp {tails[0].op_str}` at 0x{tails[0].address:08X}: it "
                   "forwards the caller's stack args and return address untouched to a target of unknown signature. Do "
                   "NOT turn it into a call and write no asm. Write everything before the jump as\n"
                   "`static hook::Tail __fastcall Fn(void* self, const uint32_t* args)` (self: ecx on entry; args: the "
                   "caller's stack arguments, read only, args[0] is the first) and end it with\n"
                   "`return hook::tail_jump(obj, (*reinterpret_cast<void***>(obj))[0xNN / 4]);` (obj: what the "
                   "original puts in ecx for the target; then the jump target, with the slot's byte offset as a "
                   "constant). A branch that jumps straight to another function (`jmp 0xADDR`): "
                   "`return hook::tail_jump(obj, CSF_ADDR_FN(0xADDR));`. "
                   "A path where the original returns itself: `return hook::tail_ret(eax_value);` "
                   "for a plain `ret`, `return hook::tail_ret(eax_value, N);` for `ret N`. Fn takes exactly those two "
                   "parameters whatever the original pops (a third one would pop stack arguments the jump target "
                   "still needs). Register with HOOK_TAIL(0xADDR, Fn, \"module\") instead of HOOK.")
    out.append("\n## Hex-Rays pseudocode (names substituted where known)\n"
               + ("Typed from our headers: `x->m_name` / `x->field_XXXX` are the members of types/auto_classes.h; "
                  "`x->vName(x, args)` is a virtual call: write `x->vName(args)` (the class's wrapper); "
                  "`__vftable` is our `vtbl`.\n" if re.search(r"->(v[A-Z]\w*|__vftable|m_\w+|field_[0-9a-fA-F]+)\b", src)
                  else "")
               + "```c\n" + substitute_names(db, src) + "\n```")
    if f["size"] <= with_asm_limit:
        out.append("\n## Disassembly (authoritative for signedness / exact semantics)\n```asm\n" +
                   "\n".join(f"{i.address:08x}  {i.mnemonic} {i.op_str}" for i in disasm(ea, f["size"])) + "\n```")
    so = stack_objects(ea, f["size"])
    if so:
        out.append("\n## Stack objects whose address the original passes to a callee\n"
                   "Callees may fill all of an object: size each buffer to the room shown (Hex-Rays often splits one "
                   "object into an array plus scalars; 0x461750 overran a 39-dword buffer that is 0xE4 bytes).\n"
                   + "\n".join(f"- [ebp-0x{x:X}]: room 0x{room:X} bytes up to the next such object / the frame end"
                                + (f"; also accessed directly at {', '.join(f'ebp-0x{y:X}' for y in inner)}" if inner else "")
                                for x, room, inner in so))
    # Functions stored or passed as values: the pseudocode shows them by name only, and above with_asm_limit no
    # disassembly gives the address (0x6E0920 compiled to nothing behind an #if for want of five of them).
    stored = []
    for i in disasm(ea, f["size"]):
        m = re.search(r"\b0x([0-9a-f]{6,8})$", i.op_str)
        if m and i.mnemonic in ("mov", "push") and int(m.group(1), 16) in db.F:
            t = int(m.group(1), 16)
            line = (f"- `{i.mnemonic} {i.op_str}` at 0x{i.address:08X}: "
                    f"{(db.rows.get(t) or {}).get('name') or f'sub_{t:X}'} = `CSF_ADDR_FN(0x{t:08X})`")
            stored.append(line)
    if stored:
        out.append("\n## Functions used as values (stored or passed, not called): their addresses, in code order\n"
                   + "\n".join(stored))
    if f["callees"]:
        out.append("\n## Callees")
        for c in f["callees"]:
            cr = db.rows.get(c)
            cs = pseudo(c)
            cproto = proto_of(cs) or "?"
            status = cr["status"] if cr else "?"
            nm = db.name(c) or f"sub_{c:X}"
            if CODEGEN_HELPER.search(nm):
                out.append(f"- 0x{c:08X} `{nm}`: MSVC code-gen helper. Do NOT call or reference it; write the plain C++ "
                           "it came from (a float->int cast, int64 arithmetic, a large local array; __CIfmod / __CIpow / __CIacos ... are "
                           "the math library with operands on the x87 stack: std::fmod, std::pow, std::acos from <cmath>)")
                continue
            import api
            crr = api.real_regs(c, db)
            if crr:
                out.append(f"- 0x{c:08X} `{nm}` [{status}] takes arguments in {', '.join(crr)} (plus any stack arguments "
                           "the asm pushes): do NOT declare it with CSF_FUNC. Call it with `hook::Regs r{}; r.eax = ...; "
                           f"r.esi = ...; uint32_t stack[] = {{first, second}}; uint32_t eax = hook::call_regs("
                           f"CSF_ADDR_FN(0x{c:08X}), &r, stack, 2);` (set the registers the original loads before the "
                           "call; pass nullptr, 0 when it pushes nothing; who pops the stack does not matter)")
                continue
            canon = api.decl_line(c)
            if c in db.F and db.F[c]["size"] <= 64:
                csw = {int(k) for k in ((ida_view(c) or {}).get("switches") or {})}
                if any(i.mnemonic == "jmp" and not i.op_str.startswith("0x") and i.address not in csw
                       for i in disasm(c, db.F[c]["size"])):
                    # 0x454070 bound two such forwarders `int __cdecl ()` and dropped `this` and the argument
                    out.append(f"- 0x{c:08X} `{nm}` [{status}] is a forwarder: it tail-jumps to a virtual / function "
                               "pointer, so its arguments are that target's, not its own prototype's. Bind it by what "
                               "THIS function's asm passes at the call: __fastcall (void* self, void* edx, ...) with "
                               "ecx as the asm loads it and one parameter per push before the call (the target pops "
                               "them unless the asm adds to esp after the call: then __cdecl)")
                    continue
            out.append(f"- 0x{c:08X} `{nm}` [{status}] rets={ret_bytes(c, db.F[c]['size']) if c in db.F else '?'} "
                       + (f"bind as: `{canon}`" + ("  (unnamed: keep this signature, the name is yours to choose)"
                                                   if api.load()[c]["name_src"] == "addr" else "")
                          if canon else f"proto: `{cproto}`")
                       + ("  NORETURN (throws/aborts): still call it on exactly the same "
                          "condition, even if the branch looks unreachable" if "__noreturn" in cproto else ""))
    vs = vtable_refs(db, ea, f["size"])
    if vs:  # constructor/destructor vtable stores were the most common semantic rejection (dropped as redundant)
        out.append("\n## Vtables referenced (each one must appear in your code, even a store overwritten right after: "
                   "base-class stores in a ctor/dtor chain are part of the behavior)\n" + "\n".join(vs))
    gl = [d for d in f["drefs"]]
    if gl:
        out.append("\n## Data referenced\n" + "\n".join(
            f"- 0x{d:08X}" + (f" `{db.globals[d]['name']}`: {db.globals[d]['type']}" if d in db.globals else "")
            for d in gl))
    if f.get("srefs"):
        out.append("\n## Strings referenced (use CSF_STR(addr, \"text\"), never a bare literal)\n" + "\n".join(
            f"- 0x{a:08X}: {s!r}" for a, s in list(zip(f["srefs"], f["strings"]))[:30]))
    return "\n".join(out)


if __name__ == "__main__":
    db = DB()
    print(build(db, int(sys.argv[1], 16)))
