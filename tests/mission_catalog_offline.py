"""Catalog name relocation regression; actual x86 constructor/lookup/parser.

Run linktest.py first. No Windows; allocation and variable lookup are doubles.
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
from unicorn import Uc, UC_ARCH_X86, UC_MODE_32, UC_HOOK_CODE
from unicorn import x86_const as X

emu.crt_ranges = lambda: ({}, {})
obj = emu.Obj(str(ROOT / 'build/standalone/src_auto_0076a540.cpp.obj'),
              'MissionScriptCatalog_Construct')
u = Uc(UC_ARCH_X86, UC_MODE_32)
packet.disasm(0x76a540, 1, linear=True)
u.mem_map(0x400000, (len(packet._img)+4095) & ~4095)
u.mem_write(0x400000, packet._img)
for addr, size in [(0, 0x10000), (emu.OBJ_BASE, obj.end-emu.OBJ_BASE),
                   (emu.EXT_BASE, 0x10000), (emu.SYS_BASE, 0x1000),
                   (0x10000000, 0x10000), (0x30000000, 0x10000)]:
    u.mem_map(addr, size)
for sec in obj.secs:
    u.mem_write(sec['base'], bytes(sec['data']))


def put(at, *values): u.mem_write(at, struct.pack('<'+'I'*len(values), *values))
def word(at): return struct.unpack('<I', u.mem_read(at, 4))[0]
def text(at): return bytes(u.mem_read(at, 256)).split(b'\0')[0]


for sym, name in linktest.demangle(list(obj.ext)).items():
    m = re.search(r'csf::(gv|fn_slot|vt)<(\d+)>', name)
    if m: put(obj.ext[sym], int(m[2]))
put(emu.COOKIE, 0xbb40e64e)
END, SELF, OUT, DEF, NODE, QUERY = 0x0f008000, 0x30000000, 0x30000100, 0x30000200, 0x30000300, 0x30000400
u.mem_write(QUERY, b'VAR\0')
heap = 0x30001000
names = []
registrations = {0x76a1b0: 6, 0x76a230: 7, 0x76a2c0: 8,
                 0x76a350: 9, 0x76a3e0: 10, 0x76a470: 11}


def ret(value=0, pop=0):
    sp = u.reg_read(X.UC_X86_REG_ESP)
    u.reg_write(X.UC_X86_REG_EAX, value)
    u.reg_write(X.UC_X86_REG_ESP, sp+4+pop)
    u.reg_write(X.UC_X86_REG_EIP, word(sp))


def hook(uc, pc, size, data):
    global heap
    sp = uc.reg_read(X.UC_X86_REG_ESP)
    if pc == END: uc.emu_stop()
    elif pc in registrations:
        names.append(text(word(sp+16)))
        ret(1, registrations[pc]*4)
    elif pc == 0x769fc0: ret(0, 4)
    elif pc == 0x769c70: ret()
    elif pc == 0x7b0c41:
        a, b = text(word(sp+4)).lower(), text(word(sp+8)).lower()
        ret(0 if a == b else 1)
    elif pc == 0x692bd0: ret(NODE, 4)
    elif pc == 0x6927f0: ret(3)
    elif pc == 0x692c40: ret(0)
    elif pc == 0x46be40: ret(0, 4)
    elif pc == 0x46be70: ret(DEF, 4)
    elif pc in (0x471450, 0x69b460):
        result = heap
        heap += 64
        ret(result)


u.hook_add(UC_HOOK_CODE, hook)


def run(pc, self=SELF, args=()):
    put(0x10008000, END, *args)
    u.reg_write(X.UC_X86_REG_ESP, 0x10008000)
    u.reg_write(X.UC_X86_REG_ECX, self)
    u.emu_start(pc, 0, count=500000)
    assert u.reg_read(X.UC_X86_REG_EIP) == END, hex(u.reg_read(X.UC_X86_REG_EIP))
    assert u.reg_read(X.UC_X86_REG_ESP) == 0x10008004 + 4*len(args)
    return u.reg_read(X.UC_X86_REG_EAX)


run(0x76a540)
original = names[:]
names.clear()
# Reproduce datagen's actual ambiguity: these text words were applied as
# code fixups, not left as the original bytes. Only standalone literals are
# safe here (the original constructor was run before corrupting its image).
for address, expected in [(0x8718c4, b'VAR'), (0x87185c, b'ANM'), (0x8716a0, b'FBS')]:
    assert text(address) == expected
    put(address, 0x0f001234)
run(obj.entry)
assert names == original and len(names) == 422
assert b'VAR' in names and b'ANM' in names
print('catalog: all 422 original names match standalone literals (including VAR/ANM)')

# Exact failure mechanism: lookup falls back to NO_FUNCTION (kind 2, opcode
# 0). Its parser branch constructs a value with no payload. VAR instead goes
# through opcode 10 and reserves a typed payload from the variable definition.
table, fallback, variable, sentinel, badname = [0x30000500+i*0x100 for i in range(5)]
put(SELF+4, table)
put(table, fallback, variable, sentinel)
put(fallback, 2, 0, 0, 0x8718cc)
put(variable, 0, 10, 3, badname)
put(sentinel+4, 0xffffffff)
put(DEF+4, 3)
u.mem_write(badname, b'not VAR\0')  # stand-in for relocated trap-pointer bytes
for valid in (False, True):
    if valid: put(variable+12, QUERY)
    definition = run(0x7692f0, args=(QUERY,))
    assert definition == (variable if valid else fallback)
    put(OUT, 0xffff0000, 0)
    run(0x46e590, args=(NODE, definition, OUT))
    value = word(OUT+4)
    assert value and word(value+8) == (0 if valid else 0xffffffff)
    assert bool(word(value+4)) == valid
print('original parser: corrupt VAR name -> null payload; correct name -> typed payload')
