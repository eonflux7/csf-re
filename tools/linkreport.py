#!/usr/bin/env python3
"""Group lld-link errors from a saved first-link build log; no error is suppressed."""
import argparse
import collections
import json
from pathlib import Path
import re


def group(text):
    groups = collections.defaultdict(set)
    for line in text.splitlines():
        if 'lld-link: error:' not in line:
            continue
        message = line.split('lld-link: error:', 1)[1].strip()
        if message.startswith('undefined symbol:'):
            symbol = message.split(':', 1)[1].strip()
            if re.search(r'fn_slot[<@]', symbol):
                kind = 'fn_slots'
            elif re.search(r'\bgv<', symbol) or '?$gv@' in symbol:
                kind = 'data_globals'
            elif re.search(r'\bvt<', symbol) or '?$vt@' in symbol:
                kind = 'data_vtables'
            elif '__imp_' in symbol or '__declspec(dllimport)' in symbol:
                kind = 'imports'
            else:
                kind = 'other_undefined'
            groups[kind].add(symbol)
        elif 'duplicate symbol:' in message:
            groups['duplicates'].add(message)
        elif 'section' in message.lower():
            groups['sections'].add(message)
        elif 'could not open' in message and '.lib' in message.lower():
            groups['import_libraries'].add(message)
        else:
            groups['other_errors'].add(message)
    kinds = ('fn_slots', 'data_globals', 'data_vtables', 'other_undefined', 'duplicates',
             'sections', 'imports', 'import_libraries', 'other_errors')
    return {k: sorted(groups[k]) for k in kinds}


if __name__ == '__main__':
    ap = argparse.ArgumentParser(description=__doc__)
    ap.add_argument('log', type=Path)
    ap.add_argument('--out', type=Path, required=True)
    args = ap.parse_args()
    groups = group(args.log.read_text())
    args.out.parent.mkdir(parents=True, exist_ok=True)
    args.out.write_text(json.dumps(dict(counts={k: len(v) for k, v in groups.items()},
                                        errors=groups), indent=1) + '\n')
    for kind, symbols in groups.items():
        print(f'{kind}: {len(symbols)}')
        for symbol in symbols:
            print('  ' + symbol)
