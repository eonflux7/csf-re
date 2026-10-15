"""Mission leave/orphan page slots vs original bytes; no Windows execution.

Explicit doubles check call order/arguments, data writes, stack balance and
callee-saved registers for both orphan entries. Run tools/linktest.py first.
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


def run(ea, original, mode):
    path = ROOT / 'build/standalone/standalone_game_plain.cpp.obj'
    obj = emu.Obj(str(path), ('LeaveLevel' if ea == 0x537490 else 'OpenNextPage')+'@leave_level')
    u = Uc(UC_ARCH_X86, UC_MODE_32)
    packet.disasm(ea, 1, linear=True)
    img = packet._img
    u.mem_map(0x400000, (len(img)+4095) & ~4095)
    u.mem_write(0x400000, img)
    u.mem_map(emu.OBJ_BASE, obj.end-emu.OBJ_BASE)
    for sec in obj.secs:
        u.mem_write(sec['base'], bytes(sec['data']))
    for at, size in [(emu.EXT_BASE, 0x10000), (emu.SYS_BASE, 0x1000),
                     (0x10000000, 0x10000), (0x30000000, 0x20000)]:
        u.mem_map(at, size)

    def put(at, *words): u.mem_write(at, struct.pack('<'+'I'*len(words), *words))
    def word(at): return struct.unpack('<I', u.mem_read(at, 4))[0]
    dem = linktest.demangle(list(obj.ext))
    for symbol, at in obj.ext.items():
        match = re.search(r'csf::(gv|fn_slot|vt)<(\d+)>', dem.get(symbol, ''))
        if match: put(at, int(match[2]))
    put(emu.COOKIE, 0xbb40e64e)
    page, app, loader, controller, menu, selected, config = [0x30000000+i*0x1000 for i in range(7)]
    vts, virtuals = {}, {}
    for i, ob in enumerate([page, app, loader, controller, menu]):
        vt = 0x30010000+i*0x100
        put(ob, vt)
        vts[ob] = vt

    def slot(ob, off, count, value=0):
        pc = 0x0f009000+len(virtuals)*16
        put(vts[ob]+off, pc)
        virtuals[pc] = (ob, off, count, value)
    slot(page, 0x78, 0)
    slot(app, 0x5c, 1)
    slot(loader, 0x2c, 0)
    slot(controller, 0x28, 0)
    slot(menu, 0x5c, 1, selected)
    slot(menu, 0x74, 2, 1)
    slot(menu, 0x60, 2)
    slot(menu, 0x58, 1)
    put(page+0x18, menu)
    put(page+0x14c, selected if mode == 'mp' else 0)
    put(app+0x6c, selected if mode == 'intro' else 0)
    u.mem_write(app+0xd0, bytes([mode == 'mp']))
    u.mem_write(loader+0xb5, b'\1')
    put(loader+0x5c, 1 if mode == 'skip' else 0)
    u.mem_write(config+0xa6c, bytes([mode == 'ready']))
    for mode_name, off in [('return', 0x187), ('special', 0x142), ('confirm', 0x271)]:
        u.mem_write(page+off, bytes([mode == mode_name]))
    # Exercise both SSO and heap-backed strings.
    put(page+4+0x174, 16 if mode == 'intro' else 0)
    put(page+4+0x160, config+0x800)
    put(config+0x21d0, 16 if mode == 'intro' else 0)
    put(config+0x21bc, config+0x900)
    for a in [0x899b14,0x899ae8,0x899b2c,0x899ad0,0x899b0c,0x899b98,0x899ad8]: put(a, a+0x100)
    calls = []
    getters = {0x50d190:app, 0x521bb0:loader, 0x4fdbb0:loader, 0x4fbde0:loader,
               0x6b1380:controller, 0x69b700:loader, 0x75fdf0:config, 0x756a10:config}
    counts = {0x4754d0:1, 0x501990:2, 0x69bd90:2, 0x542750:1, 0x6f2850:1}
    methods = {0x50d230,0x523640,0x4fc440,0x503790,0x536370,0x4fe490,
               0x457000,0x4fe4a0,0x44c510,0x652010,0x501990,0x69bd90,
               0x50c880,0x50c860,0x6f2850,0x542750}
    bodies = [(0x537490,0x537613),(0x536f20,0x5370b6)]
    def ret(value, pop):
        sp=u.reg_read(X.UC_X86_REG_ESP)
        u.reg_write(X.UC_X86_REG_EAX, value)
        u.reg_write(X.UC_X86_REG_ESP, sp+4+pop)
        u.reg_write(X.UC_X86_REG_EIP, word(sp))
    active = False
    def hook(uc, pc, size, data):
        if pc == END: uc.emu_stop(); return
        if not active: return
        if pc in virtuals:
            ob, off, n, value = virtuals[pc]
            assert uc.reg_read(X.UC_X86_REG_ECX) == ob
            sp=uc.reg_read(X.UC_X86_REG_ESP)
            calls.append(('v',ob,off,tuple(word(sp+4+4*i) for i in range(n))))
            ret(value,n*4); return
        if any(a <= pc < b for a,b in bodies): return
        if 0x400000 <= pc < 0x800000:
            n=counts.get(pc,0); sp=uc.reg_read(X.UC_X86_REG_ESP)
            args=tuple(word(sp+4+4*i) for i in range(n))
            if pc == 0x69bd90:
                assert bytes(uc.mem_read(args[1],1)) == b'\0'
                args=(args[0], '<empty>')  # CSF_STR literal, not address identity
            ecx=uc.reg_read(X.UC_X86_REG_ECX) if pc in methods else None
            calls.append((pc,ecx,args))
            if pc == 0x50c860: uc.mem_write(ecx,b'\0'*0x13);uc.mem_write(ecx+0x10,b'\1')
            if pc == 0x6f2850: put(ecx+0x24,args[0])
            ret(getters.get(pc, controller if pc == 0x652010 else 1 if pc == 0x50c880 else loader),
                n*4 if pc in methods else 0)
    u.hook_add(UC_HOOK_CODE, hook)
    for entry in initializers(path):
        put(0x10008000, END);u.reg_write(X.UC_X86_REG_ESP,0x10008000)
        u.emu_start(entry,0,count=10000)
    active=True
    put(0x10008000,END)
    u.reg_write(X.UC_X86_REG_ESP,0x10008000)
    u.reg_write(X.UC_X86_REG_ECX,page+4 if ea==0x537490 else page)
    for reg,val in [(X.UC_X86_REG_EBX,0x1234),(X.UC_X86_REG_ESI,0x2345),
                    (X.UC_X86_REG_EDI,0x3456),(X.UC_X86_REG_EBP,0x4567)]:u.reg_write(reg,val)
    u.emu_start(ea if original else obj.entry,0,count=100000)
    assert u.reg_read(X.UC_X86_REG_EIP)==END
    assert u.reg_read(X.UC_X86_REG_ESP)==0x10008004
    assert [u.reg_read(r) for r in [X.UC_X86_REG_EBX,X.UC_X86_REG_ESI,X.UC_X86_REG_EDI,X.UC_X86_REG_EBP]]==[0x1234,0x2345,0x3456,0x4567]
    return calls,bytes(u.mem_read(page,0x20000))


for ea in [0x537490,0x536f20]:
    for mode in ['default','skip','mp','ready','intro','return','special','confirm']:
        a,b=run(ea,True,mode),run(ea,False,mode)
        assert a==b,(hex(ea),mode,a[0],b[0])
        print(f'{ea:08X} {mode}: same (calls, data, ESP, preserved registers)')
