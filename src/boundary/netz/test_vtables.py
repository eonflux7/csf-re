"""Executed by test_offline: exact local contracts, including virtual dispatch.

Only audited tiny accessors/constants/no-ops execute original instruction bytes.
No transport or library implementation is run; no Windows execution is claimed.
"""
import random
import packet
from capstone import Cs, CS_ARCH_X86, CS_MODE_32

contracts = json.loads(pathlib.Path(__file__).with_name('vtable_contracts.json').read_text())
u.mem_map(0x40000000, 0x1000)
rng = random.Random(0x68b5f0)
p = 0x10005000
for address, contract in contracts.items():
    name = f'offline_{int(address, 16):08x}'
    entry = emu.Obj(path, name).entry
    code = bytes.fromhex(contract['bytes'])
    decoder = Cs(CS_ARCH_X86, CS_MODE_32)
    decoded = lambda b: [(i.mnemonic, i.op_str) for i in decoder.disasm(b, 0)]
    assert decoded(bytes(u.mem_read(entry, len(code)))) == decoded(code), (address, 'compiled instructions differ')
    u.mem_write(0x40000000, code)
    u.ctl_remove_cache(0x40000000, 0x40001000)
    for iteration in range(8):
        memory = bytes(rng.randrange(256) for _ in range(0x100))
        # Exercise null and nonnull member values, not just a default-zero object.
        if iteration == 0:
            memory = bytes(0x100)
        if contract.get('tail_dispatch'):
            memory = struct.pack('<I', 0x10007000) + memory[4:]
            put(0x10007008, 0x40000100)
            u.mem_write(0x40000100, b'\xb8\x55\x00\x00\x00\xc3')
        seed = rng.getrandbits(32)
        flags = 0x202 | (iteration & 1)
        results = []
        for target in (0x40000000, entry):
            u.mem_write(p, memory)
            u.reg_write(X.UC_X86_REG_EAX, seed)
            u.reg_write(X.UC_X86_REG_EFLAGS, flags)
            call(target, p, (0xabcdef01,) * 3, contract['ret_bytes'])
            results.append((tuple(u.reg_read(r) for r in (
                X.UC_X86_REG_EAX, X.UC_X86_REG_ECX, X.UC_X86_REG_EDX,
                X.UC_X86_REG_EFLAGS)), bytes(u.mem_read(p, 0x100))))
        assert results[0] == results[1], (address, iteration)
    if int(address, 16) == 0x68b5f0:
        put(p + 4, 0xdeadbeef)
        assert call(entry, p) == p + 4  # address, NOT the loaded member

# Preserve the binary's actual vtable targets; dispatch through all local slots
# in the three blocking tables, with each independently established ret N.
packet.disasm(0x68b5f0, 1)
tables = json.loads((pathlib.Path(__file__).resolve().parents[3] / 'db/vtables.json').read_text())
for table in tables:
    if int(table['addr'], 16) not in (0x83ff18, 0x861328, 0x861460, 0x867358):
        continue
    at = 0x10006000
    targets = struct.unpack_from('<' + 'I' * table['nslots'], packet._img,
                                 int(table['addr'], 16) - 0x400000)
    assert targets == tuple(int(s, 16) for s in table['slots'])
    for i, address in enumerate(targets):
        contract = contracts.get(f'0x{address:08X}')
        if contract:
            target = emu.Obj(path, f'offline_{address:08x}').entry
            put(at + 4 * i, target)
            if contract.get('tail_dispatch'):
                put(p, 0x10007000)
            call(words(at + 4 * i)[0], p, (0,) * 3, contract['ret_bytes'])
assert not blocks and not criticals
# The reached 867358 slot3 forwards to its real slot2 renderer. An empty
# primitive buffer must return zero without touching any RW engine interface.
put(p, 0x10007000)
put(p + 0xc, 0)
put(0x10007008, entries[0x738870])
assert call('offline_00668230', p) == 0
print(f'PASS: {len(contracts)} exact local contracts; original four-table virtual dispatch')
