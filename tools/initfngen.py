#!/usr/bin/env python3
"""Byte-driven global initializer generator. Never edits the database or existing sources.

--list / --json classify every selected function before writing. --write emits only
the closed instruction whitelist below; --check runs gate AND fresh-source emudiff
on all candidates, records both outputs, and removes failing generated candidates.
Shared IDA tail chunks are callees, not part of these entry bodies.
"""
import argparse, collections, concurrent.futures, json, os, re, subprocess, sys
from packet import DB, disasm
import api
from tinygen import code, room

ROOT = os.path.dirname(os.path.dirname(os.path.abspath(__file__)))

def targets(db):
    return [a for a, r in sorted(db.rows.items()) if r['region'] == 'game'
            and r['status'] == 'todo' and 0x8289f0 <= a < 0x82c410]

def instructions(db, ea):
    return [i for i in disasm(ea, db.F[ea]['size']) if ea <= i.address < db.F[ea]['end']]

def classify(db, ea):
    ins = instructions(db, ea)
    ms = [i.mnemonic for i in ins]
    text = '; '.join(i.mnemonic + ' ' + i.op_str for i in ins)
    if ms == ['ret']: return 'empty destructor'
    if ms == ['mov','ret'] and ins[0].op_str.startswith('dword ptr [0x'):
        return 'global vtable store'
    if ms[:3] == ['push','mov','push'] and not any(m in ('call','jmp') for m in ms) and ms[-3:] == ['mov','pop','ret'] and not any(m.startswith('f') for m in ms): return 'packed byte constant'
    if ms == ['mov', 'jmp']: return 'global destructor tail'
    if ms == ['push', 'call', 'push', 'call', 'add', 'ret']: return 'cdecl construct + atexit'
    if ms == ['push', 'call', 'pop', 'ret']:
        return 'atexit only' if '0x7aee43' in text else 'cdecl destructor'
    if any(m.startswith('f') or m == 'wait' for m in ms): return 'x87 helper'
    if any(i.mnemonic.startswith('j') and i.op_str.startswith('0x')
           and ea <= int(i.op_str, 0) < i.address for i in ins): return 'array loop'
    if '0x7aee43' in text:
        if ms in (['mov','call','push','call','pop','ret'],
                  ['push','mov','call','push','call','pop','ret'],
                  ['push','push','mov','call','push','call','pop','ret']):
            return 'thiscall construct + atexit'
        if ms == ['push','mov','call','push','mov','call','pop','ret']:
            return 'construct + vtable override + atexit'
        if ms in (['push','mov','call','pop','ret'],
                  ['push','call','xor','add','mov','mov','mov','ret']):
            return 'global stores + atexit'
        return 'compound initializer'
    return 'compound destructor / helper'

class Unsupported(Exception): pass

