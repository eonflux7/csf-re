#!/usr/bin/env python3
"""Grunt driver: packet -> DeepSeek -> gate (1 retry) -> src/auto/<ea>.cpp, or escalate to queue/.

Usage:
  grunt.py run [--n 10] [--jobs 8] [--max-size 200] [--only ea,ea] [--model deepseek-flash] [--no-smoke]
  grunt.py lanes [--total 200] [--batch 50] [--tiny-jobs 8] [--main-jobs 6]   continuous, two lanes (see lanes())
  grunt.py cost                      token usage + projected cost from work/grunt_log.jsonl

After a batch: link, deploy, smoke test (unless --no-smoke); on success mark functions done and git commit.
"""
import hashlib, argparse, collections, concurrent.futures as cf, glob, json, os, re, subprocess, sys, threading, time, urllib.request
sys.path.insert(0, os.path.dirname(__file__))
from packet import DB, build
from gate import gate
from triage import load_csv, save_csv

ROOT = os.path.dirname(os.path.dirname(os.path.abspath(__file__)))
LOG = os.path.join(ROOT, "work", "grunt_log.jsonl")
PROPOSALS = os.path.join(ROOT, "db", "proposals.jsonl")

SYSTEM = r"""You reimplement single functions of a 2006 MSVC 7.1 x86 game (Commandos: Strike Force, Pyro Studios;
identifiers/strings are Spanish) as C++20, to be compiled with clang-cl for 32-bit MSVC ABI into a hook DLL that
replaces the original function inside the running game. Goal: exact functional equivalence, readable code.

Output exactly two fenced blocks: first ```cpp (the whole file), then ```json (metadata). Nothing else.

File rules:
- Allowed includes only: <cstdint> <cstddef> <cstring> <cmath> <climits> <cfloat> "globals.h" "hook.h"
  "core/memory.h" "types/auto_classes.h" "legacy/string.h". No inline asm. No malloc/free/new/delete.
  Allocate/free with exactly the function the original calls, bound by address (the game heap
  game::Mem_Alloc/Mem_Free = 0x6910F0/0x691110 only when the original calls those; NetZ objects use NetZ's
  allocator, STL containers 0x690DC0, etc.). Mixing heaps corrupts memory.
- The gate compares addresses: every function the original calls, every string/global/vtable it references must
  appear in your code (bound by address), you must not reference addresses it doesn't, and field offsets you
  access must be ones the original accesses.
- Every function and binding in the file is `static` (file-local). Exactly one HOOK line, for the target:
    HOOK(0xADDR, FunctionName, "auto");
- Conventions: a __thiscall method is written as a free function
    static RET __fastcall Name(C_xxx* self, void* /*edx*/, ARGS...)   (ecx=this, edx unused)
  __cdecl / __stdcall functions keep that convention. The callee-popped byte count (`ret N`) MUST equal the
  original's: __thiscall/__stdcall pop 4*stack args; __cdecl pops 0.
- Calling other original functions: bind by address, e.g.
    CSF_FUNC(int, __cdecl, Foo, 0x401230, (int a, char* b));
    CSF_FUNC(void, __fastcall, Bar_Method, 0x402000, (C_xxx* self, void* edx, int a));  // a __thiscall callee
    ... Bar_Method(obj, nullptr, 5);
  Use the callee's real arg count (from its prototype / ret N). Known names from the packet are fine to use.
- Virtual calls: ((RET(__fastcall*)(void*, void*, ARGS...))obj->vtbl[SLOT])(obj, nullptr, args...) where
  SLOT = byte offset / 4. If obj's type is unknown use CSF_AT(void**, p, 0)[SLOT].
- Globals: CSF_GLOBAL(type, g_Name, 0xADDR); then use g_Name.
- String constants of the original: ALWAYS CSF_STR(0xADDR, "text") with the address from the packet, never a
  bare string literal (callers may compare pointers). Same for other constant data: bind by address.
- Fields: use the declared struct fields (types/auto_classes.h; never redefine those structs). For an offset
  that is not a declared field (inside a pad_ array, or no class given) use CSF_AT(type, ptr, 0xOFF).
  Pointer-sized fields declared uint32_t: cast as needed, e.g. reinterpret_cast<C_xxx*>(self->field_0010).
- Preserve exact semantics: signed vs unsigned compares (check the disassembly when given: jl/jg = signed,
  jb/ja = unsigned), integer widths and truncation, float vs double, evaluation and call order, side effects.
  Do not "fix" bugs. Do not drop calls whose results are unused.
- `add ecx, N` / `lea reg, [ecx+N]` computes the ADDRESS of a sub-object embedded at +N (e.g. a member object
  or secondary base): use (uint8_t*)self + N. Only `mov reg, [ecx+N]` loads a pointer stored at +N.
- Memory reads: re-read a field/array element at every point the original reads it (check the disassembly).
  Never cache a value in a local across a call if the original reloads it after the call — callees may modify
  the object (e.g. a child removing itself from the list being iterated). Don't add checks or reads the
  original doesn't have.
- Give functions descriptive names in PascalCase (English) that say what it does or returns (e.g.
  GetCoverFireTaskName, IsInnerObjectType38, SetVisibleFlag) — never placeholders like GetConstant29, Sub_4206B0,
  ReturnValue, Nullsub. Prefix by the class id for methods, e.g.
  C_0082d70c_Destroy. Name local variables meaningfully.

JSON metadata: {"name": "<function name used>", "confidence": 0.0-1.0,
 "summary": "<one line: what it does>",
 "field_names": {"C_xxx.field_0010": "suggestedName", ...},   // only if reasonably sure
 "class_guess": {"C_xxx": "CSuggestedName"},                     // optional
 "notes": "<anything uncertain>"}"""


