#!/usr/bin/env python3
"""Targeted binary-vs-COFF contracts; heap/virtual callees are explicit doubles.
No Windows or game execution. Ignore only the allocator record's undefined padding.
Run after linktest.py: .venv/bin/python tests/wmrest_offline.py
"""
import pathlib, sys, struct, re
ROOT = pathlib.Path(__file__).resolve().parents[1]
sys.path.insert(0, str(ROOT / 'tools'))
import emudiff as emu, linktest, packet
from unicorn import Uc, UC_ARCH_X86, UC_MODE_32, UC_HOOK_CODE
from unicorn import x86_const as X
emu.crt_ranges = lambda: ({}, {})
TABLE, MOVED, BLOCK, MANAGER, BOX, VT = [0x30000000 + n * 0x1000 for n in range(6)]
END, VIRTUAL = 0x0f008000, 0x0f008100

def initializers(path):
    # Obj intentionally omits .CRT metadata; execute its actual dynamic binding
    # initializers just as the standalone host CRT does before game entry.
    raw = path.read_bytes()
    nsec, _, symoff, nsym = struct.unpack_from('<HIII', raw, 2)
    bases, addr = [], emu.OBJ_BASE
    for i in range(nsec):
        nm, _, _, size, _, _, _, _, _, ch = struct.unpack_from('<8sIIIIIIHHI',raw,20+40*i)
        keep = not (ch & 0x02000a00) and not nm.startswith((b'.debug',b'.llvm',b'.drectve',b'.CRT'))
        bases.append(addr if keep else None)
        if keep: addr = (addr + max(size,1) + 15) & ~15
    strings = symoff + 18*nsym
    i=0
    while i<nsym:
        nm,val,sec,_,_,aux=struct.unpack_from('<8sIhHBB',raw,symoff+18*i)
        if nm[:4]==b'\0'*4: nm=raw[strings+struct.unpack_from('<I',nm,4)[0]:].split(b'\0')[0]
        if nm.startswith(b'??__E') and sec>0 and bases[sec-1] is not None: yield bases[sec-1]+val
        i+=1+aux

