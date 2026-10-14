#!/usr/bin/env python3
"""Run the standalone exe's dry run on Windows (WSL interop) and record how far it got.

  dryrun.py [--dir /mnt/e/dev/csf-standalone] [--exe build/csf_standalone.exe] [--timeout 60]
  dryrun.py --full [--timeout 60]   no CSF_DRYRUN: on into the game's WinMain (opens a window); killed at the
                                    timeout; output DIR/winmain.out, nothing recorded in work/dryrun.json
  dryrun.py --full --input SCRIPT --shots 20,25 [--tag menu]
      SCRIPT: scripted DirectInput events (CSF_INPUT, standalone/game_plain.cpp), "T:key:DIK;T:goto:X,Y;T:click"
      (T seconds; DIK 1 = Escape, 0x1C = Enter); --shots: screenshots at those seconds (from the start of the run)
      to DIR/shots/TAG-T.png (tools/dbg/shot.ps1, the primary screen) plus a 640-wide TAG-T.jpg to look at.
      Named scripts: --input intro-skip, see SCRIPTS. With --input, CSF_PAGE_TRACE prints "csf: page NAME" for every
      menu page requested (0x71C2A0), and the summary line ends with the pages seen.
  dryrun.py --walk [NAME,NAME..] [--timeout 40]
      every named script (default: all but intro-skip) in turn, a screenshot at its WALK time; one line each and
      work/measure/walk.json (exit, trap, first exception, pages, expected page reached); outputs kept as
      work/measure/walk-NAME.out. Exit status 1 when a script faulted, trapped or missed its page.

Copies the exe into the standalone copy (made with `playcopy.py DIR --original`), runs it with CSF_DRYRUN=1
(data initialisation and the CRT initialiser tables, then exit before WinMain), and writes work/dryrun.json:
initialisers run of the table, the last one, the trap it stopped at (if any), exit code. Output: DIR/dryrun.out.
"""
import argparse, json, os, re, shutil, subprocess, sys, threading, time

ROOT = os.path.dirname(os.path.dirname(os.path.abspath(__file__)))
# Named input scripts (800x600 menu coordinates). The intro movies skip on a key; the first ~8 s are start-up.
SKIP = "8:key:1;10:key:1;12:key:1;14:key:1;16:key:1"  # one Escape per movie: 4 logos + the intro; menu at ~20 s
SCRIPTS = {
    "intro-skip": SKIP,
    "campaign": SKIP + ";20:goto:400,253;21:click",
    "settings": SKIP + ";20:goto:400,344;21:click",
    "credits": SKIP + ";20:goto:400,388;21:click",
    "exit": SKIP + ";20:goto:400,433;21:click",
    "settings-controls": SKIP + ";20:goto:400,344;21:click;24:goto:400,203;25:click",
    "settings-video": SKIP + ";20:goto:400,344;21:click;24:goto:400,253;25:click",
    "settings-sound": SKIP + ";20:goto:400,344;21:click;24:goto:400,302;25:click",
    "settings-game": SKIP + ";20:goto:400,344;21:click;24:goto:400,352;25:click",
    "settings-back": SKIP + ";20:goto:400,344;21:click;24:goto:400,517;25:click",
    "exit-ok": SKIP + ";20:goto:400,433;21:click;24:goto:296,320;25:click",  # Exit -> Ok: the process ends
}
# --walk: screenshot time and the page the script should end on (None: not checked; "exit": the process ends with
# WinMain's return code - a different one means the CRT's exit failed; rerun with CSF_EXIT_TRACE=1 to see why).
WALK = {
    "campaign": (32, None), "credits": (32, None), "settings-controls": (32, None), "settings-video": (32, None),
    "settings-sound": (42, None), "settings-game": (32, None), "settings-back": (32, None), "exit": (32, None),
    "exit-ok": (None, "exit"),
}


