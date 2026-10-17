#!/usr/bin/env python3
"""Offline first-link traps: actual COFF definitions, stale/duplicate rejection."""
import json
from pathlib import Path
import subprocess
import sys
import tempfile
import unittest

ROOT = Path(__file__).resolve().parent.parent
sys.path.insert(0, str(ROOT / 'tools'))
import linktest
import stubgen
import initgen
import linkreport


class Generators(unittest.TestCase):
    def setUp(self):
        self.tmp = tempfile.TemporaryDirectory()
        self.addCleanup(self.tmp.cleanup)
        self.path = Path(self.tmp.name)
        self.inventory = self.path / 'linktest.json'
        self.report = dict(failed={}, duplicates={}, defined_functions=['0x401000'],
                           unresolved_functions={'game': {'0x509250': 1}})
        self.write()

    def write(self):
        self.inventory.write_text(json.dumps(self.report))

    def compile(self, name, text):
        cpp = self.path / (name + '.cpp')
        obj = cpp.with_suffix('.obj')
        cpp.write_text(text)
        result = subprocess.run(linktest.flags() + ['-I' + str(ROOT / 'standalone'),
                                    '/Fo' + str(obj), '-c', '--', str(cpp)],
                                cwd=ROOT / 'build', capture_output=True, text=True)
        self.assertEqual(result.returncode, 0, result.stdout + result.stderr)
        return obj

    def test_actual_objects_and_shrink(self):
        obj = self.compile('real', '#include "hook.h"\n'
            'static void impl() {}\nHOOK(0x00401000, impl, "test");\n'
            'void* demand() { return (void*)CSF_ADDR_FN(0x00509250); }\n')
        objects = self.path / 'objects.txt'
        objects.write_text(str(obj) + '\n')
        stubgen.generate(self.inventory, self.path, objects)
        text = (self.path / 'stubs.cpp').read_text()
        self.assertIn('HOOK(0x00509250', text)
        self.assertNotIn('HOOK(0x00401000', text)
        self.assertIn('_WinMain@16', text)
        traps = self.compile('traps', text)
        definitions, _ = linktest.symbols(str(traps))
        names = linktest.demangle(sorted(s for s in definitions if 'csf@@' in s))
        slots = [int(m[2]) for name in names.values()
                 if (m := linktest.SLOT.search(name)) and m[1] == 'fn_slot']
        self.assertEqual(slots, [0x509250])
        # A real implementation arriving eliminates the trap, not a duplicate.
        self.report['defined_functions'].append('0x509250')
        self.report['unresolved_functions'] = {}
        self.write()
        arrived = self.compile('arrived', '#include "hook.h"\nstatic void impl() {}\n'
            'HOOK(0x00509250, impl, "test");\n')
        objects.write_text(str(obj) + '\n' + str(arrived) + '\n')
        stubgen.generate(self.inventory, self.path, objects)
        self.assertNotIn('HOOK(', (self.path / 'stubs.cpp').read_text())
        self.compile('empty', (self.path / 'stubs.cpp').read_text())

    def test_refuses_defined_or_duplicates(self):
        self.report['defined_functions'].append('0x509250')
        self.write()
        with self.assertRaises(ValueError):
            stubgen.generate(self.inventory, self.path)
        self.report['duplicates'] = {'0x401000': ['one.cpp', 'two.cpp']}
        self.write()
        with self.assertRaises(ValueError):
            stubgen.generate(self.inventory, self.path)

    def test_refuses_stale_inventory(self):
        obj = self.compile('arrived', '#include "hook.h"\nstatic void impl() {}\n'
            'HOOK(0x00401000, impl, "test");\nHOOK(0x00509250, impl, "test");\n')
        objects = self.path / 'objects.txt'
        objects.write_text(str(obj) + '\n')
        with self.assertRaises(ValueError):
            stubgen.generate(self.inventory, self.path, objects)

    def test_refuses_actual_duplicates_and_missing_objects(self):
        code = '#include "hook.h"\nstatic void impl() {}\nHOOK(0x00401000, impl, "test");\n'
        one = self.compile('one', code)
        two = self.compile('two', code)
        objects = self.path / 'objects.txt'
        objects.write_text(str(one) + '\n' + str(two) + '\n')
        with self.assertRaisesRegex(ValueError, 'duplicate actual'):
            stubgen.generate(self.inventory, self.path, objects)
        objects.write_text(str(self.path / 'absent.obj') + '\n')
        with self.assertRaisesRegex(ValueError, 'missing/empty'):
            stubgen.generate(self.inventory, self.path, objects)

    def test_original_table(self):
        initgen.generate(ROOT / 'work/pc/CommXPC.exe', self.inventory, self.path)
        report = json.loads((self.path / 'initializers.json').read_text())
        self.assertEqual(report['count'], 201)
        self.assertEqual([(r['table'], r['nonzero']) for r in report['ranges']], [('xi', 5), ('xc', 196)])
        self.assertEqual(report['covered'], 0)
        self.assertEqual(len((self.path / 'initializers.tsv').read_text().splitlines()), 202)
        self.assertIn('__cinit', (self.path / 'startup-disassembly.txt').read_text())
        source = (self.path / 'initializers.cpp').read_text()
        self.assertLess(source.index('0x007AEDE3: original CRT start-up, not called'),
                        source.index('0x007B90C8: original CRT start-up, not called'))
        self.assertNotIn('    init_007B90C8();', source)
        self.compile('init', source)

    def test_error_kinds(self):
        errors = ['undefined symbol: public: static void *const csf::fn_slot<123>::p',
                  'undefined symbol: public: static unsigned char *const csf::gv<123>::d',
                  'undefined symbol: public: static unsigned char *const csf::vt<123>::t',
                  'undefined symbol: other', 'undefined symbol: __declspec(dllimport) _Api@4',
                  'duplicate symbol: _duplicate', 'section size is too large',
                  'could not open missing.lib: No such file', 'unclassified diagnostic']
        grouped = linkreport.group('\n'.join('lld-link: error: ' + e for e in errors))
        self.assertEqual({k: len(v) for k, v in grouped.items()}, {k: 1 for k in grouped})


if __name__ == '__main__':
    unittest.main()
