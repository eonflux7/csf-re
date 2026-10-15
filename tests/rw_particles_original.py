#!/usr/bin/env python3
"""Offline original-byte fixtures, plus focused original/COFF callback diffs.
Uses tools/emudiff.py's PE/COFF loader and Unicorn, not Windows or an SDK.
Run after linktest.py (build/standalone/src_boundary_rw_particles.cpp.obj).
"""
import pathlib
import struct
import sys
import re
import subprocess
ROOT = pathlib.Path(__file__).resolve().parents[1]
sys.path.insert(0, str(ROOT / 'tools'))
import emudiff as emu
import linktest
from unicorn import Uc, UC_ARCH_X86, UC_MODE_32, UC_HOOK_CODE
from unicorn import x86_const as X

ENGINE, HEAP, DATA, STACK, END, ALLOC, FREE, REALLOC, RENDER = (
    0x30000000, 0x30100000, 0x30200000, 0x30300000,
    0x30400000, 0x30400100, 0x30400200, 0x30400300, 0x30400400)
BATCH_CB=0x30400500

class Machine:
    def __init__(self, obj=None):
        self.u = u = Uc(UC_ARCH_X86, UC_MODE_32)
        img, _ = emu.exe_image()
        u.mem_map(emu.IMG, (len(img)+4095)&~4095)
        u.mem_write(emu.IMG, img)
        u.mem_map(ENGINE, 0x500000)
        self.next = HEAP
        self.renders = 0
        self.render_fail = False
        self.batch_calls=[]
        self.batch_fail=0
        self.alloc_fail=False
        self.word(0x8b68c0, ENGINE)
        for off, fn in [(0x108, ALLOC), (0x10c, FREE), (0x110, REALLOC)]: self.word(ENGINE+off, fn)
        for a in [0x8be700, 0x8be704, 0x8be708, 0x8be720, 0x8be724, 0x8be718]: self.word(a, 0)
        if obj:
            u.mem_map(emu.OBJ_BASE,obj.end-emu.OBJ_BASE)
            for sec in obj.secs: u.mem_write(sec['base'],bytes(sec['data']))
            u.mem_map(emu.EXT_BASE,0x10000)
            u.mem_map(emu.SYS_BASE,0x1000)
            for name,at in emu.NATIVE_AT.items(): u.mem_write(at,bytes.fromhex(emu.NATIVE[name]))
            self.word(emu.COOKIE,0xbb40e64e)
            names=linktest.demangle(list(obj.ext))
            for symbol,at in obj.ext.items():
                slot=re.search(r'csf::fn_slot<(\d+)>',names.get(symbol,''))
                if slot: self.word(at,int(slot[1]))
                if symbol.startswith('?state@Engine@rw@@'): self.word(at,3)
                if symbol.startswith('?memfuncs@Engine@rw@@'):
                    self.word(at,ALLOC);self.word(at+8,FREE)
                # CRT scalar math on the COFF side: actual x87 arithmetic,
                # matching the image's inlined fsin/fcos for this bounded input.
                if symbol in ('_sin','_cos','_sqrt'):
                    opcode={'_sin':b'\xd9\xfe','_cos':b'\xd9\xff','_sqrt':b'\xd9\xfa'}[symbol]
                    u.mem_write(at,b'\xdd\x44\x24\x04'+opcode+b'\xc3')
        def hook(uc, pc, size, _):
            if pc == END:
                uc.emu_stop()
            elif obj and pc in obj.ext_names:
                name=obj.ext_names[pc]
                if name in ('_sin','_cos','_sqrt'): return
                sp=uc.reg_read(X.UC_X86_REG_ESP)
                if name=='_memset':
                    dst,value,n=self.word(sp+4),self.word(sp+8),self.word(sp+12)
                    uc.mem_write(dst,bytes([value&255])*n)
                    uc.reg_write(X.UC_X86_REG_EAX,dst)
                elif name in ('_memcpy','_memmove'):
                    dst,src,n=self.word(sp+4),self.word(sp+8),self.word(sp+12)
                    uc.mem_write(dst,bytes(uc.mem_read(src,n)))
                    uc.reg_write(X.UC_X86_REG_EAX,dst)
                elif name.startswith('?plugins@csf_rw@@'):
                    uc.reg_write(X.UC_X86_REG_EAX,ENGINE+0x1000)
                elif name.startswith('?getPluginOffset@PluginList@rw@@'):
                    assert self.word(sp+4)==303
                    uc.reg_write(X.UC_X86_REG_EAX,0xa4)
                    # This is a thiscall member, not cdecl.
                    uc.reg_write(X.UC_X86_REG_ESP,sp+8)
                    uc.reg_write(X.UC_X86_REG_EIP,self.word(sp))
                    return
                else: raise AssertionError(('unexpected COFF callee',name))
                uc.reg_write(X.UC_X86_REG_ESP,sp+4)
                uc.reg_write(X.UC_X86_REG_EIP,self.word(sp))
            elif pc in (ALLOC, FREE, REALLOC, RENDER,BATCH_CB):
                sp = uc.reg_read(X.UC_X86_REG_ESP)
                if pc in (ALLOC, REALLOC):
                    n = self.word(sp+(8 if pc==REALLOC else 4))
                    p = 0 if self.alloc_fail else self.next
                    if p:
                        self.next += (n+31)&~15
                        uc.mem_write(p, b'\xcd'*n)
                    if pc==REALLOC and self.word(sp+4):
                        # Scratch growth is only used before any read in these cases.
                        pass
                    uc.reg_write(X.UC_X86_REG_EAX, p)
                elif pc==RENDER:
                    self.renders += 1
                    uc.reg_write(X.UC_X86_REG_EAX, 0 if self.render_fail else self.word(sp+4))
                elif pc==BATCH_CB:
                    args=tuple(self.word(sp+i) for i in (4,8,12))
                    self.batch_calls.append(args)
                    uc.reg_write(X.UC_X86_REG_EAX,0 if args[1]==self.batch_fail else args[1])
                else: uc.reg_write(X.UC_X86_REG_EAX, 1)
                uc.reg_write(X.UC_X86_REG_ESP, sp+4)
                uc.reg_write(X.UC_X86_REG_EIP, self.word(sp))
        u.hook_add(UC_HOOK_CODE, hook)

    def word(self, a, v=None):
        if v is not None: self.u.mem_write(a, struct.pack('<I', v&0xffffffff))
        return struct.unpack('<I', self.u.mem_read(a, 4))[0]

    def floats(self, a, *values): self.u.mem_write(a, struct.pack('<'+'f'*len(values), *values))

    def call(self, ea, *args):
        sp = STACK+0x8000
        self.u.mem_write(sp, struct.pack('<'+'I'*(len(args)+1), END, *args))
        saved = {X.UC_X86_REG_EBX: 0x12345678, X.UC_X86_REG_ESI: 0x22345678,
                 X.UC_X86_REG_EDI: 0x32345678, X.UC_X86_REG_EBP: 0x42345678}
        for reg, value in saved.items(): self.u.reg_write(reg, value)
        self.u.reg_write(X.UC_X86_REG_ESP, sp)
        try: self.u.emu_start(ea, END+1, count=200000)
        except Exception as exc:
            raise RuntimeError(('entry/eip',hex(ea),hex(self.u.reg_read(X.UC_X86_REG_EIP)))) from exc
        assert self.u.reg_read(X.UC_X86_REG_EIP)==END, hex(ea)
        assert self.u.reg_read(X.UC_X86_REG_ESP)==sp+4, ('cdecl', hex(ea))
        for reg, value in saved.items(): assert self.u.reg_read(reg)==value, ('nonvolatile', hex(ea), reg)
        return self.u.reg_read(X.UC_X86_REG_EAX)

