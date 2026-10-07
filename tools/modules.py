#!/usr/bin/env python3
"""Module carving: split the game code into address-contiguous modules (~ the original .cpp/.obj files).

MSVC lays out each .obj's functions contiguously in link order (verified on NetZ: its 109 __FILE__-named .cpp files
are 109 contiguous runs), so a module is an address range. The ranges are the optimal contiguous partition
(dynamic programming) under modularity with resolution RES and a per-module cost PENALTY, over this affinity graph:
  call    a calls b (either direction)                                            1
  class   both are methods/ctors of the same class (db/classes.json)              1/(n-1), n = its methods
  data    both reference the same global, vtable, constant or string              1/(r-1), r = referencing fns
          (items referenced by > MAXREF functions are shared infrastructure: ignored)
  rdata   they reference neighbouring data items (<= ADJ_GAP bytes apart): an     ADJ/(r1*r2)
          .obj's literals and constants are emitted together
RES/PENALTY are calibrated on NetZ, the one region with ground truth (`calibrate`: 85% of same-file neighbours kept
together, 81% of file boundaries found). STL template instances (module=stl) are not carved.

Usage:
  modules.py carve            write db/modules.yaml (names reviewed by Opus survive a re-carve when a new module
                              overlaps the old one by > 50%) and the module column of db/functions.csv
  modules.py show <id|ea>     one module: range, classes, strings, named functions (evidence for naming it)
  modules.py list             one line per module
  modules.py name FILE        apply reviewed names: lines "<ea> <name> [note]" (ea = any address inside the module)
  modules.py calibrate [--res 4,8,12] [--penalty 0.5,1,2]   score settings against NetZ's __FILE__ names
"""
import argparse, collections, csv, json, os, re, sys

sys.path.insert(0, os.path.dirname(__file__))
ROOT = os.path.dirname(os.path.dirname(os.path.abspath(__file__)))
OUT = os.path.join(ROOT, "db", "modules.yaml")
CSV = os.path.join(ROOT, "db", "functions.csv")
GAME_RANGES = [(0x401000, 0x566000), (0x690B10, 0x78D130), (0x7C1C70, 0x7C28F0)]
DATA = (0x82D000, 0x8C1000)  # .rdata + .data/.bss
MAXREF = 25
ADJ, ADJ_GAP = 0.5, 64  # weight / max byte distance of neighbouring data items
MINFNS = 3  # smaller modules join a neighbour (merge_tiny)
MAXLEN = 800  # longest module considered, in functions
RES, PENALTY = 8.0, 1.0  # calibrated on NetZ's __FILE__ ground truth (`modules.py calibrate`)


def load():
    rows = {int(r["ea"], 16): r for r in csv.DictReader(open(CSV))}
    F = {}
    for l in open(os.path.join(ROOT, "export", "functions.jsonl")):
        f = json.loads(l)
        F[f["ea"]] = f
    classes = json.load(open(os.path.join(ROOT, "db", "classes.json")))
    return rows, F, classes


NETZ_RANGES = [(0x6465B0, 0x690B10), (0x78D130, 0x7AE440)]


def sequences(rows, F, ranges=GAME_RANGES, region="game"):
    """Per address range, the ordered functions taking part in clustering."""
    seqs = []
    for lo, hi in ranges:
        s = [ea for ea in sorted(F) if lo <= ea < hi and rows.get(ea, {}).get("region") == region
             and rows[ea]["module"] != "stl"]
        if s:
            seqs.append(s)
    return seqs


