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
u.mem_map(0x10000000, 0x10000)  # stack and caller storage
u.mem_map(0x30000000, 0x100000)  # tracked heap
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
for name, at in obj.ext.items():
    if 'fn_slot' in name:
        # The only function slots are game::Mem_Alloc (6910f0) and Mem_Free (691110).
        fn = ALLOC if '$0GJBAPA@' in name else FREE
        assert '$0GJBAPA@' in name or '$0GJBBBA@' in name, name
        put(at, fn)
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
    if pc == ALLOC:
        result = next_block
        next_block += (a + 31) & ~15
        blocks[result] = a
        u.mem_write(result, bytes(a))
    elif pc == FREE:
        if a:
            assert a in blocks, f'invalid/double free {a:x}'
            del blocks[a]
    elif name == '_strlen':
        result = len(text(a))
    elif name == '_memcpy':
        u.mem_write(a, bytes(u.mem_read(b, c)))
        result = a
    elif name == '_memset':
        u.mem_write(a, bytes([b & 255]) * c)
        result = a
    else:
        raise AssertionError(name)
    uc.reg_write(X.UC_X86_REG_EAX, result)
    uc.reg_write(X.UC_X86_REG_ESP, sp + 4)
    uc.reg_write(X.UC_X86_REG_EIP, ret)

u.hook_add(UC_HOOK_CODE, hook)
checked = 0

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
print(f'PASS: {checked} emulated calls, ESP/nonvolatile registers, owned strings/list/request lifetimes; no leaked test allocations')
