"""Executed inside test_offline.py; actual game callers plus boundary contracts.
Heap/Win32/CRT are doubles. No original code or online transport is executed.
"""
import subprocess

# Test every new entry individually, including their original stack cleanups.
p, output = 0x10004000, 0x10004500
u.mem_write(p, b'\xcc' * 0x400)
heads = []
for offset in (0x9c, 0xac):
    head = call('session_container_head_regs', p + offset)
    heads.append(head)
    assert blocks[head] == 0x40 and words(head, 2) == (head, head)
    put(p + offset + 4, head, 0)
    call('session_container_clear_regs', p + offset)
assert heads[0] != heads[1]
put(p + 0xa4, 1); rejected('session_container_clear_regs', p + 0x9c); put(p + 0xa4, 0)
for head in heads: call(FREE, args=(head,))
assert call('session_embedded_string_regs', p) == p
assert text(words(p)[0]) == b'prudp:/' and words(words(p)[0] - 4)[0] == 8
assert words(p + 4)[0] == 0xcccccccc
call('session_string_destruct_regs', p)
assert call('runtime_state_regs') == data(0x8baa9c)
put(data(0x8baa9c) + 0x10, 0x11)
runtime = call('runtime_state_regs'); put(runtime + 0x10, words(runtime + 0x10)[0] | 0x4000000)
assert words(data(0x8baa9c) + 0x10)[0] == 0x4000011
put(p + 0x64, output); put(p + 0x70, output + 4)
assert call('clump_manager_get_regs', p) == output
assert call('object_manager_get_regs', p) == output + 4
put(p + 4, output + 8)
put(p + 0x5c, output + 12)
assert call('referenced_object_get_regs', p) == output + 8
assert call('scene_controller_get_regs', p) == output + 12
put(p + 4, 0)
put(p + 0x5c, 0)
assert call('referenced_object_get_regs', p) == 0
assert call('scene_controller_get_regs', p) == 0
assert call('constant_enabled_regs') & 255 == 1
put(p + 0xc, 0); assert call('selected_object_test_regs', p) & 255 == 0
put(p + 0xc, output); assert call('selected_object_test_regs', p) & 255 == 1
for flags in ((0, 0), (0, 1), (1, 0), (1, 1)):
    assert call('selection_context_regs', args=flags) == 0
assert call('singleton_lookup_regs', args=(output,)) == 0
assert call('callback_register_reject_regs', args=(output,)) == 0
assert call('callback_unregister_regs', args=(output,)) == 0
put(data(0x8ba9c4), 0)
assert call('class_index_lookup_regs', args=(0,)) == 0
# Preserve an existing genuine registry's pointer identity; no fake class object.
put(output, 1, output + 8); put(output + 8, p)
put(data(0x8ba9c4), output)
assert call('class_index_lookup_regs', args=(0,)) == p
assert call('class_index_lookup_regs', args=(1,)) == 0
put(data(0x8ba9c4), 0)
call('selection_link_id_regs', p, (17,), 4)
assert words(p, 2) == (0, 17) and u.mem_read(p + 8, 1) == b'\1'
put(output, 0); call('selection_link_reset_regs', p, (output,), 4)
assert words(p, 2) == (0, 0)
call('selection_link_release_regs', p)
put(p, output); rejected('selection_link_release_regs', p); put(p, 0)
call('selection_construct_regs', p, (0, 0), 8)
assert words(p, 3) == (0, 0, 0) and u.mem_read(p + 0xc, 1) == b'\1'
assert words(p + 0x10, 3) == (0, 0, 0xffffffff)
assert u.mem_read(p + 0x1c, 1) == b'\1' and words(p + 0x20)[0] == 0
assert words(p + 0x24)[0] == 0xcccccccc
rejected('selection_construct_regs', p, (1, 0), 8)
for entry in ('selection_reject_id_regs', 'selection_reject_object_regs'):
    assert call(entry, p, (output,), 4) == 0
