#!/usr/bin/env python3
"""Focused original-byte fixtures for the misnamed physics toolkit slots.
Not a general original/COFF differential test. Native counterparts live in
rw_team.cpp; this checks the binary-derived contracts independently of C++.
"""
import pathlib
import struct
import sys
ROOT = pathlib.Path(__file__).resolve().parents[1]
sys.path.insert(0, str(ROOT / 'tools'))
import emudiff
from unicorn import Uc, UC_ARCH_X86, UC_MODE_32, UC_HOOK_CODE
from unicorn import x86_const as X

u = Uc(UC_ARCH_X86, UC_MODE_32)
image, _ = emudiff.exe_image()
u.mem_map(emudiff.IMG, (len(image)+4095)&~4095)
u.mem_write(emudiff.IMG, image)
# RwSqrt's lookup table normally comes from engine startup. Supply x87 sqrt
# arithmetic, rather than inventing initialized engine table contents.
u.mem_write(0x622ee0,b'\xd9\x44\x24\x04\xd9\xfa\xc3')
assert b'//Physics/Rwp37Active/src/core/context/src/RwpContext.c' in image
assert b'//Physics/Rwp37Active/src/core/volume/src/RwpNull.c' in image
BASE, STACK, END = 0x30000000, 0x30100000, 0x30110000
u.mem_map(BASE, 0x120000)
def word(p, v=None):
    if v is not None: u.mem_write(p, struct.pack('<I', v & 0xffffffff))
    return struct.unpack('<I', u.mem_read(p, 4))[0]
def floats(p, values): u.mem_write(p, struct.pack('<'+'f'*len(values), *values))
def fwords(p, n): return struct.unpack('<'+'f'*n, u.mem_read(p, n*4))
def fb(v): return struct.unpack('<I', struct.pack('<f', v))[0]
starts, stops = [], []
START_CB, STOP_CB = END+0x100, END+0x200
PROPERTIES_CB, VOLUME = END+0x300, END+0x400
floats(VOLUME,[8])
u.mem_write(PROPERTIES_CB,b'\xd9\x05'+struct.pack('<I',VOLUME)+b'\xc3')
def hook(uc, pc, size, data):
    if pc == END: uc.emu_stop()
    if pc == PROPERTIES_CB:
        sp=uc.reg_read(X.UC_X86_REG_ESP)
        floats(word(sp+8),[2,3,4,0,0,0,1]);floats(word(sp+12),[1,2,3])
    if pc in (START_CB, STOP_CB):
        sp = uc.reg_read(X.UC_X86_REG_ESP)
        (starts if pc == START_CB else stops).append(word(sp+4))
        uc.reg_write(X.UC_X86_REG_EAX, 1)
        uc.reg_write(X.UC_X86_REG_ESP, sp+4)
        uc.reg_write(X.UC_X86_REG_EIP, word(sp))
u.hook_add(UC_HOOK_CODE, hook)
def call(ea, *args):
    sp = STACK+0x8000
    u.mem_write(sp, struct.pack('<'+'I'*(len(args)+1), END, *args))
    u.reg_write(X.UC_X86_REG_ESP, sp)
    saved={X.UC_X86_REG_EBX:0x12345678, X.UC_X86_REG_ESI:0x22345678,
           X.UC_X86_REG_EDI:0x32345678, X.UC_X86_REG_EBP:0x42345678}
    for r,v in saved.items(): u.reg_write(r,v)
    u.emu_start(ea, END+1, count=100000)
    assert u.reg_read(X.UC_X86_REG_EIP)==END, hex(ea)
    assert u.reg_read(X.UC_X86_REG_ESP)==sp+4, ('cdecl',hex(ea))
    for r,v in saved.items(): assert u.reg_read(r)==v, ('nonvolatile',hex(ea))
    return u.reg_read(X.UC_X86_REG_EAX)

p=BASE
assert call(0x7ffb90,p)==p
assert [word(p+4*i) for i in range(16)]==[0xffffffff,3,0,0,0,0,0,100,100,10,400,400,1000,100,1000,40]
call(0x7ffbf0,p,1,2,3,4); assert [word(p+4*i) for i in (12,13,14,15)]==[2,1,3,4]
floats(p,[-2,-4,-6,99,2,4,6,88]);call(0x8006d0,p,fb(2))
assert fwords(p,8)==(-4,-8,-12,99,4,8,12,88)
o=BASE+0x100
u.mem_write(o,b'\xa5'*96);word(o+12,0)
assert call(0x7fa5b0,o)==o and word(o+92)==0x885020
assert word(o+12)==0x20003 and word(o+64)==0xa5a5a5a5 and word(o+76)==0
descriptor,mass=BASE+0x500,BASE+0x600
word(descriptor+12,PROPERTIES_CB);word(descriptor+64,0);word(o+92,descriptor)
floats(o+48,[10,20,30]);word(mass,o)
call(0x7fa460,mass,fb(-2),1)
assert fwords(mass+4,4)==(11,22,33,16)
assert fwords(mass+20,3)==(32,48,64) and word(mass+72)==2
call(0x7fa460,mass,fb(4),0)
assert fwords(mass+48,1)==(16,) and word(mass+72)==0
word(mass+72,2);call(0x7fa460,mass,fb(1.1754944e-38),0)
assert fwords(mass+48,1)==(16,) and word(mass+72)==2
call(0x7fa460,mass,fb(1.1754944e-38),1)
assert fwords(mass+20,3)==(2,3,4)

d,b,vt=BASE+0x1000,BASE+0x2000,BASE+0x3000
enabled,active=BASE+0x4000,BASE+0x4100
word(d+92,enabled);word(d+96,active);word(d+72,1)
word(b,vt);word(vt+16,START_CB);word(vt+20,STOP_CB)
word(b+32,33);word(b+36,d)
assert call(0x7fee10,d,b,1)==d and word(enabled+4)==2
assert call(0x7fee60,d,b)==b and word(active+4)==2 and word(d+76)==1
call(0x7fee60,d,b);assert starts==[b]
assert call(0x7fef10,d,b)==b and word(active+4)==0 and word(d+76)==0
call(0x7fef10,d,b);assert stops==[b]

e,agg,state,mats,accum=BASE+0x5000,BASE+0x6000,BASE+0x6100,BASE+0x7000,BASE+0x8000
word(e,agg);word(e+4,1);word(agg+16,state);word(state,mats);word(state+48,accum)
floats(mats+64+48,[1,2,3]);floats(p,[2,0,0]);floats(p+16,[1,3,3])
assert call(0x802410,e,p,p+16)==e
assert fwords(accum+32,8)==(2,0,0,0,0,0,-2,0)
print('PASS original physics: defaults, bounds, shape/mass, activation, force/torque; cdecl/nonvolatiles (mass callback + sqrt doubles)')
