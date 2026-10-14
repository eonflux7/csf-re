"""Called by test_offline.py: execute actual compiled game wrappers in its VM.

This covers the 54 initializers with direct process-boundary calls and all their
registered destructors. 829a20's shared scene-cleanup provider getter is tested
separately: running that scene/RW object graph is outside this boundary test.
"""
import pathlib
import re
import struct
import subprocess
import packet

packet.disasm(0x8289f0, 1)  # initialize read-only image
table = struct.unpack_from('<276I', packet._img, 0x898000 - 0x400000)
root = pathlib.Path(__file__).resolve().parents[3]
REF = re.compile(r'CSF_(?:FUNC\([^;]*?,\s*|ADDR_FN\()\s*(0x[0-9A-Fa-f]+)')
def refs(ea):
    p = root / f'src/auto/{ea:08x}.cpp'
    return {int(a, 16) for a in REF.findall(p.read_text())} - {ea}
roots = [a for a in table if a >= 0x8289f0 and refs(a) & netzstub.INIT_SUPPORT.keys()]
assert len(roots) == 54
entries = {}
for a, name in netzstub.SUPPORT_NAMES.items():
    if a in {int(e['address'], 16) for e in json.loads(manifest.read_text())['targets'] if e['emitted']}:
        entries[a] = emu.Obj(path, name + '_regs').entry
for a, name in {0x64a690: 'string_default', 0x64a6b0: 'string_construct',
                0x64a6f0: 'string_destruct', 0x64a710: 'string_assign'}.items():
    entries[a] = emu.Obj(path, name + '_regs').entry
ATE, NETFREE = emu.EXT_BASE + 0x2000, emu.EXT_BASE + 0x2100
entries.update({0x7aee43: ATE, 0x6470e0: NETFREE, 0x6471c0: FREE, 0x691110: FREE})
externs[ATE] = 'test_atexit'; externs[NETFREE] = 'test_netz_free'
todo, wrappers = list(roots), {}
while todo:
    a = todo.pop()
    if a in entries or a in wrappers: continue
    wrappers[a] = root / f'build/standalone/src_auto_{a:08x}.cpp.obj'
    assert wrappers[a].exists(), f'run tools/linktest.py first: {wrappers[a]}'
    todo.extend(refs(a))
assert len(wrappers) == 109
original_objbase, original_extbase = emu.OBJ_BASE, emu.EXT_BASE
u.mem_map(original_extbase + 0x10000, 0x80000)
loaded = []
for i, (a, p) in enumerate(sorted(wrappers.items())):
    emu.OBJ_BASE = 0x51000000 + i * 0x10000
    emu.EXT_BASE = original_extbase + 0x10000 + i * 0x1000
    name = emu.hook_name(a, str(root / f'src/auto/{a:08x}.cpp'))
    o = emu.Obj(str(p), name)
    assert o.end - emu.OBJ_BASE <= 0x10000
    u.mem_map(emu.OBJ_BASE, o.end - emu.OBJ_BASE)
    for sec in o.secs: u.mem_write(sec['base'], bytes(sec['data']))
    entries[a] = o.entry
    loaded.append((o, p, emu.OBJ_BASE, emu.EXT_BASE))
for o, p, base, ext in loaded:
    names = linktest.demangle([n for n in o.ext if n.startswith('?')])
    for n, at in o.ext.items():
        decoded = names.get(n, '')
        m = re.search(r'csf::(fn_slot|gv|vt)<(\d+)>', decoded)
        if m:
            kind, a = m[1], int(m[2])
            if kind == 'fn_slot':
                assert a in entries, (p, hex(a)); put(at, entries[a])
            else:
                destination = 0x20000000 + a - 0x820000
                assert 0x20000000 <= destination < 0x20100000
                put(at, destination)
        else:
            assert n in ('_memcpy', '_memset'), (p, n)
            externs[at] = n
    # emudiff intentionally drops .CRT sections. Execute the host's constant
    # address-binding initializers explicitly, before original game startup.
    symbols = subprocess.run(['llvm-nm-21', str(p)], check=True, capture_output=True, text=True).stdout
    startup = re.findall(r'\bt (__GLOBAL__sub_I_\S+)', symbols)
    emu.OBJ_BASE, emu.EXT_BASE = base, ext
    for n in startup: call(emu.Obj(str(p), n[1:]).entry)
emu.OBJ_BASE, emu.EXT_BASE = original_objbase, original_extbase
# Restore original data values, notably the one-context default. No original
# instructions are mapped/executed; only the data backing supplied aliases.
original_data = bytes(packet._img[0x820000 - 0x400000:])
u.mem_write(0x20000000, original_data[:0x100000])
callbacks.clear()
for a in roots: call(entries[a])
registered_callbacks = len(callbacks)
while callbacks: call(callbacks.pop())
assert not blocks and not criticals
assert words(data(0x8b9a88), 2) == (0, 0)
assert words(data(0x8b9a24))[0] == 0
print(f'Actual wrappers: {len(roots)} initializers, {registered_callbacks} registered destructors, {len(wrappers)} game objects; no leaked test allocations')
