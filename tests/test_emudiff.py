"""Offline emulator regressions. Run: .venv/bin/python tests/test_emudiff.py"""
import os
from pathlib import Path
import subprocess
import sys
import tempfile
import unittest
from unittest.mock import patch
from types import SimpleNamespace
import struct
import capstone

ROOT = Path(__file__).resolve().parents[1]
sys.path.insert(0, str(ROOT / "tools"))
import emudiff
import gate
from unicorn import UC_HOOK_CODE
from unicorn import x86_const as X


class StackProbeTests(unittest.TestCase):
    @classmethod
    def setUpClass(cls):
        cls.tmp = tempfile.TemporaryDirectory()
        cls.addClassCleanup(cls.tmp.cleanup)
        source = Path(cls.tmp.name) / "probes.cpp"
        source.write_text('''
extern "C" int __cdecl FixedProbe()
{
    char buffer[0x1400];
    buffer[0] = 3;
    buffer[sizeof(buffer) - 1] = 4;
    return buffer[0] + buffer[sizeof(buffer) - 1];
}
extern "C" int __cdecl DynamicProbe(unsigned size)
{
    char* buffer = static_cast<char*>(__builtin_alloca(size));
    buffer[0] = 3;
    buffer[size - 1] = 4;
    return buffer[0] + buffer[size - 1];
}
extern "C" int __cdecl ExternalValue();
extern "C" int __cdecl DirectExternal()
{
    return ExternalValue();
}
extern "C" int __cdecl IndirectExternal(unsigned target)
{
    return reinterpret_cast<int(__cdecl*)()>(target)();
}
''')
        cls.obj_path = os.path.join(cls.tmp.name, "probes.obj")
        subprocess.run(gate.compile_cmd() + [f"/Fo{cls.obj_path}", "-c", "--", str(source)],
                       cwd=ROOT / "build", check=True, capture_output=True, text=True)
        cls.helper = emudiff.crt_ranges()[1]["__alloca_probe"]

    def run_probe(self, name, size=None):
        obj = emudiff.Obj(self.obj_path, name)
        self.assertNotIn("__chkstk", obj.ext)
        run = emudiff.Run(0, 0, obj.code, obj)
        run.args = lambda: [] if size is None else [size]
        allocations = []
        run.uc.hook_add(UC_HOOK_CODE,
                        lambda uc, addr, n, data: allocations.append(uc.reg_read(X.UC_X86_REG_EAX)),
                        begin=self.helper, end=self.helper)
        run.start(obj.entry)
        self.assertTrue(allocations, "fixture must execute the real stack-probe helper")
        self.assertEqual(run.status, "ret")
        self.assertEqual(run.events, [], "stack probes must not shift the mock call sequence")
        self.assertEqual(run.eax, 7, "both ends of the allocated buffer must remain accessible")
        self.assertEqual(run.esp, emudiff.ESP0 + 4)
        self.assertTrue(run.kept, "the stack allocation must preserve the caller's saved registers")
        return allocations

    def test_fixed_frame(self):
        self.assertGreaterEqual(self.run_probe("FixedProbe")[0], 0x1400)

    def test_dynamic_alloca(self):
        for size in (32, 0xFF0, 0x1000, 0x1400, 0x2000, 0x3101):
            with self.subTest(size=size):
                self.assertGreaterEqual(self.run_probe("DynamicProbe", size)[0], size)

    def test_external_name_requires_direct_call(self):
        for name, direct in (("DirectExternal", True), ("IndirectExternal", False)):
            with self.subTest(name=name):
                obj = emudiff.Obj(self.obj_path, name)
                target = obj.ext["_ExternalValue"]
                run = emudiff.Run(0, 0, obj.code, obj)
                run.args = lambda: [target]
                run.start(obj.entry)
                self.assertEqual(run.status, "ret")
                self.assertEqual(len(run.events), 1)
                self.assertEqual(run.events[0][0], "_ExternalValue" if direct else target)

    def test_wrapped_mock_pointer_sees_identical_object_bytes(self):
        obj = emudiff.Obj(self.obj_path, 'FixedProbe')
        original = emudiff.Run(0, 2, [], memory_obj=obj)
        candidate = emudiff.Run(0, 2, obj.code, obj)
        # The indexed element is 3 dwords wide: the full byte scale is 12.
        address = (12 * 0x2C000101 + 9) & 0xFFFFFFFF
        self.assertEqual(address, 0x10000C15)
        for run in (original, candidate):
            run.on_unmapped(run.uc, 19, address, 4, 0, None)
        self.assertEqual(original.uc.mem_read(address, 4), candidate.uc.mem_read(address, 4))
        fill = emudiff.fill_page(address & ~0xFFF, 2)[address & 0xFFF:(address & 0xFFF) + 4]
        self.assertEqual(original.uc.mem_read(address, 4), fill)
        # Even a collision at the new base is symmetric, not merely unlikely.
        address = emudiff.OBJ_BASE + 0xC15
        self.assertLess(address + 4, obj.end)
        self.assertEqual(original.uc.mem_read(address, 4), candidate.uc.mem_read(address, 4))


