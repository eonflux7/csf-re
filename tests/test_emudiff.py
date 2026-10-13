"""Offline stack-probe regressions. Run: .venv/bin/python tests/test_emudiff.py"""
import os
from pathlib import Path
import subprocess
import sys
import tempfile
import unittest

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


if __name__ == "__main__":
    unittest.main()
