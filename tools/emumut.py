#!/usr/bin/env python3
"""Seeded mutation benchmark for emudiff (offline, never installs hooks).

run --n 400 --seed 1 --jobs 8 --out work/emumut-baseline.json
run --emudiff /path/to/job/tools/emudiff.py --replay work/emumut-baseline.json --out work/emumut-job.json
compare work/emumut-baseline.json work/emumut-job.json

Only freshly compiled, installed, done functions in verify.tiers()'s diffed tier
are admitted. Candidates are lexical C++ edits, not an AST rewrite; compilation
failures and identical masked object code are excluded and recorded separately.
Classes are sampled round-robin, favouring new functions. Each emulation runs in
a short-lived subprocess, bounding Unicorn memory use. JSON embeds sources and
edits so replay compares the SAME corpus, even from another worktree.
"""
import argparse
import collections
import concurrent.futures as cf
import dataclasses
import hashlib
import importlib.util
import json
import os
from pathlib import Path
import random
import re
import struct
import subprocess
import sys
import tempfile
import time

ROOT = Path(__file__).resolve().parents[1]
sys.path.insert(0, str(ROOT / "tools"))
CLASSES = ("field_offset", "swapped_argument", "dropped_argument", "constant",
           "loop_bound", "inverted_condition", "dropped_condition", "dropped_call",
           "dropped_store", "wrong_callee", "signed_unsigned", "store_width",
           "stale_after_call", "dropped_return", "virtual_slot")
TOKEN = re.compile(r'//[^\n]*|/\*[\s\S]*?\*/|"(?:\\.|[^"\\])*"|\'(?:\\.|[^\'\\])*\'|'
                   r'0[xX][\da-fA-F]+[uUlL]*|\d+(?:\.\d*)?(?:[eE][+-]?\d+)?[fFuUlL]*|'
                   r'[A-Za-z_]\w*|->|::|<=|>=|==|!=|&&|\|\||\+\+|--|<<|>>|[^\s]')


@dataclasses.dataclass(frozen=True)
class Mutation:
    ea: str
    klass: str
    start: int
    end: int
    line: int
    before: str
    after: str

    def apply(self, source):
        if source[self.start:self.end] != self.before:
            raise ValueError("mutation does not match source")
        return source[:self.start] + self.after + source[self.end:]

    def record(self):
        d = dataclasses.asdict(self)
        d["class"] = d.pop("klass")
        d["id"] = hashlib.sha256(json.dumps(d, sort_keys=True).encode()).hexdigest()[:20]
        return d


