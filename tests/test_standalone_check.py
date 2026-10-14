"""Host-only CSF_DRYRUN input checks; build csf_standalone first."""
import json
import os
from pathlib import Path
import shutil
import sys
import tempfile
from types import SimpleNamespace
import unittest

import capstone

ROOT = Path(__file__).resolve().parent.parent
sys.path.insert(0, str(ROOT / 'tools'))
from standalone_check import audit, virtual_sites, vtable_traps, reachable_code, check_compiled_inputs


class VirtualCalls(unittest.TestCase):
    def decode(self, data):
        decoder = capstone.Cs(capstone.CS_ARCH_X86, capstone.CS_MODE_32)
        decoder.detail = True
        return list(decoder.disasm(bytes.fromhex(data), 0x1000))

    def test_exact_vtable_call(self):
        code = self.decode('ff 15 04 20 00 00')  # call [0x2004]
        exact, possible, unknown = virtual_sites(code, {0x2004: 0x3000}, {0x2000})
        self.assertEqual(exact, [(0x1000, 0x2004)])
        self.assertFalse(possible or unknown)

    def test_computed_call_is_not_certified(self):
        code = self.decode('c7 01 00 20 00 00 8b 01 ff 50 04')
        exact, possible, unknown = virtual_sites(code, {0x2004: 0x3000}, {0x2000})
        self.assertFalse(exact)
        self.assertEqual(possible, [(0x1008, 0x2004)])
        self.assertEqual(unknown, [0x1008])

    def test_non_vtable_iat_call_remains_unknown(self):
        exact, possible, unknown = virtual_sites(self.decode('ff 15 00 40 00 00'), {}, set())
        self.assertFalse(exact or possible)
        self.assertEqual(unknown, [0x1000])

    def test_called_vtable_trap_is_a_blocker(self):
        sites = [(0x1000, 0x2004)]
        self.assertEqual(vtable_traps(sites, {0x2004: 0x3000}, {0x3000}),
                         [dict(instruction='0x1000', source='0x2004', target='0x3000')])
        self.assertFalse(vtable_traps(sites, {0x2004: 0x3000}, set()))

    def test_reachable_blocks_skip_embedded_bytes(self):
        data = bytes.fromhex('eb 04 90 90 90 90 c3')
        image = SimpleNamespace(base=0x1000,
            pe=SimpleNamespace(get_data=lambda off, size: data[off:off + size]))
        decoder = capstone.Cs(capstone.CS_ARCH_X86, capstone.CS_MODE_32)
        decoder.detail = True
        code = reachable_code(image, decoder, 0x1000, len(data))
        self.assertEqual([i.address for i in code], [0x1000, 0x1006])

    def test_stale_coff_inventory_rejected(self):
        with tempfile.TemporaryDirectory() as tmp:
            root = Path(tmp)
            obj = root / 'changed.obj'
            obj.write_bytes(b'not inspected before freshness check')
            (root / 'objects.txt').write_text(str(obj) + '\n')
            inventory = root / 'linktest.json'
            inventory.write_text('{}')
            os.utime(inventory, ns=(0, 0))
            with self.assertRaisesRegex(AssertionError, 'stale COFF inventory'):
                check_compiled_inputs(root, set(), set(), set(), None)


class GeneratedInputs(unittest.TestCase):
    @classmethod
    def setUpClass(cls):
        cls.cutover = ROOT / 'build/cutover'
        cls.exe = ROOT / 'work/pc/CommXPC.exe'
        cls.result = audit(cls.cutover, cls.exe)

    def test_generated_inputs_consistent(self):
        self.assertEqual(self.result['structural_consistency'], 'ok')
        self.assertEqual(self.result['undefined_fixup_targets'], 0)
        self.assertEqual(self.result['initializers'], 201)

    def test_startup_blockers_are_not_hidden(self):
        blocked = any(self.result[k] for k in (
            'initializer_traps', 'exact_initializer_vtable_traps',
            'possible_initializer_vtable_traps', 'unanalysed_indirect_calls',
            'stopped_library_callees'))
        self.assertEqual(self.result['startup_ready'], not blocked)

    def test_entry_dryrun_contract(self):
        source = (ROOT / 'standalone/main.cpp').read_text()
        positions = [source.index(s) for s in ('csf::initialize_data();',
            'csf::run_original_initializers();', 'GetEnvironmentVariableA("CSF_DRYRUN"',
            'return Game_WinMain(')]
        self.assertEqual(positions, sorted(positions))
        dry = source[positions[2]:positions[3]]
        self.assertIn('!std::strcmp(dry, "1")', dry)
        self.assertRegex(dry, r'return 0;\s*}')

    def mutate(self, filename, change):
        with tempfile.TemporaryDirectory() as tmp:
            out = Path(tmp)
            for name in ('data', 'base.json', 'initializers.json', 'initializers.cpp', 'linktest.json', 'stubs.json'):
                src = self.cutover / name
                if src.is_dir():
                    shutil.copytree(src, out / name)
                else:
                    shutil.copy(src, out / name)
            path = out / filename
            value = json.loads(path.read_text())
            change(value)
            path.write_text(json.dumps(value))
            with self.assertRaises(AssertionError):
                audit(out, self.exe)

    def test_reordered_initializer_table_rejected(self):
        self.mutate('initializers.json', lambda r: r['table_slots'].reverse())

    def test_missing_trap_definition_rejected(self):
        self.mutate('stubs.json', lambda r: r['slots'].pop())

    def test_changed_initial_bytes_hash_rejected(self):
        self.mutate('data/manifest.json', lambda r: r.update(image_sha256='wrong'))

    def test_duplicate_applied_pointer_rejected(self):
        def duplicate(rows):
            indices = [i for i, row in enumerate(rows) if row['applied']]
            rows[indices[1]] = dict(rows[indices[0]])
        self.mutate('data/datagen-pointers.json', duplicate)


if __name__ == '__main__':
    unittest.main()
