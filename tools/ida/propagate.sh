#!/bin/bash
# The IDA feedback loop: push what the binary proves and what we know into the IDB, re-decompile everything,
# derive more, until nothing new. ~5 min per round. Usage: tools/ida/propagate.sh [max_rounds]
set -eo pipefail
cd "$(dirname "$0")/../.."
PY=.venv/bin/python
max=${1:-6}
$PY tools/ida/sync.py --protos
$PY tools/ida/export_abi.py | tail -1
for i in $(seq 1 "$max"); do
    $PY tools/ida/sync.py --this
    $PY tools/ida/export_abi.py | tail -1
    out=$($PY tools/this_class.py)
    echo "round $i: $out"
    case "$out" in *"; 0 new since"*) break ;; esac
done
$PY tools/this_class.py --report
