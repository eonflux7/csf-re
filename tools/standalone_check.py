#!/usr/bin/env python3
"""Linux-side audit of CSF_DRYRUN inputs, NOT execution or runtime certification.

Structural inconsistencies fail. Startup blockers (fatal initializer/vtable
traps and unanalysed indirect calls) are reported, never treated as success.
"""
import argparse
import collections
import csv
import hashlib
import json
from pathlib import Path
import re
import struct
import sys

import capstone
from capstone.x86 import X86_OP_IMM, X86_OP_MEM

ROOT = Path(__file__).resolve().parent.parent
sys.path.insert(0, str(ROOT / 'tools'))
from datagen import Image, definitions, number, check_object


def coff_image_bytes(path, size):
    """Read the generated image's actual initialized COFF storage (PE32 object)."""
    blob = path.read_bytes()
    machine, sections, _, symtab, count, optional, _ = struct.unpack_from('<HHIIIHH', blob)
    assert machine == 0x14C and not optional, 'expected standard x86 COFF object'
    strings = blob[symtab + count * 18:]
    matches, index = [], 0
    while index < count:
        offset = symtab + index * 18
        name = blob[offset:offset + 8]
        value, section, _, _, aux = struct.unpack_from('<IhHBB', blob, offset + 8)
        if name[:4] == bytes(4):
            start = struct.unpack_from('<I', name, 4)[0]
            name = strings[start:strings.index(b'\0', start)]
        else:
            name = name.rstrip(b'\0')
        if name.startswith(b'?image@') and section > 0:
            header = 20 + (section - 1) * 40
            raw_size, raw = struct.unpack_from('<II', blob, header + 16)
            assert raw and value + size <= raw_size, 'image storage truncated'
            matches.append(blob[raw + value:raw + value + size])
        index += 1 + aux
    assert len(matches) == 1, 'expected exactly one generated image symbol'
    return matches[0]


def check_compiled_inputs(cutover, real, traps, required, image):
    """Require fresh COFF inventory, and independently inspect generated objects.

    stubgen already independently inspected every real object; prevent use of
    older inventory after an object changes, rather than trusting stale JSON.
    """
    from linktest import symbols, demangle, SLOT
    objects = [Path(p) for p in (cutover / 'objects.txt').read_text().splitlines()]
    stamp = (cutover / 'linktest.json').stat().st_mtime_ns
    assert objects and all(p.is_file() and p.stat().st_mtime_ns <= stamp for p in objects), \
        'stale COFF inventory'
    data_obj, = [p for p in objects if p.name == 'data.cpp.obj']
    init_obj, = [p for p in objects if p.name == 'initializers.cpp.obj']
    assert data_obj.stat().st_mtime_ns >= (cutover / 'data/data.cpp').stat().st_mtime_ns
    assert init_obj.stat().st_mtime_ns >= (cutover / 'initializers.cpp').stat().st_mtime_ns
    check_object(data_obj, required)
    assert coff_image_bytes(data_obj, len(image.bytes)) == image.bytes, 'COFF image bytes differ'
    stubs_obj = cutover.parent / 'CMakeFiles/csf_standalone.dir/cutover/stubs.cpp.obj'
    assert stubs_obj.is_file(), 'build the executable trap object first'
    assert stubs_obj.stat().st_mtime_ns >= (cutover / 'stubs.cpp').stat().st_mtime_ns
    defined, _ = symbols(str(stubs_obj))
    actual = collections.Counter()
    for name in demangle(sorted(s for s in defined if 'fn_slot@' in s)).values():
        match = SLOT.search(name)
        if match and match[1] == 'fn_slot':
            actual[int(match[2])] += 1
    assert actual == collections.Counter({a: 1 for a in traps}), 'actual trap definitions differ'
    assert not real & actual.keys()


