#!/usr/bin/env python3
"""Exercise AddNamedEntry's new-name path and its nested-record contract.

Run: .venv/bin/python tests/profile_new_record.py [SOURCE]
The original bytes and freshly compiled candidate run under emudiff. External
collaborators are fixtures, not a Windows/heap test. In particular the reset
fixture checks its old buffer before installing a synthetic allocation for 20
16-byte vector elements (reserve only: the vector's size remains zero).
The default emudiff profiles all take FindNamedEntry's early return, missing this
contract. A pre-fix source fails at the reset's read of record+0x38.
"""
import pathlib
import struct
import sys

ROOT = pathlib.Path(__file__).resolve().parents[1]
sys.path.insert(0, str(ROOT / 'tools'))
import emudiff as emu
from unicorn import x86_const as X

EA = 0x005172C0
FIND, PROVIDER, RESET, ASSIGN, SETTINGS, APPEND, FREE = (
    0x00515FC0, 0x00506BD0, 0x005166E0, 0x00488830,
    0x0075FDF0, 0x00517230, 0x00690E60)
# A fixture callback with the provider slot's one-stack-argument ABI.
CALLBACK = 0x005173B0
BASE = emu.SYS_BASE + 0x500
ALLOCATION = BASE + 0x400
saved_mock = emu.Run.mock
observations = []


def mock(run, target):
    u = run.uc
    ecx = u.reg_read(X.UC_X86_REG_ECX)
    sp = u.reg_read(X.UC_X86_REG_ESP)

    def word(at):
        return struct.unpack('<I', u.mem_read(at, 4))[0]

    def put(at, *values):
        u.mem_write(at, struct.pack('<' + 'I' * len(values), *values))

    if target == RESET:
        run.profile_record = ecx
        run.profile_frees = 0
        old_buffer = word(ecx + 0x38)
        assert old_buffer == 0, (
            f'ResetSaveData would free out-of-record pointer {old_buffer:08x}')
        assert word(ecx + 0x18) == 7 and word(ecx + 0x14) == 0
        # Reset creates the embedded vector and clears the record's flags.
        put(ecx + 0x38, ALLOCATION, ALLOCATION, ALLOCATION + 20 * 16)
        put(ecx + 0x44, 0, 0xFFFFFFFF, 0)
    elif target == ASSIGN:
        assert ecx == run.profile_record
        u.mem_write(ecx + 4, 'BOT\0'.encode('utf-16le'))
        put(ecx + 0x14, 3)
    elif target == CALLBACK:
        assert word(sp + 4) == run.profile_record + 0x1C, (
            'Provider details must alias the tail of the save record')
        assert word(word(sp + 4) + 0x1C) == ALLOCATION
    elif target == APPEND:
        record = word(sp + 4)
        assert record == run.profile_record
        assert word(record + 0x38) == ALLOCATION
        observations.append(bytes(u.mem_read(record, 0x50)))
    elif target == FREE:
        assert word(sp + 4) == ALLOCATION
        run.profile_frees += 1
        assert run.profile_frees == 1

    ret = saved_mock(run, target)
    if target == FIND:
        u.reg_write(X.UC_X86_REG_EAX, 0xFFFFFFFF)  # no existing BOT
    elif target == PROVIDER:
        put(BASE + 4, BASE + 0x100)
        put(BASE + 0x100, BASE + 0x200)
        put(BASE + 0x20C, CALLBACK)
        u.reg_write(X.UC_X86_REG_EAX, BASE)
    elif target == SETTINGS:
        u.mem_write(BASE + 0x3CC, b'\0')
        u.reg_write(X.UC_X86_REG_EAX, BASE + 0x300)
    return ret


if __name__ == '__main__':
    source = sys.argv[1] if len(sys.argv) > 1 else str(ROOT / 'src/auto/005172c0.cpp')
    emu.Run.mock = mock
    try:
        verdict, detail = emu.check(EA, src=source, profiles=8, verbose=True)
    finally:
        emu.Run.mock = saved_mock
    assert verdict == 'same', (verdict, detail)
    assert len(observations) == 16, 'Every profile must append on both sides'
    print(f'new-profile nested record: {verdict} {detail}; reset/provider/append/free checked')