def run(ea, name, filename, original, case):
    obj = emu.Obj(str(ROOT / 'build/standalone' / filename), name)
    u = Uc(UC_ARCH_X86, UC_MODE_32)
    packet.disasm(ea, 1, linear=True)
    img = packet._img
    u.mem_map(0x400000, (len(img) + 4095) & ~4095); u.mem_write(0x400000, img)
    u.mem_map(emu.OBJ_BASE, obj.end - emu.OBJ_BASE)
    for sec in obj.secs: u.mem_write(sec['base'], bytes(sec['data']))
    for at, size in [(emu.EXT_BASE, 0x10000), (emu.SYS_BASE, 0x1000), (0x10000000, 0x10000), (TABLE, 0x10000)]: u.mem_map(at, size)
    def put(at, *v): u.mem_write(at, struct.pack('<' + 'I'*len(v), *v))
    def word(at): return struct.unpack('<I', u.mem_read(at,4))[0]
    dem = linktest.demangle(list(obj.ext))
    for symbol, at in obj.ext.items():
        match = re.search(r'csf::(gv|fn_slot|vt)<(\d+)>', dem.get(symbol,''))
        if match: put(at, int(match[2]))
    put(emu.COOKIE, 0xbb40e64e); u.mem_write(emu.RET_STUB, b'\xc3')
    calls = []
    if ea == 0x6a2f90:
        count, capacity, items, size, entries, allocated = case
        put(0x8a6bac, TABLE, capacity, count, 2)
        u.mem_write(TABLE, b'\xa5'*0x400); u.mem_write(MOVED, b'\xa5'*0x400)
        for n, (block, length, available) in enumerate(entries):
            put(TABLE+n*12, block, length, 0xa5a5a500 | available)
        args = (0x79, items, size)  # invalid opaque must never be dereferenced
    else:
        live, swap = case
        put(0x8b0570, MANAGER); put(MANAGER+0xf0, BOX if live else 0, 0xdead)
        put(BOX, VT); put(VT+0x40, VIRTUAL); put(BOX+0x78, 42)
        args = ()
    def ret(value, pop=0):
        sp=u.reg_read(X.UC_X86_REG_ESP); pc=word(sp)
        u.reg_write(X.UC_X86_REG_EAX, value); u.reg_write(X.UC_X86_REG_ESP,sp+4+pop); u.reg_write(X.UC_X86_REG_EIP,pc)
    def hook(uc, pc, sz, data):
        sp=uc.reg_read(X.UC_X86_REG_ESP)
        if pc == END: uc.emu_stop(); return
        if pc == 0x6910b0:
            calls.append(('mode',word(sp+4))); ret(0)
        elif pc == 0x690dc0:
            calls.append(('alloc',word(sp+4),word(sp+8),word(sp+12))); ret(allocated)
        elif pc == 0x6d9eb0:
            assert uc.reg_read(X.UC_X86_REG_ECX) == 0x8a6bac
            calls.append(('grow',word(sp+4)))
            u.mem_write(MOVED,bytes(u.mem_read(TABLE, count*12))); put(0x8a6bac,MOVED,capacity+2)
            ret(MOVED,4)
        elif pc == VIRTUAL:
            assert uc.reg_read(X.UC_X86_REG_ECX)==BOX
            calls.append(('virtual',))
            if swap:
                put(MANAGER+0xf0, BOX+0x100); put(BOX+0x178, 73)
            ret(0)
        elif pc == 0x50f790:
            calls.append(('dispatch',uc.reg_read(X.UC_X86_REG_ECX),*(word(sp+4+i*4) for i in range(4)))); ret(123,16)
    u.hook_add(UC_HOOK_CODE,hook)
    if not original:
        for entry in initializers(ROOT / 'build/standalone' / filename):
            put(0x10008000, END); u.reg_write(X.UC_X86_REG_ESP,0x10008000)
            u.emu_start(entry,END,count=10000)
            assert u.reg_read(X.UC_X86_REG_EIP)==END
    sp=0x10008000; put(sp, END, *args)
    saved={X.UC_X86_REG_EBX:0x11111111,X.UC_X86_REG_ESI:0x22222222,X.UC_X86_REG_EDI:0x33333333,X.UC_X86_REG_EBP:0x44444444}
    for r,v in saved.items(): u.reg_write(r,v)
    u.reg_write(X.UC_X86_REG_ESP,sp)
    u.emu_start(ea if original else obj.entry, END, count=10000)
    assert u.reg_read(X.UC_X86_REG_EIP)==END
    assert u.reg_read(X.UC_X86_REG_ESP)==sp+4
    for r,v in saved.items(): assert u.reg_read(r)==v
    if ea==0x6a2f90:
        n=word(0x8a6bb4); table=word(0x8a6bac)
        records=[(word(table+i*12),word(table+i*12+4),u.mem_read(table+i*12+8,1)[0]) for i in range(n)]
        state=(word(0x8a6bb0),n,records)
    else: state=(word(MANAGER+0xf0),word(MANAGER+0xf4))
    return u.reg_read(X.UC_X86_REG_EAX), calls, state

cases=[
 (0,2,4,8,[],BLOCK),                         # fresh allocation, spare capacity
 (0,0,4,8,[],BLOCK),                         # growth relocates table
 (2,4,4,8,[(BLOCK,32,1),(BLOCK+64,32,1)],BLOCK+128), # first match reused
 (2,4,4,8,[(BLOCK,32,0),(BLOCK+64,31,1)],BLOCK+128), # busy/wrong-sized skip
 (1,2,4,8,[(0,32,1)],BLOCK),                 # null match marked busy then append
 (0,2,0xffffffff,2,[],0),                    # 32-bit wrap and failed allocation recorded
]
for case in cases:
 a=run(0x6a2f90,'CoreZlibLog_AllocateReusableBlock','src_auto_006a2f90.cpp.obj',True,case)
 b=run(0x6a2f90,'CoreZlibLog_AllocateReusableBlock','src_auto_006a2f90.cpp.obj',False,case)
 assert a==b,(case,a,b)
for case in [(False,False),(True,False),(True,True)]:
 a=run(0x50f920,'GameBddLoader_LoadCompleteCallback','standalone_game_plain.cpp.obj',True,case)
 b=run(0x50f920,'GameBddLoader_LoadCompleteCallback','standalone_game_plain.cpp.obj',False,case)
 assert a==b,(case,a,b)
print('PASS: 6 allocator and 3 load-complete callback binary/COFF cases; cdecl stack/nonvolatiles, calls and defined state')
