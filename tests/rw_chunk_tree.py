#!/usr/bin/env python3
"""Inspect extracted CSF RW streams (not SDK code). Full tree goes to stdout.

CSF sector property GetSize (6B8D20) counts 12 bytes, but Write (6B8510)
writes two words, then optionally numTriangles bytes. Container lengths include
the missing word. Walk those containers structurally, not by their end offsets.
"""
import argparse
import collections
from pathlib import Path
import struct

NAMES = {1: 'STRUCT', 2: 'STRING', 3: 'EXTENSION', 6: 'TEXTURE',
         7: 'MATERIAL', 8: 'MATLIST', 9: 'SECTOR',
         10: 'PLANE', 11: 'WORLD', 16: 'CLUMP', 0x802: 'WAVE',
         0x803: 'WAVE_INFO', 0x804: 'WAVE_DATA'}


class Dump:
    def __init__(self, data):
        self.data = data
        self.counts = collections.Counter()
        self.versions = collections.Counter()
        self.short_properties = 0

    def header(self, p):
        t, n, stamp = struct.unpack_from('<III', self.data, p)
        v = ((((stamp >> 14) & 0x3ff00) + 0x30000) |
             ((stamp >> 16) & 0x3f)) if stamp & 0xffff0000 else stamp << 8
        return t, n, stamp, v

    def chunk(self, p, depth=0, parent=0, triangles=0):
        assert depth < 128
        t, n, stamp, version = self.header(p)
        self.counts[t] += 1
        self.versions[(version, stamp & 0xffff)] += 1
        print(f'{"  " * depth}{p:08x} {NAMES.get(t, hex(t))} '
              f'length={n} version={version:x} build={stamp & 0xffff:x}')
        q = p + 12
        if t in (11, 10):
            # WORLD: struct, materials, tree, extension; PLANE: struct, L, R.
            for _ in range(4 if t == 11 else 3):
                q = self.chunk(q, depth + 1, t)
        elif t == 9:
            _, size, _, _ = self.header(q)
            base, triangles, vertices = struct.unpack_from('<III', self.data, q + 12)
            print(f'{"  " * (depth+1)}base={base} triangles={triangles} vertices={vertices}')
            q = self.chunk(q, depth + 1, t)
            q = self.chunk(q, depth + 1, t, triangles)
        elif t == 3 and parent == 9:
            logical = 0
            while logical < n:
                _, size, _, _ = self.header(q)
                q = self.chunk(q, depth + 1, 9, triangles)
                logical += size + 12
            assert logical == n
        elif t == 0xffffff00 and parent == 9:
            tag, present = struct.unpack_from('<II', self.data, q)
            actual = 8 + (triangles if present else 0)
            assert n == actual + 4 and present in (0, 1)
            self.short_properties += 1
            q += actual
            print(f'{"  " * (depth+1)}CSF properties tag={tag} actual={actual} declared={n}')
        elif t in (3, 6, 7, 8, 16, 0x802):
            while q < p + 12 + n:
                q = self.chunk(q, depth + 1, t)
            assert q == p + 12 + n
        elif t == 94144:
            # Game placement record (6BEC60): 24 bytes, MATRIX, string length/string.
            q += 24
            _, size, _, _ = self.header(q)
            q += 12 + size
            size, = struct.unpack_from('<I', self.data, q)
            q += 4 + size
        else:
            q += n
        assert q <= len(self.data), (hex(p), hex(t), n, q, len(self.data))
        if q != p + 12 + n:
            print(f'{"  " * depth}actual-end={q:08x} declared-end={p+12+n:08x} delta={q-p-12-n}')
        return q


def main():
    ap = argparse.ArgumentParser()
    ap.add_argument('files', nargs='+', type=Path)
    args = ap.parse_args()
    for path in args.files:
        dump = Dump(path.read_bytes())
        print(f'FILE {path} bytes={len(dump.data)}')
        p = 0
        while p < len(dump.data):
            p = dump.chunk(p)
        print('COUNTS', dict(sorted(dump.counts.items())))
        print('VERSIONS (version, build)', dict(dump.versions))
        print('SHORT SECTOR PROPERTIES', dump.short_properties)


if __name__ == '__main__':
    main()
