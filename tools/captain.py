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
            if re.search(r"\bG\s*=[^=]", n) or re.search(r"\bG\s*(\+\+|--|[-+|&^]=)|(\+\+|--)G\b", n):
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


# ---------------------------------------------------------------- job: classes

SYSTEM_CLASSES = """You are a reverse engineer naming the C++ classes of a 2006 MSVC game (Commandos: Strike Force,
PC; Pyro Studios, Spanish identifiers). There is no RTTI. For each class you get a sheet: vtable, size, base chain
and derived classes, ctors/dtors and who creates the object (with strings near the creation site), library calls,
strings used by its methods, field accesses (+offset:size r/w) and field names grunts proposed, the names earlier
automated passes gave its methods, and its module.

The game's own conventions (attested in the binary): C* for classes (CVector3, CMenuScreen, CGuiControl,
CInterpolatorHermite3D), T* for AI tasks/states (TCombate, TFncRastrear: these return their own name from a
GetName virtual), S* for plain structs, Net* for NetZ network objects. Use English for C* names unless a Spanish
type-name string is attested.

- name: one of those prefixes + PascalCase. Must differ from every existing class name (listed). A base and its
  derived classes should read as a family (CMenuScreen -> CMenuMain).
- attested: true only when the exact name appears as a type-name string in the evidence.
- confidence: 0.9+ the role is clear, 0.7-0.85 good evidence, 0.5-0.65 plausible, <0.5 unclear (best guess).
- why: one short sentence of the deciding evidence.
Method names are hints (they can be wrong); creation sites, strings and library calls are stronger.

Existing class names (taken): {names}

Answer only with the JSON object."""

SCHEMA_CLASSES = {
    "type": "object",
    "properties": {"classes": {"type": "array", "items": {"type": "object", "properties": {
        "vtable": {"type": "string"}, "name": {"type": "string"}, "attested": {"type": "boolean"},
        "confidence": {"type": "number"}, "why": {"type": "string"}},
        "required": ["vtable", "name", "attested", "confidence", "why"]}}},
    "required": ["classes"],
}
CLASSES_YAML = os.path.join(ROOT, "db", "types", "classes.yaml")


def class_packet(db, ci, vt, mod_name):
    import contextlib, io
    buf = io.StringIO()
    with contextlib.redirect_stdout(buf):
        ci.sheet(vt)
    txt = buf.getvalue().strip()
    txt = re.sub(r"(field names \(grunts\):.{0,600}).*", r"\1", txt)
    txt = re.sub(r"(fields:(?: \S+){0,40}).*", r"\1", txt)
    # creation sites: the code around `new` + ctor call (factory switch cases, keys, strings name the class)
    sites = []
    for ct in db.classes[vt]["ctors"][:2]:
        cte = int(ct, 16)
        for caller in sorted(db.F.get(cte, {}).get("callers", []))[:3]:
            src = pseudo(caller)
            if not src:
                continue
            ls = src.splitlines()
            hits = [i for i, l in enumerate(ls) if f"sub_{cte:X}(" in l]
            for i in hits[:1]:
                win = [l.strip() for l in ls[max(0, i - 6):i + 3] if l.strip()]
                sites.append(f"{db.name(caller) or f'sub_{caller:X}'}:\n    " + "\n    ".join(
                    substitute_names(db, GENERIC_T.sub("T", l))[:150] for l in win))
        src = pseudo(cte)
        if src and len(src.splitlines()) <= 28:
            sites.append("ctor body:\n    " + "\n    ".join(substitute_names(db, GENERIC_T.sub("T", l.strip()))[:150]
                                                     for l in src.splitlines()[1:] if l.strip() and l.strip() not in "{}"))
    if sites:
        txt += "\ncreation sites:\n  " + "\n  ".join(sites[:4])
    own = sorted(int(m, 16) for m in db.classes[vt]["methods"])
    mn = [re.sub(r"^C_[0-9a-f]{8}_", "", db.rows[e]["name"]) for e in own if db.rows.get(e, {}).get("name")]
    if mn:
        txt += "\nmethod names (automated, hints): " + ", ".join(mn[:40])
    mods = collections.Counter(db.rows[e]["module"] for e in own if e in db.rows)
    if mods:
        txt += "\nmodule: " + ", ".join(f"{mod_name(m)}({n})" for m, n in mods.most_common(3))
    return txt


