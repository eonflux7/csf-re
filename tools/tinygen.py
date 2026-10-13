#!/usr/bin/env python3
"""The game functions under 6 bytes (the lanes' min_size, tools/grunt.py select()): classify them from the bytes
of the original and write their src/auto files without a model.

A function this small is one of a handful of shapes (`ret`, `ret N`, `xor eax, eax; ret`, `mov al, 1; ret N`,
`mov eax, [ecx+N]; ret`, `jmp target`, ...). Each shape has one C++ form; what the bytes do not say (the name, a
class for `this`) comes from db/functions.csv and is cosmetic. The data used:

  work/pc/CommXPC.exe      the bytes, the padding behind them, every reference to the address
  export/functions.jsonl   function bounds and direct callers
  db/vtables.json          which references are vtable slots
  tools/regargs.py         callers that keep a value in ecx across the call (HOOK_KEEP_ECX)

  tinygen.py                 summary: shapes, references, room for the 5-byte hook
  tinygen.py --list          one line per function
  tinygen.py --others        the todo functions of 6 bytes and more outside stl and queue/: why no lane took them
  tinygen.py --write [EA..]  write src/auto/<ea>.cpp for every function with a known shape and room for the hook
                             (never over an existing file); --gate runs tools/gate.py on each and keeps the passes
  tinygen.py --json FILE    full classification and reference locations (read-only analysis)

Direct and virtual jumps are naked forwarding thunks, not signature-dependent C++ calls. They preserve the
original stack and register arguments and use CSF_ADDR_FN for direct targets in both builds. No database writes.
"""
import argparse, bisect, collections, json, os, re, struct, sys
sys.path.insert(0, os.path.dirname(os.path.abspath(__file__)))

ROOT = os.path.dirname(os.path.dirname(os.path.abspath(__file__)))
IMG = 0x400000
MIN_SIZE = 6   # tools/grunt.py select(min_size)
JMP = 5        # hook/main.cpp install(): E9 rel32 at the entry
PAD = (0xCC, 0x90)

_img = None


def image():
    global _img
    if _img is None:
        import pefile
        _img = pefile.PE(os.path.join(ROOT, "work", "pc", "CommXPC.exe"), fast_load=True).get_memory_mapped_image()
    return _img


def code(ea, n):
    return bytes(image()[ea - IMG: ea - IMG + n])


def queued():
    return {int(f[:8], 16) for f in os.listdir(os.path.join(ROOT, "queue")) if re.fullmatch(r"[0-9a-f]{8}\.md", f)}


def targets(db):
    """(tiny, others): game todo under MIN_SIZE bytes; game todo of MIN_SIZE and more outside stl and queue/."""
    q = queued()
    tiny, others = [], []
    for ea, r in sorted(db.rows.items()):
        if r["region"] != "game" or r["status"] != "todo":
            continue
        if int(r["size"]) < MIN_SIZE:
            tiny.append(ea)
        elif r["module"] != "stl" and ea not in q:
            others.append(ea)
    return tiny, others


# ---- shape ----------------------------------------------------------------------------------------------------

