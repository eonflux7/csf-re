#!/usr/bin/env bash
# Build the hook DLL and copy it into the play copy of the game (never the original build).
set -euo pipefail
ROOT="$(cd "$(dirname "$0")/.." && pwd)"
PLAY=${CSF_PLAY:-/mnt/e/dev/csf-play}
"$ROOT/.venv/bin/python" "$ROOT/tools/difftable.py" >/dev/null
ninja -C "$ROOT/build"
# the play exe imports our DLL as CSFHOOK.dll (tools/playexe.py: Windows resolves DINPUT8.dll to System32 first)
DLL=$("$ROOT/.venv/bin/python" "$ROOT/tools/playexe.py" "$PLAY")
cp "$ROOT/build/dinput8.dll" "$PLAY/$DLL"
[ -f "$PLAY/csf_hook.ini" ] || cp "$ROOT/hook/csf_hook.ini" "$PLAY/csf_hook.ini"
# pending functions (never installed in a passing smoke) stay off: the game runs the verified baseline;
# tools/verifier.py enables them a group at a time. CSF_PENDING_ON=1 deploys everything enabled.
if [ "${CSF_PENDING_ON:-0}" = 1 ]; then sed -i '/# pending$/d' "$PLAY/csf_hook.ini"
else "$ROOT/.venv/bin/python" "$ROOT/tools/verify.py" pending --ini "$PLAY/csf_hook.ini"; fi
echo "deployed to $PLAY"
