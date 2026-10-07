#!/usr/bin/env python3
"""Verification ledger: how strongly each reimplemented function has been checked, tied to its compiled code.

db/verify.json, per src/auto function: {"code": hash of the compiled code, "hits": max calls in one smoke,
"runs": smokes that executed it, "diffed": differential comparisons, "mismatched": mismatching ones,
"since": date this code was first seen, "last": date of the last evidence}.
The code hash covers the function's own object code (instruction bytes + relocation types; symbol names, the HOOK
registration stub and the file's other metadata are left out), so renames don't invalidate evidence but any change
to what the function compiles to does — e.g. a class layout change in types/auto_classes.h. Evidence recorded for
another hash is STALE: the code that was exercised is not the code that runs now.

Tiers of a done function (strongest first): diffed (compared in-game against the original, 0 mismatches) >
executed (ran in a smoke without crashing) > gate (static checks only; never reached in a smoke) ; plus stale and
mismatched.

Usage:
  verify.py ingest          read the play copy's csf_hook_hits.txt + csf_diff_summary.txt (smoke.sh runs this after
                            SMOKE OK; CSF_NO_LEDGER=1 skips it) — the build dir must be what was deployed
  verify.py report [--list TIER]   tier counts (functions and bytes); --list prints the addresses of one tier
  verify.py stale           rebuild, list functions whose code changed since their evidence (exit 1 if any)
"""
import argparse, concurrent.futures as cf, csv, datetime, glob, hashlib, json, os, re, subprocess, sys

ROOT = os.path.dirname(os.path.dirname(os.path.abspath(__file__)))
LEDGER = os.path.join(ROOT, "db", "verify.json")
CACHE = os.path.join(ROOT, "work", "codehash_cache.json")
OBJ_DIR = os.path.join(ROOT, "build", "CMakeFiles", "csf_hook.dir", "src", "auto")
PLAY = os.environ.get("CSF_PLAY", "/mnt/e/dev/csf-play")
TIERS = ["diffed", "executed", "gate", "stale", "mismatched"]


def _normalize(dis):
    out, skip = [], False
    for line in dis.splitlines()[3:]:  # header: file format, blank, "Disassembly of section"
        m = re.match(r"^[0-9a-f]+ <(.+)>:$", line)
        if m:
            skip = m.group(1).startswith("??__E")  # HOOK registration stub (carries the name and a counter)
            out.append("<fn>")
            continue
        if skip or not line.strip():
            continue
        if line.startswith("Disassembly of section"):
            out.append(line)
            continue
        if "IMAGE_REL_" in line:
            out.append(re.search(r"IMAGE_REL_\w+", line).group(0))  # relocation kind, not the symbol name
            continue
        out.append(re.sub(r"\s*<[^>]*>", "", line.split(":", 1)[1] if ":" in line else line).strip())
    return "\n".join(out)


def code_hash(obj):
    dis = subprocess.run(["llvm-objdump-21", "-d", "-r", "--x86-asm-syntax=intel", obj],
                         capture_output=True, text=True).stdout
    return hashlib.sha1(_normalize(dis).encode()).hexdigest()[:16]


def code_hashes():
    """{ea: hash} for every src/auto function with a built object (cached by object mtime+size)."""
    try:
        cache = json.load(open(CACHE))
    except (OSError, ValueError):
        cache = {}
    objs = sorted(glob.glob(os.path.join(OBJ_DIR, "*.cpp.obj")))

    def one(obj):
        st = os.stat(obj)
        key = f"{st.st_mtime_ns}:{st.st_size}"
        c = cache.get(obj)
        return obj, key, c[1] if c and c[0] == key else code_hash(obj)

    out = {}
    with cf.ThreadPoolExecutor(16) as ex:
        for obj, key, h in ex.map(one, objs):
            cache[obj] = [key, h]
            src = os.path.join(ROOT, "src", "auto", os.path.basename(obj)[:-4])
            if os.path.exists(src):
                out[f"{os.path.basename(obj)[:8]}"] = h
    os.makedirs(os.path.dirname(CACHE), exist_ok=True)
    json.dump(cache, open(CACHE, "w"))
    return out


def load():
    try:
        return json.load(open(LEDGER))
    except (OSError, ValueError):
        return {}