def edges(rows, F, classes, nodes):
    w = collections.Counter()

    def add(a, b, x):
        if a != b and a in nodes and b in nodes:
            w[(a, b) if a < b else (b, a)] += x

    for a in nodes:
        for b in F[a]["callees"]:
            add(a, b, 1.0)
    for c in classes:
        ms = sorted({int(m, 16) for m in c["methods"]} | {int(m, 16) for m in c["ctors"]})
        ms = [m for m in ms if m in nodes]
        if 2 <= len(ms) <= 200:
            x = 1.0 / (len(ms) - 1)
            for i, a in enumerate(ms):
                for b in ms[i + 1:]:
                    add(a, b, x)
    S = _strings()
    refs = collections.defaultdict(list)
    for a in nodes:
        for d in set(F[a]["drefs"]) | set(F[a]["srefs"]):
            if DATA[0] <= d < DATA[1] and not S.get(d, "").endswith(".cpp"):  # __FILE__: ground truth only
                refs[d].append(a)
    for d, fs in refs.items():
        if 2 <= len(fs) <= MAXREF:
            x = 1.0 / (len(fs) - 1)
            for i, a in enumerate(fs):
                for b in fs[i + 1:]:
                    add(a, b, x)
    # an .obj's literals/constants are emitted together: referrers of neighbouring data items are related
    items = sorted(d for d, fs in refs.items() if len(fs) <= MAXREF)
    for d1, d2 in zip(items, items[1:]):
        if d2 - d1 <= ADJ_GAP:
            r1, r2 = refs[d1], refs[d2]
            x = ADJ / (len(r1) * len(r2))
            for a in r1:
                for b in r2:
                    add(a, b, x)
    return w


def cluster(seq, w, res=None, penalty=None, maxlen=None, labels=None):
    """Optimal contiguous partition of one ordered sequence (dynamic programming over segment ends) maximising
    modularity: sum over segments of in(s)/m - gamma * (deg(s)/2m)^2 - penalty/m, segments at most `maxlen`
    functions; gamma = res. (Scaling gamma with the sequence's edge weight was tried: worse on NetZ.)
    labels {ea: file}: known source files; no segment may contain two different ones."""
    import numpy as np
    L = maxlen or MAXLEN
    res = RES if res is None else res
    penalty = PENALTY if penalty is None else penalty
    idx = {ea: i for i, ea in enumerate(seq)}
    n = len(seq)
    lower = [[] for _ in range(n)]  # node q -> [(a < q, weight)]
    deg = np.zeros(n)
    for (a, b), x in w.items():
        if a in idx and b in idx:
            i, j = sorted((idx[a], idx[b]))
            lower[j].append((i, x))
            deg[i] += x
            deg[j] += x
    m = deg.sum() / 2 or 1.0
    gamma = res
    D = np.concatenate([[0.0], np.cumsum(deg)])  # D[k] = total degree of nodes [0, k)
    best = np.full(n + 1, -np.inf)
    best[0] = 0.0
    back = np.zeros(n + 1, dtype=np.int64)
    inw = np.zeros(L)  # inw[t]: internal weight of segment [j - L + t, j) for the current end j
    off = np.arange(L)
    labels = labels or {}
    last, wall = None, -1  # last known label (position, file); segments must start after `wall`
    for j in range(1, n + 1):
        q = j - 1  # node joining every candidate segment
        f = labels.get(seq[q])
        if f is not None:
            if last and last[1] != f:
                wall = last[0]
            last = (q, f)
        add = np.zeros(L)
        for a, x in lower[q]:
            t = a - (j - L)
            if t >= 0:
                add[t] += x
        inw = np.concatenate([inw[1:], [0.0]]) + np.cumsum(add[::-1])[::-1]
        ii = j - L + off
        ok = ii > wall
        iv = ii[ok]
        sc = best[iv] + inw[ok] / m - gamma * ((D[j] - D[iv]) / (2 * m)) ** 2 - penalty / m
        k = int(np.argmax(sc))
        best[j] = sc[k]
        back[j] = iv[k]
    cuts, j = [], n
    while j > 0:
        cuts.append((back[j], j))
        j = back[j]
    return [seq[a:b] for a, b in reversed(cuts)]