def candidates(ea, source, neighbours=()):
    """Conservative token edits, excluding comments, literals and declarations.

    dropped_argument changes the call's ABI as well as removing an expression;
    stale_after_call snapshots an out-argument before a call and uses that value
    at the next call. These are single bug mutations despite spanning more than
    one token. wrong_callee keeps the binding/prototype but changes its address.
    """
    ts = [(m.group(), m.start(), m.end()) for m in TOKEN.finditer(source)
          if not m.group().startswith(('//', '/*', '"', "'"))]
    pair, stack = {}, []
    for i, (v, _, _) in enumerate(ts):
        if v in ('(', '[', '{'):
            stack.append(i)
        elif v in (')', ']', '}') and stack:
            j = stack.pop()
            if ts[j][0] == {')': '(', ']': '[', '}': '{'}[v]:
                pair[j] = i
    out = []

    def emit(klass, a, b, after):
        before = source[a:b]
        if before != after:
            out.append(Mutation(ea, klass, a, b, source.count('\n', 0, a) + 1, before, after))

    def text(a, b):
        return source[ts[a][1]:ts[b - 1][2]] if a < b else ''

    def args(i):
        end = pair.get(i)
        if end is None:
            return []
        parts, start, k = [], i + 1, i + 1
        while k < end:
            if ts[k][0] == ',':
                parts.append((start, k))
                start = k + 1
            elif k in pair:
                k = pair[k]
            k += 1
        if start < end:
            parts.append((start, end))
        return parts

    bound, protected = {}, set()
    for i, (v, _, _) in enumerate(ts):
        if v.startswith(('CSF_', 'HOOK')) and i + 1 in pair:
            end = pair[i + 1]
            protected.update(range(i, end + 1))
            ps = args(i + 1)
            if v == 'CSF_FUNC' and len(ps) == 5:
                name, conv, addr = text(*ps[2]), text(*ps[1]), text(*ps[3])
                bound[name] = (conv, addr)
                near = sorted(neighbours, key=lambda n: (abs(n - int(addr, 16)), n))
                near = [n for n in near if n != int(addr, 16)]
                if near:
                    a, b = ps[3]
                    emit('wrong_callee', ts[a][1], ts[b - 1][2], f'0x{near[0]:08X}')
            if v == 'CSF_AT' and len(ps) == 3:
                # The access must be in a function body (checked below).
                protected.difference_update(range(i, end + 1))

    # Body depth, ignoring braces in macros. Skip global bindings and signatures.
    depth, depths = 0, []
    for v, _, _ in ts:
        depths.append(depth)
        depth += (v == '{') - (v == '}')
    runtime_calls = []
    offset_tokens = set()
    for i, (v, a, b) in enumerate(ts):
        if not depths[i] or i in protected:
            continue
        if v == 'CSF_AT' and i + 1 in pair:
            ps, end = args(i + 1), pair[i + 1]
            if len(ps) == 3:
                x, y = ps[2]
                offset_tokens.update(range(x, y))
                off = text(x, y)
                if re.fullmatch(r'0[xX][\da-fA-F]+|\d+', off):
                    for delta in (-4, 4):
                        if int(off, 0) + delta >= 0:
                            emit('field_offset', ts[x][1], ts[y - 1][2], hex(int(off, 0) + delta))
                typ = text(*ps[0])
                # Legacy virtual dispatch: CSF_AT(void**, object, 0)[slot])(...).
                if typ.replace(' ', '') == 'void**' and end + 1 in pair and ts[end + 1][0] == '[':
                    slot_end = pair[end + 1]
                    follow = slot_end + 1
                    while follow < len(ts) and ts[follow][0] == ')':
                        follow += 1
                    if follow < len(ts) and ts[follow][0] == '(':
                        x, y = end + 2, slot_end
                        offset_tokens.update(range(x, y))
                        emit('virtual_slot', ts[x][1], ts[y - 1][2], f'({text(x, y)}) + 1')
                if end + 1 < len(ts) and ts[end + 1][0] == '=':
                    nt = {'uint32_t': 'uint8_t', 'int32_t': 'uint8_t', 'int': 'uint8_t',
                          'unsigned': 'uint8_t', 'uint8_t': 'uint32_t', 'char': 'uint32_t'}.get(typ)
                    if nt:
                        x, y = ps[0]
                        emit('store_width', ts[x][1], ts[y - 1][2], nt)
        if re.fullmatch(r'field_[\da-fA-F]+', v) and i >= 2 and ts[i - 1][0] == '->' and re.fullmatch(r'\w+', ts[i - 2][0]):
            base, off = ts[i - 2][0], int(v[6:], 16)
            for delta in (-4, 4):
                if off + delta >= 0:
                    emit('field_offset', ts[i - 2][1], b,
                         f'CSF_AT(decltype({base}->{v}), {base}, {hex(off + delta)})')
        if v in ('if', 'while', 'for') and i + 1 in pair:
            x, y = i + 2, pair[i + 1]
            if v == 'if':
                emit('inverted_condition', ts[x][1], ts[y - 1][2], f'!({text(x, y)})')
                emit('dropped_condition', ts[x][1], ts[y - 1][2], 'true')
            else:
                for k in range(x, y):
                    if ts[k][0] in ('<', '<=', '>', '>='):
                        nv = {'<': '<=', '<=': '<', '>': '>=', '>=': '>'}[ts[k][0]]
                        emit('loop_bound', ts[k][1], ts[k][2], nv)
        if v in bound and i + 1 in pair:
            ps, end = args(i + 1), pair[i + 1]
            runtime_calls.append((i, end, ps))
            av = [text(*p) for p in ps]
            for k in range(len(ps) - 1):
                swapped = av.copy()
                swapped[k], swapped[k + 1] = swapped[k + 1], swapped[k]
                emit('swapped_argument', a, ts[end][2], f'{v}({", ".join(swapped)})')
            conv, _ = bound[v]
            # Keep register arguments; dropping a stack argument changes cleanup/count.
            for k in range(2 if conv == '__fastcall' else 0, len(ps)):
                rest = av[:k] + av[k + 1:]
                signature = ', '.join(f'decltype({s})' for s in rest)
                call = source[a:ts[end][2]]
                emit('dropped_argument', a, ts[end][2],
                     f'reinterpret_cast<decltype({call}) ({conv}*)({signature})>({v})({", ".join(rest)})')
            if end + 1 < len(ts) and ts[end + 1][0] == ';' and ts[i - 1][0] in (';', '{', '}'):
                emit('dropped_call', a, ts[end][2], '(void)0')
        # Raw vtable indices. Typed vSlot wrappers are expanded with their deduced ABI.
        if v in ('vtbl', 'vtable', 'vftable') and i + 1 in pair and ts[i + 1][0] == '[':
            x, y = i + 2, pair[i + 1]
            offset_tokens.update(range(x, y))
            emit('virtual_slot', ts[x][1], ts[y - 1][2], f'({text(x, y)}) + 1')
        if re.fullmatch(r'vSlot[\da-fA-F]+', v) and i >= 2 and ts[i - 1][0] == '->' and ts[i - 2][0].isidentifier() and i + 1 in pair:
            base, end, ps = ts[i - 2][0], pair[i + 1], args(i + 1)
            av = [text(*p) for p in ps]
            call = source[ts[i - 2][1]:ts[end][2]]
            types = ', '.join([f'decltype({base})', 'void*'] + [f'decltype({s})' for s in av])
            vals = ', '.join([base, 'nullptr'] + av)
            slot = int(v[5:], 16) + 4
            emit('virtual_slot', ts[i - 2][1], ts[end][2],
                 f'reinterpret_cast<decltype({call}) (__fastcall*)({types})>({base}->vtbl[0x{slot:X} / 4])({vals})')
        if v == 'return':
            end = i + 1
            while end < len(ts) and ts[end][0] != ';':
                end = pair.get(end, end) + 1
            if end < len(ts) and end > i + 1:
                expr = text(i + 1, end)
                emit('dropped_return', ts[i + 1][1], ts[end - 1][2],
                     f'(static_cast<void>({expr}), decltype({expr}){{}})')

    # Whole memory-store statements, preserving RHS evaluation (including calls).
    for i, (v, a, b) in enumerate(ts):
        if v != '=' or not depths[i] or i in protected:
            continue
        start = i - 1
        while start >= 0 and ts[start][0] not in (';', '{', '}'):
            start -= 1
        start += 1
        end = i + 1
        while end < len(ts) and ts[end][0] != ';':
            end = pair.get(end, end) + 1
        lhs = text(start, i)
        if end < len(ts) and ('->' in lhs or 'CSF_AT' in lhs or '[' in lhs) and not re.search(r'\b(if|for|while|return)\b', lhs):
            rhs = text(i + 1, end)
            emit('dropped_store', ts[start][1], ts[end][2], f'(void)({rhs});')

    # Comparisons of simple values: cast both operands to the other signedness.
    simple = r'(?:\b\w+->field_[\da-fA-F]+|\b\w+|CSF_AT\([^()]+\))'
    clean = list(source)
    for m in TOKEN.finditer(source):
        if m.group().startswith(('//', '/*', '"', "'")):
            clean[m.start():m.end()] = ' ' * (m.end() - m.start())
    clean = ''.join(clean)
    integers = set(re.findall(r'\b(?:int|unsigned(?:\s+int)?|int32_t|uint32_t)\s+([A-Za-z_]\w*)\b', clean))

    def integer(expr):
        return expr in integers or bool(re.fullmatch(r'-?\d+', expr)) or bool(
            re.match(r'CSF_AT\(\s*(?:int|unsigned|int32_t|uint32_t)\s*,', expr))

    for m in re.finditer(rf'({simple})\s*(<=|>=|<|>)\s*({simple}|-?\d+)', clean):
        idx = next((i for i, t in enumerate(ts) if t[1] == m.start()), None)
        if idx is not None and depths[idx] and idx not in protected:
            lhs, op, rhs = m.groups()
            if not integer(lhs) or not integer(rhs):
                continue
            # Try both; compilation/equivalence control filters inapplicable edits.
            for typ in ('uint32_t', 'int32_t'):
                emit('signed_unsigned', m.start(), m.end(),
                     f'(static_cast<{typ}>({lhs}) {op} static_cast<{typ}>({rhs}))')

    for i, (v, a, b) in enumerate(ts):
        if depths[i] and i not in protected and i not in offset_tokens and re.fullmatch(r'(?:0[xX][\da-fA-F]+|\d+(?:\.\d*)?)[uUlLfF]*', v):
            # Array extents, offsets, addresses and slot numbers belong to their own classes.
            if i and ts[i - 1][0] == '[':
                continue
            emit('constant', a, b, f'({v} + 1)')

    for n, (i, end, ps) in enumerate(runtime_calls):
        # Require two standalone statements in one lexical block.
        if ts[i - 1][0] not in (';', '{', '}') or end + 1 >= len(ts) or ts[end + 1][0] != ';':
            continue
        for p in ps:
            var = text(*p)
            if not re.fullmatch(r'&\s*[A-Za-z_]\w*', var):
                continue
            var = var.lstrip('&').strip()
            for j, je, jps in runtime_calls[n + 1:]:
                between = source[ts[end + 1][2]:ts[j][1]]
                if any(c in between for c in '{}'):
                    break
                matching = [p for p in jps if text(*p) == var]
                if matching:
                    if re.search(rf'\b{re.escape(var)}\s*(?:=(?!=)|\+=|-=|\+\+|--)', between):
                        break  # a local computation, not a stale read across the out-call
                    x, y = matching[0]
                    before = source[ts[i][1]:ts[je + 1][2]] if je + 1 < len(ts) and ts[je + 1][0] == ';' else ''
                    if before:
                        rel_a, rel_b = ts[x][1] - ts[i][1], ts[y - 1][2] - ts[i][1]
                        edited = before[:rel_a] + 'emumut_saved' + before[rel_b:]
                        emit('stale_after_call', ts[i][1], ts[je + 1][2],
                             f'{{ auto emumut_saved = {var}; {edited} }}')
                    break
    # Canonical order, no duplicate sites/replacements.
    return sorted(set(out), key=lambda m: (CLASSES.index(m.klass), m.start, m.end, m.after))


