#!/usr/bin/env python3
"""Triage jobs for the escalation queue: one function per job, related ones together (user, 2026-10-13).

  qjobs.py plan [--max N]           the batches it would start
  qjobs.py start [--max N] [--agent haiku-hi]   start them as pool jobs (tools/deep_worker.py --pool), print the names
  qjobs.py start --sweep FILE ...   the same for installed functions that differ in `emudiff.py --tier` output

Batching: a queue item goes alone unless others share its module and failure class and lie within 0x400 bytes of
it (clones of one pattern, members of one class: the second is nearly free once the first is understood); then up
to 3 together. Functions over 300 bytes always go alone; --size picks the range (default up to 320 bytes, where Haiku singles succeed and cost cents; 2026-10-14: none of nine between 370 and 800 bytes passed: small
jobs; start the bigger ones with `--size 321-99999 --agent sol-med`). Skipped: items a job has already had (`--again` takes
them too), items whose queue file holds no candidate code, duplicate-hook notes.
After starting: `tools/deep_worker.py wait NAMES --adopt` in the background.
"""
import argparse, json, os, re, subprocess, sys

ROOT = os.path.dirname(os.path.dirname(os.path.abspath(__file__)))
sys.path.insert(0, os.path.join(ROOT, "tools"))
import deep_worker, grunt  # noqa: E402

TASK = """Queue triage, {n} function(s) taken out after a failed check. Addresses: {eas}
{lines}
Each queue file says why the function was taken out (the lines of the failed check: a crash or misbehaviour in the game after it was installed, an in-game comparison mismatch, or an emulated-run difference) and holds the rejected reimplementation in a cpp block. Work the functions one at a time.
For each: read the original's disassembly next to the rejected code line by line and find what the code gets wrong. Typical causes found so far: arguments in the wrong order or count at one call, a value passed where the original passes its address (or the reverse), a field offset off by 4, one dereference too many or too few on a vtable or table pointer, a register argument (edx, esi, edi) declared as a stack parameter, a condition that treats NaN differently from the original's fnstsw/test ah sequence, a store the original makes between two calls left out, a string/vector member written relative to the wrong base, a result register (eax) the original overwrites on a path the code skips.
- Code wrong (REAL): write the corrected file (start from the cpp block), run both checks, and quote the original instructions that prove the fix in one or two lines.
- Code matches the original instruction for instruction: say so (FALSE ALARM) and name what in the failure lines points away from this function. Write the unchanged candidate to src/auto/ only if both checks pass.
- Not decided: say so and what you would need. Do not guess.
Read the whole function against the disassembly, not only the place the failure points at: check each callee's real `ret N`, whether a result is used as a byte (al) or as the full eax (never bind a callee as returning `bool` where the original uses eax or compares al with values other than 0/1), and stack objects the original keeps contiguous. A `same` from few profiles, or a crash inside a callee, does not make a FALSE ALARM: say separately what is a shown code defect, what is a tool defect, and what stays unexplained.
Do not change anything under tools/ or hook/, and do not create files other than the listed ones under src/auto/. Stop after the listed function(s).
Report a table: address, verdict, cause in one line, final gate line, final emudiff line.
"""
SWEEP_TASK = """Installed function(s) whose emulated run differs from the original. Addresses: {eas}
{lines}
Each file under src/auto/ is the current reimplementation; it is installed in the game and passed the smoke tests, but `tools/emudiff.py` (the original's bytes and our compiled code run side by side in an emulator from the same registers, arguments and memory; calls leaving the function are answered by the same mock on both sides) reports the difference quoted with it: `X vs Y` is the original's value vs ours. Work the functions one at a time.
For each: run the emudiff command first and keep its line. Read the original's disassembly next to the file line by line, starting where the line points:
- `returns N vs 1` / `vs 0`: the original returns the whole eax (often a callee's result passed through, or a pointer), the file returns a bool or a truncated value; or a callee is bound as returning `bool`/`char` where the original uses the full register. Give the function and the binding the type the instructions show.
- `call K: target A vs B`: the K-th call (from 0) reaches another function: a wrong vtable slot offset, one dereference too many or too few, a different object's vtable, a call the file adds or drops before it.
- `call K to F: ecx A vs B` or `arguments [...]`: the K-th call passes another `this` or other arguments: wrong field offset, a value where the original passes an address, swapped or missing arguments, a stale register.
- `N dword(s) of memory differ, first at ADDR`: a store with another value, width or offset, or a store the file drops; a float product rounded to float where the original keeps it on the x87 stack until the conversion.
- `N calls vs M`: the two took different paths: an inverted or missing condition, a loop bound.
Correct the file with the smallest change that makes it do what the original does. Then `.venv/bin/python tools/gate.py <address> src/auto/<address>.cpp` must print ok and the emudiff command must print same. Quote the original instructions that prove the fix in one or two lines. If the file already does what the original does instruction for instruction, change nothing and say what in the emulator's line is an artefact (FALSE ALARM), with the instructions. If you cannot decide, say so and restore the file (`git checkout -- <file>`). Do not guess.
Read the whole function, not only the place the line points at. Never bind a callee as returning `bool` where the original uses eax or compares al with values other than 0/1.
Do not change anything under tools/ or hook/, and no files other than the listed ones. Stop after the listed function(s).
Report a table: address, verdict (REAL fixed / FALSE ALARM / not decided), cause in one line, final gate line, final emudiff line.
"""
SWEEP_LINE = ("- {ea}: file src/auto/{ea}.cpp, original `.venv/bin/python tools/packet.py {ea}` and export/funcs/{ea}.c, "
              "emudiff `.venv/bin/python tools/emudiff.py {ea} --src src/auto/{ea}.cpp` now prints: {why}; "
              "gate `.venv/bin/python tools/gate.py {ea} src/auto/{ea}.cpp`")