def shots(dirpath, tag, times, t0):
    """Screenshots of the primary screen at the given seconds after t0 (PowerShell CopyFromScreen)."""
    os.makedirs(os.path.join(dirpath, "shots"), exist_ok=True)
    script = subprocess.run(["wslpath", "-w", os.path.join(ROOT, "tools", "dbg", "shot.ps1")],
                            capture_output=True, text=True).stdout.strip()
    for t in times:
        time.sleep(max(0, t0 + t - time.time()))
        out = subprocess.run(["wslpath", "-w", os.path.join(dirpath, "shots", f"{tag}-{t:g}.png")],
                             capture_output=True, text=True).stdout.strip()
        subprocess.run(["powershell.exe", "-NoProfile", "-ExecutionPolicy", "Bypass", "-File", script, out],
                       capture_output=True)
        try:
            from PIL import Image
            png = os.path.join(dirpath, "shots", f"{tag}-{t:g}.png")
            im = Image.open(png)
            im.thumbnail((640, 480))
            im.convert("RGB").save(png[:-4] + ".jpg", quality=85)
        except Exception:
            pass


def copy_exe(src, dst):
    """The previous run's process may still hold the exe for a few seconds after taskkill."""
    for _ in range(30):
        try:
            return shutil.copy2(src, dst)
        except PermissionError:
            subprocess.run(["taskkill.exe", "/IM", "csf_standalone.exe", "/F"], capture_output=True)
            time.sleep(1)
    shutil.copy2(src, dst)


def walk(names, timeout):
    """Run each named script with --full in a child dryrun.py; summarise."""
    results, bad = [], 0
    os.makedirs(os.path.join(ROOT, "work", "measure"), exist_ok=True)
    for name in names:
        shot, want = WALK.get(name, (32, None))
        cmd = [sys.executable, os.path.abspath(__file__), "--full", "--timeout", str(timeout), "--input", name,
               "--tag", name] + (["--shots", f"{shot:g}"] if shot else [])
        line = subprocess.run(cmd, capture_output=True, text=True).stdout.strip().splitlines()
        line = line[-1] if line else "(no output)"
        out = open("/mnt/e/dev/csf-standalone/winmain.out", errors="replace").read()
        shutil.copy("/mnt/e/dev/csf-standalone/winmain.out", os.path.join(ROOT, "work", "measure", f"walk-{name}.out"))
        pages = re.findall(r"^csf: page (\S+)", out, re.M)
        exc = re.search(r"^csf: EXCEPTION .*$", out, re.M)
        trap = re.search(r"^csf: UNRESOLVED .*$", out, re.M)
        code = re.search(r"full run: exit (\S+)", line)
        code = code.group(1).rstrip(";") if code else "?"
        ret = re.search(r"^csf: WinMain returned (-?\d+)", out, re.M)
        if want == "exit" and not (ret and code == ret.group(1)):  # the CRT's exit must keep WinMain's code
            code += f" (WinMain {ret.group(1) if ret else 'did not return'})"
        ok = not exc and not trap and (want != "exit" or (ret and code == ret.group(1))) and (want in (None, "exit") or
                                                                           (pages and pages[-1] == want))
        bad += not ok
        r = dict(name=name, ok=ok, exit=code, pages=pages, trap=trap.group(0) if trap else None,
                 exception=exc.group(0) if exc else None,
                 shot=f"/mnt/e/dev/csf-standalone/shots/{name}-{shot:g}.jpg" if shot else None)
        results.append(r)
        print(f"{'ok  ' if ok else 'FAIL'} {name:18} exit {code:8} pages {' > '.join(pages) or '-'}"
              f"{'; ' + r['trap'] if trap else ''}{'; ' + r['exception'] if exc else ''}", flush=True)
        time.sleep(2)
    json.dump(dict(at=time.time(), results=results), open(os.path.join(ROOT, "work", "measure", "walk.json"), "w"),
              indent=1)
    print(f"walk: {len(names) - bad} of {len(names)} ok; work/measure/walk.json")
    return 1 if bad else 0


