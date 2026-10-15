#!/usr/bin/env python3
"""Optional retail-data test; no copyrighted fixtures are checked in.

Extract only .anm entries from a retail PC PAK and pass them to the native
animation test. PAK framing follows pak-man's MIT docs/FORMAT.md (not RW SDK
material); animation decoding is done only by the tested boundary.
"""
import argparse
import hashlib
import json
import pathlib
import struct
import subprocess
import zlib

ROOT = pathlib.Path(__file__).resolve().parents[1]


def extract(pak, output):
    output.mkdir(parents=True, exist_ok=True)
    output = output.resolve()
    manifest = []
    files = []
    with pak.open('rb') as stream:
        magic, version, platform, count = struct.unpack('<4sIII', stream.read(16))
        assert magic in (b'PAKA', b'PAKC') and (version, platform) == (5, 1)
        assert count < 1_000_000
        entries = []
        for _ in range(count):
            name = bytearray()
            while (byte := stream.read(1)) != b'\0':
                assert byte and len(name) < 65536
                name += byte
            offset, size, _ = struct.unpack('<IIQ', stream.read(16))
            entries.append((name.decode('cp1252'), offset, size))
        base, total = stream.tell(), pak.stat().st_size
        for index, (name, offset, size) in enumerate(entries):
            if not name.lower().endswith('.anm'):
                continue
            path = pathlib.PurePosixPath(name)
            assert not path.is_absolute() and '..' not in path.parts and ':' not in name and '\\' not in name
            dest = output.joinpath(*path.parts).resolve()
            assert dest.is_relative_to(output)
            start = base + offset - 13
            end = base + entries[index + 1][1] - 13 if index + 1 < len(entries) else total
            assert base <= start <= end <= total and size <= 256 * 1024 * 1024
            stream.seek(start)
            if magic == b'PAKA':
                data = stream.read(size)
            else:
                data = bytearray()
                while len(data) < size:
                    assert stream.tell() + 8 <= end
                    compressed, raw = struct.unpack('<II', stream.read(8))
                    assert 0 < raw <= 4096 and raw <= size - len(data)
                    assert compressed <= 65536 and stream.tell() + compressed <= end
                    block = zlib.decompress(stream.read(compressed))
                    assert len(block) == raw
                    data += block
                assert stream.read(4) == b'\0' * 4
            assert len(data) == size and stream.tell() == end
            dest.parent.mkdir(parents=True, exist_ok=True)
            dest.write_bytes(data)
            files.append(dest)
            manifest.append(dict(path=name, size=size, sha256=hashlib.sha256(data).hexdigest(),
                                 scheme=struct.unpack_from('<I', data, 16)[0]))
    assert files, 'PAK has no animations'
    (output / 'manifest.json').write_text(json.dumps(manifest, indent=2))
    return files, manifest


def main():
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument('--pak', type=pathlib.Path, required=True)
    parser.add_argument('--output', type=pathlib.Path, default=ROOT / 'work/rwanim/retail')
    parser.add_argument('--test', type=pathlib.Path,
                        default=ROOT / 'work/rwcheck-asan/src/boundary/rw/csf_rw_animation_link')
    args = parser.parse_args()
    files, manifest = extract(args.pak, args.output)
    subprocess.run([str(args.test.resolve()), *map(str, files)], check=True, cwd=ROOT)
    print(f'rw animation retail: {len(files)} files, {sum(x["size"] for x in manifest)} bytes, '
          f'schemes {sorted({x["scheme"] for x in manifest})}: PASS')


if __name__ == '__main__':
    main()