def job_classes(args):
    import classinfo as ci
    db = DB()
    for e, f in db.F.items():  # reverse call graph (creation sites)
        for c in f["callees"]:
            if c in db.F:
                db.F[c].setdefault("callers", []).append(e)
    allnames = {v["name"] for v in db.class_names.values()}
    import yaml
    raw = yaml.safe_load(open(CLASSES_YAML)) or {}
    have = {(f"{k:08x}" if isinstance(k, int) else str(k).lower()) for k in raw}  # any entry, even low conf
    allnames |= {v.get("name") for v in raw.values() if isinstance(v, dict)}
    named_methods = collections.Counter()
    for ea, r in db.rows.items():
        m = re.match(r"C_([0-9a-f]{8})_", r["name"] or "")
        if m:
            named_methods[m.group(1)] += 1
    held_path = os.path.join(WORK, "held_classes.jsonl")
    if args.retry_held and os.path.exists(held_path):
        args.only = (args.only or set()) | {json.loads(l).get("vtable", "").lower().replace("0x", "") for l in open(held_path)}
    todo = [vt for vt in db.classes if vt not in have and (not args.only or vt in args.only)
            and (named_methods[vt] or len(db.classes[vt]["methods"]) >= args.min_methods)]
    todo.sort(key=lambda vt: (-named_methods[vt], -len(db.classes[vt]["methods"])))
    todo = todo[:args.limit]
    # families together: order by root of the chain within the priority list, keep batches small
    root = {vt: (ci.chain(vt) or [vt])[-1] for vt in todo}
    fam = collections.defaultdict(list)
    for vt in todo:
        fam[root[vt]].append(vt)
    order = []
    for vt in todo:
        if vt not in order:
            order += [x for x in fam[root[vt]] if x not in order]
    batches = [order[i:i + args.batch] for i in range(0, len(order), args.batch)]
    system = SYSTEM_CLASSES.format(names=", ".join(sorted(n for n in allnames if n)))
    live(job="classes", phase="running", model=args.model, effort=args.effort,
         counts=dict(total=len(order), batches=len(batches), done=0, applied=0, held=0, rejected=0, fn_proposals=0))
    print(f"{len(order)} classes in {len(batches)} calls", flush=True)
    applied = []
    for bi, batch in enumerate(batches):
        prompt = f"Name these {len(batch)} classes.\n\n" + "\n\n".join(class_packet(db, ci, vt, module_name) for vt in batch)
        if args.dry:
            print(prompt)
            return
        live(current=dict(batch=bi + 1, addrs=batch, since=time.time(), chars=len(prompt)))
        try:
            out, u = chat(args.model, args.effort, system, prompt, SCHEMA_CLASSES)
        except Exception as e:
            print(f"batch {bi + 1}: {e}", flush=True)
            bump(errors=1)
            continue
        account(u)
        acc, held, rej = [], [], []
        for p in out.get("classes", []):
            vt = str(p.get("vtable", "")).lower().replace("0x", "").zfill(8)
            n = p.get("name", "")
            if vt not in batch:
                rej.append(dict(p, reason="not in this batch"))
            elif not re.fullmatch(r"(C|T|S|Net)[A-Z][A-Za-z0-9_]{1,40}", n) or re.fullmatch(r"C_?[0-9a-fA-F]{6,8}", n):
                rej.append(dict(p, reason=f"name {n!r} breaks the convention"))
            elif n in allnames:
                rej.append(dict(p, reason=f"name {n!r} already taken"))
            else:
                p["vtable"] = vt
                (acc if p["confidence"] >= args.min_conf else held).append(p)
                allnames.add(n)
        if acc and not args.no_apply:
            with Locked(), open(CLASSES_YAML, "a") as f:
                if not applied:
                    f.write(f"\n# ---- captain {time.strftime('%Y-%m-%d')} ({args.model}): names only; review before relying on them\n")
                for p in acc:
                    why = p["why"].replace('"', "'")[:200]
                    f.write(f'"{p["vtable"]}": {{name: {p["name"]}, attested: {str(bool(p["attested"])).lower()}, '
                            f'conf: {round(p["confidence"], 2)}, src: captain, why: "{why}"}}\n')
            applied += acc
        with open(held_path, "a") as f:
            for e in held + rej:
                f.write(json.dumps(dict(e, ts=time.time(), model=args.model)) + "\n")
        rec = dict(ts=time.time(), job="classes", batch=batch, usage=u,
                   accepted=[(p["vtable"], p["name"], "class", p["confidence"]) for p in acc],
                   held=[(p["vtable"], p["name"], p["confidence"]) for p in held],
                   rejected=[(p.get("vtable"), p.get("name"), p["reason"]) for p in rej],
                   why={p["vtable"]: p["why"] for p in acc + held})
        with open(LOG, "a") as f:
            f.write(json.dumps(rec) + "\n")
        bump(done=len(batch), applied=len(acc), held=len(held), rejected=len(rej))
        _live["recent"] = (rec["accepted"] + [(a, n, "", c) for a, n, c in rec["held"]] + _live["recent"])[:12]
        live(current=None)
        print(f"batch {bi + 1}/{len(batches)}: {len(acc)} applied, {len(held)} held, {len(rej)} rejected, "
              f"{u['secs']}s, in {u['input'] + u['cache_write'] + u['cache_read']} out {u['output']}", flush=True)
    if applied and not args.no_apply:
        with Locked():
            subprocess.run(["git", "-C", ROOT, "commit", "-q", "-m",
                            f"captain: {len(applied)} classes named (classes.yaml only; rename pass separate)\n\n"
                            "Co-Authored-By: Claude Opus 5.5 <noreply@anthropic.com>", "--", "db/types/classes.yaml"])
    live(phase="finished", current=None)


