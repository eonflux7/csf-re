"""Offline mutation benchmark regressions: .venv/bin/python tests/test_emumut.py."""
import contextlib
import argparse
import io
import json
from pathlib import Path
import sys
import tempfile
import unittest
from unittest import mock

ROOT = Path(__file__).resolve().parents[1]
sys.path.insert(0, str(ROOT / 'tools'))
import emumut
import gate

FIXTURE = '''
#include <cstdint>
#include "globals.h"
CSF_FUNC(int, __cdecl, Compute, 0x00401060, (int a, int b));
CSF_FUNC(void, __cdecl, Write, 0x00401090, (int* out));
struct Object { void** vtbl; int field_0004; int field_0008; };
extern "C" int __cdecl Fixture(Object* self, int a, int b)
{
    // Compute(999, 999); if (a) { CSF_AT(int, self, 0x44) = 999; }
    CSF_AT(uint32_t, self, 0x10) = 7;
    self->field_0004 = a;
    if (a < b) Compute(a, b);
    for (int i = 0; i < 3; ++i) self->field_0008 = i;
    Compute(a, b);
    int output = 1;
    Write(&output);
    Compute(output, b);
    reinterpret_cast<int(__cdecl*)(int)>(self->vtbl[2])(a);
    return Compute(a, b);
}
'''