def merge_tiny(segs, w, labels=None, minfns=None):
    """Segments of < MINFNS functions join the neighbour they have more affinity to (hub utilities such as the
    allocator are cheap to isolate under modularity, but .obj files of 1-2 functions are rare). Known files stay apart."""
    minfns = MINFNS if minfns is None else minfns
    labels = labels or {}
    nbw = collections.defaultdict(float)
    for (a, b), x in w.items():
        nbw[a, b] = nbw[b, a] = x
    adj = collections.defaultdict(list)
    for a, b in list(nbw):
        adj[a].append(b)
    segs = [list(x) for x in segs]
    changed = True
    while changed:
        changed = False
        for i, seg in enumerate(segs):
            if len(seg) >= minfns or len(segs) == 1:
                continue
            own = set(seg)
            files = {labels[e] for e in seg if e in labels}

            def aff(j):
                if j < 0 or j >= len(segs):
                    return -1.0
                other = {labels[e] for e in segs[j] if e in labels}
                if files and other and files != other:
                    return -1.0
                o = set(segs[j])
                return sum(nbw[a, b] for a in own for b in adj[a] if b in o)
            l, r = aff(i - 1), aff(i + 1)
            if max(l, r) < 0:
                continue
            j = i - 1 if l >= r else i + 1
            lo, hi = min(i, j), max(i, j)
            segs[lo:hi + 1] = [segs[lo] + segs[hi]]
            changed = True
            break
    return segs


_S = None


def _strings():
    global _S
    if _S is None:
        _S = {int(a, 16): v for a, v in json.load(open(os.path.join(ROOT, "export", "strings.json"))).items()}
    return _S


def file_labels(F):
    """Ground truth where the binary has it: functions referencing a __FILE__-style "X.cpp" string (NetZ asserts)."""
    S = _strings()
    lab = {}
    for ea, f in F.items():
        c = [re.split(r"[\\/]", x)[-1] for x in (S.get(a, "") for a in f["srefs"]) if x.endswith(".cpp")]
        if c:
            lab[ea] = collections.Counter(c).most_common(1)[0][0]
    return lab


def score(segs, lab):
    """Consecutive labelled pairs: same file kept together / different files split. Returns (together, split, n)."""
    mod = {ea: i for i, s in enumerate(segs) for ea in s}
    seq = sorted(ea for ea in lab if ea in mod)
    same = [mod[a] == mod[b] for a, b in zip(seq, seq[1:]) if lab[a] == lab[b]]
    diff = [mod[a] != mod[b] for a, b in zip(seq, seq[1:]) if lab[a] != lab[b]]
    return sum(same) / max(1, len(same)), sum(diff) / max(1, len(diff)), (len(same), len(diff))


def calibrate(ress, pens, minfns=None):
    rows, F, classes = load()
    seqs = sequences(rows, F, NETZ_RANGES, "netz")
    nodes = {ea for s in seqs for ea in s}
    w = edges(rows, F, classes, nodes)
    lab = {ea: v for ea, v in file_labels(F).items() if ea in nodes}
    print(f"NetZ ground truth: {len(lab)} functions in {len(set(lab.values()))} files")
    for g in ress:
        for p in pens:
            segs = [x for q in seqs for x in merge_tiny(cluster(q, w, g, p), w, minfns=minfns)]
            t, sp, n = score(segs, lab)
            print(f"  res {g:5} penalty {p:5}: {len(segs):4} modules  together {t:.2f} (of {n[0]})  "
                  f"split {sp:.2f} (of {n[1]})  balanced {(t + sp) / 2:.2f}")


# ---- labelling and output

def _owned_classes(classes, mod):
    """{module index: Counter(vtable -> methods in it)} from each class's methods/ctors."""
    out = collections.defaultdict(collections.Counter)
    for c in classes:
        for m in set(c["methods"]) | set(c["ctors"]):
            i = mod.get(int(m, 16))
            if i is not None:
                out[i][c["vtable"]] += 1
    return out


def _good_string(x):
    return 4 <= len(x) <= 48 and re.search(r"[A-Za-z]{3}", x) and not re.fullmatch(r"[%\w.: -]*%[-\w.]*[dsfx]\W*", x)