def virtual_sites(code, slots, table_starts):
    """Exact absolute indirect calls plus conservative local vtable candidates.

No interprocedural register/alias proof is claimed. Computed calls remain an
explicit unknown even when a local table/offset pair provides candidates.
"""
    tables = {op.imm for i in code for op in i.operands
              if op.type == X86_OP_IMM and op.imm in table_starts}
    exact, candidates, unknown = [], [], []
    for i in code:
        if i.mnemonic not in ('call', 'jmp') or not i.operands:
            continue
        op = i.operands[0]
        if op.type == X86_OP_IMM:
            continue
        if op.type == X86_OP_MEM and not op.mem.base and not op.mem.index:
            if op.mem.disp in slots:
                exact.append((i.address, op.mem.disp))
            else:
                unknown.append(i.address)
        else:
            unknown.append(i.address)
            if op.type == X86_OP_MEM and not op.mem.index:
                for table in tables:
                    if table + op.mem.disp in slots:
                        candidates.append((i.address, table + op.mem.disp))
    return exact, candidates, unknown


def vtable_traps(sites, slots, traps):
    return [dict(instruction=hex(i), source=hex(a), target=hex(slots[a]))
            for i, a in sorted(set(sites)) if slots[a] in traps]


def reachable_code(image, decoder, start, size):
    """Decode only reachable basic blocks, not inline switch data or next thunks."""
    pending, decoded = [start], {}
    end = start + size
    while pending:
        pc = pending.pop()
        while start <= pc < end and pc not in decoded:
            ins = next(decoder.disasm(image.pe.get_data(pc - image.base, min(15, end - pc)),
                                     pc, count=1), None)
            assert ins is not None, f'undecodable initializer path {pc:#x}'
            decoded[pc] = ins
            if ins.mnemonic.startswith('ret') or ins.mnemonic in ('int3', 'ud2'):
                break
            if ins.mnemonic.startswith('j') or ins.mnemonic.startswith('loop'):
                if ins.operands[0].type == X86_OP_IMM and start <= ins.operands[0].imm < end:
                    pending.append(ins.operands[0].imm)
                if ins.mnemonic == 'jmp':
                    break
            pc += ins.size
    return [decoded[a] for a in sorted(decoded)]


