#!/usr/bin/env bash
# Launch the play copy, wait until it is READY (the dev start reached the scene / save, or for "menu" the hooks are
# installed; up to CSF_READY_TIMEOUT s, default 120), run N more seconds (default 15; 60 for a wider net), then kill
# it and show the hook log. Exit 0 if the game was still running at the deadline (no crash), 1 otherwise —
# including when it never became ready (startup hang/crash: a smoke that never reached the mission proves nothing).
# Result files of the previous run are deleted first, so a run that writes none can't be read as fresh evidence.
# Differential testing is on (CSF_DIFF=0 to disable): MISMATCH lines name functions whose result differed.
# Starts straight into a mission (src/core/devcmd.cpp), windowed 1280x960, muted unless CSF_MUTE=0, off-screen unless CSF_BG=0:
#   CSF_START="scene Maps/FR01/Parachut.scn" (default) | "loadsave 1.sav" | "menu" (normal intro + menus)
set -uo pipefail
PLAY=${CSF_PLAY:-/mnt/e/dev/csf-play}
SECS=${1:-15}
START=${CSF_START:-scene Maps/FR01/Parachut.scn}
INI="$PLAY/csf_hook.ini"
sed -i '/^\(scene\|loadsave\|windowed\|background\|cmdline\|autopilot\|probe\|diff\|diff_limit\|diff_calls\|menutour\|mute\) /d' "$INI"
[ "${CSF_DIFF:-1}" = 1 ] && echo "diff 1" >> "$INI"  # differential testing of eligible hooks (hook/diff.cpp)
echo "windowed 1280x960" >> "$INI"
# background mode (src/core/devcmd.cpp): off-screen window, no pause on focus loss, focus handed back (CSF_BG=0: visible)
[ "${CSF_BG:-1}" = 1 ] && echo "background 1" >> "$INI"
# muted (src/core/devcmd.cpp: the process's audio session; CSF_MUTE=0: audible)
[ "${CSF_MUTE:-1}" = 1 ] && echo "mute 1" >> "$INI"
[ "$START" != menu ] && echo "$START" >> "$INI"
# extra ini lines for this run only, e.g. autopilot steps: CSF_INI_EXTRA=$'autopilot 9000 key ESCAPE\n...'
[ -n "${CSF_INI_EXTRA:-}" ] && printf '%s\n' "$CSF_INI_EXTRA" >> "$INI"
case "$START" in
  scene*) READY="devcmd: starting scene";;
  loadsave*) READY="devcmd: dev start";;
  *) READY="hooks installed=";;
esac
rm -f "$PLAY/csf_hook_hits.txt" "$PLAY/csf_diff_summary.txt" "$PLAY/csf_diff.log" "$PLAY/csf_hook.log"
WINPLAY=$(wslpath -w "$PLAY")
result=$(powershell.exe -NoProfile -Command "
\$p = Start-Process -FilePath '$WINPLAY\\CommXPC.exe' -WorkingDirectory '$WINPLAY' -PassThru
\$t0 = Get-Date; \$ready = \$false
\$nohook = \$false
while (-not \$p.HasExited -and ((Get-Date) - \$t0).TotalSeconds -lt ${CSF_READY_TIMEOUT:-120}) {
  # no hook log after 10 s: our DLL isn't running, so neither is windowed/off-screen/muted mode: kill it now
  if (((Get-Date) - \$t0).TotalSeconds -gt 10 -and -not (Test-Path '$WINPLAY\\csf_hook.log')) { \$nohook = \$true; break }
  try { if (Select-String -Path '$WINPLAY\\csf_hook.log' -SimpleMatch '$READY' -Quiet) { \$ready = \$true; break } } catch {}
  Start-Sleep -Milliseconds 250
}
\$rs = [int]((Get-Date) - \$t0).TotalSeconds
\$alive = \$ready -and -not \$p.WaitForExit(${SECS}000)
if (-not \$p.HasExited) { Stop-Process -Id \$p.Id -Force; \$p.WaitForExit(5000) | Out-Null }
Write-Output \"nohook=\$nohook pid=\$(\$p.Id) ready=\$ready after=\${rs}s alive=\$alive exit=\$(if(\$alive){'killed'}else{\$p.ExitCode}) stillrunning=\$(-not \$p.HasExited)\"
" | tr -d '\r')
echo "$result"
echo "---- hits"; { tr -d "\r" < "$PLAY/csf_hook_hits.txt"; } 2>/dev/null | sort -k2 -n -r | head -10
echo "---- diff ($(cat "$PLAY/csf_diff_summary.txt" 2>/dev/null | wc -l) functions compared)"
{ tr -d '\r' < "$PLAY/csf_diff_summary.txt"; } 2>/dev/null | awk '$4 > 0 {print "MISMATCH " $0}'
{ tr -d '\r' < "$PLAY/csf_diff.log"; } 2>/dev/null | head -20
grep -A5 CRASH "$PLAY/csf_hook.log" 2>/dev/null
echo "---- csf_hook.log"; tr -d '\r' < "$PLAY/csf_hook.log" 2>/dev/null | tail -20
crashes=$(grep -c CRASH "$PLAY/csf_hook.log" 2>/dev/null)
case "$result" in
  *stillrunning=True*) echo "WARNING: could not kill the game"; exit 2;;
  *alive=True*) [ "${crashes:-0}" -gt 0 ] && { echo "SMOKE FAIL (alive, but $crashes CRASH logged: the game caught it)"; exit 1; }
                echo "SMOKE OK (alive after ${SECS}s)"
                # verification ledger (db/verify.json): which functions ran / were diffed, tied to their code hash
                [ "${CSF_NO_LEDGER:-0}" = 1 ] || "$(dirname "$0")/../.venv/bin/python" "$(dirname "$0")/verify.py" ingest;;
  *nohook=True*) echo "SMOKE FAIL (no csf_hook.log within 10s: the hook DLL did not initialize; game killed)"; exit 1;;
  *ready=False*) echo "SMOKE FAIL (never ready: no '$READY' in csf_hook.log within ${CSF_READY_TIMEOUT:-120}s)"; exit 1;;
  *) echo "SMOKE FAIL (exited/crashed early)"; exit 1;;
esac
