#!/usr/bin/env python3
"""Grunt driver: packet -> DeepSeek -> gate (1 retry) -> src/auto/<ea>.cpp, or escalate to queue/.

Usage:
  grunt.py run [--n 10] [--jobs 8] [--max-size 200] [--only ea,ea] [--model deepseek-flash] [--no-smoke]
  grunt.py lanes [--total 200] [--batch 60] [--tiny-jobs 8] [--main-jobs 6]   continuous, two lanes (see lanes())
  grunt.py cost                      token usage, review hit rate from work/grunt_log.jsonl

Stop a lanes run cleanly: `touch work/grunt_stop` (in-flight calls finish, the batch is smoked and committed).

After a batch: link, deploy, smoke test (unless --no-smoke); on success mark functions done and git commit.
"""
import hashlib, argparse, collections, concurrent.futures as cf, glob, json, os, re, subprocess, sys, threading, time, urllib.request
sys.path.insert(0, os.path.dirname(__file__))
from packet import DB, build
from gate import gate
from triage import load_csv, save_csv
from limits import codex_usage
import evidence

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
 "notes": "<anything uncertain>"}- A tail jump (`jmp` to another function / `jmp dword ptr [addr]` without a matching `call`) forwards the caller's
  stack arguments and registers unchanged. Reproduce it with the callee's exact signature, or, when that is unknown
  (indirect jmp through a pointer / vtable slot), as a naked thunk: `static __declspec(naked) void F() { __asm { jmp dword ptr ds:[0xADDR] } }`
  — at most 8 of mov/lea/add/sub/test/cmp/je/jne/ret/call reg plus labels, around the indirect jmp, copied from the original.