def shape(ea, size):
    """(class, text, facts) of a function's bytes. facts: ret (bytes popped), value (what eax/al/st0 holds), and
    for a jump the target."""
    from capstone import Cs, CS_ARCH_X86, CS_MODE_32
    md = Cs(CS_ARCH_X86, CS_MODE_32)
    md.detail = False
    ins = list(md.disasm(code(ea, size), ea))
    text = "; ".join(f"{i.mnemonic} {i.op_str}".strip() for i in ins)
    if sum(i.size for i in ins) != size or not ins:
        return "undecoded", text or code(ea, size).hex(), {}
    last = ins[-1]
    if last.mnemonic == "jmp" and len(ins) == 1 and last.op_str.startswith("0x"):
        return "jmp target", text, {"target": int(last.op_str, 16)}
    if len(ins) == 2 and text.startswith("mov eax, dword ptr [ecx]; jmp dword ptr [eax"):
        return "mov eax, [ecx]; jmp [eax+N]", text, {"vjmp": [f"{i.mnemonic} {i.op_str}" for i in ins]}
    if last.mnemonic != "ret":
        return "other", text, {}
    pop = int(last.op_str, 16) if last.op_str else 0
    f = {"ret": pop}
    body = [f"{i.mnemonic} {i.op_str}".strip() for i in ins[:-1]]
    rn = "ret N" if pop else "ret"
    if not body:
        return rn, text, f
    if len(body) > 1:
        return "other", text, f
    b = body[0]
    m = re.fullmatch(r"(xor|sub) (eax|al), \2", b)
    if m:
        f["value"] = ("int" if m.group(2) == "eax" else "bool", 0)
        return f"{'xor eax, eax' if m.group(2) == 'eax' else 'xor al, al'}; {rn}", text, f
    m = re.fullmatch(r"mov al, (0x[0-9a-f]+|\d+)", b)
    if m:
        f["value"] = ("bool", int(m.group(1), 0))
        return f"mov al, imm; {rn}", text, f
    m = re.fullmatch(r"or eax, 0xffffffff", b)
    if m:
        f["value"] = ("int", -1)
        return f"or eax, -1; {rn}", text, f
    if b == "xor ax, ax":
        f["value"] = ("int16", 0)
        return f"xor ax, ax; {rn}", text, f
    off = r"\[ecx(?: ([+-]) (0x[0-9a-f]+|\d+))?\]"
    disp = lambda sign, n: (-1 if sign == "-" else 1) * int(n or "0", 0)
    m = re.fullmatch(rf"mov byte ptr {off}, (0x[0-9a-f]+|\d+)", b)
    if m:
        f["store"] = ("uint8_t", disp(m.group(1), m.group(2)), "=", int(m.group(3), 0))
        return f"mov byte [ecx+N], imm; {rn}", text, f
    m = re.fullmatch(rf"and dword ptr {off}, (0x[0-9a-f]+|\d+)", b)
    if m:
        f["store"] = ("uint32_t", disp(m.group(1), m.group(2)), "&=", int(m.group(3), 0))
        return f"and dword [ecx+N], imm; {rn}", text, f
    m = re.fullmatch(rf"inc dword ptr {off}", b)
    if m:
        f["store"] = ("uint32_t", disp(m.group(1), m.group(2)), "++", None)
        return f"inc dword [ecx+N]; {rn}", text, f
    m = re.fullmatch(rf"movzx eax, (byte|word) ptr {off}", b)
    if m:
        f["value"] = ("zx8" if m.group(1) == "byte" else "zx16", disp(m.group(2), m.group(3)))
        return f"movzx eax, [ecx+N]; {rn}", text, f
    m = re.fullmatch(rf"lea eax, {off}", b)
    if m and m.group(1) == "-":
        f["value"] = ("addr", disp(m.group(1), m.group(2)))
        return f"lea eax, [ecx-N]; {rn}", text, f
    if b == "mov eax, ecx":
        f["value"] = ("this", None)
        return f"mov eax, ecx; {rn}", text, f
    m = re.fullmatch(r"(mov|lea) eax, (?:dword ptr )?\[ecx(?: \+ (0x[0-9a-f]+|\d+))?\]", b)
    if m:
        f["value"] = ("field" if m.group(1) == "mov" else "addr", int(m.group(2) or "0", 0))
        return f"{m.group(1)} eax, [ecx+N]; {rn}", text, f
    m = re.fullmatch(r"mov (al|ax), (?:byte|word) ptr \[ecx(?: \+ (0x[0-9a-f]+|\d+))?\]", b)
    if m:
        f["value"] = ("field8" if m.group(1) == "al" else "field16", int(m.group(2) or "0", 0))
        return f"mov {m.group(1)}, [ecx+N]; {rn}", text, f
    m = re.fullmatch(r"fld dword ptr \[ecx(?: \+ (0x[0-9a-f]+|\d+))?\]", b)
    if m:
        f["value"] = ("fieldf", int(m.group(1) or "0", 0))
        return f"fld [ecx+N]; {rn}", text, f
    if b in ("fldz", "fld1"):
        f["value"] = ("float", 0.0 if b == "fldz" else 1.0)
        return f"{b}; {rn}", text, f
    return "other", text, f


# ---- references -----------------------------------------------------------------------------------------------