class CleanupInferenceTests(unittest.TestCase):
    def infer(self, code, direct_pop=0):
        md = capstone.Cs(capstone.CS_ARCH_X86, capstone.CS_MODE_32)
        md.detail = True
        ins = {i.address: i for i in md.disasm(bytes.fromhex(code), 0x1000)}
        return ins, emudiff.infer_stack_pops(ins, lambda site, target: direct_pop)

    def indirect_counts(self, ins, counts):
        return [counts.get(pc) for pc, i in ins.items()
                if i.mnemonic == "call" and i.operands[0].type != capstone.x86.X86_OP_IMM]

    def test_zero_argument_call_does_not_pop_saved_register(self):
        # push ebp; mov ebp,esp; push esi; mov esi,ecx; push 7; call direct(ret 4);
        # mov ecx,esi; call [eax+28]; pop esi; pop ebp; ret 4.
        ins, counts = self.infer("55 8bec 56 8bf1 6a07 e800000000 8bce ff5028 5e 5d c20400", 4)
        self.assertEqual(self.indirect_counts(ins, counts), [0])

    def test_early_argument_belongs_to_later_direct_call(self):
        # Two pushed arguments, a virtual call, then a direct call that pops one:
        # the virtual must pop exactly one, leaving the early push for the direct call.
        ins, counts = self.infer("55 8bec 83ec1c 56 8bf1 6a01 6a02 ff508c e800000000 5e 8be5 5d c20400", 4)
        self.assertEqual(self.indirect_counts(ins, counts), [4])

    def test_conditional_argument_call(self):
        # A branch around push 0; call [eax+b4] must rejoin with the same stack.
        ins, counts = self.infer("55 8bec 56 8bf1 85c0 7405 6a00 ff50b4 5e 5d c3")
        self.assertEqual(self.indirect_counts(ins, counts), [4])

    def test_ambiguous_cleanup_keeps_export(self):
        # Either call could consume the early push; mov esp,ebp discards leftovers.
        ins, counts = self.infer("55 8bec 6a01 ff5028 ff502c 8be5 5d c3")
        self.assertEqual(self.indirect_counts(ins, counts), [None, None])

    def test_repeated_receiver_slot_relates_original_stack_constraints(self):
        ins, counts = self.infer('55 8bec 56 6a01 ff5028 6a02 ff5028 5e 5d c3')
        sites = [pc for pc, i in ins.items() if i.mnemonic == 'call']
        self.assertEqual(self.indirect_counts(ins, counts), [None, None])
        counts = emudiff.infer_stack_pops(ins, lambda pc, target: 0,
                                         groups={pc: ('receiver', 0x28) for pc in sites})
        self.assertEqual(self.indirect_counts(ins, counts), [4, 4])

    def test_by_value_outgoing_reservation_after_unknown_call(self):
        # Unknown call must pop its argument. The later SUB/ESP alias is a fully
        # initialized 16-byte by-value argument, not a second local frame.
        ins, counts = self.infer('55 8bec 56 6a01 ff5028 83ec10 8bfc '
                                 '8907 894704 894708 89470c 6a00 e800000000 5e 5d c3', 20)
        self.assertEqual(self.indirect_counts(ins, counts), [4])
        _, counts = self.infer('55 8bec 56 6a01 ff5028 83ec10 8bfc '
                              '8907 894704 894708 6a00 e800000000 5e 5d c3', 20)
        self.assertEqual(counts, {})

    def test_binary_getter_leaves_argument_for_later_setter(self):
        facts = emudiff.resolved_call_facts(0x54A910)
        self.assertEqual(facts[0x54AA98]['pop'], 0)
        self.assertEqual(facts[0x54AAB1]['pop'], 8)
        self.assertEqual(emudiff.call_pops()[0x54AA98], 4)  # export remains unchanged

    def test_receiver_resolves_balanced_indirect_tail(self):
        self.assertIsNone(emudiff.dispatch_call_pop(0x4F5170))
        self.assertEqual(emudiff.dispatch_call_pop(0x4F5170, '0083f1a0'), 4)

    def test_unknown_vtable_ret_does_not_narrow_cleanup(self):
        receiver = ('load', ('input', 'ecx'), 0)
        ev = {0x1000: dict(group=(receiver, 0), virtual=True, slot=0)}
        with patch.object(emudiff, 'load', return_value=[dict(addr='00009000', slots=['00002000'])]), \
                patch.object(emudiff, 'binary_call_pop', return_value=None):
            self.assertEqual(emudiff.virtual_pop_domains(ev), ({}, {}))

    def test_incomplete_vtable_catalog_cannot_erase_caller_proof(self):
        ea = 0x438880
        evidence = emudiff.dispatch_evidence(ea)
        false_domains = {ev['group']: {0} for ev in evidence.values() if 'group' in ev}
        emudiff.stack_call_pops.cache_clear()
        with patch.object(emudiff, 'virtual_pop_domains', return_value=(false_domains, {})):
            self.assertEqual(emudiff.stack_call_pops(ea)[0x43895A], 12)
        emudiff.stack_call_pops.cache_clear()

    def test_ret_evidence_has_priority_over_legacy_solver(self):
        emudiff.resolved_call_facts.cache_clear()
        with patch.object(emudiff, 'indirect_facts', return_value={0x1000: dict(pop=12)}), \
                patch.object(emudiff, 'stack_call_pops', return_value={0x1000: 8}), \
                patch.object(emudiff, 'dispatch_evidence', return_value={0x1000: dict(target=0x2000)}), \
                patch.object(emudiff, 'binary_call_pop', return_value=4):
            self.assertEqual(emudiff.resolved_call_facts(0x1000)[0x1000]['pop'], 4)
            self.assertEqual(len(emudiff.resolved_call_facts(0x1000)[0x1000]['disagreements']), 2)
        emudiff.resolved_call_facts.cache_clear()

    def test_dynamic_frame_keeps_export(self):
        ins, counts = self.infer("55 8bec 83e4f0 ff5028 8be5 5d c3")
        self.assertEqual(counts, {})

    def test_leave_restores_ebp_and_import_cleanup_is_general(self):
        ins, counts = self.infer('55 8bec 83ec20 56 57 6a01 6a02 6a03 6a04 ff1500200000 5f 5e c9 c20800')
        self.assertEqual(self.indirect_counts(ins, counts), [16])
        # The undecorated import is inferred from its caller, not its name.
        self.assertEqual(emudiff.import_call_pop(0x7AF69A, 'UNKNOWN.dll!UnknownImport'), 16)
        self.assertEqual(emudiff.import_call_pop(0, 'UNKNOWN.dll!_Other@20'), 20)

    def test_throw_exception_mock_preserves_saved_registers(self):
        ea = 0x7AF666
        own, ret_site = emudiff.own_ranges(ea)
        run = emudiff.Run(ea, 2, own)
        run.ret_site = ret_site
        run.args = lambda: [0, 0]
        run.start(ea)
        self.assertEqual(run.status, 'ret')
        self.assertTrue(run.kept)
        self.assertEqual(run.esp, emudiff.ESP0 + 12)
        self.assertEqual(run.events[0][1:3], (16, 4))


