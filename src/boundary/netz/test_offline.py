#!/usr/bin/env python3
"""Execute the generated COFF boundary in Unicorn, with a tracked fake game heap.
Not a game/SP test or an original-function equivalence test.
Usage: .venv/bin/python src/boundary/netz/test_offline.py OBJECT [MANIFEST]
"""
import pathlib
import json
import struct
import sys

sys.path.insert(0, str(pathlib.Path(__file__).resolve().parents[3] / 'tools'))
import emudiff as emu
import linktest
import re
from unicorn import Uc, UC_ARCH_X86, UC_MODE_32, UC_HOOK_CODE
from unicorn import x86_const as X

# Never execute the original CRT. Supply tracked test implementations instead.
emu.crt_ranges = lambda: ({}, {})
path = sys.argv[1]
obj = emu.Obj(path, 'request_construct_regs')
u = Uc(UC_ARCH_X86, UC_MODE_32)
u.mem_map(emu.OBJ_BASE, obj.end - emu.OBJ_BASE)
for sec in obj.secs:
    u.mem_write(sec['base'], bytes(sec['data']))
u.mem_map(emu.EXT_BASE, 0x10000)
u.mem_map(emu.SYS_BASE, 0x1000)
u.mem_write(emu.RET_STUB, b'\xc3')
u.mem_write(emu.COOKIE, struct.pack('<I', 0xbb40e64e))
u.mem_map(0x10000000, 0x10000)  # stack and caller storage
u.mem_map(0x30000000, 0x100000)  # tracked heap
u.mem_map(0x20000000, 0x100000)  # alias-preserving test data/vtables
END, ALLOC, FREE = emu.EXT_BASE + 0x1000, emu.EXT_BASE + 0x1100, emu.EXT_BASE + 0x1200
next_block, blocks = 0x30000000, {}

def words(at, count=1):
    return struct.unpack('<' + 'I' * count, u.mem_read(at, count * 4))

def put(at, *values):
    u.mem_write(at, struct.pack('<' + 'I' * len(values), *values))

def text(at):
    result = bytearray()
    while at and u.mem_read(at, 1) != b'\0':
        result += u.mem_read(at, 1)
        at += 1
    return bytes(result)

externs = {}
demangled = linktest.demangle([name for name in obj.ext if name.startswith('?')])
aliases = {}
criticals = set()
callbacks = []
class OfflineRejected(Exception): pass
for name, at in obj.ext.items():
    if 'fn_slot' in name:
        # The only function slots are game::Mem_Alloc (6910f0) and Mem_Free (691110).
        fn = ALLOC if '$0GJBAPA@' in name else FREE
        assert '$0GJBAPA@' in name or '$0GJBBBA@' in name, name
        put(at, fn)
    elif 'csf::gv<' in demangled.get(name, '') or 'csf::vt<' in demangled.get(name, ''):
        original = int(re.search(r'<(\d+)>', demangled[name])[1])
        target = 0x20000000 + original - 0x820000
        assert 0x20000000 <= target < 0x20100000
        aliases[original] = target
        put(at, target)
    elif name.startswith('__imp_'):
        entry = emu.EXT_BASE + 0x4000 + len(externs) * 16
        put(at, entry)
        externs[entry] = name
    else:
        externs[at] = name

