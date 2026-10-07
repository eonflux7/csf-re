#!/usr/bin/env bash
# Launch the play copy for N seconds (default 15; 60 for a wider net), then kill it and show the hook log.
# Exit 0 if the game was still running at the deadline (no crash), 1 otherwise.
# Differential testing is on (CSF_DIFF=0 to disable): MISMATCH lines name functions whose result differed.
# Starts straight into a mission (src/core/devcmd.cpp), windowed 1280x960:
#   CSF_START="scene Maps/FR01/Parachut.scn" (default) | "loadsave 1.sav" | "menu" (normal intro + menus)
set -uo pipefail
PLAY=${CSF_PLAY:-/mnt/e/dev/csf-play}
SECS=${1:-15}
START=${CSF_START:-scene Maps/FR01/Parachut.scn}
INI="$PLAY/csf_hook.ini"
sed -i '/^\(scene\|loadsave\|windowed\|cmdline\|autopilot\|probe\|diff\|diff_limit\|diff_calls\) /d' "$INI"
[ "${CSF_DIFF:-1}" = 1 ] && echo "diff 1" >> "$INI"  # differential testing of eligible hooks (hook/diff.cpp)
echo "windowed 1280x960" >> "$INI"
[ "$START" != menu ] && echo "$START" >> "$INI"
WINPLAY=$(wslpath -w "$PLAY")
result=$(powershell.exe -NoProfile -Command "
\$p = Start-Process -FilePath '$WINPLAY\\CommXPC.exe' -WorkingDirectory '$WINPLAY' -PassThru
\$alive = -not \$p.WaitForExit(${SECS}000)
if (\$alive) { Stop-Process -Id \$p.Id -Force; \$p.WaitForExit(5000) | Out-Null }
Write-Output \"pid=\$(\$p.Id) alive=\$alive exit=\$(if(\$alive){'killed'}else{\$p.ExitCode}) stillrunning=\$(-not \$p.HasExited)\"
" | tr -d '\r')
echo "$result"
echo "---- hits"; tr -d "\r" < "$PLAY/csf_hook_hits.txt" 2>/dev/null | sort -k2 -n -r | head -10
echo "---- diff ($(wc -l < "$PLAY/csf_diff_summary.txt" 2>/dev/null || echo 0) functions compared)"
tr -d '\r' < "$PLAY/csf_diff_summary.txt" 2>/dev/null | awk '$4 > 0 {print "MISMATCH " $0}'
tr -d '\r' < "$PLAY/csf_diff.log" 2>/dev/null | head -20
grep -A5 CRASH "$PLAY/csf_hook.log" 2>/dev/null
echo "---- csf_hook.log"; tr -d '\r' < "$PLAY/csf_hook.log" 2>/dev/null | tail -20
crashes=$(grep -c CRASH "$PLAY/csf_hook.log" 2>/dev/null)
case "$result" in
  *stillrunning=True*) echo "WARNING: could not kill the game"; exit 2;;
  *alive=True*) [ "${crashes:-0}" -gt 0 ] && { echo "SMOKE FAIL (alive, but $crashes CRASH logged: the game caught it)"; exit 1; }
                echo "SMOKE OK (alive after ${SECS}s)";;
  *) echo "SMOKE FAIL (exited/crashed early)"; exit 1;;
esac