class Refs:
    """Every reference to a set of addresses in the image: direct call / jump (rel32 from an instruction IDA knows),
    vtable slot, other pointer (callback table, `push offset`, `mov [x], offset`)."""

    def __init__(self, db, eas):
        from packet import disasm
        self.db = db
        want = set(eas)
        self.calls = collections.defaultdict(set)   # ea -> calling functions
        self.jumps = collections.defaultdict(set)   # ea -> functions that jump to it (tail calls, thunks)
        self.vslots = collections.defaultdict(list)  # ea -> [(vtable, slot)]
        self.ptrs = collections.defaultdict(list)   # ea -> [address of the pointer]
        self.string_matches = collections.defaultdict(list)  # coincidences, e.g. 0x00752520 = b' %u\0'
        for c, f in db.F.items():
            if not want & set(f.get("callees") or []):
                continue
            for i in disasm(c, f["size"]):
                if i.op_str.startswith("0x") and (i.mnemonic == "call" or i.mnemonic.startswith("j")):
                    t = int(i.op_str, 16)
                    if t in want:
                        (self.calls if i.mnemonic == "call" else self.jumps)[t].add(c)
        slot_at = {}
        for v in json.load(open(os.path.join(ROOT, "db", "vtables.json"))):
            a = int(v["addr"], 16)
            for k, s in enumerate(v["slots"]):
                slot_at[a + 4 * k] = (a, k)
                if int(s, 16) in want:
                    self.vslots[int(s, 16)].append((a, k))
        img = image()
        # A byte search alone invents references in format strings (even at aligned addresses).
        strings = []
        for a, s in json.load(open(os.path.join(ROOT, 'export', 'strings.json'))).items():
            a = int(a, 16)
            try:
                literal = s.encode('latin-1') + b'\0'
            except UnicodeEncodeError:
                continue
            if code(a, len(literal)) == literal:
                strings.append((a, a + len(literal)))
        strings.sort()
        string_starts = [a for a, _ in strings]
        for ea in want:
            pat, pos = struct.pack("<I", ea), 0
            while True:
                pos = img.find(pat, pos)
                if pos < 0:
                    break
                at = IMG + pos
                pos += 1
                if at in slot_at:
                    continue
                k = bisect.bisect_right(string_starts, at) - 1
                if k >= 0 and at < strings[k][1]:
                    self.string_matches[ea].append(at)
                    continue
                # a rel32 that happens to spell the address is not a pointer: only data sections and operands of
                # instructions inside known functions count
                if self.in_code(at) and not self.is_operand(at, ea):
                    continue
                self.ptrs[ea].append(at)

    def in_code(self, at):
        fn = self.func_at(at)
        return fn is not None

    _starts = None

    def func_at(self, at):
        if self._starts is None:
            self._starts = sorted(self.db.F)
        k = bisect.bisect_right(self._starts, at) - 1
        if k < 0:
            return None
        f = self.db.F[self._starts[k]]
        return f["ea"] if f["ea"] <= at < f["end"] else None

    def is_operand(self, at, ea):
        from packet import disasm
        fn = self.func_at(at)
        for i in disasm(fn, self.db.F[fn]["size"]):
            if i.address < at < i.address + i.size and at + 4 <= i.address + i.size:
                return i.mnemonic != "call" and not i.mnemonic.startswith("j") and f"0x{ea:x}" in i.op_str
        return False

    def kind(self, ea):
        d = bool(self.calls[ea] or self.jumps[ea])
        v = bool(self.vslots[ea])
        p = bool(self.ptrs[ea])
        return ("direct" if d else "") + ("+" if d and (v or p) else "") + ("vtable" if v else "") + \
               ("+" if v and p else "") + ("pointer" if p else "") or "none"


def room(db, ea, starts):
    """(bytes up to the next function, are the bytes the 5-byte jump overwrites behind the function padding)."""
    size = db.F[ea]["size"]
    k = bisect.bisect_right(starts, ea)
    nxt = starts[k] if k < len(starts) else ea + 99
    tail = code(ea + size, max(0, JMP - size))
    return nxt - ea, all(b in PAD for b in tail), tail


def analyze(db, eas):
    starts = sorted(db.F)
    refs = Refs(db, eas)
    out = []
    for ea in eas:
        size = db.F[ea]["size"]
        cls, text, facts = shape(ea, size)
        r, pad, tail = room(db, ea, starts)
        out.append(dict(ea=ea, size=size, cls=cls, text=text, facts=facts, room=r, pad_ok=pad, tail=tail.hex(),
                        refs=refs.kind(ea), ncalls=len(refs.calls[ea]), njumps=len(refs.jumps[ea]),
                        nvslots=len(refs.vslots[ea]), nptrs=len(refs.ptrs[ea]),
                        callers=sorted(refs.calls[ea] | refs.jumps[ea]), name=db.name(ea) or "",
                        module=db.rows[ea]["module"], hookable=r >= JMP and pad))
    return out, refs