def load_key():
    for line in open(os.path.expanduser("~/.config/csf-re/env")):
        if line.startswith("DEEPSEEK_API_KEY="):
            return line.split("=", 1)[1].strip()
    raise SystemExit("no DEEPSEEK_API_KEY in ~/.config/csf-re/env")


def chat(model, messages, key, effort="high"):
    # Thinking on, explicit effort; output cap large enough that reasoning can't starve the answer.
    body = json.dumps(dict(model=model, messages=messages, max_tokens=64000, thinking={"type": "enabled"},
                           reasoning_effort=effort)).encode()
    req = urllib.request.Request("https://api.deepseek.com/chat/completions", data=body,
                                 headers={"Authorization": f"Bearer {key}", "Content-Type": "application/json"})
    for attempt in range(4):
        try:
            with urllib.request.urlopen(req, timeout=1200) as r:
                d = json.load(r)
            return d["choices"][0]["message"]["content"], d.get("usage", {})
        except Exception as e:  # network / rate limit: back off
            if attempt == 3:
                raise
            time.sleep(5 * (attempt + 1))


CLAUDE_CWD = os.path.expanduser("~/.cache/csf-re-claude")  # outside the repo: no CLAUDE.md / memory pulled in


def chat_claude(model, messages):
    """Claude via the Claude Code CLI in headless mode (user's subscription). No tools."""
    os.makedirs(CLAUDE_CWD, exist_ok=True)
    system = messages[0]["content"]
    convo = []
    for m in messages[1:]:
        tag = "PACKET / REQUEST" if m["role"] == "user" else "YOUR PREVIOUS ANSWER"
        convo.append(f"=== {tag} ===\n{m['content']}")
    r = subprocess.run(["claude", "-p", "--tools", "", "--model", model, "--no-session-persistence",
                        "--output-format", "json", "--system-prompt", system],
                       input="\n\n".join(convo), capture_output=True, text=True, cwd=CLAUDE_CWD, timeout=900)
    d = json.loads(r.stdout)
    if d.get("is_error") or d.get("terminal_reason") not in (None, "completed", "end_turn"):
        raise RuntimeError(f"claude -p failed: {d.get('terminal_reason')} {str(d.get('result'))[:200]}")
    u = d.get("usage", {})
    return d["result"], dict(prompt_tokens=u.get("input_tokens", 0) + u.get("cache_read_input_tokens", 0) +
                             u.get("cache_creation_input_tokens", 0), completion_tokens=u.get("output_tokens", 0),
                             backend="claude", model=model)


