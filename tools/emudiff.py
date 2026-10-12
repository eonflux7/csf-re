#!/usr/bin/env python3
"""Emulated differential test (spike): the original function's bytes and our compiled object run side by side in
Unicorn from the same state; what they do is compared. No game, no smoke, no coverage needed.

Both sides start with the same registers, arguments (typed from db/api.json) and memory: the exe image at its
base, everything else mapped on first touch with a deterministic fill (pointers into fresh zones, small ints,
floats, zeros; several profiles, so several paths). A call that leaves the function is not executed: it is an
event (target, arguments, ecx) answered by a mock with a deterministic result, popping what the original's call
site pops (export/calls.json; tools/ida/export_calls.py). CRT helpers both sides use run for real.

Compared per profile: the call events in order, the bytes left at every non-stack address either side wrote,
the return value (eax / st0 by the prototype), esp after the return, ebx/esi/edi/ebp. Floats may differ in the
last bits. A profile where the original faults or runs past the instruction cap is inconclusive, not a pass.

  emudiff.py EA [EA ...] [-v]        one line per function: same | DIFF | inconclusive, with what differed
  emudiff.py --tier diffed [--n 100] functions of an evidence tier (verify.py), summary at the end
  emudiff.py EA --obj FILE.obj       another object for the same address (an old revision, a candidate)
"""
import argparse, collections, csv, gc, json, os, re, struct, sys

ROOT = os.path.dirname(os.path.dirname(os.path.abspath(__file__)))
sys.path.insert(0, os.path.dirname(os.path.abspath(__file__)))
from unicorn import Uc, UcError, UC_ARCH_X86, UC_MODE_32, UC_HOOK_BLOCK, UC_HOOK_MEM_WRITE, UC_HOOK_MEM_UNMAPPED  # noqa: E402
from unicorn import x86_const as X  # noqa: E402

EXE = os.path.join(ROOT, "work", "pc", "CommXPC.exe")
OBJ_DIR = os.path.join(ROOT, "build", "CMakeFiles", "csf_hook.dir", "src", "auto")
IMG = 0x400000
OBJ_BASE = 0x10000000   # our object's sections
EXT_BASE = 0x0F000000   # one address per unresolved external of the object
SYS_BASE = 0x0D000000   # mock return stub, the magic return address, a `ret`, the security cookie
RET_MAGIC, FLD_STUB, RET_STUB, COOKIE, FVAL = SYS_BASE + 0x800, SYS_BASE + 0x10, SYS_BASE + 0x20, SYS_BASE + 0x40, SYS_BASE + 0x50
STACK_LO, STACK_HI = 0x0E000000, 0x0E100000
ESP0 = STACK_HI - 0x8000
TEB = 0x0C000000
ZONES = 0x20000000      # where filled pointers point
INSN_CAP = 300_000
PROFILES = 8  # 6 and 7: every untyped value is a pointer (int-typed arguments and results often are)
# CRT math the object imports where the original has the x87 instruction inline: tiny native stubs, not events
NATIVE = {"_sqrt": "DD442404D9FAC3", "_sin": "DD442404D9FEC3", "_cos": "DD442404D9FFC3", "_fabs": "DD442404D9E1C3",
          "_sqrtf": "D9442404D9FAC3", "_sinf": "D9442404D9FEC3", "_cosf": "D9442404D9FFC3", "_fabsf": "D9442404D9E1C3",
          # the original goes through __CIatan2 & co (CRT, run for real): fpatan / fptan here
          "_atan2": "DD442404DD44240CD9F3C3", "_atan2f": "D9442404D9442408D9F3C3",
          "_atan": "DD442404D9E8D9F3C3", "_atanf": "D9442404D9E8D9F3C3",
          "_tan": "DD442404D9F2DDD8C3", "_tanf": "D9442404D9F2DDD8C3"}
NATIVE_AT = {n: SYS_BASE + 0x100 + 16 * i for i, n in enumerate(NATIVE)}
FLOATS = [0.0, 1.0, 0.5, 2.0, -1.0, 100.0]

BYTE_TYPES = ("bool", "char", "uint8_t", "unsigned char", "int8_t", "signed char", "_BOOL1", "BOOL1")

_cache = {}


def load(name, fn):
    if name not in _cache:
        _cache[name] = fn()
    return _cache[name]