def code_fingerprint(path):
    """COFF code bytes with relocation fields masked; omit HOOK registration.

    Include relocation kind, target and addend so changing an external callee or
    referenced datum is not accidentally called equivalent. Include readonly
    data too: identical instructions can still load a mutated float constant.
    Debug info and registration/name strings do not affect the hooked function.
    """
    d = Path(path).read_bytes()
    nsec, _, symoff, nsym = struct.unpack_from('<HIII', d, 2)
    strtab = symoff + 18 * nsym

    def name(raw):
        if raw[:4] == bytes(4):
            off = struct.unpack_from('<I', raw, 4)[0]
            return d[strtab + off:].split(b'\0', 1)[0].decode('latin1')
        return raw.rstrip(b'\0').decode('latin1')

    syms, section_functions = {}, collections.defaultdict(list)
    i = 0
    while i < nsym:
        raw, val, sec, typ, scl, naux = struct.unpack_from('<8sIhHBB', d, symoff + 18 * i)
        nm = name(raw)
        syms[i] = (nm, val, sec)
        if sec > 0 and typ & 0x20:
            section_functions[sec].append((val, nm))
        i += 1 + naux
    parts = []
    for sec in range(1, nsec + 1):
        raw, _, _, size, ptr, relptr, _, nrel, _, flags = struct.unpack_from('<8sIIIIIIHHI', d, 20 + 40 * (sec - 1))
        nm = raw.rstrip(b'\0').decode('latin1')
        if nm.startswith('/'):
            nm = d[strtab + int(nm[1:]):].split(b'\0', 1)[0].decode('latin1')
        if not flags & 0x20:
            continue
        funcs = sorted(set(section_functions[sec])) or [(0, nm)]
        for j, (start, fn) in enumerate(funcs):
            end = funcs[j + 1][0] if j + 1 < len(funcs) else size
            if fn.startswith(('??__E', '??__F', '__GLOBAL__sub_I_')):
                continue
            data, rels = bytearray(d[ptr + start:ptr + end]), []
            for k in range(nrel):
                off, idx, typ = struct.unpack_from('<IIH', d, relptr + 10 * k)
                if not start <= off < end:
                    continue
                target, value, target_sec = syms[idx]
                off -= start
                addend = bytes(data[off:off + 4]).hex()
                data[off:off + 4] = bytes(4)
                payload = None
                if target_sec > 0:
                    hdr = 20 + 40 * (target_sec - 1)
                    rsize, rptr = struct.unpack_from('<II', d, hdr + 16)
                    tflags = struct.unpack_from('<I', d, hdr + 36)[0]
                    if not tflags & 0x20:
                        payload = d[rptr + value:rptr + rsize].hex()
                    else:
                        value = 0  # placement in .text is not behaviour
                rels.append((off, typ, target, value, addend, payload))
            parts.append((bytes(data).hex(), rels))
    if not parts:
        raise ValueError('object has no function code')
    return hashlib.sha256(json.dumps(parts, sort_keys=True).encode()).hexdigest()


