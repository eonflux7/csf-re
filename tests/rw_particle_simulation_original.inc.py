import math
import random

def object_for(path, name):
    syms=subprocess.check_output(['llvm-nm-21',str(path)],text=True)
    qualified=re.search(r'\?('+re.escape(name)+r'@\?A[^@]+)@@',syms).group(1)
    return emu.Obj(str(path),qualified)

# Include exponent/mantissa boundaries, signed zero, denormals, infinities and
# NaNs. This compares the whole eax, not just the low compressed word.
codec=object_for(ROOT/'build/standalone/src_boundary_rw_plugins.cpp.obj','compressedFloat')
orig,port=Machine(),Machine(codec)
bits=[0,0x80000000,0x7f800000,0xff800000,0x7fc00000,0x7fffffff]
for exponent in range(256):
    for mantissa in (0,0x7ff,0x800,0xfff,0x1000,0x7fefff,0x7ff000,0x7fffff):
        for sign in (0,0x80000000): bits.append(sign|(exponent<<23)|mantissa)
rng=random.Random(8)
bits += [rng.getrandbits(32) for _ in range(1000)]
for value in bits:
    assert orig.call(0x61f5e0,value)==port.call(codec.entry,value),('codec',hex(value))
print('PASS: compressedFloat',len(bits),'original/COFF bit patterns')

E,EC,PC,ET,PT,B,LTM,DT,BUF,IN,OUT,STR=[DATA+i*0x1000 for i in range(12)]
ep={0:0,1:40,2:212,3:276,5:340,4:372,8:388,6:504,0x1000008:556}
ps={0:0,6:16,1:28,2:40,3:72,5:104,4:136,7:144}
math_snapshot=None

def table(n,t,props,sizes):
    n.word(t+0x10,len(props));n.word(t+0x18,t+0x100);n.word(t+0x1c,t+0x200);n.word(t+0x20,t+0x300)
    for k,(id,off) in enumerate(props.items()):
        n.word(t+0x100+4*k,id);n.word(t+0x200+4*k,off);n.word(t+0x300+4*k,sizes[k])