- Return width: `xor eax, eax` / `movzx eax, ...` before `setcc al` (or any full-eax result) means a 32-bit
  int/BOOL return; declare it `int`, never `bool` (bool leaves eax's upper bytes undefined; callers test eax).
  Only a bare `mov al, ...` / `setcc al` without zeroing eax is a genuine byte/bool return.
"""


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


# Live state for tools/monitor.py: in-flight model calls, per-backend token totals, run counters.
LIVE = os.path.join(ROOT, "work", "grunt_state.json")
_live_lock = threading.Lock()
_live = dict(pid=os.getpid(), started=time.time(), mode="", phase="", calls={}, tokens={}, counts={})
_tl = threading.local()


def live(**kw):
    with _live_lock:
        for k, v in kw.items():
            if isinstance(v, dict) and isinstance(_live.get(k), dict) and k == "counts":
                _live[k].update(v)
            else:
                _live[k] = v
        _live["updated"] = time.time()
        tmp = f"{LIVE}.{os.getpid()}"
        with open(tmp, "w") as f:
            json.dump(_live, f)
        os.replace(tmp, LIVE)


CODEX_PAUSED = threading.Event()  # set by the lanes quota guard: no new Codex calls (writes, fallbacks, reviews)


CALLS = os.path.join(ROOT, "work", "calls")  # full transcript of every model call (gitignored)


def log_call(backend, model, effort, messages, text, usage, secs, error=None):
    """work/calls/<ea>/<time>-<kind>-<backend>.json: the whole conversation + answer. System prompts are stored once
    under work/calls/system/<sha>.txt and referenced by hash (they're large and constant)."""
    try:
        ea = getattr(_tl, "ea", 0)
        kind = "review" if messages[0]["content"] is REVIEW_SYSTEM else "write"
        msgs = []
        for m in messages:
            if m["role"] == "system":
                h = hashlib.sha1(m["content"].encode()).hexdigest()[:12]
                sp = os.path.join(CALLS, "system", f"{h}.txt")
                if not os.path.exists(sp):
                    os.makedirs(os.path.dirname(sp), exist_ok=True)
                    open(sp, "w").write(m["content"])
                msgs.append(dict(role="system", sha=h))
            else:
                msgs.append(m)
        d = os.path.join(CALLS, f"{ea:08x}")
        os.makedirs(d, exist_ok=True)
        stamp = time.strftime("%Y%m%d-%H%M%S") + f"-{int(time.time() * 1000) % 1000:03d}"
        with open(os.path.join(d, f"{stamp}-{kind}-{backend}.json"), "w") as f:
            json.dump(dict(ea=f"{ea:08x}", kind=kind, backend=backend, model=model, effort=effort, secs=round(secs, 1),
                           usage=usage, error=error, messages=msgs, answer=text), f, indent=1)
    except Exception as e:  # logging must never break a run
        print(f"log_call failed: {e}", file=sys.stderr)


def ask(backend, model, messages, key, effort="high"):
    if backend == "codex" and CODEX_PAUSED.is_set():
        raise RuntimeError("Codex paused by the quota guard")
    t0 = time.time()
    try:
        text, usage = _ask(backend, model, messages, key, effort)
    except Exception as e:
        log_call(backend, model, effort, messages, None, None, time.time() - t0, error=str(e))
        raise
    log_call(backend, model, effort, messages, text, usage, time.time() - t0)
    return text, usage


def _ask(backend, model, messages, key, effort="high"):
    cid = f"{threading.get_ident()}"
    with _live_lock:
        _live["calls"][cid] = dict(ea=getattr(_tl, "ea", 0), backend=backend, model=model, effort=effort,
                                   kind="review" if messages[0]["content"] is REVIEW_SYSTEM else "write",
                                   since=time.time())
    live()
    try:
        if backend == "claude":
            text, usage = chat_claude(model, messages)
        elif backend == "codex":
            text, usage = chat_codex(model, messages, effort)
        else:
            text, usage = chat(model, messages, key, effort)
        with _live_lock:
            t = _live["tokens"].setdefault(backend, dict(calls=0, prompt=0, cached=0, completion=0))
            t["calls"] += 1
            t["prompt"] += usage.get("prompt_tokens", 0) or 0
            t["cached"] += usage.get("cached_tokens", 0) or 0
            t["completion"] += usage.get("completion_tokens", 0) or 0
        return text, usage
    finally:
        with _live_lock:
            _live["calls"].pop(cid, None)
        live()


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
    """Second opinion by Codex on a gate-passed file. Returns '' if fine, the issues text, or None if not run."""
    msgs = [{"role": "system", "content": REVIEW_SYSTEM},
            {"role": "user", "content": f"{packet}\n\n## Reimplementation under review\n```cpp\n{code}\n```"}]
    try:
        text, usage = ask("codex", "gpt-6.1-sol", msgs, None, "high")
    except Exception:
        return None, None
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


def work(ea, db, key, ladder, out_dir=None, example=None, review_over=None, evidence_budget=0):
    """ladder: [(backend, model, attempts), ...] tried in order; the conversation (with gate errors) carries over.
    example: ea of a done clone sibling (its file is shown). review_over: Codex-review DeepSeek outputs above N bytes.
    evidence_budget: read-only evidence requests the model may make (tools/evidence.py); they aren't attempts."""
    _tl.ea = ea
    packet = build(db, ea) + (example_section(db, example) if example else "")
    ev_left, n_ev = evidence_budget, 0
    if evidence_budget:
        packet += "\n" + evidence.PROMPT % evidence_budget
    msgs = [{"role": "system", "content": SYSTEM}, {"role": "user", "content": packet}]
    path = os.path.join(out_dir or os.path.join(ROOT, "src", "auto"), f"{ea:08x}.cpp")
    os.makedirs(os.path.dirname(path), exist_ok=True)
    tmp = os.path.join(ROOT, "work", "grunt", ladder[0][0], f"{ea:08x}.cpp")
    os.makedirs(os.path.dirname(tmp), exist_ok=True)
    usage_all, err, meta, text = [], "", {}, ""
    steps = [(b, m) for b, m, n in ladder for _ in range(n)]
    dead, reviews = set(), []  # reviews: True = reviewer rejected that attempt
    for attempt, (backend, model) in enumerate(steps):
        # first attempt per backend at high effort, retries at max
        first = attempt == 0 or steps[attempt - 1][0] != backend
        if backend in dead:
            continue
        try:
            text, usage = ask(backend, model, msgs, key, "high" if first else "max")
            usage_all.append(usage)
            code, meta = parse(text)
            while not code and isinstance(meta.get("actions"), list) and meta["actions"] and ev_left > 0:
                reply, used = evidence.answer(db, meta["actions"], ev_left)
                ev_left, n_ev = ev_left - used, n_ev + used
                msgs += [{"role": "assistant", "content": text}, {"role": "user", "content": reply}]
                text, usage = ask(backend, model, msgs, key, "high" if first else "max")
                usage_all.append(usage)
                code, meta = parse(text)
        except Exception as e:  # out of credit / CLI failure: fall through to the next backend
            dead.add(backend)
            err = f"{backend} error: {e}"
            continue
        if code:
            code = re.sub(r"^(\s*HOOK(?:_METHOD)?\(.*\))\s*$", r"\1;", code, flags=re.M)
        if not code:
            ok, err = False, "no ```cpp block found in your answer"
        else:
            open(tmp, "w").write(code)
            ok, err = gate(ea, tmp, db)
        if ok and backend == "deepseek" and review_over is not None and review_over >= 0 \
                and db.F[ea]["size"] > review_over:
            issues, u = review(ea, packet, code)
            if u:
                usage_all.append(u)
            if issues is not None:
                reviews.append(bool(issues))
            if issues:
                ok, err = False, "a reviewer compared your file with the original and found differences:\n" + issues
        if ok:
            os.replace(tmp, path)
            return dict(ea=ea, ok=True, attempts=attempt + 1, meta=meta, usage=usage_all, by=f"{backend}:{model}",
                        example=example, reviews=reviews, evidence=n_ev)
        msgs += [{"role": "assistant", "content": text},
                 {"role": "user", "content": f"The gate rejected the file:\n{err}\nFix it and answer again in the same format."}]
    if "codex" in dead and CODEX_PAUSED.is_set():  # not a real failure: retry in a later run
        return dict(ea=ea, ok=False, deferred=True, attempts=len(usage_all), err=err[:500], usage=usage_all,
                    reviews=reviews)
    qdir = os.path.join(out_dir, "queue") if out_dir else os.path.join(ROOT, "queue")
    os.makedirs(qdir, exist_ok=True)
    with open(os.path.join(qdir, f"{ea:08x}.md"), "w") as f:
        f.write(f"# Escalation 0x{ea:08X}\n\n## Last gate error\n```\n{err}\n```\n\n## Last answer\n{text}\n\n"
                f"## Packet\n{packet}\n")
    return dict(ea=ea, ok=False, attempts=len(steps), err=err[:500], usage=usage_all, reviews=reviews, evidence=n_ev)


_clusters = None


def ev_budget(args, db, ea):
    """Evidence requests offered for this function (none for small ones: their packet is complete enough)."""
    return args.evidence if db.F[ea]["size"] > args.evidence_over else 0


def clone_clusters():
    global _clusters
    if _clusters is None:
        _clusters = {int(m, 16): i for i, c in enumerate(json.load(open(os.path.join(ROOT, "export", "clones.json"))))
                     for m in c["members"]}
    return _clusters


def select(db, n, max_size, min_size=6, exclude=()):
    """Leaf-first: fewest not-done game callees, then smallest. Returns [(ea, example_ea|None)].
    Clone clusters: while no member is done, only one member is offered; once one is done, the others are offered
    with it as the example (cheap: same shape)."""
    rows = db.rows
    cluster = clone_clusters()
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


SMOKE_SECS, SMOKE_LONG_SECS, SMOKE_LONG_EVERY = 15, 60, 5  # seconds after ready (smoke.sh); every 5th smoke runs long
# Long smokes rotate the start (wider coverage for db/verify.json); short ones always use smoke.sh's FR01 scene.
SMOKE_STARTS = [{}, {"CSF_START": "menu", "CSF_INI_EXTRA": "menutour 1500"}, {"CSF_START": "loadsave 1.sav"}]
_last_smoke = (None, {})  # (secs, env) of the last deployed smoke: a bisect repeats exactly that run
_smokes = 0


def smoke(deploy=True, secs=None):
    global _smokes, _last_smoke
    if deploy:
        r = subprocess.run([os.path.join(ROOT, "tools", "deploy.sh")], capture_output=True, text=True)
        if r.returncode:
            return False, "build/deploy failed:\n" + (r.stdout + r.stderr)[-3000:]
        _smokes += 1
        long = _smokes % SMOKE_LONG_EVERY == 0
        env = SMOKE_STARTS[(_smokes // SMOKE_LONG_EVERY) % len(SMOKE_STARTS)] if long else {}
        _last_smoke = (secs or (SMOKE_LONG_SECS if long else SMOKE_SECS), env)
    secs, env = (secs or _last_smoke[0] or SMOKE_SECS), _last_smoke[1]
    r = subprocess.run([os.path.join(ROOT, "tools", "smoke.sh"), str(secs)], capture_output=True, text=True,
                       env={**os.environ, **env})
    return r.returncode == 0, r.stdout  # full: diff_rejects() needs the MISMATCH lines near the top


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


PLAY = os.environ.get("CSF_PLAY", "/mnt/e/dev/csf-play")
BISECT_TAG = "# grunt-bisect"


def set_disabled(eas):
    """Disable exactly these hooks in the play copy's ini (lines tagged BISECT_TAG; others are left alone)."""
    ini = os.path.join(PLAY, "csf_hook.ini")
    lines = [l for l in open(ini).read().splitlines() if not l.endswith(BISECT_TAG)]
    lines += [f"disable 0x{ea:X} {BISECT_TAG}" for ea in sorted(eas)]
    open(ini, "w").write("\n".join(lines) + "\n")


def bisect(batch, max_culprits=4):
    """The deployed DLL holds `batch` (uncommitted) and its smoke failed. Find the culprits by toggling hooks in the
    ini (no rebuild): control run with the whole batch off, then binary search with the rest of the batch off.
    Returns (culprits, why): culprits=None when the batch is not to blame or the result is unreliable."""
    n = 0

    def fails(on):
        nonlocal n
        n += 1
        set_disabled(set(batch) - set(on))
        live(phase=f"bisect: smoke {n} ({len(on)} on)")
        return not smoke(deploy=False)[0]

    try:
        if fails([]):
            return None, "control run (whole batch disabled) also fails: crash is not from this batch"
        culprits, rest = [], list(batch)
        while fails(rest):
            if len(culprits) >= max_culprits:
                return None, f"more than {max_culprits} culprits: {' '.join(f'{c:x}' for c in culprits)} ..."
            cur = rest
            while len(cur) > 1:
                a, b = cur[:len(cur) // 2], cur[len(cur) // 2:]
                if fails(a):
                    cur = a
                elif fails(b):
                    cur = b
                else:
                    return None, f"neither half of {len(cur)} fails alone (interaction or flaky crash)"
            culprits.append(cur[0])
            rest.remove(cur[0])
        if not culprits:
            return None, "the full batch passed on re-run: flaky crash, not bisectable"
        return culprits, f"{n} smokes"
    finally:
        set_disabled([])


def quarantine(ea, why, smoke_out=""):
    """Move an uncommitted src/auto file to queue/ (excluded from select() from now on)."""
    src = os.path.join(ROOT, "src", "auto", f"{ea:08x}.cpp")
    code = open(src).read() if os.path.exists(src) else ""
    os.makedirs(os.path.join(ROOT, "queue"), exist_ok=True)
    with open(os.path.join(ROOT, "queue", f"{ea:08x}.md"), "w") as f:
        f.write(f"# Escalation 0x{ea:08X}: {why}\n\n## Smoke log tail\n```\n{smoke_out[-2500:]}\n```\n\n"
                f"## Rejected file\n```cpp\n{code}\n```\n")
    if os.path.exists(src):
        os.remove(src)


STOP = os.path.join(ROOT, "work", "grunt_stop")


def run(args):
    db = DB()
    key = load_key()
    picks = [(int(x, 16), None) for x in args.only.split(",")] if args.only else select(db, args.n, args.max_size, args.min_size)
    targets = [ea for ea, _ in picks]
    example = dict(picks)
    live(mode="run" + (" (dry)" if args.dry else ""), phase="grunting", counts=dict(total=len(targets), submitted=len(targets)))
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
        futs = {ex.submit(work, ea, db, key, ladder_of(ea), out_dir, example.get(ea), args.review_over,
                          ev_budget(args, db, ea)): ea
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
    files = [f"src/auto/{r['ea']:08x}.cpp" for r in good] + ["db/functions.csv", "db/proposals.jsonl", "db/globals.yaml",
                                                       "db/verify.json"]
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
    stopping, quota_t = False, 0
    if os.path.exists(STOP):
        os.remove(STOP)
    live(mode="lanes", phase="grunting", counts=dict(total=args.total, batch=args.batch, ok=0, esc=0, pending=0,
                                                      submitted=0, committed=0))

    def lane_of(ea, ex):
        size = db.F[ea]["size"]
        if size < args.tiny or (ex is not None and size <= 400):
            return "deep", [("deepseek", args.model, 2)] + codex + esc
        return "main", codex + esc

    def quota():
        """Pause/resume Codex on the 5h window (checked every 2 min). While paused the main lane gets no new work and
        the DeepSeek lane runs without Codex fallback/review."""
        nonlocal quota_t
        if args.codex_cap <= 0 or time.time() - quota_t < 120:
            return
        quota_t = time.time()
        try:
            pct, reset = codex_usage()["5h"]
        except Exception:
            return  # endpoint down: keep the current state
        if pct >= args.codex_cap and not CODEX_PAUSED.is_set():
            CODEX_PAUSED.set()
            print(f"Codex 5h window at {pct:.0f}% (cap {args.codex_cap}): main lane paused, resets in {reset // 60} min",
                  flush=True)
        elif pct < args.codex_cap and CODEX_PAUSED.is_set():
            CODEX_PAUSED.clear()
            print(f"Codex 5h window at {pct:.0f}%: main lane resumed", flush=True)
        live(codex_paused=CODEX_PAUSED.is_set(), codex_5h=pct)

    def refill():
        nonlocal submitted, stopping
        if not stopping and os.path.exists(STOP):
            stopping = True
            print("stop file found: finishing in-flight tasks, then smoke + commit", flush=True)
            live(phase="stopping")
        if stopping:
            return
        quota()
        busy = {ea for ea, _ in inflight.values()}
        cnt = collections.Counter(l for _, l in inflight.values())
        if CODEX_PAUSED.is_set():
            cnt["main"] = caps["main"]
        if submitted >= args.total or all(cnt[l] >= caps[l] for l in caps):
            return
        # wide window: the leaf-first head is almost all tiny (DeepSeek lane); the Codex lane needs larger candidates
        for ea, ex in select(db, 4000, args.max_size, args.min_size, exclude=busy):
            lane, ladder = lane_of(ea, ex)
            if cnt[lane] >= caps[lane]:
                continue
            fut = pools[lane].submit(work, ea, db, key, ladder, None, ex, args.review_over, ev_budget(args, db, ea))
            inflight[fut] = (ea, lane)
            cnt[lane] += 1
            submitted += 1
            if submitted >= args.total:
                break
        live(counts=dict(submitted=submitted))

    def flush():
        nonlocal good, total_ok, total_esc
        if not good:
            return True
        live(phase="smoke")
        if not args.no_smoke:
            ok, out = smoke()
            print(out[-1500:], flush=True)
            if not ok and not out.startswith("build/deploy failed") and not args.no_bisect:
                print(f"smoke failed with {len(good)} new functions: bisecting", flush=True)
                culprits, why = bisect([r["ea"] for r in good])
                print(f"bisect: {why}" + (f"; culprits {' '.join(f'{c:x}' for c in culprits)}" if culprits else ""),
                      flush=True)
                if culprits:
                    for c in culprits:
                        quarantine(c, "in-game smoke failure (lanes auto-bisect)", out)
                        db.rows[c]["status"] = "todo"
                    good = [r for r in good if r["ea"] not in culprits]
                    total_ok, total_esc = total_ok - len(culprits), total_esc + len(culprits)
                    live(counts=dict(ok=total_ok, esc=total_esc, pending=len(good)))
                    return flush()
            if not ok:
                print(f"SMOKE FAILED with {len(good)} new functions: left uncommitted in src/auto; bisect with "
                      "csf_hook.ini `disable 0x...`", flush=True)
                live(phase="SMOKE FAILED")
                return False
            good, rej = diff_rejects(good, out)
            if rej:
                print(f"diff mismatches: {len(rej)} sent to queue/", flush=True)
        commit(good, f"grunt lanes: {len(good)} functions reimplemented")
        live(phase="grunting", counts=dict(committed=_live["counts"].get("committed", 0) + len(good), pending=0))
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
            elif r.get("deferred"):
                db.rows[ea]["status"] = "deferred"  # in memory only: not re-picked this run
            else:
                total_esc += 1
            live(counts=dict(ok=total_ok, esc=total_esc, pending=len(good)))
            print(f"  [{lane}] {ea:08x} {'OK ' if r['ok'] else 'DEF' if r.get('deferred') else 'ESC'} "
                  f"{r.get('meta', {}).get('name', '') if r['ok'] else r.get('err', '')[:90]}"
                  f"{'  (example %08x)' % r['example'] if r.get('example') else ''}", flush=True)
        if len(good) >= args.batch and not flush():
            for p in pools.values():
                p.shutdown(wait=True, cancel_futures=True)
            return
        refill()
    if flush():
        live(phase="stopped" if stopping else "finished")
    print(f"lanes done: {total_ok} ok, {total_esc} escalated", flush=True)


def cost(_):
    """Token totals per writing backend, and how often the Codex review of DeepSeek outputs rejected one (logged
    since the `reviews` field exists): if it almost never rejects, --review-over -1 saves Codex quota."""
    by = collections.defaultdict(lambda: dict(n=0, ok=0, prompt=0, cached=0, completion=0))
    reviewed = rejected = rev_fns = rev_fns_rej = 0
    ev = dict(fns=0, reqs=0, ok=0, offered=0)
    for l in open(LOG):
        r = json.loads(l)
        if "evidence" in r:  # logged since evidence requests exist
            ev["offered"] += 1
            if r["evidence"]:
                ev["fns"] += 1
                ev["reqs"] += r["evidence"]
                ev["ok"] += bool(r["ok"])
        b = by[(r.get("by") or ("unknown" if r["ok"] else "failed")).split(":")[0]]
        b["n"] += 1
        b["ok"] += bool(r["ok"])
        for u in r.get("usage") or []:
            b["prompt"] += u.get("prompt_tokens", 0) or 0
            b["completion"] += u.get("completion_tokens", 0) or 0
            b["cached"] += (u.get("cached_tokens") or u.get("prompt_cache_hit_tokens") or 0)
        rv = r.get("reviews")
        if rv:
            reviewed += len(rv)
            rejected += sum(rv)
            rev_fns += 1
            rev_fns_rej += any(rv)
    for k, b in sorted(by.items()):
        print(f"{k:9} tasks {b['n']:5} ok {b['ok']:5}  prompt {b['prompt']:>10} (cached {b['cached']:>9})  "
              f"completion {b['completion']:>9}")
    print(f"codex reviews: {reviewed} on {rev_fns} functions, {rejected} rejections "
          f"({rev_fns_rej} functions had >=1 rejection)" if reviewed else "codex reviews: none logged yet")
    print(f"evidence requests: {ev['reqs']} in {ev['fns']} of {ev['offered']} logged tasks ({ev['ok']} of those passed)")


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
    r.add_argument("--evidence", type=int, default=3, help="evidence requests per function (tools/evidence.py)")
    r.add_argument("--evidence-over", type=int, default=150, help="offer evidence requests above N bytes")
    r.add_argument("--review-over", type=int, default=100, help="Codex-review DeepSeek outputs above N bytes (-1: off)")
    l = sub.add_parser("lanes")
    l.add_argument("--total", type=int, default=200)
    l.add_argument("--batch", type=int, default=60, help="smoke + commit every N gate passes")
    l.add_argument("--tiny-jobs", type=int, default=8)
    l.add_argument("--main-jobs", type=int, default=6)
    l.add_argument("--max-size", type=int, default=600)
    l.add_argument("--min-size", type=int, default=6)
    l.add_argument("--tiny", type=int, default=48)
    l.add_argument("--model", default="deepseek-flash")
    l.add_argument("--codex-model", default="gpt-6.1-sol")
    l.add_argument("--claude-model", default="sonnet")
    l.add_argument("--escalate-claude", action="store_true")
    l.add_argument("--evidence", type=int, default=3, help="evidence requests per function (tools/evidence.py)")
    l.add_argument("--evidence-over", type=int, default=150, help="offer evidence requests above N bytes")
    l.add_argument("--review-over", type=int, default=100)
    l.add_argument("--no-smoke", action="store_true")
    l.add_argument("--no-bisect", action="store_true", help="stop on a smoke failure instead of bisecting it")
    l.add_argument("--codex-cap", type=float, default=90, help="pause Codex at this 5h-window %% (0: off)")
    sub.add_parser("cost")
    a = ap.parse_args()
    {"run": run, "lanes": lanes, "cost": cost}[a.cmd](a)
