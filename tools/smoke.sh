#!/usr/bin/env bash
# Launch the play copy for N seconds (default 30), then kill it and show the hook log.
# Exit 0 if the game was still running at the deadline (no crash), 1 otherwise.
set -uo pipefail
PLAY=${CSF_PLAY:-/mnt/e/dev/csf-play}
SECS=${1:-30}
WINPLAY=$(wslpath -w "$PLAY")
(cd "$PLAY" && cmd.exe /c start "" /D "$WINPLAY" CommXPC.exe >/dev/null 2>&1)
alive=0
for ((i = 0; i < SECS; i++)); do
  sleep 1
  if tasklist.exe /FI "IMAGENAME eq CommXPC.exe" 2>/dev/null | grep -q CommXPC; then alive=1; else alive=0; break; fi
done
taskkill.exe /F /IM CommXPC.exe >/dev/null 2>&1
echo "---- csf_hook.log"; cat "$PLAY/csf_hook.log" 2>/dev/null | tr -d '\r' | tail -20
if [ $alive = 1 ]; then echo "SMOKE OK (alive after ${SECS}s)"; else echo "SMOKE FAIL (exited/crashed early)"; exit 1; fi