class Emitter:
    """Translate only fully decoded integer/global shapes, with explicit stack accounting.

    Register temporaries are values, not an interpreter: the emitted body is ordinary
    C++ stores/calls/conditions. Each push is snapshotted at its original position.
    Local stack addresses refer to a fixed buffer; no original stack is manufactured.
    """
    def __init__(self, db, ea):
        self.db, self.ea = db, ea
        self.ins = instructions(db, ea)
        self.vts = {int(v['addr'], 16) for v in json.load(open(os.path.join(ROOT, 'db/vtables.json')))}
        self.strings = {int(a,16):s for a,s in json.load(open(os.path.join(ROOT,'export/strings.json'))).items()}
        self.lines, self.binds, self.vars = [], {}, []
        self.stack, self.delta, self.regs = {}, 0, set()
        self.flags = None
        self.labels = {int(i.op_str, 0) for i in self.ins if i.mnemonic.startswith('j')
                       and i.op_str.startswith('0x') and ea <= int(i.op_str,0) < db.F[ea]['end']}

    def emit(self, line): self.lines.append('    ' + line)
    def temp(self, expr):
        name = 'value' + str(len(self.vars))
        self.vars.append(name)
        self.emit(name + ' = ' + expr + ';')
        return name
    def addr(self, n):
        if n in self.db.F: return f'CSF_ADDR_FN(0x{n:08X})'
        if n in self.vts: return f'CSF_ADDR_VT(0x{n:08X})'
        if n in self.strings:
            return f'(uintptr_t)CSF_STR(0x{n:08X}, {json.dumps(self.strings[n],ensure_ascii=True)})'
        if 0x400000 <= n < 0xa00000: return f'CSF_ADDR_DATA(0x{n:08X})'
        return f'0x{n & 0xffffffff:X}u'
    def value(self, op):
        if re.fullmatch(r'-?(0x[0-9a-f]+|\d+)', op): return self.addr(int(op, 0))
        reg = {'al': 'eax', 'ax': 'eax', 'cl':'ecx', 'dl':'edx'}.get(op, op)
        if reg in ('eax','ebx','ecx','edx','esi','edi','ebp'):
            if reg not in self.regs: raise Unsupported('read of undefined ' + reg)
            return f'({reg} & 0xFFu)' if op.endswith('l') else f'({reg} & 0xFFFFu)' if op=='ax' else reg
        m = re.fullmatch(r'(byte|word|dword) ptr \[(.*)\]', op)
        if m:
            t = {'byte':'uint8_t','word':'uint16_t','dword':'uint32_t'}[m[1]]
            return f'CSF_AT({t}, {self.memaddr(m[2])}, 0)'
        raise Unsupported('operand ' + op)
    def memaddr(self, op):
        if op.startswith('0x'): return self.addr(int(op,0))
        m = re.fullmatch(r'(e\w+)\s*(?:([+-])\s*(0x[0-9a-f]+|\d+))?',op)
        if not m: raise Unsupported('memory ' + op)
        reg, sign, off = m.groups(); off = int(off or '0',0) * (-1 if sign=='-' else 1)
        if reg == 'esp':
            if not -256 <= self.delta + off < 0: raise Unsupported('nonlocal stack address ' + op)
            return f'reinterpret_cast<uintptr_t>(local + {256 + self.delta + off})'
        return self.value(reg) + (f' + {off}' if off else '')
    def store(self, dest, expr):
        if dest in ('eax','ebx','ecx','edx','esi','edi','ebp'):
            self.regs.add(dest); self.emit(f'{dest} = {expr};')
        elif 'ptr [' in dest: self.emit(self.value(dest) + ' = ' + expr + ';')
        else: raise Unsupported('destination ' + dest)

    def call(self, target, tail=False):
        sig = api.load().get(target)
        if not sig or api.real_regs(target,self.db): raise Unsupported(f'nonstandard callee {target:08x}')
        sig = dict(sig)
        # Fastcall integer method ABI, where packet explicitly warns about a
        # virtual forwarder, must be inferred from this call site's actual pushes.
        from packet import ret_bytes
        if api.takes_reg(target,self.db,'ecx') and sig['conv'] != '__fastcall':
            raise Unsupported(f'canonical binding omits ecx {target:08x}')
        ps = split_params(sig['params'])
        conv = sig['conv']
        exprs = []
        for k, p in enumerate(ps):
            ty = param_type(p)
            if conv == '__fastcall' and k < 2:
                v = self.value('ecx') if k == 0 else (self.value('edx') if 'edx' in self.regs else '0')
            else:
                off = self.delta + 4 * (k - (2 if conv=='__fastcall' else 0))
                if off not in self.stack: raise Unsupported(f'missing stack argument {target:08x} {k}')
                v = self.stack[off]
            exprs.append(f'({ty})({v})')
        self.binds[target] = api.decl_line(target)
        call = sig['name'] + '(' + ', '.join(exprs) + ')'
        if sig['ret'] == 'void': self.emit(call + ';')
        else: self.store('eax', f'(uintptr_t)({call})')
        if conv != '__cdecl':
            n = len(ps) - (2 if conv=='__fastcall' else 0)
            for _ in range(n): self.stack.pop(self.delta,None); self.delta += 4
        # Calls destroy caller-save registers. EAX is usable only for nonvoid.
        self.regs.discard('ecx'); self.regs.discard('edx')
        if sig['ret']=='void': self.regs.discard('eax')
        if tail: self.emit('return' + (' eax;' if sig['ret']!='void' else ' 0;'))

    def generate(self):
        ea = self.ea
        header = ('// Generated by tools/initfngen.py from decoded CommXPC.exe instructions.\n'
                  f'// Shape: {classify(self.db,ea)}. Original bytes: {code(ea,self.db.F[ea]["size"]).hex()}\n'
                  '#include <cstdint>\n#include <cstddef>\n#include "globals.h"\n#include "hook.h"\n'
                  '#include "types/auto_classes.h"\n#include "legacy/string.h"\n'
                  '#include "types/vc71_string.h"\n'
                  'using Char = char;\nusing String = legacy::vc71::string<Char>;\n\n')
        fn = f'Global_{ea:08X}'
        if classify(self.db,ea) == 'packed byte constant':
            # The five colors build four bytes in an EBP local then copy the
            # little-endian dword. Validate every store and all four offsets.
            vals={}; regs={}; target=None
            for i in self.ins:
                m, ops=i.mnemonic,i.op_str.split(', ')
                if m=='xor' and ops[0]==ops[1]: regs[ops[0]]=0
                elif m=='mov' and ops[0] in ('al','cl'): regs[ops[0]]=int(ops[1],0)
                elif m=='mov' and ops[0].startswith('byte ptr [ebp - '):
                    off=int(ops[0][16:-1],0)
                    vals[off]=regs[ops[1]] if ops[1] in regs else int(ops[1],0)
                elif m=='mov' and ops[0].startswith('dword ptr [0x'): target=int(ops[0][11:-1],0)
            if set(vals)!={1,2,3,4} or target is None: raise Unsupported('incomplete packed constant')
            n=sum(vals[k] << (8*(4-k)) for k in vals)
            return (header+f'CSF_GLOBAL(uint32_t, color, 0x{target:08X});\n'
                    f'static uint32_t __cdecl {fn}() {{ color = 0x{n:08X}u; return 0x{n:08X}u; }}\n'
                    f'HOOK(0x{ea:08X}, {fn}, "initfn");\n')
        if [i.mnemonic for i in self.ins] == ['mov','jmp']:
            a = int(self.ins[0].op_str.split(', ')[1],0); t = int(self.ins[1].op_str,0)
            # An IDA-owned shared jump label is not a function-address slot.
            # Collapse only an unconditional direct-jump chain in this entry's
            # exported chunks; never reproduce a library body.
            shared={i.address:i for i in disasm(ea,self.db.F[ea]['size']) if i not in self.ins}
            seen=set()
            while t in shared and t not in seen:
                seen.add(t); jump=shared[t]
                if jump.mnemonic!='jmp' or not jump.op_str.startswith('0x'):break
                t=int(jump.op_str,0)
            if t not in self.db.F:raise Unsupported(f'tail into non-function shared code {t:08x}')
            return (header + f'static const uintptr_t object_addr = CSF_ADDR_DATA(0x{a:08X});\n'
                    f'static const uintptr_t target_addr = CSF_ADDR_FN(0x{t:08X});\n'
                    f'static __declspec(naked) void {fn}() {{\n    __asm {{\n'
                    '        mov ecx, dword ptr [object_addr]\n        jmp dword ptr [target_addr]\n    }\n}\n'
                    f'HOOK(0x{ea:08X}, {fn}, "initfn");\n')
        for i in self.ins:
            try:
                if i.address in self.labels: self.lines.append(f'label_{i.address:X}:;')
                m, op = i.mnemonic, i.op_str
                ops = op.split(', ')
                if m in ('nop',): continue
                if m == 'lea' and ops[1] in ('[ecx]', '[esp]') and ops[0]==ops[1][1:-1]: continue
                if m == 'mov': self.store(ops[0], self.value(ops[1]))
                elif m == 'lea': self.store(ops[0], self.memaddr(ops[1][1:-1]))
                elif m == 'xor' and ops[0]==ops[1]: self.store(ops[0], '0')
                elif m == 'push':
                    # Saved registers or a reserved stack word may be undefined;
                    # reject if consumed as an argument rather than restored.
                    self.delta -= 4
                    try: v = self.temp(self.value(op))
                    except Unsupported: v = 'UNDEFINED'
                    self.stack[self.delta] = v
                elif m == 'pop':
                    v = self.stack.pop(self.delta, 'UNDEFINED'); self.delta += 4
                    if v != 'UNDEFINED': self.store(op,v)
                    else: self.regs.discard(op)
                elif m in ('add','sub') and ops[0]=='esp':
                    n = int(ops[1],0) * (1 if m=='add' else -1)
                    if n>0:
                        for off in range(self.delta,self.delta+n,4): self.stack.pop(off,None)
                    self.delta += n
                elif m in ('add','sub','and','or'):
                    symbol={'add':'+','sub':'-','and':'&','or':'|'}[m]
                    self.store(ops[0], f'{self.value(ops[0])} {symbol} {self.value(ops[1])}')
                elif m == 'dec':
                    self.store(op,self.value(op)+' - 1'); self.flags=(self.value(op),'0')
                elif m in ('cmp','test'):
                    self.flags=(self.temp(self.value(ops[0])),self.temp(self.value(ops[1])))
                    if m=='test': self.flags=(f'({self.flags[0]} & {self.flags[1]})','0')
                elif m == 'call': self.call(int(op,0))
                elif m.startswith('j'):
                    t=int(op,0)
                    if t not in self.labels:
                        if m!='jmp': raise Unsupported('external conditional jump')
                        self.call(t,tail=True)
                    else:
                        if m=='jmp': self.emit(f'goto label_{t:X};')
                        else:
                            if not self.flags: raise Unsupported('unknown flags')
                            relation={'je':'==','jne':'!=','jb':'<','jbe':'<=','ja':'>','jae':'>='}.get(m)
                            if relation is None: raise Unsupported('condition '+m)
                            self.emit(f'if ({self.flags[0]} {relation} {self.flags[1]}) goto label_{t:X};')
                elif m == 'ret':
                    if op or self.delta: raise Unsupported('unbalanced / callee-pop return')
                    self.emit('return eax;' if 'eax' in self.regs else 'return 0;')
                else: raise Unsupported('instruction '+m)
            except (Unsupported,ValueError) as e:
                raise Unsupported(f'{i.address:08x}: {i.mnemonic} {i.op_str}: {e}')
        text = '\n'.join(self.lines)
        if 'UNDEFINED' in text: raise Unsupported('undefined saved register consumed')
        return (header + '\n'.join(self.binds.values()) + '\n\n'
                f'static uint32_t __cdecl {fn}() {{\n'
                '    uintptr_t eax=0, ebx=0, ecx=0, edx=0, esi=0, edi=0, ebp=0;\n'
                '    alignas(16) uint8_t local[256];\n'
                + (('    uintptr_t '+', '.join(self.vars)+';\n') if self.vars else '')
                + text + '\n}\n' + f'HOOK(0x{ea:08X}, {fn}, "initfn");\n')

