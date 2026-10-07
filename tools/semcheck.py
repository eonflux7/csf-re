#!/usr/bin/env python3
"""Semantic cross-check of a compiled reimplementation against the original function (part of the gate).

Compares absolute addresses referenced by our object code (CSF_FUNC / CSF_GLOBAL / CSF_STR bind by address, so
they appear as immediates/displacements) with the original's references:
  - every direct callee of the original must be referenced           (dropped calls)
  - every string, writable global (.data/.bss) and vtable it references must be referenced
  - we must not reference game addresses the original doesn't (wrong callee / wrong global)
  - .rdata constants (floats etc.) are soft: a literal in our code is fine
  - field displacements we use on non-stack bases must occur in the original (wrong field offset)

Usage: semcheck.py --all         run over every src/auto file, report failures (false-positive survey)
"""
import concurrent.futures as cf, glob, json, os, re, subprocess, sys, tempfile
sys.path.insert(0, os.path.dirname(__file__))
from packet import DB, disasm

ROOT = os.path.dirname(os.path.dirname(os.path.abspath(__file__)))
IMG_LO, IMG_HI = 0x401000, 0x900000
_sect = None
_vt = None


def section_of(addr):
    global _sect
    if _sect is None:
        import pefile
        pe = pefile.PE(os.path.join(ROOT, "work", "pc", "CommXPC.exe"), fast_load=True)
        _sect = [(0x400000 + s.VirtualAddress, 0x400000 + s.VirtualAddress + max(s.Misc_VirtualSize, s.SizeOfRawData),
                  s.Name.rstrip(b"\0").decode()) for s in pe.sections]
    for lo, hi, n in _sect:
        if lo <= addr < hi:
            return n
    return None


def vtables():
    global _vt
    if _vt is None:
        _vt = {int(v["addr"], 16)
               for v in json.load(open(os.path.join(ROOT, "db", "vtables.json")))}
    return _vt


HEX = re.compile(r"0x[0-9a-f]+")
DISP = re.compile(r"\[(e[a-z]{2})\s*([+-])\s*(0x[0-9a-f]+|\d+)\]")


def our_refs(obj):
    """(absolute game addresses, displacements on non-stack bases) used anywhere in the object's code."""
    dis = subprocess.run(["llvm-objdump-21", "-d", "--no-show-raw-insn", "--x86-asm-syntax=intel", obj],
                         capture_output=True, text=True).stdout
    addrs, disps = set(), set()
    skip = False
    for line in dis.splitlines():
        m = re.match(r"^[0-9a-f]+ <(.+)>:$", line)
        if m:  # compiler-generated initializers / hook registrars: their field offsets aren't the function's
            skip = bool(re.search(r"\?\?__E|_GLOBAL__|__dyn_tls|_Init_thread|\?\$TSS|@hook@@", m.group(1)))
            continue
        if "\t" not in line:
            continue
        ins = line.split("\t", 1)[1]
        if ins.startswith(("call", "jmp")) and "<" in ins and "[" not in ins:  # relative: own helpers / imports
            continue
        for h in HEX.findall(ins):
            v = int(h, 16)
            if IMG_LO <= v < IMG_HI:
                addrs.add(v)
        for base, sign, off in ([] if skip else DISP.findall(ins)):  # addresses: all code (CSF_GLOBAL binds in init)
            if base not in ("esp", "ebp") and sign == "+":
                disps.add(int(off, 0))
    # Constant-initialized bindings (CSF_GLOBAL references) are 4-byte pointer variables in .rdata/.data sections.
    # Only whole 4-byte sections and .data are scanned: other .rdata holds strings, whose bytes look like addresses.
    raw = subprocess.run(["llvm-objdump-21", "-s", "-j", ".data", "-j", ".rdata", obj], capture_output=True,
                         text=True).stdout
    secs, cur = [], None
    for line in raw.splitlines():
        if line.startswith("Contents of section"):
            cur = [line.split()[-1].rstrip(":"), bytearray()]
            secs.append(cur)
            continue
        m = re.match(r"^ [0-9a-f]{4,} ((?:[0-9a-f]{2,8} ?){1,4})", line)
        if m and cur:
            cur[1] += bytes.fromhex(m.group(1).replace(" ", ""))
    for name, blob in secs:
        if name == ".rdata" and len(blob) != 4:
            continue
        for i in range(0, len(blob) - 3, 4):
            v = int.from_bytes(blob[i:i + 4], "little")
            if IMG_LO <= v < IMG_HI:
                addrs.add(v)
    return addrs, disps


def code_of(ea, db):
    """Original instructions, following tail jumps into shared code blocks that aren't function starts."""
    f = db.F[ea]
    out = list(disasm(ea, f["size"]))
    for i in list(out):
        if i.mnemonic == "jmp" and i.op_str.startswith("0x"):
            t = int(i.op_str, 16)
            if not (f["ea"] <= t < f["end"]) and t not in db.F:
                for j in disasm(t, 256, linear=True):
                    out.append(j)
                    if j.mnemonic in ("ret", "jmp", "int3"):
                        break
    return out