def exe_image():
    def go():
        import pefile
        pe = pefile.PE(EXE, fast_load=True)
        size = (pe.OPTIONAL_HEADER.SizeOfImage + 0xFFF) & ~0xFFF
        img, init = bytearray(size), bytearray(size)  # init: 1 where the file holds the byte (else .bss: filled)
        for s in pe.sections:
            d = s.get_data()[:s.Misc_VirtualSize or s.SizeOfRawData]
            img[s.VirtualAddress:s.VirtualAddress + len(d)] = d
            init[s.VirtualAddress:s.VirtualAddress + len(d)] = b"\1" * len(d)
        runs, a = [], 0  # page runs holding file bytes; pages of pure .bss stay unmapped: filled on first touch
        while a < size:
            if any(init[a:a + 0x1000]):
                b = a
                while b < size and any(init[b:b + 0x1000]):
                    b += 0x1000
                runs.append((a, b))
                a = b
            else:
                a += 0x1000
        return bytes(img), runs
    return load("exe", go)


def functions():
    return load("csv", lambda: {int(r["ea"], 16): r for r in csv.DictReader(open(os.path.join(ROOT, "db", "functions.csv")))})


def crt_ranges():
    """Sorted [(start, end)] of CRT functions: run for real on both sides. Names -> address for the object's externs."""
    def go():
        rs, names = [], {}
        for ea, r in functions().items():
            if r["region"] == "crt":
                rs.append((ea, ea + int(r["size"])))
                names.setdefault(r["name"], ea)
        return sorted(rs), names
    return load("crt", go)


def api():
    return load("api", lambda: json.load(open(os.path.join(ROOT, "db", "api.json"))))


def call_pops():
    return load("calls", lambda: {int(k, 16): v for k, v in json.load(open(os.path.join(ROOT, "export", "calls.json"))).items()})


def split_params(p):
    out, depth, cur = [], 0, ""
    for ch in p or "":
        depth += ch in "(<"
        depth -= ch in ")>"
        if ch == "," and not depth:
            out.append(cur.strip())
            cur = ""
        else:
            cur += ch
    return [x for x in out + [cur.strip()] if x and x != "void"]


def proto(ea):
    """(uses_ecx, [stack arg types], ret type, callee pops) from db/api.json, or None."""
    a = api().get(f"{ea:08x}")
    if not a or not a.get("conv"):
        return None
    ps = [re.sub(r"\s*\w+$", "", x) if not x.endswith("*") else x for x in split_params(a.get("params"))]
    conv = a["conv"]
    ecx = conv in ("__fastcall", "__thiscall")
    stack = ps[2:] if conv == "__fastcall" else ps[1:] if conv == "__thiscall" else ps
    size = sum(8 if t.strip() in ("double", "long long", "int64_t", "uint64_t", "__int64") else 4 for t in stack)
    return ecx, stack, (a.get("ret") or "int").strip(), 0 if conv == "__cdecl" else size


def eax_meaningless(ea):
    """No caller reads eax, or the original leaves it undefined on some path (api.py; the in-game diff's rule)."""
    def go():
        from packet import DB
        return DB()
    try:
        import api as apimod
        db = load("db", go)
        return bool(apimod.eax_undefined_on_return(ea, db) or apimod.ret_unused(ea, db))
    except Exception:
        return False


def returns_al(ea):
    """The original's last write to eax before some `ret` is to al only (setcc al / mov al / xor al, al): a byte
    return whatever the prototype says (0x468880: `or eax, -1; ...; setne al; ret`, typed int)."""
    def go():
        import capstone
        return capstone.Cs(capstone.CS_ARCH_X86, capstone.CS_MODE_32)
    md = load("cs", go)
    img, _ = exe_image()
    for a, b in own_ranges(ea)[0]:
        insns = list(md.disasm(bytes(img[a - IMG:b - IMG]), a))
        for i, ins in enumerate(insns):
            if ins.mnemonic != "ret":
                continue
            for prev in reversed(insns[max(0, i - 8):i]):
                if prev.mnemonic in ("pop", "leave", "add", "mov") and prev.op_str.split(",")[0].strip() in ("esi", "edi", "ebx", "ebp", "esp", ""):
                    continue
                dst = prev.op_str.split(",")[0].strip()
                if prev.mnemonic.startswith(("set", "mov", "xor", "or", "and", "sbb")) and dst == "al":
                    return True
                break
    return False


def h32(x):
    x = (x ^ (x >> 16)) * 0x7FEB352D & 0xFFFFFFFF
    x = (x ^ (x >> 15)) * 0x846CA68B & 0xFFFFFFFF
    return x ^ (x >> 16)


