#!/usr/bin/env python3
"""Captain: naming / RE worker beside grunt (Claude via `claude -p`, one instance).

  .venv/bin/python tools/captain.py globals [--limit 89] [--min-modules 3] [--batch 6] [--model sonnet] [--dry]
  .venv/bin/python tools/captain.py packet 0x8AFC90          print one global's packet (debug)

Job `globals`: writable game globals shared by several modules, most used first. Related globals (adjacent, or
written by the same functions) go in one call so their names agree. Per global the packet holds mechanical facts
(section, initial value, extent, writers) and the *distinct* usage patterns over all accessors with counts (one
line stands for e.g. 81 call sites), plus the short callees that consume it. The answer is schema-checked JSON;
names/types are checked mechanically (convention, uniqueness, types that compile) and applied to db/globals.yaml
as `src: captain` (pinned: tools/globals.py won't override it), low confidence is held for Opus review
(work/captain/held.jsonl). Function names the model is sure of (helpers it read) are recorded as proposals.

Live state for tools/monitor.py: work/captain_state.json; per-call log: work/captain_log.jsonl.
"""
import argparse, collections, fcntl, json, os, re, struct, subprocess, sys, time
sys.path.insert(0, os.path.dirname(os.path.abspath(__file__)))
from packet import DB, pseudo, substitute_names  # noqa: E402
from semcheck import section_of  # noqa: E402
import globals as gl  # noqa: E402

ROOT = os.path.dirname(os.path.dirname(os.path.abspath(__file__)))
WORK = os.path.join(ROOT, "work", "captain")
STATE = os.path.join(ROOT, "work", "captain_state.json")
LOG = os.path.join(ROOT, "work", "captain_log.jsonl")
LOCK = os.path.join(ROOT, "work", "captain.lock")
CLAUDE_CWD = os.path.expanduser("~/.cache/csf-re-claude")
EXE = os.path.join(ROOT, "work", "pc", "CommXPC.exe")
PRIMS = {"std::int8_t", "std::uint8_t", "std::int16_t", "std::uint16_t", "std::int32_t", "std::uint32_t",
         "std::int64_t", "std::uint64_t", "int", "unsigned int", "bool", "char", "float", "double", "void*",
         "const char*", "char*", "const wchar_t*", "const char**"}
GENERIC_T = re.compile(r"\b(Concurrency::\w+(::\w+)*|std::_\w+|_RTL_\w+|_EXCEPTION\w*|CWnd|CObject)\b")

# ---------------------------------------------------------------- facts

_img = None


def read_u32(addr):
    """Initial dword at addr from the exe (0 for .bss-like tails beyond raw data)."""
    global _img
    if _img is None:
        import pefile
        pe = pefile.PE(EXE, fast_load=True)
        _img = (pe, {s.Name.rstrip(b"\0").decode(): s for s in pe.sections})
    pe, _ = _img
    try:
        return struct.unpack("<I", pe.get_data(addr - 0x400000, 4))[0]
    except Exception:
        return 0


def read_cstr(addr, n=80):
    """NUL-terminated printable string at addr (exe image), or None."""
    read_u32(addr)  # loads the image once
    pe = _img[0]
    try:
        b = pe.get_data(addr - 0x400000, n)
    except Exception:
        return None
    b = b.split(b"\0")[0]
    return b.decode("latin-1") if len(b) >= 2 and all(32 <= c < 127 or c in (9, 10) or c >= 0xA0 for c in b) else None


def describe_ptr(db, v):
    """What a pointer-looking initial value points to: string text, function, vtable/class, global."""
    if not 0x401000 <= v < 0x900000:
        return ""
    sec = section_of(v)
    if v in db.F:
        return f" -> function {db.name(v) or 'sub_%X' % v}"
    vt = f"{v:08x}"
    if vt in db.classes:
        return f" -> vtable of {db.cls_name(vt)}"
    if sec in (".rdata", ".data"):
        st = read_cstr(v)
        if st:
            return f' -> "{st}"'
        if v in db.globals:
            return f" -> global {db.globals[v]['name']}"
    return ""


def data_globals(db):
    """{addr: set(accessor eas)} for writable globals referenced by game code."""
    use = collections.defaultdict(set)
    for ea, r in db.rows.items():
        if r["region"] != "game" or ea not in db.F:
            continue
        for d in db.F[ea]["drefs"]:
            if section_of(d) in (".data", "_rwdseg"):
                use[d].add(ea)
    return use


def ida_tok(addr):
    return re.compile(rf"\b(?:dword|word|byte|flt|dbl|qword|off|unk|stru)_{addr:X}\b")


def norm_line(line, tok):
    """One pseudocode line as a usage pattern: the global as G, locals/args as v/a, IDA's bogus library types out."""
    s = tok.sub("G", line.strip())
    s = GENERIC_T.sub("T", s)
    s = re.sub(r"\bv\d+\b", "v", s)
    s = re.sub(r"\ba\d+\b", "a", s)
    s = re.sub(r"\bthis\b", "this", s)
    return s