def split_params(s):
    return [p.strip() for p in s.split(',') if p.strip() and p.strip()!='void']

def param_type(p):
    p = re.sub(r'/\*.*?\*/','',p).strip()
    p = re.sub(r'(\(\s*__cdecl\s*\*)\s*\w+(\))',r'\1\2',p)
    if '(' in p: return p
    # Remove an optional identifier, not a bare type such as unsigned int.
    m = re.match(r'^(.*[\s*&])([A-Za-z_]\w*)$',p)
    if m and m[2] not in ('int','char','short','long','double','float','void'): return m[1].strip()
    return p

def check_source(ea, path, timeout):
    results={}
    for tool in ('gate','emudiff'):
        cmd=[sys.executable,os.path.join(ROOT,'tools',tool+'.py'),f'{ea:08x}']
        cmd += [path] if tool=='gate' else ['--src',path]
        try:
            p=subprocess.run(cmd,cwd=ROOT,text=True,capture_output=True,timeout=timeout)
            results[tool]=(p.stdout+p.stderr).strip()
            results[tool+'_exit']=p.returncode
        except subprocess.TimeoutExpired:
            results[tool]=f'timeout after {timeout}s (no verdict)'
            results[tool+'_exit']=124
    results['passed']=(results['gate_exit']==0 and results['gate']=='ok'
                       and results['emudiff_exit']==0
                       and bool(re.search(r'\bsame\s+\d+/\d+ profiles',results['emudiff'])))
    return results

