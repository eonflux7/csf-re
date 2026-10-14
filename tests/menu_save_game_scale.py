#!/usr/bin/env python3
"""Exercise 544780's uncached path against the original bytes, offline.

Run after linktest.py: .venv/bin/python tests/menu_save_game_scale.py [OBJECT]
To reproduce with a pre-fix object, append --regs (its old entry wrapper).
The owner getter executes the original 68B5F0 bytes (plain ret). Other
collaborators are fixtures, not a GUI/RW or Windows execution test.
"""
import pathlib
import re
import struct
import sys

ROOT = pathlib.Path(__file__).resolve().parents[1]
sys.path.insert(0, str(ROOT / 'tools'))
import emudiff as emu
import linktest
from unicorn import Uc, UcError, UC_ARCH_X86, UC_MODE_32, UC_HOOK_CODE
from unicorn import x86_const as X

EA = 0x544780
SELF, SCREEN, HOLDER, ROOT_OBJECT, NODE, PANEL, OWNER = (
    0x30001000, 0x30002000, 0x30003000, 0x30004000,
    0x30005000, 0x30006000, 0x30007000)
SCREEN_VT, ROOT_VT, PANEL_VT = 0x30008000, 0x30009000, 0x3000A000
GET_ROOT, GET_PANEL, SCALE, END = 0x3000B000, 0x3000B100, 0x3000B200, 0x3000B300
SP = 0x3001F000


def run(obj, cached=0, found=True, initializer=None):
    u = Uc(UC_ARCH_X86, UC_MODE_32)
    img, _ = emu.exe_image()
    u.mem_map(emu.IMG, (len(img) + 4095) & ~4095)
    u.mem_write(emu.IMG, img)
    u.mem_map(0x30000000, 0x20000)
    u.reg_write(X.UC_X86_REG_FPCW, 0x027F)

    def put(at, *values):
        u.mem_write(at, struct.pack('<' + 'I' * len(values), *values))

    def word(at):
        return struct.unpack('<I', u.mem_read(at, 4))[0]

    if obj:
        u.mem_map(emu.OBJ_BASE, obj.end - emu.OBJ_BASE)
        for section in obj.secs:
            u.mem_write(section['base'], bytes(section['data']))
        u.mem_map(emu.EXT_BASE, 0x10000)
        names = linktest.demangle(list(obj.ext))
        for name, at in obj.ext.items():
            match = re.search(r'csf::(?:fn_slot|gv)<(\d+)>', names[name])
            assert match, name
            put(at, int(match[1]))
        # CSF_GLOBAL binds references through CRT startup, just as in the exe.
        put(SP, END)
        u.reg_write(X.UC_X86_REG_ESP, SP)
        u.emu_start(initializer, END, count=10000)
        assert u.reg_read(X.UC_X86_REG_EIP) == END

    put(SELF + 0x44, cached)
    put(SCREEN, SCREEN_VT)
    put(SCREEN_VT + 0x54, GET_ROOT)
    put(HOLDER, NODE)
    # This is a decoy word, not a freed panel. The broken caller's ESP is
    # eight bytes low, so its saved-self load instead fetches HOLDER.
    put(HOLDER + 0x44, 0xFEEEFEEE)
    put(ROOT_OBJECT, ROOT_VT)
    put(ROOT_VT + 0x18, GET_PANEL)
    put(PANEL, PANEL_VT)
    put(PANEL_VT + 0x150, SCALE)
    put(OWNER, 0x861460, 2560, 1440)
    assert word(0x861460 + 0x18) == 0x68B5F0
    put(0x899B9C, 800, 600)
    calls, scales, getter_stacks = [], [], []

    def hook(uc, pc, size, data):
        if pc == END:
            uc.emu_stop()
            return
        if pc == 0x68B5F0:
            getter_stacks.append(uc.reg_read(X.UC_X86_REG_ESP))
            return  # execute the original lea eax,[ecx+4]; ret
        if pc not in (GET_ROOT, GET_PANEL, SCALE, 0x693360, 0x6B1380, 0x652160):
            return
        calls.append(pc)
        esp = uc.reg_read(X.UC_X86_REG_ESP)
        ecx = uc.reg_read(X.UC_X86_REG_ECX)
        pop = 0
        if pc == GET_ROOT:
            assert ecx == SCREEN
            result = ROOT_OBJECT
        elif pc == 0x693360:
            assert ecx == NODE
            name = bytes(uc.mem_read(word(esp + 4), 17))
            assert name == b'.PANEL_PRINCIPAL\0', name
            result, pop = NODE, 4
        elif pc == GET_PANEL:
            assert ecx == ROOT_OBJECT and word(esp + 4) == NODE
            result, pop = (PANEL if found else 0), 4
        elif pc == 0x6B1380:
            result = 0x3000C000
        elif pc == 0x652160:
            assert ecx == 0x3000C000
            result = OWNER
        else:
            assert ecx == PANEL
            scales.append(struct.unpack('<ff', uc.mem_read(esp + 4, 8)))
            result, pop = 123, 8
        uc.reg_write(X.UC_X86_REG_EAX, result)
        uc.reg_write(X.UC_X86_REG_EIP, word(esp))
        uc.reg_write(X.UC_X86_REG_ESP, esp + 4 + pop)

    u.hook_add(UC_HOOK_CODE, hook)
    put(SP, END, SCREEN, HOLDER)
    saved = {X.UC_X86_REG_EBX: 0x11223344, X.UC_X86_REG_ESI: 0x55667788,
             X.UC_X86_REG_EDI: 0x99AABBCC, X.UC_X86_REG_EBP: SP + 0x100}
    for reg, value in saved.items():
        u.reg_write(reg, value)
    u.reg_write(X.UC_X86_REG_ESP, SP)
    u.reg_write(X.UC_X86_REG_ECX, SELF)
    try:
        u.emu_start(obj.entry if obj else EA, END + 1, count=10000)
    except UcError:
        print(f'fault at {u.reg_read(X.UC_X86_REG_EIP):08X}, '
              f'eax={u.reg_read(X.UC_X86_REG_EAX):08X}; '
              f'getter stacks={[hex(s) for s in getter_stacks]}')
        raise
    assert u.reg_read(X.UC_X86_REG_EIP) == END
    assert u.reg_read(X.UC_X86_REG_ESP) == SP + 12
    assert all(u.reg_read(reg) == value for reg, value in saved.items())
    return u.reg_read(X.UC_X86_REG_EAX), word(SELF + 0x44), calls, scales


path = sys.argv[1] if len(sys.argv) > 1 else str(
    ROOT / 'build/standalone/src_auto_00544780.cpp.obj')
entry_name = emu.hook_name(EA) + ('_regs' if '--regs' in sys.argv else '')
obj = emu.Obj(path, entry_name)
initializer = emu.Obj(path, '_GLOBAL__sub_I_00544780.cpp').entry
for cached, found in ((0, True), (PANEL, True), (0, False)):
    original = run(None, cached, found)
    ours = run(obj, cached, found, initializer)
    assert original == ours, (original, ours)
    if not cached and found:
        assert len(ours[2]) == 8 and len(ours[3]) == 1
        assert all(abs(a - b) < 1e-6 for a, b in zip(ours[3][0], (3.2, 2.4)))
print('PASS: original/COFF uncached, cached, missing-panel paths; scale, cleanup, saved registers')