def usage(db, addr, accessors, max_patterns=14):
    """Distinct usage patterns (with counts), writer lines, callees that receive G, modules."""
    tok = ida_tok(addr)
    pats, writes, consumers = collections.Counter(), collections.Counter(), collections.Counter()
    writers = set()
    for ea in accessors:
        src = pseudo(ea)
        if not src:
            continue
        for line in src.splitlines():
            if not tok.search(line):
                continue
            n = substitute_names(db, norm_line(line, tok))
            if re.search(r"\bG\s*=[^=]", n) or re.search(r"\bG\s*(\+\+|--|[-+|&^]=)", n):
                writes[n] += 1
                writers.add(ea)
            else:
                pats[n] += 1
            for m in re.finditer(r"\b(sub_[0-9A-F]+|[A-Za-z_][\w:]+)\((?:\([^()]*\))?G\b", n):
                consumers[m.group(1)] += 1
    mods = collections.Counter(db.rows[ea]["module"] for ea in accessors)
    return pats.most_common(max_patterns), writes.most_common(6), writers, consumers.most_common(4), mods


def module_name(mid, _cache={}):
    if not _cache:
        import yaml
        for m in yaml.safe_load(open(os.path.join(ROOT, "db", "modules.yaml"))) or []:
            _cache[m.get("id")] = m.get("name") or m.get("label") or m.get("id")
    return _cache.get(mid, mid)


def short_pseudo(db, name, limit=14):
    """Pseudocode of a small consumer function (by IDA or our name), or None when big/unknown."""
    m = re.fullmatch(r"sub_([0-9A-F]+)", name)
    ea = int(m.group(1), 16) if m else next((e for e, r in db.rows.items() if r["name"] == name), None)
    if ea is None:
        return None
    src = pseudo(ea)
    if not src:
        return None
    lines = [l for l in src.splitlines() if l.strip() and not re.match(r"\s*//", l)]
    if len(lines) > limit:
        return None
    return substitute_names(db, GENERIC_T.sub("T", "\n".join(lines)))


def global_packet(db, addr, accessors, nxt, neighbours=()):
    pats, writes, writers, consumers, mods = usage(db, addr, accessors)
    g = db.globals.get(addr)
    ext = min(nxt - addr, 32) if nxt else 4
    words = [read_u32(addr + i) for i in range(0, max(4, ext), 4)]
    out = [f"### 0x{addr:08X}  (section {section_of(addr)}, {nxt - addr if nxt else '?'} bytes to the next "
           f"referenced global)", "initial data: " + "  ".join(
               f"0x{w:08X}" + (describe_ptr(db, w) or (f" ({struct.unpack('<f', struct.pack('<I', w))[0]:g}f)"
                                                       if w and 0x30000000 < (w & 0x7FFFFFFF) < 0x4F000000 else ""))
               for w in words)]
    run = [a for a in range(addr - 16, addr + 20, 4) if a != addr and a in neighbours]
    if run:  # members of the same small struct/vector: their initial values say what it is
        out.append("neighbouring globals: " + ", ".join(
            f"0x{a:X}={read_u32(a):#x}" + (describe_ptr(db, read_u32(a)) or (
                f"({struct.unpack('<f', struct.pack('<I', read_u32(a)))[0]:g}f)"
                if 0x30000000 < (read_u32(a) & 0x7FFFFFFF) < 0x4F000000 else ""))
            + (f" {db.globals[a]['name']}" if a in db.globals else "") for a in run))
    if g:
        out.append(f"current name: {g['name']} ({g['type']}, src {g.get('src')})")
    out.append(f"referenced by {len(accessors)} functions in {len(mods)} modules: "
               + ", ".join(f"{module_name(m)}({n})" for m, n in mods.most_common(6)))
    if writes:
        out.append("writes (G = ...):")
        out += [f"  {n:4}x  {p}" for p, n in writes]
        wn = sorted({db.name(w) or f"sub_{w:X}" for w in writers})[:6]
        out.append("  written by: " + ", ".join(wn))
    out.append("uses (distinct lines, G = this global, v/a = locals/args):")
    out += [f"  {n:4}x  {p[:160]}" for p, n in pats]
    for name, n in consumers:
        sp = short_pseudo(db, name)
        if sp:
            out.append(f"consumer {name} ({n} uses):\n```c\n{sp}\n```")
    return "\n".join(out)


def clusters(db, use, todo):
    """Related globals together: adjacent (< 0x10 apart) or sharing most writers/accessors."""
    parent = {a: a for a in todo}

    def find(a):
        while parent[a] != a:
            parent[a] = parent[parent[a]]
            a = parent[a]
        return a
    s = sorted(todo)
    for x, y in zip(s, s[1:]):
        if y - x < 0x10:
            parent[find(y)] = find(x)
    for i, x in enumerate(s):
        for y in s[i + 1:i + 40]:
            a, b = use[x], use[y]
            if len(a & b) >= 0.6 * min(len(a), len(b)):
                parent[find(y)] = find(x)
    groups = collections.defaultdict(list)
    for a in todo:
        groups[find(a)].append(a)
    return sorted(groups.values(), key=lambda g: -max(len(use[a]) for a in g))


