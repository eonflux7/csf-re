#!/usr/bin/env python3
"""Run the standalone exe's dry run on Windows (WSL interop) and record how far it got.

  dryrun.py [--dir /mnt/e/dev/csf-standalone] [--exe build/csf_standalone.exe] [--timeout 60]
  dryrun.py --full [--timeout 60]   no CSF_DRYRUN: on into the game's WinMain; killed at the timeout; output
                                    DIR/winmain.out, nothing recorded in work/dryrun.json
      Every --full run is OFFSCREEN and silent (standalone/harness.cpp, CSF_DISPLAY=offscreen): a 1280x960 window
      outside the desktop that never takes focus, the keyboard or the mouse, movies without sound, the intro skipped.
      --show: a visible 1280x960 window instead (for a person to watch); --intro: play the intro movies.
  dryrun.py --full --goto P_Option        open a menu page directly once the main menu is up
  dryrun.py --full --mission Maps/FR01/Ransom.scn --timeout 200 --shots 30,m60
                                          start a scenario directly (as the scenario select page does)
      --shots: back-buffer PNGs DIR/shots/TAG-T.png (+ 640-wide .jpg to look at) at T seconds after the first frame,
      or mT seconds after the main menu. --quit-after T closes the window T s after the first frame.
  dryrun.py --full --input SCRIPT         scripted DirectInput clicks/keys (CSF_INPUT "T:key:DIK;T:goto:X,Y;T:click;
      T:char:C", T seconds after the main menu, 800x600 menu coordinates) for flows that need a page's own state;
      named scripts in SCRIPTS. CSF_PAGE_TRACE prints "csf: page NAME" for every page requested (0x71C2A0).
  dryrun.py --walk [NAME,NAME..] [--timeout 30]
      every WALK entry (default: all but mission) in turn, offscreen, one after another: one line each and
      work/measure/walk.json (exit, trap, first exception, pages, expected page reached); outputs kept as
      work/measure/walk-NAME.out, shots DIR/shots/NAME-mT.jpg. Exit status 1 when one faulted, trapped or missed
      its page.

--full runs back up Documents/CSF Data (profiles, saves; shared with the original game) to work/csfdata-run and put
it back afterwards; CSF_KEEP_SAVES=1 keeps what the run wrote.

Copies the exe into the standalone copy (made with `playcopy.py DIR --original`), runs it with CSF_DRYRUN=1
(data initialisation and the CRT initialiser tables, then exit before WinMain), and writes work/dryrun.json:
initialisers run of the table, the last one, the trap it stopped at (if any), exit code. Output: DIR/dryrun.out.
"""
import argparse, json, os, re, shutil, subprocess, sys, threading, time

