#!/bin/bash
# The IDA feedback loop: push what the binary proves and what we know into the IDB, re-decompile everything,
# derive more, until nothing new. ~6 min per round. Usage: tools/ida/propagate.sh [max_rounds]
#   sync.py --protos   cdecl prototypes the callers' cleanup proves longer
#   sync.py --this     `this` / parameters typed as class pointers (this_class.json, arg_types.json)
#   sync.py            names, structs, typed vtables, member pointer types, global types
#   export_abi.py      Hex-Rays' view again -> this_class.py derives classes, parameter and member types
set -eo pipefail
cd "$(dirname "$0")/../.."
PY=.venv/bin/python
max=${1:-6}
$PY tools/ida/sync.py --protos
$PY tools/ida/export_abi.py | tail -1
for i in $(seq 1 "$max"); do
    $PY tools/ida/sync.py --this
    $PY tools/ida/sync.py | tail -1
    $PY tools/ida/export_abi.py | tail -1
    out=$($PY tools/this_class.py)
    echo "round $i: $out"
    case "$out" in *"; 0 new since"*) break ;; esac
done
$PY tools/this_class.py --report
