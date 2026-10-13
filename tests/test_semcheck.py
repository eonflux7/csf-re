"""Offline regressions for queue semantic checks.

Run: .venv/bin/python tests/test_semcheck.py
"""
from pathlib import Path
import re
import subprocess
import sys
import tempfile
import unittest

ROOT = Path(__file__).resolve().parents[1]
sys.path.insert(0, str(ROOT / "tools"))
import gate
import semcheck
from packet import DB


class SemanticQueueTests(unittest.TestCase):
    @classmethod
    def setUpClass(cls):
        cls.db = DB()
        cls.cc = gate.compile_cmd()

    def source(self, ea):
        p = ROOT / f"src/auto/{ea:08x}.cpp"
        if not p.exists():  # a queued function whose candidate is not in the tree (yet)
            self.skipTest(f"no source for {ea:08x}")
        return p.read_text()

    def errors(self, ea, source):
        with tempfile.TemporaryDirectory() as td:
            src, obj = Path(td) / "test.cpp", Path(td) / "test.obj"
            src.write_text(source)
            subprocess.run(self.cc + [f"/Fo{obj}", "-c", "--", str(src)],
                           cwd=ROOT / "build", check=True, capture_output=True)
            fn = re.search(r"HOOK(?:_REGS|_TAIL)?\(\s*0x\w+\s*,\s*(\w+)", source)[1]
            return (semcheck.check(ea, str(obj), self.db)
                    + semcheck.byte_return_of_bool(ea, str(obj), fn, self.db)
                    + semcheck.behaviour(ea, str(obj), fn, source, self.db))

    def assert_detected(self, ea, source, message):
        errs = self.errors(ea, source)
        self.assertTrue(any(message in e for e in errs), errs)

    def test_regs_parameter_is_not_hook_infrastructure(self):
        self.assertFalse(semcheck.infrastructure_symbol(
            "?InputDInput_SetBufferSize@@YAIPAURegs@hook@@PBI@Z"))
        self.assertTrue(semcheck.infrastructure_symbol(
            "?call_regs@hook@@YAIPAXPAURegs@1@PBIH@Z"))
        ea = 0x712F80
        src = self.source(ea)
        self.assertEqual(self.errors(ea, src), [])
        self.assert_detected(ea, src.replace("[0x18 / 4]", "[0x20 / 4]"), semcheck.VSLOT_ERR)

    def test_magic_division_requires_the_right_divisor(self):
        for ea in (0x534110, 0x534210):
            src = self.source(ea)
            self.assertEqual(self.errors(ea, src), [])
            self.assert_detected(ea, src.replace("/ 28", "/ 29"), semcheck.CONST_ERR)

    def test_folded_global_value_requires_the_same_global(self):
        ea = 0x532A40
        src = self.source(ea)
        self.assertEqual(self.errors(ea, src), [])
        self.assert_detected(ea, src.replace("0x0083EE90", "0x0083EE9C"), "original does not")

    def test_and_or_mask_still_detects_wrong_bits(self):
        ea = 0x4D3830
        src = self.source(ea)
        self.assertEqual(self.errors(ea, src), [])
        self.assert_detected(ea, src.replace("0xFFFFFF3Fu", "0xFFFFFFBFu"), semcheck.CONST_ERR)

    def test_fixed_copy_accepts_its_span_only(self):
        ea = 0x70F890
        src = self.source(ea)
        self.assertEqual(self.errors(ea, src), [])
        mutant = src.replace("&sound->field_002c", "reinterpret_cast<uint8_t*>(sound) + 0x200")
        self.assert_detected(ea, mutant, "field offsets")

    def test_negative_cursor_displacements(self):
        ea = 0x6E7000
        src = self.source(ea)
        self.assertEqual(self.errors(ea, src), [])
        self.assert_detected(ea, src.replace("record[3]", "record[30]"), "field offsets")

    def test_pointer_initializer_references_all_four_strings(self):
        ea = 0x6DBDB0
        src = self.source(ea)
        self.assertEqual(self.errors(ea, src), [])
        self.assert_detected(ea, src.replace("0x8613A8", "0x8613AC"), "original does not")

    def test_indexed_dispatch_preserves_the_table(self):
        ea = 0x6A0570
        src = self.source(ea)
        self.assertEqual(self.errors(ea, src), [])
        self.assert_detected(ea, src.replace("CSF_VTBL(0x0085ECE0)", "CSF_VTBL(0x0085ECE4)"), "never does")

    def test_internal_bool_does_not_widen_a_byte_return(self):
        ea = 0x702E20
        self.assertEqual(self.errors(ea, self.source(ea)), [])
        # The historical 0x47F9B0 control really returns zero-extended eax.
        fixture = '''#include "globals.h"
#include "hook.h"
static bool __fastcall WrongByteReturn(void* self, void*)
{ return CSF_AT(uint32_t, self, 0xC) == 0; }
HOOK(0x0047F9B0, WrongByteReturn, "auto");
'''
        self.assert_detected(0x47F9B0, fixture, "BOOL")

    def test_adjustor_thunk_ignores_unreachable_sibling_only(self):
        ea = 0x537270
        src = self.source(ea)
        self.assertEqual(gate.effective_rets(ea, self.db), [4])
        self.assertEqual(self.errors(ea, src), [])
        mutant = src.replace("    C_00843720_ReleaseSubobjects(object, nullptr);", "")
        self.assert_detected(ea, mutant, "call to 0x537130")
        mutant = src.replace("        CoreMemory_Free(object);", "        (void)object;")
        self.assert_detected(ea, mutant, "call to 0x691110")


if __name__ == "__main__":
    unittest.main()