def check_source_shared(ea,path,db):
    """Unbounded shared-cache mode for retrying completed emulator comparisons.

    Exactly gate.gate and emudiff.check(src=...), with fresh compiles. Useful
    when repeated DB/caller-index construction dominates the process timeout.
    """
    import gate,emudiff
    ok,detail=gate.gate(ea,path,db)
    verdict,detail2=emudiff.check(ea,src=path)
    return {'gate':'ok' if ok else detail,'gate_exit':0 if ok else 1,
            'emudiff':f'{ea:08x} {verdict} {detail2}','emudiff_exit':0,
            'passed':ok and verdict=='same'}

def main():
    ap=argparse.ArgumentParser(); ap.add_argument('eas',nargs='*')
    for flag in ('list','write','check'): ap.add_argument('--'+flag,action='store_true')
    ap.add_argument('--json'); ap.add_argument('--results',default='work/initfn/results.json')
    ap.add_argument('--jobs',type=int,default=1)
    ap.add_argument('--timeout',type=int,default=60)
    ap.add_argument('--prior',help='retain earlier failure diagnostics for candidates already removed')
    ap.add_argument('--shared',action='store_true',help='unbounded tools with shared caches (requires --jobs 1)')
    args=ap.parse_args(); db=DB(); eas=[int(a,16) for a in args.eas] or targets(db)
    if args.shared and args.jobs!=1:ap.error('--shared requires --jobs 1')
    allowed=set(targets(db))
    if any(ea not in allowed for ea in eas):
        ap.error('addresses must belong to the selected game/todo initializer range; library code is never emitted')
    prior={r['ea']:r for r in json.load(open(args.prior))} if args.prior else {}
    rows=[]; starts=sorted(db.F)
    for ea in eas:
        r={'ea':f'{ea:08x}','shape':classify(db,ea),'size':db.F[ea]['size']}
        try:
            span,pad,_=room(db,ea,starts)
            if span<5 or not pad: raise Unsupported('no safe five-byte patch room')
            src=Emitter(db,ea).generate(); r['supported']=True
        except Unsupported as e: src=None; r['supported']=False; r['reason']=str(e)
        path=os.path.join(ROOT,'src','auto',f'{ea:08x}.cpp')
        if args.write and src and not os.path.exists(path):
            with open(path,'x') as f:f.write(src)
        rows.append(r)
        if args.list: print(r)
    if args.check:
        # Worker processes are tool invocations, not code-producing agents.
        # Timeouts are failures, never an excuse to label an inconclusive same.
        with concurrent.futures.ThreadPoolExecutor(max_workers=args.jobs) as pool:
            pending={}
            for r in rows:
                path=os.path.join(ROOT,'src','auto',r['ea']+'.cpp')
                if os.path.exists(path):
                    if args.shared:
                        future=pool.submit(check_source_shared,int(r['ea'],16),path,db)
                    else:future=pool.submit(check_source,int(r['ea'],16),path,args.timeout)
                    pending[future]=(r,path)
                else:
                    if r['ea'] in prior:
                        old=prior[r['ea']]
                        for key in ('gate','gate_exit','emudiff','emudiff_exit'):
                            if key in old:r[key]=old[key]
                        r['rechecked']=False
                    else:r['gate']='no candidate: '+r.get('reason','file absent')
                    r['passed']=False
            for future in concurrent.futures.as_completed(pending):
                r,path=pending[future]; r.update(future.result())
                if not r['passed']:
                    # Keep the rejected text for diagnosis, outside src/.
                    os.makedirs(os.path.join(ROOT,'work/initfn/rejected'),exist_ok=True)
                    with open(os.path.join(ROOT,'work/initfn/rejected',r['ea']+'.cpp'),'w') as f:f.write(open(path).read())
                    os.remove(path)
                print(r['ea']+' '+('PASS' if r['passed'] else 'FAIL')+' '+r['gate']+' | '+r['emudiff'],flush=True)
                with open(args.results,'w') as f:json.dump(rows,f,indent=2)
    if args.json:
        with open(args.json,'w') as f:json.dump(rows,f,indent=2)
    if args.check:
        with open(args.results,'w') as f:json.dump(rows,f,indent=2)
    counts=collections.Counter(r['shape'] for r in rows)
    print(json.dumps(dict(counts),indent=2))
    print(f"total {len(rows)}, supported {sum(r['supported'] for r in rows)}, passed {sum(r.get('passed',False) for r in rows)}")

if __name__=='__main__': main()