CODEX_CWD = os.path.expanduser("~/.cache/csf-re-codex")
CODEX_EFFORT = {"high": "high", "max": "xhigh"}


# Codex agent features that add tool schemas / instructions to every request; grunts need none of them.
CODEX_OFF = ["shell_tool", "unified_exec", "view_image", "multi_agent", "apps", "plugins", "browser_use",
             "browser_use_external", "computer_use", "image_generation", "goals", "sleep_tool", "tool_suggest",
             "skill_search", "hooks", "in_app_browser", "mentions_v2", "remote_plugin", "workspace_dependencies",
             "worktrees"]


def chat_codex(model, messages, effort="high"):
    """GPT via the Codex CLI headless (user's ChatGPT subscription). Ephemeral, read-only sandbox, no user config
    (no MCP servers), run outside the repo. Our system prompt replaces Codex's base instructions and the agent tools
    are off (~14k -> ~4k tokens of harness overhead per call); the conversation goes in as one prompt."""
    os.makedirs(CODEX_CWD, exist_ok=True)
    sysp = messages[0]["content"]
    instr = os.path.join(CODEX_CWD, f"sys-{hashlib.sha1(sysp.encode()).hexdigest()[:12]}.md")
    if not os.path.exists(instr):
        tmp = f"{instr}.{os.getpid()}.{threading.get_ident()}"
        open(tmp, "w").write(sysp + "\n\nAnswer directly from the packet. You have no tools.\n")
        os.replace(tmp, instr)
    convo = []
    for m in messages[1:]:
        tag = "PACKET / REQUEST" if m["role"] == "user" else "YOUR PREVIOUS ANSWER"
        convo.append(f"=== {tag} ===\n{m['content']}")
    out = os.path.join(CODEX_CWD, f"out-{os.getpid()}-{threading.get_ident()}.txt")
    feats = [a for f in CODEX_OFF for a in ("-c", f"features.{f}=false")]
    feats += ["-c", f'model_instructions_file="{instr}"', "-c", "web_search=disabled"]
    if os.environ.get("CSF_CODEX_SLIM") == "0":  # A/B: stock Codex harness, system prompt inlined
        feats, convo = [], [sysp, "Answer directly from the packet below. Do not run commands or use tools."] + convo
    r = subprocess.run(["codex", "exec", "--ephemeral", "--skip-git-repo-check", "--ignore-user-config",
                        "-s", "read-only", "-m", model, "-c", f'model_reasoning_effort="{CODEX_EFFORT.get(effort, effort)}"',
                        *feats,
                        "-o", out, "--json", "-"],
                       input="\n\n".join(convo), capture_output=True, text=True, cwd=CODEX_CWD, timeout=1800)
    if r.returncode or not os.path.exists(out):
        raise RuntimeError(f"codex exec failed rc={r.returncode}: {(r.stderr or r.stdout)[-300:]}")
    text = open(out).read()
    os.remove(out)
    u = {}
    for line in r.stdout.splitlines():
        if '"turn.completed"' in line:
            u = json.loads(line).get("usage", {})
    return text, dict(prompt_tokens=u.get("input_tokens", 0), cached_tokens=u.get("cached_input_tokens", 0),
                      completion_tokens=u.get("output_tokens", 0) + u.get("reasoning_output_tokens", 0),
                      backend="codex", model=model)