# ---------------------------------------------------------------- model

SYSTEM = """You are a reverse engineer naming the global variables of a 2006 MSVC game (Commandos: Strike Force,
PC; Pyro Studios, Spanish identifiers). For each global you get mechanical facts and its distinct usage patterns
across all functions (count x line; G = the global, v/a = locals/arguments; IDA's library type guesses were
removed and show as T).

Existing function names come from earlier automated passes and can be wrong: *_GetField0XXX / C_<hex>_* are
placeholders, and a tiny getter's class prefix may be from another class (one body is shared by every class with
the same layout). Trust the code, not the prefix.

Name each global for what it IS in the game (its role), from the evidence:
- g_ prefix, then PascalCase: g_pGameWorld, g_bPaused, g_CurrentMissionIndex, g_MenuStack. Pointer to an object:
  g_p...; bool flags: g_b...; counts: g_num.../...Count; arrays/tables: plural or ...Table.
- Never encode the address, never generic words (Value, Data, Global, Object, Dword, Unk, Var).
- Follow the pattern of already-named neighbours/families (shown in the packet) so a family reads uniformly.
- Related globals in one request must have consistent names (a pointer + its count, members of one struct).
- type: one of std::int8_t std::uint8_t std::int16_t std::uint16_t std::int32_t std::uint32_t std::int64_t
  std::uint64_t int unsigned int bool char float double void* const char* char* const wchar_t* const char**, or a pointer to a class listed under "Known
  classes" (ClassName*). Use void* for an object pointer whose class isn't listed.
- confidence: 0.9+ when the usage makes the role clear, 0.6-0.8 for a reasonable guess, below 0.5 when unclear
  (then still give your best name).
- why: one short sentence of evidence.
- functions: optionally, functions you can name with confidence >= 0.8 from what you read (callers/consumers in
  the packet, e.g. sub_44FC30 -> GameWorld_GetPlayer). Use Class_Verb / Subsystem_Verb style. Skip if unsure.
Answer only with the JSON object."""

SCHEMA = {
    "type": "object",
    "properties": {
        "globals": {"type": "array", "items": {"type": "object", "properties": {
            "addr": {"type": "string"}, "name": {"type": "string"}, "type": {"type": "string"},
            "confidence": {"type": "number"}, "why": {"type": "string"}},
            "required": ["addr", "name", "type", "confidence", "why"]}},
        "functions": {"type": "array", "items": {"type": "object", "properties": {
            "ref": {"type": "string"}, "name": {"type": "string"}, "confidence": {"type": "number"}},
            "required": ["ref", "name", "confidence"]}},
    },
    "required": ["globals"],
}


def chat(model, effort, system, prompt, schema=None):
    """One headless Claude Code call: our system prompt, no tools/settings/MCP/skills (~560 fixed tokens, cached
    across calls), structured output against SCHEMA."""
    os.makedirs(CLAUDE_CWD, exist_ok=True)
    cmd = ["claude", "-p", "--tools", "", "--model", model, "--effort", effort, "--no-session-persistence",
           "--strict-mcp-config", "--setting-sources", "", "--disable-slash-commands", "--output-format", "json",
           "--system-prompt", system, "--json-schema", json.dumps(schema or SCHEMA)]
    t0 = time.time()
    r = subprocess.run(cmd, input=prompt, capture_output=True, text=True, cwd=CLAUDE_CWD, timeout=900)
    try:
        d = json.loads(r.stdout)
    except ValueError:
        raise RuntimeError(f"claude -p rc={r.returncode}: {(r.stderr or r.stdout)[-300:]}")
    if d.get("is_error"):
        raise RuntimeError(f"claude -p error: {str(d.get('result'))[:300]}")
    out = d.get("structured_output")
    if out is None:
        m = re.search(r"\{.*\}", d.get("result") or "", re.S)
        out = json.loads(m.group(0)) if m else {}
    u = d.get("usage", {})
    usage = dict(input=u.get("input_tokens", 0), cache_write=u.get("cache_creation_input_tokens", 0),
                 cache_read=u.get("cache_read_input_tokens", 0), output=u.get("output_tokens", 0),
                 cost_usd=d.get("total_cost_usd", 0), secs=round(time.time() - t0, 1), model=model, effort=effort)
    return out, usage


# ---------------------------------------------------------------- checks / apply

def known_classes(db):
    names = {}
    for vt, c in db.class_names.items():
        names[c["name"]] = vt
    return names