def fill_word(addr, prof):
    """The dword a never-written address holds under a profile. Even profiles lean to pointers, odd ones to ints."""
    h = h32(addr ^ (prof * 0x9E3779B1 & 0xFFFFFFFF))
    sel = h % 10
    ptr_until = 9 if prof == 6 else 5 if prof % 2 == 0 else 2
    if sel < ptr_until:
        return ZONES + ((h >> 4) & 0xFFFF) * 0x1000 + 0x100
    if sel < 7:
        return (h >> 8) % (8 if prof < 4 else 3)
    if sel == 7:
        return 0
    if sel == 8:
        return struct.unpack("<I", struct.pack("<f", FLOATS[(h >> 8) % len(FLOATS)]))[0]
    return h if prof % 2 == 0 else (h >> 8) & 1


def fill_page(base, prof):
    return b"".join(struct.pack("<I", fill_word(base + i, prof)) for i in range(0, 0x1000, 4))


# ---------------------------------------------------------------- our object

class Obj:
    """A COFF i386 object laid out at OBJ_BASE with its relocations applied."""

    def __init__(self, path, hook_name):
        d = open(path, "rb").read()
        nsec, _, symoff, nsym = struct.unpack_from("<HIII", d, 2)
        strtab = symoff + 18 * nsym
        name_of = lambda raw: (d[strtab + struct.unpack("<I", raw[4:])[0]:].split(b"\0", 1)[0] if raw[:4] == b"\0\0\0\0"
                               else raw.rstrip(b"\0")).decode("latin1")
        secs, addr = [], OBJ_BASE
        for i in range(nsec):
            nm, vsz, va, rsz, rptr, relptr, _, nrel, _, ch = struct.unpack_from("<8sIIIIIIHHI", d, 20 + 40 * i)
            nm = nm.rstrip(b"\0").decode()
            if nm.startswith("/"):
                nm = d[strtab + int(nm[1:]):].split(b"\0", 1)[0].decode()
            keep = not (ch & 0x02000A00) and not nm.startswith((".debug", ".llvm", ".drectve", ".CRT"))
            base = None
            if keep:
                base, addr = addr, (addr + max(rsz, 1) + 15) & ~15
            secs.append(dict(name=nm, base=base, size=rsz, data=bytearray(d[rptr:rptr + rsz] if rptr else bytes(rsz)),
                             relptr=relptr, nrel=nrel, code=bool(ch & 0x20)))
        self.end = (addr + 0xFFF) & ~0xFFF
        syms, i = [], 0
        while i < nsym:
            raw, val, sec, _, scl, naux = struct.unpack_from("<8sIhHBB", d, symoff + 18 * i)
            s = dict(name=name_of(raw), value=val, sec=sec, scl=scl)
            if scl == 105 and naux:  # weak external: falls back to another symbol
                s["alias"] = struct.unpack_from("<I", d, symoff + 18 * (i + 1))[0]
            syms.append(s)
            syms.extend([None] * naux)
            i += 1 + naux
        self.ext, self.natives = {}, {}
        crt_names = crt_ranges()[1]

        def resolve(idx):
            s = syms[idx]
            if s["sec"] > 0:
                b = secs[s["sec"] - 1]["base"]
                return None if b is None else b + s["value"]
            if s["sec"] == -1:
                return s["value"]
            if "alias" in s:
                return resolve(s["alias"])
            n = s["name"]
            if n in crt_names:
                return crt_names[n]
            if n in NATIVE_AT:
                return NATIVE_AT[n]
            if n == "___security_cookie":
                return COOKIE
            if n in ("@__security_check_cookie@4", "__fltused", "___CxxFrameHandler3"):
                return RET_STUB
            return self.ext.setdefault(n, EXT_BASE + 16 * len(self.ext))

        for s in secs:
            if s["base"] is None:
                continue
            for j in range(s["nrel"]):
                off, idx, typ = struct.unpack_from("<IIH", d, s["relptr"] + 10 * j)
                tgt = resolve(idx)
                if tgt is None:
                    continue
                cur = struct.unpack_from("<I", s["data"], off)[0]
                if typ == 6:    # DIR32
                    v = cur + tgt
                elif typ == 20:  # REL32
                    v = cur + tgt - (s["base"] + off + 4)
                else:
                    continue
                struct.pack_into("<I", s["data"], off, v & 0xFFFFFFFF)
        self.secs = [s for s in secs if s["base"] is not None]
        self.code = [(s["base"], s["base"] + s["size"]) for s in self.secs if s["code"]]
        cands = [s for s in syms if s and s["sec"] > 0 and re.match(rf"(\?{re.escape(hook_name)}@@|[_@]{re.escape(hook_name)}(@\d+)?$)", s["name"])]
        if not cands:
            raise ValueError(f"no symbol for {hook_name}")
        self.entry = secs[cands[0]["sec"] - 1]["base"] + cands[0]["value"]
        self.ext_names = {v: k for k, v in self.ext.items()}


