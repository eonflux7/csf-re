#!/usr/bin/env python3
"""Inspect real CSF CPU WORLDs, including the sector-property size bug.

References: original 59FEA0/59F990/59F440, 6B8D20 (size), 6B8510
(write), 6B8D50 (read). No SDK input. --tree prints every WORLD descendant.
"""
import argparse
import collections
from pathlib import Path
import struct

NAMES = {1: 'STRUCT', 2: 'STRING', 3: 'EXTENSION', 6: 'TEXTURE',
         7: 'MATERIAL', 8: 'MATLIST', 9: 'SECTOR', 10: 'PLANE', 11: 'WORLD'}


def inspect(path, tree=False):
    data = path.read_bytes()
    counts = collections.Counter()
    stamps = collections.Counter()
    errors = []
    totals = [0, 0]

    def chunk(at, depth=0, sector=None):
        kind, length, stamp = struct.unpack_from('<III', data, at)
        version = ((((stamp >> 14) & 0x3ff00) + 0x30000) |
                   ((stamp >> 16) & 0x3f)) if stamp & 0xffff0000 else stamp << 8
        counts[kind] += 1
        stamps[(stamp, version)] += 1
        start = pos = at + 12
        detail = ''
        if kind == 0xffffff00 and sector is not None:
            ver, present = struct.unpack_from('<II', data, pos)
            assert ver == 1 and present in (0, 1)
            assert length == 12 + (sector if present else 0)
            pos += 8 + (sector if present else 0)
            detail = f' property v={ver} present={present}'
        elif kind in (11, 10):
            # Both WORLD and PLANE have fixed semantic child counts.
            children = 4 if kind == 11 else 3
            for _ in range(children):
                pos = chunk(pos, depth + 1)
        elif kind == 9:
            nt, nv = struct.unpack_from('<II', data, pos + 16)
            totals[0] += nt
            totals[1] += nv
            pos = chunk(pos, depth + 1)
            pos = chunk(pos, depth + 1, nt)
            detail = f' triangles={nt} vertices={nv}'
        elif kind in (3, 6, 7, 8):
            # Extension accounting uses declared sizes, as 63CB30 does.
            budget = length
            while budget:
                child_length = struct.unpack_from('<I', data, pos + 4)[0]
                budget -= 12 + child_length
                assert budget >= 0
                pos = chunk(pos, depth + 1, sector if kind == 3 else None)
        else:
            pos += length
        assert pos <= len(data)
        delta = length - (pos - start)
        if delta:
            errors.append((at, kind, length, pos - start))
        if tree:
            print(f'{"  " * depth}{at:08x} {NAMES.get(kind, hex(kind))}'
                  f' declared={length} physical={pos-start} stamp={stamp:08x}'
                  f' version={version:05x}{detail}')
        return pos

    # RWS also contains game-specific top-level records with their own size
    # conventions. Locate WORLD by its STRUCT prefix, not blind top-level skips.
    needle = struct.pack('<I', 11)
    at = -1
    found = 0
    while True:
        at = data.find(needle, at + 1)
        if at < 0:
            break
        if data[at+12:at+20] != struct.pack('<II', 1, 64):
            continue
        found += 1
        header = struct.unpack_from('<16I', data, at + 24)
        print(f'{path.name}: bytes={len(data)} WORLD at=0x{at:x}'
              f' triangles={header[4]} vertices={header[5]}'
              f' planes={header[6]} leaves={header[7]} flags=0x{header[9]:x}')
        end = chunk(at)
        assert totals == [header[4], header[5]]
        assert counts[9] == header[7] and counts[10] == header[6]
        print(f'WORLD physical end=0x{end:x}; remaining={len(data)-end};'
              f' overstatement={errors[-1][2]-errors[-1][3] if errors else 0}')
    assert found == 1
    print('chunks:', ' '.join(f'{NAMES.get(k,hex(k))}={n}' for k, n in sorted(counts.items())))
    print('versions:', dict(stamps))
    print('size mismatches by kind:', dict(collections.Counter(k for _, k, _, _ in errors)))


if __name__ == '__main__':
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument('files', nargs='*', type=Path)
    parser.add_argument('--tree', action='store_true')
    args = parser.parse_args()
    files = args.files or list(Path('/mnt/e/dev/csf-paks/Ransom/Maps/FR01').glob('*.rws'))
    if not files:
        print('SKIP: extracted FR01 RWS files absent')
    for path in files:
        inspect(path, args.tree)