def describe(segs, rows, F, classes):
    from gen_classes_h import load_names
    names = load_names()
    S = _strings()
    mod = {ea: i for i, s in enumerate(segs) for ea in s}
    owned = _owned_classes(classes, mod)
    nref = collections.Counter(d for f in F.values() for d in set(f["srefs"]))
    out = []
    for i, seg in enumerate(segs):
        cls = [(names[v]["name"] if v in names else f"C_{v}", k) for v, k in owned[i].most_common() if k >= 2][:6]
        strs = collections.Counter()
        for ea in seg:
            for a in set(F[ea]["srefs"]):
                x = S.get(a, "")
                if _good_string(x) and nref[a] <= 3:
                    strs[x] += 1
        named = [rows[ea]["name"] for ea in seg if rows[ea]["name"]]
        pref = collections.Counter(n.split("_")[0] for n in named
                                   if re.match(r"[CT][A-Z][A-Za-z0-9]+_", n))  # CGuiButton_Draw, not C_0082D410_x
        if cls and not cls[0][0].startswith("C_"):
            label, src = cls[0][0], "class"
        elif pref and pref.most_common(1)[0][1] >= 2:
            label, src = pref.most_common(1)[0][0], "names"
        else:
            label, src = "", ""
        out.append(dict(classes=cls, strings=[x for x, _ in strs.most_common(5)], label=label, label_src=src,
                        named=len(named)))
    return out


def _yq(x):
    return json.dumps(x, ensure_ascii=False)


def write_yaml(mods):
    lines = ["# Modules of the game code (tools/modules.py carve): address-contiguous ranges ~ original .obj files.",
             "# id = m<start>. name: empty until reviewed; name_src opus = reviewed (kept across re-carves).",
             "# label = automatic hint (dominant named class or grunt name prefix). end is exclusive.",
             "# fns/kb exclude STL instances inside the range. classes = classes with >= 2 methods here.", ""]
    for m in mods:
        lines.append(f"- {{id: {m['id']}, start: 0x{m['start']:X}, end: 0x{m['end']:X}, fns: {m['fns']}, "
                     f"kb: {m['kb']}, done: {m['done']}, name: {_yq(m.get('name', ''))}, "
                     f"name_src: {m.get('name_src', '')}, label: {_yq(m['label'])}, cohesion: {m['cohesion']},")
        lines.append(f"   classes: {_yq([c for c, _ in m['classes']])}, strings: {_yq(m['strings'])}" +
                     (f", note: {_yq(m['note'])}" if m.get("note") else "") + "}")
    open(OUT, "w").write("\n".join(lines) + "\n")


def read_yaml():
    import yaml
    return yaml.safe_load(open(OUT)) if os.path.exists(OUT) else []


def carve():
    rows, F, classes = load()
    seqs = sequences(rows, F)
    nodes = {ea for s in seqs for ea in s}
    w = edges(rows, F, classes, nodes)
    lab, desc_file = file_labels(F), {}
    segs, seq_end = [], set()
    for q in seqs:
        segs += merge_tiny(cluster(q, w, labels=lab), w, lab)
        seq_end.add(len(segs) - 1)  # last module of a sequence ends at its last function, not at the next module
    mod = {ea: i for i, s in enumerate(segs) for ea in s}
    for i, seg in enumerate(segs):
        fs = sorted({lab[e] for e in seg if e in lab})
        if fs:
            desc_file[i] = fs[0]
    inner, deg = collections.Counter(), collections.Counter()
    for (a, b), x in w.items():
        deg[mod[a]] += x
        deg[mod[b]] += x
        if mod[a] == mod[b]:
            inner[mod[a]] += 2 * x
    desc = describe(segs, rows, F, classes)
    for i, f in desc_file.items():
        desc[i].update(label=f.rsplit(".", 1)[0], label_src="file")
    old = read_yaml()
    mods = []
    for i, seg in enumerate(segs):
        start, last = seg[0], seg[-1]
        nxt = F[last]["end"] if i in seq_end else segs[i + 1][0]
        m = dict(id=f"m{start:06X}", start=start, end=nxt, fns=len(seg),
                 kb=round(sum(F[e]["size"] for e in seg) / 1024, 1),
                 done=sum(rows[e]["status"] in ("done", "verified") for e in seg),
                 cohesion=round(inner[i] / deg[i], 2) if deg[i] else 0.0, **desc[i])
        size = m["end"] - m["start"]
        for o in old:
            if o.get("name_src") == "opus":
                ov = min(m["end"], o["end"]) - max(m["start"], o["start"])
                if ov > 0.5 * size and ov > 0.5 * (o["end"] - o["start"]):
                    m.update(name=o["name"], name_src="opus", note=o.get("note", ""))
        mods.append(m)
    write_yaml(mods)
    # module column of functions.csv: carved functions get their module id (STL and libraries keep theirs)
    for i, seg in enumerate(segs):
        for ea in seg:
            rows[ea]["module"] = mods[i]["id"]
    from triage import FIELDS
    with open(CSV, "w", newline="") as f:
        wr = csv.DictWriter(f, fieldnames=FIELDS, lineterminator="\n")
        wr.writeheader()
        for ea in sorted(rows):
            wr.writerow(rows[ea])
    kb = sorted(m["kb"] for m in mods)
    print(f"{len(mods)} modules over {len(nodes)} functions; KB median {kb[len(kb) // 2]}, max {kb[-1]}; "
          f"{sum(1 for m in mods if m.get('name_src') == 'opus')} reviewed names kept -> db/modules.yaml")