def hook_name(ea, src=None):
    src = src or os.path.join(ROOT, "src", "auto", f"{ea:08x}.cpp")
    for m in re.finditer(r"\b(HOOK(?:_METHOD|_KEEP_ECX_CDECL|_KEEP_ECX|_REGS|_TRAMP|_TAIL)?)\(\s*0x([0-9A-Fa-f]+)\s*,\s*&?([\w:]+)", open(src).read()):
        if int(m.group(2), 16) == ea:
            return m.group(3) + {"HOOK_KEEP_ECX": "_keep_ecx", "HOOK_KEEP_ECX_CDECL": "_keep_ecx", "HOOK_TAIL": "_tail", "HOOK_REGS": "_regs"}.get(m.group(1), "")
    raise ValueError("no HOOK line")


# ---------------------------------------------------------------- one run

class Run:
    made = 0

    def __init__(self, ea, prof, own, obj=None, script=None):
        """own: [(start, end)] code that is the function itself. script: the original's events (replayed pops)."""
        self.ea, self.prof, self.own, self.obj, self.script = ea, prof, own, obj, script
        self.events, self.written, self.status, self.pending = [], set(), None, None
        self.crt = crt_ranges()[0]
        # unicorn's hook closures refer to themselves and to the Uc: a finished Run (the image mapped, ~10 MB) is only
        # freed by the cyclic collector, which counts objects, not megabytes (8 workers of a sweep took 15 GB)
        Run.made += 1
        if Run.made % 16 == 0:
            gc.collect()
        uc = self.uc = Uc(UC_ARCH_X86, UC_MODE_32)
        img, runs = exe_image()
        for a, b in runs:
            uc.mem_map(IMG + a, b - a)
            uc.mem_write(IMG + a, img[a:b])
        uc.mem_map(SYS_BASE, 0x1000)
        uc.mem_write(FLD_STUB, b"\xD9\x05" + struct.pack("<I", FVAL) + b"\xC3")
        uc.mem_write(RET_STUB, b"\xC3")
        for n, code in NATIVE.items():
            uc.mem_write(NATIVE_AT[n], bytes.fromhex(code))
        uc.mem_write(COOKIE, struct.pack("<I", 0xBB40E64E))
        uc.mem_map(STACK_LO, STACK_HI - STACK_LO)
        uc.mem_write(STACK_LO, b"\xCC" * (STACK_HI - STACK_LO))
        uc.mem_map(TEB, 0x1000)
        uc.mem_write(TEB, struct.pack("<I", 0xFFFFFFFF) + struct.pack("<II", STACK_HI, STACK_LO))
        uc.mem_write(TEB + 0x18, struct.pack("<I", TEB))
        gdt = TEB + 0x800  # flat segments + fs -> TEB (SEH prologues read and write fs:[0])
        desc = lambda base, limit, access, flags: struct.pack(
            "<HHBBBB", limit & 0xFFFF, base & 0xFFFF, (base >> 16) & 0xFF, access, ((limit >> 16) & 0xF) | (flags << 4), base >> 24)
        for i, d in ((14, desc(0, 0xFFFFF, 0xFB, 0xC)), (15, desc(0, 0xFFFFF, 0xF3, 0xC)),
                     (16, desc(TEB, 0xFFF, 0xF3, 0x4)), (17, desc(0, 0xFFFFF, 0x93, 0xC))):
            uc.mem_write(gdt + 8 * i, d)
        uc.reg_write(X.UC_X86_REG_GDTR, (0, gdt, 18 * 8 - 1, 0))
        uc.reg_write(X.UC_X86_REG_SS, 0x88)
        uc.reg_write(X.UC_X86_REG_CS, 0x73)
        for r in (X.UC_X86_REG_DS, X.UC_X86_REG_ES):
            uc.reg_write(r, 0x7B)
        uc.reg_write(X.UC_X86_REG_FS, 0x83)
        if obj:
            uc.mem_map(OBJ_BASE, obj.end - OBJ_BASE)
            for s in obj.secs:
                uc.mem_write(s["base"], bytes(s["data"]))
            uc.mem_map(EXT_BASE, 0x1000)
        self.mapped = set()
        uc.hook_add(UC_HOOK_BLOCK, self.on_block)
        uc.hook_add(UC_HOOK_MEM_WRITE, self.on_write)
        uc.hook_add(UC_HOOK_MEM_UNMAPPED, self.on_unmapped)

    def args(self):
        p = proto(self.ea)
        types = p[1] if p else ["int"] * 4
        vals = []
        for i, t in enumerate(types + ["int"] * 4):  # spare dwords above the real arguments
            h = h32(0xA000 + i + self.prof * 131)
            if "*" in t or "&" in t:
                vals.append(ZONES + 0x08000000 + i * 0x10000 + 0x100)
            elif t.strip() == "float":
                vals.append(struct.unpack("<I", struct.pack("<f", FLOATS[h % len(FLOATS)]))[0])
            elif self.prof >= 6:
                vals.append(ZONES + 0x08000000 + i * 0x10000 + 0x100)
            else:
                vals.append(h % (4 if self.prof < 4 else 2) if self.prof else 1)
        return vals

    def start(self, entry):
        uc = self.uc
        esp = ESP0
        vals = self.args()
        uc.mem_write(esp, struct.pack(f"<{1 + len(vals)}I", RET_MAGIC, *vals))
        for r, v in ((X.UC_X86_REG_ESP, esp), (X.UC_X86_REG_EBP, ESP0 + 0x4000), (X.UC_X86_REG_ECX, ZONES + 0x0A000100),
                     (X.UC_X86_REG_EDX, ZONES + 0x0A100100), (X.UC_X86_REG_EAX, 0), (X.UC_X86_REG_EBX, ZONES + 0x0A200100),
                     (X.UC_X86_REG_ESI, ZONES + 0x0A300100), (X.UC_X86_REG_EDI, ZONES + 0x0A400100)):
            uc.reg_write(r, v)
        self.saved = {r: uc.reg_read(r) for r in (X.UC_X86_REG_EBX, X.UC_X86_REG_ESI, X.UC_X86_REG_EDI, X.UC_X86_REG_EBP)}
        pc, self.first = entry, True
        while self.status is None:
            self.pending = None
            try:
                uc.emu_start(pc, 0xFFFFFFF0, count=INSN_CAP)
            except UcError as e:
                if self.pending is None and self.status is None:
                    self.status = f"fault {e} at {uc.reg_read(X.UC_X86_REG_EIP):08x}"
            if self.status:
                break
            if self.pending is None or len(self.events) >= 2000:
                self.status = "cap"
                break
            pc = self.mock(self.pending)
        self.eax = uc.reg_read(X.UC_X86_REG_EAX)
        self.esp = uc.reg_read(X.UC_X86_REG_ESP)
        try:
            self.st0 = uc.reg_read(X.UC_X86_REG_ST0)
        except Exception:
            self.st0 = None
        self.kept = all(uc.reg_read(r) == v for r, v in self.saved.items())
        return self

    def inside(self, a):
        if any(s <= a < e for s, e in self.own):
            return True
        if SYS_BASE <= a < RET_MAGIC:
            return True
        import bisect
        i = bisect.bisect_right(self.crt, (a, 0xFFFFFFFF)) - 1
        return i >= 0 and self.crt[i][0] <= a < self.crt[i][1]

    def on_block(self, uc, addr, size, _):
        self.blocks += 1
        if addr == RET_MAGIC:
            self.status = "ret"
            uc.emu_stop()
        elif not self.inside(addr) or (addr == self.ea and not self.first and not self.obj):
            self.pending = addr
            uc.emu_stop()
        self.first = False

    def on_write(self, uc, access, addr, size, value, _):
        if not (STACK_LO <= addr < STACK_HI):
            self.written.update(range(addr, addr + size))

    def on_unmapped(self, uc, access, addr, size, value, _):
        base = addr & ~0xFFF
        if access == 21 or addr < 0x10000 or addr >= 0xC0000000:  # fetch from nowhere: a call target (on_block can't see it)
            if access == 21:
                self.pending = addr
            return False
        if base in self.mapped:
            return False
        self.mapped.add(base)
        uc.mem_map(base, 0x1000)
        uc.mem_write(base, fill_page(base, self.prof))
        return True

    def norm(self, v):
        if STACK_LO <= v < ESP0 + 0x40:  # the frame, or the address of an argument (ours may pass a copy's)
            return "stack"
        if OBJ_BASE <= v < OBJ_BASE + 0x100000:
            return "own-data"
        return v

    def mock(self, target):
        """An outside call: record it, answer it. Returns where execution continues."""
        uc = self.uc
        esp = uc.reg_read(X.UC_X86_REG_ESP)
        ret = struct.unpack("<I", uc.mem_read(esp, 4))[0]
        i = len(self.events)
        name = self.obj.ext_names.get(target) if self.obj else None
        p = proto(target) if IMG <= target < IMG + 0x500000 else None
        if self.script is None:  # the original: its call site says what the callee pops
            pop = call_pops().get(self.ret_site.get(ret), p[3] if p else 0) if ret != RET_MAGIC else (p[3] if p else 0)
            nargs = len(p[1]) if p else pop // 4
            use_ecx = p[0] if p else True
            rt = p[2] if p else "?"
        elif i < len(self.script):
            _, pop, nargs, use_ecx, rt = self.script[i][:5]
        else:
            pop, nargs, use_ecx, rt = 0, 0, False, "?"
        argv = struct.unpack(f"<{nargs}I", uc.mem_read(esp + 4, 4 * nargs)) if nargs else ()
        self.events.append((name or target, pop, nargs, use_ecx, rt, tuple(self.norm(a) for a in argv),
                            self.norm(uc.reg_read(X.UC_X86_REG_ECX)) if use_ecx else None))
        h = h32(0xE000 + i * 977 + self.prof * 131)
        if rt in BYTE_TYPES:  # before the pointer profiles: the caller extends a byte, the original keeps eax
            val = h & 1
        elif self.prof >= 6 and rt not in ("float", "double", "long double", "void"):
            val = ZONES + 0x0C000000 + i * 0x10000 + 0x100
        elif "*" in rt or rt == "?":
            val = ZONES + 0x0C000000 + i * 0x10000 + 0x100 if (rt != "?" or self.prof % 2 == 0) else h % 3
        elif rt in ("bool", "char", "uint8_t", "unsigned char", "int8_t"):
            val = h & 1
        else:
            val = h % 4
        uc.reg_write(X.UC_X86_REG_EAX, val)
        self.last_mock = (val, rt)
        if rt in ("float", "double", "long double"):
            uc.mem_write(FVAL, struct.pack("<f", FLOATS[h % len(FLOATS)]))
            uc.mem_write(esp + pop, struct.pack("<I", ret))
            uc.reg_write(X.UC_X86_REG_ESP, esp + pop)
            return FLD_STUB
        uc.reg_write(X.UC_X86_REG_ESP, esp + 4 + pop)
        if ret == RET_MAGIC:
            self.status = "ret"
            self.tail = True
        return ret

    tail = False
    ret_site = {}
    last_mock = None
    blocks = 0


