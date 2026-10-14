#!/usr/bin/env python3
"""Generate standalone data, NOT a link or a runnable standalone executable.

Output lives in build/datagen (generated inputs, never hand-maintained src/). See
docs/DATAGEN.md for layout, the stripped-relocation limitation and startup contract.
Default: confirmed fixups, words equal to a known item's start, and words into the data sections;
--candidate-fixups additionally relocates heuristic
aligned image-address words. Neither mode claims a complete relocation inventory
when the PE is stripped and the export has no data-to-data xrefs.
"""
import argparse
import collections
import csv
import hashlib
import json
from pathlib import Path
import re
import struct
import subprocess
import sys
import time

import pefile
import yaml

ROOT = Path(__file__).resolve().parent.parent
sys.path.insert(0, str(ROOT / 'tools'))


def number(x):
    return int(x, 16) if isinstance(x, str) else x


def dump(path, value):
    path.write_text(json.dumps(value, indent=1) + '\n')


class Image:
    def __init__(self, path):
        self.pe = pefile.PE(str(path))
        assert self.pe.OPTIONAL_HEADER.Magic == 0x10b, 'only PE32 supported'
        self.base = self.pe.OPTIONAL_HEADER.ImageBase
        self.sections = []
        for s in self.pe.sections:
            self.sections.append(dict(name=s.Name.rstrip(b'\0').decode(),
                                      start=self.base + s.VirtualAddress,
                                      size=max(s.Misc_VirtualSize, s.SizeOfRawData),
                                      virtual_size=s.Misc_VirtualSize,
                                      raw_size=s.SizeOfRawData,
                                      executable=bool(s.Characteristics & 0x20000000)))
        data = [s for s in self.sections if not s['executable']]
        self.start = min(s['start'] for s in data)
        self.end = max(s['start'] + s['size'] for s in data)
        self.bytes = bytearray(self.end - self.start)
        for s, meta in zip(self.pe.sections, self.sections):
            if not meta['executable']:
                raw = s.get_data()[:meta['raw_size']]
                off = meta['start'] - self.start
                self.bytes[off:off + len(raw)] = raw

    def section(self, addr):
        return next((s for s in self.sections if s['start'] <= addr < s['start'] + s['size']), None)

    def data(self, addr):
        s = self.section(addr)
        return s is not None and not s['executable']

    def code(self, addr):
        s = self.section(addr)
        return s is not None and s['executable']

    def word(self, addr):
        return struct.unpack_from('<I', self.bytes, addr - self.start)[0]


def definitions(text):
    """Check both required coverage and absence of duplicates in generated source."""
    return collections.Counter((k, int(a, 16)) for k, a in re.findall(
        r'template <> unsigned char\* const (gv|vt)<0x([0-9A-F]+)>::[dt]', text))


def check_object(obj, required):
    from linktest import symbols, demangle, SLOT
    defined, _ = symbols(str(obj))
    counts = collections.Counter()
    for symbol, name in demangle(sorted(defined)).items():
        m = SLOT.search(name)
        if m and m[1] in ('gv', 'vt'):
            counts[m[1], int(m[2])] += 1
    assert counts == collections.Counter({k: 1 for k in required}), 'object definition coverage mismatch'


def text_shaped(w):
    """The 4 bytes of w read as the end or middle of a C string: printable bytes, then NULs only."""
    b = struct.pack('<I', w)
    n = b.find(b'\0')
    body, tail = (b, b'') if n < 0 else (b[:n], b[n:])
    return len(body) >= 1 and all(32 <= c < 127 for c in body) and not tail.strip(b'\0')