def compile_source(source, obj, command):
    r = subprocess.run(command + [f'/Fo{obj}', '-c', '--', str(source)],
                       cwd=ROOT / 'build', capture_output=True, text=True, timeout=60)
    return None if r.returncode == 0 else (r.stdout + r.stderr)[-3000:]


def build_mutant(m, source, directory, command, baseline_fingerprint):
    """Compile once, then exclude equivalence before any emulation/scoring."""
    rec = m.record()
    src = Path(directory) / f'm{rec["id"]}.cpp'
    obj = src.with_suffix('.obj')
    src.write_text(m.apply(source))
    error = compile_source(src, obj, command)
    if error:
        return dict(rec, exclude='compile_error', detail=error), src, obj
    fingerprint = code_fingerprint(obj)
    if fingerprint == baseline_fingerprint:
        return dict(rec, exclude='equivalent', fingerprint=fingerprint), src, obj
    return dict(rec, fingerprint=fingerprint), src, obj


def worker(emudiff_path, ea, obj, source):
    # Bind data to this checkout even if the Python implementation is in a job tree.
    spec = importlib.util.spec_from_file_location('emumut_target', emudiff_path)
    module = importlib.util.module_from_spec(spec)
    spec.loader.exec_module(module)
    sys.path.insert(0, str(ROOT / 'tools'))  # packet/api data belongs to this corpus
    module.ROOT, module.EXE = str(ROOT), str(ROOT / 'work/pc/CommXPC.exe')
    module.OBJ_DIR = str(ROOT / 'build/CMakeFiles/csf_hook.dir/src/auto')
    try:
        verdict, detail = module.check(int(ea, 16), obj_path=obj, src=source)
        result = dict(verdict=verdict, detail=detail)
    except Exception as e:
        result = dict(verdict='error', detail=f'{type(e).__name__}: {e}')
    print(json.dumps(result))