def hook(uc, pc, size, data):
    global next_block
    if pc == END:
        uc.emu_stop()
        return
    if pc not in externs and pc not in (ALLOC, FREE):
        return
    sp = uc.reg_read(X.UC_X86_REG_ESP)
    ret, a, b, c = words(sp, 4)
    name = externs.get(pc)
    result = 0
    pop = 0
    if pc == ALLOC:
        result = next_block
        next_block += (a + 31) & ~15
        blocks[result] = a
        u.mem_write(result, bytes(a))
    elif pc == FREE:
        if a:
            assert a in blocks, f'invalid/double free {a:x}'
            assert a not in criticals, 'critical section freed before DeleteCriticalSection'
            del blocks[a]
    elif name == '_strlen':
        result = len(text(a))
    elif name in ('_memcpy', '_memmove'):
        u.mem_write(a, bytes(u.mem_read(b, c)))
        result = a
    elif name == '_memset':
        u.mem_write(a, bytes([b & 255]) * c)
        result = a
    elif name == '__imp__InitializeCriticalSection@4':
        assert blocks[a] == 24 and a not in criticals
        criticals.add(a); u.mem_write(a, bytes(24)); put(a + 4, 0xffffffff)
        pop = 4
    elif name == '__imp__DeleteCriticalSection@4':
        assert a in criticals; criticals.remove(a); pop = 4
    elif name in ('__imp__QueryPerformanceCounter@4', '__imp__QueryPerformanceFrequency@4'):
        u.mem_write(a, struct.pack('<Q', 123456789 if 'Counter' in name else 1000000))
        result = 1; pop = 4
    elif name == '__imp__GetCurrentThreadId@0':
        result = 123
    elif name == '__aulldiv':
        numerator = a | b << 32; denominator = c | words(sp + 16)[0] << 32
        quotient = numerator // denominator
        result = quotient & 0xffffffff; uc.reg_write(X.UC_X86_REG_EDX, quotient >> 32); pop = 16
    elif name == 'test_atexit':
        callbacks.append(a)
    elif name == 'test_netz_free':
        assert b in blocks, f'invalid NetZ free {b:x}'; del blocks[b]
    elif name == 'test_custom_clock':
        assert uc.reg_read(X.UC_X86_REG_ECX) == 0x10002f00
        u.mem_write(a, struct.pack('<Q', 0x123456789abc))
        pop = 4
    elif name == '_abort':
        raise OfflineRejected()
    else:
        raise AssertionError(name)
    uc.reg_write(X.UC_X86_REG_EAX, result)
    uc.reg_write(X.UC_X86_REG_ESP, sp + 4 + pop)
    uc.reg_write(X.UC_X86_REG_EIP, ret)

u.hook_add(UC_HOOK_CODE, hook)
checked = 0
tested = set()

def call(name, self=0, args=(), pop=0):
    global checked
    entry = name if isinstance(name, int) else emu.Obj(path, name).entry
    sp = 0x10008000
    put(sp, END, *args, 0, 0, 0)
    saved = [(X.UC_X86_REG_EBX, 0x12345678), (X.UC_X86_REG_ESI, 0x23456789),
             (X.UC_X86_REG_EDI, 0x3456789a), (X.UC_X86_REG_EBP, 0x456789ab)]
    for reg, value in saved:
        u.reg_write(reg, value)
    u.reg_write(X.UC_X86_REG_ESP, sp)
    u.reg_write(X.UC_X86_REG_ECX, self)
    u.reg_write(X.UC_X86_REG_EDX, 0)
    u.emu_start(entry, END + 1, timeout=1000000, count=100000)
    assert u.reg_read(X.UC_X86_REG_EIP) == END, name
    assert u.reg_read(X.UC_X86_REG_ESP) == sp + 4 + pop, name
    for reg, value in saved:
        assert u.reg_read(reg) == value, name
    checked += 1
    if isinstance(name, str): tested.add(name)
    return u.reg_read(X.UC_X86_REG_EAX)

# Owned output: initialized format replacement, separate URL output, final frees.
s, out, source = 0x10000100, 0x10000200, 0x10000300
u.mem_write(source, b'name\0')
assert call('string_construct_regs', s, (source,), 4) == s
assert text(words(s)[0]) == b'name'
assert words(words(s)[0] - 4)[0] == 5
call('empty_format_output_regs', args=(s, source))
assert text(words(s)[0]) == b''
assert call('empty_string_output_regs', args=(out,), pop=4) == out
assert text(words(out)[0]) == b''
assert words(words(out)[0] - 4)[0] == 1
call('string_destruct_regs', s)
call('string_destruct_regs', out)
assert not blocks

