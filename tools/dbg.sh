#!/usr/bin/env bash
# Run a tools/dbg/<script>.py under the Windows x64dbg-automate venv (E:\dev\tools\x64dbg-venv), symbolized.
# Usage: tools/dbg.sh crash [seconds]      play copy under x32dbg; report the first crash
set -uo pipefail
ROOT="$(cd "$(dirname "$0")/.." && pwd)"
S=$(wslpath -w "$ROOT/tools/dbg/$1.py"); shift
# A standalone exe given as a Windows path is refreshed from build/ first (as dryrun.py does), so the run and
# tools/dbg/stdsym.py's link map describe the same build.
for a in "$@"; do
  case "$a" in *csf_standalone.exe) cp "$ROOT/build/csf_standalone.exe" "$(wslpath -u "$a")" || exit 1 ;; esac
done
powershell.exe -NoProfile -Command "E:\\dev\\tools\\x64dbg-venv\\Scripts\\python.exe '$S' $*" 2>&1 | tr -d '\r' \
  | "$ROOT/.venv/bin/python" "$ROOT/tools/dbg/symbolize.py"