assert call('selection_first_regs', p, (output,), 4) == output and words(output)[0] == 0xffffffff
assert call('selection_last_regs', p, (output,), 4) == output and words(output)[0] == 0
sentinel = call(ALLOC, args=(24,))
u.mem_write(sentinel, b'\xcc' * 24)
put(sentinel + 20, 0xfeedface)
call('selection_sentinel_regs', sentinel, (1,), 4)
assert words(sentinel)[0] == data(0x859fa0)
assert u.mem_read(sentinel + 4, 2) == b'\1\0'
assert u.mem_read(sentinel + 6, 2) == b'\xcc\xcc'
assert words(sentinel + 8)[0] == 0 and words(sentinel + 16)[0] == 1
assert words(sentinel + 12)[0] == 0xcccccccc
assert words(sentinel + 20)[0] == 0xfeedface
call('selection_assign_regs', p, (sentinel,), 4)
assert u.mem_read(sentinel + 4, 2) == b'\2\0'
assert words(p + 0x10, 3) == (sentinel, 0xffffffff, 0)
call('selection_release_regs', sentinel)
call('selection_assign_regs', p, (sentinel,), 4) # self-assignment must not free
call('selection_addref_regs', sentinel); call('selection_release_regs', sentinel)
call('selection_assign_regs', p, (0,), 4)
assert sentinel not in blocks
sentinel = call(ALLOC, args=(20,)); call('selection_sentinel_regs', sentinel, (1,), 4)
call('selection_delete_regs', sentinel, (0,), 4)
assert sentinel in blocks and u.mem_read(sentinel + 4, 2) == b'\0\0'
call('selection_delete_regs', sentinel, (1,), 4)
assert not blocks

# Actual compiled game wrappers. All unexercised dependencies trap rather than
# being mocked as success; the offline branch must genuinely avoid them.
game_roots = (0x73f080, 0x73c380, 0x697130, 0x697070, 0x4f21d0, 0x4856e0, 0x74a910, 0x73eea0,
              0x73ecc0, 0x74a770, 0x73d710, 0x73de40, 0x743a10, 0x743a70, 0x7437e0, 0x738870)
emitted = [e for e in json.loads(manifest.read_text())['targets'] if e['emitted']]
entries = {}
for e in emitted:
    a = int(e['address'], 16)
    if a in netzstub.SUPPORT_NAMES:
        name = netzstub.SUPPORT_NAMES[a] + '_regs'
    elif a in netzstub.STRINGS:
        name = {0x64a690:'string_default', 0x64a6b0:'string_construct',
                0x64a6f0:'string_destruct', 0x64a710:'string_assign'}[a] + '_regs'
    elif a == netzstub.CTOR: name = 'gathering_construct_regs'
    else: name = f'offline_{a:08x}'
    entries[a] = emu.Obj(path, name).entry
entries.update({0x6470e0: NETFREE, 0x6471c0: FREE, 0x647160: ALLOC,
                0x691110: FREE, 0x690e60: FREE, 0x6910f0: ALLOC})
oldbase, oldext = emu.OBJ_BASE, emu.EXT_BASE
loaded = []
for i, a in enumerate(game_roots):
    f = root / f'build/standalone/src_auto_{a:08x}.cpp.obj'
    assert f.exists(), f'run tools/linktest.py: {f}'
    emu.OBJ_BASE = 0x59000000 + i * 0x10000
    emu.EXT_BASE = oldext + 0x90000 + i * 0x1000
    u.mem_map(emu.EXT_BASE, 0x1000)
    o = emu.Obj(str(f), emu.hook_name(a, str(root / f'src/auto/{a:08x}.cpp')))
    u.mem_map(emu.OBJ_BASE, o.end - emu.OBJ_BASE)
    for sec in o.secs: u.mem_write(sec['base'], bytes(sec['data']))
    entries[a] = o.entry; loaded.append((o, f, emu.OBJ_BASE, emu.EXT_BASE))
