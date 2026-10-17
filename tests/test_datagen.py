"""Offline data/coverage regressions: .venv/bin/python tests/test_datagen.py.

Uses real PE + cached linktest inventory; no link or game execution.
"""
import contextlib
import io
import json
from pathlib import Path
import re
import sys
import tempfile
from types import SimpleNamespace
import unittest

ROOT = Path(__file__).resolve().parent.parent
sys.path.insert(0, str(ROOT / 'tools'))
from datagen import Image, definitions, generate, number


class DataGeneration(unittest.TestCase):
    @classmethod
    def setUpClass(cls):
        cls.tmp = tempfile.TemporaryDirectory()
        cls.out = Path(cls.tmp.name)
        cls.image = Image(ROOT / 'work/pc/CommXPC.exe')
        cls.inventory = json.loads((ROOT / 'work/linktest.json').read_text())
        with contextlib.redirect_stdout(io.StringIO()):
            cls.summary = generate(SimpleNamespace(
                exe=ROOT / 'work/pc/CommXPC.exe', linktest=ROOT / 'work/linktest.json',
                output=cls.out / 'generated', report=cls.out / 'datagen.json',
                data_refs=None, candidate_fixups=False, compile_check=False))
        cls.src = (cls.out / 'generated/data.cpp').read_text()
        cls.rejected = json.loads((cls.out / 'datagen-rejected.json').read_text())
        cls.pointers = json.loads((cls.out / 'datagen-pointers.json').read_text())

    @classmethod
    def tearDownClass(cls):
        cls.tmp.cleanup()

    def test_each_reference_defined_once_or_explicitly_rejected(self):
        required = {('gv' if k == 'globals' else 'vt', number(a))
                    for k in ('globals', 'vtables') for a in self.inventory[k]}
        emitted = definitions(self.src)
        rejected = {(r['kind'], number(r['address'])) for r in self.rejected}
        self.assertTrue(all(n == 1 for n in emitted.values()))
        self.assertFalse(set(emitted) & rejected)
        self.assertEqual(set(emitted) | rejected, required)
        self.assertEqual(self.summary['all_linktest_definitions'], not rejected)

    def test_initialized_bytes_and_bss_exact(self):
        init = self.src.split('] = {', 1)[1].split('};', 1)[0]
        b = bytearray(int(x, 16) for x in re.findall(r'0x([0-9A-F]{2})', init))
        b.extend(bytes(len(self.image.bytes) - len(b)))
        self.assertEqual(b, self.image.bytes)
        for sec, meta in zip(self.image.pe.sections, self.image.sections):
            if meta['executable']:
                continue
            off = meta['start'] - self.image.start
            self.assertEqual(b[off:off + meta['raw_size']], sec.get_data())
            if meta['virtual_size'] > meta['raw_size']:
                self.assertFalse(any(b[off + meta['raw_size']:off + meta['virtual_size']]))

    def test_aliases_preserve_all_offsets_and_gv_vt_identity(self):
        for kind, addr, offset in re.findall(
                r'(gv|vt)<0x([0-9A-F]+)>::[dt] = image \+ (\d+);', self.src):
            self.assertEqual(int(addr, 16) - self.image.start, int(offset))
        # The rule is independent of inferred object size and applies equally
        # to overlapping globals and vtable aliases.
        aliases = [line for line in self.src.splitlines() if line.startswith('template <>')]
        self.assertTrue(all(re.search(r'= image \+ \d+;', line) for line in aliases))

    def test_fixups_use_own_storage_or_fn_slots(self):
        applied = [r for r in self.pointers if r['applied']]
        self.assertEqual(len(applied), self.summary['fixups'])
        fixes = {int(off): expr for off, expr in re.findall(r'put\((\d+), (.*)\);', self.src)}
        for r in applied:
            a, t = number(r['source']), number(r['target'])
            self.assertEqual(self.image.word(a), t)
            self.assertTrue(r['confirmed'] or r['provenance'] == 'aligned-word-known-target' or
                            (r['provenance'] == 'aligned-word-image-range' and not self.image.code(t)))
            self.assertNotIn('+text', r['provenance'])
            expr = f'fn_slot<0x{t:08X}>::p' if self.image.code(t) else f'image + {t - self.image.start}'
            self.assertEqual(fixes[a - self.image.start], expr)

    def test_pointer_source_locations_unique(self):
        self.assertEqual(len(self.pointers), len({r['source'] for r in self.pointers}))

    def test_stripped_pe_does_not_claim_complete_relocations(self):
        if self.image.pe.OPTIONAL_HEADER.DATA_DIRECTORY[5].Size == 0:
            self.assertTrue(self.summary['relocations_stripped'])
            self.assertFalse(self.summary['relocation_inventory_complete'])
            self.assertTrue(self.summary['warning'])


if __name__ == '__main__':
    unittest.main()