def check(db, prop, batch_addrs, taken, classes):
    """(ok, reason) for one proposed global."""
    try:
        addr = int(str(prop["addr"]), 16)
    except ValueError:
        return False, "bad addr"
    if addr not in batch_addrs:
        return False, "not in this batch"
    n, t = prop["name"], prop["type"].replace(" *", "*").strip()
    if not re.fullmatch(r"g_[A-Za-z][A-Za-z0-9_]{2,60}", n):
        return False, f"name {n!r} breaks the convention"
    if gl.generic(n, addr):
        return False, f"generic name {n!r}"
    if n in taken and taken[n] != addr:
        return False, f"name {n!r} already used by 0x{taken[n]:X}"
    if t not in PRIMS and not (t.endswith("*") and t[:-1] in classes):
        return False, f"type {t!r} wouldn't compile"
    return True, ""


class Locked:
    """Exclusive lock on work/db.lock: tools/globals.py (grunt's commit) takes it too, so the read-modify-write of
    db/globals.yaml never interleaves."""
    def __enter__(self):
        self.f = open(os.path.join(ROOT, "work", "db.lock"), "w")
        fcntl.flock(self.f, fcntl.LOCK_EX)
        return self

    def __exit__(self, *a):
        fcntl.flock(self.f, fcntl.LOCK_UN)
        self.f.close()


def apply_globals(entries):
    """Pin accepted names in db/globals.yaml (src: captain); commit() once at the end of the run."""
    import yaml
    with Locked():
        cur = yaml.safe_load(open(gl.PATH)) or []
        by = {int(g["addr"]): g for g in cur}
        for e in entries:
            old = by.get(e["addr"], {})
            alt = sorted(set(old.get("alt", [])) | ({old["name"]} if old.get("name") and old["name"] != e["name"] else set()))
            by[e["addr"]] = dict(addr=e["addr"], name=e["name"], type=e["type"], uses=old.get("uses", 0),
                                 src="captain", conf=round(e["confidence"], 2), **({"alt": alt} if alt else {}))
        gl.write(sorted(by.values(), key=lambda g: g["addr"]))


def commit(n, job):
    """One commit per run (db/globals.yaml only); grunt may be committing: retry on the index lock."""
    with Locked():
        for i in range(10):  # grunt may be committing: retry on the index lock
            r = subprocess.run(["git", "-C", ROOT, "commit", "-q", "-m",
                                f"captain: {n} {job} named\n\nCo-Authored-By: Claude Opus 5.5 <noreply@anthropic.com>",
                                "--", "db/globals.yaml"], capture_output=True, text=True)
            if r.returncode == 0 or "nothing" in r.stdout + r.stderr:
                return True
            time.sleep(3)
        print("commit failed:", r.stderr[-200:], flush=True)
        return False


# ---------------------------------------------------------------- live state

_live = dict(pid=os.getpid(), started=time.time(), job="", phase="", model="", effort="", counts={}, tokens={},
             current=None, recent=[])


def live(**kw):
    _live.update(kw)
    _live["updated"] = time.time()
    tmp = f"{STATE}.{os.getpid()}"
    with open(tmp, "w") as f:
        json.dump(_live, f)
    os.replace(tmp, STATE)


def bump(**kw):
    c = _live["counts"]
    for k, v in kw.items():
        c[k] = c.get(k, 0) + v


# ---------------------------------------------------------------- job: strings (mechanical, no model)

def string_name(st):
    """Name for a pointer to `st`: paths by directory + file stem (Menus/P_Help.fbs -> g_pszMenu_P_Help), other
    identifier-like text sanitized (.VIDEO -> g_psz_VIDEO). None for prose."""
    m = re.fullmatch(r"(?:([A-Za-z]\w*)[/\\])+?([\w.-]+?)(?:\.\w{1,4})?", st)
    if m and ("/" in st or "\\" in st):
        d = re.split(r"[/\\]", st)[-2]
        return "g_psz" + re.sub(r"s$", "", d.capitalize()) + "_" + re.sub(r"\W", "_", m.group(2))
    t = re.sub(r"\W+", "_", st).strip("_")
    if 2 <= len(t) <= 40 and " " not in st.strip():
        return ("g_pszKey_" if st.startswith(".") else "g_psz_") + t  # .KEY: config/asset file keys
    return None


def job_strings(args):
    """Never-written globals whose initial value points to a string: named after the string, const char*."""
    db = DB()
    use = data_globals(db)
    taken = {g["name"]: a for a, g in db.globals.items()}
    acc = []
    for a in sorted(use):
        g = db.globals.get(a)
        if g and g.get("src") in ("opus", "user"):
            continue
        st = read_cstr(read_u32(a)) if 0x401000 <= read_u32(a) < 0x900000 else None
        n = string_name(st) if st else None
        if not n:
            continue
        _, writes, _, _, _ = usage(db, a, use[a])
        if writes:
            continue  # a variable that happens to start at a string
        if n in taken and taken[n] != a:  # the same key in another module's table: qualify by module
            mod = module_name(db.rows[min(use[a])]["module"])
            n = f"{n}_{mod}" if not re.fullmatch(r"m[0-9A-F]+", mod) else f"{n}_{a:X}"
        base, k = n, 2
        while n in taken and taken[n] != a:
            n, k = f"{base}_{k}", k + 1
        taken[n] = a
        acc.append(dict(addr=a, name=n, type="const char*", confidence=1.0, why=f'points to "{st}"'))
    print(f"{len(acc)} string-pointer globals")
    for e in acc[:args.show]:
        print(f"  0x{e['addr']:X} {e['name']:40} {e['why']}")
    if acc and not args.dry:
        apply_globals(acc)
        commit(len(acc), "string-pointer globals")


