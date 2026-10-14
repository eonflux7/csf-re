#!/usr/bin/env python3
"""Emulated differential test (spike): the original function's bytes and our compiled object run side by side in
Unicorn from the same state; what they do is compared. No game, no smoke, no coverage needed.

Both sides start with the same registers, arguments (typed from db/api.json) and memory: the exe image at its
base, everything else mapped on first touch with a deterministic fill (pointers into fresh zones, small ints,
floats, zeros; several profiles, so several paths). A call that leaves the function is not executed: it is an
event (target, arguments, ecx) answered by a mock with a deterministic result, popping what the original's call
site pops (export/calls.json; tools/ida/export_calls.py). CRT helpers both sides use run for real,
except process callback registration (atexit), which is an exact-argument event.

Compared per profile: the call events in order, the bytes left at every non-stack address either side wrote,
the return value (eax / st0 by the prototype), esp after the return, ebx/esi/edi/ebp. Floats may differ in the
last bits. A profile where the original faults or runs past the instruction cap is inconclusive, not a pass.

  emudiff.py EA [EA ...] [-v]        one line per function: same | DIFF | inconclusive, with what differed
  emudiff.py --tier diffed [--n 100] functions of an evidence tier (verify.py), summary at the end
  emudiff.py EA --obj FILE.obj       another object for the same address (an old revision, a candidate)
"""
import argparse, collections, csv, functools, gc, json, os, re, struct, sys

ROOT = os.path.dirname(os.path.dirname(os.path.abspath(__file__)))
sys.path.insert(0, os.path.dirname(os.path.abspath(__file__)))
from unicorn import Uc, UcError, UC_ARCH_X86, UC_MODE_32, UC_HOOK_BLOCK, UC_HOOK_CODE, UC_HOOK_MEM_WRITE, UC_HOOK_MEM_UNMAPPED  # noqa: E402
from unicorn import x86_const as X  # noqa: E402

EXE = os.path.join(ROOT, "work", "pc", "CommXPC.exe")
OBJ_DIR = os.path.join(ROOT, "build", "CMakeFiles", "csf_hook.dir", "src", "auto")
IMG = 0x400000
OBJ_BASE = 0x50000000   # our object's sections (outside the common wrapped profile pointers)
EXT_BASE = 0x0F000000   # one address per unresolved external of the object
SYS_BASE = 0x0D000000   # mock return stub, the magic return address, a `ret`, the security cookie
RET_MAGIC, FLD_STUB, RET_STUB, COOKIE, FVAL = SYS_BASE + 0x800, SYS_BASE + 0x10, SYS_BASE + 0x20, SYS_BASE + 0x40, SYS_BASE + 0x50
STACK_LO, STACK_HI = 0x0E000000, 0x0E100000
ESP0 = STACK_HI - 0x8000
TEB = 0x0C000000
ZONES = 0x20000000      # where filled pointers point
INSN_CAP = 300_000
RUN_SECS = 5.0          # wall clock per run
PROFILES = 8  # 6 and 7: every untyped value is a pointer (int-typed arguments and results often are)
# CRT math the object imports where the original has the x87 instruction inline: tiny native stubs, not events
NATIVE = {"_sqrt": "DD442404D9FAC3", "_sin": "DD442404D9FEC3", "_cos": "DD442404D9FFC3", "_fabs": "DD442404D9E1C3",
          "_sqrtf": "D9442404D9FAC3", "_sinf": "D9442404D9FEC3", "_cosf": "D9442404D9FFC3", "_fabsf": "D9442404D9E1C3",
          # the original goes through __CIatan2 & co (CRT, run for real): fpatan / fptan here
          "_atan2": "DD442404DD44240CD9F3C3", "_atan2f": "D9442404D9442408D9F3C3",
          "_atan": "DD442404D9E8D9F3C3", "_atanf": "D9442404D9E8D9F3C3",
          "_tan": "DD442404D9F2DDD8C3", "_tanf": "D9442404D9F2DDD8C3",
          "_log2": "D9E8DD442404D9F1C3"}  # fld1; fld double argument; fyl2x
NATIVE_AT = {n: SYS_BASE + 0x100 + 16 * i for i, n in enumerate(NATIVE)}
FLOATS = [0.0, 1.0, 0.5, 2.0, -1.0, 100.0]

GARBAGE_HI = {(ZONES + 0x0A000100 + k * 0x100000) >> 16 for k in (1, 2, 3, 4)}  # initial edx, ebx, esi, edi
SAVED_REGS = {ZONES + 0x0A000100 + k * 0x100000 for k in (2, 3, 4)}  # initial ebx, esi, edi
ECX0 = ZONES + 0x0A000100
EAX0 = ZONES + 0x0A500100  # a pointer: a few functions take an argument in eax (0x6A0540, zlib's static helpers)
BYTE_TYPES = ("bool", "char", "uint8_t", "unsigned char", "int8_t", "signed char", "_BOOL1", "BOOL1",
              "std::uint8_t", "std::int8_t")

_cache = {}
import threading  # noqa: E402
_tls = threading.local()  # the gate runs checks from lane threads: the last DIFF's call sequences, per thread


def load(name, fn):
    if name not in _cache:
        _cache[name] = fn()
    return _cache[name]


# what the Win32 calls of the CRT's heap pop (the CRT runs for real and has no call sites in export/calls.json)
IMPORT_POPS = {"HeapAlloc": 12, "HeapFree": 12, "HeapReAlloc": 16, "HeapSize": 12,
               "RaiseException": 16, "ExitProcess": 4, "ExitThread": 4, "FatalExit": 4}


def imports():
    """{what an import-table slot holds in the file: "DLL!name"}. Nothing binds the slots here, so a
    `call [__imp_x]` on either side arrives at that value (timeGetTime: 0x496C5E, the middle of a game function)."""
    def go():
        import pefile
        pe = pefile.PE(EXE, fast_load=True)
        pe.parse_data_directories(directories=[pefile.DIRECTORY_ENTRY["IMAGE_DIRECTORY_ENTRY_IMPORT"]])
        out = {}
        for d in pe.DIRECTORY_ENTRY_IMPORT:
            for i in d.imports:
                raw = struct.unpack("<I", pe.get_data(i.address - IMG, 4))[0]
                out[raw] = f"{d.dll.decode()}!{(i.name or b'#%d' % (i.ordinal or 0)).decode()}"
        return out
    return load("imports", go)


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


@functools.lru_cache(maxsize=1)
def crt_events():
    """Process callback registration is a boundary, not a pure CRT helper.

    Running atexit's onexit table/locks/heap over profile-fill is not a model
    of initialized CRT state. Record the registration itself on either side.
    Key by CRT symbol, not by the initializer being checked.
    """
    conventions = {'_atexit': (0, 1, False, 'int'),
                   '__CxxThrowException@8': (8, 2, False, 'void'),
                   '_abort': (0, 0, False, 'void'), '_exit': (0, 1, False, 'void'),
                   '__exit': (0, 1, False, 'void'), '__Exit': (0, 1, False, 'void')}
    return {ea: conventions[name] for name, ea in crt_ranges()[1].items() if name in conventions}


def no_return(target, argv):
    """Only documented nonreturning boundaries, not every exception/API call.

    RaiseException with zero flags can be resumed by a handler. The C++ throw
    helper raises EXCEPTION_NONCONTINUABLE; no normal continuation is possible.
    """
    if isinstance(target, int):
        name = functions().get(target, {}).get('name', '')
    else:
        name = target.split('!')[-1]
    name = re.sub(r'@\d+$', '', name)
    return name in ('__CxxThrowException', '_abort', '_exit', '__exit', '__Exit',
                    'ExitProcess', 'ExitThread', 'FatalExit') or (
                        name == 'RaiseException' and len(argv) > 1 and argv[1] & 1)


def api():
    return load("api", lambda: json.load(open(os.path.join(ROOT, "db", "api.json"))))


def call_pops():
    return load("calls", lambda: {int(k, 16): v for k, v in json.load(open(os.path.join(ROOT, "export", "calls.json"))).items()})