# Circular sentinel, one String node, counter and clear retain head for caller free.
owner = 0x10000400
head = call('list_head_regs')
assert words(head, 3) == (head, head, 0)
put(owner, 0, head, 0)
node = call('list_node_regs', owner, (head, head, source), 12)
assert words(node, 2) == (head, head)
assert text(words(node + 8)[0]) == b''
put(head, node, node)
call('list_count_regs', owner, (1,), 4)
assert words(owner + 8)[0] == 1
call('list_clear_strings_regs', owner)
assert words(head, 2) == (head, head) and words(owner + 8)[0] == 0
assert blocks == {head: 12}
call(FREE, args=(head,))
assert not blocks

# Gathering local assignment deep-copies temporary and frees both names.
gathering = 0x10001000
call('gathering_construct_regs', gathering)
call('string_construct_regs', s, (source,), 4)
assert call('gathering_name_regs', gathering, (s,), 4) == gathering + 0x24
assert text(words(gathering + 0x24)[0]) == b'name'
assert words(gathering + 0x24)[0] != words(s)[0]
call('string_destruct_regs', s)
call('gathering_type_regs', gathering, (7, 0), 8)
assert words(gathering + 0x14, 2) == (7, 0)
call('gathering_destruct_regs', gathering)
assert not blocks

# Request object: no pending state, matched six-slot ABI, caller deleting destructor.
request = call(ALLOC, args=(0x70,))
assert call('request_construct_regs', request) == request
assert words(request + 0xc)[0] == 3
assert words(request + 0x44)[0] == 0x80010001
vtable = words(request)[0]
for i, pop in enumerate((0, 0, 0, 8, 0), 1):
    assert call(words(vtable + 4 * i)[0], request, (0, 0), pop) == 0
call(words(vtable)[0], request, (1,), 4)
assert not blocks

# Base connection: exactly 92 bytes; ensure no 112-byte derived-object overrun.
connection = call(ALLOC, args=(92,))
u.mem_write(connection + 92, b'canary!!')
assert call('connection_construct_regs', connection) == connection
assert u.mem_read(connection + 92, 8) == b'canary!!'
assert words(connection + 0xc)[0] == 3
assert words(connection + 0x44)[0] == 0x80010001
vtable = words(connection)[0]
for i, pop in enumerate((0, 0, 0, 8, 0), 1):
    assert call(words(vtable + 4 * i)[0], connection, (0, 0), pop) == 0
call(words(vtable)[0], connection, (1,), 4)
assert not blocks

# Session strings share the prefix ABI, but have their own generated wrappers.
call('session_string_construct_regs', s, (source,), 4)
call('session_string_construct_regs', out, (source,), 4)
old = words(out)[0]
assert call('session_string_assign_regs', out, (s,), 4) == out
assert old not in blocks and words(out)[0] != words(s)[0]
call('session_string_destruct_regs', s)
assert text(words(out)[0]) == b'name'
call('session_string_assign_regs', out, (out,), 4)  # self-assignment
call('session_string_destruct_regs', out)
assert not blocks

# Borrowed endpoint objects: two distinct 28-byte objects; virtual reset ret 0,
# delete ret 4 must not attempt to free process-owned storage even for flags=1.
first = call('primary_endpoint_regs')
second = call('secondary_endpoint_regs')
assert first != second and first == call('primary_endpoint_regs')
for endpoint in (first, second):
    assert words(endpoint + 4, 6) == (0, 0, 0xffffffff, 0, 0, 0)
    vt = words(endpoint)[0]
    assert call(words(vt + 4)[0], endpoint) == 0
    assert call(words(vt)[0], endpoint, (1,), 4) == endpoint
assert not blocks

# Plain list counterpart and borrowed diagnostic result.
head = call('plain_list_head_regs')
node = call(ALLOC, args=(12,))
put(node, head, head, 123)
put(head, node, node)
put(owner, 0, head, 1)
call('list_clear_plain_regs', owner)
assert blocks == {head: 12}
call(FREE, args=(head,))
call('offline_result_regs', out)
assert words(out)[0] == 0x80010001 and text(words(out + 4)[0]) == b'offline'
assert call('allocator_token_regs') == call('allocator_token_regs')