def setup(n,ea,variant):
    # Execute the image's math table initializers and install its scalar
    # transform/multiply dispatchers, rather than mocking vector arithmetic.
    n.word(0x8b66a8,0x2000);n.word(0x8b66dc,0x2100)
    global math_snapshot
    if math_snapshot is None:
        n.call(0x6225b0);n.call(0x622690)
        n.call(0x622e70,0,0,0,0);n.call(0x6242e0,0)
        math_snapshot=(bytes(n.u.mem_read(ENGINE+0x2000,0x200)),bytes(n.u.mem_read(HEAP,0x8020)),n.next)
    else:
        dispatch,tables,n.next=math_snapshot
        n.u.mem_write(ENGINE+0x2000,dispatch);n.u.mem_write(HEAP,tables)
    table(n,ET,ep,[40,172,64,64,32,16,116,52,12]);table(n,PT,ps,[16,12,12,32,32,32,8,64])
    n.word(E+0xc,EC);n.word(E+0x18,PC);n.word(E+0x20,B);n.word(E+0x14,8)
    n.word(EC+0x10,ET);n.word(PC+0x10,PT);n.word(PC+0xc,208)
    n.word(B,B);n.word(B+4,PC);n.word(B+0xc,8);n.word(B+0x14,32)
    s=E+40;n.word(s,variant*0x1234567+1)
    n.floats(s+8,1,0);n.floats(s+16,.1,-.2,.3)
    n.floats(s+28,1,2,3,.2,.3,.4)
    n.floats(s+64,0);n.word(s+68,3);n.word(s+72,1)
    n.floats(s+76,2,.2,2,.4);n.floats(s+92,0,0,1,.1,.2,.3)
    n.floats(s+116,1,1);n.word(s+168,LTM)
    n.floats(LTM,1,0,0,0,0,1,0,0,0,0,1,0,10,20,30,0)
    for id in (2,3): n.floats(E+ep[id],*[float(j+1) for j in range(16)])
    n.floats(E+ep[5],1,2,.1,.2,3,4,.3,.4)
    n.floats(E+ep[4],3,.4,2,.5)
    n.floats(E+ep[8],*[0.0]*29);n.floats(E+ep[8]+64,0,0,1,.1,.1,.1,0,1,0,.1,.1,.1)
    n.floats(E+ep[0x1000008],2,1);n.word(E+ep[0x1000008]+8,variant%2)
    tank=E+ep[6];flags=[0,1,2,4,32,128,0xa7,8][variant%8]
    if variant in (17,18): flags=8
    if variant==19: flags=0xa7
    if variant==20: flags=9
    if variant==21: flags=0xaf
    n.word(tank,flags);n.word(tank+12,100);n.word(tank+16,flags);n.word(tank+20,flags)
    n.word(tank+28,IN);n.word(tank+32,OUT);n.word(tank+36,STR)
    k=0
    for flag,stride in [(1,12),(2,4),(4,8),(8,64),(32,4),(128,16)]:
        if flags&flag:
            n.word(IN+4*k,BUF+0x100*k);n.word(OUT+4*k,BUF+0x100*k);n.word(STR+4*k,stride)
            for j in range(8):
                if flag==8: n.floats(BUF+0x100*k+j*stride,1,0,0,0,0,1,0,0,0,0,1,0,10,20,30,0)
                else: n.floats(BUF+0x100*k+j*stride,*([float(j+1)]*(stride//4)))
            k+=1
    # Std callback's caller normally caches these offsets before dispatch.
    for addr,id in [(0x8be738,1),(0x8be73c,2),(0x8be740,3),(0x8be744,5),(0x8be748,6),(0x8be74c,8),(0x8be750,4)]: n.word(addr,ep[id])
    for addr,id in [(0x8be754,1),(0x8be75c,2),(0x8be760,3),(0x8be764,5),(0x8be768,6),(0x8be76c,7),(0x8be770,4)]: n.word(addr,ps[id])
    n.floats(DT,.25)
    if ea==0x57cfe0:
        n.word(B+8,4)
        for j in range(4):
            p=B+32+208*j
            n.floats(p,0,2 if j==1 else 0,1,.5)
            n.floats(p+16,1,2,3);n.floats(p+28,10,20,30)
            for off in (40,72): n.floats(p+off,1,2,3,4,5,6,7,8)
            n.floats(p+104,1,2,3,4,1,2,0,0);n.floats(p+136,1,2)
            n.floats(p+144,1,0,0,0,0,1,0,0,0,0,1,0,0,0,0,0)
    elif ea==0x57bbd0:
        n.word(B+8,3);n.word(B+16,1)
        for slot in range(k): n.word(OUT+4*slot,n.word(OUT+4*slot)+n.word(STR+4*slot)*3)
    if variant==8:
        # Optional color, UV, size, rotation and matrix properties absent.
        for id,addr in [(2,0x8be73c),(3,0x8be740),(5,0x8be744),(4,0x8be750),(8,0x8be74c)]:
            n.word(ET+0x100+4*list(ep).index(id),id+100);n.word(addr,-1)
        for id,addr in [(2,0x8be75c),(3,0x8be760),(5,0x8be764),(4,0x8be770),(7,0x8be76c)]:
            n.word(PT+0x100+4*list(ps).index(id),id+100);n.word(addr,-1)
    if variant==9: n.word(E+0x20,0) if ea!=0x57cfe0 else n.word(B+8,0)
    if variant==10:
        if ea==0x57bbd0: n.word(B+16,-1)
        elif ea==0x57dba0: n.floats(s+64,2)
        else:
            for j in range(4): n.floats(B+32+208*j+4,1)
    if variant==11:
        if ea==0x57bbd0: n.word(B+16,3)
        elif ea==0x57dba0: n.word(s+68,0);n.word(s+72,0)
        else: n.floats(DT,2)
    if variant==12 and ea==0x57dba0: n.word(tank+12,0)
    if variant==13 and ea==0x57dba0: n.word(B+8,8)
    if variant==14 and ea==0x57dba0: n.word(E+0x20,0);n.alloc_fail=True
    if variant==15 and ea==0x57dba0: n.word(B+8,8);n.word(E+0x14,2)
    if variant==16:
        n.word(ET+0x100+4*list(ep).index(6),999);n.word(0x8be748,-1)
    if variant in (17,18):
        n.word(PT+0x100+4*list(ps).index(7),999);n.word(0x8be76c,-1)
        if variant==17:
            n.floats(E+ep[8],1,0,0,0,0,1,0,0,0,0,1,0,5,6,7,0);n.word(E+ep[8]+112,1)
    if variant==19:
        n.u.mem_write(DATA+0xc000,bytes(n.u.mem_read(BUF,0x600)))
        for slot in range(k): n.word(OUT+4*slot,n.word(OUT+4*slot)-BUF+DATA+0xc000)
    if variant in (22,23):
        # Exercise point versus vector transforms with a nonidentity basis,
        # including nonuniform scale; translation alone cannot distinguish them.
        n.floats(LTM,0,2,0,0,-3,0,0,0,0,0,.5,0,-7,11,4,0)
    return flags

def close_words(a,b,label):
    assert len(a)==len(b)
    for off in range(0,len(a),4):
        x,y=a[off:off+4],b[off:off+4]
        if x==y: continue
        xf,yf=struct.unpack('<f',x)[0],struct.unpack('<f',y)[0]
        assert math.isfinite(xf) and math.isfinite(yf) and math.isclose(xf,yf,rel_tol=3e-6,abs_tol=2e-6),(label,off,x.hex(),y.hex(),xf,yf)

for name,ea in [('standardParticleUpdate',0x57cfe0),('standardEmit',0x57dba0),('sphereEmit',0x57bbd0)]:
    obj=object_for(path,name)
    for variant in range(24):
        machines=[Machine(),Machine(obj)]
        results=[]
        for original,n in zip((True,False),machines):
            flags=setup(n,ea,variant)
            args=(E,B,DT) if ea==0x57cfe0 else (0,E,DT)
            ret=n.call(ea if original else obj.entry,*args)
            results.append(ret)
        assert results[0]==results[1],(name,variant,'return',results)
        a,b=machines
        for start,size in [(E,36),(E+40,8),(E+ep[6],52),(B,32),(IN,64),(OUT,64)]:
            assert bytes(a.u.mem_read(start,size))==bytes(b.u.mem_read(start,size)),(name,variant,'control words',hex(start))
        for start,size,label in [(E,568,'emitter'),(B,32+208*4,'batch'),(BUF,0x600,'clusters'),(DATA+0xc000,0x600,'separate outputs'),(IN,64,'input cursors'),(OUT,64,'output cursors')]:
            # Batch's undefined first header word stays untouched; original
            # emitter pointer/control words and RNG must match exactly.
            close_words(bytes(a.u.mem_read(start,size)),bytes(b.u.mem_read(start,size)),(name,variant,label))
        assert a.word(E+40)==b.word(E+40),(name,variant,'seed')
        if ea==0x57dba0 and a.word(E+0x20) not in (0,B):
            fresh=a.word(E+0x20)
            for off in (0,4,8,12,16,20): assert a.word(fresh+off)==b.word(fresh+off),(name,variant,'new header',off)
            for particle in range(a.word(fresh+8)):
                base=fresh+32+208*particle
                # Only defined payload fields: do not certify arbitrary fresh
                # allocation padding, unused lifetime word or Matrix flags.
                for off,size in [(4,36),(40,64),(104,24),(136,8)]+[(144+4*j,4) for j in (0,1,2,4,5,6,8,9,10,12,13,14)]:
                    close_words(bytes(a.u.mem_read(base+off,size)),bytes(b.u.mem_read(base+off,size)),(name,variant,'new payload',particle,off))
        if flags&2:
            slot=1 if flags&1 else 0
            assert bytes(a.u.mem_read(BUF+slot*0x100,32))==bytes(b.u.mem_read(BUF+slot*0x100,32)),(name,variant,'color bytes')
print('PASS: 72 simulation original/COFF cases; float tolerance 3e-6, exact ABI/cursors/counts/seed/control words')