def infer_stack_pops(ins, direct_pops, entry=None, known=None, groups=None, domains=None, strict=False,
                     clean_frame=False, switches=None):
    """Prove call cleanup from the original's CFG, without consulting the candidate.

    SP and outstanding pushed arguments are affine expressions in unknown indirect-call pops.
    Saved-register restores and CFG joins give equalities. Calls cannot consume locals or saved
    registers, so outstanding arguments must remain nonnegative. Only uniquely determined indirect
    counts replace the export; ambiguous and unsupported frames retain the exported counts.
    """
    import capstone
    known, groups, domains = known or {}, groups or {}, domains or {}
    switches = switches or {}
    failure = None if strict else {}
    if not ins:
        return failure
    regs = ('ebx', 'esi', 'edi', 'ebp')
    popregs = {i.op_str for i in ins.values() if i.mnemonic == 'pop'}
    if any(i.mnemonic == 'leave' for i in ins.values()):
        popregs.add('ebp')  # LEAVE restores the frame register just like POP EBP.
    aliases = {'bl': 'ebx', 'bh': 'ebx', 'bx': 'ebx', 'si': 'esi', 'di': 'edi', 'bp': 'ebp'}

    # Affine expression {None: constant, call-site: coefficient}.
    def plus(a, b, n=1):
        result = a.copy()
        for k, v in b.items():
            result[k] = result.get(k, 0) + n * v
            if not result[k]:
                del result[k]
        return result

    equalities, nonnegative, pops, variables, seen = [], [], {}, set(), {}
    sites = {}
    # MSVC reserves small EBP-framed locals with PUSH ECX, not SUB ESP.
    # Only the contiguous reservation immediately following MOV EBP,ESP
    # qualifies; later pushes of ECX are outgoing arguments.
    local_pushes = set()
    for pc, i in ins.items():
        if i.mnemonic == 'mov' and i.op_str == 'ebp, esp':
            p = pc + i.size
            while p in ins and ins[p].mnemonic == 'push' and ins[p].op_str == 'ecx':
                local_pushes.add(p)
                p += ins[p].size
    todo = [(entry if entry is not None else min(ins), ({}, {}, None, {}, frozenset(regs)))]

    def equal(a, b):
        equalities.append(plus(a, b, -1))

    def outgoing_reservation(pc, size):
        """SUB ESP,N whose entire range is filled as a by-value call argument.

        Track copies of ESP while scanning only a straight-line sequence to the
        next call. A local/dynamic allocation or partial initialization is not a
        proof. PUSH may move ESP; a saved alias still points at the reservation.
        """
        aliases, covered, shift = {}, set(), 0
        pc += ins[pc].size
        for _ in range(64):
            j = ins.get(pc)
            if j is None:
                return False
            m, parts = j.mnemonic, j.op_str.split(', ')
            if m == 'call':
                return covered == set(range(size))
            if m.startswith('j') or m in ('ret', 'pop', 'leave') or parts[0] == 'esp':
                return False
            if m == 'push':
                shift -= 4
            elif m == 'mov' and len(parts) == 2 and parts[1] == 'esp':
                aliases[parts[0]] = shift
            else:
                match = re.fullmatch(r'(byte|word|dword|qword) ptr \[(e[a-z]{2})(?: \+ (0x[0-9a-f]+|\d+))?\]', parts[0])
                if match and m in ('mov', 'fst', 'fstp'):
                    base = shift if match[2] == 'esp' else aliases.get(match[2])
                    if base is not None:
                        off = base + int(match[3] or '0', 0)
                        width = {'byte': 1, 'word': 2, 'dword': 4, 'qword': 8}[match[1]]
                        covered.update(x for x in range(off, off + width) if 0 <= x < size)
                aliases.pop(parts[0], None)
            pc += j.size
        return False

    while todo:
        pc, state = todo.pop()
        sp, args, bp, saves, pristine = state
        if pc not in ins:
            return failure  # Unknown control flow: do not infer from a partial function.
        if pc in seen:
            old = seen[pc]
            if (set(old[3]) != set(saves) or old[4] - old[3].keys() != pristine - saves.keys()
                    or (old[2] is None) != (bp is None)):
                return failure  # Incompatible frame/register histories.
            if bp is not None:
                equal(bp, old[2])
            equal(sp, old[0])
            equal(args, old[1])
            for r in saves:
                equal(saves[r], old[3][r])
            continue
        seen[pc] = state
        i = ins[pc]
        mn, op, nextpc = i.mnemonic, i.op_str, pc + i.size
        sp, args, saves = sp.copy(), args.copy(), saves.copy()
        if mn == 'push':
            sp = plus(sp, {None: -4})
            if pc in local_pushes:
                pass
            elif op in pristine and op in popregs and op not in saves:
                equal(args, {})
                saves[op] = sp.copy()
            else:
                args = plus(args, {None: 4})
        elif mn == 'pop':
            if op in saves:
                equal(sp, saves.pop(op))
                equal(args, {})
                pristine = pristine | {op}
            else:
                args = plus(args, {None: -4})
                nonnegative.append(args)
                pristine = pristine - {op}
            sp = plus(sp, {None: 4})
        elif mn in ('sub', 'add') and op.startswith('esp, '):
            try:
                n = int(op.split(', ')[1], 0) * (1 if mn == 'add' else -1)
            except ValueError:
                return failure
            sp = plus(sp, {None: n})
            if n < 0:
                # Initial frame allocation is local storage, rather than outgoing arguments.
                if variables or args:
                    if not outgoing_reservation(pc, -n):
                        return failure
                    args = plus(args, {None: -n})
            else:
                args = plus(args, {None: -n})
                nonnegative.append(args)
        elif mn == 'mov' and op == 'ebp, esp':
            bp = sp.copy()
        elif mn == 'mov' and op == 'esp, ebp':
            if bp is None:
                return failure
            if clean_frame:
                equal(args, {})
            sp, args = bp.copy(), {}
        elif mn == 'leave':
            if bp is None or 'ebp' not in saves:
                return failure
            equal(bp, saves.pop('ebp'))
            if clean_frame:
                equal(args, {})
            sp, args = plus(bp, {None: 4}), {}
        elif mn == 'lea' and op == 'esp, [esp]':
            pass  # MSVC alignment NOP, not a dynamic stack adjustment.
        elif mn == 'call':
            n = known.get(pc)
            if n is None and i.operands[0].type == capstone.x86.X86_OP_IMM:
                n = direct_pops(pc, i.operands[0].imm)
            if n is not None:
                pops[pc] = n
                sp, args = plus(sp, {None: n}), plus(args, {None: -n})
            else:
                key = groups.get(pc, pc)
                sites[pc] = key
                variables.add(key)
                sp, args = plus(sp, {key: 1}), plus(args, {key: -1})
            nonnegative.append(args)
        elif mn == 'ret' or (mn == 'jmp' and i.operands[0].type == capstone.x86.X86_OP_IMM
                            and i.operands[0].imm not in ins) or (mn == 'jmp'
                            and i.operands[0].type == capstone.x86.X86_OP_MEM and not i.operands[0].mem.index):
            equal(sp, {})
            continue
        elif mn == 'jmp' and i.operands[0].type != capstone.x86.X86_OP_IMM:
            if pc not in switches or not switches[pc] or any(t not in ins for t in switches[pc]):
                return failure  # Unresolved switch: do not infer from a partial function.
        elif any(i.reg_name(r) == 'esp' for r in i.regs_access()[1]):
            return failure  # Dynamic/aligned frames and unmodelled stack instructions.
        if mn not in ('push', 'call', 'pop'):
            _, written = i.regs_access()
            pristine -= frozenset(aliases.get(i.reg_name(r), i.reg_name(r)) for r in written)
        state = (sp, args, bp, saves, pristine)
        if mn == 'jmp':
            if pc in switches:
                todo.extend((target, state) for target in switches[pc])
            else:
                todo.append((i.operands[0].imm, state))
        elif mn.startswith('j'):
            todo.extend([(i.operands[0].imm, state), (nextpc, state)])
        else:
            todo.append((nextpc, state))

    bounds = {v: [min(domains[v]), max(domains[v])] if domains.get(v) else [0, 1 << 30]
              for v in variables}
    inequalities = nonnegative + equalities + [{k: -v for k, v in x.items()} for x in equalities]
    for _ in range(len(bounds) * 4 + 20):
        changed = False
        for exp in inequalities:
            maximum = exp.get(None, 0) + sum(c * bounds[k][1 if c > 0 else 0]
                                            for k, c in exp.items() if k is not None)
            if maximum < 0:
                return failure
            for k, c in exp.items():
                if k is None:
                    continue
                other = maximum - c * bounds[k][1 if c > 0 else 0]
                idx = 0 if c > 0 else 1
                n = (-other + c - 1) // c if c > 0 else other // -c
                n = (n + 3) // 4 * 4 if c > 0 else n // 4 * 4
                if domains.get(k):
                    possible = [x for x in domains[k] if x >= n] if c > 0 else [x for x in domains[k] if x <= n]
                    if not possible:
                        return failure
                    n = min(possible) if c > 0 else max(possible)
                if (c > 0 and n > bounds[k][0]) or (c < 0 and n < bounds[k][1]):
                    bounds[k][idx] = n
                    changed = True
                if bounds[k][0] > bounds[k][1]:
                    return failure
        if not changed:
            break
    pops.update({site: bounds[key][0] for site, key in sites.items()
                 if bounds[key][0] == bounds[key][1]})
    return pops


@functools.lru_cache(maxsize=None)
def binary_call_pop(target):
    """Use the callee's actual RET immediate, rather than a decompiler's call-site assignment."""
    if target not in functions():
        return None
    from packet import DB
    from gate import effective_rets
    counts = set(effective_rets(target, load('db', DB)))
    return next(iter(counts)) if len(counts) == 1 else None


@functools.lru_cache(maxsize=None)
def stack_call_pops(ea, constraints_only=False):
    import capstone
    md = capstone.Cs(capstone.CS_ARCH_X86, capstone.CS_MODE_32)
    md.detail = True
    img, _ = exe_image()
    ins = {}
    for lo, hi in own_ranges(ea)[0]:
        ins.update((i.address, i) for i in md.disasm(bytes(img[lo - IMG:hi - IMG]), lo))
    evidence = dispatch_evidence(ea)
    known = {pc: n for pc, v in evidence.items() if 'target' in v
             and (n := dispatch_call_pop(v['target'], v.get('receiver'))) is not None}
    groups = {pc: v['group'] for pc, v in evidence.items() if 'group' in v}
    direct = lambda site, target: dispatch_call_pop(target, evidence.get(site, {}).get('receiver'))
    # A prototype may constrain an ambiguous caller, but must cover the exact
    # machine dispatch sequence and satisfy the *whole* original stack CFG.
    typed = dict(typed_indirect_calls(ea))
    typed.update(global_callback_types(ea))
    # Same slot does not imply same cleanup when a receiver is used through
    # different prototypes (e.g. optional argument at 535F60's slot 5C).
    for pc in list(groups):
        if pc in typed:
            groups[pc] = (groups[pc], typed[pc]['pop'])
    switches = binary_switches(ea)
    solve = lambda *args, **kw: infer_stack_pops(*args, switches=switches, **kw)
    initial = solve(ins, direct, ea, known, groups)
    fixed = dict(known)
    fixed.update(initial)  # Binary/unique caller facts outrank typed metadata.
    fixed.update({pc: v['pop'] for pc, v in typed.items() if v['pop'] is not None and pc not in fixed})
    if typed and solve(ins, direct, ea, fixed, groups, strict=True, clean_frame=True) is not None:
        known = fixed
        initial = solve(ins, direct, ea, known, groups)
    if constraints_only:
        return initial
    domains, patterns = virtual_pop_domains(evidence)
    for receiver, choices in patterns.items():
        sites = {pc: group for pc, group in groups.items() if group[0] == receiver}
        if all(pc in initial for pc in sites) or len(choices) > 128:
            continue
        possible = []
        for choice in choices:
            fixed = dict(known)
            fixed.update({pc: choice[group] for pc, group in sites.items()})
            if solve(ins, direct, ea, fixed, groups, domains, strict=True) is not None:
                possible.append(choice)
        if possible:
            for group in sites.values():
                domains[group] = {p[group] for p in possible}
    refined = solve(ins, direct, ea, known, groups, domains)
    # The vtable inventory can be incomplete (notably library receivers). It
    # may bound an otherwise ambiguous call, but cannot invalidate or replace
    # a cleanup already proved from the original caller without that inventory.
    refined.update(initial)
    return refined


