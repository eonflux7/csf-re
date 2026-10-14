"""Regression for IDA offset facts that actually contain short text (crash2).

Run: .venv/bin/python tests/test_datagen_text.py
Only needs the original PE and read-only exports, not a cached linktest run.
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
from datagen import Image, generate, number


class ShortTextFixups(unittest.TestCase):
    def test_xy_stays_text_but_its_pointer_and_adjacent_vtable_relocate(self):
        with tempfile.TemporaryDirectory() as tmp:
            out = Path(tmp)
            manifest = out / 'linktest.json'
            manifest.write_text(json.dumps(dict(globals=['0x008A85EC', '0x008A8600'],
                                                vtables=['0x00864A58'])))
            with contextlib.redirect_stdout(io.StringIO()):
                generate(SimpleNamespace(
                    exe=ROOT / 'work/pc/CommXPC.exe', linktest=manifest,
                    output=out / 'data', report=out / 'datagen.json',
                    data_refs=ROOT / 'export/dataptrs.json',
                    candidate_fixups=False, compile_check=False))
            rows = {number(r['source']): r for r in
                    json.loads((out / 'datagen-pointers.json').read_text())}
            image = Image(ROOT / 'work/pc/CommXPC.exe')
            self.assertEqual(image.word(0x00864A54), int.from_bytes(b'.XY\0', 'little'))
            self.assertEqual(rows[0x00864A54]['target'], '0x59582e')
            self.assertTrue(rows[0x00864A54]['confirmed'])  # erroneous IDA fact
            self.assertFalse(rows[0x00864A54]['applied'])
            self.assertTrue(rows[0x00864A54]['provenance'].endswith('+text'))
            self.assertEqual(rows[0x008A85EC]['target'], '0x864a54')
            self.assertTrue(rows[0x008A85EC]['applied'])
            self.assertEqual(rows[0x00864A58]['target'], '0x71d820')
            self.assertTrue(rows[0x00864A58]['applied'])
            source = (out / 'data/data.cpp').read_text()
            fixes = {int(off): expr for off, expr in
                     re.findall(r'put\((\d+), (.*)\);', source)}
            self.assertNotIn(0x00864A54 - image.start, fixes)
            self.assertEqual(fixes[0x008A85EC - image.start],
                             f'image + {0x00864A54 - image.start}')
            self.assertEqual(fixes[0x00864A58 - image.start], 'fn_slot<0x0071D820>::p')
            self.assertEqual(image.word(0x008305F4), int.from_bytes(b'.ID\0', 'little'))
            self.assertFalse(rows[0x008305F4]['applied'])
            self.assertNotIn(0x008305F4 - image.start, fixes)
            self.assertEqual(rows[0x008A8600]['target'], '0x8305f4')
            self.assertEqual(fixes[0x008A8600 - image.start],
                             f'image + {0x008305F4 - image.start}')
            for a in (0x008631B8, 0x008631BC):  # crash1 must remain fixed, too
                self.assertFalse(rows[a]['applied'])
            # Audit all literal keys in the builder's contiguous keyword table:
            # an applied pointer write must not overlap any of their text.
            applied = [number(r['source']) for r in rows.values() if r['applied']]
            for a in range(0x008A85EC, 0x008A8780, 4):
                key = image.word(a)
                self.assertTrue(image.data(key))
                start = key - image.start
                length = image.bytes.index(0, start) - start + 1
                self.assertFalse(any(p < key + length and p + 4 > key for p in applied),
                                 f'keyword at {key:#x} is overwritten')


if __name__ == '__main__':
    unittest.main()