def our_addr_ofs(obj):
    dis = subprocess.run(["llvm-objdump-21", "-d", "--no-show-raw-insn", "--x86-asm-syntax=intel", obj],
                         capture_output=True, text=True).stdout
    return {int(m.group(1), 0) for m in re.finditer(
        r"\t(?:add\s+(?!esp|ebp)\w+, |lea\s+\w+, \[(?!esp|ebp)\w+ \+ )(0x[0-9a-f]+|\d+)", dis)}


def orig_refs(ea, db):
    imms, disps = set(), set()
    for i in code_of(ea, db):
        for h in HEX.findall(i.op_str):
            v = int(h, 16)
            imms.add(v)
        for base, sign, off in DISP.findall(i.op_str):
            disps.add(int(off, 0))
        for m in re.finditer(r"[+-] (0x[0-9a-f]+|\d+)\]", i.op_str):
            disps.add(int(m.group(1), 0))
    return imms, disps


# Compiler EH/SEH runtime reached from the function's unwind thunk; our compiler emits its own.
EH_HELPER = re.compile(r"CxxFrameHandler|_EH_prolog|_SEH_prolog|_SEH_epilog|_except_handler|__security_check")


def dead_calls(ea, db):
    """Callees reached only behind an always-false unsigned check (`cmp r, -1; jbe skip; call T`: VC7.1 STL's
    max_size() is -1). MSVC keeps the call, any modern compiler folds it away, so it can't be required."""
    ins = code_of(ea, db)
    dead, live = set(), set()
    for k, i in enumerate(ins):
        if i.mnemonic != "call" or not i.op_str.startswith("0x"):
            continue
        t = int(i.op_str, 16)
        if (k >= 2 and ins[k - 1].mnemonic == "jbe" and ins[k - 2].mnemonic == "cmp"
                and ins[k - 2].op_str.endswith(", -1") and ins[k - 1].op_str == hex(i.address + i.size)):
            dead.add(t)
        else:
            live.add(t)
    return dead - live


def check(ea, obj, db):
    f = db.F[ea]
    ours, our_disps = our_refs(obj)
    imms, orig_disps = orig_refs(ea, db)
    own = range(f["ea"], f["end"])
    errs = []
    required = {}
    for c in f["callees"]:
        if c not in own and c not in dead_calls(ea, db) and not EH_HELPER.search(db.name(c) or ""):
            required[c] = f"call to 0x{c:X} ({db.name(c) or 'sub_%X' % c})"
    for s in f.get("srefs", []):
        required[s] = f"string at 0x{s:X}"
    for d in f["drefs"]:
        if d in own or d in required:
            continue
        sec = section_of(d)
        if sec in (".data", "_rwdseg") or d in vtables():
            required[d] = f"{'vtable' if d in vtables() else 'global'} 0x{d:X}"
    for a, what in sorted(required.items()):
        if a not in ours:
            errs.append(f"the original references {what}, your code never does (dropped call/access, or wrong address)")
    known = set(f["callees"]) | set(f.get("srefs", [])) | set(f["drefs"]) | imms
    for a in sorted(ours - known - {ea}):  # own address: the HOOK registration
        if section_of(a) in (".text", ".data", ".rdata", "_rwdseg", "_rwcseg"):
            n = db.name(a) if a in db.F else None
            errs.append(f"your code references 0x{a:X}{' (' + n + ')' if n else ''}, which the original does not")
    # address-of an embedded sub-object (add ecx, N / lea r, [ecx + N]) must stay an address computation in ours
    addr_of = {int(m.group(1), 16) for i in code_of(ea, db)
               for m in [re.match(r"(?:add ecx, |lea \w+, \[ecx \+ )(0x[0-9a-f]+)", f"{i.mnemonic} {i.op_str}")] if m}
    for n in sorted(addr_of):
        if n not in our_addr_ofs(obj):
            errs.append(f"the original computes the ADDRESS this+0x{n:X} (add/lea: an embedded object), your code "
                        f"doesn't — did you load a pointer from +0x{n:X} instead of taking its address?")
    bad = sorted(d for d in our_disps if d >= 8 and d not in orig_disps and d not in imms)
    if bad:
        errs.append("your code accesses field offsets the original never uses: " + ", ".join(f"0x{d:X}" for d in bad))
    return errs


def survey():
    from gate import compile_cmd
    db = DB()
    files = sorted(glob.glob(os.path.join(ROOT, "src", "auto", "*.cpp")))

    def one(p):
        ea = int(os.path.basename(p)[:8], 16)
        with tempfile.TemporaryDirectory() as td:
            obj = os.path.join(td, "t.obj")
            r = subprocess.run(compile_cmd() + [f"/Fo{obj}", "-c", "--", p], capture_output=True, text=True,
                               cwd=os.path.join(ROOT, "build"))
            if r.returncode:
                return ea, ["compile failed"]
            return ea, check(ea, obj, db)

    bad = 0
    with cf.ThreadPoolExecutor(16) as ex:
        for ea, errs in ex.map(one, files):
            if errs:
                bad += 1
                print(f"{ea:08x}: " + " | ".join(errs))
    print(f"{bad}/{len(files)} flagged")


if __name__ == "__main__":
    if "--all" in sys.argv:
        survey()