ROOT = os.path.dirname(os.path.dirname(os.path.abspath(__file__)))
# Walk entries. Pages that open on their own are opened directly (--goto: the menu manager's open-page call, as the
# page's button would); flows that need the page's own state (profile selection, typing a name, the Exit
# confirmation) use scripted DirectInput clicks (CSF_INPUT, 800x600 menu coordinates), timed from the main menu.
SCRIPTS = {
    "exit": "0:goto:400,433;1:click",
    "exit-ok": "0:goto:400,433;1:click;4:goto:296,320;5:click",  # Exit -> Ok: the process ends
    # Campaign profiles page: list entries from (494,218), 22 px apart; Select (200,222), New (200,267), Back (400,517)
    "campaign-select": "0:goto:400,253;1:click;4:goto:494,218;5:click;7:goto:200,222;8:click",
    # Campaign page: Play Mission (230,177); difficulty: Easy (240,320) Normal (400,320) Hard (560,320)
    "mission-menus": "0:goto:400,253;1:click;4:goto:494,218;5:click;7:goto:200,222;8:click"
                     ";11:goto:230,177;12:click;15:goto:400,320;16:click",
    # deletes the third profile (the one campaign-new-ok adds) after its confirmation fades in
    "campaign-delete-3": "0:goto:400,253;1:click;4:goto:494,262;5:click;7:goto:200,312;8:click;16:goto:296,320;17:click",
    "campaign-new": "0:goto:400,253;1:click;4:goto:200,267;5:click",
    # name entry: a key press is a DirectInput key event plus WM_CHAR; "TEST", Backspace (DIK 14), "T"
    "campaign-new-type": "0:goto:400,253;1:click;4:goto:200,267;5:click"
                         ";7:char:84;7:key:20;7.5:char:69;7.5:key:18;8:char:83;8:key:31;8.5:char:84;8.5:key:20"
                         ";9:key:14;9.5:char:84;9.5:key:20",
    "campaign-new-ok": "0:goto:400,253;1:click;4:goto:200,267;5:click"
                       ";7:char:66;7:key:48;7.5:char:79;7.5:key:24;8:char:84;8:key:20;10:goto:588,517;11:click",
}
# --walk: name -> dict(goto=PAGE | input=SCRIPT | mission=SCN, after=seconds after the main menu for the screenshot
# (None: no shot), want=the last page requested ("exit": the process must end with WinMain's return code; None: not
# checked)).
WALK = {
    "menu": dict(after=3, want="Menus/P_MePrin.fbs"),
    "campaign": dict(goto="P_Profil", after=3, want="Menus/P_Profil.fbs"),
    "credits": dict(goto="P_Credits", after=3, want="Menus/P_Credits.fbs"),
    "settings": dict(goto="P_Option", after=3, want="Menus/P_Option.fbs"),
    "settings-controls": dict(goto="P_Contro", after=3, want="Menus/P_Contro.fbs"),
    "settings-video": dict(goto="P_Video", after=3, want="Menus/P_Video.fbs"),
    "settings-sound": dict(goto="P_Sound", after=3, want="Menus/P_Sound.fbs"),
    "settings-game": dict(goto="P_MOG", after=3, want="Menus/P_MOG.fbs"),
    "exit": dict(input="exit", after=3, want="Menus/P_MsgBox.fbs"),
    "exit-ok": dict(input="exit-ok", after=None, want="exit"),
    "campaign-select": dict(input="campaign-select", after=10, want="Menus/P_Campan.fbs"),
    "campaign-new": dict(input="campaign-new", after=7, want="Menus/P_Edit.fbs"),
    "campaign-new-type": dict(input="campaign-new-type", after=11, want="Menus/P_Edit.fbs"),
    "campaign-new-ok": dict(input="campaign-new-ok", after=14, want=None),
    "mission": dict(mission="Maps/FR01/Ransom.scn", after=60, want=None),
}


def thumbnails(dirpath, tag):
    """640-wide JPEGs of the run's back-buffer PNGs (standalone/harness.cpp writes DIR/shots/TAG-T.png)."""
    import glob
    for png in glob.glob(os.path.join(dirpath, "shots", f"{tag}-*.png")):
        try:
            from PIL import Image
            im = Image.open(png)
            im.thumbnail((640, 480))
            im.convert("RGB").save(png[:-4] + ".jpg", quality=85)
        except Exception:
            pass


def winpath(path):
    return subprocess.run(["wslpath", "-w", path], capture_output=True, text=True).stdout.strip()