def ask(backend, model, messages, key, effort="high"):
    if backend == "claude":
        return chat_claude(model, messages)
    if backend == "codex":
        return chat_codex(model, messages, effort)
    return chat(model, messages, key, effort)


def parse(text):
    code = re.search(r"```(?:cpp|c\+\+)\s*\n(.*?)```", text, re.S)
    meta = re.search(r"```json\s*\n(.*?)```", text, re.S)
    try:
        m = json.loads(meta.group(1)) if meta else {}
    except json.JSONDecodeError:
        m = {}
    return (code.group(1) if code else None), m


REVIEW_SYSTEM = """You review a C++ reimplementation of one x86 function from a 2006 MSVC game against the original's
disassembly and pseudocode. Look only for real behavioral differences: wrong offsets, signedness, widths, float vs
double, missing/extra/reordered calls or memory accesses, wrong loop bounds, wrong constants, values cached across
calls where the original reloads. Ignore naming and style. Output exactly one ```json block:
{"ok": true|false, "issues": ["<concrete difference, with the asm address>", ...]}"""


def review(ea, packet, code):
    """Second opinion by Codex on a gate-passed file. Returns '' if fine, else the issues text."""
    msgs = [{"role": "system", "content": REVIEW_SYSTEM},
            {"role": "user", "content": f"{packet}\n\n## Reimplementation under review\n```cpp\n{code}\n```"}]
    try:
        text, usage = ask("codex", "gpt-6.1-sol", msgs, None, "high")
    except Exception:
        return "", None
    m = re.search(r"```json\s*\n(.*?)```", text, re.S)
    try:
        d = json.loads(m.group(1)) if m else {}
    except json.JSONDecodeError:
        d = {}
    if d.get("ok", True) or not d.get("issues"):
        return "", usage
    return "\n".join(f"- {i}" for i in d["issues"]), usage


def example_section(db, ex):
    path = os.path.join(ROOT, "src", "auto", f"{ex:08x}.cpp")
    if not os.path.exists(path):
        return ""
    return (f"\n\n## Same-shape function, already reimplemented and verified: 0x{ex:08X}\n"
            "The target has the same instruction shape (a clone: other element size / constants / callees). Follow this "
            "structure, but use the target's own addresses, constants, offsets and callee bindings from the packet above.\n"
            f"```cpp\n{open(path).read()}\n```")


def work(ea, db, key, ladder, out_dir=None, example=None, review_over=None):
    """ladder: [(backend, model, attempts), ...] tried in order; the conversation (with gate errors) carries over.
    example: ea of a done clone sibling (its file is shown). review_over: Codex-review DeepSeek outputs above N bytes."""
    packet = build(db, ea) + (example_section(db, example) if example else "")
    msgs = [{"role": "system", "content": SYSTEM}, {"role": "user", "content": packet}]
    path = os.path.join(out_dir or os.path.join(ROOT, "src", "auto"), f"{ea:08x}.cpp")
    os.makedirs(os.path.dirname(path), exist_ok=True)
    tmp = os.path.join(ROOT, "work", "grunt", ladder[0][0], f"{ea:08x}.cpp")
    os.makedirs(os.path.dirname(tmp), exist_ok=True)
    usage_all, err, meta, text = [], "", {}, ""
    steps = [(b, m) for b, m, n in ladder for _ in range(n)]
    dead = set()
    for attempt, (backend, model) in enumerate(steps):
        # first attempt per backend at high effort, retries at max
        first = attempt == 0 or steps[attempt - 1][0] != backend
        if backend in dead:
            continue
        try:
            text, usage = ask(backend, model, msgs, key, "high" if first else "max")
        except Exception as e:  # out of credit / CLI failure: fall through to the next backend
            dead.add(backend)
            err = f"{backend} error: {e}"
            continue
        usage_all.append(usage)
        code, meta = parse(text)
        if code:
            code = re.sub(r"^(\s*HOOK(?:_METHOD)?\(.*\))\s*$", r"\1;", code, flags=re.M)
        if not code:
            ok, err = False, "no ```cpp block found in your answer"
        else:
            open(tmp, "w").write(code)
            ok, err = gate(ea, tmp, db)
        if ok and backend == "deepseek" and (review_over or -1) >= 0 and db.F[ea]["size"] > review_over:
            issues, u = review(ea, packet, code)
            if u:
                usage_all.append(u)
            if issues:
                ok, err = False, "a reviewer compared your file with the original and found differences:\n" + issues
        if ok:
            os.replace(tmp, path)
            return dict(ea=ea, ok=True, attempts=attempt + 1, meta=meta, usage=usage_all, by=f"{backend}:{model}",
                        example=example)
        msgs += [{"role": "assistant", "content": text},
                 {"role": "user", "content": f"The gate rejected the file:\n{err}\nFix it and answer again in the same format."}]
    qdir = os.path.join(out_dir, "queue") if out_dir else os.path.join(ROOT, "queue")
    os.makedirs(qdir, exist_ok=True)
    with open(os.path.join(qdir, f"{ea:08x}.md"), "w") as f:
        f.write(f"# Escalation 0x{ea:08X}\n\n## Last gate error\n```\n{err}\n```\n\n## Last answer\n{text}\n\n"
                f"## Packet\n{packet}\n")
    return dict(ea=ea, ok=False, attempts=len(steps), err=err[:500], usage=usage_all)