# ---------------------------------------------------------------- job: family (agentic class naming)

SYSTEM_FAMILY = """You are a reverse engineer naming the unnamed C++ classes of one area of a 2006 MSVC game
(Commandos: Strike Force, PC; Pyro Studios, Spanish identifiers). There is no RTTI. You work in the project
repository with read-only tools; investigate until each class's role is clear, then answer with the JSON object.

Where the evidence is (all addresses lowercase 8-hex in file names, e.g. 0053c5e0):
  export/funcs/<ea>.c      Hex-Rays pseudocode of a function (callees appear as sub_XXXXXX)
  db/functions.csv         ea,size,region,module,name,...: the current names of functions (C_<vtable>_ = method
                           of an unnamed class; CMenuX_ = method of a named class)
  src/auto/<ea>.cpp        our reimplementation of a function (better names, typed fields), when it exists
  db/types/classes.yaml    the named classes (keys = vtable) with the evidence for each
  export/strings.json      {address: string} of every string in the binary
  db/modules.yaml          source modules (id, address range, name, classes)
Commands you may run (Bash, nothing else):
  .venv/bin/python tools/classinfo.py <vtable>         class sheet: base chain, ctors/dtors and their callers,
                                                       field accesses, method slots
  .venv/bin/python tools/evidence.py pseudo <ea>       pseudocode with current names substituted
  .venv/bin/python tools/evidence.py callers <ea>      who calls a function
  .venv/bin/python tools/evidence.py class <vt|name>   struct + vtable slots with names
Strong evidence: who creates the object and what it is stored into (factory switch cases, .fbs file names,
control/keyword strings), strings and library calls in its methods, the named base/derived classes. Method
names in functions.csv were given by automated passes and can be wrong. Budget your reading: start from the
creation sites and the biggest methods; stop when the role is clear.

The game's conventions (attested): C* classes (CVector3, CMenuScreen, CGuiControl), T* AI tasks/states,
S* plain structs, Net* NetZ objects. English C* names unless a Spanish type-name string is attested. A derived
class should read as its family (CMenuScreen -> CMenuSaveGame). Names must differ from every existing class.

Per class: name, attested (true only if the exact name is a type-name string in the binary), confidence (0.9+
role clear, 0.7-0.85 good evidence, 0.5-0.65 plausible, <0.5 unclear), why (the deciding evidence, one or two
sentences with addresses), and fields: up to 12 {offset: name} you established (e.g. "0x0058": "m_selected"),
only fields whose meaning you saw (offset hex as in classinfo)."""