LINE = ("- {ea}: queue file queue/{ea}.md, original `.venv/bin/python tools/packet.py {ea}` and export/funcs/{ea}.c, "
        "write the corrected file to src/auto/{ea}.cpp, check with `.venv/bin/python tools/gate.py {ea} src/auto/{ea}.cpp` "
        "(must print ok) and `.venv/bin/python tools/emudiff.py {ea} --src src/auto/{ea}.cpp` (must print same)")


def items(again):
    rows = grunt.load_csv()
    had = set() if again else {e for j in deep_worker.jobs() for e in j.get("eas", [])}
    out = []
    for f in sorted(os.listdir(os.path.join(ROOT, "queue"))):
        ea = f[:8]
        text = open(os.path.join(ROOT, "queue", f), errors="replace").read()
        head = text.splitlines()[0] if text else ""
        cls = re.sub(r"^# Escalation 0x[0-9A-Fa-f]+:?\s*", "", head)
        cls = re.split(r"[(:]", cls)[0].strip()[:40] or "other"
        code = re.search(r"```cpp\n(.*?)```", text, re.S)
        if ea in had or "duplicate hook" in head or not (code and code.group(1).strip()):
            continue
        r = rows.get(int(ea, 16), {})
        out.append(dict(ea=ea, cls=cls, module=r.get("module", ""), size=int(r.get("size") or 0)))
    return out


def sweep_items(path, again):
    """Installed functions with an emudiff DIFF: lines of `emudiff.py` output (address DIFF n/m profiles; ...)."""
    rows = grunt.load_csv()
    had = set() if again else {e for j in deep_worker.jobs() for e in j.get("eas", [])}
    out = []
    for l in open(path):
        m = re.match(r"([0-9a-f]{8}) DIFF\s+\S+ profiles; (.*)", l.strip())
        if not m or m.group(1) in had:
            continue
        ea, why = m.groups()
        r = rows.get(int(ea, 16), {})
        cls = re.sub(r"[0-9a-f]{3,8}|\d+", "N", re.sub(r"^profile \d+: ", "", why))[:24]
        out.append(dict(ea=ea, cls=cls, module=r.get("module", ""), size=int(r.get("size") or 0), why=why[:200]))
    return out


def batches(its):
    left, out = list(its), []
    while left:
        a = left.pop(0)
        b = [a]
        if a["size"] <= 300:
            for o in list(left):
                if len(b) < 3 and o["size"] <= 300 and (o["module"], o["cls"]) == (a["module"], a["cls"]) \
                        and abs(int(o["ea"], 16) - int(a["ea"], 16)) <= 0x400:
                    b.append(o)
                    left.remove(o)
        out.append(b)
    return out


def main():
    ap = argparse.ArgumentParser(description=__doc__, formatter_class=argparse.RawDescriptionHelpFormatter)
    ap.add_argument("cmd", choices=["plan", "start"])
    ap.add_argument("--max", type=int, default=8, help="jobs to start at most")
    ap.add_argument("--agent", default="haiku-hi")
    ap.add_argument("--again", action="store_true")
    ap.add_argument("--sweep", help="instead of queue/: a file of emudiff output lines (installed functions that DIFF)")
    ap.add_argument("--size", default="0-320", help="function size range in bytes, LO-HI (bigger ones: --agent sol-med)")
    a = ap.parse_args()
    lo, hi = map(int, a.size.split("-"))
    its = sweep_items(a.sweep, a.again) if a.sweep else items(a.again)
    bs = batches([i for i in its if lo <= i["size"] <= hi])
    print(f"{sum(map(len, bs))} queue items to triage in {len(bs)} jobs ({sum(len(b) > 1 for b in bs)} batched); "
          f"taking {min(a.max, len(bs))}", file=sys.stderr)
    names = []
    for b in bs[:a.max]:
        name = ("s" if a.sweep else "q") + b[0]["ea"][2:] + (f"x{len(b)}" if len(b) > 1 else "")
        if a.cmd == "plan":
            print(name, " ".join(f"{i['ea']}({i['size']}B {i['module']} {i['cls']})" for i in b))
            continue
        if a.sweep:
            task = SWEEP_TASK.format(eas=" ".join(i["ea"] for i in b),
                                     lines="\n".join(SWEEP_LINE.format(ea=i["ea"], why=i["why"]) for i in b))
        else:
            task = TASK.format(n=len(b), eas=" ".join(i["ea"] for i in b), lines="\n".join(LINE.format(ea=i["ea"]) for i in b))
        r = subprocess.run([sys.executable, os.path.join(ROOT, "tools", "deep_worker.py"), "start", name, "--task", "-",
                            "--agent", a.agent, "--pool", "--force", "--timeout", "1200"], input=task, text=True,
                           capture_output=True)
        if r.returncode:
            print(f"{name}: not started: {(r.stdout + r.stderr).strip()[-200:]}", file=sys.stderr)
            continue
        names.append(name)
    if names:
        print(" ".join(names))
    return 0


if __name__ == "__main__":
    sys.exit(main())
