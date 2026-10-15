#!/usr/bin/env python3
"""Original-byte check: 4FF2F0 must pass one contiguous 12-byte descriptor.

The two native world visitors are mocked, not executed. They inspect the same
flag/filter fields consumed by 4FEE70 and 4FEF20. No Windows/game is run.
"""
from pathlib import Path
import struct
import sys

ROOT = Path(__file__).resolve().parents[1]
sys.path.insert(0, str(ROOT / 'tools'))
import emudiff as emu
from unicorn import x86_const as X

EA, LOOKUP, ACCESSOR = 0x004FF2F0, 0x00652160, 0x006B13A0
MATERIALS, CLUMPS = 0x0058CF30, 0x0058CEF0
BASE = emu.SYS_BASE + 0x500
saved_mock = emu.Run.mock
observations = []


def mock(run, target):
    u = run.uc
    sp = u.reg_read(X.UC_X86_REG_ESP)

    def word(at):
        return struct.unpack('<I', u.mem_read(at, 4))[0]

    def put(at, value):
        u.mem_write(at, struct.pack('<I', value))

    if target in (MATERIALS, CLUMPS):
        descriptor = word(sp + 12)
        first, second = word(descriptor + 4), word(descriptor + 8)
        assert (first, second) == (2, 8), (
            f'visitor {target:08x}: descriptor +4/+8 = {first:08x}/{second:08x}, expected 2/8')
        observations.append((target, bytes(u.mem_read(descriptor, 1)), first, second))
    ret = saved_mock(run, target)
    if target == LOOKUP:
        put(BASE, BASE + 0x100)
        put(BASE + 0x118, ACCESSOR)
        u.reg_write(X.UC_X86_REG_EAX, BASE)
    elif target == ACCESSOR:
        put(BASE + 0x220, 2)
        put(BASE + 0x21C, 8)
        u.reg_write(X.UC_X86_REG_EAX, BASE + 0x200)
    return ret


if __name__ == '__main__':
    source = sys.argv[1] if len(sys.argv) > 1 else str(ROOT / 'src/auto/004ff2f0.cpp')
    emu.Run.mock = mock
    try:
        verdict, detail = emu.check(EA, src=source, profiles=8, verbose=True)
    finally:
        emu.Run.mock = saved_mock
    assert verdict == 'same', (verdict, detail)
    assert len(observations) == 32, 'Both visitors must execute on both sides of every profile'
    print(f'world finaliser descriptor: {verdict} {detail}; {len(observations)} visitor field checks')