factory = call('gathering_factory')
assert words(factory + 0x14)[0] == 1
assert call('gathering_class_name', factory, (out,), 4) == out
assert text(words(out)[0]) == b'Gathering'
call('string_destruct_regs', out)
call('gathering_delete', factory, (1,), 4)
assert not blocks

# Every result-only wrapper, using audited binary cleanup from the manifest.
manifest = pathlib.Path(sys.argv[2]) if len(sys.argv) > 2 else pathlib.Path(__file__).with_name('manifest.json')
for entry in json.loads(manifest.read_text())['targets']:
    if entry['emitted'] and entry['family'] in (
            'audited-offline-rejection', 'offline-false', 'offline-noop',
            'offline-zero', 'offline-error'):
        address = int(entry['address'], 16)
        expected = 0xffffffff if entry['family'] == 'offline-error' else 0
        assert call(f'offline_{address:08x}', self=owner,
                    args=(0,) * 8, pop=entry['ret_bytes']) == expected
        assert u.reg_read(X.UC_X86_REG_ECX) == owner
assert not blocks

# Process-object initializers: reproduce their ordered boundary calls, retained
# vtable overrides, sentinel writes, and reverse lifetime cleanup. Heap/Win32
# imports are tracked doubles, not claims of Windows execution.
def data(original): return aliases[original]
def rejected(name, self=0, args=(), pop=0):
    try: call(name, self, args, pop)
    except OfflineRejected: return
    raise AssertionError(f'{name}: fabricated success for an unowned online state')
put(data(0x8a6544), 1)
put(data(0x8b9a88), 0)
put(data(0x8b9a8c), 0)
put(data(0x8b9a24), 0)
put(data(0x8b99b0), 0)
objects = 0x10002000
u.mem_write(objects, b'\xcc' * 0x4000)

# 828ba0/828bc0/828cc0: one current-thread tree node per object;
# shared base registry remains alive until the last destructor.
maps = []
for i, (ctor, dtor, value) in enumerate((
        ('thread_map_construct_regs', 'thread_map_destruct_regs', 0),
        ('thread_map_construct_regs', 'thread_map_destruct_regs', 17),
        ('thread_value_construct_regs', 'thread_value_destruct_regs', 29))):
    p = objects + i * 0x20
    assert call(ctor, p, (value,), 4) == p
    h = words(p + 0x10)[0]; n = words(h + 4)[0]
    assert words(h, 3) == (n, n, n)
    assert words(n, 5) == (h, h, h, 123, value)
    assert u.mem_read(h + 0x14, 2) == b'\1\1'
    assert u.mem_read(n + 0x14, 2) == b'\1\0'
    assert words(p + 0x14)[0] == 1 and words(p + 0x18)[0] == 0xcccccccc
    maps.append((p, dtor))
assert words(data(0x8b9a0c) + 8)[0] == 3
for p, dtor in reversed(maps): call(dtor, p)
assert words(data(0x8b9a0c) + 4, 2) == (0, 0) and not criticals and not blocks

# Every direct critical-section global has a separately owned native-size object.
for i in range(12):
    p = objects + i * 8
    put(p + 4, 0x13579bdf)
    assert call('critical_construct_regs', p) == p
    assert blocks[words(p)[0]] == 24 and words(p + 4)[0] == 0x13579bdf
    call('critical_destruct_regs', p)
assert not blocks and not criticals

call('registry_construct_regs', objects)
call('registry_destruct_regs', objects)
# Scalar registry objects: base +4 next, +8 context storage, two initial/current
# values. Register a real chain and unlink interior/head/tail, not virtual success.
registered = []
for i, name in enumerate(('context_dword_647840_regs', 'context_dword_6523a0_regs',
                          'context_dword_67ac40_regs', 'context_word_657810_regs')):
    p = objects + i * 0x30
    call('registry_base_construct_regs', p)
    put(p + 8, 0); put(p + 0x10, 0x12340000 + i)
    u.mem_write(p + 0xe, struct.pack('<H', 0x3456))
    call('registry_append_regs', args=(p,), pop=4)
    call(name, p)
    q = words(p + 8)[0]
    assert bytes(u.mem_read(q, 2 if i == 3 else 4)) == bytes(u.mem_read(p + (0xe if i == 3 else 0x10), 2 if i == 3 else 4))
    registered.append(p)
