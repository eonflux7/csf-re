"""Original bytes vs freshly linktested x86 COFF; no Windows execution.

Explicit doubles test the dialog's contiguous wstring contract and the missed
sound-manager destructor entries. Run tools/linktest.py first.
"""
import pathlib
import re
import struct
import sys

ROOT = pathlib.Path(__file__).resolve().parents[1]
sys.path.insert(0, str(ROOT / 'tools'))
import emudiff as emu
import linktest
import packet
from unicorn import Uc, UC_ARCH_X86, UC_MODE_32, UC_HOOK_CODE, UC_HOOK_MEM_INVALID
from unicorn import x86_const as X

emu.crt_ranges = lambda: ({}, {})
DATA, CAPTION, TEXT, HEAP, WINDOW = [0x30000000 + i * 0x1000 for i in range(5)]
END, MESSAGE = 0x0f008000, 0x0f008100


def initializers(path):
    raw = path.read_bytes()
    nsec, _, symoff, nsym = struct.unpack_from('<HIII', raw, 2)
    bases, addr = [], emu.OBJ_BASE
    for i in range(nsec):
        nm, _, _, size, _, _, _, _, _, ch = struct.unpack_from('<8sIIIIIIHHI', raw, 20+40*i)
        keep = not (ch & 0x02000a00) and not nm.startswith((b'.debug', b'.llvm', b'.drectve', b'.CRT'))
        bases.append(addr if keep else None)
        if keep:
            addr = (addr + max(size, 1) + 15) & ~15
    strings = symoff + 18*nsym
    i = 0
    while i < nsym:
        nm, val, sec, _, _, aux = struct.unpack_from('<8sIhHBB', raw, symoff+18*i)
        if nm[:4] == b'\0'*4:
            nm = raw[strings+struct.unpack_from('<I', nm, 4)[0]:].split(b'\0')[0]
        if nm.startswith(b'??__E') and sec > 0 and bases[sec-1] is not None:
            yield bases[sec-1] + val
        i += 1 + aux