def emulate(path, ea, obj, source):
    try:
        r = subprocess.run([sys.executable, str(Path(__file__).resolve()), '_worker',
                            str(path), ea, str(obj), str(source)],
                           capture_output=True, text=True, timeout=100)
        if r.returncode:
            return dict(verdict='error', detail=(r.stdout + r.stderr)[-2000:])
        return json.loads(r.stdout.splitlines()[-1])
    except (subprocess.TimeoutExpired, ValueError, IndexError) as e:
        return dict(verdict='error', detail=str(e))


def outcome(verdict):
    return {'DIFF': 'caught', 'same': 'missed', 'inconclusive': 'inconclusive', 'error': 'error'}[verdict]


def schedule(pools, n, seed):
    """Stable class round-robin, favouring unused functions; never mutate pools."""
    rng = random.Random(seed)
    pools = {k: list(pools.get(k, ())) for k in CLASSES}
    for pool in pools.values():
        rng.shuffle(pool)
    used, mutations = set(), []
    for _ in range(max(n, len(CLASSES))):
        for klass in CLASSES:
            pool = pools[klass]
            if not pool:
                continue
            idx = next((i for i, m in enumerate(pool) if m.ea not in used), len(pool) - 1)
            m = pool.pop(idx)
            used.add(m.ea)
            mutations.append(m)
        if len(mutations) >= n:
            break
    return mutations[:n]