def st0_float(v):
    if v is None:
        return None
    if isinstance(v, tuple):
        mant, exp = v
        if mant == 0:
            return 0.0
        sign = -1.0 if exp & 0x8000 else 1.0
        return sign * (mant / (1 << 63)) * 2.0 ** ((exp & 0x7FFF) - 16383)
    return float(v)


def tn(x):
    return x if isinstance(x, str) else f"{x:08x}"


def close(a, b):
    return a == b or (a is not None and b is not None and abs(a - b) <= 1e-5 * max(1.0, abs(a), abs(b)))


def same_args(eo, em):
    """Stack arguments of one call, original vs ours. A byte or short parameter is compared in its width (the
    original pushes a dword whose upper bytes are whatever its frame held: 0x512400 `mov edx, [ebp-4]; push edx`
    for a char); bytes of the original's value still holding the never-written stack fill (0xCC) are not
    compared (a frame dword passed on before it is written: 0x4696C0, 0x47EE50)."""
    if eo[5] == em[5]:
        return True
    if len(eo[5]) != len(em[5]):
        return False
    p = proto(eo[0]) if isinstance(eo[0], int) else None
    types = [t.strip() for t in p[1]] if p else []
    for i, (a, b) in enumerate(zip(eo[5], em[5])):
        if a == b:
            continue
        if isinstance(a, str) or isinstance(b, str):
            return False
        t = types[i] if i < len(types) else ""
        mask = 0xFF if t in BYTE_TYPES else 0xFFFF if t in ("short", "uint16_t", "int16_t", "unsigned short") else 0xFFFFFFFF
        for k in range(4):
            if (a >> 8 * k) & 0xFF == 0xCC:
                mask &= ~(0xFF << 8 * k)
        if (a ^ b) & mask:
            return False
    return True