def run(ea, name, filename, original, case):
    path = ROOT / 'build/standalone' / filename
    obj = emu.Obj(str(path), name)
    u = Uc(UC_ARCH_X86, UC_MODE_32)
    packet.disasm(ea, 1, linear=True)
    img = packet._img
    u.mem_map(0x400000, (len(img)+4095) & ~4095)
    u.mem_write(0x400000, img)
    u.mem_map(emu.OBJ_BASE, obj.end-emu.OBJ_BASE)
    for sec in obj.secs:
        u.mem_write(sec['base'], bytes(sec['data']))
    for at, size in [(emu.EXT_BASE, 0x10000), (emu.SYS_BASE, 0x1000),
                     (0x10000000, 0x10000), (DATA, 0x10000)]:
        u.mem_map(at, size)

    def put(at, *words):
        u.mem_write(at, struct.pack('<'+'I'*len(words), *words))

    def word(at):
        return struct.unpack('<I', u.mem_read(at, 4))[0]

    def wide(at):
        chars = []
        if not at:
            return None
        for i in range(256):
            ch = struct.unpack('<H', u.mem_read(at+i*2, 2))[0]
            if not ch:
                return ''.join(chars)
            chars.append(chr(ch))
        raise AssertionError('unterminated MessageBox text')

    dem = linktest.demangle(list(obj.ext))
    for symbol, at in obj.ext.items():
        match = re.search(r'csf::(gv|fn_slot|vt)<(\d+)>', dem.get(symbol, ''))
        if match:
            put(at, int(match[2]))
    put(emu.COOKIE, 0xbb40e64e)
    u.mem_write(emu.RET_STUB, b'\xc3')
    for native, code in emu.NATIVE.items():
        u.mem_write(emu.NATIVE_AT[native], bytes.fromhex(code))
    calls = []
    dialog = ea == 0x69ce50
    if dialog:
        content, mode = case
        u.mem_write(CAPTION, 'ERROR\0'.encode('utf-16le'))
        u.mem_write(TEXT, (content+'\0').encode('utf-16le'))
        put(0x82d290, MESSAGE)
        put(0x8bafd0, WINDOW)
        args = (CAPTION, TEXT, 0x44)
        this = 0
    else:
        offset, flags = case
        args = (flags,)
        this = DATA + offset

    def ret(value, pop=0):
        sp = u.reg_read(X.UC_X86_REG_ESP)
        pc = word(sp)
        u.reg_write(X.UC_X86_REG_EAX, value)
        u.reg_write(X.UC_X86_REG_ESP, sp+4+pop)
        u.reg_write(X.UC_X86_REG_EIP, pc)

    def hook(uc, pc, size, data):
        sp = uc.reg_read(X.UC_X86_REG_ESP)
        if pc == END:
            uc.emu_stop()
        elif pc == obj.ext.get('_memset'):
            dest, value, length = (word(sp+4+i*4) for i in range(3))
            uc.mem_write(dest, bytes([value & 255])*length)
            ret(dest)
        elif dialog and pc == 0x696f50:
            dest = uc.reg_read(X.UC_X86_REG_ECX)
            assert word(sp+4) == TEXT
            # ConstructWideString creates the by-value source for the formatter.
            put(dest, 0, TEXT, 0, 0, 0, len(content), 31)
            calls.append(('construct', wide(TEXT)))
            ret(dest, 4)
        elif dialog and pc == 0x6966e0:
            dest = word(sp+4)
            assert word(dest+0x18) == 7 and word(dest+0x14) == 0
            assert bytes(uc.mem_read(dest+4, 2)) == b'\0\0'
            assert word(sp+12) == TEXT and word(sp+28) == len(content)
            assert word(sp+32) == 31
            calls.append(('format', wide(TEXT)))
            if mode == 'inline':
                assert len(content) < 8
                uc.mem_write(dest+4, (content+'\0').encode('utf-16le'))
                put(dest+0x14, len(content), 7)
            else:
                uc.mem_write(HEAP, (content+'\0').encode('utf-16le'))
                put(dest+4, HEAP if mode == 'heap' else 0)
                put(dest+0x14, len(content), 31)
            ret(dest)
        elif dialog and pc == MESSAGE:
            args = [word(sp+4+i*4) for i in range(4)]
            calls.append(('message', args[0], wide(args[1]), wide(args[2]), args[3]))
            ret(77, 16)
        elif dialog and pc == 0x690e60:
            calls.append(('free', word(sp+4)))
            ret(91)
        elif not dialog and pc == 0x5238b0:
            calls.append(('reset', uc.reg_read(X.UC_X86_REG_ECX)))
            # Verify destructor retains this even if the reset helper clobbers eax.
            ret(0x12345678)
        elif not dialog and pc == 0x691110:
            calls.append(('free', word(sp+4)))
            ret(0x87654321)

    u.hook_add(UC_HOOK_CODE, hook)
    def invalid(uc, access, address, size, value, data):
        raise AssertionError((hex(ea), original, case, 'unmapped',
                              hex(uc.reg_read(X.UC_X86_REG_EIP)), hex(address), calls))
    u.hook_add(UC_HOOK_MEM_INVALID, invalid)
    if not original:
        for entry in initializers(path):
            put(0x10008000, END)
            u.reg_write(X.UC_X86_REG_ESP, 0x10008000)
            u.emu_start(entry, END, count=10000)
            assert u.reg_read(X.UC_X86_REG_EIP) == END
    sp = 0x10008000
    put(sp, END, *args)
    saved = {X.UC_X86_REG_EBX: 0x11111111, X.UC_X86_REG_ESI: 0x22222222,
             X.UC_X86_REG_EDI: 0x33333333, X.UC_X86_REG_EBP: 0x44444444}
    for reg, value in saved.items():
        u.reg_write(reg, value)
    u.reg_write(X.UC_X86_REG_ECX, this)
    u.reg_write(X.UC_X86_REG_ESP, sp)
    try:
        u.emu_start(ea if original else obj.entry, END, count=10000)
    except Exception as err:
        raise AssertionError((hex(ea), original, case, hex(u.reg_read(X.UC_X86_REG_EIP)), calls)) from err
    assert u.reg_read(X.UC_X86_REG_EIP) == END
    assert u.reg_read(X.UC_X86_REG_ESP) == sp+(4 if dialog else 8)
    for reg, value in saved.items():
        assert u.reg_read(reg) == value
    return u.reg_read(X.UC_X86_REG_EAX), calls


for content, mode in [('ERROR', 'inline'), ('Sound initialization failed.', 'heap'), ('', 'null-heap')]:
    case = (content, mode)
    a = run(0x69ce50, 'Dialog_ShowLocalizedMessage', 'src_auto_0069ce50.cpp.obj', True, case)
    b = run(0x69ce50, 'Dialog_ShowLocalizedMessage', 'src_auto_0069ce50.cpp.obj', False, case)
    assert a == b, (case, a, b)
    assert b[1][2] == ('message', WINDOW, content if mode != 'null-heap' else None, 'ERROR', 0x30)
for ea, name, offset in [(0x5240f0, 'CSoundManager_ScalarDelete', 0),
                         (0x523920, 'CSoundManager_DeleteFromBase4', 4),
                         (0x523930, 'CSoundManager_DeleteFromBase8', 8)]:
    for flags in [0, 1, 2, 3, 0x100]:
        a = run(ea, name, 'standalone_game_plain.cpp.obj', True, (offset, flags))
        b = run(ea, name, 'standalone_game_plain.cpp.obj', False, (offset, flags))
        expected = [('reset', DATA)] + ([('free', DATA)] if flags & 1 else [])
        assert a == b == (DATA, expected), (ea, flags, a, b)
print('PASS: 3 dialog wstring and 15 deleting-destructor binary/COFF cases; text/calls, stack and nonvolatiles')
