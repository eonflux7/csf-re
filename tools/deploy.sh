#!/usr/bin/env bash
# Build the hook DLL and copy it into the play copy of the game (never the original build).
set -euo pipefail
ROOT="$(cd "$(dirname "$0")/.." && pwd)"
PLAY=${CSF_PLAY:-/mnt/e/dev/csf-play}
ninja -C "$ROOT/build"
cp "$ROOT/build/dinput8.dll" "$PLAY/dinput8.dll"
[ -f "$PLAY/csf_hook.ini" ] || cp "$ROOT/hook/csf_hook.ini" "$PLAY/csf_hook.ini"
echo "deployed to $PLAY"
