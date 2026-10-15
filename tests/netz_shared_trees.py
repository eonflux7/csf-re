#!/usr/bin/env python3
"""Original-byte vs fresh-source checks on concrete VC7.1 iterator graphs.

Default emudiff inputs finish only 2/8 profiles for these pointer walks. Cover
head, every node of a balanced seven-node tree, and a one-node tree explicitly.
No game/Windows execution or library implementation source is used.
"""
import pathlib
import subprocess
import sys
import tempfile

ROOT = pathlib.Path(__file__).resolve().parents[1]
sys.path.insert(0, str(ROOT / 'tools'))
import emudiff as e
import gate
from emuprofiles import Graph


def graph(count, current):
    g = Graph()
    nodes = [g.alloc(str(n)) for n in range(count + 1)]
    head = nodes[0]
    # head's +0x15 flag and tree links are the exact offsets in both binaries.
    g.put(head + 20, 0x100)
    if count == 7:
        root, low, high = 4, 1, 7
        for n in range(1, 8):
            left = n - 1 if n in (2, 4, 6) else 0
            right = n + 1 if n in (2, 4, 6) else 0
            if n == 4:
                left, right = 2, 6
            parent = 0 if n == 4 else (4 if n in (2, 6) else (2 if n < 4 else 6))
            for offset, index in ((0, left), (4, parent), (8, right)):
                g.put(nodes[n] + offset, nodes[index])
    else:
        root = low = high = 1
        for offset in (0, 4, 8):
            g.put(nodes[1] + offset, head)
    for offset, index in ((0, low), (4, root), (8, high)):
        g.put(head + offset, nodes[index])
    g.put(e.ECX0, nodes[current])
    return g


def main():
    total = 0
    with tempfile.TemporaryDirectory() as directory:
        for ea in (0x660630, 0x665E40):
            source = ROOT / f'src/auto/{ea:08x}.cpp'
            path = str(pathlib.Path(directory) / f'{ea:x}.obj')
            subprocess.run(gate.compile_cmd() + [f'/Fo{path}', '-c', '--', str(source)],
                           cwd=ROOT / 'build', check=True, capture_output=True)
            obj = e.Obj(path, e.hook_name(ea, str(source)))
            own, ret_site = e.own_ranges(ea)
            for count in (1, 7):
                for current in range(count + 1):
                    g = graph(count, current)
                    original = e.Run(ea, 0, own, memory_obj=obj, graph=g)
                    original.ret_site = ret_site
                    original.start(ea)
                    candidate = e.Run(ea, 0, obj.code, obj, original.events, graph=g).start(obj.entry)
                    assert original.status == candidate.status == 'ret', (ea, count, current)
                    differences = e.compare(ea, original, candidate)
                    assert not differences, (hex(ea), count, current, differences)
                    total += 1
    print(f'NetZ shared tree iterators: {total} concrete original/candidate cases same')


if __name__ == '__main__':
    main()