assert words(data(0x8b9a88))[0] == registered[0]
assert words(data(0x8b9a8c))[0] == 4
for i in (1, 0, 3, 2):
    p = registered[i]; call('registry_unlink_regs', args=(p,), pop=4)
    call(FREE, args=(words(p + 8)[0],)); call('registry_base_destruct_regs', p)
assert words(data(0x8b9a88), 2) == (0, 0) and not blocks

# 828da0: current/default two-word clock samples, copied not borrowed.
sample = objects + 0x200
assert call('counter_sample_regs', args=(sample,)) == sample
assert struct.unpack('<Q', u.mem_read(sample, 8))[0] == 123456
custom_clock = emu.EXT_BASE + 0x2200
externs[custom_clock] = 'test_custom_clock'
put(0x10002f00, 0x10002f20); put(0x10002f20, 0, custom_clock)
put(data(0x8b99b0), 0x10002f00)
call('counter_sample_regs', args=(sample,))
assert struct.unpack('<Q', u.mem_read(sample, 8))[0] == 0x123456789abc
put(data(0x8b99b0), 0); call('counter_sample_regs', args=(sample,))
p = objects + 0x240
call('context_pair_construct_regs', p, (sample,), 4)
assert words(p + 0x10, 4) == (123456, 0, 123456, 0) and words(p + 8)[0] == 0
call('registry_unlink_regs', args=(p,), pop=4); call('registry_base_destruct_regs', p)

# 828eb0: all sixteen 0x50-byte objects and their temporary 0x20-byte values.
temporary = objects + 0x300
for i in range(16):
    p = objects + 0x400 + i * 0x50
    call('value_construct_regs', temporary)
    assert words(temporary + 0x18, 2) == (0, 0)
    call('context_value_construct_regs', p, (temporary,), 4)
    assert words(p + 0x18)[0] == words(temporary + 8)[0]
    assert words(p + 8)[0] == 0
    call('value_destruct_regs', temporary)
assert words(data(0x8b9a8c))[0] == 16
for i in reversed(range(16)):
    p = objects + 0x400 + i * 0x50
    call('registry_unlink_regs', args=(p,), pop=4)
    call('context_value_destruct_regs', p)
    call('value_destruct_regs', p + 0x30); call('value_destruct_regs', p + 0x10)
    call('registry_base_destruct_regs', p)
assert words(data(0x8b9a88), 2) == (0, 0)

# 828f80 and 829110/829240/82a3a0/82a3d0: independently owned 12-byte heads.
for name in ('list_factory_668450_regs', 'list_factory_66d400_regs',
             'list_factory_6882d0_regs', 'list_factory_796490_regs', 'list_factory_79c090_regs'):
    h = call(name, temporary); assert words(h, 2) == (h, h) and blocks[h] == 12
    put(temporary + 4, h, 0)
    if name == 'list_factory_668450_regs':
        p = objects + 0x1000
        call('context_list_construct_regs', p, (temporary,), 4)
        h1 = words(p + 0x10)[0]; h2 = words(p + 0x20)[0]
        assert h1 != h2 != h and words(h1, 2) == (h1, h1) and words(h2, 2) == (h2, h2)
        call('context_list_destruct_regs', p)
    call('list_clear_plain_regs', temporary); call(FREE, args=(h,))
assert not blocks

# 8290b0/82a500: exact marker +0x14=1, +0x15 initially zero,
# initializer supplies sentinel self-links; atexit empty erase fills its iterator.
for factory, clear in (('tree_factory_66b390_regs', 'tree_range_66bbe0_regs'),
                       ('tree_factory_7aa3c0_regs', 'tree_range_7aa730_regs')):
    p = objects + 0x1100; h = call(factory, p)
    assert blocks[h] == 24 and words(h, 3) == (0, 0, 0)
    assert u.mem_read(h + 0x14, 2) == b'\1\0'
    u.mem_write(h + 0x15, b'\1'); put(h, h, h, h); put(p + 4, h, 0)
    assert call(clear, p, (out, h, h), 12) == out and words(out)[0] == h
    put(p + 8, 1)
    rejected(clear, p, (out, h, h), 12)
    put(p + 8, 0)
    call(FREE, args=(h,))