def module_of(key, mods):
    """By id (m4F9190), reviewed name (dev_console) or any address inside it."""
    for m in mods:
        if key.lower() in (m["id"].lower(), (m.get("name") or "").lower()):
            return m
    try:
        ea = int(key[1:] if re.fullmatch(r"m[0-9A-Fa-f]{6}", key) else key, 16)
    except ValueError:
        return None
    return next((m for m in mods if m["start"] <= ea < m["end"]), None)


def apply_names(path):
    mods = read_yaml()
    n = 0
    for line in open(path):
        line = line.strip()
        if not line or line.startswith("#"):
            continue
        ea, name, *note = line.split(None, 2)
        m = module_of(ea, mods)
        if not m:
            sys.exit(f"no module contains {ea}")
        m.update(name=name, name_src="opus", note=note[0] if note else m.get("note", ""))
        n += 1
    names = collections.Counter(m["name"] for m in mods if m.get("name"))
    dup = [k for k, v in names.items() if v > 1]
    if dup:
        sys.exit(f"duplicate module names: {dup}")
    for m in mods:
        m["classes"] = [(c, 0) for c in m["classes"]]
    write_yaml(mods)
    print(f"{n} names applied; {len(names)}/{len(mods)} modules named")


def show(key):
    mods = read_yaml()
    m = module_of(key, mods)
    if not m:
        sys.exit("no module there")
    rows, F, _ = load()
    print(f"{m['id']}  0x{m['start']:X}-0x{m['end']:X}  {m['fns']} fns  {m['kb']} KB  done {m['done']}  "
          f"cohesion {m['cohesion']}  name {m['name'] or '-'} ({m['name_src'] or 'unreviewed'})  label {m['label']}")
    print("classes:", ", ".join(m["classes"]) or "-")
    print("strings:", " | ".join(m["strings"]) or "-")
    S = _strings()
    for ea in sorted(e for e in F if m["start"] <= e < m["end"]):
        r = rows.get(ea, {})
        ss = [S[a] for a in F[ea]["srefs"] if a in S][:2]
        print(f"  {ea:08X} {F[ea]['size']:5} {r.get('status', ''):8} {r.get('module', '') if r.get('module') == 'stl' else '':4}"
              f"{r.get('name') or ''}  {' | '.join(x[:40] for x in ss)}")


def main():
    ap = argparse.ArgumentParser()
    sub = ap.add_subparsers(dest="cmd", required=True)
    sub.add_parser("carve")
    sub.add_parser("list")
    sub.add_parser("name").add_argument("file")
    s_ = sub.add_parser("show")
    s_.add_argument("key")
    c = sub.add_parser("calibrate")
    c.add_argument("--res", default="4,8,12")
    c.add_argument("--penalty", default="0.5,1,2")
    a = ap.parse_args()
    if a.cmd == "carve":
        carve()
    elif a.cmd == "show":
        show(a.key)
    elif a.cmd == "name":
        apply_names(a.file)
    elif a.cmd == "list":
        for m in read_yaml():
            print(f"{m['id']}  {m['fns']:4} fns {m['kb']:6} KB  done {m['done']:3}  coh {m['cohesion']:.2f}  "
                  f"{m['name'] or '(' + (m['label'] or '?') + ')'}")
    else:
        calibrate([float(x) for x in a.res.split(",")], [float(x) for x in a.penalty.split(",")])


if __name__ == "__main__":
    main()
