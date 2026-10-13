"""Offline binary ABI regressions: .venv/bin/python tests/test_abi.py"""
from pathlib import Path
from types import SimpleNamespace
import sys
import unittest
from unittest.mock import patch

sys.path.insert(0, str(Path(__file__).resolve().parents[1] / "tools"))
import api
import gate
import packet
import stackabi
import regargs
import capstone


def instructions(ops):
    return [SimpleNamespace(address=0x1000 + k, size=1, mnemonic=m, op_str=s)
            for k, (m, s) in enumerate(ops)]


class BinaryAbiTests(unittest.TestCase):
    @classmethod
    def setUpClass(cls):
        cls.db = packet.DB()

    def test_crt_constant_register_initialization(self):
        self.assertFalse(api.takes_reg(0x7B2E90, self.db, "ecx"))
        self.assertEqual(api.abi_sig(0x7B2E90, self.db)["conv"], "__cdecl")

    def test_stale_hexrays_ecx_location_is_not_an_input(self):
        self.assertEqual(regargs.entry_register_reads(self.db, 0x698820, 'ecx'), 0)
        self.assertFalse(api.takes_reg(0x698820, self.db, 'ecx'))
        # The stale ECX is disproved, but the EDX proof is inconclusive: do not
        # invent a standard signature for the remaining EDX-only export.
        self.assertIsNone(api.abi_sig(0x698820, self.db))
        decl = 'CSF_FUNC(char, __cdecl, Shutdown, 0x00698820, ());'
        self.assertEqual(api.binding_errors(decl, self.db), [])

    def test_spurious_volatile_locations_removed_from_generated_prototype(self):
        sig = api.abi_sig(0x465060, self.db)
        self.assertEqual(sig['conv'], '__stdcall')
        self.assertEqual(sig['params'], 'int a3, int a4, int a5')

    def register_proof(self, code, callee=None):
        md = capstone.Cs(capstone.CS_ARCH_X86, capstone.CS_MODE_32)
        ins = list(md.disasm(bytes.fromhex(code), 0x1000))
        codes = {0x1000: ins}
        if callee is not None:
            codes[0x2000] = list(md.disasm(bytes.fromhex(callee), 0x2000))
        db = SimpleNamespace(F={ea: dict(size=sum(i.size for i in ii)) for ea, ii in codes.items()},
                             _import_slots=set())
        with patch.object(packet, 'disasm', side_effect=lambda ea, size: codes[ea]), \
                patch.object(packet, 'callable_shared_chunk', return_value=None):
            return regargs.entry_register_reads(db, 0x1000, 'ecx')

    def test_register_proof_keeps_partial_pointer_and_conditional_inputs(self):
        # MOV CL,AL; MOV EAX,[ECX]: incoming upper pointer bits remain live.
        self.assertEqual(self.register_proof('8ac8 8b01 c3'), 0xffffff00)
        # One branch reads ECX, the other kills it: still a genuine conditional input.
        self.assertEqual(self.register_proof('85c0 7404 8bc1 c3 90 31c9 c3'), 0xffffffff)
        self.assertEqual(self.register_proof('8ac8 84c9 31c9 c3'), 0)

    def test_register_proof_distinguishes_preservation_and_unknown_dispatch(self):
        self.assertEqual(self.register_proof('e8fb0f0000 c3', 'c3'), 0)
        # A real input can first be read AFTER an original callee preserving ECX.
        self.assertEqual(self.register_proof('e8fb0f0000 8b01 c3', 'c3'), 0xffffffff)
        self.assertIsNone(self.register_proof('ff1500900000 c3'))

    def test_unreachable_dispatch_heads_do_not_supply_receiver_facts(self):
        md = capstone.Cs(capstone.CS_ARCH_X86, capstone.CS_MODE_32)
        ins = list(md.disasm(bytes.fromhex('eb05 8b01 ff10 c3 c3'), 0x1000))
        db = SimpleNamespace(F={0x1000: dict(size=8)}, classes={}, class_names={}, class_of=lambda ea: None)
        with patch.object(packet, 'disasm', return_value=ins):
            self.assertEqual(stackabi.call_evidence(0x1000, db), {})

    def test_image_global_callback_target_not_ida_delta(self):
        md = capstone.Cs(capstone.CS_ARCH_X86, capstone.CS_MODE_32)
        ins = list(md.disasm(bytes.fromhex('ff1500900000 c3'), 0x1000))
        db = SimpleNamespace(F={0x1000: dict(size=7), 0x2000: dict(size=1)},
                             classes={}, class_names={}, class_of=lambda ea: None)
        import semcheck
        with patch.object(packet, 'disasm', return_value=ins), \
                patch.object(semcheck, 'image_dword', return_value=0x2000):
            self.assertEqual(stackabi.call_evidence(0x1000, db), {0x1000: dict(target=0x2000)})

    def test_known_class_vtable_target(self):
        fact = stackabi.call_evidence(0x6F5510, self.db)[0x6F5529]
        self.assertEqual(fact['target'], 0x6F57D0)
        self.assertEqual(gate.effective_rets(fact['target'], self.db), [12])

    def test_static_class_with_incompatible_override_is_not_exact_target(self):
        md = capstone.Cs(capstone.CS_ARCH_X86, capstone.CS_MODE_32)
        codes = {0x1000: list(md.disasm(bytes.fromhex('8b01 ff10 c3'), 0x1000)),
                 0x2000: list(md.disasm(bytes.fromhex('c3'), 0x2000)),
                 0x3000: list(md.disasm(bytes.fromhex('c20400'), 0x3000))}
        db = SimpleNamespace(F={ea: dict(ea=ea, size=sum(i.size for i in ii), end=ea+16)
                                for ea, ii in codes.items()},
                             classes={'00009000': dict(base=None), '00009010': dict(base='00009000')},
                             class_names={}, class_of=lambda ea: '00009000' if ea == 0x1000 else None,
                             _dispatch_metadata=({}, {}, {0x9000: ['00002000'], 0x9010: ['00003000']}))
        with patch.object(packet, 'disasm', side_effect=lambda ea, size: codes[ea]):
            fact = stackabi.call_evidence(0x1000, db)[0x1002]
        self.assertNotIn('target', fact)
        self.assertEqual(fact['tables'], (0x9000, 0x9010))

    def test_binary_constructor_fields_override_propagated_base_type(self):
        # Factory -> concrete constructor -> field getter. The propagated base
        # field says C_00833908, whose slot +18 is an unrelated RET 0 stub.
        # Original constructor stores instead resolve the concrete backend.
        facts = stackabi.call_evidence(0x4FCB90, self.db)
        self.assertEqual(facts[0x4FCC2E]['receiver'], '0083f1a0')
        self.assertEqual(facts[0x4FCCF1]['target'], 0x6FC220)
        self.assertEqual(gate.effective_rets(facts[0x4FCCF1]['target'], self.db), [4])

    def test_typed_partial_register_argument(self):
        self.assertFalse(api.takes_reg(0x7FEFA0, self.db, "ecx"))
        self.assertEqual(api.abi_sig(0x7FEFA0, self.db)["conv"], "__cdecl")

    def scan(self, ops, abi=None):
        ins = instructions(ops)
        db = SimpleNamespace(F={0x1000: dict(size=len(ins))})
        with patch.object(packet, "disasm", return_value=ins), patch.object(api, "ida_abi", return_value=abi or {}):
            return api.reads_before_write(0x1000, db, "ecx")

    def test_partial_write_does_not_erase_live_pointer(self):
        self.assertTrue(self.scan([("mov", "cx, word ptr [esp + 4]"), ("mov", "eax, dword ptr [ecx]"), ("ret", "")]))

    def test_partial_write_and_typed_push(self):
        ops = [("mov", "cx, word ptr [esp + 4]"), ("push", "ecx"), ("call", "0x2000"), ("ret", "")]
        self.assertFalse(self.scan(ops, {0x2000: dict(args=[("unsigned __int16", "a", "stack")])}))
        self.assertTrue(self.scan(ops, {0x2000: dict(args=[("int", "a", "stack")])}))
        self.assertTrue(self.scan(ops))

    def test_constant_and_nonconstant_or(self):
        self.assertFalse(self.scan([("or", "ecx, 0xffffffff"), ("push", "ecx"), ("ret", "")]))
        self.assertTrue(self.scan([("or", "ecx, 0xfffffffe"), ("ret", "")]))

    def test_callable_shared_chunks_and_invalid_labels(self):
        for ea, size in ((0x42B320, 74), (0x661DC0, 8)):
            self.assertEqual(packet.callable_shared_chunk(ea, self.db)["size"], size)
            self.assertTrue(api.takes_reg(ea, self.db, "ecx"))
            self.assertEqual(gate.effective_rets(ea, self.db), [0])
        self.assertIsNone(packet.callable_shared_chunk(0x661DC2, self.db))
        self.assertIsNone(packet.callable_shared_chunk(0x42B338, self.db))
        decl = "CSF_FUNC(int, __fastcall, F, 0x00661DC0, (void* self, void* edx));"
        self.assertEqual(gate.check_bindings(decl, self.db), [])
        self.assertTrue(gate.check_bindings(decl.replace("void* edx", "void* edx, int arg"), self.db))
        self.assertTrue(api.binding_errors(decl.replace("__fastcall", "__cdecl"), self.db))

    def test_shared_vcall_pop_proof(self):
        ins = packet.disasm(0x661DC0, 8, linear=True)
        self.assertEqual(packet.vcall_tail_pops(ins), {0x661DC4: 4})
        self.assertEqual(packet.vcall_tail_pops(instructions([
            ("mov", "eax, dword ptr [ecx]"), ("push", "eax"),
            ("call", "dword ptr [eax + 8]"), ("ret", "")])), {})

    def test_indirect_pops_from_stack_restoration(self):
        self.assertEqual(stackabi.indirect_facts(0x537130, self.db), {0x53717B: dict(pop=4, nargs=1)})
        facts = stackabi.indirect_facts(0x708AB0, self.db)
        self.assertEqual(facts[0x708CF6], dict(pop=12, nargs=3))
        for pc in (0x708C18, 0x708C24, 0x708CBA, 0x708CC8):
            self.assertEqual(facts[pc], dict(pop=0, nargs=2, ecx=False))
        facts = stackabi.indirect_facts(0x425EA0, self.db)
        self.assertEqual(facts[0x42606C], dict(pop=4, nargs=1))
        self.assertEqual(facts[0x425EE3], dict(pop=0))
        self.assertEqual(stackabi.indirect_facts(0x69FA20, self.db), {0x69FA35: dict(pop=0, nargs=4)})

    def test_shared_entry_keeps_custom_register_checks(self):
        ins = instructions([("mov", "edi, eax"), ("ret", "")])
        db = SimpleNamespace(F={}, rows={})
        with patch.object(packet, "callable_shared_chunk", return_value=dict(size=len(ins))), \
                patch.object(packet, "disasm", return_value=ins):
            self.assertEqual(api.real_regs(0x1000, db), ["eax"])

    def derive(self, ops):
        ins = instructions(ops)
        db = SimpleNamespace(F={0x1000: dict(size=len(ins))})
        with patch.object(packet, "disasm", return_value=ins), patch.object(packet, "ida_view", return_value=None):
            return stackabi.indirect_facts(0x1000, db)

    def test_underdetermined_calls_stay_unknown(self):
        self.assertEqual(self.derive([
            ("push", "1"), ("call", "dword ptr [eax]"),
            ("push", "2"), ("call", "dword ptr [edx]"), ("ret", "")]), {})

    def test_unsupported_stack_assignment_stays_unknown(self):
        self.assertEqual(self.derive([
            ("push", "1"), ("call", "dword ptr [eax]"), ("mov", "esp, eax"), ("ret", "")]), {})

    def test_reserved_outgoing_slots_do_not_imply_zero_pops(self):
        self.assertEqual(self.derive([
            ("push", "ebp"), ("mov", "ebp, esp"), ("sub", "esp, 4"),
            ("mov", "dword ptr [esp], 1"), ("call", "dword ptr [eax]"),
            ("mov", "esp, ebp"), ("pop", "ebp"), ("ret", "")]), {})


if __name__ == "__main__":
    unittest.main(warnings="ignore")
