"""Paired semantic regression on 300 installed functions and their mutants.

Save the pre-change checker with git show REV:tools/semcheck.py to
work/qsem_semcheck_before.py. Then run this script with build, followed by
compare after editing the checker. Objects, selected addresses and full
results remain under work/qsem_regression; no ledger or source is changed.
"""
import concurrent.futures as cf
import importlib.util
import functools
import json
import os
from pathlib import Path
import re
import subprocess
import sys

ROOT = Path(__file__).resolve().parents[1]
sys.path.insert(0, str(ROOT / 'tools'))
from gate import compile_cmd
from packet import DB
import controls
import semcheck

OUT = ROOT / 'work/qsem_regression'
OUT.mkdir(exist_ok=True)
spec = importlib.util.spec_from_file_location('qsem_before', ROOT / 'work/qsem_semcheck_before.py')
before = importlib.util.module_from_spec(spec)
spec.loader.exec_module(before)
before.ROOT = str(ROOT)
before.code_of = functools.lru_cache(None)(before.code_of)
semcheck.code_of = functools.lru_cache(None)(semcheck.code_of)
db = DB()
CC = compile_cmd()

def errors(module, ea, obj, src):
    fn = controls._hooked(src)
    return (module.check(ea, str(obj), db)
            + module.byte_return_of_bool(ea, str(obj), fn, db)
            + module.behaviour(ea, str(obj), fn, src, db))

def build():
    ledger = json.loads((ROOT / 'db/verify.json').read_text())
    installed = sorted(e for e, v in ledger.items() if v.get('ok')
                       and (ROOT / f'src/auto/{e}.cpp').exists())
    selected = [installed[i * (len(installed) - 1) // 299] for i in range(300)]
    assert len(set(selected)) == 300
    (OUT / 'selected.json').write_text(json.dumps(selected, indent=2))
    tasks = []
    for e in selected:
        src = (ROOT / f'src/auto/{e}.cpp').read_text()
        tasks.append((e, 'original', src))
        for kind in ('offset', 'callee', 'global', 'string', 'dropcall', 'const'):
            mutant = (controls.mut_const if kind == 'const' else controls.OPERATORS[kind])(src)
            if mutant and mutant != src:
                tasks.append((e, kind, mutant))

    def one(task):
        e, kind, src = task
        p = OUT / f'{e}_{kind}.cpp'
        obj = p.with_suffix('.obj')
        p.write_text(src)
        r = subprocess.run(CC + [f'/Fo{obj}', '-c', '--', str(p)],
                           cwd=ROOT / 'build', text=True, capture_output=True)
        row = dict(ea=e, kind=kind, compiled=r.returncode == 0)
        if not r.returncode:
            row['before'] = errors(before, int(e, 16), obj, src)
        else:
            row['compile_error'] = r.stderr[-500:]
        return row

    with cf.ThreadPoolExecutor(4) as ex:
        rows = list(ex.map(one, tasks))
    (OUT / 'baseline.json').write_text(json.dumps(rows, indent=2))
    print('baseline', len(selected), 'installed;', len(rows), 'variants;',
          sum(r['compiled'] for r in rows), 'compiled', flush=True)

def compare():
    rows = json.loads((OUT / 'baseline.json').read_text())
    def one(row):
        if row['compiled']:
            p = OUT / f"{row['ea']}_{row['kind']}.cpp"
            row['after'] = errors(semcheck, int(row['ea'], 16), p.with_suffix('.obj'), p.read_text())
        return row
    with cf.ThreadPoolExecutor(4) as ex:
        rows = list(ex.map(one, rows))
    compiled = [r for r in rows if r['compiled']]
    new = [r for r in compiled if set(r['after']) - set(r['before'])]
    lost = [r for r in compiled if set(r['before']) - set(r['after'])]
    (OUT / 'comparison.json').write_text(json.dumps(rows, indent=2))
    (OUT / 'changes.json').write_text(json.dumps(dict(new=new, lost=lost), indent=2))
    originals = [r for r in compiled if r['kind'] == 'original']
    mutants = [r for r in compiled if r['kind'] != 'original']
    print('originals:', len(originals), 'before alarms:', sum(bool(r['before']) for r in originals),
          'after alarms:', sum(bool(r['after']) for r in originals))
    print('mutants:', len(mutants), 'before rejected:', sum(bool(r['before']) for r in mutants),
          'after rejected:', sum(bool(r['after']) for r in mutants))
    print('new alarm rows:', len(new), 'lost alarm rows:', len(lost))
    for r in new + lost:
        print(r['ea'], r['kind'], 'new:', set(r['after']) - set(r['before']),
              'lost:', set(r['before']) - set(r['after']))

if __name__ == '__main__':
    {'build': build, 'compare': compare}[sys.argv[1]]()