SCHEMA_FAMILY = {
    "type": "object",
    "properties": {"classes": {"type": "array", "items": {"type": "object", "properties": {
        "vtable": {"type": "string"}, "name": {"type": "string"}, "attested": {"type": "boolean"},
        "confidence": {"type": "number"}, "why": {"type": "string"},
        "fields": {"type": "object", "additionalProperties": {"type": "string"}}},
        "required": ["vtable", "name", "attested", "confidence", "why"]}}},
    "required": ["classes"],
}
FAMILY_TOOLS = ["Read", "Grep", "Glob", "Bash(.venv/bin/python tools/classinfo.py:*)",
                "Bash(.venv/bin/python tools/evidence.py:*)"]


def agent(model, effort, system, prompt, schema, budget):
    """One agentic headless session in the repo: read-only tools, everything else denied (dontAsk)."""
    cmd = ["claude", "-p", "--tools", "Read,Grep,Glob,Bash", "--allowedTools", *FAMILY_TOOLS,
           "--permission-mode", "dontAsk", "--model", model, "--effort", effort, "--no-session-persistence",
           "--strict-mcp-config", "--setting-sources", "", "--disable-slash-commands", "--output-format", "json",
           "--max-budget-usd", str(budget), "--system-prompt", system, "--json-schema", json.dumps(schema)]
    t0 = time.time()
    r = subprocess.run(cmd, input=prompt, capture_output=True, text=True, cwd=ROOT, timeout=3600)
    try:
        d = json.loads(r.stdout)
    except ValueError:
        raise RuntimeError(f"claude -p rc={r.returncode}: {(r.stderr or r.stdout)[-300:]}")
    out = d.get("structured_output")
    if out is None:
        m = re.search(r"\{.*\}", d.get("result") or "", re.S)
        out = json.loads(m.group(0)) if m else {}
    if d.get("is_error") and not out:
        raise RuntimeError(f"claude -p error: {str(d.get('result'))[:300]}")
    u = d.get("usage", {})
    usage = dict(input=u.get("input_tokens", 0), cache_write=u.get("cache_creation_input_tokens", 0),
                 cache_read=u.get("cache_read_input_tokens", 0), output=u.get("output_tokens", 0),
                 cost_usd=d.get("total_cost_usd", 0), secs=round(time.time() - t0, 1), model=model, effort=effort,
                 turns=d.get("num_turns", 0))
    return out, usage


def family_targets(db, modules):
    """Unnamed classes listed by the modules whose name matches one of the globs, grouped by module."""
    import fnmatch, yaml
    raw = yaml.safe_load(open(CLASSES_YAML)) or {}
    have = {(f"{k:08x}" if isinstance(k, int) else str(k).lower()) for k in raw}
    groups, seen = [], set()
    for m in yaml.safe_load(open(os.path.join(ROOT, "db", "modules.yaml"))):
        if not any(fnmatch.fnmatch(m.get("name") or m["id"], g) or m["id"] == g for g in modules):
            continue
        vts = [c[2:] for c in m.get("classes", []) if re.fullmatch(r"C_[0-9a-f]{8}", c)]
        vts = [v for v in vts if v in db.classes and v not in have and v not in seen]
        seen |= set(vts)
        if vts:
            groups.append((m, vts))
    return groups