class GeneratorTests(unittest.TestCase):
    @classmethod
    def setUpClass(cls):
        cls.command = gate.compile_cmd()

    def compile(self, source, folder, name):
        src, obj = Path(folder) / (name + '.cpp'), Path(folder) / (name + '.obj')
        src.write_text(source)
        error = emumut.compile_source(src, obj, self.command)
        self.assertIsNone(error, error)
        return obj

    def test_deterministic_and_one_edit(self):
        left = emumut.candidates('00401060', FIXTURE, [0x00401060, 0x00401090, 0x004010c0])
        right = emumut.candidates('00401060', FIXTURE, [0x00401060, 0x00401090, 0x004010c0])
        self.assertEqual(left, right)
        self.assertEqual({m.klass for m in left}, set(emumut.CLASSES))
        for m in left:
            self.assertEqual(m.line, FIXTURE.count('\n', 0, m.start) + 1)
            edited = m.apply(FIXTURE)
            self.assertEqual(edited[:m.start], FIXTURE[:m.start])
            self.assertEqual(edited[m.start + len(m.after):], FIXTURE[m.end:])
            self.assertNotIn('999', m.before)
            self.assertEqual(m.record(), m.record())
        pools = {k: [m for m in left if m.klass == k] for k in emumut.CLASSES}
        self.assertEqual(emumut.schedule(pools, 30, 1), emumut.schedule(pools, 30, 1))
        self.assertNotEqual(emumut.schedule(pools, 30, 1), emumut.schedule(pools, 30, 2))
        self.assertEqual(left, emumut.candidates('00401060', FIXTURE, [0x00401060, 0x00401090, 0x004010c0]))

    def test_each_class_compiles(self):
        mutants = emumut.candidates('00401060', FIXTURE, [0x00401060, 0x00401090, 0x004010c0])
        with tempfile.TemporaryDirectory() as tmp:
            base = self.compile(FIXTURE, tmp, 'base')
            baseline = emumut.code_fingerprint(base)
            for klass in emumut.CLASSES:
                with self.subTest(klass=klass):
                    # Every class must have a compiling, non-equivalent fixture mutant.
                    found, errors = False, []
                    for i, m in enumerate(x for x in mutants if x.klass == klass):
                        src, obj = Path(tmp) / f'{klass}{i}.cpp', Path(tmp) / f'{klass}{i}.obj'
                        src.write_text(m.apply(FIXTURE))
                        error = emumut.compile_source(src, obj, self.command)
                        if error:
                            errors.append(error)
                            continue
                        if emumut.code_fingerprint(obj) != baseline:
                            found = True
                            break
                    self.assertTrue(found, '\n'.join(errors))

    def test_legacy_virtual_slot(self):
        src = FIXTURE.replace('self->vtbl[2]', 'CSF_AT(void**, self, 0)[2]')
        mutants = [m for m in emumut.candidates('00401060', src) if m.klass == 'virtual_slot']
        self.assertTrue(mutants)
        with tempfile.TemporaryDirectory() as tmp:
            self.compile(mutants[0].apply(src), tmp, 'legacy')

    def test_typed_virtual_slot(self):
        src = FIXTURE.replace('int field_0008; };', 'int field_0008; int vSlot0008(int); };')
        src = src.replace('reinterpret_cast<int(__cdecl*)(int)>(self->vtbl[2])(a)', 'self->vSlot0008(a)')
        mutants = [m for m in emumut.candidates('00401060', src) if m.klass == 'virtual_slot']
        self.assertEqual(len(mutants), 1)
        with tempfile.TemporaryDirectory() as tmp:
            self.compile(mutants[0].apply(src), tmp, 'typed')

    def test_equivalent_control_and_relocated_constants(self):
        with tempfile.TemporaryDirectory() as tmp:
            a = self.compile(FIXTURE, tmp, 'a')
            b = self.compile(FIXTURE.replace('return Compute(a, b);', 'return (Compute(a, b));'), tmp, 'b')
            self.assertEqual(emumut.code_fingerprint(a), emumut.code_fingerprint(b))
            start = FIXTURE.index('Compute(a, b)', FIXTURE.index('return'))
            mutation = emumut.Mutation('00401060', 'dropped_return', start, start + len('Compute(a, b)'),
                                       FIXTURE.count('\n', 0, start) + 1, 'Compute(a, b)', '(Compute(a, b))')
            record, _, _ = emumut.build_mutant(mutation, FIXTURE, tmp, self.command, emumut.code_fingerprint(a))
            self.assertEqual(record['exclude'], 'equivalent')
            self.assertNotIn('outcome', record)
            # Code sections alone miss float constants in a relocated rdata section.
            c = self.compile('extern "C" float Value() { return 1.25f; }', tmp, 'c')
            d = self.compile('extern "C" float Value() { return 1.5f; }', tmp, 'd')
            self.assertNotEqual(emumut.code_fingerprint(c), emumut.code_fingerprint(d))
            # A relocated external target change must survive masking, too.
            external = 'extern "C" int First(); extern "C" int Second(); extern "C" int Value() { return First(); }'
            g = self.compile(external, tmp, 'g')
            h = self.compile(external.replace('return First();', 'return Second();'), tmp, 'h')
            self.assertNotEqual(emumut.code_fingerprint(g), emumut.code_fingerprint(h))
            # HOOK registration embeds line numbers and filenames, unlike the hook.
            hooked = '#include "hook.h"\nextern "C" int Hooked() { return 7; }\nHOOK(0x00401060, Hooked, "auto");\n'
            e = self.compile(hooked, tmp, 'e')
            f = self.compile('\n' + hooked.replace('return 7;', 'return (7);'), tmp, 'f')
            self.assertEqual(emumut.code_fingerprint(e), emumut.code_fingerprint(f))

    def test_compare_reports_lost_and_original_changes(self):
        m = emumut.candidates('00401060', FIXTURE)[0].record()
        m.update(fingerprint='mutant', verdict='DIFF', outcome='caught', detail='call differs')
        a = dict(mutants=[m], originals={'00401060': dict(fingerprint='base', verdict='same', detail='8/8')}, excluded=[])
        b = dict(a, mutants=[dict(m, verdict='inconclusive', outcome='inconclusive', detail='cap')],
                 originals={'00401060': dict(fingerprint='base', verdict='DIFF', detail='returns')})
        with tempfile.TemporaryDirectory() as tmp:
            paths = [Path(tmp) / f'{k}.json' for k in 'ab']
            for p, data in zip(paths, (a, b)):
                p.write_text(json.dumps(data))
            output = io.StringIO()
            with contextlib.redirect_stdout(output):
                emumut.compare(*paths)
            self.assertIn('LOST 00401060', output.getvalue())
            self.assertIn('ORIGINAL 00401060', output.getvalue())
            self.assertIn('NOT COVERED:', output.getvalue())
            b['mutants'][0]['fingerprint'] = 'changed-object'
            paths[1].write_text(json.dumps(b))
            with contextlib.redirect_stdout(io.StringIO()), self.assertRaisesRegex(ValueError, 'object code changed'):
                emumut.compare(*paths)

    def test_replay_preserves_rejected_original_controls(self):
        mutation = next(m for m in emumut.candidates('00401060', FIXTURE) if m.klass == 'constant')
        prior = dict(seed=1, mutants=[mutation.record()],
                     originals={'00401060': {}, '00401090': {}},
                     sources={'00401060': FIXTURE, '00401090': FIXTURE})
        import verify
        with tempfile.TemporaryDirectory() as tmp:
            replay, output = Path(tmp) / 'prior.json', Path(tmp) / 'result.json'
            replay.write_text(json.dumps(prior))
            args = argparse.Namespace(emudiff=ROOT / 'tools/emudiff.py', replay=replay,
                                      seed=1, n=1, jobs=1, out=output)
            fingerprint = lambda p: 'mutated' if Path(p).stem.startswith('m') else 'original'
            with mock.patch.object(emumut, 'compile_source', return_value=None), \
                 mock.patch.object(verify, 'code_hash', return_value='hash'), \
                 mock.patch.object(emumut, 'code_fingerprint', side_effect=fingerprint), \
                 mock.patch.object(emumut, 'emulate', return_value=dict(verdict='same', detail='8/8')), \
                 contextlib.redirect_stdout(io.StringIO()):
                emumut.run(args)
            result = json.loads(output.read_text())
            self.assertEqual(set(result['originals']), set(prior['originals']))
            self.assertEqual(len(result['mutants']), 1)


if __name__ == '__main__':
    unittest.main()