def src_refs():
    """address -> number of src/ files that bind it (CSF_FUNC / CSF_ADDR_FN): what the standalone link asks for."""
    import glob
    n = collections.Counter()
    for p in glob.glob(os.path.join(ROOT, "src", "**", "*.cpp"), recursive=True):
        s = open(p, errors="replace").read()
        seen = {int(m, 16) for m in re.findall(r"CSF_FUNC\([^;]*?,\s*0x([0-9A-Fa-f]+)\s*,", s)}
        seen |= {int(m, 16) for m in re.findall(r"CSF_ADDR_FN\(\s*0x([0-9A-Fa-f]+)\s*\)", s)}
        for a in seen:
            n[a] += 1
    return n


def summary(rows, db):
    sr = src_refs()
    print(f"{len(rows)} functions, {sum(r['size'] for r in rows)} bytes")
    by = collections.defaultdict(list)
    for r in rows:
        by[r["cls"]].append(r)
    print(f"\n{'shape':28} {'n':>4} {'bytes':>5} {'direct':>6} {'vtable':>6} {'both':>5} {'ptr':>4} {'none':>4} "
          f"{'room>=5':>7} {'bound in src':>12}")
    for cls, rs in sorted(by.items(), key=lambda x: -len(x[1])):
        d = sum(1 for r in rs if (r["ncalls"] or r["njumps"]) and not r["nvslots"])
        v = sum(1 for r in rs if r["nvslots"] and not (r["ncalls"] or r["njumps"]))
        b = sum(1 for r in rs if r["nvslots"] and (r["ncalls"] or r["njumps"]))
        p = sum(1 for r in rs if r["nptrs"])
        z = sum(1 for r in rs if r["refs"] == "none")
        print(f"{cls:28} {len(rs):4} {sum(r['size'] for r in rs):5} {d:6} {v:6} {b:5} {p:4} {z:4} "
              f"{sum(1 for r in rs if r['hookable']):7} {sum(1 for r in rs if sr.get(r['ea'])):12}")
    print(f"\nsizes: {dict(sorted(collections.Counter(r['size'] for r in rows).items()))}")
    print(f"room to the next function: {dict(sorted(collections.Counter(r['room'] for r in rows).items()))}")
    print(f"bytes the hook overwrites behind the function: "
          f"{dict(collections.Counter(r['tail'] or '-' for r in rows).most_common())}")
    done = lambda c: db.rows.get(c, {}).get("status") == "done"
    callers = {c for r in rows for c in r["callers"]}
    print(f"direct callers: {len(callers)} functions, {sum(1 for c in callers if done(c))} of them done; "
          f"bound by src/ files today: {sum(1 for r in rows if sr.get(r['ea']))} of {len(rows)} "
          f"({sum(sr.get(r['ea'], 0) for r in rows)} bindings)")


def listing(rows):
    for r in rows:
        print(f"{r['ea']:08x} {r['size']} {code(r['ea'], r['size']).hex():10} {r['cls']:24} room {r['room']:2} "
              f"{'pad' if r['pad_ok'] else 'NOPAD':5} calls {r['ncalls']:3} jumps {r['njumps']:2} vslots {r['nvslots']:3} "
              f"ptrs {r['nptrs']:2}  {r['text']:34} {r['name']}")