def schema(m, c):
    t=m.word(c+0x10); n=m.word(t+0x10)
    ids=[m.word(m.word(t+0x18)+4*i) for i in range(n)]
    sizes=[m.word(m.word(t+0x20)+4*i) for i in range(n)]
    offsets=[m.word(m.word(t+0x1c)+4*i) for i in range(n+1)]
    expected=[0]
    for size in sizes: expected.append(expected[-1]+size)
    assert offsets==expected and m.word(c+0xc)==expected[-1]
    return ids, sizes

m=Machine()
for ea, flags, ids, sizes in [
    (0x5784e0,131131,[0,1,2,6,5,4,0x1000007],[40,172,64,52,32,16,20]),
    (0x5784e0,131187,[0,1,2,6,8,5,0x1000007],[40,172,64,52,116,32,20]),
    (0x57f970,117,[0,6,2,5,4],[16,12,32,32,8]),
    (0x57f970,101,[0,6,2,5],[16,12,32,32]),
    (0x5785b0,69,[0,6,2],[16,12,32]),
    (0x57f970,2,[0,1],[16,12]),
    (0x57fa90,33,[0,1,6],[40,172,52]),
]:
    c=m.call(ea,flags,0)
    assert schema(m,c)==(ids,sizes), (hex(ea),flags,schema(m,c))
    assert m.call(ea,flags,0)==c and m.word(c+8)==2
    if flags==33: ec=c