def saves_dir():
    """The game's profiles and saves (Documents/CSF Data), shared with the original game the user plays."""
    home = subprocess.run(["cmd.exe", "/c", "echo %USERPROFILE%"], capture_output=True, text=True,
                          cwd="/mnt/c").stdout.strip()
    path = subprocess.run(["wslpath", "-u", home], capture_output=True, text=True).stdout.strip() if home else ""
    return os.path.join(path, "Documents", "CSF Data") if path else None


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
        w = WALK[name]
        want, after = w.get("want"), w.get("after")
        shot = f"m{after:g}" if after is not None else None
        cmd = [sys.executable, os.path.abspath(__file__), "--full", "--timeout", str(timeout), "--tag", name]
        for key in ("goto", "input", "mission"):
            if w.get(key):
                cmd += [f"--{key}", w[key]]
        if shot:
            cmd += ["--shots", shot]
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
        ended = want != "exit" and code != "timeout"  # the run should still be going at the timeout
        ok = not exc and not trap and not ended and (want != "exit" or (ret and code == ret.group(1))) and (
            want in (None, "exit") or
                                                                           (pages and pages[-1] == want))
        bad += not ok
        r = dict(name=name, ok=ok, exit=code, pages=pages, trap=trap.group(0) if trap else None,
                 exception=exc.group(0) if exc else None,
                 shot=f"/mnt/e/dev/csf-standalone/shots/{name}-{shot}.jpg" if shot else None)
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
    ap.add_argument("--show", action="store_true", help="a visible 1280x960 window (default: offscreen and silent)")
    ap.add_argument("--goto", help="open this menu page directly once the main menu is up (e.g. P_Option)")
    ap.add_argument("--mission", help="start this scenario directly once the main menu is up (e.g. Maps/FR01/Ransom.scn)")
    ap.add_argument("--intro", action="store_true", help="play the intro movies (default: skipped)")
    ap.add_argument("--quit-after", type=float, help="close the game window this many seconds after the first frame")
    a = ap.parse_args()
    if a.walk is not None:
        sys.exit(walk([n for n in a.walk.split(",") if n] or [n for n in WALK if n != "mission"],
                        a.timeout if a.timeout != 60 else 30))
    if a.input:
        os.environ["CSF_INPUT"] = SCRIPTS.get(a.input, a.input)
    if a.full:
        # standalone/harness.cpp. Never a full-screen window on the user's desktop: offscreen (no focus, no input
        # taken, no sound) unless --show asks for a visible 1280x960 window.
        os.environ["CSF_DISPLAY"] = "window" if a.show else "offscreen"
        if not a.intro:
            os.environ.setdefault("CSF_SKIP_INTRO", "1")
        if a.goto:
            os.environ["CSF_GOTO"] = a.goto
        if a.mission:
            os.environ["CSF_MISSION"] = a.mission
        if a.quit_after:
            os.environ["CSF_QUIT_AFTER"] = f"{a.quit_after:g}"
        if a.shots:
            os.makedirs(os.path.join(a.dir, "shots"), exist_ok=True)
            os.environ.update(CSF_SHOTS=a.shots, CSF_SHOT_DIR=winpath(os.path.join(a.dir, "shots")), CSF_SHOT_TAG=a.tag)
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
    for v in ("CSF_RW_TRACE", "CSF_HANG_SECONDS", "CSF_NO_DIALOGS", "CSF_INPUT", "CSF_EXIT_TRACE", "CSF_EXCEPTION_TRACE", "CSF_PAGE_TRACE", "CSF_HEAP_CHECK",
              "CSF_DISPLAY", "CSF_SKIP_INTRO", "CSF_GOTO", "CSF_MISSION", "CSF_QUIT_AFTER", "CSF_SHOTS", "CSF_SHOT_DIR",
              "CSF_SHOT_TAG", "CSF_AUDIO_BACKEND"):  # HANG: the exe dumps its main thread then (tools/dbg/stdsym.py)
        if v in os.environ:
            exports.append(v + "/w")
    env = dict(os.environ, CSF_DRYRUN="0" if a.full else "1",
               WSLENV=":".join(e for e in exports if e))
    # Scripted runs create and delete profiles: keep the user's saves as they were (CSF_KEEP_SAVES=1 keeps the run's).
    saves = saves_dir() if a.full else None
    backup = os.path.join(ROOT, "work", "csfdata-run")
    if saves and os.path.isdir(saves):
        shutil.rmtree(backup, ignore_errors=True)
        shutil.copytree(saves, backup)
    t0 = time.time()
    try:
        p = subprocess.run(["./csf_standalone.exe"], cwd=a.dir, env=env, capture_output=True, text=True,
                           errors="replace", timeout=a.timeout)
        out, code = p.stdout + p.stderr, p.returncode
    except subprocess.TimeoutExpired as e:
        out, code = ((e.stdout or b"") + (e.stderr or b"")).decode(errors="replace"), "timeout"
        subprocess.run(["taskkill.exe", "/IM", "csf_standalone.exe", "/F"], capture_output=True)
    if a.full and a.shots:
        thumbnails(a.dir, a.tag)
    if saves and os.path.isdir(backup) and os.environ.get("CSF_KEEP_SAVES") != "1":
        time.sleep(1)  # the killed process may still hold a file for a moment
        shutil.rmtree(saves, ignore_errors=True)
        shutil.copytree(backup, saves)
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
