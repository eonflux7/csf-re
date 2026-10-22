#!/usr/bin/env python3
"""Make the play copy load our hook DLL under its own name (play copy only; never the original builds).

Processes started from WSL inherit the "prefer System32 images" mitigation (GetProcessMitigationPolicy:
PreferSystem32Images=1, also through powershell.exe Start-Process): any DLL name that exists in System32 is loaded
from there and the game directory's copy is never checked. Our smokes start the game from WSL, so the game ran
without our dinput8.dll proxy (2026-10-09): no hooks, so no windowed / off-screen / muted mode either. The play copy's import is renamed
DINPUT8.dll -> CSFHOOK.dll (same length) and the DLL is deployed as CSFHOOK.dll; it still forwards
DirectInput8Create to System32's dinput8.dll (hook/main.cpp). Idempotent; the unpatched exe is kept as
CommXPC.exe.orig.

Usage: playexe.py [PLAY_DIR]      prints the DLL file name to deploy to
"""
import os, shutil, sys
sys.path.insert(0, os.path.dirname(os.path.abspath(__file__)))
from paths import PLAY

DLL = "CSFHOOK.dll"
OLD, NEW = b"DINPUT8.dll\0", DLL.encode() + b"\0"


def ensure(play):
    exe = os.path.join(play, "CommXPC.exe")
    data = open(exe, "rb").read()
    if data.count(NEW) == 1:
        return DLL
    if data.count(OLD) != 1:
        raise SystemExit(f"{exe}: expected exactly one {OLD!r} import name, found {data.count(OLD)}")
    if not os.path.exists(exe + ".orig"):
        shutil.copy2(exe, exe + ".orig")
    open(exe, "wb").write(data.replace(OLD, NEW))
    stale = os.path.join(play, "dinput8.dll")  # the old proxy name: nothing loads it any more
    if os.path.exists(stale):
        os.remove(stale)
    return DLL


if __name__ == "__main__":
    print(ensure(sys.argv[1] if len(sys.argv) > 1 else PLAY))