# ---------------------------------------------------------------- job: modules

PLACEHOLDER = re.compile(r"^(C_[0-9a-fA-F]+_|sub_|nullsub|j_)|Field[0-9A-F]{3,4}|_[0-9A-F]{6}$|^(Get|Set|Return|Is)"
                         r"(Const|Constant|Value|Field|Global|Flag)?\w{0,6}$")

SYSTEM_MODULES = """You are a reverse engineer naming the source modules of a 2006 MSVC game (Commandos: Strike Force,
PC; Pyro Studios, Spanish identifiers/strings). A module is an address-contiguous range that approximates one
original .cpp/.obj file. Name each module from its evidence: classes, strings (often Spanish config keys or asset
paths), named functions, library calls, which named modules call it / it calls, and its address neighbours (the
link order keeps related files together, so neighbours' names are strong hints).

- name: snake_case, 3-32 chars, subsystem first, like the existing names below (hud_objectives, ai_move_tasks,
  menu_mp_sessions, gfx_matrix, net_session). Never encode the address; never generic (misc, utils, module, code).
  Must differ from every existing name.
- description: one short line of the evidence that decided it.
- confidence: 0.9+ clear, 0.6-0.8 reasonable guess, <0.5 unclear (still give your best name).
Function names from earlier automated passes can be wrong placeholders; trust strings, classes and library calls more.

Existing module names (style reference, all taken):
{names}

Answer only with the JSON object."""

SCHEMA_MODULES = {
    "type": "object",
    "properties": {"modules": {"type": "array", "items": {"type": "object", "properties": {
        "id": {"type": "string"}, "name": {"type": "string"}, "description": {"type": "string"},
        "confidence": {"type": "number"}}, "required": ["id", "name", "description", "confidence"]}}},
    "required": ["modules"],
}


def module_packet(db, m, mods, by_ea, S):
    eas = [e for e in db.F if m["start"] <= e < m["end"] and db.rows.get(e, {}).get("region") == "game"]
    cls = collections.Counter(db.class_of(e) for e in eas if db.class_of(e))
    strs = collections.Counter()
    gnames = collections.Counter()
    libs = collections.Counter()
    callers, callees = collections.Counter(), collections.Counter()
    for e in eas:
        f = db.F[e]
        for a in f.get("srefs", []):
            if a in S:
                strs[S[a][:60]] += 1
        for d in f["drefs"]:
            g = db.globals.get(d)
            if g and g.get("src") in ("captain", "opus", "user") and not g["name"].startswith("g_pszKey_"):
                gnames[re.sub(r"_[YZGBA]$", "", g["name"])] += 1  # vector/colour components -> the vector
            elif g and g["name"].startswith("g_pszKey_"):
                strs[read_cstr(read_u32(d)) or g["name"]] += 1
        for c in f["callees"]:
            r = db.rows.get(c)
            if not r:
                continue
            if r["region"] != "game" and r["name"]:
                libs[r["name"]] += 1
            else:
                mm = by_ea(c)
                if mm and mm is not m:
                    callees[mm["name"] or mm["id"]] += 1
        for caller in f.get("callers", []):
            mm = by_ea(caller)
            if mm and mm is not m:
                callers[mm["name"] or mm["id"]] += 1
    named = [db.rows[e]["name"] for e in sorted(eas) if db.rows[e]["name"] and not PLACEHOLDER.search(db.rows[e]["name"])]
    i = mods.index(m)
    nb = [x for x in mods[max(0, i - 3):i + 4] if x is not m]
    out = [f"### {m['id']}  0x{m['start']:X}-0x{m['end']:X}, {m['fns']} functions, {m['kb']} KB"]
    out.append("neighbours by address: " + ", ".join(
        f"{'<' if x['start'] < m['start'] else '>'}{x['name'] or x['id']}" for x in nb))
    if cls:
        out.append("classes: " + ", ".join(f"{db.cls_name(vt)}({n})" for vt, n in cls.most_common(8)))
    if strs:
        out.append("strings: " + " | ".join(s_ for s_, _ in strs.most_common(24)))
    if gnames:
        out.append("named globals used: " + ", ".join(g for g, _ in gnames.most_common(10)))
    if named:
        out.append("named functions: " + ", ".join(named[:16]))
    if libs:
        out.append("library calls: " + ", ".join(n for n, _ in libs.most_common(10)))
    if callers:
        out.append("called from: " + ", ".join(f"{n}({k})" for n, k in callers.most_common(6)))
    if callees:
        out.append("calls into: " + ", ".join(f"{n}({k})" for n, k in callees.most_common(6)))
    return "\n".join(out)