def job_family(args):
    import classinfo as ci
    db = DB()
    for e, f in db.F.items():
        for c in f["callees"]:
            if c in db.F:
                db.F[c].setdefault("callers", []).append(e)
    import yaml
    raw = yaml.safe_load(open(CLASSES_YAML)) or {}
    allnames = {v["name"] for v in db.class_names.values()} | {v.get("name") for v in raw.values() if isinstance(v, dict)}
    groups = family_targets(db, args.modules)
    # sessions of up to --batch classes; neighbouring modules share one session (same family, shared reading)
    sessions, cur = [], []
    for m, vts in groups:
        for vt in vts:
            cur.append((m, vt))
            if len(cur) >= args.batch:
                sessions.append(cur)
                cur = []
    if cur:
        sessions.append(cur)
    n = sum(len(s) for s in sessions)
    live(job="family", phase="running", model=args.model, effort=args.effort,
         counts=dict(total=n, batches=len(sessions), done=0, applied=0, held=0, rejected=0))
    print(f"{n} classes in {len(sessions)} sessions", flush=True)
    held_path = os.path.join(WORK, "held_family.jsonl")
    applied = []
    for si, sess in enumerate(sessions):
        batch = [vt for _, vt in sess]
        named = sorted({db.class_names[c[2:]]["name"] for m, _ in sess for c in m.get("classes", [])
                        if c[2:] in db.class_names} | {c for m, _ in sess for c in m.get("classes", [])
                                                         if not c.startswith("C_")})
        prompt = (f"Name these {len(batch)} unnamed classes. Modules: "
                  + ", ".join(sorted({f"{m['id']} {m.get('name') or ''}".strip() for m, _ in sess}))
                  + (f". Named classes in the same modules: {', '.join(named)}" if named else "")
                  + "\n\nExisting class names (taken): " + ", ".join(sorted(x for x in allnames if x))
                  + "\n\nStarting sheets (dig further with the tools):\n\n"
                  + "\n\n".join(class_packet(db, ci, vt, module_name) for vt in batch))
        if args.dry:
            print(prompt)
            return
        live(current=dict(batch=si + 1, addrs=batch, since=time.time(), chars=len(prompt)))
        try:
            out, u = agent(args.model, args.effort, SYSTEM_FAMILY, prompt, SCHEMA_FAMILY, args.budget)
        except Exception as e:
            print(f"session {si + 1}: {e}", flush=True)
            bump(errors=1)
            continue
        account(u)
        acc, held, rej = [], [], []
        for p in out.get("classes", []):
            vt = str(p.get("vtable", "")).lower().replace("0x", "").zfill(8)
            nm = p.get("name", "")
            if vt not in batch:
                rej.append(dict(p, reason="not in this session"))
            elif not re.fullmatch(r"(C|T|S|Net)[A-Z][A-Za-z0-9_]{1,40}", nm) or re.fullmatch(r"C_?[0-9a-fA-F]{6,8}", nm):
                rej.append(dict(p, reason=f"name {nm!r} breaks the convention"))
            elif nm in allnames:
                rej.append(dict(p, reason=f"name {nm!r} already taken"))
            else:
                p["vtable"] = vt
                (acc if p["confidence"] >= args.min_conf else held).append(p)
                allnames.add(nm)
        if acc and not args.no_apply:
            with Locked(), open(CLASSES_YAML, "a") as f:
                if not applied:
                    f.write(f"\n# ---- captain family {time.strftime('%Y-%m-%d')} ({args.model}, agentic): "
                            + ", ".join(args.modules) + "\n")
                for p in acc:
                    why = p["why"].replace('"', "'").replace("\n", " ")[:300]
                    fl = {k: v for k, v in (p.get("fields") or {}).items()
                          if re.fullmatch(r"(0x)?[0-9a-fA-F]{1,4}", k) and re.fullmatch(r"[A-Za-z_]\w{1,40}", v)}
                    fs = (", fields: {" + ", ".join(f"0x{int(k, 16):04X}: {v}" for k, v in sorted(
                        fl.items(), key=lambda kv: int(kv[0], 16))) + "}") if fl else ""
                    f.write(f'"{vt_key(p["vtable"])}": {{name: {p["name"]}, attested: '
                            f'{str(bool(p["attested"])).lower()}, conf: {round(p["confidence"], 2)}, src: captain, '
                            f'why: "{why}"{fs}}}\n')
            applied += acc
        with open(held_path, "a") as f:
            for e in held + rej:
                f.write(json.dumps(dict(e, ts=time.time(), model=args.model)) + "\n")
        rec = dict(ts=time.time(), job="family", batch=batch, usage=u,
                   accepted=[(p["vtable"], p["name"], "class", p["confidence"]) for p in acc],
                   held=[(p["vtable"], p["name"], p["confidence"]) for p in held],
                   rejected=[(p.get("vtable"), p.get("name"), p["reason"]) for p in rej],
                   why={p["vtable"]: p["why"] for p in acc + held})
        with open(LOG, "a") as f:
            f.write(json.dumps(rec) + "\n")
        bump(done=len(batch), applied=len(acc), held=len(held), rejected=len(rej))
        _live["recent"] = (rec["accepted"] + [(a, nm, "", c) for a, nm, c in rec["held"]] + _live["recent"])[:12]
        live(current=None)
        print(f"session {si + 1}/{len(sessions)}: {len(acc)} applied, {len(held)} held, {len(rej)} rejected, "
              f"{u['turns']} turns, {u['secs']}s, ${u['cost_usd']:.2f}", flush=True)
        for p in acc + held:
            print(f"   {p['vtable']} {p['name']:28} {p['confidence']:.2f}  {p['why'][:110]}", flush=True)
    if applied and not args.no_apply:
        with Locked():
            subprocess.run(["git", "-C", ROOT, "commit", "-q", "-m",
                            f"captain family: {len(applied)} classes named ({', '.join(args.modules)}; classes.yaml "
                            "only, rename pass separate)\n\nCo-Authored-By: Claude Opus 5.5 <noreply@anthropic.com>",
                            "--", "db/types/classes.yaml"])
    live(phase="finished", current=None)


