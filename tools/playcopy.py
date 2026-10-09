#!/usr/bin/env python3
"""Make another copy of the play copy: big read-only data hard-linked (no disk cost), the rest copied.

Hard-linked: files of at least 1 MB that the game only reads (.pak maps, .bik videos, sound banks, ...). Copied:
executables and DLLs (deploy.sh / playexe.py rewrite them per copy), Config/ (the game writes Juego.cfg etc.), and
everything small. Our run files (csf_hook.log, hits, diff logs, shots/) are not carried over.

  playcopy.py DST [--src /mnt/e/dev/csf-play]             a hook copy (smoke pool instance; deploy.sh fills it)
  playcopy.py DST --original                              the unhooked original game (CommXPC.exe.orig, no hook
                                                          DLL): for playing / making savegames by hand
Savegames live in Documents\\CSF Data\\Games, shared by every copy. Idempotent: existing files are replaced.
Never point DST at /mnt/e/dev/csf-builds (read-only originals).
"""
import argparse, os, shutil, sys

LINK_MIN = 1 << 20
NEVER_LINK = {".exe", ".dll", ".cfg", ".ini", ".txt", ".log", ".orig", ".dat"}
SKIP = {"csf_hook.log", "csf_hook_hits.txt", "csf_diff.log", "csf_diff_summary.txt", "shots"}


def main():
    ap = argparse.ArgumentParser()
    ap.add_argument("dst")
    ap.add_argument("--src", default="/mnt/e/dev/csf-play")
    ap.add_argument("--original", action="store_true", help="unhooked original exe, no hook DLL / ini")
    a = ap.parse_args()
    src, dst = os.path.abspath(a.src), os.path.abspath(a.dst)
    if "csf-builds" in dst or dst == src:
        sys.exit(f"refusing to write {dst}")
    if not os.path.exists(os.path.join(src, "CommXPC.exe")):
        sys.exit(f"{src}: no CommXPC.exe")
    linked = copied = 0
    for root, dirs, files in os.walk(src):
        rel = os.path.relpath(root, src)
        dirs[:] = [d for d in dirs if d not in SKIP]
        os.makedirs(os.path.join(dst, rel), exist_ok=True)
        for f in files:
            if f in SKIP or (a.original and (f in ("CSFHOOK.dll", "csf_hook.ini", "CommXPC.exe"))):
                continue
            s, d = os.path.join(root, f), os.path.join(dst, rel, f)
            if os.path.lexists(d):
                os.remove(d)
            ext = os.path.splitext(f)[1].lower()
            if os.path.getsize(s) >= LINK_MIN and ext not in NEVER_LINK and rel.split(os.sep)[0] != "Config":
                os.link(s, d)
                linked += 1
            else:
                shutil.copy2(s, d)
                copied += 1
    if a.original:
        orig = os.path.join(src, "CommXPC.exe.orig")
        if not os.path.exists(orig):
            sys.exit(f"{orig} missing (tools/playexe.py keeps the unpatched exe there)")
        shutil.copy2(orig, os.path.join(dst, "CommXPC.exe"))
        os.remove(os.path.join(dst, "CommXPC.exe.orig"))
    print(f"{dst}: {linked} files hard-linked, {copied} copied{' (original, unhooked)' if a.original else ''}")


if __name__ == "__main__":
    main()