for o, f, base, ext in loaded:
    names = linktest.demangle([n for n in o.ext if n.startswith('?')])
    for n, at in o.ext.items():
        m = re.search(r'csf::(fn_slot|gv|vt)<(\d+)>', names.get(n, ''))
        if m:
            kind, a = m[1], int(m[2])
            if kind == 'fn_slot':
                if a in entries: put(at, entries[a])
                else: put(at, at); externs[at] = '_abort'
            else:
                destination = 0x20000000 + a - 0x820000 if a >= 0x820000 else 0x200f8000
                aliases[a] = destination
                put(at, destination)
        else:
            externs[at] = n if n in ('_memcpy', '_memset', '_memmove') else '_abort'
    symbols = subprocess.run(['llvm-nm-21', str(f)], check=True, capture_output=True, text=True).stdout
    emu.OBJ_BASE, emu.EXT_BASE = base, ext
    for n in re.findall(r'\bt (__GLOBAL__sub_I_\S+)', symbols): call(emu.Obj(str(f), n[1:]).entry)
emu.OBJ_BASE, emu.EXT_BASE = oldbase, oldext
put(data(0x859fa0), *(entries[a] for a in (0x662bd0,0x64fe50,0x64fe90,0x662c30,0x662c00,0x662c60,0x662d10)))
# Exercise every slot through the relocated original table, not a test dummy.
sentinel = call(ALLOC, args=(20,))
call('selection_sentinel_regs', sentinel, (1,), 4)
table = words(sentinel)[0]
call(words(table + 4)[0], sentinel)
assert u.mem_read(sentinel + 4, 2) == b'\2\0'
call(words(table + 8)[0], sentinel)
assert u.mem_read(sentinel + 4, 2) == b'\1\0'
for offset in (12, 16):
    assert call(words(table + offset)[0], sentinel, (output,), 4) == 0
for offset, expected in ((20, 0xffffffff), (24, 0)):
    assert call(words(table + offset)[0], sentinel, (output,), 4) == output
    assert words(output)[0] == expected
call(words(table)[0], sentinel, (1,), 4)
assert not blocks
# Both notice maps really exist; game check of a null entry must record failure
# and skip the unconditionally dereferenced registry-result/class virtual branch.
for a in (0x8b9980, 0x8b9968): call('thread_map_construct_regs', data(a), (0,), 4)
map_blocks = set(blocks)
call('status_notice_regs', args=(0,0,0xa0030004,0x77))
for a, expected in ((0x8b9980,0xa0030004),(0x8b9968,0x77)):
    head = words(data(a) + 0x10)[0]; node = words(head + 4)[0]
    assert words(node + 0x10)[0] == expected
for _ in range(2):
    u.mem_write(p, b'\xcc' * 0x400)
    call(entries[0x73f080], p)
    assert words(p + 0xa4)[0] == words(p + 0xb4)[0] == 0
    assert words(p + 0xa0)[0] != words(p + 0xb0)[0]
    assert text(words(p + 0x160)[0]) == b'prudp:/'
    assert words(p + 356)[0] == 0xcccccccc
    call(entries[0x73eea0], p)
    assert words(p + 0xa0)[0] == words(p + 0xb0)[0] == 0
    assert set(blocks) == map_blocks
u.mem_write(p, b'\xcc' * 0x100)
call(entries[0x73d710], p)
assert words(p, 3) == (0,0,0) # caller sees empty selection, no online loop
call('selection_assign_regs', p, (0,), 4)
call(entries[0x73de40]) # full game iterator allocate/retain/release/clear cycle
call('selection_link_id_regs', p, (0,), 4)
assert call(entries[0x743a10], p) & 255 == 0
for offset in range(0, 0x34, 4): put(data(0x8bc12c) + offset, 0)
put(data(0x8bc188), 0)
call(entries[0x743a70]) # stopped services, failed reference check, skip object+68
call(entries[0x7437e0]) # absent singleton skips setter calls and virtual+40 tail
for a in (0x8b9968, 0x8b9980): call('thread_map_destruct_regs', data(a))
assert not blocks and not criticals
assert {name + '_regs' for name in netzstub.WINMAIN_SUPPORT.values()} <= tested
print(f'WinMain boundary: all 17 slots plus ownership/virtual support; {len(game_roots)} actual game callers')
