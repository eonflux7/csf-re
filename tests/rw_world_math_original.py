#!/usr/bin/env python3
"""Focused original-byte / compiled x86 boundary math checks, no Windows.

Usage: .venv/bin/python tests/rw_world_math_original.py [world.cpp.obj]
The normalize/inverse-sqrt dependency is doubled with scalar arithmetic;
these cases verify intersection contracts, not RW's fast-sqrt lookup table.
"""
import math
import pathlib
import re
import random
import struct
import subprocess
import sys

ROOT=pathlib.Path(__file__).resolve().parents[1]
sys.path.insert(0,str(ROOT/'tools'))
import emudiff as emu
from unicorn import Uc,UC_ARCH_X86,UC_MODE_32,UC_HOOK_CODE
from unicorn import x86_const as X

DATA,STACK,END,FLOAT,CALLBACK=0x30000000,0x30010000,0x30020000,0x30020100,0x30020200

class Machine:
    def __init__(self,obj=None):
        self.obj=obj
        self.events=[];self.stop=False
        self.u=u=Uc(UC_ARCH_X86,UC_MODE_32)
        image,_=emu.exe_image();u.mem_map(emu.IMG,(len(image)+4095)&~4095);u.mem_write(emu.IMG,image)
        u.mem_map(DATA,0x30000);u.mem_write(END,b'\xcc')
        for pc in (0x6231B0,0x622F40): u.mem_write(pc,b'\xd9\x05'+struct.pack('<I',FLOAT)+b'\xc3')
        if obj:
            u.mem_map(emu.OBJ_BASE,obj.end-emu.OBJ_BASE)
            for section in obj.secs:
                if section['base'] is not None: u.mem_write(section['base'],bytes(section['data']))
            u.mem_map(emu.EXT_BASE,0x10000);u.mem_map(emu.SYS_BASE,0x1000)
            for name,pc in emu.NATIVE_AT.items(): u.mem_write(pc,bytes.fromhex(emu.NATIVE[name]))
            self.word(emu.COOKIE,0xbb40e64e)
            for name,pc in obj.ext.items():
                if name in ('_sqrtf','_sqrt'): u.mem_write(pc,b'\xd9\x05'+struct.pack('<I',FLOAT)+b'\xc3')

        def hook(uc,pc,size,_):
            if pc==END: uc.emu_stop();return
            sp=uc.reg_read(X.UC_X86_REG_ESP)
            if pc==CALLBACK:
                self.events.append(self.word(sp+8));uc.reg_write(X.UC_X86_REG_EAX,0 if self.stop else self.word(sp+8))
                uc.reg_write(X.UC_X86_REG_ESP,sp+4);uc.reg_write(X.UC_X86_REG_EIP,self.word(sp));return
            if pc==0x6231B0:
                output,source=self.word(sp+4),self.word(sp+8);v=self.floats(source,3)
                length=math.sqrt(sum(x*x for x in v));inverse=1/length if length else 0
                self.write_floats(output,[x*inverse for x in v]);self.write_floats(FLOAT,[inverse]);return
            if pc==0x622F40:
                v=self.floats(sp+4,1)[0];self.write_floats(FLOAT,[1/math.sqrt(v) if v else 0]);return
            if not obj or pc not in obj.ext_names: return
            name=obj.ext_names[pc]
            if name in ('_sqrtf','_sqrt'):
                value=self.floats(sp+4,1)[0] if name=='_sqrtf' else struct.unpack('<d',uc.mem_read(sp+4,8))[0]
                self.write_floats(FLOAT,[math.sqrt(value)]);return
            if name.startswith('?cross@rw@@'):
                dst,a,b=(self.word(sp+x) for x in (4,8,12));v=self.floats(a,3);w=self.floats(b,3)
                self.write_floats(dst,[v[1]*w[2]-v[2]*w[1],v[2]*w[0]-v[0]*w[2],v[0]*w[1]-v[1]*w[0]])
                uc.reg_write(X.UC_X86_REG_EAX,dst);uc.reg_write(X.UC_X86_REG_ESP,sp+4);uc.reg_write(X.UC_X86_REG_EIP,self.word(sp));return
            raise AssertionError(('unmocked external',name,hex(pc)))
        u.hook_add(UC_HOOK_CODE,hook)

    def word(self,at,value=None):
        if value is not None: self.u.mem_write(at,struct.pack('<I',value))
        return struct.unpack('<I',self.u.mem_read(at,4))[0]
    def floats(self,at,n): return struct.unpack('<'+'f'*n,self.u.mem_read(at,n*4))
    def write_floats(self,at,values): self.u.mem_write(at,struct.pack('<'+'f'*len(values),*values))
    def call(self,pc,*args):
        sp=STACK+0x8000;self.u.mem_write(sp,struct.pack('<'+'I'*(len(args)+1),END,*args));self.u.reg_write(X.UC_X86_REG_ESP,sp)
        saved={X.UC_X86_REG_EBX:123456,X.UC_X86_REG_ESI:234567,X.UC_X86_REG_EDI:345678,X.UC_X86_REG_EBP:456789}
        for r,v in saved.items():self.u.reg_write(r,v)
        self.u.emu_start(pc,END+1,count=100000)
        assert self.u.reg_read(X.UC_X86_REG_EIP)==END and self.u.reg_read(X.UC_X86_REG_ESP)==sp+4
        for r,v in saved.items(): assert self.u.reg_read(r)==v
        return self.u.reg_read(X.UC_X86_REG_EAX)