def job_modules(args):
    import yaml
    sys.path.insert(0, os.path.dirname(os.path.abspath(__file__)))
    import modules as mo
    db = DB()
    S = {int(a, 16): v for a, v in json.load(open(os.path.join(ROOT, "export", "strings.json"))).items()}
    mods = mo.read_yaml()
    starts = sorted((x["start"], i) for i, x in enumerate(mods))
    import bisect

    def by_ea(ea):
        k = bisect.bisect_right(starts, (ea, 1 << 30)) - 1
        if k < 0:
            return None
        x = mods[starts[k][1]]
        return x if x["start"] <= ea < x["end"] else None
    for e, f in db.F.items():  # reverse call graph (the export has callees only)
        for c in f["callees"]:
            if c in db.F:
                db.F[c].setdefault("callers", []).append(e)
    held_path = os.path.join(WORK, "held_modules.jsonl")
    if args.retry_held and os.path.exists(held_path):
        args.only = (args.only or set()) | {json.loads(l).get("id") for l in open(held_path)}
    todo = [x for x in mods if not x.get("name") and (not args.only or x["id"] in args.only)]

    def evidence(x):  # rich modules first: their names then show up in the harder ones' packets
        pk = module_packet(db, x, mods, by_ea, S)
        return (len(re.findall(r" \| ", pk)) + 3 * len(re.findall(r"\b(?!C_[0-9a-f])[A-Z]\w+\(\d+\)", pk.split("called from")[0]))
                + len(re.findall(r"named functions: (.*)", pk)[0].split(", ") if "named functions" in pk else []))
    todo.sort(key=evidence, reverse=True)
    todo = todo[:args.limit]
    batches = [todo[i:i + args.batch] for i in range(0, len(todo), args.batch)]
    taken = {x["name"] for x in mods if x.get("name")}
    system = SYSTEM_MODULES.format(names=", ".join(sorted(taken)))
    live(job="modules", phase="running", model=args.model, effort=args.effort,
         counts=dict(total=len(todo), batches=len(batches), done=0, applied=0, held=0, rejected=0, fn_proposals=0))
    print(f"{len(todo)} modules in {len(batches)} calls", flush=True)
    applied_total = 0
    for bi, batch in enumerate(batches):
        prompt = f"Name these {len(batch)} modules.\n\n" + "\n\n".join(module_packet(db, x, mods, by_ea, S) for x in batch)
        if args.dry:
            print(system[:400] + "...\n\n" + prompt)
            return
        live(current=dict(batch=bi + 1, addrs=[x["id"] for x in batch], since=time.time(), chars=len(prompt)))
        try:
            out, u = chat(args.model, args.effort, system, prompt, SCHEMA_MODULES)
        except Exception as e:
            print(f"batch {bi + 1}: {e}", flush=True)
            bump(errors=1)
            continue
        account(u)
        ids = {x["id"]: x for x in batch}
        acc, held, rej = [], [], []
        for p in out.get("modules", []):
            n = p.get("name", "")
            if p.get("id") not in ids:
                rej.append(dict(p, reason="not in this batch"))
            elif not re.fullmatch(r"[a-z][a-z0-9_]{2,31}", n) or re.search(r"(^|_)(misc|utils?|module|code|m[0-9a-f]{6})($|_)", n):
                rej.append(dict(p, reason=f"name {n!r} breaks the convention"))
            elif n in taken:
                rej.append(dict(p, reason=f"name {n!r} already taken"))
            else:
                (acc if p["confidence"] >= args.min_conf else held).append(p)
                taken.add(n)
        if acc and not args.no_apply:
            with Locked():
                cur = mo.read_yaml()
                for p in acc:
                    for x in cur:
                        if x["id"] == p["id"]:
                            x.update(name=p["name"], name_src="captain", note=p["description"][:120])
                for x in cur:
                    x["classes"] = [(c, 0) for c in x["classes"]]
                mo.write_yaml(cur)
            for p in acc:
                ids[p["id"]]["name"] = p["name"]  # later batches see it as a neighbour/caller name
            applied_total += len(acc)
        with open(os.path.join(WORK, "held_modules.jsonl"), "a") as f:
            for e in held + rej:
                f.write(json.dumps(dict(e, ts=time.time(), model=args.model)) + "\n")
        rec = dict(ts=time.time(), job="modules", batch=list(ids), usage=u,
                   accepted=[(p["id"], p["name"], "module", p["confidence"]) for p in acc],
                   held=[(p["id"], p["name"], p["confidence"]) for p in held],
                   rejected=[(p.get("id"), p.get("name"), p["reason"]) for p in rej],
                   why={p["id"]: p["description"] for p in acc + held})
        with open(LOG, "a") as f:
            f.write(json.dumps(rec) + "\n")
        bump(done=len(batch), applied=len(acc), held=len(held), rejected=len(rej))
        _live["recent"] = (rec["accepted"] + [(a, n, "", c) for a, n, c in rec["held"]] + _live["recent"])[:12]
        live(current=None)
        print(f"batch {bi + 1}/{len(batches)}: {len(acc)} applied, {len(held)} held, {len(rej)} rejected, "
              f"{u['secs']}s, in {u['input'] + u['cache_write'] + u['cache_read']} out {u['output']}", flush=True)
    if applied_total and not args.no_apply:
        with Locked():
            for i in range(10):
                r = subprocess.run(["git", "-C", ROOT, "commit", "-q", "-m",
                                    f"captain: {applied_total} modules named\n\nCo-Authored-By: Claude Opus 5.5 <noreply@anthropic.com>",
                                    "--", "db/modules.yaml"], capture_output=True, text=True)
                if r.returncode == 0:
                    break
                time.sleep(3)
    live(phase="finished", current=None)