def table(data):
    totals = collections.Counter()
    print('class                  caught missed inconclusive error  kill/all')
    for klass in CLASSES:
        c = collections.Counter(m['outcome'] for m in data['mutants'] if m['class'] == klass)
        totals.update(c)
        n = c.total()
        print(f'{klass:23} {c["caught"]:5} {c["missed"]:6} {c["inconclusive"]:12} {c["error"]:5} '
              + (f'{100*c["caught"]/n:8.1f}%' if n else '       -'))
    n = totals.total()
    print(f'{"TOTAL":23} {totals["caught"]:5} {totals["missed"]:6} {totals["inconclusive"]:12} {totals["error"]:5} '
          + (f'{100*totals["caught"]/n:8.1f}%' if n else '-'))
    originals = collections.Counter(m['verdict'] for m in data['originals'].values())
    print(f'{len({m["ea"] for m in data["mutants"]})} mutated functions; '
          f'{len(data["originals"])} originals {dict(originals)}; {len(data["excluded"])} excluded candidates; '
          f'{data.get("elapsed_seconds", 0):.1f}s')
    confounded = sum(m['outcome'] == 'caught' and data['originals'][m['ea']]['verdict'] != 'same'
                     for m in data['mutants'])
    if confounded:
        print(f'CAUTION: {confounded} caught verdicts have non-same original controls; not evidence of mutant detection')
    missing = [k for k in CLASSES if not any(m['class'] == k for m in data['mutants'])]
    if missing:
        print('NOT COVERED: ' + ', '.join(missing))


