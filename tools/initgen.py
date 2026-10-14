#!/usr/bin/env python3
"""Read the original CRT's __cinit ranges; emit ordered slot calls, never code bytes.

All original entries are retained, including missing library startup. A missing
initializer traps rather than being silently skipped or replaced with a no-op.
"""
import argparse
import collections
import csv
import json
from pathlib import Path
import struct

import capstone
from capstone.x86 import X86_OP_IMM, X86_OP_REG
import pefile
import yaml

ROOT = Path(__file__).resolve().parent.parent


def generate(exe, inventory, output):
    pe = pefile.PE(str(exe))
    base = pe.OPTIONAL_HEADER.ImageBase
    assert pe.OPTIONAL_HEADER.Magic == 0x10b
    with (ROOT / 'db/functions.csv').open() as stream:
        functions = {int(r['ea'], 16): r for r in csv.DictReader(stream)}
    cinit, = [a for a, r in functions.items() if r['name'] == '__cinit']
    winmain, = [a for a, r in functions.items() if r['name'].lower() in ('_winmain@16', 'winmain')]
    entry = base + pe.OPTIONAL_HEADER.AddressOfEntryPoint
    decoder = capstone.Cs(capstone.CS_ARCH_X86, capstone.CS_MODE_32)
    decoder.detail = True

    def instructions(a, size):
        return list(decoder.disasm(pe.get_data(a - base, size), a))

    calls = [i.operands[0].imm for i in instructions(entry, int(functions[entry]['size']))
             if i.mnemonic == 'call' and i.operands[0].type == X86_OP_IMM]
    assert cinit in calls and winmain in calls and calls.index(cinit) < calls.index(winmain)
    code = instructions(cinit, int(functions[cinit]['size']))
    # Binary __cinit has two inlined _initterm loops. The first traverses ecx..edi
    # and checks eax (C initializers); the second esi..edi ignores the result.
    constants = [(i.reg_name(i.operands[0].reg), i.operands[1].imm, i.address)
                 for i in code if i.mnemonic == 'mov' and len(i.operands) == 2
                 and i.operands[0].type == X86_OP_REG and i.operands[1].type == X86_OP_IMM
                 and pe.get_section_by_rva(i.operands[1].imm - base) is not None
                 and not pe.get_section_by_rva(i.operands[1].imm - base).Characteristics & 0x20000000]
    assert [r for r, _, _ in constants] == ['ecx', 'edi', 'esi', 'edi'], constants
    ranges = [('xi', constants[0][1], constants[1][1]), ('xc', constants[2][1], constants[3][1])]
    defined = {int(a, 16) for a in json.loads(inventory.read_text())['defined_functions']}
    regions = yaml.safe_load((ROOT / 'db/regions.yaml').read_text())

    def region(a):
        if a in functions:
            return functions[a]['region']
        found = 'unknown'
        for r in regions:
            if r['start'] <= a < r['end']:
                found = r['lib'] or 'game'
        return found

    rows = []
    table_rows = []
    for kind, start, end in ranges:
        assert start < end and (end - start) % 4 == 0
        assert struct.unpack('<I', pe.get_data(start - base, 4))[0] == 0
        assert struct.unpack('<I', pe.get_data(end - base, 4))[0] == 0
        words = struct.unpack('<' + 'I' * ((end - start) // 4), pe.get_data(start - base, end - start))
        for index, target in enumerate(words):
            row = dict(table=kind, source=f'0x{start + 4 * index:08X}',
                       target=f'0x{target:08X}', index=index)
            table_rows.append(row)
            if not target:
                continue
            section = pe.get_section_by_rva(target - base)
            assert section is not None and section.Characteristics & 0x20000000
            # For unlisted compiler thunks, annotate callees, not an invented
            # game/library verdict from the coarse mixed-tail region alone.
            direct = []
            for i in instructions(target, 256):
                if i.mnemonic in ('call', 'jmp') and i.operands[0].type == X86_OP_IMM:
                    a = i.operands[0].imm
                    direct.append(dict(address=f'0x{a:08X}', region=region(a),
                                       name=functions.get(a, {}).get('name', ''),
                                       listed=a in functions, covered=a in defined))
                if i.mnemonic.startswith('ret'):
                    break
                if i.mnemonic == 'jmp':
                    # Do not disassemble the next thunk after a tail transfer.
                    # Callees are annotations, not a control-flow reconstruction.
                    break
            rows.append(dict(**row, name=functions.get(target, {}).get('name') or '<not named in db/functions.csv>',
                             listed=target in functions, region=region(target),
                             covered=target in defined, direct_callees=direct))
    output.mkdir(parents=True, exist_ok=True)
    source = ['// Generated from CRT startup by tools/initgen.py. DO NOT EDIT.',
              '#include "globals.h"', '#include "runtime.h"', '#include <cstdio>']
    for r in rows:
        a = int(r['target'], 16)
        ret = 'int' if r['table'] == 'xi' else 'void'
        source.append(f'CSF_FUNC({ret}, __cdecl, init_{a:08X}, 0x{a:08X}, ());')
    source += ['int csf::run_original_initializers() {']
    for r in rows:
        a = int(r['target'], 16)
        if r['region'] == 'crt':
            # The original C runtime's own start-up (heap, SSE2 probe, locale, stdio): the standalone exe has its
            # own runtime, which has done its start-up before main. Not called; listed so the order stays visible.
            source.append(f'    // {r["table"]}[{r["index"]}] {r["target"]}: original CRT start-up, not called')
            continue
        source.append(f'    std::fputs("csf: {r["table"]}[{r["index"]}] {r["target"]}\\n", stderr);')
        if r['table'] == 'xi':
            source.append(f'    if (int result = init_{a:08X}()) return result;')
        else:
            source.append(f'    init_{a:08X}();')
    source += ['    return 0;', '}', '']
    (output / 'initializers.cpp').write_text('\n'.join(source))
    evidence = [f'PE entry 0x{entry:08X}', *[
        f'{i.address:08X}: {i.mnemonic} {i.op_str}'
        for i in instructions(entry, int(functions[entry]['size']))],
        f'\n__cinit 0x{cinit:08X}', *[
        f'{i.address:08X}: {i.mnemonic} {i.op_str}' for i in code]]
    (output / 'startup-disassembly.txt').write_text('\n'.join(evidence) + '\n')
    listing = ['table\tindex\tsource\ttarget\tcovered\tlisted\tregion\tname\tdirect callees (coverage)']
    for r in rows:
        callees = '; '.join(f"{c['address']} {c['name']} [{c['region']}; "
                            f"{'covered' if c['covered'] else 'missing'}]" for c in r['direct_callees'])
        listing.append('\t'.join(str(r[k]) for k in
            ('table', 'index', 'source', 'target', 'covered', 'listed', 'region', 'name')) + '\t' + callees)
    (output / 'initializers.tsv').write_text('\n'.join(listing) + '\n')
    game_callees = {c['address']: c for r in rows for c in r['direct_callees'] if c['region'] == 'game'}
    report = dict(entry=hex(entry), cinit=hex(cinit), winmain=hex(winmain),
                  ranges=[dict(table=k, start=hex(a), end=hex(z), slots=(z-a)//4,
                               nonzero=sum(r['table'] == k for r in rows)) for k, a, z in ranges],
                  count=len(rows), covered=sum(r['covered'] for r in rows),
                  missing=sum(not r['covered'] for r in rows),
                  listed_regions=dict(collections.Counter(r['region'] for r in rows if r['listed'])),
                  entries_with_game_callees=sum(any(c['region'] == 'game' for c in r['direct_callees']) for r in rows),
                  game_direct_callees=dict(count=len(game_callees),
                      covered=sum(c['covered'] for c in game_callees.values()),
                      missing=sum(not c['covered'] for c in game_callees.values())),
                  unlisted=sum(not r['listed'] for r in rows), entries=rows, table_slots=table_rows)
    (output / 'initializers.json').write_text(json.dumps(report, indent=1) + '\n')
    print(f"original initializers: {len(rows)}; covered {report['covered']}; missing {report['missing']}; "
          f"unlisted {report['unlisted']} (coarse mixed-tail regions are not a game classification)")


if __name__ == '__main__':
    ap = argparse.ArgumentParser(description=__doc__)
    ap.add_argument('--exe', type=Path, default=ROOT / 'work/pc/CommXPC.exe')
    ap.add_argument('--linktest', type=Path, required=True)
    ap.add_argument('--output', type=Path, required=True)
    args = ap.parse_args()
    generate(args.exe, args.linktest, args.output)
