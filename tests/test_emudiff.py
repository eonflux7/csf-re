"""Offline emulator regressions. Run: .venv/bin/python tests/test_emudiff.py"""
import os
from pathlib import Path
import subprocess
import sys
import tempfile
import unittest
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

    def test_dynamic_frame_keeps_export(self):
        ins, counts = self.infer("55 8bec 83e4f0 ff5028 8be5 5d c3")
        self.assertEqual(counts, {})


if __name__ == "__main__":
    unittest.main()