def run(a):
    import gate
    import verify
    started = time.monotonic()
    target = Path(a.emudiff).resolve()
    if not target.is_file():
        raise ValueError(f'no emudiff implementation: {target}')
    command = gate.compile_cmd()
    data = dict(schema=1, seed=a.seed, requested=a.n, emudiff=str(target),
                emudiff_sha256=hashlib.sha256(target.read_bytes()).hexdigest(),
                compile_command=command, originals={}, mutants=[], excluded=[], sources={})
    data['inputs_sha256'] = {p: hashlib.sha256((ROOT / p).read_bytes()).hexdigest() for p in
                             ('work/pc/CommXPC.exe', 'db/api.json', 'db/functions.csv', 'export/calls.json')}
    if a.replay:
        prior = json.loads(Path(a.replay).read_text())
        data['replay'] = str(Path(a.replay).resolve())
        data['seed'], data['requested'] = prior['seed'], len(prior['mutants'])
        sources = prior['sources']
        mutations = [Mutation(m['ea'], m['class'], m['start'], m['end'], m['line'], m['before'], m['after'])
                     for m in prior['mutants']]
    else:
        led, rows = verify.load(), verify.done_rows()
        sources = {}
        for ea, e in sorted(led.items()):
            src = ROOT / 'src/auto' / f'{ea}.cpp'
            if e.get('ok') and e.get('diffed') and not e.get('mismatched') and ea in rows and src.is_file():
                sources[ea] = src.read_text()
        neighbours = sorted(int(ea, 16) for ea in rows)
        pools = {k: [] for k in CLASSES}
        for ea, source in sources.items():
            for m in candidates(ea, source, neighbours):
                pools[m.klass].append(m)
        # Extra candidates replace uncompilable, stale, and equivalent edits.
        mutations = schedule(pools, a.n * 12, a.seed)
        print(f'{len(sources)} ledger candidates; {len(mutations)} mutation candidates', flush=True)

    with tempfile.TemporaryDirectory(prefix='emumut-') as tmp, cf.ThreadPoolExecutor(a.jobs) as ex:
        tmp = Path(tmp)
        original_cache = {}

        def original(ea):
            src, obj = tmp / f'{ea}.cpp', tmp / f'{ea}.obj'
            src.write_text(sources[ea])
            error = compile_source(src, obj, command)
            if error:
                return dict(exclude='original_compile_error', detail=error)
            h = verify.code_hash(str(obj))
            if not a.replay and verify.tiers({ea: h}).get(ea) != 'diffed':
                return dict(exclude='stale_evidence', code_hash=h)
            fingerprint = code_fingerprint(obj)
            result = emulate(target, ea, obj, src)
            return dict(result, code_hash=h, fingerprint=fingerprint, source_sha256=hashlib.sha256(sources[ea].encode()).hexdigest())

        def mutant(m):
            rec = m.record()
            base = original_cache[m.ea]
            if 'exclude' in base:
                return dict(rec, exclude=base['exclude'])
            if not a.replay and base['verdict'] != 'same':
                return dict(rec, exclude='baseline_' + base['verdict'], detail=base['detail'])
            rec, src, obj = build_mutant(m, sources[m.ea], tmp, command, base['fingerprint'])
            if 'exclude' in rec:
                return rec
            result = emulate(target, m.ea, obj, src)
            return dict(rec, **result, outcome=outcome(result['verdict']))

        # Include rejected-original controls on replay too: a variant may turn an
        # old inconclusive/DIFF control into same, even without an accepted mutant.
        if a.replay:
            eas = sorted(prior['originals'])
            for ea, result in zip(eas, ex.map(original, eas)):
                original_cache[ea] = result
                if 'exclude' not in result:
                    data['originals'][ea] = result
                    data['sources'][ea] = sources[ea]

        cursor = 0
        while cursor < len(mutations) and (a.replay or len(data['mutants']) < a.n):
            batch = mutations[cursor:cursor + min(a.jobs, data['requested'] - len(data['mutants']))]
            if not batch:
                break
            eas = sorted({m.ea for m in batch} - original_cache.keys())
            for ea, result in zip(eas, ex.map(original, eas)):
                original_cache[ea] = result
                if 'exclude' not in result:
                    data['originals'][ea] = result
                    data['sources'][ea] = sources[ea]
            for rec in ex.map(mutant, batch):
                data['excluded' if 'exclude' in rec else 'mutants'].append(rec)
            cursor += len(batch)
            if len(data['mutants']) // 40 != (len(data['mutants']) - len(batch)) // 40:
                print(f'{len(data["mutants"])}/{data["requested"]} mutants, '
                      f'{len(data["originals"])} functions, {time.monotonic()-started:.1f}s', flush=True)
        data['elapsed_seconds'] = round(time.monotonic() - started, 3)
        data['complete'] = len(data['mutants']) == data['requested']
        Path(a.out).parent.mkdir(parents=True, exist_ok=True)
        Path(a.out).write_text(json.dumps(data, indent=2) + '\n')
    table(data)
    if not data['complete']:
        raise ValueError(f'corpus incomplete: {len(data["mutants"])}/{data["requested"]}; see exclusions')


