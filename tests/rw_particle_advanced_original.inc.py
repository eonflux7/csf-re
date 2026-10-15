# Runs in rw_particles_original.py's explicit x86 image/COFF fixture harness.
def template_object(name, enabled):
    syms=subprocess.check_output(['llvm-nm-21',str(path)],text=True)
    encoded='$00' if enabled else '$0A@'
    symbol=next(line.split()[-1] for line in syms.splitlines()
                if line.split() and line.split()[-1].startswith('??$'+name+'@'+encoded+'@?A'))
    return emu.Obj(str(path),symbol[1:symbol.index('@@Y')])

advanced_cases=0
for name,ea,enabled in [('multiEmit',0x578760,True),('multiEmit',0x579000,False),
                        ('multiParticleUpdate',0x578670,True),('multiParticleUpdate',0x578de0,False),
                        ('shapedEmit',0x57b640,True),('shapedEmit',0x57ae40,False),
                        ('subEmitterEmit',0x57a9a0,None)]:
    obj=object_for(path,name) if enabled is None else template_object(name,enabled)
    for variant in range(8):
        machines=[Machine(),Machine(obj)];results=[]
        for original,n in zip((True,False),machines):
            setup(n,0x57bbd0,1)
            extras={0x1000001:600,0x1000002:640,0x1000003:784,0x1000004:936,
                    0x1000005:1088,0x1000006:1200,0x1000007:1320}
            # Include explicit padding properties: particleProperty's native
            # widening walk (like the binary table) expects contiguous offsets.
            eprops={**ep,999:568,**extras};esize=[40,172,64,64,32,16,116,52,12,32,40,144,152,152,112,120,20]
            pprops={**ps,0x1000001:208,0x1000002:216,0x1000003:328,0x1000004:440,0x1000005:552}
            table(n,ET,eprops,esize);table(n,PT,pprops,[16,12,12,32,32,32,8,64,8,112,112,112,64])
            n.word(PC+12,1024);n.word(B+8,3);n.word(B+16,0 if variant!=7 else 0xffffffff)
            n.word(E+40,0x1234567*variant+1)
            n.floats(E+40+64,2 if variant==6 else 0)
            for id in (0x1000002,0x1000003,0x1000004,0x1000005):
                base=E+extras[id];header=28 if id in (0x1000003,0x1000004) else 20;stride=24 if id==0x1000005 else 40
                n.word(base,3);n.word(base+4,base+header)
                offsets={0x1000002:[4,16,64],0x1000003:[4,16,40,64,88],
                         0x1000004:[4,16,0,40,0],0x1000005:[4,16,40]}[id]
                for k,v in enumerate(offsets): n.word(base+8+4*k,v)
                for k in range(3):
                    n.floats(base+header+k*stride,.2+.2*k,.01, *[float(j+k+2)*.02 for j in range(stride//4-2)])
            n.floats(E+600,.3,.1,.4,.2);n.floats(E+1320,2,.5,1);n.word(E+1332,variant%2);n.floats(E+1336,.7)
            n.word(E+1200,4);n.word(E+1204,1);n.word(E+1208,variant%2);n.word(E+1212,(variant//2)%2)
            n.word(E+1216,E+1224);n.word(E+1220,E+1272)
            for k in range(4): n.floats(E+1224+12*k,k+1,k+2,k+3);n.floats(E+1272+12*k,0,1,0)
            # The target supplies position and velocity; PTank output cursors
            # point just after the emitted range, exactly as in the dispatcher.
            n.word(OUT,BUF+36)
            n.u.mem_write(B+32,bytes(3*1024))
            for k in range(3):
                p=B+32+k*1024;n.floats(p,0,0 if variant!=5 else .2,10,.1)
                n.floats(p+16,1,2,3);n.floats(p+28,5,6,7)
                for off in (40,72): n.floats(p+off,1,2,3,4,5,6,7,8)
                n.floats(p+104,1,2,3,4)
                for id in (0x1000002,0x1000003,0x1000004,0x1000005):
                    off=pprops[id];n.word(p+off,1 if variant%2 else 0)
                    n.floats(p+off+4,.1,.2,.3)
                    for j in range(4,16): n.floats(p+off+j*4,float(j))
            n.floats(DT,.25)
            args=(E,B,DT) if name=='multiParticleUpdate' else (0,E,DT)
            results.append(n.call(ea if original else obj.entry,*args))
        assert results[0]==results[1],(name,variant,'return')
        a,b=machines
        assert a.word(E+40)==b.word(E+40),(name,ea,variant,'seed',hex(a.word(E+40)),hex(b.word(E+40)))
        for at,size,label in [(B,32+3*1024,'batch'),(BUF,36,'positions'),(E+1200,16,'point controls')]:
            close_words(bytes(a.u.mem_read(at,size)),bytes(b.u.mem_read(at,size)),(name,ea,variant,label))
        advanced_cases+=1
print('PASS:',advanced_cases,'advanced original/COFF emission/key-switch cases; exact ABI/seed and tolerant floats')