e=m.call(0x580100,ec,0)
assert e and m.word(e+4)==0x7f
t=m.word(ec+0x10)
so=m.call(0x57c8b0,t,1); to=m.call(0x57c8b0,t,6)
assert so==40 and to==212
s=e+so
assert m.word(s)==1 and m.word(s+4)==0
for off in [0x74,0x78,0x8c,0x90]: assert m.word(s+off)==0x3f800000
assert m.word(s+0x7c)==0xffffffff
for off in [0x80,0x84,0x88,0x94,0x98,0x9c,0xa0,0xa4]: assert m.word(s+off)==0
assert m.word(e+to+0x10)==0xffffffff and m.word(e+to+0x28)==2

# Focused COFF equivalence: no allocator/container/device callees are needed.
emu.crt_ranges=lambda: ({},{})
path=ROOT/'build/standalone/src_boundary_rw_particles.cpp.obj'
symbols=subprocess.check_output(['llvm-nm-21',str(path)],text=True)
for ea, name in [(0x57ed00,'emitterCreateCallback'), (0x57f050,'emitterSchedule'),
                 (0x57f0b0,'emitterEndUpdate'), (0x57ef90,'emitterBeginUpdate')]:
    # Obj's normal entry finder expects a global name. Include the compiler's
    # anonymous-namespace discriminator for this private boundary function.
    qualified=re.search(r'\?('+re.escape(name)+r'@\?A[^@]+)@@',symbols).group(1)
    obj=emu.Obj(str(path),qualified)
    for variant in range(5 if ea==0x57f0b0 else 4):
        results=[]
        for original in [True,False]:
            n=Machine(None if original else obj)
            E,C,T,IDS,OFF,SIZES,A= [DATA+i*0x1000 for i in range(7)]
            n.word(E+0xc,C);n.word(C+0x10,T)
            n.word(T+0x10,2);n.word(T+0x18,IDS);n.word(T+0x1c,OFF);n.word(T+0x20,SIZES)
            n.word(IDS,0);n.word(IDS+4,1 if ea!=0x57f0b0 else 6)
            n.word(OFF,0);n.word(OFF+4,40);n.word(SIZES,40);n.word(SIZES+4,172 if ea!=0x57f0b0 else 52)
            n.u.mem_write(E+40,b'\xa5'*172)
            if ea==0x57ed00:
                if variant==1: n.word(IDS+4,999)
                n.word(E+4,0x180)
            elif ea==0x57f050:
                n.word(E+40,1 if variant!=3 else 0xffffffff)
                n.floats(E+48,.25 if variant!=2 else float('nan'))
                n.floats(E+40+0x34,.5,.125,0.,.0 if variant!=1 else 1.)
                if original: n.word(0x8be738,40)
            elif ea==0x57f0b0:
                n.word(E+0x10,0 if variant==0 else 1)
                n.word(E+40+8,0 if variant==1 else 1)
                n.word(E+40+0x18,A);n.word(A+0x48,RENDER)
                n.render_fail=variant==3
                if variant==4: n.word(IDS+4,999)
            else:
                F,P,D=DATA+0x8000,DATA+0x9000,DATA+0xa000
                n.word(A+4,F);n.word(F+0xa0,F);n.word(A+0xa4,P);n.word(0x8b09ec,0xa4)
                n.word(T+0x10,3);n.word(IDS+8,6);n.word(OFF+8,212);n.word(SIZES+8,52)
                n.word(E+212+0x10,0x20000 if variant else 0)
                n.word(E+212+0x18,A)
                n.floats(E+40+8,0.0);n.floats(E+40+0x80,1.0)
                n.floats(P+0xc0,0.0 if variant<2 else (3.1 if variant==2 else -3.1))
                n.floats(D,-.25 if variant==3 else .25)
                n.word(0x8be738,40);n.word(0x8be748,212)
            ret=n.call(ea if original else obj.entry,A if ea==0x57ef90 else 0,E,D if ea==0x57ef90 else 0)
            results.append((ret,bytes(n.u.mem_read(E,264 if ea==0x57ef90 else 212)),n.renders,
                            bytes(n.u.mem_read(P,0x138)) if ea==0x57ef90 else b''))
        assert results[0]==results[1], (hex(ea),variant,results)