def select(db, n, max_size, min_size=6, exclude=()):
    """Leaf-first: fewest not-done game callees, then smallest. Returns [(ea, example_ea|None)].
    Clone clusters: while no member is done, only one member is offered; once one is done, the others are offered
    with it as the example (cheap: same shape)."""
    rows = db.rows
    clones = json.load(open(os.path.join(ROOT, "export", "clones.json")))
    cluster = {}
    for i, c in enumerate(clones):
        for m in c["members"]:
            cluster[int(m, 16)] = i
    done_in = {}
    busy = set()
    for ea, i in cluster.items():
        if rows.get(ea, {}).get("status") in ("done", "verified"):
            done_in.setdefault(i, ea)
        if ea in exclude:
            busy.add(i)
    eas = sorted(db.F)
    room = {eas[i]: eas[i + 1] - eas[i] for i in range(len(eas) - 1)}
    cands = []
    for ea, r in rows.items():
        if r["status"] != "todo" or r["region"] != "game" or ea in exclude:
            continue
        f = db.F.get(ea)
        if not f or f["size"] > max_size or f["size"] < min_size or room.get(ea, 99) < 5 or not os.path.exists(os.path.join(ROOT, "export", "funcs", f"{ea:08x}.c")):
            continue
        if os.path.exists(os.path.join(ROOT, "queue", f"{ea:08x}.md")):
            continue
        ci = cluster.get(ea)
        ex = done_in.get(ci) if ci is not None else None
        if ci is not None and ex is None and ci in busy:
            continue  # a sibling is in flight; wait for it to become the example
        pending = sum(1 for c in f["callees"] if rows.get(c, {}).get("status") == "todo")
        cands.append((ex is None, pending, f["size"], ea, ex))
    cands.sort()
    out, taken = [], set()
    for _, _, _, ea, ex in cands:
        ci = cluster.get(ea)
        if ex is None and ci is not None:
            if ci in taken:
                continue
            taken.add(ci)
        out.append((ea, ex))
        if len(out) >= n:
            break
    return out


def smoke():
    r = subprocess.run([os.path.join(ROOT, "tools", "deploy.sh")], capture_output=True, text=True)
    if r.returncode:
        return False, "build/deploy failed:\n" + (r.stdout + r.stderr)[-3000:]
    r = subprocess.run([os.path.join(ROOT, "tools", "smoke.sh"), "60"], capture_output=True, text=True)
    return r.returncode == 0, r.stdout[-3000:]