def compare(ea, o, m):
    """[difference, ...] between the original's run and ours (empty: same)."""
    out = []
    for i, (eo, em) in enumerate(zip(o.events, m.events)):
        if eo[0] != em[0]:
            out.append(f"call {i}: target {tn(eo[0])} vs {tn(em[0])}")
            return out
        if not same_args(eo, em):
            out.append(f"call {i} to {tn(eo[0])}: arguments {[tn(x) for x in eo[5]]} vs {[tn(x) for x in em[5]]}")
            return out
        if eo[6] != em[6]:
            out.append(f"call {i} to {tn(eo[0])}: ecx {tn(eo[6])} vs {tn(em[6])}")
            return out
    if len(o.events) != len(m.events):
        out.append(f"{len(o.events)} calls vs {len(m.events)}")
        return out
    if m.status != "ret":
        if m.status == "cap" and o.blocks > 5000:
            # a long loop over filled memory the original just finished under the instruction cap (0x4A35D0: a
            # count from garbage; ours takes a few instructions more per round)
            return ["?ours runs past the cap after a long run of the original"]
        return [f"ours does not return ({m.status})"]
    if o.esp != m.esp and not (o.tail or m.tail):
        out.append(f"esp after return {o.esp - ESP0:+d} vs {m.esp - ESP0:+d}")
    if not m.kept and o.kept:
        out.append("ours changes ebx/esi/edi/ebp")
    p = proto(ea)
    rt = p[2] if p else "int"
    if rt in ("float", "double", "long double"):
        fo, fm = st0_float(o.st0), st0_float(m.st0)
        # the emulator loads a denormal float (a small int read as a float) with a wrong exponent: 0x401FA0 got
        # 1.47e19 from (4, 0, 5) read as floats. A result no game value has says nothing either way
        absurd = lambda v: v is not None and (v != v or abs(v) > 1e15 or 0 < abs(v) < 1e-30)
        if not close(fo, fm) and not absurd(fo) and not absurd(fm):
            out.append(f"st0 {fo} vs {fm}")
    elif rt != "void" and not (o.tail or m.tail) and not eax_meaningless(ea):
        mask = 0xFF if rt in BYTE_TYPES or returns_al(ea) else \
            0xFFFF if rt in ("short", "uint16_t", "int16_t", "unsigned short") else 0xFFFFFFFF
        # eax still holds what the last callee left, and that callee returns a byte, a float or nothing while this
        # function is typed wider: a leftover, not a return value (0x525A50, 0x6CAF00: Hex-Rays says int)
        lm = o.last_mock
        left = mask == 0xFFFFFFFF and lm and lm[0] == o.eax and lm[1] in BYTE_TYPES + ("void", "float", "double", "long double")
        if not left and o.norm(o.eax & mask) != m.norm(m.eax & mask):
            out.append(f"returns {o.eax & mask:x} vs {m.eax & mask:x}")
    skip = lambda a: TEB <= a < TEB + 0x1000 or OBJ_BASE <= a < OBJ_BASE + 0x100000 or SYS_BASE <= a < SYS_BASE + 0x1000
    addrs = sorted(a for a in o.written | m.written if not skip(a))
    bad = []
    for a in addrs:
        try:
            bo, bm = o.uc.mem_read(a, 1), m.uc.mem_read(a, 1)
        except UcError:
            bad.append(a)
            continue
        if bo != bm:
            bad.append(a)
    groups = sorted({a & ~3 for a in bad})
    real = []
    for g in groups:
        try:
            fo, fm = struct.unpack("<f", o.uc.mem_read(g, 4))[0], struct.unpack("<f", m.uc.mem_read(g, 4))[0]
            wo, wm = struct.unpack("<I", o.uc.mem_read(g, 4))[0], struct.unpack("<I", m.uc.mem_read(g, 4))[0]
        except UcError:
            real.append((g, None, None))
            continue
        if o.norm(wo) == m.norm(wm) and isinstance(o.norm(wo), str):
            continue  # a pointer into each side's own frame / data
        if fo == fo and fm == fm and abs(wo - wm) < 64 and close(fo, fm):
            continue  # last bits of a float
        real.append((g, wo, wm))
    if real:
        g, wo, wm = real[0]
        out.append(f"{len(real)} dword(s) of memory differ, first at {g:08x}: "
                   + (f"{wo:08x} vs {wm:08x}" if wo is not None else "written by one side only"))
    return out


