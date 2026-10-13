"""Offline diff-harness regressions. Run: .venv/bin/python tests/test_hookdiff.py"""
from pathlib import Path
import subprocess
import sys
import tempfile
import unittest
from unittest.mock import patch

ROOT = Path(__file__).resolve().parents[1]
sys.path.insert(0, str(ROOT / "tools"))
import difftable


class ArgumentWidthTests(unittest.TestCase):
    def test_scalar_types(self):
        for ty in ("char", "signed char", "unsigned __int8", "bool", "_BYTE", "BYTE", "const uint8_t"):
            with self.subTest(ty=ty):
                self.assertEqual(difftable.arg_width(ty), 1)
        for ty in ("short", "unsigned short", "__int16", "_WORD", "WORD", "wchar_t", "u_short"):
            with self.subTest(ty=ty):
                self.assertEqual(difftable.arg_width(ty), 2)

    def test_pointers_and_unknown_types_stay_full(self):
        for ty in ("char *", "const short *", "char &", "char[4]", "LPBYTE", "LPWORD", "BOOL", "_BOOL4",
                   "enum char", "Unknown", "int", "unsigned int", "float"):
            with self.subTest(ty=ty):
                self.assertEqual(difftable.arg_width(ty), 4)

    def info(self, args, pop):
        # Synthetic ABI: no original-address special cases, no binary or database writes.
        ea = 1
        db = type("DB", (), {"F": {ea: {"size": 1}}})()
        abi = {f"{ea:08x}": {"args": args, "cc": "thiscall", "ret": "int"}}
        with patch.object(difftable, "ABI", abi), patch.object(difftable, "ret_bytes", return_value=[pop]), \
                patch.object(difftable, "entry_reads", return_value=True), patch("api.returns_fpu", return_value=False):
            return difftable.callee_info(db, ea)

    def test_registers_do_not_shift_stack_widths(self):
        ci = self.info([["void *", "this", "ecx"], ["short", "reg", "edx"],
                        ["int", "a", "stack"], ["char", "b", "stack"]], 8)
        self.assertEqual(ci["regs"], 3)
        self.assertEqual(ci["nargs"], 2)
        self.assertEqual(ci["widths"], [4, 1, 4, 4, 4, 4, 4, 4])

    def test_wide_arguments_keep_two_slots(self):
        ci = self.info([["double", "a", "stack"], ["char", "b", "stack"],
                        ["unsigned __int64", "c", "stack"], ["__int16", "d", "stack"]], 24)
        self.assertEqual(ci["nargs"], 6)
        self.assertEqual(ci["widths"], [4, 4, 1, 4, 4, 2, 4, 4])

    def test_cleanup_only_words_stay_full(self):
        ci = self.info([["char", "a", "stack"]], 12)
        self.assertEqual(ci["nargs"], 3)
        self.assertEqual(ci["widths"], [1, 4, 4, 4, 4, 4, 4, 4])
        self.assertEqual(self.info([], 32)["widths"], [4] * 8)

    def test_recording_limit(self):
        ci = self.info([["__int16", f"a{i}", "stack"] for i in range(10)], 40)
        self.assertEqual(ci["nargs"], 8)
        self.assertEqual(ci["widths"], [2] * 8)


class CompareTests(unittest.TestCase):
    def test_stack_boundaries_and_defined_bits(self):
        with tempfile.TemporaryDirectory() as tmp:
            tmp = Path(tmp)
            # Supply synthetic TEB bounds so the actual comparison helpers can run on the Linux host.
            (tmp / "intrin.h").write_text('''
#pragma once
inline unsigned __readfsdword(unsigned offset) { return offset == 8 ? 0x100000u : 0x200000u; }
''')
            (tmp / "compare.cpp").write_text('''
#include <cassert>
#include "diff_compare.h"
using namespace hook::diff_detail;
int main() {
    assert(on_stack(0x100000));
    assert(!on_stack(0xFFFFF));
    assert(!on_stack(0x200000));
    assert(same_frame_return(0x19DD08, 0x19DCF8, 0x19DE00));
    assert(!same_frame_return(0x19DD08, 0x19DE00, 0x19DE00));
    assert(!same_frame_return(0x19DE00, 0x19DCF8, 0x19DE00));
    assert(!same_frame_return(0x19DD08, 0x300000, 0x19DE00));
    assert(!same_frame_return(0x300000, 0x19DCF8, 0x19DE00));
    assert(!same_frame_return(0x100, 0x200, 0x19DE00));
    assert(same_arg(0x1C2D7D01, 1, 1));
    assert(!same_arg(0x1C2D7D01, 0, 1));
    assert(same_arg(0x1234ABCD, 0x5678ABCD, 2));
    assert(!same_arg(0x1234ABCD, 0x5678ABCE, 2));
    assert(!same_arg(0x1C2D7D01, 1, 4));
    assert(!same_arg(0x1C2D7D01, 1, 0));
    assert(same_arg(0x19DD08, 0x19DCF8));
    assert(!same_arg(0x19DD08, 0x19DCF8, 1));
    assert(!same_arg(0x19DD08, 0x19DCF8, 2));
    assert(!same_arg(0x19DD08, 0x300000));
}
''')
            subprocess.run(["clang++-21", "-std=c++20", "-I", str(tmp), "-I", str(ROOT / "hook"),
                            str(tmp / "compare.cpp"), "-o", str(tmp / "compare")], check=True, capture_output=True)
            subprocess.run([str(tmp / "compare")], check=True)


if __name__ == "__main__":
    unittest.main()