for name,ea in [('allBatches',0x580160),('newBatch',0x580230)]:
    qualified=re.search(r'\?('+re.escape(name)+r'@\?A[^@]+)@@',symbols).group(1)
    obj=emu.Obj(str(path),qualified)
    for variant in range(4):
        results=[]
        for original in [True,False]:
            n=Machine(None if original else obj)
            E,B0,B1,C,CBS=[DATA+i*0x1000 for i in range(5)]
            if name=='allBatches':
                n.word(E+0x20,0 if variant==0 else B1)
                n.word(B0,B1);n.word(B1,B0)
                n.batch_fail={2:B0,3:B1}.get(variant,0)
                ret=n.call(ea if original else obj.entry,E,BATCH_CB,C)
                results.append((ret,n.batch_calls))
            else:
                n.word(E+0x18,C);n.word(E+0x14,3)
                n.word(C+0xc,28);n.word(C+0x14,1);n.word(C+0x18,CBS)
                n.word(CBS+8,BATCH_CB)
                n.alloc_fail=variant==3
                if variant==1: n.batch_fail=HEAP
                if variant==2:
                    n.word(E+0x1c,B0);n.word(B0,B0);n.word(B0+4,C)
                    n.word(B0+0xc,3);n.word(B0+0x14,32)
                    n.word(B0+8,2);n.word(B0+0x10,1)
                ret=n.call(ea if original else obj.entry,E)
                defined=tuple(n.word(ret+off) for off in (0,4,8,12,16,20)) if ret else ()
                results.append((ret,defined,n.word(E+0x1c),n.word(E+0x20),n.batch_calls))
        assert results[0]==results[1],(name,variant,results)

# Original lock/unlock bytes and the output/dirty rules used by the native test.
n=Machine();A,T,BUF,OUT=DATA,DATA+0x1000,DATA+0x2000,DATA+0x3000
n.word(0x8b09ec,0xa4);n.word(A+0xa4,T)
n.word(T+0xa0,0x27);n.word(T+0x4c,BUF);n.word(T+0x50,12)
n.word(T+0x64,BUF+64);n.word(T+0x68,8)
assert n.call(0x5821d0,A,OUT,1,0x40000000)==1
assert (n.word(OUT),n.word(OUT+4),n.word(T+0x3c))==(BUF,12,0x40000001)
assert n.call(0x5821d0,A,OUT,4,0x40000000)==1 and n.word(OUT+4)==8
assert n.call(0x5823e0,A)==A and n.word(T+0x40)==0x40000005 and n.word(T+0x3c)==0
n.word(T+0x9c,28)
assert n.call(0x5821d0,A,OUT,1,0)==1 and n.word(OUT+4)==28
assert n.call(0x5821d0,A,OUT,8,0)==0 and n.word(OUT)==0 and n.word(OUT+4)==0
n.word(OUT,BUF);n.word(OUT+4,99)
assert n.call(0x5821d0,A,OUT,3,0)==1 and n.word(OUT)==BUF and n.word(OUT+4)==28
n.word(T+0xa0,0x1ff);n.word(T+0x9c,0)
for flag,off,stride in [(1,0x4c,12),(2,0x6c,4),(4,0x64,8),(8,0x54,64),(16,0x5c,12),
                        (32,0x7c,4),(64,0x74,16),(128,0x84,16),(256,0x8c,32)]:
    n.word(T+off,BUF+off*16);n.word(T+off+4,stride)
    assert n.call(0x5821d0,A,OUT,flag,0x40000000)==1
    assert n.word(OUT)==BUF+off*16 and n.word(OUT+4)==stride
assert n.word(T+0x3c)==0x400001ff
assert n.call(0x5823e0,A)==A and n.word(T+0x40)==0x400001ff and n.word(T+0x3c)==0
print('PASS: original schemas/cache/defaults and PTank bytes; 25 original/COFF create/schedule/end/begin/batch cases (cdecl/nonvolatiles; explicit allocator/registry/callback doubles)')

# Slice 8b uses the same original-image/COFF machines and ABI assertions.
exec((ROOT/'tests/rw_particle_simulation_original.inc.py').read_text())
exec((ROOT/'tests/rw_particle_advanced_original.inc.py').read_text())