def audit(cutover, exe):
    image = Image(exe)
    data = cutover / 'data'
    source = (data / 'data.cpp').read_text()
    manifest = json.loads((data / 'manifest.json').read_text())
    report = json.loads((data / 'datagen.json').read_text())
    inventory = json.loads((cutover / 'linktest.json').read_text())
    traps = json.loads((cutover / 'stubs.json').read_text())
    initializers = json.loads((cutover / 'initializers.json').read_text())
    pointers = json.loads((data / 'datagen-pointers.json').read_text())
    assert not inventory['failed'] and not inventory['duplicates']
    assert not json.loads((data / 'datagen-rejected.json').read_text()), 'rejected data remains'
    real = {number(a) for a in inventory['defined_functions']}
    stub = {number(r['address']) for r in traps['slots']}
    missing = {number(a) for group in inventory['unresolved_functions'].values() for a in group}
    assert stub == missing and not real & stub and len(stub) == traps['count']
    assert traps['counts'] == dict(collections.Counter(r['kind'] for r in traps['slots']))
    assert manifest['image_size'] == len(image.bytes)
    assert number(manifest['image_base']) == image.start
    assert manifest['image_sha256'] == hashlib.sha256(image.bytes).hexdigest()
    emitted = bytearray(int(x, 16) for x in re.findall(
        r'0x([0-9A-F]{2})', source.split('] = {', 1)[1].split('};', 1)[0]))
    emitted.extend(bytes(len(image.bytes) - len(emitted)))
    assert emitted == image.bytes, 'generated initial bytes differ from PE data'
    required = {('gv' if k == 'globals' else 'vt', number(a))
                for k in ('globals', 'vtables') for a in manifest[k]}
    base = json.loads((cutover / 'base.json').read_text())
    assert required == {('gv' if k == 'globals' else 'vt', number(a))
                        for k in ('globals', 'vtables') for a in base[k]}
    assert definitions(source) == collections.Counter({k: 1 for k in required})
    aliases = re.findall(r'(gv|vt)<0x([0-9A-F]+)>::[dt] = image \+ (\d+);', source)
    assert len(aliases) == len(required)
    for kind, address, offset in aliases:
        assert image.data(int(address, 16)) and int(address, 16) - image.start == int(offset)
    fixes = re.findall(r'put\((\d+), (.*)\);', source)
    assert len(fixes) == len({int(off) for off, _ in fixes}) == report['fixups']
    fixes = {int(off): expr for off, expr in fixes}
    applied = [r for r in pointers if r['applied']]
    assert len(applied) == len(fixes)
    assert len(applied) == len({number(r['source']) for r in applied}), 'duplicate pointer source'
    assert {number(r['source']) - image.start for r in applied} == fixes.keys()
    code_fixups = 0
    for r in applied:
        a, target = number(r['source']), number(r['target'])
        assert r['confirmed'] and image.data(a) and image.data(a + 3)
        assert image.word(a) == target
        if image.code(target):
            assert target in real | stub, f'undefined fix-up target {target:#x}'
            expr = f'fn_slot<0x{target:08X}>::p'
            code_fixups += 1
        else:
            assert image.data(target), f'outside-image fix-up target {target:#x}'
            expr = f'image + {target - image.start}'
        assert fixes[a - image.start] == expr

    # Fixed original CRT table boundaries are independent of initgen's manifest.
    # Exact binary __cinit range/disassembly evidence is maintained by initgen.
    ranges = [('xi', 0x898454, 0x89846C), ('xc', 0x898000, 0x898450)]
    expected = []
    for kind, start, end in ranges:
        assert image.word(start) == image.word(end) == 0
        for index, a in enumerate(range(start, end, 4)):
            expected.append((kind, index, a, image.word(a)))
    actual = [(r['table'], r['index'], number(r['source']), number(r['target']))
              for r in initializers['table_slots']]
    assert actual == expected, 'initializer table order/contents differs from original'
    nonzero = [row for row in expected if row[3]]
    assert initializers['count'] == len(nonzero)
    assert [(r['table'], r['index'], number(r['source']), number(r['target']))
            for r in initializers['entries']] == nonzero
    init_source = (cutover / 'initializers.cpp').read_text().split('int csf::run_original_initializers() {', 1)[1]
    # the original CRT's own start-up entries are listed in order but not called (tools/initgen.py)
    crt_own = {number(r['target']) for r in initializers['entries'] if r['region'] == 'crt'}
    assert [int(a, 16) for a in re.findall(r'init_([0-9A-F]+)\(\)', init_source)] == \
        [r[3] for r in nonzero if r[3] not in crt_own]
    assert [int(a, 16) for a in re.findall(r'// \w+\[\d+\] 0x([0-9A-F]+): original CRT', init_source)] == \
        [r[3] for r in nonzero if r[3] in crt_own]
    called = [r for r in nonzero if r[3] not in crt_own]
    init_traps = [hex(r[3]) for r in called if r[3] in stub]
    assert all(r[3] in real | stub for r in called)
    assert all(r['covered'] == (number(r['target']) in real) for r in initializers['entries'])
    assert initializers['covered'] == len(called) - len(init_traps) + sum(r[3] in real for r in nonzero if r[3] in crt_own)
    assert initializers['missing'] == len(init_traps) + sum(r[3] not in real for r in nonzero if r[3] in crt_own)

    vt_rows = json.loads((data / 'datagen-vtables.json').read_text())
    slots = {number(v['address']) + 4 * i: number(t)
             for v in vt_rows for i, t in enumerate(v['slots']) if number(t)}
    for a, target in slots.items():
        assert image.word(a) == target
        assert fixes.get(a - image.start) == f'fn_slot<0x{target:08X}>::p'
        assert target in real | stub
    with (ROOT / 'db/functions.csv').open() as stream:
        functions = {number(r['ea']): r for r in csv.DictReader(stream)}
    decoder = capstone.Cs(capstone.CS_ARCH_X86, capstone.CS_MODE_32)
    decoder.detail = True
    # Follow initializer direct-call annotations and listed game functions.
    # Library callees are not reverse-implemented or declared safe by this audit.
    queue = [r[3] for r in nonzero]
    seen, exact, candidates, unknown, stopped = set(), [], [], [], []
    while queue:
        a = queue.pop()
        if a in seen:
            continue
        seen.add(a)
        f = functions.get(a)
        if f and f['region'] != 'game':
            stopped.append(hex(a))
            continue
        # Unlisted thunks have a bounded scan, not an invented function extent.
        size = int(f['size']) if f else 256
        code = reachable_code(image, decoder, a, size)
        e, c, u = virtual_sites(code, slots, {number(v['address']) for v in vt_rows})
        exact.extend(e); candidates.extend(c); unknown.extend(u)
        for ins in code:
            if ins.mnemonic in ('call', 'jmp') and ins.operands and ins.operands[0].type == X86_OP_IMM:
                target = ins.operands[0].imm
                if ins.mnemonic == 'jmp' and a <= target < a + size:
                    continue  # local block, not another initializer callee
                if image.code(target):
                    queue.append(target)
        if len(seen) > 10000:
            raise AssertionError('initializer call graph exceeds audit budget')
    exact_traps = vtable_traps(exact, slots, stub)
    possible_traps = vtable_traps(candidates, slots, stub)
    check_compiled_inputs(cutover, real, stub, required, image)
    result = dict(structural_consistency='ok', image_bytes=len(image.bytes), aliases=len(required),
                  compiled_generated_objects='ok; real COFF inventory fresh',
                  fixups=len(fixes), code_fixups=code_fixups, undefined_fixup_targets=0,
                  initializer_order='original C then C++, including nulls', initializers=len(nonzero),
                  initializer_traps=init_traps, scanned_initializer_callees=len(seen),
                  exact_initializer_vtable_traps=exact_traps,
                  possible_initializer_vtable_traps=possible_traps,
                  unanalysed_indirect_calls=[hex(a) for a in sorted(set(unknown))],
                  stopped_library_callees=sorted(set(stopped)),
                  startup_ready=not (init_traps or exact_traps or possible_traps or unknown or stopped),
                  limitation='Static direct-call/local-table audit, not interprocedural alias proof or execution. '
                             'Unresolved computed calls, library internals, IAT pointers, and stripped relocation '
                             'completeness are not certified.')
    return result


def main():
    ap = argparse.ArgumentParser(description=__doc__)
    ap.add_argument('--cutover', type=Path, default=ROOT / 'build/cutover')
    ap.add_argument('--exe', type=Path, default=ROOT / 'work/pc/CommXPC.exe')
    ap.add_argument('--out', type=Path)
    ap.add_argument('--require-ready', action='store_true',
                    help='also fail for startup blockers (diagnostic default reports them)')
    args = ap.parse_args()
    result = audit(args.cutover, args.exe)
    if args.out:
        args.out.write_text(json.dumps(result, indent=1) + '\n')
    print(f"dry-run inputs: {result['structural_consistency']}; {result['fixups']} fixups; "
          f"undefined targets {result['undefined_fixup_targets']}; initializer traps {len(result['initializer_traps'])}; "
          f"vtable traps {len(result['exact_initializer_vtable_traps'])} exact / "
          f"{len(result['possible_initializer_vtable_traps'])} possible; "
          f"unanalysed indirect calls {len(result['unanalysed_indirect_calls'])}; "
          f"startup ready: {result['startup_ready']}")
    return 2 if args.require_ready and not result['startup_ready'] else 0


if __name__ == '__main__':
    sys.exit(main())
