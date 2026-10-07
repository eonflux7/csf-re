#!/usr/bin/env bash
# Launch the play copy for N seconds (default 60), then kill it and show the hook log.
# Exit 0 if the game was still running at the deadline (no crash), 1 otherwise.
# Starts straight into a mission (src/core/devcmd.cpp), windowed 1280x960:
#   CSF_START="scene Maps/FR01/Parachut.scn" (default) | "loadsave 1.sav" | "menu" (normal intro + menus)
set -uo pipefail
PLAY=${CSF_PLAY:-/mnt/e/dev/csf-play}
SECS=${1:-60}
START=${CSF_START:-scene Maps/FR01/Parachut.scn}
INI="$PLAY/csf_hook.ini"
sed -i '/^\(scene\|loadsave\|windowed\|cmdline\|autopilot\|probe\) /d' "$INI"
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
echo "---- csf_hook.log"; tr -d '\r' < "$PLAY/csf_hook.log" 2>/dev/null | tail -20
case "$result" in
  *stillrunning=True*) echo "WARNING: could not kill the game"; exit 2;;
  *alive=True*) echo "SMOKE OK (alive after ${SECS}s)";;
  *) echo "SMOKE FAIL (exited/crashed early)"; exit 1;;
esac