def string_words(img, pointers, slots):
    """Words that are text although they read as code addresses, so they stay raw. "png\0" at 0x8631BC is
    0x00676E70, a NetZ function's start, and IDA made it an offset; translated, RwImageRegisterImageFormat got
    garbage for its extension and every PNG read failed (crash1). Two sources: (1) a text-shaped word that continues
    a printable string begun at least 4 bytes before it (".fli" in "Menus.fli", "Pos" in "...istanciaPos"): a
    pointer table entry is never preceded by text; (2) db/datagen_text.yaml, short strings of their own checked by
    hand ("bmp", "png"). A pointer-table entry such as "LfI\0" at 0x82D060 looks the same as "png\0" locally,
    so (1) does not guess at those. Vtable slots never are text."""
    listed = {number(r['address']) for r in (yaml.safe_load((ROOT / 'db/datagen_text.yaml').read_text()) or [])}
    text = set(listed)
    for a, (t, _) in pointers.items():
        if a in slots or not img.code(t) or not text_shaped(img.word(a)):
            continue
        before = bytes(img.bytes[a - img.start - 4:a - img.start])
        if len(before) == 4 and all(32 <= c < 127 for c in before):
            text.add(a)
    return text


def generate(args):
    t0 = time.monotonic()
    # Compiler runs from build/; custom relative output paths must still denote
    # the same files as the generator (which runs from the caller's cwd).
    args.output = args.output.resolve()
    args.report = args.report.resolve()
    img = Image(args.exe)
    linked = json.loads(args.linktest.read_text())
    globals_ = [number(a) for a in linked['globals']]
    vtables = [number(a) for a in linked['vtables']]
    required = {('gv', a) for a in globals_} | {('vt', a) for a in vtables}
    assert len(required) == len(globals_) + len(vtables), 'duplicate linktest entries'
    rejected = [dict(kind=k, address=hex(a), reason='outside non-executable image sections',
                     section=img.section(a)) for k, a in sorted(required) if not img.data(a)]
    # Do not invent storage at an unmapped address or present machine code as a
    # vtable. Emit useful partial output and a nonzero exit for this input error.
    required = {k for k in required if img.data(k[1])}
    globals_ = [a for a in globals_ if ('gv', a) in required]
    vtables = [a for a in vtables if ('vt', a) in required]

    with (ROOT / 'db/functions.csv').open() as f:
        funcs = {number(r['ea']): r for r in csv.DictReader(f)}
    regions = yaml.safe_load((ROOT / 'db/regions.yaml').read_text())
    from api import strip_comments
    # api.scan() supplies signatures, but deliberately omits HOOK_TAIL/REGS;
    # those DO define fn_slot storage. Scan every src/ TU, not just src/auto.
    impls = set()
    hook = re.compile(r'\b(HOOK(?:_[A-Z_]+)?)\(\s*(0[xX][0-9a-fA-F]+)\s*,')
    # Prefer actual COFF coverage when supplied by the first-link target.
    # Source regexes cannot know whether a HOOK is inside a hook-only #ifdef.
    if 'defined_functions' in linked:
        impls = {number(a) for a in linked['defined_functions']}
    else:
        for path in (ROOT / 'src').rglob('*.cpp'):
            for macro, a in hook.findall(strip_comments(path.read_text())):
                if macro != 'HOOK_VSLOT':
                    impls.add(number(a))
    strings = {number(a) for a in json.loads((ROOT / 'export/strings.json').read_text())}
    anchors = set(globals_) | strings
    with (ROOT / 'export/functions.jsonl').open() as stream:
        for line in stream:
            f = json.loads(line)
            anchors.update(f.get('drefs', []))
            anchors.update(f.get('srefs', []))

    def region(a):
        # CSV also knows RWA (not represented in the coarse regions.yaml).
        if a in funcs:
            return funcs[a]['region']
        r = 'unknown'
        for row in regions:
            if row['start'] <= a < row['end']:
                r = row['lib'] or 'game'
        return r

    known_vts = {number(v['addr']): v for v in json.loads((ROOT / 'db/vtables.json').read_text())}
    starts = sorted(set(known_vts) | set(vtables))
    slots = {}
    vt_rows = []
    for idx, a in enumerate(starts):
        assert img.data(a), f'vtable outside data: {a:#x}'
        if a in known_vts:
            targets = [number(t) for t in known_vts[a]['slots']]
        else:
            # Missing exported table: run of executable-address pointers, bounded
            # by the next table start / section end. Includes interior table aliases.
            sec = img.section(a)
            end = min(sec['start'] + sec['size'], starts[idx + 1] if idx + 1 < len(starts) else img.end)
            targets = []
            p = a
            while p + 4 <= end:
                target = img.word(p)
                if not img.code(target):
                    if target != 0:
                        break
                    q = p + 4
                    while q + 4 <= end and img.word(q) == 0:
                        q += 4
                    if q + 4 > end or not img.code(img.word(q)):
                        break
                targets.append(target)
                p += 4
        if not targets:
            rejected.append(dict(kind='vt', address=hex(a), reason='no function slots recovered'))
            required.discard(('vt', a))
            vtables = [v for v in vtables if v != a]
            continue
        for i, target in enumerate(targets):
            source = a + 4 * i
            assert img.word(source) == target, f'vtable export differs from image at {source:#x}'
            if target == 0:
                continue  # null virtual slots remain zero; never bind fn_slot<0>
            assert img.code(target), f'non-code vtable slot at {source:#x}'
            assert source not in slots or slots[source] == target
            slots[source] = target
        vt_rows.append(dict(address=hex(a), referenced=a in vtables,
                            provenance='db/vtables.json' if a in known_vts else 'binary-function-run',
                            slots=[hex(t) for t in targets]))

    confirmed = {a: (t, 'vtable') for a, t in slots.items()}
    reloc = img.pe.OPTIONAL_HEADER.DATA_DIRECTORY[5]
    stripped = not (reloc.VirtualAddress and reloc.Size)
    if not stripped:
        for block in img.pe.DIRECTORY_ENTRY_BASERELOC:
            for entry in block.entries:
                a = img.base + entry.rva
                if entry.type == 0 or not img.data(a):
                    continue
                assert entry.type == 3, f'unsupported relocation type {entry.type}'
                confirmed[a] = (img.word(a), 'pe-HIGHLOW')
    # Optional read-only export of actual pointer source locations. Existing
    # export/disasm drefs have INSTRUCTION sources and cannot provide this.
    if args.data_refs:
        for row in json.loads(args.data_refs.read_text()):
            a, target = number(row['source']), number(row['target'])
            assert img.data(a) and a + 4 <= img.end
            assert img.word(a) == target, f'data xref/image mismatch: {a:#x}'
            confirmed[a] = (target, 'data-xref')

    candidates = {}
    for s in img.sections:
        if s['executable']:
            continue
        for a in range(s['start'], s['start'] + s['raw_size'] - 3, 4):
            target = img.word(a)
            if img.section(target):
                candidates[a] = (target, 'aligned-word-known-target' if target in anchors or target in funcs
                                 else 'aligned-word-image-range')
    pointers = {**candidates, **confirmed}
    textwords = string_words(img, pointers, slots)

    virtual_targets = set(slots.values())

    def classify(target):
        if img.code(target):
            return 'function' if target in funcs or target in virtual_targets else 'code-interior'
        if target in slots:
            return 'vtable'
        if target in strings:
            return 'string'
        if img.data(target):
            return 'data'
        return 'external'

    counts, confirmed_counts, library, unconfirmed = (collections.Counter() for _ in range(4))
    rows, missing, fixes = [], [], []
    for a, (target, provenance) in sorted(pointers.items()):
        kind = classify(target)
        reg = region(target) if img.code(target) else None
        is_lib = reg not in (None, 'game', 'unknown')
        # An aligned word equal to the start of a known string, function or data item is a pointer in practice
        # (0x8A66C8: a language table whose name pointers IDA has as plain dwords; left raw, the first read of one
        # crashed the standalone run). So is a word inside the original's data sections: the same table's 2-letter
        # language codes ("CH", "EK") are strings IDA never made. As integers such words would be 8.5-9.2 million,
        # as floats denormals: rare in this data. Words into the middle of code stay raw unless --candidate-fixups.
        applied = (a in confirmed or provenance == 'aligned-word-known-target' or args.candidate_fixups
                   or (provenance == 'aligned-word-image-range' and not img.code(target)))
        if a in textwords:  # text that reads as a code address, IDA's data xrefs included (string_words)
            applied, provenance = False, provenance + '+text'
        row = dict(source=hex(a), target=hex(target), kind=kind, provenance=provenance,
                   confirmed=a in confirmed, applied=applied, region=reg,
                   implemented=target in impls if img.code(target) else None)
        rows.append(row)
        counts[kind] += 1
        (confirmed_counts if a in confirmed else unconfirmed)[kind] += 1
        if is_lib:
            library[reg] += 1
        if img.code(target) and (is_lib or target not in impls):
            missing.append(row)
        if not applied:
            continue
        off = a - img.start
        if img.code(target):
            expr = f'fn_slot<0x{target:08X}>::p'
        elif img.data(target):
            expr = f'image + {target - img.start}'
        else:
            raise ValueError(f'cannot relocate external target {target:#x}')
        fixes.append(f'    put({off}, {expr});')

    args.output.mkdir(parents=True, exist_ok=True)
    args.report.parent.mkdir(parents=True, exist_ok=True)
    src = ['// Generated by tools/datagen.py; see docs/DATAGEN.md. DO NOT EDIT.',
           '#include <windows.h>', '#include "globals.h"', '#include <cstdio>', '#include <cstring>',
           'static_assert(sizeof(void*) == 4, "original data layout is PE32");',
           'namespace csf {', 'namespace {',
           f'alignas(4096) unsigned char image[{len(img.bytes)}] = {{']
    # Omit the trailing zero-fill (the linker supplies it), including original BSS.
    initialized = len(img.bytes.rstrip(b'\0'))
    for off in range(0, initialized, 32):
        src.append('    ' + ','.join(f'0x{b:02X}' for b in img.bytes[off:off + 32]) + ',')
    src += ['};', 'void put(size_t offset, const void* value) {',
            '    std::memcpy(image + offset, &value, sizeof(value));', '}']
    # The original's import table (IAT) is part of the copied data, but holds the file's name offsets, not
    # addresses: game code calls Windows through these slots (CSF_GLOBAL on an IAT address), so bind every one.
    src += ['struct Import { size_t offset; const char* dll; const char* name; unsigned short ordinal; };',
            'const Import imports[] = {']
    for d in img.pe.DIRECTORY_ENTRY_IMPORT:
        for i in d.imports:
            name = f'"{i.name.decode()}"' if i.name else 'nullptr'
            src.append(f'    {{{i.address - img.start}, "{d.dll.decode()}", {name}, {i.ordinal or 0}}},')
    src += ['};', 'void bind_imports() {', '    for (const Import& i : imports) {',
            '        HMODULE m = LoadLibraryA(i.dll);',
            '        FARPROC p = !m ? nullptr : GetProcAddress(m, i.name ? i.name : MAKEINTRESOURCEA(i.ordinal));',
            '        if (!p) std::fprintf(stderr, "csf: import %s!%s not found\\n", i.dll, i.name ? i.name : "#");',
            '        put(i.offset, reinterpret_cast<const void*>(p));', '    }', '}', '}']
    for k, a in sorted(required):
        src.append(f'template <> unsigned char* const {k}<0x{a:08X}>::{"d" if k == "gv" else "t"} = image + {a - img.start};')
    src += ['void initialize_data() {', '    static bool done = false;',
            '    if (done) return;', *fixes, '    bind_imports();', '    done = true;', '}', '} // namespace csf', '']
    text = '\n'.join(src)
    assert definitions(text) == collections.Counter({k: 1 for k in required})
    cpp = args.output / 'data.cpp'
    cpp.write_text(text)
    dump(args.output / 'manifest.json', dict(globals=[hex(a) for a in globals_],
                                           vtables=[hex(a) for a in vtables],
                                           image_base=hex(img.start), image_size=len(img.bytes),
                                           image_sha256=hashlib.sha256(img.bytes).hexdigest()))
    dump(args.report.with_name('datagen-pointers.json'), rows)
    dump(args.report.with_name('datagen-unresolved.json'), missing)
    vt_missing = [r for r in missing if number(r['source']) in slots]
    dump(args.report.with_name('datagen-vtable-unresolved.json'), vt_missing)
    dump(args.report.with_name('datagen-vtables.json'), vt_rows)
    dump(args.report.with_name('datagen-rejected.json'), rejected)
    summary = dict(relocations_stripped=stripped, image_base=hex(img.start), image_bytes=len(img.bytes),
                   implementation_inventory='COFF' if 'defined_functions' in linked else 'source-HOOK-scan',
                   sections=img.sections, globals=len(globals_), vtables=len(vtables),
                   vtable_unique_slots=len(slots), pointer_counts=dict(counts),
                   confirmed_pointer_counts=dict(confirmed_counts), unconfirmed_pointer_counts=dict(unconfirmed),
                   library_pointer_counts=dict(library), unresolved_pointer_sites=len(missing),
                   unresolved_function_targets=len({r['target'] for r in missing}),
                   vtable_unresolved_sites=len(vt_missing),
                   vtable_unresolved_targets=len({r['target'] for r in vt_missing}),
                   vtable_library_counts=dict(collections.Counter(r['region'] for r in vt_missing
                       if r['region'] not in ('game', 'unknown'))),
                   fixups=len(fixes), candidate_fixups=args.candidate_fixups,
                   relocation_inventory_complete=not stripped, definition_check='ok',
                   all_linktest_definitions=not rejected, rejected_references=len(rejected),
                   warning='Stripped PE: supplied IDA pointer sites are fixed, but inventory completeness '
                           'is not certified; aligned words are candidates, NOT proven pointers. '
                           'Other (including unaligned) pointers may be missed.'
                           if stripped else None)
    if args.compile_check:
        from linktest import flags
        obj = args.output / 'data.obj'
        r = subprocess.run(flags() + [f'/Fo{obj}', '-c', '--', str(cpp)],
                           cwd=ROOT / 'build', capture_output=True, text=True)
        if r.returncode:
            raise RuntimeError(r.stdout + r.stderr)
        check_object(obj, required)
        summary['object_definition_check'] = 'ok'
    summary['seconds'] = round(time.monotonic() - t0, 3)
    dump(args.report, summary)
    print(json.dumps(summary, indent=1))
    return summary