def vt_key(vt):
    return vt.lower().zfill(8)


# ---------------------------------------------------------------- job: functions (placeholder names)

FN_PLACEHOLDER = re.compile(
    r"^(?:C_[0-9a-f]{8}_|[A-Z]\w*?_)?(Return(MinusOne|Zero|One|True|False|\d+)|Get(Returned)?Const(ant)?\w*|"
    r"Is\w*Field\w*Null|\w*Field_?0?[0-9A-Fa-f]{2,4}\w*|\w*At0x\w*|\w*Offset[0-9A-F]+\w*|\w*_[0-9A-F]{6}|"
    r"\w*(Vtbl|Slot)\d+\w*|\w*Unk\w*|[Ss]ub_[0-9A-F]+|Nullsub\w*)$")

SYSTEM_FUNCTIONS = """You are a reverse engineer renaming functions of a 2006 MSVC game (Commandos: Strike Force, PC;
Pyro Studios, Spanish identifiers). Earlier automated passes gave these functions placeholder names that only
describe mechanics (GetField0050, ReturnMinusOne, InvokeVtbl34, IsField90Null). For each you get its pseudocode,
how its callers use it (distinct call lines with counts, F = this function), its class and module.

Name what it means in the game, from the evidence:
- PascalCase English verb phrase; keep the given prefix (ClassName_ or C_<vtable>_) when there is one, else
  prefix with the module's subsystem in PascalCase when it fits (module hud_objectives -> HudObjectives_...).
- A tiny function shared by unrelated classes (identical code folded by the linker) gets a neutral name that
  says what it does to its argument (e.g. GetSecondPointer, ReturnFalse is fine for a real constant stub).
- Never placeholders: no field offsets, slot numbers or addresses in the name unless nothing else is known.
- confidence: 0.9+ clear, 0.7-0.85 good, <0.6 unsure (still give your best name).
Answer only with the JSON object."""

SCHEMA_FUNCTIONS = {
    "type": "object",
    "properties": {"functions": {"type": "array", "items": {"type": "object", "properties": {
        "ea": {"type": "string"}, "name": {"type": "string"}, "confidence": {"type": "number"},
        "why": {"type": "string"}}, "required": ["ea", "name", "confidence", "why"]}}},
    "required": ["functions"],
}


def function_packet(db, ea):
    r, f = db.rows[ea], db.F[ea]
    src = pseudo(ea) or ""
    lines = [l for l in src.splitlines() if l.strip()]
    body = "\n".join(substitute_names(db, GENERIC_T.sub("T", l)) for l in lines[:40])
    ctx = []  # per caller: its name, the call line and the two after it (what the result is used for)
    tok = re.compile(rf"\b(?:sub_{ea:X}|{re.escape(r['name'])})\(")
    seen = set()
    for c in sorted(f.get("callers", []), key=lambda c: (not db.rows.get(c, {}).get("name"), c))[:12]:
        ls = (pseudo(c) or "").splitlines()
        for i, l in enumerate(ls):
            if tok.search(l):
                win = [norm_line(tok.sub("F(", x), re.compile(r"(?!x)x")) for x in ls[i:i + 3] if x.strip() not in ("{", "}", "")]
                key = tuple(win)
                if key not in seen:
                    seen.add(key)
                    ctx.append(f"  in {db.name(c) or f'sub_{c:X}'}:  " + "  /  ".join(substitute_names(db, w)[:110] for w in win))
                break
        if len(ctx) >= 6:
            break
    slots = [(vt_, i) for vt_, c_ in db.classes.items() for i, m in enumerate(c_.get("slots", []) or []) if int(m, 16) == ea]
    vt = db.class_of(ea)
    mod = db.modules.get(r["module"], {})
    out = [f"### 0x{ea:08X}  current name: {r['name']}  ({f['size']} bytes, {len(f.get('callers', []))} callers)"]
    if vt:
        out.append(f"class: {db.cls_name(vt)}")
    if mod.get("name"):
        out.append(f"module: {mod['name']}" + (f" ({mod.get('note', '')[:80]})" if mod.get("note") else ""))
    out.append("```c\n" + body + ("\n..." if len(lines) > 40 else "") + "\n```")
    if ctx:
        out.append("callers:\n" + "\n".join(ctx))
    elif not f.get("callers"):
        out.append("no direct callers: a virtual method (called through the vtable)")
    return "\n".join(out)