@functools.lru_cache(maxsize=None)
def original_stack_code(ea):
    import capstone
    md = capstone.Cs(capstone.CS_ARCH_X86, capstone.CS_MODE_32)
    md.detail = True
    img, _ = exe_image()
    return {i.address: i for lo, hi in own_ranges(ea)[0]
            for i in md.disasm(img[lo - IMG:hi - IMG], lo)}


def register_parent(reg):
    """A partial GPR write invalidates provenance of the whole pointer."""
    return {'al': 'eax', 'ah': 'eax', 'ax': 'eax',
            'bl': 'ebx', 'bh': 'ebx', 'bx': 'ebx',
            'cl': 'ecx', 'ch': 'ecx', 'cx': 'ecx',
            'dl': 'edx', 'dh': 'edx', 'dx': 'edx',
            'si': 'esi', 'di': 'edi', 'bp': 'ebp', 'sp': 'esp'}.get(reg, reg)


def profile_stack_consistent(ea, pops):
    return infer_stack_pops(original_stack_code(ea), lambda pc, t: binary_call_pop(t),
                            ea, pops, strict=True, switches=binary_switches(ea)) is not None


@functools.lru_cache(maxsize=None)
def binary_switches(ea):
    """Bounded MSVC switch tables, validated from CMP/JA and image bytes.

    Support direct dword tables and byte-remapped tables. No guessed table
    length or IDA-only edge: the unsigned bound covers every possible index.
    Unknown forms still abort stack inference.
    """
    import capstone
    ins = list(original_stack_code(ea).values())
    heads = {i.address for i in ins}
    img, _ = exe_image()
    out, guards = {}, {}
    for k, i in enumerate(ins):
        if i.mnemonic != 'jmp' or not i.operands or i.operands[0].type != capstone.x86.X86_OP_MEM:
            continue
        mem = i.operands[0].mem
        if mem.base or not mem.index or mem.scale != 4 or not IMG <= mem.disp < IMG + len(img):
            continue
        idx, bound_reg, lookup = i.reg_name(mem.index), i.reg_name(mem.index), None
        n = k - 1
        if n >= 0 and ins[n].mnemonic == 'movzx' and ins[n].op_str.split(', ')[0] == idx:
            op = ins[n].operands[1]
            if op.type != capstone.x86.X86_OP_MEM or op.size != 1 or op.mem.index or not op.mem.base:
                continue
            lookup, bound_reg = op.mem.disp, ins[n].reg_name(op.mem.base)
            n -= 1
        if n < 1 or ins[n].mnemonic != 'ja' or ins[n-1].mnemonic != 'cmp':
            continue
        chain = ins[n-1:k+1]
        if any(a.address + a.size != b.address for a, b in zip(chain, chain[1:])):
            continue
        cmp = ins[n-1]
        if cmp.op_str.split(', ')[0] != bound_reg or cmp.operands[1].type != capstone.x86.X86_OP_IMM:
            continue
        limit = cmp.operands[1].imm
        if not 0 <= limit <= 4096 or ins[n].operands[0].imm not in heads:
            continue
        indices = range(limit + 1)
        if lookup is not None:
            if not IMG <= lookup or lookup + limit + 1 > IMG + len(img):
                continue
            indices = set(img[lookup - IMG:lookup - IMG + limit + 1])
        if mem.disp + 4 * (max(indices) + 1) > IMG + len(img):
            continue
        targets = {struct.unpack_from('<I', img, mem.disp - IMG + 4 * j)[0] for j in indices}
        if targets and targets <= heads:
            out[i.address] = targets
            guards[i.address] = cmp.address
    # Syntactic adjacency is not dominance. A direct edge or another table
    # case entering after CMP can bypass the bound (or its unsigned JA).
    incoming = {ea} | {i.operands[0].imm for i in ins
                      if (i.mnemonic.startswith('j') or i.mnemonic.startswith('loop'))
                      and i.operands and i.operands[0].type == capstone.x86.X86_OP_IMM}
    incoming.update(t for targets in out.values() for t in targets)
    return {pc: targets for pc, targets in out.items()
            if not any(guards[pc] < t <= pc for t in incoming)}


