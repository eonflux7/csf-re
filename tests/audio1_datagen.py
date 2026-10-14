"""Audit the audio-init ASLR defect and exercise its proposed text override.

Uses an in-memory override because workers cannot edit db/datagen_text.yaml.
No Windows execution. Run after the standalone build (uses its link map).
"""
import contextlib
import io
import json
import pathlib
import re
import struct
import sys
import tempfile
from types import SimpleNamespace

ROOT = pathlib.Path(__file__).resolve().parents[1]
sys.path.insert(0, str(ROOT / 'tools'))
import datagen

WIN = 0x85ec30
image = datagen.Image(ROOT / 'work/pc/CommXPC.exe')
assert image.word(WIN) == int.from_bytes(b'win\0', 'little')
assert image.word(0x8a6ba4) == WIN
original = datagen.string_words
with tempfile.TemporaryDirectory() as temp:
    def generate(label):
        out = pathlib.Path(temp) / label
        args = SimpleNamespace(exe=ROOT / 'work/pc/CommXPC.exe',
                               linktest=ROOT / 'work/linktest.json', output=out,
                               report=out / 'report.json', data_refs=ROOT / 'export/dataptrs.json',
                               candidate_fixups=False, compile_check=False)
        with contextlib.redirect_stdout(io.StringIO()):
            datagen.generate(args)
        rows = json.loads((out / 'datagen-pointers.json').read_text())
        return next(r for r in rows if datagen.number(r['source']) == WIN)

    before = generate('before')
    assert datagen.number(before['target']) == image.word(WIN), before
    assert before['confirmed'], before
    assert before['applied'] or before['provenance'].endswith('+text'), before
    # Simulate exactly the effect of adding the demonstrated short string to
    # db/datagen_text.yaml; do not weaken pointer heuristics globally.
    datagen.string_words = lambda img, pointers, slots: original(img, pointers, slots) | {WIN}
    try:
        after = generate('after')
    finally:
        datagen.string_words = original
    assert not after['applied'] and after['provenance'].endswith('+text'), after
    print('Current win word:', before)
    print('Proposed text override: does not translate win word; PASS')

if not before['applied']:
    # Once the manifest is fixed, its now-unreferenced erroneous trap can
    # disappear from the link map. The protection regression still applies.
    print('PASS: manifest already protects win; pre-fix ASLR model not needed')
    sys.exit(0)

mapping = (ROOT / 'build/cutover/csf_standalone.map').read_text(errors='replace')
preferred = int(re.search(r'Preferred load address is\s+([0-9a-fA-F]+)', mapping)[1], 16)


def rva(symbol):
    line = next(line for line in mapping.splitlines() if symbol in line)
    return int(line.split()[2], 16) - preferred


# The string directly precedes the first pointer of vtable 0085EC34. A rebased
# pointer overwrites ALL FOUR bytes of "win\0", including its terminator.
trap = rva('?trap_006E6977@@')
destructor = rva('?C_0085ebdc_ScalarDeletingDestructor@@')
lengths = []
for base in [0x400000, 0xc00000]:
    corrupt = struct.pack('<II', base+trap, base+destructor)
    length = corrupt.find(b'\0')
    length = 8 if length < 0 else length
    lengths.append(length)
    print(f'Model load base {base:08X}: first 8 name bytes {corrupt.hex()} '
          f'-> name length {length if length < 8 else ">=8"}, '
          f'RtFS name-size check {"passes" if length < 8 else "fails"}')
    assert (b'win\0'+corrupt[4:]).find(b'\0') == 3
assert lengths[0] < 8 and lengths[1] == 8, lengths
print('PASS: generated fixup defect, proposed manifest protection, ASLR-dependent name-size failure model')