def job_functions(args):
    from triage import load_csv, save_csv
    db = DB()
    for e, f in db.F.items():
        for c in f["callees"]:
            if c in db.F:
                db.F[c].setdefault("callers", []).append(e)
    taken = {r["name"] for r in db.rows.values() if r["name"]}
    todo = [ea for ea, r in db.rows.items() if r["region"] == "game" and r["name"] and FN_PLACEHOLDER.match(r["name"])
            and (not args.only or ea in args.only)]
    by_mod = sorted(todo, key=lambda e: (db.class_of(e) or "", db.rows[e]["module"], e))  # class/module together
    by_mod = by_mod[:args.limit]
    batches = [by_mod[i:i + args.batch] for i in range(0, len(by_mod), args.batch)]
    live(job="functions", phase="running", model=args.model, effort=args.effort,
         counts=dict(total=len(by_mod), batches=len(batches), done=0, applied=0, held=0, rejected=0, fn_proposals=0))
    print(f"{len(by_mod)} placeholder-named functions in {len(batches)} calls", flush=True)
    renames = {}
    for bi, batch in enumerate(batches):
        prompt = f"Rename these {len(batch)} functions.\n\n" + "\n\n".join(function_packet(db, e) for e in batch)
        if args.dry:
            print(prompt)
            return
        live(current=dict(batch=bi + 1, addrs=[f"0x{e:X}" for e in batch], since=time.time(), chars=len(prompt)))
        try:
            out, u = chat(args.model, args.effort, SYSTEM_FUNCTIONS, prompt, SCHEMA_FUNCTIONS)
        except Exception as e:
            print(f"batch {bi + 1}: {e}", flush=True)
            bump(errors=1)
            continue
        account(u)
        acc, held, rej = [], [], []
        for p in out.get("functions", []):
            try:
                ea = int(str(p["ea"]), 16)
            except ValueError:
                ea = None
            n = p.get("name", "")
            if ea not in batch:
                rej.append(dict(p, reason="not in this batch"))
            elif not re.fullmatch(r"[A-Za-z][A-Za-z0-9_]{2,80}", n) or (FN_PLACEHOLDER.match(n) and p["confidence"] < 0.9):
                rej.append(dict(p, reason=f"name {n!r} is still a placeholder / bad identifier"))
            elif n in taken and n != db.rows[ea]["name"]:
                rej.append(dict(p, reason=f"name {n!r} already used"))
            else:
                p["ea_i"] = ea
                (acc if p["confidence"] >= args.min_conf else held).append(p)
                taken.add(n)
        if acc and not args.no_apply:
            for p in acc:
                renames[p["ea_i"]] = (db.rows[p["ea_i"]]["name"], p["name"])
        with open(os.path.join(WORK, "held_functions.jsonl"), "a") as f:
            for e in held + rej:
                f.write(json.dumps({k: v for k, v in dict(e, ts=time.time(), model=args.model).items() if k != "ea_i"}) + "\n")
        rec = dict(ts=time.time(), job="functions", batch=[f"0x{e:X}" for e in batch], usage=u,
                   accepted=[(f"0x{p['ea_i']:X}", p["name"], db.rows[p["ea_i"]]["name"], p["confidence"]) for p in acc],
                   held=[(f"0x{p['ea_i']:X}", p["name"], p["confidence"]) for p in held],
                   rejected=[(p.get("ea"), p.get("name"), p["reason"]) for p in rej],
                   why={f"0x{p['ea_i']:X}": p["why"] for p in acc + held})
        with open(LOG, "a") as f:
            f.write(json.dumps(rec) + "\n")
        bump(done=len(batch), applied=len(acc), held=len(held), rejected=len(rej))
        _live["recent"] = ([(a, n, "fn", c) for a, n, _, c in rec["accepted"]] + [(a, n, "", c) for a, n, c in rec["held"]]
                           + _live["recent"])[:12]
        live(current=None)
        print(f"batch {bi + 1}/{len(batches)}: {len(acc)} applied, {len(held)} held, {len(rej)} rejected, "
              f"{u['secs']}s, in {u['input'] + u['cache_write'] + u['cache_read']} out {u['output']}", flush=True)
    if renames:
        apply_function_renames(renames)
    live(phase="finished", current=None)