def diff_rejects(good, smoke_out):
    """Functions of this batch with differential-test mismatches: removed from `good`, sent to queue/ with the log."""
    bad = {int(m.group(1), 16) for m in re.finditer(r"^MISMATCH ([0-9A-F]{8})", smoke_out, re.M)}
    rej = [r for r in good if r["ea"] in bad]
    for r in rej:
        ea = r["ea"]
        lines = "\n".join(l for l in smoke_out.splitlines() if l.startswith(f"DIFF {ea:08X}"))
        os.makedirs(os.path.join(ROOT, "queue"), exist_ok=True)
        src = os.path.join(ROOT, "src", "auto", f"{ea:08x}.cpp")
        code = open(src).read() if os.path.exists(src) else ""
        with open(os.path.join(ROOT, "queue", f"{ea:08x}.md"), "w") as f:
            f.write(f"# Escalation 0x{ea:08X}: differential test mismatch in game\n\n```\n{lines}\n```\n\n"
                    f"## Rejected file\n```cpp\n{code}\n```\n")
        if os.path.exists(src):
            os.remove(src)
    return [r for r in good if r["ea"] not in bad], rej


def run(args):
    db = DB()
    key = load_key()
    picks = [(int(x, 16), None) for x in args.only.split(",")] if args.only else select(db, args.n, args.max_size, args.min_size)
    targets = [ea for ea, _ in picks]
    example = dict(picks)
    esc = [("claude", args.claude_model, 2)] if args.escalate_claude else []
    codex = [("codex", args.codex_model, 2)]
    if args.backend == "claude":
        ladder_of = lambda ea: [("claude", args.claude_model, 2)]
    elif args.backend == "deepseek":
        ladder_of = lambda ea: [("deepseek", args.model, 2)] + esc
    elif args.backend == "codex":
        ladder_of = lambda ea: codex + esc
    else:  # auto: tiny functions to DeepSeek flash (codex as fallback), the rest to Codex
        ladder_of = lambda ea: ([("deepseek", args.model, 2)] if db.F[ea]["size"] < args.tiny else []) + codex + esc
    print(f"batch of {len(targets)}: {' '.join(f'{t:x}' for t in targets)}", flush=True)
    results = []
    with cf.ThreadPoolExecutor(args.jobs) as ex:
        out_dir = os.path.join(ROOT, "work", "ab", args.backend) if args.dry else None
        futs = {ex.submit(work, ea, db, key, ladder_of(ea), out_dir, example.get(ea), args.review_over): ea
                for ea in targets}
        for fu in cf.as_completed(futs):
            try:
                r = fu.result()
            except Exception as e:
                r = dict(ea=futs[fu], ok=False, err=f"exception: {e}", usage=[])
            results.append(r)
            with open(LOG, "a") as f:
                f.write(json.dumps(dict(r, ts=time.time())) + "\n")
            print(f"  {r['ea']:08x} {'OK ' if r['ok'] else 'ESC'} {r.get('meta', {}).get('name', '') if r['ok'] else r.get('err', '')[:100]}",
                  flush=True)
    good = [r for r in results if r["ok"]]
    print(f"gate: {len(good)}/{len(results)} passed", flush=True)
    if args.dry:
        return
    if not good:
        return
    if not args.no_smoke:
        ok, out = smoke()
        print(out, flush=True)
        if not ok:
            print("SMOKE FAILED: files left in src/auto uncommitted; bisect with csf_hook.ini (disable 0x...)")
            return
        good, rej = diff_rejects(good, out)
        if rej:
            print(f"diff mismatches: {len(rej)} sent to queue/: {' '.join(f'{r['ea']:08x}' for r in rej)}")
    commit(good, f"grunt batch: {len(good)} functions reimplemented ({args.backend})")