def virtual_pop_domains(evidence):
    """Conservative RET domains for unresolved original virtual receivers.

    Keep every exported vtable supporting *all* slots used on the same receiver.
    Do not select a class or a most-common RET. A missing/ambiguous callee RET
    disables the bound. Domains alone never choose cleanup: original caller
    stack constraints must leave exactly one value. Try the slot correlations
    of each supporting table, then retain the union of feasible RET values;
    never select the first or most common class.
    """
    tables = load('vtables', lambda: json.load(open(os.path.join(ROOT, 'db', 'vtables.json'))))
    slots = collections.defaultdict(set)
    allowed = {}
    for ev in evidence.values():
        if ev.get('virtual'):
            slots[ev['group'][0]].add(ev['slot'])
            if 'tables' in ev:
                allowed[ev['group'][0]] = set(ev['tables'])
    domains, patterns = {}, {}
    for receiver, offsets in slots.items():
        candidates = [v for v in tables if len(v['slots']) * 4 > max(offsets)
                      and (receiver not in allowed or int(v['addr'], 16) in allowed[receiver])]
        choices = []
        for v in candidates:
            choice = {(receiver, slot): binary_call_pop(int(v['slots'][slot // 4], 16)) for slot in offsets}
            if None in choice.values():
                choices = []
                break  # an unknown target is not evidence to exclude its table
            if choice not in choices:
                choices.append(choice)
        if choices:
            patterns[receiver] = choices
        for slot in offsets:
            counts = {binary_call_pop(int(v['slots'][slot // 4], 16)) for v in candidates}
            if counts and None not in counts:
                domains[(receiver, slot)] = counts
    return domains, patterns


@functools.lru_cache(maxsize=None)
def dispatch_evidence(ea):
    from packet import DB
    from stackabi import call_evidence
    return call_evidence(ea, load('db', DB))


@functools.lru_cache(maxsize=None)
def dispatch_call_pop(target, receiver=None):
    pop = binary_call_pop(target)
    if pop is not None or not receiver:
        return pop
    from packet import DB, disasm
    from stackabi import call_evidence
    db = load('db', DB)
    f = db.F.get(target)
    if not f:
        return None
    ev = call_evidence(target, db, receiver=receiver)
    tails = [i for i in disasm(target, f['size']) if i.mnemonic == 'jmp'
             and not i.op_str.startswith('0x')]
    counts = {binary_call_pop(ev[i.address]['target']) if i.address in ev and 'target' in ev[i.address]
              else None for i in tails}
    # The tail's RET belongs to this caller only if the forwarding body hands it
    # the original caller frame, not an unbalanced/unsupported stack transform.
    import capstone
    md = capstone.Cs(capstone.CS_ARCH_X86, capstone.CS_MODE_32)
    md.detail = True
    code = {i.address: next(md.disasm(bytes(i.bytes), i.address)) for i in disasm(target, f['size'])}
    known = {pc: n for pc, fact in ev.items() if 'target' in fact
             and (n := binary_call_pop(fact['target'])) is not None}
    if infer_stack_pops(code, lambda pc, t: binary_call_pop(t), target, known, strict=True) is None:
        return None
    return next(iter(counts)) if len(counts) == 1 and None not in counts else None


@functools.lru_cache(maxsize=None)
def resolved_call_facts(ea):
    """RET evidence > unique original-stack solution > legacy solver > IDA delta."""
    facts = {pc: dict(v, source='stack') for pc, v in indirect_facts(ea).items()}
    for pc, pop in stack_call_pops(ea).items():
        if pc in facts and facts[pc]['pop'] != pop:
            facts[pc].setdefault('disagreements', []).append(f"legacy stack = {facts[pc]['pop']}")
            facts[pc].pop('nargs', None)
        facts.setdefault(pc, {}).update(pop=pop, source='stack')
    for pc, ev in dispatch_evidence(ea).items():
        if 'target' not in ev:
            continue
        n = dispatch_call_pop(ev['target'], ev.get('receiver'))
        if n is not None:
            if pc in facts and facts[pc]['pop'] != n:
                facts[pc].setdefault('disagreements', []).append(f"caller stack = {facts[pc]['pop']}")
                facts[pc].pop('nargs', None)
            kind = 'forwarded RET' if binary_call_pop(ev['target']) is None else 'RET'
            facts.setdefault(pc, {}).update(pop=n, source=f"callee {ev['target']:08x} {kind}")
    return facts


@functools.lru_cache(maxsize=None)
def import_call_pop(ret, name):
    """Import cleanup from a known ABI, or stack constraints in its original caller.

    CRT calls are absent from export/calls.json and the hook's ret_site map. Find
    the enclosing original function, never infer cleanup from candidate code.
    None means ambiguous, not a zero-argument stdcall.
    """
    decorated = re.search(r'@(\d+)$', name.split('!')[-1])
    if decorated:
        return int(decorated.group(1))
    known = IMPORT_POPS.get(name.split('!')[-1])
    if known is not None:
        return known
    import bisect, capstone
    starts = load('starts', lambda: sorted(functions()))
    k = bisect.bisect_right(starts, ret - 1) - 1
    if k < 0:
        return None
    ea = starts[k]
    img, _ = exe_image()
    md = capstone.Cs(capstone.CS_ARCH_X86, capstone.CS_MODE_32)
    for lo, hi in own_ranges(ea)[0]:
        if lo <= ret - 1 < hi:
            for ins in md.disasm(img[lo - IMG:hi - IMG], lo):
                if ins.address + ins.size == ret and ins.mnemonic == 'call':
                    return stack_call_pops(ea).get(ins.address)
    return None


@functools.lru_cache(maxsize=None)
def indirect_facts(ea):
    from packet import DB
    from stackabi import indirect_facts as derive
    return derive(ea, load("db", DB))


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


@functools.lru_cache(maxsize=None)
def eax_meaningless(ea):
    """The original leaves eax undefined on some path; unused is not undefined.

    Caller usage does not change a function's observable return contract.
    """
    def go():
        from packet import DB
        return DB()
    try:
        import api as apimod
        db = load("db", go)
        return bool(apimod.eax_undefined_on_return(ea, db))
    except Exception:
        return False


@functools.lru_cache(maxsize=None)
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


@functools.lru_cache(maxsize=None)
def callsite_abi(ea, site):
    """Convention evidence for an unresolved indirect call, never the candidate.

    A uniform set of IDA function-pointer declarations/casts supplies a
    stack-only convention; mixed/untyped functions remain unknown. Callback
    arguments have a more precise proof: CALL [EBP+offset] names the typed
    argument directly. Unknown or mixed conventions retain ECX comparison.
    """
    import capstone
    md = capstone.Cs(capstone.CS_ARCH_X86, capstone.CS_MODE_32)
    img, _ = exe_image()
    ins = [i for lo, hi in own_ranges(ea)[0] for i in md.disasm(img[lo-IMG:hi-IMG], lo)]
    calls = [i for i in ins if i.mnemonic == 'call' and not i.op_str.startswith('0x')]
    call = next((i for i in calls if i.address == site), None)
    if call is None:
        return None
    callback = global_callback_types(ea).get(site)
    if callback:
        return callback['ecx']
    path = os.path.join(ROOT, 'export', 'funcs', f'{ea:08x}.c')
    if os.path.exists(path):
        with open(path) as f:
            text = f.read()
        casts = re.findall(r'\(__(cdecl|stdcall|thiscall|fastcall)\s*\*', text)
        if casts and len(set(casts)) == 1 and casts[0] in ('cdecl', 'stdcall'):
            return False
    # Function-pointer parameters are not casts in the decompilation.
    m = re.fullmatch(r'dword ptr \[ebp \+ (0x[0-9a-f]+|\d+)\]', call.op_str)
    if m:
        abi = load('abi', lambda: json.load(open(os.path.join(ROOT, 'export', 'abi.json'))))
        args = [t for t, _, loc in abi.get(f'{ea:08x}', {}).get('args', []) if loc == 'stack']
        k = (int(m[1], 0) - 8) // 4
        if 0 <= k < len(args) and re.search(r'\(__(?:cdecl|stdcall)\s*\*\)', args[k]):
            return False
    return None


@functools.lru_cache(maxsize=None)
def global_callback_types(ea):
    """Pair named callback-table casts with exact original global loads/slots.

    Mixed virtual and cdecl dispatches cannot use the old whole-function
    convention heuristic. This proof requires a straight-line reaching MOV
    from a named global and matching table offset in the typed export.
    """
    import yaml
    path = os.path.join(ROOT, 'export', 'funcs', f'{ea:08x}.c')
    if not os.path.exists(path):
        return {}
    with open(path) as stream:
        source = stream.read()
    pattern = r'\(__(cdecl|stdcall)\s*\*\*\)\(([^()]*)\)\)\s*(\w+)\s*\+\s*(\d+)'
    matches = list(re.finditer(pattern, source))
    if not matches:
        return {}
    def names():
        with open(os.path.join(ROOT, 'db', 'globals.yaml')) as stream:
            return {v['name']: v['addr'] for v in yaml.safe_load(stream)}
    named = load('global_names', names)
    declarations = {}
    for match in matches:
        conv, params, name, slot = match.groups()
        if name not in named:
            continue
        types = split_params(params)
        width = sum(8 if t in ('double', '__int64') else 4 for t in types)
        fact = dict(ecx=False, nargs=width // 4, pop=0 if conv == 'cdecl' else width)
        key = named[name], int(slot) * 4
        if key in declarations and declarations[key] != fact:
            declarations[key] = None
        else:
            declarations[key] = fact
    result, registers = {}, {}
    code = original_stack_code(ea)
    import capstone
    switches = binary_switches(ea)
    if any(i.mnemonic == 'jmp' and i.operands[0].type != capstone.x86.X86_OP_IMM
           and i.address not in switches for i in code.values()):
        return {}  # unknown computed edges can bypass the claimed global load
    leaders = {i.operands[0].imm for i in code.values()
               if (i.mnemonic.startswith('j') or i.mnemonic.startswith('loop'))
               and i.operands and i.operands[0].type == capstone.x86.X86_OP_IMM}
    leaders.update(t for targets in switches.values() for t in targets)
    previous_end = None
    for pc, ins in sorted(code.items()):
        if pc in leaders or previous_end != pc:
            registers.clear()  # no reaching-load claim across a join/chunk gap
        previous_end = pc + ins.size
        if ins.mnemonic == 'call':
            match = re.fullmatch(r'dword ptr \[(e[a-z]{2})(?: \+ (0x[0-9a-f]+|\d+))?\]', ins.op_str)
            if match:
                key = registers.get(match[1]), int(match[2] or '0', 0)
                if declarations.get(key):
                    result[pc] = declarations[key]
            for reg in ('eax', 'ecx', 'edx'):
                registers.pop(reg, None)
            continue
        if ins.mnemonic.startswith(('j', 'loop')) or ins.mnemonic in ('ret', 'leave'):
            registers.clear()
            continue
        parts = ins.op_str.split(', ')
        value = None
        if ins.mnemonic == 'mov' and len(parts) == 2:
            match = re.fullmatch(r'dword ptr \[(0x[0-9a-f]+)\]', parts[1])
            value = int(match[1], 16) if match else registers.get(parts[1])
        for reg in ins.regs_access()[1]:
            registers.pop(register_parent(ins.reg_name(reg)), None)
        if value is not None:
            registers[parts[0]] = value
    return result


@functools.lru_cache(maxsize=None)
def typed_indirect_calls(ea):
    """Typed casts paired with the exact original sequence of vtable offsets.

    Refuse partial/mixed syntax and reordered dispatches; this is metadata,
    not RET evidence. stack_call_pops additionally requires CFG consistency.
    """
    import capstone
    path = os.path.join(ROOT, 'export', 'funcs', f'{ea:08x}.c')
    if not os.path.exists(path):
        return {}
    with open(path) as f:
        source = f.read()
    casts = []
    pattern = r'\(__(cdecl|stdcall|thiscall|fastcall)\s*\*\*\)\(([^()]*)\)\)\('
    for m in re.finditer(pattern, source):
        start, end, depth = m.end(), m.end(), 1
        while end < len(source) and depth:
            depth += (source[end] == '(') - (source[end] == ')')
            end += 1
        expr = source[start:end-1].strip()
        offset = re.search(r'\+\s*(0x[\da-fA-F]+|\d+)\s*$', expr)
        slot = int(offset[1], 0) if offset else 0
        params = split_params(m[2])
        conv = m[1]
        stack = params[1:] if conv == 'thiscall' else params[2:] if conv == 'fastcall' else params
        pop = 0 if conv == 'cdecl' else sum(8 if t.strip() in ('double', '__int64', 'long long') else 4 for t in stack)
        casts.append(dict(slot=slot, pop=pop, ecx=conv in ('thiscall', 'fastcall'), types=stack))
    md = capstone.Cs(capstone.CS_ARCH_X86, capstone.CS_MODE_32)
    img, _ = exe_image()
    calls = [i for lo, hi in own_ranges(ea)[0] for i in md.disasm(img[lo-IMG:hi-IMG], lo)
             if i.mnemonic == 'call' and not i.op_str.startswith('0x')]
    if len(casts) != len(calls) or not casts:
        return {}
    result = {}
    for i, v in zip(calls, casts):
        m = re.fullmatch(r'dword ptr \[e[a-z]{2}(?: \+ (0x[\da-f]+|\d+))?\]', i.op_str)
        if not m or int(m[1] or '0', 0) != v['slot']:
            return {}
        result[i.address] = v
    return result


@functools.lru_cache(maxsize=None)
def callback_return_type(ea, site):
    """Return type of a CALL through a typed EBP stack parameter."""
    from packet import disasm
    ins = next((i for lo, hi in own_ranges(ea)[0] for i in disasm(lo, hi-lo) if i.address == site), None)
    m = re.fullmatch(r'dword ptr \[ebp \+ (0x[0-9a-f]+|\d+)\]', ins.op_str) if ins else None
    if not m:
        return None
    abi = load('abi', lambda: json.load(open(os.path.join(ROOT, 'export', 'abi.json'))))
    args = [t for t, _, loc in abi.get(f'{ea:08x}', {}).get('args', []) if loc == 'stack']
    k = (int(m[1], 0) - 8) // 4
    if 0 <= k < len(args):
        match = re.match(r'(.+?)\s*\(__\w+\s*\*\)', args[k])
        if match:
            t = match[1].strip()
            return 'char' if t in ('unsigned __int8', '__int8') else t
    return None


@functools.lru_cache(maxsize=None)
def indirect_float_returns(ea):
    """Uniform, explicitly floating-point casts covering every indirect call.

    Require the exact machine slot sequence as well as complete typed-export
    coverage. This supplies ST0 to a mock, not a rounding exemption. Mixed,
    missing or differently ordered casts provide no evidence.
    """
    path = os.path.join(ROOT, 'export', 'funcs', f'{ea:08x}.c')
    if not os.path.exists(path):
        return {}
    with open(path) as stream:
        source = stream.read()
    casts = list(re.finditer(r'\b([\w]+(?:\s+\w+)?)\s+\(__(?:thiscall|cdecl|stdcall|fastcall)\s*\*\*?\)\([^()]*\)\)', source))
    calls = [i for i in original_stack_code(ea).values() if i.mnemonic == 'call' and not i.op_str.startswith('0x')]
    if not casts or len(casts) != len(calls) or any(m[1] not in ('float', 'double', 'long double') for m in casts):
        return {}
    result = {}
    for k, (cast, call) in enumerate(zip(casts, calls)):
        end = casts[k + 1].start() if k + 1 < len(casts) else len(source)
        expr = source[cast.end():end].split(';', 1)[0]
        slot = re.search(r'\+\s*(0x[\da-fA-F]+|\d+)\)\)', expr)
        machine = re.fullmatch(r'dword ptr \[e[a-z]{2} \+ (0x[\da-f]+|\d+)\]', call.op_str)
        if not slot or not machine or int(slot[1], 0) != int(machine[1], 0):
            return {}
        result[call.address] = cast[1]
    return result


@functools.lru_cache(maxsize=None)
def named_vslot_types(ea, site):
    """An explicit typed vSlotXX cast supplies parameter *widths*, not cleanup."""
    from packet import disasm
    ins = next((i for lo, hi in own_ranges(ea)[0] for i in disasm(lo, hi-lo) if i.address == site), None)
    m = re.fullmatch(r'dword ptr \[e[a-z]{2} \+ (0x[\da-f]+|\d+)\]', ins.op_str) if ins else None
    path = os.path.join(ROOT, 'export', 'funcs', f'{ea:08x}.c')
    if not m or not os.path.exists(path):
        return []
    with open(path) as f:
        source = f.read()
    slot = int(m[1], 0)
    choices = []
    for cast in re.finditer(r'\(__(thiscall|stdcall|cdecl|fastcall)\s*\*\)\(([^()]*)\)\)\w+->vSlot([\da-fA-F]+)', source):
        if int(cast[3], 16) == slot:
            params = split_params(cast[2])
            choices.append(params[1:] if cast[1] == 'thiscall' else params[2:] if cast[1] == 'fastcall' else params)
    return choices[0] if choices and all(p == choices[0] for p in choices) else []


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
        # profiles 2 and 7: the pointer's low byte has flag bits set (and bit 0, so a byte test and a bool test
        # agree), or a field that is a pointer in every profile never passes `test byte ptr [x], 0x20`
        return ZONES + ((h >> 4) & 0xFFFF) * 0x1000 + (0x1F5 if prof in (2, 7) else 0x100)
    if sel < 7:
        if prof == 5:  # the flags profile: any byte, so `test byte ptr [x], 0x20` goes both ways (0x6B31E0)
            return (h >> 8) & 0xFF
        return (h >> 8) % (8 if prof < 4 else 3)
    if sel == 7:
        return 0
    if sel == 8:
        return struct.unpack("<I", struct.pack("<f", FLOATS[(h >> 8) % len(FLOATS)]))[0]
    return h if prof % 2 == 0 else (h >> 8) & 1


def fill_page(base, prof):
    return b"".join(struct.pack("<I", fill_word(base + i, prof)) for i in range(0, 0x1000, 4))


def mock_outputs(target, index, prof, argv, ecx, result):
    """Narrow, instruction-audited boundary output contracts.

    These are writes, not comparison exemptions. Each side receives identical
    event/profile-dependent values at its own output addresses. Initial output
    contents are irrelevant only for these proven overwrites. No blanket
    pointer-argument dirtying (many pointers are input-only).
    """
    ptr = ZONES + 0x0D000000 + index * 0x10000 + 0x100
    word = lambda n: struct.pack('<I', n)
    if target == 0x4DDF80 and result & 0xFF:
        # Success stores [arg2] and [arg3]: 4DDFD9 / 4DDFF7.
        return [(argv[k], word((prof + index) % 3)) for k in (2, 3)]
    if target == 0x4DE230:
        # Four bounds through the two coordinate-conversion calls at 4DE27F
        # and 4DE296 after ComputeFloatExtents; model completed output writes.
        return [(argv[k], word((prof + index + (k > 2)) % 2)) for k in range(1, 5)]
    if target == 0x6AC000:
        # Six bounds initialized through the six output pointers before
        # traversal, 6AC0C3..6AC0E8; never consume prior output values.
        return [(a, struct.pack('<f', -1.0 if k % 2 == 0 else 1.0)) for k, a in enumerate(argv)]
    if target in (0x655E40, 0x655F00):
        # ReadCounterWord/ReadDword pass arg0 and size 2/4 to 6581C0.
        return [(argv[0], word(1 + (prof + index) % 7)[:2 if target == 0x655E40 else 4])]
    if target == 0x64A6B0:
        # String construction delegates to 64A630, which overwrites [arg0]
        # at 64A677 (and on its null path); arg0 here is ECX.
        return [(ecx, word(ptr))]
    if target == 0x6622F0:
        # Both FindProperty exits write arg0: 662343 and 66235A.
        return [(argv[0], word(ptr))]
    if target == 0x7234F0:
        # Plain copy stores 7234FC..72351D and 723542/723549. The last word
        # may reuse a call return slot. Do not fill string padding: its
        # constructor initializes only length/capacity and a terminator.
        return [(ecx, b''.join(word(ptr + k * 4) for k in range(6))),
                (ecx + 52, word(ptr + 52) + word(ptr + 56)),
                (ecx + 44, word(0) + word(7)), (ecx + 28, b'\0\0')]
    return []


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
        self.ext, self.natives, self.external_calls = {}, {}, {}
        crt_names = crt_ranges()[1]
        # i386 clang-cl emits __chkstk for large frames / dynamic alloca; VC7.1 names the same
        # allocating stack-probe ABI __alloca_probe (eax = bytes, helper adjusts esp). Run the
        # original CRT helper, just as on the original side: mocking it leaves esp unchanged
        # and adds a spurious call event. This is not the probe-only x64 __chkstk ABI.
        crt_aliases = {"__chkstk": "__alloca_probe", "_chkstk": "__alloca_probe"}

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
            if n in crt_aliases and crt_aliases[n] in crt_names:
                return crt_names[crt_aliases[n]]
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
                    # Only a CALL relocation makes an address an external symbol.
                    # An arbitrary indirect target can coincide with our stub range.
                    if s["code"] and off and s["data"][off - 1] == 0xE8:
                        name = next((n for n, a in self.ext.items() if a == tgt), None)
                        if name:
                            self.external_calls[s["base"] + off + 4] = (tgt, name)
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

    def __init__(self, ea, prof, own, obj=None, script=None, memory_obj=None, graph=None):
        """own: [(start, end)] code that is the function itself. script: the original's events (replayed pops)."""
        self.ea, self.prof, self.own, self.obj, self.script = ea, prof, own, obj, script
        self.graph = graph
        self.events, self.written, self.status, self.pending = [], set(), None, None
        self.float_written = set()
        self.reserved_done = {}
        self.proven_pops = resolved_call_facts(ea) if not obj and ea in functions() else {}
        self.crt = crt_ranges()[0]
        # unicorn's hook closures refer to themselves and to the Uc: a finished Run (the image mapped, ~10 MB) is only
        # freed by the cyclic collector, which counts objects, not megabytes (8 workers of a sweep took 15 GB)
        Run.made += 1
        if Run.made % 16 == 0:
            gc.collect()
        uc = self.uc = Uc(UC_ARCH_X86, UC_MODE_32)
        # Unicorn reports CW=0 at reset although its arithmetic initially uses
        # reset precision. FNSTCW/FLDCW would activate that spurious zero.
        # Windows CRT startup selects 53-bit precision: __fpmath -> 7B388C
        # calls __controlfp(_PC_53=0x10000, _MCW_PC=0x30000). Mask exceptions,
        # round to nearest. An explicit later FLDCW (e.g. 82C404) still runs.
        uc.reg_write(X.UC_X86_REG_FPCW, 0x027F)
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
        # __getptd (per-thread CRT data: rand's seed, errno, strtok) reaches TLS imports: a fixed block instead
        # (0x6F98F0 called rand, and the "calls" that followed were the CRT's own, into nowhere)
        ptd = crt_ranges()[1].get("__getptd")
        if ptd:
            uc.mem_write(ptd, b"\xB8" + struct.pack("<I", SYS_BASE + 0x400) + b"\xC3")
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
        # Numeric pointers from mocks can wrap into the object's address range.
        # Mirror its bytes in the original run too: moving it merely relocates
        # the collision and cannot prevent arbitrary 32-bit pointer arithmetic.
        image_obj = obj or memory_obj
        if image_obj:
            uc.mem_map(OBJ_BASE, image_obj.end - OBJ_BASE)
            for s in image_obj.secs:
                uc.mem_write(s["base"], bytes(s["data"]))
        if obj:
            uc.mem_map(EXT_BASE, 0x1000)
        self.mapped = set()
        if graph:
            # Seed before either function runs, never change a faulting state
            # into a successful state mid-execution. Existing image pages get
            # only the explicit input-field overrides, not zero-filled code.
            for base in sorted(graph.pages):
                try:
                    uc.mem_read(base, 1)
                except UcError:
                    uc.mem_map(base, 0x1000)
                    uc.mem_write(base, graph.page(base))
                    self.mapped.add(base)
            for address, value in graph.words.items():
                uc.mem_write(address, struct.pack('<I', value))
        if not obj:
            # `push ebp; mov ebp, esp; push ecx [; push ecx]` reserves locals: the slot holds ecx until written, and
            # the original may pass it on unwritten (0x516C50 forwards [ebp-4] as a 4th argument). It becomes
            # stack fill, which same_args does not compare
            raw = bytes(img[ea - IMG:ea - IMG + 5])
            n = 2 if raw == b"\x55\x8B\xEC\x51\x51" else 1 if raw[:4] == b"\x55\x8B\xEC\x51" else 0
            pr = proto(ea)
            if not pr or pr[0]:  # a __thiscall keeps `this` in that slot on purpose (0x4A45D0 returns it from there)
                n = 0
            for k in range(n):
                uc.hook_add(UC_HOOK_CODE, self.on_reserved, begin=ea + 4 + k, end=ea + 4 + k)
        # rdtsc is the host's clock here: the two sides would never agree (0x69CB30). A counter instead, set after
        # the instruction; a 0F 31 that is part of another instruction has no instruction start behind it
        self.tsc = 0
        for lo, hi in self.own:
            code = bytes(uc.mem_read(lo, hi - lo))
            k = code.find(b"\x0F\x31")
            while k >= 0:
                uc.hook_add(UC_HOOK_CODE, self.on_tsc, begin=lo + k + 2, end=lo + k + 2)
                k = code.find(b"\x0F\x31", k + 2)
        uc.hook_add(UC_HOOK_BLOCK, self.on_block)
        uc.hook_add(UC_HOOK_MEM_WRITE, self.on_write)
        uc.hook_add(UC_HOOK_MEM_UNMAPPED, self.on_unmapped)
        self.eax_width = 32
        self.indirect_sites = set()
        self.eax_defs = {}
        if not obj:
            import capstone
            md = capstone.Cs(capstone.CS_ARCH_X86, capstone.CS_MODE_32)
            md.detail = True
            for lo, hi in own:
                for ins in md.disasm(bytes(uc.mem_read(lo, hi-lo)), lo):
                    if ins.mnemonic == 'call':
                        self.eax_defs[ins.address] = 32  # callee's full mocked result
                        if ins.operands[0].type != capstone.x86.X86_OP_IMM:
                            self.indirect_sites.add(ins.address)
                    elif ins.mnemonic not in ('cmp', 'test', 'push'):
                        written = {ins.reg_name(r) for r in ins.regs_access()[1]}
                        for reg, width in (('eax', 32), ('ax', 16), ('al', 8)):
                            if reg in written:
                                self.eax_defs[ins.address] = width
                                break
        self.eax_def_sites = sorted(self.eax_defs)

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
        if self.graph:
            for k, v in self.graph.args.items():
                if k < len(vals):
                    vals[k] = v
        uc.mem_write(esp, struct.pack(f"<{1 + len(vals)}I", RET_MAGIC, *vals))
        for r, v in ((X.UC_X86_REG_ESP, esp), (X.UC_X86_REG_EBP, ESP0 + 0x4000), (X.UC_X86_REG_ECX, ZONES + 0x0A000100),
                     (X.UC_X86_REG_EDX, ZONES + 0x0A100100), (X.UC_X86_REG_EAX, EAX0), (X.UC_X86_REG_EBX, ZONES + 0x0A200100),
                     (X.UC_X86_REG_ESI, ZONES + 0x0A300100), (X.UC_X86_REG_EDI, ZONES + 0x0A400100)):
            uc.reg_write(r, v)
        self.saved = {r: uc.reg_read(r) for r in (X.UC_X86_REG_EBX, X.UC_X86_REG_ESI, X.UC_X86_REG_EDI, X.UC_X86_REG_EBP)}
        pc, self.first = entry, True
        import time
        t_end = time.time() + RUN_SECS
        while self.status is None:
            self.pending = None
            try:
                # the timeout too: one `rep movs` with a count from the fill is one instruction and 2^29 writes
                uc.emu_start(pc, 0xFFFFFFF0, timeout=int(max(0.1, t_end - time.time()) * 1e6), count=INSN_CAP)
            except UcError as e:
                if self.pending is None and self.status is None:
                    self.status = f"fault {e} at {uc.reg_read(X.UC_X86_REG_EIP):08x}"
            if self.status:
                break
            if time.time() >= t_end:
                self.status = "cap"
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
        if a in crt_events():
            return False
        return self.in_crt(a)

    def in_crt(self, a):
        import bisect
        i = bisect.bisect_right(self.crt, (a, 0xFFFFFFFF)) - 1
        return i >= 0 and self.crt[i][0] <= a < self.crt[i][1]

    def on_block(self, uc, addr, size, _):
        self.blocks += 1
        # A basic block cannot branch around a definition within it. Record
        # its last EAX definition without a Python hook on every instruction;
        # faults/caps mid-block are never compared as completed original runs.
        if not self.obj and self.eax_def_sites:
            import bisect
            k = bisect.bisect_left(self.eax_def_sites, addr + size) - 1
            if k >= 0 and self.eax_def_sites[k] >= addr:
                self.eax_width = self.eax_defs[self.eax_def_sites[k]]
        if addr == RET_MAGIC:
            self.status = "ret"
            uc.emu_stop()
        elif not self.obj and self.in_crt(addr) and addr not in crt_events():
            ret = struct.unpack('<I', uc.mem_read(uc.reg_read(X.UC_X86_REG_ESP), 4))[0]
            site = self.ret_site.get(ret)
            if site in self.indirect_sites and dispatch_evidence(self.ea).get(site, {}).get('target') != addr:
                self.status = 'synthetic indirect CRT target'
                uc.emu_stop()
        elif not self.inside(addr) or (not self.first and addr == (self.obj.entry if self.obj else self.ea)):
            self.pending = self.ea if self.obj and addr == self.obj.entry else addr
            uc.emu_stop()
        self.first = False

    def on_tsc(self, uc, addr, size, _):
        self.tsc += 1
        uc.reg_write(X.UC_X86_REG_EAX, h32(0x7500 + self.tsc * 31 + self.prof * 131))
        uc.reg_write(X.UC_X86_REG_EDX, self.tsc)

    def on_reserved(self, uc, addr, size, _):
        if not self.reserved_done.get(addr):  # the first pass through the prologue only (a recursive call re-enters)
            self.reserved_done[addr] = True
            uc.mem_write(uc.reg_read(X.UC_X86_REG_ESP), b"\xCC" * 4)

    def on_write(self, uc, access, addr, size, value, _):
        if not (STACK_LO <= addr < STACK_HI):
            self.written.update(range(addr, addr + size))
            if not self.obj:
                pc = uc.reg_read(X.UC_X86_REG_EIP)
                if self.is_float_store(pc):
                    self.float_written.update(range(addr, addr + size))
                else:
                    self.float_written.difference_update(range(addr, addr + size))
            if len(self.written) > 1_000_000:  # a runaway copy / fill
                self.status = "cap"
                uc.emu_stop()

    def is_float_store(self, pc):
        """Only FP stores in the original justify a rounding comparison."""
        cache = getattr(self, '_float_store_cache', None)
        if cache is None:
            cache = self._float_store_cache = {}
        if pc not in cache:
            import capstone
            md = load('cs', lambda: capstone.Cs(capstone.CS_ARCH_X86, capstone.CS_MODE_32))
            ins = next(md.disasm(bytes(self.uc.mem_read(pc, 15)), pc), None)
            # FIST(P) is deliberately excluded: it writes integers. SSE scalar
            # stores are MOVSS/MOVSD; other MOVSD is the integer string copy.
            cache[pc] = bool(ins and (ins.mnemonic in ('fst', 'fstp') or
                                     ins.mnemonic in ('movss', 'movsd') and 'xmm' in ins.op_str))
        return cache[pc]

    def on_unmapped(self, uc, access, addr, size, value, _):
        base = addr & ~0xFFF
        # fetch from nowhere: a call target (on_block can't see it). Any other address above the null pages is
        # memory: a raw fill value used as a pointer (0x6B31E0's vtable field under profile 2) reads a filled page
        if access == 21 or addr < 0x10000 or base >= 0xFFFFF000:
            if access == 21:
                self.pending = addr
            return False
        if base in self.mapped:
            return False
        self.mapped.add(base)
        uc.mem_map(base, 0x1000)
        # Graph heap pages are real zero-initialized backing storage; default
        # profiles retain their original deterministic mixed fill.
        uc.mem_write(base, self.graph.page(base) if self.graph and
                     ZONES <= base < OBJ_BASE else fill_page(base, self.prof))
        return True

    def norm(self, v):
        if STACK_LO <= v < ESP0 + 0x40:  # the frame, or the address of an argument (ours may pass a copy's)
            return "stack"
        if OBJ_BASE <= v < OBJ_BASE + 0x100000:
            if self.obj and v == self.obj.entry:
                return self.ea  # the function's own address (a recursive callback: 0x6B41F0)
            return "own-data"
        return v

    def mock(self, target):
        """An outside call: record it, answer it. Returns where execution continues."""
        uc = self.uc
        esp = uc.reg_read(X.UC_X86_REG_ESP)
        ret = struct.unpack("<I", uc.mem_read(esp, 4))[0]
        i = len(self.events)
        name = None
        if self.obj:
            # A random indirect target can equal one of our external stubs. Only a
            # direct CALL relocation gives that numeric address the external's name.
            external = self.obj.external_calls.get(ret)
            if external and external[0] == target:
                name = external[1]
        imp = imports().get(target)  # a Windows / Bink / vorbis API: no prototype of ours, and ecx is no argument
        name = imp or name
        p = proto(target) if IMG <= target < IMG + 0x500000 and not imp else None
        if self.script is None:  # the original: its call site says what the callee pops
            site = self.ret_site.get(ret)
            fact = self.proven_pops.get(site, {})
            pop = fact.get('pop', call_pops().get(site, p[3] if p else 0)) if ret != RET_MAGIC else (p[3] if p else 0)
            # Even an indirect call has a resolved numeric target *now*.
            # Its actual RET outranks both stack solvers and IDA's delta.
            runtime_pop = dispatch_call_pop(target, dispatch_evidence(self.ea).get(site, {}).get('receiver')) if not imp else None
            if runtime_pop is not None:
                pop = runtime_pop
            elif site and not imp:
                # An IDA delta or a catalog restricted to unrelated synthetic
                # vtables cannot decide an unresolved callee's cleanup. Do not
                # execute a guessed ABI and compare the resulting broken stack.
                proved = stack_call_pops(self.ea, constraints_only=True).get(site)
                if proved is None:
                    self.status = 'unknown call cleanup'
                    return ret
                pop = proved
            nargs = len(p[1]) if p else pop // 4
            use_ecx = p[0] if p else not imp
            rt = p[2] if p else "?"
            nargs = fact.get("nargs", nargs)
            if not p and 'pop' in fact and 'nargs' not in fact:
                nargs = pop // 4
            use_ecx = fact.get("ecx", use_ecx)
            abi_ecx = callsite_abi(self.ea, site) if site else None
            if abi_ecx is not None:
                use_ecx = abi_ecx
            elif p:
                use_ecx = p[0] and takes_ecx(target)
            if not p and site:
                rt = callback_return_type(self.ea, site) or indirect_float_returns(self.ea).get(site, rt)
            if pop and not p:
                nargs = pop // 4
            callback = global_callback_types(self.ea).get(site)
            if callback:
                nargs, use_ecx = callback['nargs'], callback['ecx']
            if imp:
                proven = import_call_pop(ret, imp)
                if proven is not None:
                    pop = proven
                    nargs = pop // 4
                use_ecx = False
            if not imp and rt not in BYTE_TYPES and IMG <= target < IMG + 0x500000 and target in functions() and returns_al(target):
                rt = "char"  # the callee sets al only, whatever its prototype says
        elif i < len(self.script):
            _, pop, nargs, use_ecx, rt = self.script[i][:5]
        else:
            pop, nargs, use_ecx, rt = 0, 0, False, "?"
        # Known boundary ABI on BOTH sides; a malformed candidate must not
        # acquire its argument count from whatever event it replaced.
        if target in crt_events():
            pop, nargs, use_ecx, rt = crt_events()[target]
        argv = struct.unpack(f"<{nargs}I", uc.mem_read(esp + 4, 4 * nargs)) if nargs else ()
        self.events.append((name or target, pop, nargs, use_ecx, rt, tuple(self.norm(a) for a in argv),
                            self.norm(uc.reg_read(X.UC_X86_REG_ECX)) if use_ecx else None,
                            self.ret_site.get(ret) if self.script is None else None))
        if no_return(name or target, argv):
            # Preserve the prefix (including argument and memory differences),
            # but do not fabricate a return register or a normal caller frame.
            self.status = 'noreturn'
            return ret
        h = h32(0xE000 + i * 977 + self.prof * 131)
        # pointer results end in a non-zero byte: `test al, al` and `test eax, eax` on an int-typed callee that
        # returns a clean 0 / 1 (0x492BD0) must agree; a callee that sets al only gets garbage above it instead,
        # so a caller that binds it as int and tests eax still differs
        # every result is 0 or has bit 0 set as well: ours may hold it in a `bool` (clang tests bit 0 only), the
        # original tests al or eax (0x477B40: a result of 2 sent the two sides different ways)
        odd = lambda v: v if v < 2 else 2 * v - 1
        if rt in BYTE_TYPES:  # before the pointer profiles: the caller extends a byte, the original keeps eax
            val = (h & 1) | (0x5A5A5A00 if self.prof % 2 else 0)
        elif self.prof in (2, 6, 7) and rt not in ("float", "double", "long double", "void"):
            val = ZONES + 0x0C000000 + i * 0x10000 + 0x101
        elif "*" in rt or rt == "?":
            val = ZONES + 0x0C000000 + i * 0x10000 + 0x101 if (rt != "?" or self.prof % 2 == 0) else odd(h % 3)
        else:
            val = odd(h % 4)
        if rt == "void" and use_ecx:
            # typed void, but a constructor-like callee returns `this` and the original may use it (0x751830:
            # `call 0x64A6B0; push eax`); ours passes the object's address itself
            val = uc.reg_read(X.UC_X86_REG_ECX)
        if self.graph:
            original_site = self.ret_site.get(ret) if self.script is None else (
                self.script[i][7] if i < len(self.script) else None)
            val = self.graph.results.get(original_site, val)
        for address, data in mock_outputs(target, i, self.prof, argv, uc.reg_read(X.UC_X86_REG_ECX), val):
            for base in range(address & ~0xFFF, (address + len(data) + 0xFFF) & ~0xFFF, 0x1000):
                try:
                    uc.mem_read(base, 1)
                except UcError:
                    if not self.on_unmapped(uc, 19, base, 1, 0, None):
                        self.status = 'fault mock output'
                        return ret
            uc.mem_write(address, data)
            if not STACK_LO <= address < STACK_HI:
                self.written.update(range(address, address + len(data)))
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


@functools.lru_cache(maxsize=None)
def takes_ecx(target):
    """Does the callee read ecx before writing it (api.reads_before_write)? Unknown targets: yes. A prototype
    that says __thiscall for a function that never looks at ecx (0x70DDF0 `fld [..]; ret`) is no argument."""
    def go():
        from packet import DB
        return DB()
    if not isinstance(target, int):
        return True
    try:
        import api as apimod
        db = load("db", go)
        return target not in db.F or bool(apimod.reads_before_write(target, db, "ecx"))
    except Exception:
        return True


@functools.lru_cache(maxsize=None)
def unread_args(target):
    """Stack argument indices an ebp-framed callee never refers to (no `[ebp + 8 + 4k]` anywhere in it, and no
    `lea` of its argument area): an allocator passed by value to an STL helper is a garbage dword in the
    original, 0 or nothing in ours (0x502760 -> 0x500D60, 0x728160 -> 0x727540)."""
    if not isinstance(target, int) or target not in functions():
        return frozenset()
    def go():
        import capstone
        return capstone.Cs(capstone.CS_ARCH_X86, capstone.CS_MODE_32)
    md = load("cs", go)
    img, _ = exe_image()
    if bytes(img[target - IMG:target - IMG + 3]) != b"\x55\x8B\xEC":
        return frozenset()
    used, text = set(), []
    for a, b in own_ranges(target)[0]:
        for ins in md.disasm(bytes(img[a - IMG:b - IMG]), a):
            text.append(ins.op_str)
            if ins.mnemonic == "lea" and "ebp +" in ins.op_str:
                return frozenset()
            for m in re.finditer(r"ebp \+ (0x[0-9a-f]+|\d+)\]", ins.op_str):
                used.add((int(m.group(1), 0) - 8) // 4)
    return frozenset(k for k in range(12) if k not in used)


R8 = {"al": "eax", "bl": "ebx", "cl": "ecx", "dl": "edx"}


@functools.lru_cache(maxsize=None)
def literal_pushes(site):
    """Argument indices pushed as immediates in the original basic block.

    A literal 0xCC byte is defined data, not evidence of an unwritten local.
    Keep the uninitialized-stack rule for cases not proven by the binary.
    """
    import bisect, capstone
    starts = load('starts', lambda: sorted(functions()))
    k = bisect.bisect_right(starts, site) - 1
    if k < 0 or site - starts[k] > 0x4000:
        return frozenset()
    img, _ = exe_image()
    md = capstone.Cs(capstone.CS_ARCH_X86, capstone.CS_MODE_32)
    md.detail = True
    ins = list(md.disasm(img[starts[k] - IMG:site - IMG], starts[k]))
    out, idx = set(), 0
    for i in reversed(ins):
        if i.mnemonic in ('call', 'ret', 'pop') or i.mnemonic.startswith('j'):
            break
        if i.mnemonic == 'push':
            if i.operands[0].type == capstone.x86.X86_OP_IMM:
                out.add(idx)
            idx += 1
    return frozenset(out)


@functools.lru_cache(maxsize=None)
def byte_pushes(site):
    """Indices of the stack arguments of the original's call at `site` that are a byte in a pushed register:
    `setne dl; push edx` (0x71FCB0 to a vtable slot: nothing types the parameter, and edx's upper bytes are whatever
    it held). Read backwards from the call in its block: the i-th push, then the last write to that register."""
    import bisect, capstone
    starts = load("starts", lambda: sorted(functions()))
    k = bisect.bisect_right(starts, site) - 1
    if k < 0 or site - starts[k] > 0x4000:
        return frozenset()
    img, _ = exe_image()
    md = capstone.Cs(capstone.CS_ARCH_X86, capstone.CS_MODE_32)
    ins = [i for i in md.disasm(bytes(img[starts[k] - IMG:site - IMG]), starts[k])]
    out, idx = set(), 0
    for n in range(len(ins) - 1, max(len(ins) - 40, -1), -1):
        i = ins[n]
        if i.mnemonic in ("call", "ret", "pop") or i.mnemonic.startswith("j"):
            break
        if i.mnemonic != "push":
            continue
        r = i.op_str.strip()
        if r in R8.values():
            for j in reversed(ins[max(n - 12, 0):n]):
                d = j.op_str.split(",")[0].strip()
                if j.mnemonic in ("call", "ret") or j.mnemonic.startswith("j"):
                    break
                if R8.get(d) == r and (j.mnemonic.startswith("set") or j.mnemonic == "mov"):
                    out.add(idx)
                    break
                if d == r and j.mnemonic not in ("push", "cmp", "test"):
                    break
        idx += 1
    return frozenset(out)


def same_args(eo, em):
    """Stack arguments of one call, original vs ours. A byte or short parameter is compared in its width (the
    original pushes a dword whose upper bytes are whatever its frame held: 0x512400 `mov edx, [ebp-4]; push edx`
    for a char); bytes of the original's value still holding the never-written stack fill (0xCC) are not
    compared (a frame dword passed on before it is written: 0x4696C0, 0x47EE50)."""
    if eo[0] in crt_events():
        # Callback identity is exact: never treat it as unread stack garbage
        # or as a floating-point value with a rounding tolerance.
        return eo[5] == em[5]
    if eo[5] == em[5]:
        return True
    if len(eo[5]) != len(em[5]):
        return False
    p = proto(eo[0]) if isinstance(eo[0], int) else None
    types = [t.strip() for t in p[1]] if p else []
    if not p and len(eo) > 7 and eo[7]:
        import bisect
        starts = load('starts', lambda: sorted(functions()))
        k = bisect.bisect_right(starts, eo[7]) - 1
        if k >= 0:
            types = typed_indirect_calls(starts[k]).get(eo[7], {}).get('types', []) or named_vslot_types(starts[k], eo[7])
    literals = literal_pushes(eo[7]) if len(eo) > 7 and eo[7] else frozenset()
    for i, (a, b) in enumerate(zip(eo[5], em[5])):
        if a == b:
            continue
        if i in unread_args(eo[0]) or a in SAVED_REGS:
            continue  # before the stack test: ours may hold a frame address where the original has a leftover
        if isinstance(a, str) or isinstance(b, str):
            return False
        t = types[i] if i < len(types) else ""
        mask = 0xFF if t in BYTE_TYPES else 0xFFFF if t in ("short", "uint16_t", "int16_t", "unsigned short") else 0xFFFFFFFF
        if len(eo) > 7 and eo[7] and i in byte_pushes(eo[7]):
            mask = 0xFF
        for k in range(4):
            if i not in literals and (a >> 8 * k) & 0xFF == 0xCC:
                mask &= ~(0xFF << 8 * k)
        # the upper bytes are still the caller's ebx / esi / edi / edx: the original set bl (or bx) and pushed the
        # whole register for a byte parameter the prototype calls int (0x48EF60 `sete bl; push ebx`)
        if i in unread_args(eo[0]):
            continue
        if a in SAVED_REGS:
            continue  # a register the prologue saved: the event counts more arguments than the original pushed
        if a >> 16 in GARBAGE_HI:
            mask &= 0xFF if (a >> 8) & 0xFF == 0x01 else 0xFFFF
        if (a ^ b) & mask:
            fa, fb = struct.unpack("<ff", struct.pack("<II", a, b))
            if mask == 0xFFFFFFFF and abs(a - b) < 64 and fa == fa and fb == fb and close(fa, fb) and abs(fa) > 1e-30:
                continue  # last bits of a float (0x6AE210: 3eeb5d7f vs 3eeb5d80)
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
        # the original's ecx is still the value this function was entered with, and the function takes no ecx
        # argument: a leftover at an indirect call (0x6BC2D0 `call [esi + 0x48]` to a __cdecl callback)
        pr = proto(ea)
        if eo[6] == ECX0 and pr and not pr[0]:
            continue
        if eo[6] != em[6] and takes_ecx(eo[0]):
            if isinstance(eo[0], int) and eo[0] not in functions() and not proto(eo[0]):
                return ['?unresolved indirect-call convention (ECX may not be an argument)']
            out.append(f"call {i} to {tn(eo[0])}: ecx {tn(eo[6])} vs {tn(em[6])}")
            return out
    if m.status == 'cap' and o.blocks > 5000:
        return ['?ours runs past the cap after a long run of the original']
    if len(o.events) != len(m.events):
        out.append(f"{len(o.events)} calls vs {len(m.events)}")
        return out
    if o.status == 'noreturn' or m.status == 'noreturn':
        if o.status != m.status:
            return [f"termination {o.status} vs {m.status}"]
    elif m.status != "ret":
        if m.status == "cap" and o.blocks > 5000:
            # a long loop over filled memory the original just finished under the instruction cap (0x4A35D0: a
            # count from garbage; ours takes a few instructions more per round)
            return ["?ours runs past the cap after a long run of the original"]
        return [f"ours does not return ({m.status})"]
    terminated = o.status == 'noreturn'
    if not terminated and o.esp != m.esp and not (o.tail or m.tail):
        out.append(f"esp after return {o.esp - ESP0:+d} vs {m.esp - ESP0:+d}")
    if not terminated and not m.kept and o.kept:
        out.append("ours changes ebx/esi/edi/ebp")
    p = proto(ea)
    rt = p[2] if p else "int"
    if terminated:
        pass  # a nonreturning prefix has no return-value/callee-save contract
    elif rt in ("float", "double", "long double"):
        fo, fm = st0_float(o.st0), st0_float(m.st0)
        # the emulator loads a denormal float (a small int read as a float) with a wrong exponent: 0x401FA0 got
        # 1.47e19 from (4, 0, 5) read as floats. A result no game value has says nothing either way
        absurd = lambda v: v is not None and (v != v or abs(v) > 1e15 or 0 < abs(v) < 1e-30)
        if not close(fo, fm) and not absurd(fo) and not absurd(fm):
            out.append(f"st0 {fo} vs {fm}")
    elif rt != "void" and not (o.tail or m.tail) and not eax_meaningless(ea):
        mask = 0xFF if rt in BYTE_TYPES or returns_al(ea) else \
            0xFFFF if rt in ("short", "uint16_t", "int16_t", "unsigned short") else 0xFFFFFFFF
        # eax still holds what the last callee left, and that callee returns a float or nothing while this
        # function is typed wider: a leftover, not a return value (0x525A50, 0x6CAF00: Hex-Rays says int)
        lm = o.last_mock
        left = mask == 0xFFFFFFFF and lm and lm[0] == o.eax and lm[1] in ("void", "float", "double", "long double")
        # eax is still the old SEH chain head the prologue loaded (`mov eax, fs:[0]`): a void function with a
        # try block on a path that calls nothing (0x40B4C0, an STL fill over an empty range)
        img, _ = exe_image()
        if o.eax == 0xFFFFFFFF and bytes(img[ea - IMG + 3:ea - IMG + 5]) == b"\x6A\xFF":
            left = True
        if o.eax == EAX0:  # never written: the caller's eax
            left = True
        for k in range(4):  # bytes of the original's eax that are never-written stack fill (0x48CFE0 returns a
            if (o.eax >> 8 * k) & 0xFF == 0xCC:  # 16-bit local loaded as a dword: cccc0101)
                mask &= ~(0xFF << 8 * k)
        if not left and o.norm(o.eax & mask) != m.norm(m.eax & mask):
            if mask == 0xFFFFFFFF and getattr(o, 'eax_width', 32) == 8 and (o.eax & 0xFF) == (m.eax & 0xFF):
                return ['?AL-only return path with an unresolved full-EAX contract']
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
    real, soft = [], []
    for g in groups:
        try:
            fo, fm = struct.unpack("<f", o.uc.mem_read(g, 4))[0], struct.unpack("<f", m.uc.mem_read(g, 4))[0]
            wo, wm = struct.unpack("<I", o.uc.mem_read(g, 4))[0], struct.unpack("<I", m.uc.mem_read(g, 4))[0]
        except UcError:
            real.append((g, None, None))
            continue
        if o.norm(wo) == m.norm(wm) and isinstance(o.norm(wo), str):
            continue  # a pointer into each side's own frame / data
        fp_store = all(g + k in o.float_written for k in range(4))
        if fp_store and fo == fo and fm == fm and abs(wo - wm) < 64 and close(fo, fm):
            continue  # last bits of a float
        ccmask = 0xFFFFFFFF
        for k in range(4):  # bytes of the original's dword that are never-written stack fill it copied along
            if (wo >> 8 * k) & 0xFF == 0xCC:
                ccmask &= ~(0xFF << 8 * k)
        if ccmask != 0xFFFFFFFF and not (wo ^ wm) & ccmask:
            continue  # 0x4C0280 stores a byte local as a dword: 23cccccc vs 23000000
        huge = lambda w, f: (w & 0x7F800000) == 0x7F800000 or abs(f) > 1e15
        if fp_store and huge(wo, fo) and huge(wm, fm):
            continue  # NaN / infinity / beyond any game value on both sides (0x6F5E60: ffffffff vs 7fc00000)
        tiny = lambda w, f: (w & 0x7FFFFFFF) == 0 or ((w & 0x7F800000) != 0 and abs(f) < 1e-30)
        if fp_store and tiny(wo, fo) and tiny(wm, fm) and (wo | wm) & 0x7F800000:
            continue  # a float below anything the game computes against a zero (0x4E2110: -6.9e-32 vs -0.0)
        fill = fill_word(g, o.prof)
        if fp_store and 0 < fill < 0x00800000 and fill in (wo, wm) and wo != wm:
            soft.append(g)  # one side left a small int there, the other computed with it as a (denormal) float
            continue
        real.append((g, wo, wm))
    if soft and not real and not out:
        return ["?float code over small ints (denormals: the emulator and float precision disagree)"]
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
    if verbose:
        for pc, fact in sorted(resolved_call_facts(ea).items()):
            old = call_pops().get(pc)
            if old is not None and old != fact['pop'] or fact.get('disagreements'):
                other = '; '.join(fact.get('disagreements', []) + [f'IDA delta = {old}'])
                print(f"  cleanup {pc:08x}: {fact['source']} = {fact['pop']} bytes; {other} (disagree)")
    from packet import DB, callable_shared_chunk, disasm, vcall_tail_pops
    shared = []
    if len(own) > 1:
        db = load("db", DB)
        shared = [(a, b) for a, b in own[1:] if callable_shared_chunk(a, db)]
        for a, b in shared:
            call_pops().update(vcall_tail_pops(disasm(a, b - a, linear=True)))
    _tls.trace = ""
    done, diffs, why = 0, [], collections.Counter()
    from emuprofiles import build as graph_profile, discover as discover_graph
    from types import SimpleNamespace
    implementation = SimpleNamespace(**globals())  # also works with importlib benchmark loading
    inputs = [(prof, None) for prof in range(profiles)]
    graph_keys = set()
    for variant in range(2):
        graph = graph_profile(implementation, ea, variant)
        if graph:
            key = tuple(tuple(sorted(v.items())) for v in
                        (graph.words, graph.args, graph.results, graph.site_tables))
            if key not in graph_keys:
                graph_keys.add(key)
                inputs.append((2, graph))
    for number, (prof, graph) in enumerate(inputs):
        if graph:
            o = discover_graph(implementation, ea, prof, own, ret_site, obj, graph)
            if verbose:
                for proof in graph.evidence:
                    print(f"  profile {number} pointer: {proof}")
        else:
            o = Run(ea, prof, own, memory_obj=obj)
            o.ret_site = ret_site
            o.start(ea)
        if o.status not in ("ret", "noreturn"):
            why[o.status.split(" at ")[0][:40]] += 1
            continue
        # IDA appends shared tail chunks to the original's ranges. A candidate can delegate such a chunk
        # with CSF_FUNC: run the same bytes instead of mocking the chunk as an extra outside call.
        m = Run(ea, prof, obj.code + shared, obj, o.events, graph=graph).start(obj.entry)
        d = compare(ea, o, m)
        if d and d[0].startswith("?"):
            why[d[0][1:41]] += 1
            continue
        done += 1
        if verbose:
            print(f"  profile {number}: {len(o.events)} calls, {len(o.written)} bytes written: {'; '.join(d) or 'same'}")
        if d:
            if not diffs:  # the two call sequences of the first differing profile (the gate shows them)
                seq = lambda r: ", ".join((tn(e[0]) if isinstance(e[0], int) and e[0] in functions() else
                                           "indirect" if isinstance(e[0], int) else e[0]) + f"({e[2]} args)"
                                          for e in r.events[:10]) + (" ..." if len(r.events) > 10 else "")
                _tls.trace = f"original calls: {seq(o) or 'none'}\nyours calls:    {seq(m) or 'none'}"
            diffs.append(f"profile {number}: {d[0]}")
    if diffs:
        return "DIFF", f"{len(diffs)}/{done} profiles; {diffs[0]}"
    if not done:
        return "inconclusive", ", ".join(f"{k} x{v}" for k, v in why.most_common())
    return "same", f"{done}/{len(inputs)} profiles"


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