def main():
    ap = argparse.ArgumentParser()
    ap.add_argument("--dir", default="/mnt/e/dev/csf-standalone")
    ap.add_argument("--exe", default=os.path.join(ROOT, "build", "csf_standalone.exe"))
    ap.add_argument("--timeout", type=int, default=60)
    ap.add_argument("--full", action="store_true")
    ap.add_argument("--input", help="scripted input (CSF_INPUT) or a name from SCRIPTS")
    ap.add_argument("--shots", help="comma-separated seconds for screenshots")
    ap.add_argument("--tag", default="shot")
    ap.add_argument("--walk", nargs="?", const="", help="comma-separated script names (default: all of WALK)")
    a = ap.parse_args()
    if a.walk is not None:
        sys.exit(walk([n for n in a.walk.split(",") if n] or list(WALK), a.timeout if a.timeout != 60 else 40))
    if a.input:
        os.environ["CSF_INPUT"] = SCRIPTS.get(a.input, a.input)
        os.environ.setdefault("CSF_PAGE_TRACE", "1")  # "csf: page NAME" from 0x71C2A0 RequestMenuByName
    if "csf-builds" in a.dir:
        raise SystemExit("refusing to run in the read-only originals")
    copy_exe(a.exe, os.path.join(a.dir, "csf_standalone.exe"))
    # Keep optional boundary tracing visible across WSL interop as well as
    # the dry-run switch. No tracing is enabled unless the caller asks for it.
    exports = [os.environ.get("WSLENV", ""), "CSF_DRYRUN/w"]
    if a.full:  # message boxes print and answer OK (standalone/main.cpp); CSF_NO_DIALOGS=0 keeps them
        os.environ.setdefault("CSF_NO_DIALOGS", "1")
        os.environ.setdefault("CSF_EXCEPTION_TRACE", "1")  # first faults print where they were (tools/dbg/stdsym.py)
    for v in ("CSF_RW_TRACE", "CSF_HANG_SECONDS", "CSF_NO_DIALOGS", "CSF_INPUT", "CSF_EXIT_TRACE", "CSF_EXCEPTION_TRACE", "CSF_PAGE_TRACE"):  # HANG: the exe dumps its main thread then (tools/dbg/stdsym.py)
        if v in os.environ:
            exports.append(v + "/w")
    env = dict(os.environ, CSF_DRYRUN="0" if a.full else "1",
               WSLENV=":".join(e for e in exports if e))
    t0 = time.time()
    shooter = None
    if a.shots:
        shooter = threading.Thread(target=shots, args=(a.dir, a.tag, [float(x) for x in a.shots.split(",")], t0),
                                   daemon=True)
        shooter.start()
    try:
        p = subprocess.run(["./csf_standalone.exe"], cwd=a.dir, env=env, capture_output=True, text=True,
                           errors="replace", timeout=a.timeout)
        out, code = p.stdout + p.stderr, p.returncode
    except subprocess.TimeoutExpired as e:
        out, code = ((e.stdout or b"") + (e.stderr or b"")).decode(errors="replace"), "timeout"
        subprocess.run(["taskkill.exe", "/IM", "csf_standalone.exe", "/F"], capture_output=True)
    if shooter:
        shooter.join(timeout=1)
    if isinstance(code, int) and code in (5, 0xC0000005 - (1 << 32)):
        code = "0xC0000005 (access violation; WSL shows 5)"
    if a.full:
        open(os.path.join(a.dir, "winmain.out"), "w").write(out)
        trap = re.search(r"^csf: UNRESOLVED .*$", out, re.M)
        hang = "; HANG dump: tools/dbg/stdsym.py" if "csf: HANG" in out else ""
        exc = re.search(r"^csf: EXCEPTION .*$", out, re.M)
        if exc:
            hang += f"; {exc.group(0)} (tools/dbg/stdsym.py)"
        pages = re.findall(r"^csf: page (\S+)", out, re.M)
        if pages:
            hang += "; pages " + " > ".join(pages)
        print(f"full run: exit {code}; {trap.group(0) if trap else 'no trap'}; {a.dir}/winmain.out{hang}")
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