def account(u):
    t = _live["tokens"]
    for k in ("input", "cache_write", "cache_read", "output"):
        t[k] = t.get(k, 0) + u[k]
    t["cost_usd"] = round(t.get("cost_usd", 0) + (u["cost_usd"] or 0), 4)
    t["calls"] = t.get("calls", 0) + 1


# ---------------------------------------------------------------- job: globals

def job_globals(args):
    db = DB()
    use = data_globals(db)
    pinned = {a for a, g in db.globals.items() if g.get("src") in ("opus", "user", "captain")}
    held_path = os.path.join(WORK, "held.jsonl")
    if args.retry_held:  # held/rejected globals of earlier runs, still unpinned
        hs = {int(str(json.loads(l)["addr"]), 16) if isinstance(json.loads(l)["addr"], str) else json.loads(l)["addr"]
              for l in open(held_path)} if os.path.exists(held_path) else set()
        args.only = (args.only or set()) | hs
        args.min_modules = 0
    cand = [a for a, acc in use.items() if len({db.rows[e]["module"] for e in acc}) >= args.min_modules
            and a not in pinned and (not args.only or a in args.only)]
    cand.sort(key=lambda a: -len(use[a]))
    cand = cand[:args.limit]
    allref = sorted(use)
    nxt = {a: next((b for b in allref if b > a), None) for a in cand}
    groups = clusters(db, use, cand)
    batches, cur = [], []
    split = []
    for g in groups:  # an over-merged cluster (globals sharing a few big accessors) is cut by address
        g = sorted(g)
        split += [g[i:i + args.batch] for i in range(0, len(g), args.batch)]
    for g in split:  # pack whole clusters, up to --batch globals per call
        if cur and len(cur) + len(g) > args.batch:
            batches.append(cur)
            cur = []
        cur = cur + g
    if cur:
        batches.append(cur)
    classes = known_classes(db)
    taken = {g["name"]: a for a, g in db.globals.items() if g.get("src") in ("opus", "user", "captain")}
    os.makedirs(WORK, exist_ok=True)
    live(job="globals", phase="running", model=args.model, effort=args.effort,
         counts=dict(total=len(cand), batches=len(batches), done=0, applied=0, held=0, rejected=0, fn_proposals=0))
    print(f"{len(cand)} globals in {len(batches)} calls", flush=True)
    for bi, batch in enumerate(batches):
        packets = [global_packet(db, a, use[a], nxt.get(a), use) for a in sorted(batch)]
        rel = sorted({db.cls_name(vt) for a in batch for e in use[a] for vt in [db.class_of(e)] if vt})
        rel = [c for c in rel if c in classes][:40]
        prompt = (f"Name these {len(batch)} globals.\n\nKnown classes (usable as ClassName* types): "
                  + (", ".join(rel) or "none") + "\n\n" + "\n\n".join(packets))
        if args.dry:
            print(prompt)
            return
        live(current=dict(batch=bi + 1, addrs=[f"0x{a:X}" for a in sorted(batch)], since=time.time(),
                          chars=len(prompt)))
        try:
            out, u = chat(args.model, args.effort, SYSTEM, prompt)
        except Exception as e:
            print(f"batch {bi + 1}: {e}", flush=True)
            bump(errors=1)
            continue
        t = _live["tokens"]
        for k in ("input", "cache_write", "cache_read", "output"):
            t[k] = t.get(k, 0) + u[k]
        t["cost_usd"] = round(t.get("cost_usd", 0) + (u["cost_usd"] or 0), 4)
        t["calls"] = t.get("calls", 0) + 1
        acc, held, rej = [], [], []
        for p in out.get("globals", []):
            ok, why = check(db, p, set(batch), taken, classes)
            if not ok:
                rej.append(dict(p, reason=why))
                continue
            e = dict(addr=int(p["addr"], 16), name=p["name"], type=p["type"].replace(" *", "*").strip(),
                     confidence=float(p["confidence"]), why=p.get("why", ""))
            (acc if e["confidence"] >= args.min_conf else held).append(e)
            taken[e["name"]] = e["addr"]
        fns = [f for f in out.get("functions", []) if f.get("confidence", 0) >= 0.8]
        with open(os.path.join(WORK, "held.jsonl"), "a") as f:
            for e in held + rej:
                f.write(json.dumps(dict(e, ts=time.time(), model=args.model)) + "\n")
        with open(os.path.join(WORK, "fn_proposals.jsonl"), "a") as f:
            for x in fns:
                f.write(json.dumps(dict(x, ts=time.time(), model=args.model)) + "\n")
        if acc and not args.no_apply:
            apply_globals(acc)
            for e in acc:  # later calls of this run see these names (neighbours, substituted pseudocode)
                db.globals[e["addr"]] = dict(addr=e["addr"], name=e["name"], type=e["type"], src="captain")
        rec = dict(ts=time.time(), job="globals", batch=[f"0x{a:X}" for a in sorted(batch)], usage=u,
                   accepted=[(f"0x{e['addr']:X}", e["name"], e["type"], e["confidence"]) for e in acc],
                   held=[(f"0x{e['addr']:X}", e["name"], e["confidence"]) for e in held],
                   why={f"0x{e['addr']:X}": e["why"] for e in acc + held},
                   rejected=[(p.get("addr"), p.get("name"), p["reason"]) for p in rej], functions=len(fns))
        with open(LOG, "a") as f:
            f.write(json.dumps(rec) + "\n")
        bump(done=len(batch), applied=len(acc), held=len(held), rejected=len(rej), fn_proposals=len(fns))
        _live["recent"] = (rec["accepted"] + [(a, n, "", c) for a, n, c in rec["held"]])[-12:] + _live["recent"]
        _live["recent"] = _live["recent"][:12]
        live(current=None)
        print(f"batch {bi + 1}/{len(batches)}: {len(acc)} applied, {len(held)} held, {len(rej)} rejected, "
              f"{u['secs']}s, in {u['input'] + u['cache_write'] + u['cache_read']} out {u['output']}", flush=True)
    if _live["counts"].get("applied") and not args.no_apply:
        commit(_live["counts"]["applied"], "globals")
    live(phase="finished", current=None)