# 828dd0: eight zero words, +0x20 byte enabled, +0x24 zero.
p = objects + 0x1200
call('settings_construct_regs', p)
assert words(p, 8) == (0,) * 8 and u.mem_read(p + 0x20, 1) == b'\1' and words(p + 0x24)[0] == 0
call('settings_destruct_regs', p)
# 828df0/829150: two owned, different-size children, no online extension.
for i in range(2):
    p = objects + 0x1300 + i * 0x20
    call('factory_construct_regs', p)
    child1, child2, extension = words(p + 4, 3)
    assert blocks[child1] == 4 and blocks[child2] == 32 and extension == 0
    assert words(child2 + 0x1c)[0] == 123456
    put(p + 0xc, 1); rejected('factory_destruct_regs', p); put(p + 0xc, 0)
    call('factory_destruct_regs', p)
assert not blocks

# 828f00: seed=1, 624 seed words with original 69069 multiplier, trailing index/twist.
p = objects + 0x1800
call('random_construct_regs', p)
seed = 1
for i in range(624):
    assert words(p + i * 4)[0] == seed; seed = seed * 69069 & 0xffffffff
assert words(p + 0x9c0, 3) == (624, 0, 0x9908b0df)
assert words(p + 0x9cc)[0] == 0xcccccccc
# RootDO and references: only the exact bytes written by original constructors.
p = objects + 0x2800
call('root_construct_regs', p, (1,), 4)
assert words(p + 4)[0] == words(p + 0xc)[0] == 0 and u.mem_read(p + 0x10, 1) == b'\1'
call('root_destruct_regs', p)
call('reference_construct_regs', p)
assert words(p)[0] == 0 and u.mem_read(p + 4, 2) == b'\0\0'
call('timeout_construct_regs', p)
assert words(p, 4) == (0x40800000, 0xbf800000, 1000, 250)
assert u.mem_read(p + 0x10, 1) == b'\1' and words(p + 0x14)[0] == 3500
call('interpolation_construct_regs', p)
assert words(p, 8) == (0,) * 7 + (0xbf800000,)
put(p + 0xc, 0x12345678)
assert call('provider_get_regs', p) == 0x12345678  # not a fake/null provider

# 829270/829290: actual PRUDP named stopwatch records and process registration.
stops = [objects + 0x2900, objects + 0x2940]
for p in stops:
    assert call('stopwatch_construct_regs', p, (source, 60000), 8) == p
    assert words(p + 0xc, 3) == (source, 123456, 60000) and words(p + 0x20)[0] == 0x3f800000
assert words(words(data(0x8b9a24))[0] + 8)[0] == 2
for p in reversed(stops): call('stopwatch_destruct_regs', p)
assert words(data(0x8b9a24))[0] == 0 and not blocks and not criticals
put(data(0x8a6544), 0)
rejected('context_dword_647840_regs', objects)
put(data(0x8a6544), 1)
import netzstub
assert {v + '_regs' for v in netzstub.INIT_SUPPORT.values()} <= tested
exec(compile(pathlib.Path(__file__).with_name('test_initializers.py').read_text(),
             str(pathlib.Path(__file__).with_name('test_initializers.py')), 'exec'))
exec(compile(pathlib.Path(__file__).with_name('test_winmain.py').read_text(),
             str(pathlib.Path(__file__).with_name('test_winmain.py')), 'exec'))
exec(compile(pathlib.Path(__file__).with_name('test_vtables.py').read_text(),
             str(pathlib.Path(__file__).with_name('test_vtables.py')), 'exec'))
print(f'PASS: {checked} emulated calls, all 46 initializer and 17 WinMain slots, ESP/nonvolatile registers; no leaked test allocations')
