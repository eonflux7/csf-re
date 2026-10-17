"""The required-check runner must reject failures, including formerly piped checks."""
import os
from pathlib import Path
import shutil
import subprocess
import tempfile
import unittest

ROOT = Path(__file__).resolve().parents[1]


class CheckRunnerTests(unittest.TestCase):
    def setUp(self):
        self.temp = tempfile.TemporaryDirectory()
        self.addCleanup(self.temp.cleanup)
        self.root = Path(self.temp.name)
        for name in ('tools', 'src/boundary/rw', '.venv/bin', 'bin'):
            (self.root / name).mkdir(parents=True)
        shutil.copy(ROOT / 'tools/check_all.sh', self.root / 'tools/check_all.sh')
        (self.root / 'src/boundary/rw/CMakeLists.txt').write_text(
            'add_executable(csf_rw_fixture_link fixture.cpp)\n')
        self.script('bin/cmake', '''#!/bin/bash
if [[ "$*" == *"$FAIL_CHECK"* && -n "$FAIL_CHECK" ]]; then exit 23; fi
if [[ "$1" == -S ]]; then
  mkdir -p work/rwcheck-asan/src/boundary/rw
  for t in fixture world world_userdata clump; do
    printf '#!/bin/bash\\n[[ "$FAIL_CHECK" != native ]] || exit 23\\n' > "work/rwcheck-asan/src/boundary/rw/csf_rw_${t}_link"
    chmod +x "work/rwcheck-asan/src/boundary/rw/csf_rw_${t}_link"
  done
fi
''')
        self.script('.venv/bin/python', '''#!/bin/bash
if [[ "$*" == *"$FAIL_CHECK"* && -n "$FAIL_CHECK" ]]; then
  echo 'injected failure' >&2
  exit 23
fi
echo 'fixture check passed'
''')

    def script(self, name, content):
        path = self.root / name
        path.write_text(content)
        path.chmod(0o755)

    def run_checks(self, failure='', args=('--offline',)):
        return subprocess.run(['bash', 'tools/check_all.sh', *args], cwd=self.root,
                              env={**os.environ, 'PATH': str(self.root / 'bin') + ':' + os.environ['PATH'],
                                   'FAIL_CHECK': failure}, capture_output=True, text=True)

    def test_success_and_offline_skip(self):
        result = self.run_checks()
        self.assertEqual(result.returncode, 0, result.stderr)
        self.assertIn('all requested checks passed', result.stdout)
        self.assertIn('dryrun skipped', result.stdout)
        self.assertFalse((self.root / 'work/check-walk.log').exists())

    def test_failed_checks_stop_and_preserve_exit_status(self):
        for failure in ('-S', 'native', 'linktest.py', 'csf_standalone', 'netzstub.py',
                        'mission_catalog_offline.py', '-m unittest'):
            with self.subTest(failure=failure):
                result = self.run_checks(failure)
                self.assertEqual(result.returncode, 23, result.stdout + result.stderr)
                self.assertIn('FAIL (exit=23;', result.stderr)
                self.assertNotIn('all requested checks passed', result.stdout)

    def test_failed_walk_cannot_be_hidden_by_output_filter(self):
        result = self.run_checks('--walk', args=())
        self.assertEqual(result.returncode, 23, result.stdout + result.stderr)
        self.assertIn('menu walk: FAIL', result.stderr)


if __name__ == '__main__':
    unittest.main()