class ComparisonRuleTests(unittest.TestCase):
    def memory_run(self, word, floating=False):
        address = 0x2A000150
        data = struct.pack('<I', word)
        uc = SimpleNamespace(mem_read=lambda a, n: data[a-address:a-address+n])
        return SimpleNamespace(events=[], status='ret', esp=emudiff.ESP0 + 4,
                               tail=False, kept=True, eax=0, st0=None, last_mock=None,
                               blocks=1, uc=uc, prof=0, written=set(range(address,address+4)),
                               float_written=set(range(address,address+4)) if floating else set(),
                               norm=lambda x: x)

    def test_integer_stores_are_not_float_rounding(self):
        with patch.object(emudiff, 'proto', return_value=(False, [], 'void', 0)):
            for word in (1, 3, 6):
                self.assertTrue(emudiff.compare(0, self.memory_run(0), self.memory_run(word)))
            # Identical bit deltas are tolerated only for original FP stores.
            self.assertTrue(emudiff.compare(0, self.memory_run(0x3F800000), self.memory_run(0x3F800001)))
            self.assertEqual(emudiff.compare(0, self.memory_run(0x3F800000, True), self.memory_run(0x3F800001)), [])
            with patch.object(emudiff, 'fill_word', return_value=6):
                result = emudiff.compare(0, self.memory_run(0), self.memory_run(6))
                self.assertTrue(result)
                self.assertFalse(result[0].startswith('?'))
            self.assertTrue(emudiff.compare(0, self.memory_run(0xFFFFFFFF), self.memory_run(0x7FC00000)))

    def test_float_store_instruction_classification_and_overwrite(self):
        run = emudiff.Run(0, 0, [])
        pc = emudiff.SYS_BASE + 0x300
        for code, floating in (('d91f', True), ('db1f', False),
                               ('f30f1107', True), ('f20f1107', True), ('a5', False)):
            run.uc.mem_write(pc, bytes.fromhex(code) + b'\x90' * 15)
            run._float_store_cache = {}
            self.assertEqual(run.is_float_store(pc), floating)
            run.uc.reg_write(X.UC_X86_REG_EIP, pc)
            run.on_write(run.uc, 0, 0x2A000150, 4, 0, None)
            self.assertEqual(bool(run.float_written), floating)

    def test_unused_return_is_not_undefined(self):
        import api
        emudiff.eax_meaningless.cache_clear()
        with patch.object(api, 'ret_unused', return_value=True), \
             patch.object(api, 'eax_undefined_on_return', return_value=False), \
             patch.object(emudiff, 'load', return_value=object()):
            self.assertFalse(emudiff.eax_meaningless(0))
        emudiff.eax_meaningless.cache_clear()

    def test_literal_cc_argument_is_not_stack_garbage(self):
        eo = ('callee', 4, 1, False, 'int', (0xCCCCCCCC,), None, 0x401000)
        em = ('callee', 4, 1, False, 'int', (1,), None, None)
        with patch.object(emudiff, 'literal_pushes', return_value=frozenset({0})):
            self.assertFalse(emudiff.same_args(eo, em))
        with patch.object(emudiff, 'literal_pushes', return_value=frozenset()):
            self.assertTrue(emudiff.same_args(eo, em))

    def test_literal_pushes_read_original_instructions(self):
        emudiff.literal_pushes.cache_clear()
        # push 0xCCCCCCCC; push eax; push 0; call ...
        code = bytes.fromhex('68cccccccc 50 6a00 e800000000')
        with patch.object(emudiff, 'load', return_value=[emudiff.IMG]), \
             patch.object(emudiff, 'exe_image', return_value=(code, [])):
            self.assertEqual(emudiff.literal_pushes(emudiff.IMG + 8), frozenset({0, 2}))
        emudiff.literal_pushes.cache_clear()


if __name__ == "__main__":
    unittest.main()
