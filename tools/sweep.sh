#!/usr/bin/env bash
# Coverage sweep: smoke the deployed DLL from every start that reaches different code, so db/verify.json records
# which functions run (each SMOKE OK ingests). ~4 min. Starts: FR01 scene, the menu tour (devcmd `menutour`), the
# mid-mission save 1.sav (Fiord), three more scenes. Movement scripts and other idle scenes add almost nothing; the
# AI combat tasks need real play (saves taken in combat: add them to SAVES).
# Usage: sweep.sh [secs per start, default 40]
set -uo pipefail
cd "$(dirname "$0")/.."
SECS=${1:-40}
SAVES=${CSF_SWEEP_SAVES:-1.sav}
fail=0
run() { echo "== $1"; out=$(env "${@:2}" tools/smoke.sh "$SECS"); rc=$?
        grep -E "^SMOKE|^MISMATCH|^verify:" <<< "$out"; [ $rc = 0 ] || fail=1; }
run "scene FR01"
run "menu tour" CSF_START=menu "CSF_INI_EXTRA=menutour 1500"
for s in $SAVES; do run "loadsave $s" "CSF_START=loadsave $s"; done
for s in Maps/ST04/Snipers.scn Maps/FR03/Convoy.scn Maps/NR01/NR01A/Fiord.scn; do run "scene $s" "CSF_START=scene $s"; done
.venv/bin/python tools/verify.py report
exit $fail