def own_ranges(ea):
    p = os.path.join(ROOT, "export", "disasm", f"{ea:08x}.json")
    if os.path.exists(p):
        d = json.load(open(p))
        insns = d["insns"]
        return [tuple(c) for c in d["chunks"]], {b: a for a, b in zip(insns, insns[1:])}
    r = functions()[ea]
    return [(ea, ea + int(r["size"]))], {}


def check(ea, obj_path=None, src=None, profiles=PROFILES, verbose=False):
    """-> (verdict, detail): same | DIFF | inconclusive | error."""
    tmp = None
    if src and not obj_path:  # a source without an object: compile it as the hook build does
        import subprocess, tempfile, gate
        tmp = obj_path = tempfile.mktemp(suffix=".obj")
        r = subprocess.run(gate.compile_cmd() + [f"/Fo{tmp}", "-c", "--", os.path.abspath(src)], capture_output=True,
                           text=True, cwd=os.path.join(ROOT, "build"))
        if r.returncode:
            return "error", "compile: " + (r.stdout + r.stderr).strip()[:200]
    try:
        obj = Obj(obj_path or os.path.join(OBJ_DIR, f"{ea:08x}.cpp.obj"), hook_name(ea, src))
    except (OSError, ValueError, struct.error) as e:
        return "error", str(e)
    finally:
        if tmp and os.path.exists(tmp):
            os.remove(tmp)
    own, ret_site = own_ranges(ea)
    done, diffs, why = 0, [], collections.Counter()
    for prof in range(profiles):
        o = Run(ea, prof, own)
        o.ret_site = ret_site
        o.start(ea)
        if o.status != "ret":
            why[o.status.split(" at ")[0][:40]] += 1
            continue
        m = Run(ea, prof, obj.code, obj, o.events).start(obj.entry)
        d = compare(ea, o, m)
        if d and d[0].startswith("?"):
            why[d[0][1:41]] += 1
            continue
        done += 1
        if verbose:
            print(f"  profile {prof}: {len(o.events)} calls, {len(o.written)} bytes written: {'; '.join(d) or 'same'}")
        if d:
            diffs.append(f"profile {prof}: {d[0]}")
    if diffs:
        return "DIFF", f"{len(diffs)}/{done} profiles; {diffs[0]}"
    if not done:
        return "inconclusive", ", ".join(f"{k} x{v}" for k, v in why.most_common())
    return "same", f"{done}/{profiles} profiles"