path=pathlib.Path(sys.argv[1]) if len(sys.argv)>1 else ROOT/'build/standalone/src_boundary_rw_world.cpp.obj'
symbols=subprocess.check_output(['llvm-nm-21','--defined-only',str(path)],text=True)
cases={
    'lineTriangle':(0x620640,[
        ([.2,.2,1],[0,0,-2],[0,0,0],[1,0,0],[0,1,0]),
        ([.2,.2,1],[0,0,-2],[0,0,0],[0,1,0],[1,0,0]),
        ([0,0,0],[0,0,-1],[0,0,0],[1,0,0],[0,1,0]),
        ([.2,.2,1],[0,0,-.1],[0,0,0],[1,0,0],[0,1,0]),
        ([2,2,1],[0,0,-2],[0,0,0],[1,0,0],[0,1,0]),
        ([-.000004,.2,1],[0,0,-2],[0,0,0],[1,0,0],[0,1,0]),
    ]),
    'lineSphere':(0x7DA4E0,[
        ([0,0,0,0,0,1],[0,0,4,1]),
        ([0,0,0,0,0,1],[2,0,4,1]),
        ([0,0,0,0,0,1],[0,0,-.1,1]),
        ([0,0,0,0,0,1],[1,0,0,1]),
        ([0,0,0,0,0,0],[0,0,0,1]),
    ]),
    'sphereTriangle':(0x620840,[
        ([.2,.2,.1,.2],[0,0,0],[1,0,0],[0,1,0]),
        ([1.05,0,0,.2],[0,0,0],[1,0,0],[0,1,0]),
        ([2,2,0,.2],[0,0,0],[1,0,0],[0,1,0]),
        ([.2,.2,.2,.2],[0,0,0],[1,0,0],[0,1,0]),
        ([.2,.2,-.2,.2],[0,0,0],[1,0,0],[0,1,0]),
        ([0,0,0,1],[0,0,0],[0,0,0],[0,0,0]),
    ]),
}
# Fixed seed, ordinary nondegenerate inputs: exercise misses and output writes
# as well as successful hits. This is deliberately not a bit-exact FP claim.
rng=random.Random(0x435346)
def vector(): return [rng.uniform(-2,2) for _ in range(3)]
for _ in range(64):
    cases['lineTriangle'][1].append(tuple(vector() for _ in range(5)))
    cases['lineSphere'][1].append((vector()+vector(),vector()+[rng.uniform(.05,2)]))
    cases['sphereTriangle'][1].append((vector()+[rng.uniform(.05,2)],vector(),vector(),vector()))
total=0
for name,(ea,fixtures) in cases.items():
    qualified=re.search(r'\?('+name+r'@\?A[^@]+)@@',symbols).group(1);obj=emu.Obj(str(path),qualified)
    for fixture in fixtures:
        results=[]
        for compiled in (False,True):
            m=Machine(obj if compiled else None);args=[]
            for i,values in enumerate(fixture):
                at=DATA+i*256;m.write_floats(at,values);args.append(at)
            normal,distance=DATA+0x2000,DATA+0x2100;m.write_floats(normal,[11,12,13]);m.write_floats(distance,[123])
            if name=='sphereTriangle': args.append(normal)
            args.append(distance);ret=m.call(obj.entry if compiled else ea,*args)
            results.append((ret,m.floats(normal,3),m.floats(distance,1)))
        assert results[0][0]==results[1][0],(name,fixture,results)
        for x,y in zip(results[0][1]+results[0][2],results[1][1]+results[1][2]):
            assert math.isclose(x,y,rel_tol=1e-5,abs_tol=1e-6),(name,fixture,results)
        total+=1
print(f'rw world math original/COFF: {total} cases passed (sqrt dependencies doubled)')

# Independently establish the coarse traversal rules used by the native BSP
# fixture: line/box/point do not perform a world-box reject; sphere does.
world,node,left,right,query=[DATA+0x4000+i*0x1000 for i in range(5)]
sector_cases=[
    (4,[110,110,110,100,100,100],False,[right]),
    (2,[0,100,100],False,[left,right]),
    (1,[0,100,100,0,110,100],False,[right,left]),
    (3,[0,0,0,.1],False,[left,right]),
    (3,[100,0,0,.1],False,[]),
    (2,[0,0,0],True,[left]),
]
for kind,values,stop,expected in sector_cases:
    m=Machine();m.stop=stop;m.word(world+0x1c,node)
    m.word(node,0);m.write_floats(node+4,[0]);m.word(node+8,left);m.word(node+12,right);m.write_floats(node+16,[.5,-.5])
    for sector,sup,inf in [(left,[.5,4,4],[-4,-4,-4]),(right,[4,4,4],[-.5,-4,-4])]:
        m.word(sector,0xffffffff);m.write_floats(sector+0x48,sup+inf)
    m.write_floats(query,[0]*6);m.write_floats(query,values);m.word(query+24,kind)
    assert m.call(0x56B9B0,world,query,CALLBACK,0x1234)==world
    assert m.events==expected,(kind,values,m.events,expected)
print(f'rw world original BSP traversal: {len(sector_cases)} cases passed')