def generate(db, r):
    """A closed whitelist of byte shapes. Return None for anything not understood or without patch room.

    Return widths come from the instructions, not guessed prototypes; ret N supplies ignored stack dwords.
    ECX-relative leaves use fastcall's self/dummy EDX. Keep ECX where the existing caller analysis requires it.
    A raw byte result is uint8_t, never bool: field bytes need not be normalized to 0/1.
    """
    import regargs
    record = db.rows.get(r['ea'], {})
    if record.get('region') != 'game' or record.get('status') != 'todo' \
            or not r['hookable'] or not 0 < r['size'] < MIN_SIZE:
        return None
    ea, f = r['ea'], r['facts']
    fn = f"Tiny_{ea:08X}"  # stable, unique even when DB names collide
    head = ('// Generated by tools/tinygen.py from CommXPC.exe; do not infer a signature from the name.\n'
            f"// Original {ea:08X}: {code(ea, r['size']).hex()} ({r['text']}); "
            f"next function +{r['room']}, overwritten padding {r['tail'] or 'none'}.\n"
            '#include <cstdint>\n#include "globals.h"\n#include "hook.h"\n\n')
    if 'target' in f:
        # One memory-indirect jump: no scratch register and no outgoing argument reconstruction.
        return (head + f"static const uintptr_t tiny_target = CSF_ADDR_FN(0x{f['target']:08X});\n"
                f"static __declspec(naked) void {fn}() {{\n"
                "    __asm { jmp dword ptr [tiny_target] }\n}\n"
                f'HOOK(0x{ea:08X}, {fn}, "{r["module"] or "tiny"}");\n')
    if 'vjmp' in f:
        return (head + f"static __declspec(naked) void {fn}() {{\n    __asm {{\n"
                + ''.join(f"        {i}\n" for i in f['vjmp']) + "    }\n}\n"
                + f'HOOK(0x{ea:08X}, {fn}, "{r["module"] or "tiny"}");\n')
    if 'ret' not in f:
        return None
    pop = f['ret']
    if pop % 4 or pop > 32:
        return None
    params = [f"uint32_t /*a{i + 1}*/" for i in range(pop // 4)]
    ret, body, this = 'void', '', False
    if 'store' in f:
        t, off, op, val = f['store']
        expr = f"CSF_AT({t}, self, {off:#x})"
        body, this = (f"++{expr};" if op == '++' else f"{expr} {op} {val};"), True
    elif 'value' in f:
        kind, val = f['value']
        if kind in ('int', 'int16', 'bool', 'float'):
            ret = {'int': 'int32_t', 'int16': 'int16_t', 'bool': 'uint8_t', 'float': 'float'}[kind]
            body = f"return {val}{'f' if kind == 'float' else ''};"
        elif kind in ('field', 'field8', 'field16', 'fieldf', 'zx8', 'zx16'):
            t = {'field': 'uint32_t', 'field8': 'uint8_t', 'field16': 'uint16_t',
                 'fieldf': 'float', 'zx8': 'uint8_t', 'zx16': 'uint16_t'}[kind]
            ret = 'uint32_t' if kind.startswith('zx') else t
            body, this = f"return CSF_AT({t}, self, {val:#x});", True
        elif kind in ('addr', 'this'):
            ret, this = 'void*', True
            body = 'return self;' if kind == 'this' else f"return static_cast<uint8_t*>(self) {'-' if val < 0 else '+'} {abs(val):#x};"
        else:
            return None
    elif r['cls'] not in ('ret', 'ret N'):
        return None
    keep = regargs.needs_keep_ecx(db, ea)
    if this or keep:
        params = ['void* self', 'void* /*edx*/'] + params
        conv = '__fastcall'
    else:
        conv = '__stdcall' if pop else '__cdecl'
    macro = f'HOOK_KEEP_ECX(0x{ea:08X}, {fn}, {pop // 4},' if keep else f'HOOK(0x{ea:08X}, {fn},'
    return (head + f"static {ret} {conv} {fn}({', '.join(params)}) {{\n    {body}\n}}\n"
            + f'{macro} "{r["module"] or "tiny"}");\n')


def write_sources(db, rows, run_gate):
    import subprocess
    failed = 0
    for r in rows:
        src = generate(db, r)
        if src is None:
            print(f"skip {r['ea']:08x}: unsupported shape or no safe five-byte patch room")
            continue
        path = os.path.join(ROOT, 'src', 'auto', f"{r['ea']:08x}.cpp")
        if os.path.exists(path):
            print(f"skip {r['ea']:08x}: file exists")
            continue
        with open(path, 'x') as out:
            out.write(src)
        if run_gate:
            p = subprocess.run([sys.executable, os.path.join(ROOT, 'tools', 'gate.py'),
                                f"{r['ea']:08x}", path], capture_output=True, text=True, cwd=ROOT)
            if p.returncode:
                os.remove(path)  # only this invocation's newly created candidate
                failed += 1
                print(f"FAIL {r['ea']:08x}: {(p.stdout + p.stderr).strip()}", flush=True)
                continue
        print(f"{'PASS' if run_gate else 'wrote'} {r['ea']:08x}", flush=True)
    return int(bool(failed))


def others_report(db, eas):
    """One line per function: which of select()'s and the wave ranking's conditions keeps it from every lane."""
    import glob, math
    starts = sorted(db.F)
    # Same score as grunt.rank_modules, without importing/running the production driver.
    fanin = collections.Counter()
    for c, f in db.F.items():
        r = db.rows.get(c, {})
        if r.get('region') != 'game':
            continue
        for t in f['callees']:
            tr = db.rows.get(t, {})
            if tr.get('region') == 'game' and tr['module'] != r['module']:
                fanin[t] += 1
    scores = collections.Counter()
    for ea, r in db.rows.items():
        if r['region'] == 'game' and r['status'] == 'todo' and r['module'] and r['module'] != 'stl' and ea in db.F:
            scores[r['module']] += db.F[ea]['size'] * math.log2(2 + fanin[ea])
    ranks = {m: i + 1 for i, (m, _) in enumerate(scores.most_common())}
    owned = {}
    for p in glob.glob(os.path.join(ROOT, "src", "**", "*.cpp"), recursive=True):
        if os.sep + "auto" + os.sep not in p:
            for m in re.findall(r"^HOOK(?:_METHOD|_TRAMP)?\(\s*0x([0-9A-Fa-f]+)", open(p).read(), re.M):
                owned[int(m, 16)] = os.path.relpath(p, ROOT)
    try:
        clusters = json.load(open(os.path.join(ROOT, "export", "clones.json")))
    except OSError:
        clusters = []
    pf = os.path.join(ROOT, "export", "pseudo_failures.txt")
    pfail = open(pf).read() if os.path.exists(pf) else ""
    q = queued()
    for ea in eas:
        r, why = db.rows[ea], []
        f = db.F.get(ea)
        if ea in owned:
            why.append(f"hooked by hand-written {owned[ea]} (select() skips owned addresses; the row was never set done)")
        if not f:
            why.append("no record in export/functions.jsonl (IDA has no function there)")
        else:
            k = bisect.bisect_right(starts, ea)
            gap = (starts[k] if k < len(starts) else ea + 99) - ea
            if gap < JMP:
                why.append(f"next function starts {gap} bytes in (select(): room < 5)")
            if f["size"] != int(r["size"]):
                why.append(f"size {r['size']} in functions.csv, {f['size']} in the export")
        if not os.path.exists(os.path.join(ROOT, "export", "funcs", f"{ea:08x}.c")):
            m = re.search(rf"(?im)^.*{ea:x}.*$", pfail)
            why.append("no pseudocode export/funcs/<ea>.c (select() requires it)"
                       + (f": {m.group(0).strip()[:90]}" if m else ""))
        if not r["module"]:
            why.append("no module (module waves and the ranking never see it)")
        elif ranks.get(r['module'], 0) > 60:
            why.append(f"module rank {ranks[r['module']]}, outside rolling admit()'s top-60 window")
        if os.path.exists(os.path.join(ROOT, "src", "auto", f"{ea:08x}.cpp")):
            why.append("src/auto file exists (written, not yet installed in a passing smoke: the verifier's)")
        for c in clusters if isinstance(clusters, list) else []:
            mem = [int(x, 16) if isinstance(x, str) else x for x in (c.get("members", c) if isinstance(c, dict) else c)]
            if ea in mem:
                st = collections.Counter("queue" if m in q else db.rows.get(m, {}).get("status", "?") for m in mem)
                why.append(f"clone cluster of {len(mem)} ({dict(st)})")
        if not why:
            pend = [c for c in (f.get("callees") or []) if db.rows.get(c, {}).get("status") == "todo"]
            why.append(f"selectable today (passes every select() condition; {len(pend)} todo callees)")
        print(f"{ea:08x} {int(r['size']):6} {r['module'] or '-':9} {r['name'] or '-':40} " + "; ".join(why))


def main():
    ap = argparse.ArgumentParser()
    ap.add_argument("eas", nargs="*")
    ap.add_argument("--list", action="store_true")
    ap.add_argument("--others", action="store_true")
    ap.add_argument("--write", action="store_true")
    ap.add_argument("--gate", action="store_true")
    ap.add_argument("--json", metavar="FILE")
    a = ap.parse_args()
    from packet import DB
    db = DB()
    tiny, others = targets(db)
    if a.others:
        return others_report(db, others)
    eas = [int(x, 16) for x in a.eas] or tiny
    rows, refs = analyze(db, eas)
    if a.json:
        full = []
        for r in rows:
            ea = r['ea']
            full.append(dict(r, bytes=code(ea, r['size']).hex(),
                             calls=sorted(refs.calls[ea]), jumps=sorted(refs.jumps[ea]),
                             vslots=refs.vslots[ea], pointers=refs.ptrs[ea],
                             ignored_string_matches=refs.string_matches[ea]))
        with open(a.json, 'w') as out:
            json.dump(full, out, indent=1)
    if a.write:
        return write_sources(db, rows, a.gate)
    if a.gate:
        ap.error('--gate requires --write')
    if a.list:
        listing(rows)
    else:
        summary(rows, db)


if __name__ == "__main__":
    sys.exit(main() or 0)