def main():
    ap = argparse.ArgumentParser(description=__doc__)
    ap.add_argument('--exe', type=Path, default=ROOT / 'work/pc/CommXPC.exe')
    ap.add_argument('--linktest', type=Path, default=ROOT / 'work/linktest.json')
    ap.add_argument('--output', type=Path, default=ROOT / 'build/datagen')
    ap.add_argument('--report', type=Path, default=ROOT / 'work/datagen.json')
    ap.add_argument('--data-refs', type=Path, help='read-only JSON list of {source,target} absolute addresses')
    ap.add_argument('--candidate-fixups', action='store_true', help='UNSAFE: also relocate unproven aligned words')
    ap.add_argument('--compile-check', action='store_true', help='compile standalone and check COFF gv/vt symbols')
    ap.add_argument('--allow-rejected', action='store_true',
                    help='first-link diagnostic only: emit partial data and let the linker report rejected bindings')
    args = ap.parse_args()
    if not args.data_refs and (ROOT / 'export/dataptrs.json').exists():  # tools/ida/export_dataptrs.py
        args.data_refs = ROOT / 'export/dataptrs.json'
    summary = generate(args)
    if summary['rejected_references'] and args.allow_rejected:
        print(f"WARNING: {summary['rejected_references']} data bindings deliberately NOT defined; "
              'see datagen-rejected.json and final linker errors', file=sys.stderr)
    return 2 if summary['rejected_references'] and not args.allow_rejected else 0


if __name__ == '__main__':
    sys.exit(main())