def compare(a, b):
    left, right = [json.loads(Path(p).read_text()) for p in (a, b)]
    for label, d in ((a, left), (b, right)):
        print(label)
        table(d)
    ls, rs = [{m['id']: m for m in d['mutants']} for d in (left, right)]
    shared = ls.keys() & rs.keys()
    print(f'paired mutants: {len(shared)}; A only: {len(ls.keys()-rs.keys())}; B only: {len(rs.keys()-ls.keys())}')
    if left.get('inputs_sha256') != right.get('inputs_sha256'):
        print('CAUTION: input hashes differ or were not recorded; check corpus data before interpreting rates')
    if any(ls[k]['fingerprint'] != rs[k]['fingerprint'] or
           left['originals'][ls[k]['ea']]['fingerprint'] != right['originals'][rs[k]['ea']]['fingerprint'] for k in shared):
        raise ValueError('paired corpus object code changed; comparison is not controlled')
    for k in sorted(shared):
        m, n = ls[k], rs[k]
        if m['outcome'] == 'caught' and n['outcome'] != 'caught':
            print(f'LOST {m["ea"]} {m["class"]} line {m["line"]}: caught -> {n["outcome"]}')
            print(f'  {m["before"]!r} -> {m["after"]!r}')
            print(f'  A: {m["detail"]}; B: {n["detail"]}')
    for k in sorted(ls.keys() - rs.keys()):
        m = ls[k]
        if m['outcome'] == 'caught':
            excluded = next((x for x in right['excluded'] if x['id'] == k), None)
            why = excluded['exclude'] if excluded else 'absent from B corpus'
            print(f'UNPAIRED A-CAUGHT {m["ea"]} {m["class"]} line {m["line"]}: {why}')
            print(f'  {m["before"]!r} -> {m["after"]!r}; replay A to measure this mutant in B')
    for ea in sorted(left['originals'].keys() & right['originals'].keys()):
        x, y = left['originals'][ea], right['originals'][ea]
        if x['verdict'] != y['verdict']:
            print(f'ORIGINAL {ea}: {x["verdict"]} -> {y["verdict"]}; {x["detail"]} -> {y["detail"]}')


def main():
    if len(sys.argv) > 1 and sys.argv[1] == '_worker':
        worker(*sys.argv[2:])
        return
    ap = argparse.ArgumentParser(description=__doc__, formatter_class=argparse.RawDescriptionHelpFormatter)
    sub = ap.add_subparsers(dest='cmd', required=True)
    r = sub.add_parser('run')
    r.add_argument('--n', type=int, default=400)
    r.add_argument('--seed', type=int, default=1)
    r.add_argument('--emudiff', default=str(ROOT / 'tools/emudiff.py'))
    r.add_argument('--jobs', type=int, default=8)
    r.add_argument('--out', required=True)
    r.add_argument('--replay', help='rerun the exact embedded corpus in an earlier result')
    c = sub.add_parser('compare')
    c.add_argument('a')
    c.add_argument('b')
    a = ap.parse_args()
    try:
        if a.cmd == 'run':
            if a.n < 1 or a.jobs < 1:
                ap.error('--n and --jobs must be positive')
            run(a)
        else:
            compare(a.a, a.b)
    except (OSError, ValueError, subprocess.TimeoutExpired) as e:
        ap.exit(1, f'emumut: {e}\n')


if __name__ == '__main__':
    main()
