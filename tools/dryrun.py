#!/usr/bin/env python3
"""Run the standalone exe's dry run on Windows (WSL interop) and record how far it got.

  dryrun.py [--dir /mnt/e/dev/csf-standalone] [--exe build/csf_standalone.exe] [--timeout 60]
  dryrun.py --full [--timeout 60]   no CSF_DRYRUN: on into the game's WinMain (opens a window); killed at the
                                    timeout; output DIR/winmain.out, nothing recorded in work/dryrun.json

Copies the exe into the standalone copy (made with `playcopy.py DIR --original`), runs it with CSF_DRYRUN=1
(data initialisation and the CRT initialiser tables, then exit before WinMain), and writes work/dryrun.json:
initialisers run of the table, the last one, the trap it stopped at (if any), exit code. Output: DIR/dryrun.out.
"""
import argparse, json, os, re, shutil, subprocess, time

ROOT = os.path.dirname(os.path.dirname(os.path.abspath(__file__)))


def main():
    ap = argparse.ArgumentParser()
    ap.add_argument("--dir", default="/mnt/e/dev/csf-standalone")
    ap.add_argument("--exe", default=os.path.join(ROOT, "build", "csf_standalone.exe"))
    ap.add_argument("--timeout", type=int, default=60)
    ap.add_argument("--full", action="store_true")
    a = ap.parse_args()
    if "csf-builds" in a.dir:
        raise SystemExit("refusing to run in the read-only originals")
    shutil.copy2(a.exe, os.path.join(a.dir, "csf_standalone.exe"))
    env = dict(os.environ, CSF_DRYRUN="0" if a.full else "1", WSLENV="CSF_DRYRUN/w")
    t0 = time.time()
    try:
        p = subprocess.run(["./csf_standalone.exe"], cwd=a.dir, env=env, capture_output=True, text=True,
                           errors="replace", timeout=a.timeout)
        out, code = p.stdout + p.stderr, p.returncode
    except subprocess.TimeoutExpired as e:
        out, code = ((e.stdout or b"") + (e.stderr or b"")).decode(errors="replace"), "timeout"
        subprocess.run(["taskkill.exe", "/IM", "csf_standalone.exe", "/F"], capture_output=True)
    if isinstance(code, int) and code in (5, 0xC0000005 - (1 << 32)):
        code = "0xC0000005 (access violation; WSL shows 5)"
    if a.full:
        open(os.path.join(a.dir, "winmain.out"), "w").write(out)
        trap = re.search(r"^csf: UNRESOLVED .*$", out, re.M)
        print(f"full run: exit {code}; {trap.group(0) if trap else 'no trap'}; {a.dir}/winmain.out")
        return
    open(os.path.join(a.dir, "dryrun.out"), "w").write(out)
    ran = re.findall(r"^csf: (xc)\[(\d+)\] (0x[0-9A-Fa-f]+)", out, re.M)
    trap = re.search(r"^csf: UNRESOLVED (0x[0-9A-Fa-f]+) (.*?) \[(\w[\w-]*)\]", out, re.M)
    ini = [e for e in json.load(open(os.path.join(ROOT, "build", "cutover", "initializers.json")))["entries"]
           if e["table"] == "xc"]  # the exe prints the C++ ones; the C (xi) ones run silently before
    res = dict(at=time.time(), took=round(time.time() - t0, 1), exit=code,
               exe_mtime=os.path.getmtime(a.exe), total=len(ini), ran=len(ran),
               last=dict(table=ran[-1][0], index=int(ran[-1][1]), target=ran[-1][2]) if ran else None,
               trap=dict(address=trap.group(1), name=trap.group(2), region=trap.group(3)) if trap else None,
               finished=code == 0 and not trap)
    json.dump(res, open(os.path.join(ROOT, "work", "dryrun.json"), "w"), indent=1)
    stop = f"trap {res['trap']['address']} [{res['trap']['region']}]" if trap else f"exit {code}"
    print(f"dry run: {res['ran']} of {res['total']} C++ initialisers, {stop}"
          f"{' (finished)' if res['finished'] else ''}; {a.dir}/dryrun.out")


if __name__ == "__main__":
    main()