def apply_function_renames(renames):
    """functions.csv (name_src captain) + the identifier in the function's own src/auto file (its HOOK'd name);
    other files bind by address with their own local names. Rebuild; on a build failure the src edits are undone."""
    from triage import load_csv, save_csv
    with Locked():
        rows = load_csv()
        changed = []
        for ea, (old, new) in renames.items():
            r = rows.get(ea) if isinstance(next(iter(rows)), int) else rows.get(f"{ea:08x}")
            if r is None:
                continue
            r["name"], r["name_src"] = new, "captain"
            p = os.path.join(ROOT, "src", "auto", f"{ea:08x}.cpp")
            if os.path.exists(p):
                t = open(p).read()
                t2 = re.sub(rf"\b{re.escape(old)}\b", new, t)
                if t2 != t:
                    open(p, "w").write(t2)
                    changed.append(p)
        save_csv(rows)
    b = subprocess.run(["ninja", "-C", os.path.join(ROOT, "build")], capture_output=True, text=True)
    if b.returncode:
        print("build failed after renames: reverting src edits\n" + b.stdout[-800:], flush=True)
        subprocess.run(["git", "-C", ROOT, "checkout", "--"] + changed)
        changed = []
    files = ["db/functions.csv"] + [os.path.relpath(p, ROOT) for p in changed]
    with Locked():
        subprocess.run(["git", "-C", ROOT, "commit", "-q", "-m",
                        f"captain: {len(renames)} placeholder function names replaced\n\n"
                        "Co-Authored-By: Claude Opus 5.5 <noreply@anthropic.com>", "--"] + files)
    print(f"{len(renames)} renamed, {len(changed)} src files updated, build {'ok' if not b.returncode else 'FAILED'}",
          flush=True)


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
    cl = sub.add_parser("classes", help="name unnamed classes (db/types/classes.yaml)")
    cl.add_argument("--limit", type=int, default=1000)
    cl.add_argument("--batch", type=int, default=6)
    cl.add_argument("--min-methods", type=int, default=3, help="also classes without named methods if this many methods")
    cl.add_argument("--model", default="opus")
    cl.add_argument("--effort", default="medium")
    cl.add_argument("--min-conf", type=float, default=0.7)
    cl.add_argument("--only", type=lambda s: {x.lower().replace("0x", "").zfill(8) for x in s.split(",")})
    cl.add_argument("--dry", action="store_true")
    cl.add_argument("--no-apply", action="store_true")
    cl.add_argument("--retry-held", action="store_true")
    fn = sub.add_parser("functions", help="replace placeholder function names (db/functions.csv + own src file)")
    fn.add_argument("--limit", type=int, default=1000)
    fn.add_argument("--batch", type=int, default=10)
    fn.add_argument("--model", default="sonnet")
    fn.add_argument("--effort", default="medium")
    fn.add_argument("--min-conf", type=float, default=0.75)
    fn.add_argument("--only", type=lambda s: {int(x, 16) for x in s.split(",")})
    fn.add_argument("--dry", action="store_true")
    fn.add_argument("--no-apply", action="store_true")
    fa = sub.add_parser("family", help="agentic: one read-only Claude session per group of unnamed classes")
    fa.add_argument("modules", nargs="+", help="module names/ids or globs (menu_*)")
    fa.add_argument("--batch", type=int, default=6, help="classes per session")
    fa.add_argument("--model", default="opus")
    fa.add_argument("--effort", default="medium")
    fa.add_argument("--budget", type=float, default=3.0, help="USD cap per session")
    fa.add_argument("--min-conf", type=float, default=0.7)
    fa.add_argument("--dry", action="store_true")
    fa.add_argument("--no-apply", action="store_true")
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
    {"globals": job_globals, "strings": job_strings, "modules": job_modules,
     "classes": job_classes, "functions": job_functions,
     "family": job_family}[a.cmd](a)


if __name__ == "__main__":
    main()