def main():
    ap = argparse.ArgumentParser()
    sub = ap.add_subparsers(dest="cmd", required=True)
    g = sub.add_parser("globals")
    g.add_argument("--limit", type=int, default=89)
    g.add_argument("--min-modules", type=int, default=3)
    g.add_argument("--batch", type=int, default=6, help="globals per call (whole clusters)")
    g.add_argument("--model", default="opus", help="A/B 2026-10-08: opus named globals clearly better than sonnet")
    g.add_argument("--effort", default="medium")
    g.add_argument("--min-conf", type=float, default=0.7, help="apply at/above, hold below")
    g.add_argument("--only", type=lambda s: {int(x, 16) for x in s.split(",")})
    g.add_argument("--dry", action="store_true", help="print the first prompt, no call")
    g.add_argument("--no-apply", action="store_true", help="log/hold only (A/B)")
    g.add_argument("--retry-held", action="store_true", help="re-ask the held/rejected globals of earlier runs")
    mm = sub.add_parser("modules", help="name unnamed modules (db/modules.yaml)")
    mm.add_argument("--limit", type=int, default=1000)
    mm.add_argument("--batch", type=int, default=8)
    mm.add_argument("--model", default="opus")
    mm.add_argument("--effort", default="medium")
    mm.add_argument("--min-conf", type=float, default=0.7)
    mm.add_argument("--only", type=lambda s: set(s.split(",")))
    mm.add_argument("--dry", action="store_true")
    mm.add_argument("--no-apply", action="store_true")
    mm.add_argument("--retry-held", action="store_true", help="re-ask held/rejected modules of earlier runs")
    st = sub.add_parser("strings", help="mechanical: pointer-to-string globals named after the string")
    st.add_argument("--dry", action="store_true")
    st.add_argument("--show", type=int, default=30)
    p = sub.add_parser("packet")
    p.add_argument("addr", type=lambda s: int(s, 16))
    a = ap.parse_args()
    if a.cmd == "packet":
        db = DB()
        use = data_globals(db)
        refs = sorted(use)
        print(global_packet(db, a.addr, use[a.addr], next((b for b in refs if b > a.addr), None), use))
        return
    lock = open(LOCK, "w")
    try:
        fcntl.flock(lock, fcntl.LOCK_EX | fcntl.LOCK_NB)
    except BlockingIOError:
        sys.exit("captain is already running (work/captain.lock)")
    {"globals": job_globals, "strings": job_strings, "modules": job_modules}[a.cmd](a)


if __name__ == "__main__":
    main()