def main():
    ap = argparse.ArgumentParser(description=__doc__, formatter_class=argparse.RawDescriptionHelpFormatter)
    ap.add_argument("eas", nargs="*")
    ap.add_argument("--tier", help="functions of this evidence tier (diffed, executed, gate)")
    ap.add_argument("--n", type=int, default=0, help="with --tier: at most N (every k-th)")
    ap.add_argument("--obj")
    ap.add_argument("--src")
    ap.add_argument("-v", "--verbose", action="store_true")
    ap.add_argument("--quiet", action="store_true", help="only DIFF lines and the summary")
    a = ap.parse_args()
    eas = [int(x, 16) for x in a.eas]
    if a.tier:
        import verify
        eas = sorted(int(ea, 16) for ea, t in verify.tiers().items() if t == a.tier)
        if a.n and len(eas) > a.n:
            eas = eas[::len(eas) // a.n][:a.n]
    tally = collections.Counter()
    for ea in eas:
        try:
            v, detail = check(ea, a.obj, a.src, verbose=a.verbose)
        except Exception as e:  # spike: a tool error is a result too
            v, detail = "error", f"{type(e).__name__}: {e}"
        tally[v] += 1
        if not a.quiet or v in ("DIFF", "error"):
            print(f"{ea:08x} {v:12s} {detail}", flush=True)
    if len(eas) > 1:
        print("summary: " + ", ".join(f"{k} {v}" for k, v in tally.most_common()) + f" of {len(eas)}")


if __name__ == "__main__":
    main()
