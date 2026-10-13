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