def commit(good, title):
    """Mark done, record proposals, regenerate globals.yaml, commit exactly these files."""
    rows = load_csv()
    with open(PROPOSALS, "a") as pf:
        for r in good:
            row = rows[r["ea"]]
            row["status"], row["module"] = "done", row["module"] or "auto"
            if r["meta"].get("name"):
                row["name"], row["name_src"] = r["meta"]["name"], "agent"
            pf.write(json.dumps(dict(ea=f"{r['ea']:08x}", **{k: r["meta"].get(k) for k in
                                                            ("name", "summary", "confidence", "field_names", "class_guess", "notes")})) + "\n")
    save_csv(rows)
    subprocess.run([sys.executable, os.path.join(ROOT, "tools", "globals.py")], check=False)
    files = [f"src/auto/{r['ea']:08x}.cpp" for r in good] + ["db/functions.csv", "db/proposals.jsonl", "db/globals.yaml"]
    files += [os.path.relpath(p, ROOT) for p in glob.glob(os.path.join(ROOT, "queue", "*.md"))]
    if subprocess.run(["git", "-C", ROOT, "ls-files", "--", "queue"], capture_output=True, text=True).stdout:
        files.append("queue")  # also stages escalations resolved (deleted) since the last commit
    subprocess.run(["git", "-C", ROOT, "add", "-A", "--", *files], check=False)
    msg = f"{title}\n\nCo-Authored-By: Claude Opus 5.5 <noreply@anthropic.com>"
    subprocess.run(["git", "-C", ROOT, "commit", "-q", "-m", msg, "--", *files], check=False)
    print("committed", flush=True)


def lanes(args):
    """Continuous grunting in two lanes: DeepSeek (tiny functions, and clone siblings that have an example; Codex as
    fallback) and Codex (the rest). Every --batch gate passes: build + in-mission smoke + commit. Stops on a smoke
    failure (files left uncommitted for bisecting) or after --total functions."""
    db = DB()
    key = load_key()
    esc = [("claude", args.claude_model, 2)] if args.escalate_claude else []
    codex = [("codex", args.codex_model, 2)]
    pools = {"deep": cf.ThreadPoolExecutor(args.tiny_jobs), "main": cf.ThreadPoolExecutor(args.main_jobs)}
    caps = {"deep": args.tiny_jobs * 2, "main": args.main_jobs * 2}
    inflight, good, submitted, total_ok, total_esc = {}, [], 0, 0, 0

    def lane_of(ea, ex):
        size = db.F[ea]["size"]
        if size < args.tiny or (ex is not None and size <= 400):
            return "deep", [("deepseek", args.model, 2)] + codex + esc
        return "main", codex + esc

    def refill():
        nonlocal submitted
        busy = {ea for ea, _ in inflight.values()}
        cnt = collections.Counter(l for _, l in inflight.values())
        if submitted >= args.total or all(cnt[l] >= caps[l] for l in caps):
            return
        for ea, ex in select(db, 600, args.max_size, args.min_size, exclude=busy):
            lane, ladder = lane_of(ea, ex)
            if cnt[lane] >= caps[lane]:
                continue
            fut = pools[lane].submit(work, ea, db, key, ladder, None, ex, args.review_over)
            inflight[fut] = (ea, lane)
            cnt[lane] += 1
            submitted += 1
            if submitted >= args.total:
                break

    def flush():
        nonlocal good
        if not good:
            return True
        if not args.no_smoke:
            ok, out = smoke()
            print(out[-600:], flush=True)
            if not ok:
                print(f"SMOKE FAILED with {len(good)} new functions: left uncommitted in src/auto; bisect with "
                      "csf_hook.ini `disable 0x...`", flush=True)
                return False
            good, rej = diff_rejects(good, out)
            if rej:
                print(f"diff mismatches: {len(rej)} sent to queue/", flush=True)
        commit(good, f"grunt lanes: {len(good)} functions reimplemented")
        good = []
        return True

    refill()
    while inflight:
        done, _ = cf.wait(list(inflight), timeout=60, return_when=cf.FIRST_COMPLETED)
        for fu in done:
            ea, lane = inflight.pop(fu)
            try:
                r = fu.result()
            except Exception as e:
                r = dict(ea=ea, ok=False, err=f"exception: {e}", usage=[])
            with open(LOG, "a") as f:
                f.write(json.dumps(dict(r, ts=time.time(), lane=lane)) + "\n")
            if r["ok"]:
                db.rows[ea]["status"] = "done"  # in memory: siblings may now use it as their example
                good.append(r)
                total_ok += 1
            else:
                total_esc += 1
            print(f"  [{lane}] {ea:08x} {'OK ' if r['ok'] else 'ESC'} "
                  f"{r.get('meta', {}).get('name', '') if r['ok'] else r.get('err', '')[:90]}"
                  f"{'  (example %08x)' % r['example'] if r.get('example') else ''}", flush=True)
        if len(good) >= args.batch and not flush():
            for p in pools.values():
                p.shutdown(wait=True, cancel_futures=True)
            return
        refill()
    flush()
    print(f"lanes done: {total_ok} ok, {total_esc} escalated", flush=True)