def save(led):
    with open(LEDGER, "w") as f:
        json.dump(dict(sorted(led.items())), f, indent=0, separators=(",", ":"))
        f.write("\n")


def _read(name):
    p = os.path.join(PLAY, name)
    return open(p, errors="replace").read().replace("\r", "") if os.path.exists(p) else ""


def ingest():
    hashes = code_hashes()
    led = load()
    today = datetime.date.today().isoformat()
    hits = {}
    for line in _read("csf_hook_hits.txt").splitlines():
        f = line.split()
        if len(f) >= 2 and re.fullmatch(r"[0-9a-fA-F]{8}", f[0]):
            hits[f[0].lower()] = int(f[1])
    diffs = {}
    for line in _read("csf_diff_summary.txt").splitlines():
        f = line.split()
        if len(f) >= 4 and re.fullmatch(r"[0-9a-fA-F]{8}", f[0]):
            diffs[f[0].lower()] = (int(f[1]), int(f[3]))
    n_new = n_ev = 0
    for ea, h in hashes.items():
        e = led.get(ea)
        if e is None or e["code"] != h:  # new code: earlier evidence was for other code
            e = led[ea] = dict(code=h, hits=0, runs=0, diffed=0, mismatched=0, since=today, last=None)
            n_new += 1
        if hits.get(ea, 0) > 0:
            e["hits"] = max(e["hits"], hits[ea])
            e["runs"] += 1
            e["last"] = today
            n_ev += 1
        if ea in diffs:
            e["diffed"] += diffs[ea][0]
            e["mismatched"] += diffs[ea][1]
    save(led)
    print(f"verify: {n_ev} functions executed in this smoke, {len(diffs)} diffed; {n_new} new code hashes")


def done_rows():
    with open(os.path.join(ROOT, "db", "functions.csv")) as f:
        return {r["ea"].lower().replace("0x", "").zfill(8): r for r in csv.DictReader(f) if r["status"] in
                ("done", "verified")}


def tiers(hashes=None):
    hashes = hashes if hashes is not None else code_hashes()
    led = load()
    out = {}
    for ea in hashes:
        e = led.get(ea)
        if e is None:
            t = "gate"
        elif e["code"] != hashes[ea]:
            t = "stale" if e["runs"] or e["diffed"] else "gate"
        elif e["mismatched"]:
            t = "mismatched"
        elif e["diffed"]:
            t = "diffed"
        elif e["runs"]:
            t = "executed"
        else:
            t = "gate"
        out[ea] = t
    return out


def summary_lines(hashes=None):
    """Short table for status.py: tier, functions, bytes."""
    t = tiers(hashes)
    rows = done_rows()
    size = {ea: int(r["size"]) for ea, r in rows.items()}
    lines = []
    for name in TIERS:
        eas = [ea for ea, v in t.items() if v == name]
        if eas or name in ("diffed", "executed", "gate"):
            lines.append(f"  {name:11} {len(eas):6} fns {sum(size.get(e, 0) for e in eas) / 1024:7.1f} KB")
    return lines


def main():
    ap = argparse.ArgumentParser()
    sub = ap.add_subparsers(dest="cmd", required=True)
    sub.add_parser("ingest")
    r = sub.add_parser("report")
    r.add_argument("--list", choices=TIERS)
    sub.add_parser("stale")
    a = ap.parse_args()
    if a.cmd == "ingest":
        ingest()
    elif a.cmd == "report":
        if a.list:
            rows = done_rows()
            for ea, v in sorted(tiers().items()):
                if v == a.list:
                    r = rows.get(ea, {})
                    print(f"{ea} {r.get('size', '?'):>5} {r.get('name', '')}")
        else:
            print("\n".join(summary_lines()))
    elif a.cmd == "stale":
        r = subprocess.run(["ninja", "-C", os.path.join(ROOT, "build")], capture_output=True, text=True)
        if r.returncode:
            sys.exit("build failed:\n" + (r.stdout + r.stderr)[-2000:])
        st = [ea for ea, v in sorted(tiers().items()) if v == "stale"]
        for ea in st:
            print(f"stale {ea}")
        print(f"{len(st)} functions changed since their evidence: re-smoke (smoke.sh), the next ingest re-records them")
        sys.exit(1 if st else 0)


if __name__ == "__main__":
    main()
