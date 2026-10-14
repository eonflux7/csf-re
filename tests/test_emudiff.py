"""Offline emulator regressions. Run: .venv/bin/python tests/test_emudiff.py"""
import os
import io
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
extern "C" int __cdecl atexit(void (__cdecl*)());
extern "C" int __cdecl RegisterCallback(unsigned callback)
{
    return atexit(reinterpret_cast<void (__cdecl*)()>(callback));
}
extern "C" void __cdecl RegisterSelf()
{
    atexit(RegisterSelf);
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

    def test_atexit_is_an_exact_callback_event_not_crt_startup(self):
        target = emudiff.crt_ranges()[1]['_atexit']
        obj = emudiff.Obj(self.obj_path, 'RegisterCallback')
        runs = []
        for callback in (0x82A5C0, 0x82A5E0):
            run = emudiff.Run(0, 0, obj.code, obj)
            run.args = lambda callback=callback: [callback]
            run.start(obj.entry)
            self.assertEqual(run.status, 'ret')
            self.assertEqual(run.esp, emudiff.ESP0 + 4)
            self.assertTrue(run.kept)
            self.assertEqual(run.written, set())
            self.assertEqual(len(run.events), 1)
            self.assertEqual(run.events[0][:4], (target, 0, 1, False))
            self.assertEqual(run.events[0][5], (callback,))
            runs.append(run)
        self.assertFalse(emudiff.same_args(runs[0].events[0], runs[1].events[0]))
        # Callback pointers within the candidate's own image retain the
        # existing original-address normalization, rather than comparing
        # its relocated entry against the original's numeric address.
        obj = emudiff.Obj(self.obj_path, 'RegisterSelf')
        run = emudiff.Run(0x401000, 0, obj.code, obj).start(obj.entry)
        self.assertEqual(run.status, 'ret')
        self.assertEqual(run.events[0][5], (0x401000,))

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

    def test_noncontinuable_exception_ends_at_boundary(self):
        ea = 0x7AF666
        own, ret_site = emudiff.own_ranges(ea)
        run = emudiff.Run(ea, 2, own)
        run.ret_site = ret_site
        run.args = lambda: [0, 0]
        run.start(ea)
        self.assertEqual(run.status, 'noreturn')
        self.assertEqual(len(run.events), 1)
        self.assertEqual(run.events[0][0], 'KERNEL32.dll!RaiseException')
        self.assertEqual(run.events[0][1:3], (16, 4))
        self.assertTrue(emudiff.no_return('KERNEL32.dll!RaiseException', (0, 1, 0, 0)))
        self.assertFalse(emudiff.no_return('KERNEL32.dll!RaiseException', (0, 0, 0, 0)))

    def test_mixed_named_callback_conventions_are_paired_by_global_and_slot(self):
        facts = emudiff.global_callback_types(0x738070)
        self.assertEqual(facts[0x7380A5], dict(ecx=False, nargs=2, pop=0))
        self.assertNotIn(0x738085, facts)  # actual virtual call, not RW callback
        self.assertFalse(emudiff.callsite_abi(0x738070, 0x7380A5))
        self.assertIsNone(emudiff.callsite_abi(0x738070, 0x738085))

    def test_named_callback_load_cannot_cross_join_or_partial_register_write(self):
        md = capstone.Cs(capstone.CS_ARCH_X86, capstone.CS_MODE_32)
        md.detail = True
        source = '(*((int (__cdecl **)(int, int))table + 8))(1, 2);'
        # A branch can bypass the global load, or AL can change the loaded
        # pointer. Neither supports pairing the call with the named table.
        for code, expected in (('a100900000 ff5020 c3', True),
                               ('85d2 7405 a100900000 ff5020 c3', False),
                               ('ffe2 a100900000 ff5020 c3', False),
                               ('a100900000 b000 ff5020 c3', False)):
            with self.subTest(code=code):
                ins = {i.address: i for i in md.disasm(bytes.fromhex(code), 0x1000)}
                emudiff.global_callback_types.cache_clear()
                with patch.object(emudiff, 'original_stack_code', return_value=ins), \
                        patch.object(emudiff, 'binary_switches', return_value={}), \
                        patch.object(emudiff.os.path, 'exists', return_value=True), \
                        patch('builtins.open', return_value=io.StringIO(source)), \
                        patch.object(emudiff, 'load', return_value={'table': 0x9000}):
                    self.assertEqual(bool(emudiff.global_callback_types(0x1000)), expected)
        emudiff.global_callback_types.cache_clear()

    def test_complete_float_dispatch_casts_supply_st0_without_exemptions(self):
        sites = emudiff.indirect_float_returns(0x7377F0)
        self.assertEqual(len(sites), 8)
        self.assertEqual(set(sites.values()), {'double'})
        self.assertEqual(emudiff.indirect_float_returns(0x738070), {})  # mixed calls
        own, ret_site = emudiff.own_ranges(0x7377F0)
        run = emudiff.Run(0x7377F0, 2, own)
        run.ret_site = ret_site
        run.start(0x7377F0)
        self.assertEqual(run.status, 'ret')
        calls = [event for event in run.events if event[7] in sites]
        self.assertEqual(len(calls), 8)
        self.assertTrue(all(event[4] == 'double' for event in calls))

    def test_bounded_switch_cases_preserve_caller_cleanup_proof(self):
        self.assertEqual(emudiff.binary_switches(0x6E3A70)[0x6E3B17],
                         {0x6E3B1E, 0x6E3B2D, 0x6E3B3B, 0x6E3B49, 0x6E3B55})
        pops = emudiff.stack_call_pops(0x6E3A70, constraints_only=True)
        for site in emudiff.global_callback_types(0x6E3A70):
            self.assertEqual(pops[site], 0)
        # An unbounded computed jump or an edge outside the decoded function
        # must not yield a proof from just the supported prefix.
        ins, _ = self.infer('55 8bec 6a01 ff5028 ff24c500200000')
        self.assertEqual(emudiff.infer_stack_pops(ins, lambda pc, t: 0), {})
        jump = next(pc for pc, i in ins.items() if i.mnemonic == 'jmp')
        self.assertEqual(emudiff.infer_stack_pops(ins, lambda pc, t: 0,
                                                switches={jump: {0x9999}}), {})

    def test_switch_bound_must_not_be_bypassed_by_an_incoming_edge(self):
        md = capstone.Cs(capstone.CS_ARCH_X86, capstone.CS_MODE_32)
        md.detail = True
        ea = emudiff.IMG + 0x1000
        for prefix, reenter, expected in (('', False, True),
                                          ('85d2 7405', False, False),
                                          ('', True, False)):
            with self.subTest(prefix=prefix, reenter=reenter):
                # cmp eax,1; ja default; jmp [eax*4+table]; default: ret
                raw = bytes.fromhex(prefix + ' 83f801 7707 ff248500204000 c3')
                ins = {i.address: i for i in md.disasm(raw, ea)}
                jump = next(pc for pc, i in ins.items() if i.mnemonic == 'jmp')
                target = jump if reenter else ea + len(raw) - 1
                img = bytearray(0x3000)
                struct.pack_into('<II', img, 0x2000, target, target)
                emudiff.binary_switches.cache_clear()
                with patch.object(emudiff, 'original_stack_code', return_value=ins), \
                        patch.object(emudiff, 'exe_image', return_value=(img, [])):
                    self.assertEqual(bool(emudiff.binary_switches(ea)), expected)
        emudiff.binary_switches.cache_clear()

    def test_late_register_saves_are_not_virtual_arguments(self):
        import emuprofiles
        for ea, site in ((0x7215F0, 0x721624), (0x721900, 0x721934)):
            graph = emuprofiles.build(emudiff, ea)
            own, ret_site = emudiff.own_ranges(ea)
            run = emudiff.Run(ea, 2, own, graph=graph)
            run.ret_site = ret_site
            run.start(ea)
            self.assertEqual(run.status, 'ret')
            event = next(event for event in run.events if event[7] == site)
            self.assertEqual(event[1:3], (4, 1))
            self.assertNotIn(emudiff.ZONES + 0x0A200100, event[5])
        # PUSH EDI at 6F11A4 is another late save, before a zero-arg getter.
        graph = emuprofiles.build(emudiff, 0x6F1170)
        own, ret_site = emudiff.own_ranges(0x6F1170)
        run = emudiff.Run(0x6F1170, 2, own, graph=graph)
        run.ret_site = ret_site
        run.start(0x6F1170)
        event = next(event for event in run.events if event[7] == 0x6F11A7)
        self.assertEqual(event[1:3], (0, 0))

    def test_nil_iterator_exception_profile_stops_before_tree_erasure(self):
        import emuprofiles
        ea = 0x6B7670
        graph = emuprofiles.build(emudiff, ea)
        own, ret_site = emudiff.own_ranges(ea)
        run = emudiff.Run(ea, 2, own, graph=graph)
        run.ret_site = ret_site
        run.start(ea)
        self.assertEqual(run.status, 'noreturn')
        self.assertEqual(run.events[-1][0], 0x7AF666)
        self.assertNotIn(0x690E60, [event[0] for event in run.events])  # no free after throw


class GraphProfileTests(unittest.TestCase):
    def probe(self, hexcode):
        import emuprofiles
        pc = emudiff.SYS_BASE + 0x300
        code = bytes.fromhex(hexcode)
        md = capstone.Cs(capstone.CS_ARCH_X86, capstone.CS_MODE_32)
        md.detail = True
        instructions = {i.address: i for i in md.disasm(code, pc)}
        cls = emudiff.Run
        def make(*args, **kwargs):
            run = cls(*args, **kwargs)
            run.uc.mem_write(pc, code)
            return run
        make.made = cls.made
        graph = emuprofiles.Graph()
        graph.put(emudiff.ECX0 + 4, 0)
        with patch.object(emudiff, 'Run', make), \
                patch.object(emudiff, 'original_stack_code', return_value=instructions):
            original = emuprofiles.discover(emudiff, pc, 2, [(pc, pc + len(code))], {}, None, graph)
        return original, graph, make, pc, code

    def test_recursive_fault_fields_are_seeded_then_mirrored(self):
        # mov eax,[ecx+4]; mov eax,[eax+8]; mov dword [eax],77; ret
        original, graph, make, pc, code = self.probe('8b4104 8b4008 c7004d000000 c3')
        self.assertEqual(original.status, 'ret')
        self.assertEqual(len(graph.evidence), 2)
        child = graph.words[emudiff.ECX0 + 4]
        leaf = graph.words[child + 8]
        candidate = make(pc, 2, [(pc, pc + len(code))], graph=graph).start(pc)
        self.assertEqual(candidate.status, 'ret')
        self.assertEqual(original.uc.mem_read(leaf, 4), candidate.uc.mem_read(leaf, 4))
        with patch.object(emudiff, 'proto', return_value=(False, [], 'void', 0)):
            self.assertEqual(emudiff.compare(pc, original, candidate), [])
            candidate.uc.mem_write(leaf, struct.pack('<I', 78))
            self.assertTrue(emudiff.compare(pc, original, candidate))

    def test_discovery_cannot_repair_a_store_from_the_original(self):
        # A function-generated null is a failure, not a repairable input.
        original, graph, _, _, _ = self.probe('c7410400000000 8b4104 8b4008 c3')
        self.assertTrue(original.status.startswith('fault'))
        self.assertEqual(graph.evidence, [])

    def test_partial_register_write_invalidates_pointer_load_provenance(self):
        # mov eax,[ecx+4]; mov al,4; mov eax,[eax+8]; ret
        # The dereference uses a computed value, not a copied input pointer.
        original, graph, _, _, _ = self.probe('8b4104 b004 8b4008 c3')
        self.assertTrue(original.status.startswith('fault'))
        self.assertEqual(graph.evidence, [])


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

    def test_noreturn_compares_prefix_not_return_frame(self):
        original, candidate = self.memory_run(1), self.memory_run(1)
        original.status = candidate.status = 'noreturn'
        candidate.esp += 32
        candidate.eax = 77
        candidate.kept = False
        with patch.object(emudiff, 'proto', return_value=(False, [], 'int', 0)):
            self.assertEqual(emudiff.compare(0, original, candidate), [])
            self.assertTrue(emudiff.compare(0, original, self.memory_run(2)))
            changed = self.memory_run(2)
            changed.status = 'noreturn'
            self.assertTrue(emudiff.compare(0, original, changed))

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


class FalseAlarmRegressionTests(unittest.TestCase):
    def test_resolved_indirect_target_ret_overrides_export_and_stack_facts(self):
        # CALL EAX, not a direct relocation. Deliberately supply stale pop=0.
        target = emudiff.EXT_BASE + 0x100
        entry = emudiff.SYS_BASE + 0x600
        for pop in (4, 8, 12):
            code = b'\x6a\x07' * (pop // 4) + b'\xb8' + struct.pack('<I', target) + b'\xff\xd0\xc3'
            site = entry + len(code) - 3
            run = emudiff.Run(0, 0, [(entry, entry + len(code))])
            run.uc.mem_write(entry, code)
            run.ret_site = {site + 2: site}
            run.proven_pops = {site: dict(pop=0)}
            with patch.object(emudiff, 'dispatch_call_pop', return_value=pop), \
                 patch.object(emudiff, 'dispatch_evidence', return_value={}), \
                 patch.object(emudiff, 'callsite_abi', return_value=False), \
                 patch.object(emudiff, 'callback_return_type', return_value=None):
                run.start(entry)
            self.assertEqual(run.status, 'ret')
            self.assertEqual(run.esp, emudiff.ESP0 + 4)
            self.assertEqual(run.events[0][1:4], (pop, pop // 4, False))
            self.assertEqual(run.events[0][5], (7,) * (pop // 4))

    def test_output_fill_is_mirrored_and_not_a_comparison_exemption(self):
        original = emudiff.Run(0, 3, [])
        candidate = emudiff.Run(0, 3, [])
        addresses = [emudiff.ESP0 - 16, emudiff.ESP0 - 32]
        events = []
        for run, address, initial in zip((original, candidate), addresses, (b'\xcc' * 4, b'\0' * 4)):
            run.uc.mem_write(address, initial)
            run.uc.mem_write(emudiff.ESP0, struct.pack('<II', emudiff.RET_MAGIC, address))
            run.uc.reg_write(X.UC_X86_REG_ESP, emudiff.ESP0)
            run.uc.reg_write(X.UC_X86_REG_ECX, emudiff.ECX0)
            # Candidate normally replays the original ABI, as it does here.
            run.script = [(0x655F00, 4, 1, True, 'uint32_t*')]
            run.mock(0x655F00)
            self.assertNotEqual(bytes(run.uc.mem_read(address, 4)), initial)
            events.append(bytes(run.uc.mem_read(address, 4)))
        self.assertEqual(events[0], events[1])
        # A stale value from before the out-call is still observable.
        eo = ('consumer', 4, 1, False, 'int', (struct.unpack('<I', events[0])[0],), None)
        em = ('consumer', 4, 1, False, 'int', (0,), None)
        self.assertFalse(emudiff.same_args(eo, em))
        self.assertEqual(emudiff.mock_outputs(0x655F00 + 1, 0, 0, (), 0, 0), [])
        # The success-only coordinate writes must not happen on failure.
        self.assertEqual(emudiff.mock_outputs(0x4DDF80, 0, 0, (0, 0, 1, 2), 0, 0), [])

    def test_unproved_cleanup_is_unknown_not_an_exported_zero(self):
        run = emudiff.Run(0, 0, [])
        site = 0x20001000
        run.ret_site = {site + 2: site}
        run.proven_pops = {site: dict(pop=0, source='stack')}
        run.uc.reg_write(X.UC_X86_REG_ESP, emudiff.ESP0)
        run.uc.mem_write(emudiff.ESP0, struct.pack('<I', site + 2))
        with patch.object(emudiff, 'dispatch_call_pop', return_value=None), \
             patch.object(emudiff, 'dispatch_evidence', return_value={}), \
             patch.object(emudiff, 'stack_call_pops', return_value={}):
            run.mock(emudiff.EXT_BASE + 0x100)
        self.assertEqual(run.status, 'unknown call cleanup')
        self.assertEqual(run.events, [])

    def test_stack_only_conventions_and_unknown_convention_are_distinct(self):
        for ea, site in ((0x6A18C0, 0x6A18F2), (0x7111F0, 0x7111F6), (0x769010, 0x769021)):
            self.assertIs(emudiff.callsite_abi(ea, site), False)
        # A known thiscall still compares ECX, including a wrong-this mutant.
        original = ComparisonRuleTests().memory_run(0)
        candidate = ComparisonRuleTests().memory_run(0)
        original.events = [(0x6E5A80, 4, 1, True, 'int', (1,), 0x20001000)]
        candidate.events = [(0x6E5A80, 4, 1, True, 'int', (1,), 0x20002000)]
        with patch.object(emudiff, 'proto', return_value=(True, [], 'void', 0)), \
             patch.object(emudiff, 'takes_ecx', return_value=True):
            self.assertIn('ecx', emudiff.compare(0, original, candidate)[0])

    def test_byte_alias_and_instruction_typed_argument_keep_low_bits_exact(self):
        eo = (0x20001000, 4, 1, False, 'int', (0x2A000100,), None, 0x47FED3)
        em = (0x20001000, 4, 1, False, 'int', (0,), None, None)
        # Use the real site from the original, independent of source line numbering.
        from packet import disasm
        site = next(i.address for i in disasm(0x47FDF0, int(emudiff.functions()[0x47FDF0]['size']))
                    if i.mnemonic == 'call' and '0x1ec' in i.op_str)
        eo = eo[:7] + (site,)
        self.assertEqual(emudiff.named_vslot_types(0x47FDF0, site), ['bool'])
        self.assertTrue(emudiff.same_args(eo, em))
        self.assertFalse(emudiff.same_args(eo, em[:5] + ((1,),) + em[6:]))
        self.assertIn('std::uint8_t', emudiff.BYTE_TYPES)
        self.assertEqual(emudiff.callback_return_type(0x769010, 0x769055), 'char')
        original = ComparisonRuleTests().memory_run(0)
        candidate = ComparisonRuleTests().memory_run(0)
        original.eax, candidate.eax = 0x5A5A5A01, 1
        with patch.object(emudiff, 'proto', return_value=(False, [], 'std::uint8_t', 0)), \
             patch.object(emudiff, 'eax_meaningless', return_value=False):
            self.assertEqual(emudiff.compare(0, original, candidate), [])
            candidate.eax = 0
            self.assertTrue(emudiff.compare(0, original, candidate))
        # A byte callee's full EAX can itself be the caller's int return.
        # Do not skip that comparison just because AL was tested after CALL.
        original.eax, candidate.eax = 0x5A5A5A00, 0
        original.last_mock = (original.eax, 'char')
        with patch.object(emudiff, 'proto', return_value=(False, [], 'int', 0)), \
             patch.object(emudiff, 'eax_meaningless', return_value=False), \
             patch.object(emudiff, 'returns_al', return_value=False):
            self.assertTrue(emudiff.compare(0, original, candidate))

    def test_self_recursion_is_a_boundary_on_both_sides(self):
        ea = 0x401000
        original = emudiff.Run(0, 0, [])
        candidate = emudiff.Run(0, 0, [])
        for run, obj in ((original, None), (candidate, SimpleNamespace(entry=emudiff.OBJ_BASE))):
            run.ea, run.obj, run.first = ea, obj, False
            address = obj.entry if obj else ea
            run.own = [(address, address + 0x10)]
            run.on_block(run.uc, address, 1, None)
            self.assertEqual(run.pending, ea)
        # Ordinary internal basic blocks must not become extra call events.
        candidate.pending = None
        candidate.on_block(candidate.uc, candidate.obj.entry + 4, 1, None)
        self.assertIsNone(candidate.pending)

    def test_log2_native_shim_and_explicit_x87_startup(self):
        run = emudiff.Run(0, 0, [])
        self.assertEqual(run.uc.reg_read(X.UC_X86_REG_FPCW), 0x027F)
        image, _ = emudiff.exe_image()
        # Startup helper's PUSH mask / PUSH precision for __controlfp.
        self.assertEqual(image[0x7B388C-emudiff.IMG:0x7B3896-emudiff.IMG],
                         bytes.fromhex('6800000300 6800000100'))
        run.args = lambda: list(struct.unpack('<II', struct.pack('<d', 4.0)))
        run.start(emudiff.NATIVE_AT['_log2'])
        self.assertEqual(run.status, 'ret')
        self.assertEqual(run.events, [])
        # Unicorn's ST0 accessor exposes only the mantissa. FPn exposes both
        # mantissa/exponent; TOP selects the physical register containing ST0.
        top = (run.uc.reg_read(X.UC_X86_REG_FPSW) >> 11) & 7
        self.assertAlmostEqual(emudiff.st0_float(run.uc.reg_read(X.UC_X86_REG_FP0 + top)), 2.0)

    def test_frame_discard_cannot_validate_a_wrong_hidden_result_prototype(self):
        self.assertNotIn(0x4F4F0A, emudiff.stack_call_pops(0x4F4EF0, constraints_only=True))
        self.assertEqual(emudiff.stack_call_pops(0x42F4F0, constraints_only=True)[0x42F5D3], 4)
        self.assertEqual(emudiff.stack_call_pops(0x535F60, constraints_only=True)[0x535F7F], 8)
        self.assertEqual(emudiff.stack_call_pops(0x535F60, constraints_only=True)[0x535F86], 4)

    def test_synthetic_crt_dispatch_is_not_a_valid_function_return(self):
        run = emudiff.Run(0, 0, [])
        site = 0x20001000
        run.indirect_sites = {site}
        run.ret_site = {site + 2: site}
        run.uc.reg_write(X.UC_X86_REG_ESP, emudiff.ESP0)
        run.uc.mem_write(emudiff.ESP0, struct.pack('<I', site + 2))
        target = emudiff.crt_ranges()[1]['__ftol2']
        with patch.object(emudiff, 'dispatch_evidence', return_value={}):
            run.on_block(run.uc, target, 1, None)
        self.assertEqual(run.status, 'synthetic indirect CRT target')

    def test_cap_preceding_a_late_call_is_inconclusive_not_call_count_diff(self):
        original = ComparisonRuleTests().memory_run(0)
        candidate = ComparisonRuleTests().memory_run(0)
        original.blocks = 10000
        original.events = [('late call', 0, 0, False, 'int', (), None)]
        candidate.status = 'cap'
        self.assertTrue(emudiff.compare(0, original, candidate)[0].startswith('?'))

    def test_real_triage_rows_and_adopted_controls(self):
        # Fresh compilation is essential: installed build objects can be stale.
        broken = (0x710850, 0x711160, 0x769010)
        adopted = (0x42DA20, 0x42F4F0, 0x468500, 0x495250, 0x4E9200,
                   0x6B8E60, 0x744E00, 0x7450D0, 0x745490)
        for ea in broken + adopted:
            with self.subTest(ea=f'{ea:08x}'):
                verdict, detail = emudiff.check(ea, src=str(ROOT / 'src/auto' / f'{ea:08x}.cpp'))
                self.assertEqual(verdict, 'DIFF' if ea in broken else 'same', detail)


if __name__ == "__main__":
    unittest.main()