def cost(_):
    tin = tout = cached = n = ok = 0
    for l in open(LOG):
        r = json.loads(l)
        n += 1
        ok += r["ok"]
        for u in r.get("usage", []):
            tin += u.get("prompt_tokens", 0)
            tout += u.get("completion_tokens", 0)
            cached += u.get("prompt_cache_hit_tokens", 0)
    print(f"tasks {n} ok {ok}  prompt {tin} (cached {cached})  completion {tout}  "
          f"avg/task in {tin // max(n, 1)} out {tout // max(n, 1)}")


if __name__ == "__main__":
    ap = argparse.ArgumentParser()
    sub = ap.add_subparsers(dest="cmd", required=True)
    r = sub.add_parser("run")
    r.add_argument("--n", type=int, default=10)
    r.add_argument("--jobs", type=int, default=8)
    r.add_argument("--max-size", type=int, default=200)
    r.add_argument("--only")
    r.add_argument("--model", default="deepseek-flash")
    r.add_argument("--backend", choices=["auto", "deepseek", "codex", "claude"], default="auto")
    r.add_argument("--codex-model", default="gpt-6.1-sol")
    r.add_argument("--tiny", type=int, default=48, help="auto: functions smaller than this go to DeepSeek flash")
    r.add_argument("--claude-model", default="sonnet")
    r.add_argument("--escalate-claude", action="store_true", help="after 2 DeepSeek failures, try Claude twice")
    r.add_argument("--min-size", type=int, default=6)
    r.add_argument("--dry", action="store_true", help="write to work/ab/<backend>/, no smoke/commit/status")
    r.add_argument("--no-smoke", action="store_true")
    r.add_argument("--review-over", type=int, default=100, help="Codex-review DeepSeek outputs above N bytes (-1: off)")
    l = sub.add_parser("lanes")
    l.add_argument("--total", type=int, default=200)
    l.add_argument("--batch", type=int, default=50, help="smoke + commit every N gate passes")
    l.add_argument("--tiny-jobs", type=int, default=8)
    l.add_argument("--main-jobs", type=int, default=6)
    l.add_argument("--max-size", type=int, default=600)
    l.add_argument("--min-size", type=int, default=6)
    l.add_argument("--tiny", type=int, default=48)
    l.add_argument("--model", default="deepseek-flash")
    l.add_argument("--codex-model", default="gpt-6.1-sol")
    l.add_argument("--claude-model", default="sonnet")
    l.add_argument("--escalate-claude", action="store_true")
    l.add_argument("--review-over", type=int, default=100)
    l.add_argument("--no-smoke", action="store_true")
    sub.add_parser("cost")
    a = ap.parse_args()
    {"run": run, "lanes": lanes, "cost": cost}[a.cmd](a)
