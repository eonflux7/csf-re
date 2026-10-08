#!/usr/bin/env python3
"""Grunt driver: packet -> DeepSeek -> gate (1 retry) -> src/auto/<ea>.cpp, or escalate to queue/.

Usage:
  grunt.py run [--n 10] [--jobs 8] [--max-size 200] [--only ea,ea] [--model deepseek-flash] [--no-smoke]
  grunt.py lanes [--total 200] [--batch 60] [--module m1,m2]   continuous, one lane with size tiers S/M/L (see lanes())
  grunt.py cost                      token usage, review hit rate from work/grunt_log.jsonl

Stop a lanes run cleanly: `touch work/grunt_stop` (in-flight calls finish, the batch is smoked and committed).

After a batch: link, deploy, smoke test (unless --no-smoke); on success mark functions done and git commit.
"""
import hashlib, argparse, collections, concurrent.futures as cf, glob, json, os, re, subprocess, sys, threading, time, urllib.error, urllib.request
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
  C_0082d70c_Destroy. Functions outside a class: prefix by the packet's Module in PascalCase when it fits
  (module bdd_weapon_defs -> WeaponDefs_ParseMuzzleFlash). Name local variables meaningfully.

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
# The grunts' own Claude account (a second subscription, logged in once with
# `CLAUDE_CONFIG_DIR=~/.config/csf-re/claude-grunts claude` -> /login); the captain keeps the main account.
CLAUDE_GRUNTS = os.path.expanduser("~/.config/csf-re/claude-grunts")


USAGE_CACHE = {"main": os.path.join(ROOT, "work", "claude_usage.json"),
               "grunts": os.path.join(ROOT, "work", "claude_grunts_usage.json")}


def claude_usage(which="main"):
    """5h / 7d windows of a Claude plan (main = the captain's account, grunts = CLAUDE_GRUNTS), from the endpoint
    Claude Code's /usage reads (the account's own OAuth token). Rate-limited (429) when polled often: asked at most
    once a minute (shared by monitor and lanes through work/claude_*usage.json), after an error again after its
    Retry-After or 5 min; meanwhile the last good answer is returned with its age (`_age`)."""
    from datetime import datetime
    cache = USAGE_CACHE[which]
    try:
        c = json.load(open(cache))
    except (OSError, ValueError):
        c = {}
    wait = c.get("retry_after") or 300 if c.get("error") else 60
    if time.time() - c.get("asked", 0) >= wait:
        c["asked"] = time.time()
        try:
            cred = os.path.join(CLAUDE_GRUNTS if which == "grunts" else os.path.expanduser("~/.claude"),
                                ".credentials.json")
            tok = json.load(open(cred))["claudeAiOauth"]["accessToken"]
            req = urllib.request.Request("https://api.anthropic.com/api/oauth/usage", headers={
                "Authorization": f"Bearer {tok}", "anthropic-beta": "oauth-2025-04-20", "User-Agent": "csf-re-monitor"})
            d = json.load(urllib.request.urlopen(req, timeout=8))
            iso = lambda v: datetime.fromisoformat(v.replace("Z", "+00:00")).timestamp()  # noqa: E731
            c.update(error=None, at=time.time(), u={k: (d[w]["utilization"], iso(d[w]["resets_at"])) for k, w in
                                                     (("5h", "five_hour"), ("7d", "seven_day")) if d.get(w)})
        except Exception as e:
            c["error"] = str(e)[:60]
            ra = getattr(e, "headers", None) and e.headers.get("retry-after")
            c["retry_after"] = min(3600, max(60, int(ra))) if ra and str(ra).isdigit() else None
        tmp = cache + ".tmp"
        json.dump(c, open(tmp, "w"))
        os.replace(tmp, cache)
    if not c.get("u"):
        raise RuntimeError(c.get("error") or "no data yet")
    out = {k: (pct, at - time.time()) for k, (pct, at) in c["u"].items()}
    out["_age"] = time.time() - c.get("at", 0)
    return out


def chat_claude(model, messages, effort=None):
    """Claude via the Claude Code CLI in headless mode, on the grunts' own account (never the captain's). No tools."""
    if not claude_grunts_ready():
        raise RuntimeError(f"grunts' Claude account not logged in: CLAUDE_CONFIG_DIR={CLAUDE_GRUNTS} claude, /login")
    os.makedirs(CLAUDE_CWD, exist_ok=True)
    env = dict(os.environ, CLAUDE_CONFIG_DIR=CLAUDE_GRUNTS)
    eff = ["--effort", effort] if effort in ("low", "medium", "high", "xhigh", "max") else []
    system = messages[0]["content"]
    convo = []
    for m in messages[1:]:
        tag = "PACKET / REQUEST" if m["role"] == "user" else "YOUR PREVIOUS ANSWER"
        convo.append(f"=== {tag} ===\n{m['content']}")
    # no tools/settings/MCP/skills: ~560 fixed tokens instead of ~4k (the account's claude.ai connectors alone ~3.4k)
    r = subprocess.run(["claude", "-p", "--tools", "", "--model", model, "--no-session-persistence",
                        "--strict-mcp-config", "--setting-sources", "", "--disable-slash-commands",
                        "--output-format", "json", "--system-prompt", system, *eff],
                       input="\n\n".join(convo), capture_output=True, text=True, cwd=CLAUDE_CWD, timeout=900, env=env)
    d = json.loads(r.stdout)
    if d.get("is_error") or d.get("terminal_reason") not in (None, "completed", "end_turn"):
        raise RuntimeError(f"claude -p failed: {d.get('terminal_reason')} {str(d.get('result'))[:200]}")
    u = d.get("usage", {})
    return d["result"], dict(prompt_tokens=u.get("input_tokens", 0) + u.get("cache_read_input_tokens", 0) +
                             u.get("cache_creation_input_tokens", 0), completion_tokens=u.get("output_tokens", 0),
                             backend="claude", model=model)


CODEX_CWD = os.path.expanduser("~/.cache/csf-re-codex")
CODEX_EFFORT = {"high": os.environ.get("CSF_CODEX_EFFORT", "high"), "max": "xhigh"}  # env: A/B low/medium


# Codex agent features that add tool schemas / instructions to every request; grunts need none of them.
CODEX_OFF = ["shell_tool", "unified_exec", "view_image", "multi_agent", "apps", "plugins", "browser_use",
             "browser_use_external", "computer_use", "image_generation", "goals", "sleep_tool", "tool_suggest",
             "skill_search", "hooks", "in_app_browser", "mentions_v2", "remote_plugin", "workspace_dependencies",
             "worktrees"]


CODEX_URL = "https://chatgpt.com/backend-api/codex/responses"
CODEX_AUTH = os.path.expanduser("~/.codex/auth.json")


def _codex_token():
    """ChatGPT-subscription bearer token the Codex CLI keeps in ~/.codex/auth.json (valid ~10 days). Near expiry,
    one trivial CLI call makes the CLI refresh it."""
    import base64
    t = json.load(open(CODEX_AUTH))["tokens"]
    p = t["access_token"].split(".")[1]
    exp = json.loads(base64.urlsafe_b64decode(p + "=" * (-len(p) % 4)))["exp"]
    if exp - time.time() < 3600:
        subprocess.run(["codex", "exec", "--ephemeral", "--skip-git-repo-check", "-s", "read-only", "-"],
                       input="Reply OK.", capture_output=True, text=True, cwd=CODEX_CWD, timeout=300)
        t = json.load(open(CODEX_AUTH))["tokens"]
    return t["access_token"], t["account_id"]


def chat_codex_direct(model, messages, effort="high"):
    """GPT via the ChatGPT Codex backend directly (same subscription and endpoint as the Codex CLI, no CLI harness:
    ~1.4k instead of ~5.7k fixed tokens per call). A fixed session id per system prompt makes the backend cache
    the shared prefix reliably (the CLI's per-run session id hit the cache about half the time)."""
    token, account = _codex_token()
    sysp = messages[0]["content"]
    sid = "csf-grunt-" + hashlib.sha1(sysp.encode()).hexdigest()[:12]
    items = [{"type": "message", "role": m["role"],
              "content": [{"type": "input_text" if m["role"] == "user" else "output_text", "text": m["content"]}]}
             for m in messages[1:]]
    body = {"model": model, "store": False, "stream": True, "instructions": sysp, "input": items,
            "reasoning": {"effort": CODEX_EFFORT.get(effort, effort)}, "text": {"verbosity": "low"},
            "prompt_cache_key": sid}
    hdr = {"Authorization": f"Bearer {token}", "chatgpt-account-id": account, "OpenAI-Beta": "responses=experimental",
           "originator": "codex_cli_rs", "accept": "text/event-stream", "content-type": "application/json",
           "session_id": sid}
    req = urllib.request.Request(CODEX_URL, data=json.dumps(body).encode(), headers=hdr, method="POST")
    try:
        r = urllib.request.urlopen(req, timeout=1800)
    except urllib.error.HTTPError as e:
        msg = e.read()[:300]
        if e.code == 429 and b"usage_limit" in msg:
            CODEX_PAUSED.set()  # the 5h/7d window is spent: defer (not escalate) until the quota guard resumes it
        raise RuntimeError(f"codex backend HTTP {e.code}: {msg!r}")
    out, u = [], None
    for raw in r:
        line = raw.decode("utf-8", "replace").strip()
        if not line.startswith("data:"):
            continue
        ev = json.loads(line[5:])
        if ev.get("type") == "response.output_text.delta":
            out.append(ev["delta"])
        elif ev.get("type") == "response.completed":
            u = ev["response"].get("usage") or {}
        elif ev.get("type") in ("response.failed", "error"):
            raise RuntimeError(f"codex backend: {json.dumps(ev)[:300]}")
    if u is None:
        raise RuntimeError("codex backend: stream ended without response.completed")
    return "".join(out), dict(prompt_tokens=u.get("input_tokens", 0),
                              cached_tokens=(u.get("input_tokens_details") or {}).get("cached_tokens", 0),
                              completion_tokens=u.get("output_tokens", 0), backend="codex", model=model)


def chat_codex(model, messages, effort="high"):
    if os.environ.get("CSF_CODEX_TRANSPORT", "direct") != "direct":
        return chat_codex_cli(model, messages, effort)
    for i in range(3):  # transport hiccups (dropped stream, 5xx, timeout) are retried, not counted as an attempt
        try:
            return chat_codex_direct(model, messages, effort)
        except Exception as e:
            s = str(e)
            transient = ("stream ended" in s or re.search(r"HTTP 5\d\d", s) or "timed out" in s
                         or isinstance(e, (urllib.error.URLError, ConnectionError, TimeoutError)))
            if i == 2 or not transient or CODEX_PAUSED.is_set():
                raise
            with _live_lock:
                c = _live["counts"]
                c["transport_retry"] = c.get("transport_retry", 0) + 1
            time.sleep(5 * (i + 1))


def chat_codex_cli(model, messages, effort="high"):
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
    feats += ["-c", f'model_instructions_file="{instr}"', "-c", "web_search=disabled",
              "-c", "include_environment_context=false", "-c", "include_permissions_instructions=false"]  # ~180 tokens
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


CODEX_RESERVED = threading.Event()  # Codex 5h window past --codex-reserve: S/M Codex rungs go to CODEX_SUB
CODEX_SUB = None  # (backend, model) standing in for Codex in S/M while reserved or paused, e.g. claude:...@medium
CLAUDE_PAUSED = threading.Event()  # set by the lanes quota guard: the grunts' Claude account is near its limit
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
    if "@" in model:  # a pinned effort for every attempt of this rung: "claude-haiku-5-5@low", "gpt-6.1-sol@xhigh"
        model, effort = model.split("@", 1)
    if backend == "codex" and CODEX_PAUSED.is_set():
        raise RuntimeError("Codex paused by the quota guard")
    if backend == "claude" and CLAUDE_PAUSED.is_set():
        raise RuntimeError("Claude paused by the quota guard")
    t0 = time.time()
    shown = CODEX_EFFORT.get(effort, effort) if backend == "codex" else effort  # what the backend really gets
    try:
        text, usage = _ask(backend, model, messages, key, effort)
    except Exception as e:
        log_call(backend, model, shown, messages, None, None, time.time() - t0, error=str(e))
        raise
    log_call(backend, model, shown, messages, text, usage, time.time() - t0)
    return text, usage


def _ask(backend, model, messages, key, effort="high"):
    cid = f"{threading.get_ident()}"
    with _live_lock:
        sysmsg = messages[0]["content"]
        # write = first attempt; then work() says what the call is: retry (same rung, gate errors attached),
        # escalate (first call of a later rung) or evidence (answering the model's own evidence requests)
        kind = ("review" if sysmsg is REVIEW_SYSTEM else "batch" if sysmsg is SYSTEM_BATCH
                else "write" if len(messages) <= 2 else getattr(_tl, "step", "retry"))
        _live["calls"][cid] = dict(ea=getattr(_tl, "ea", 0), backend=backend, model=model,
                                   effort=CODEX_EFFORT.get(effort, effort) if backend == "codex" else effort,
                                   kind=kind, n=len(getattr(_tl, "batch", ())) if kind == "batch" else 1,
                                   transport=os.environ.get("CSF_CODEX_TRANSPORT", "direct")
                                   if backend == "codex" else "api", since=time.time())
    live()
    try:
        if backend == "claude":
            text, usage = chat_claude(model, messages, effort)
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


def normalize(code):
    """Mechanical fixes applied before the gate (each used to cost a full retry call): the HOOK line's missing
    semicolon; #include "globals.h" when CSF_* macros are used without it (132 of ~220 retries, 2026-10-07)."""
    code = re.sub(r"^(\s*HOOK(?:_METHOD)?\(.*\))\s*$", r"\1;", code, flags=re.M)
    hr = {"_DWORD": "std::uint32_t", "_WORD": "std::uint16_t", "_BYTE": "std::uint8_t", "_QWORD": "std::uint64_t"}
    if re.search(r"\b_(DWORD|WORD|BYTE|QWORD)\b", code):  # Hex-Rays types copied from the pseudocode
        code = re.sub(r"\b_(DWORD|WORD|BYTE|QWORD)\b", lambda m: hr[m.group(0)], code)
        if "<cstdint>" not in code:
            code = "#include <cstdint>\n" + code
    if re.search(r"\bCSF_(GLOBAL|FUNC|AT|STR)\b", code) and not re.search(r'#\s*include\s*"globals.h"', code):
        incs = list(re.finditer(r'^\s*#\s*include[^\n]*\n', code, re.M))
        at = incs[-1].end() if incs else 0
        code = code[:at] + '#include "globals.h"\n' + code[at:]
    return code


def autofix(code):
    """normalize(), counted for the monitor (each fix is a retry call saved)."""
    fixed = normalize(code)
    if fixed != code:
        with _live_lock:
            c = _live["counts"]
            c["autofix"] = c.get("autofix", 0) + 1
    return fixed


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


REVIEW_EFFORT = "medium"  # lanes --review-effort; reviewing is easier than writing (was high until 2026-10-08)


def review(ea, packet, code):
    """Second opinion by Codex on a gate-passed file. Returns '' if fine, the issues text, or None if not run."""
    msgs = [{"role": "system", "content": REVIEW_SYSTEM},
            {"role": "user", "content": f"{packet}\n\n## Reimplementation under review\n```cpp\n{code}\n```"}]
    try:
        text, usage = ask("codex", "gpt-6.1-sol", msgs, None, REVIEW_EFFORT)
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


def claude_grunts_ready():
    """The grunts' Claude account is logged in (never fall back onto the main account the captain uses)."""
    return os.path.exists(os.path.join(CLAUDE_GRUNTS, ".credentials.json"))


def substitute(backend, model, tier):
    """The rung that really runs: Codex is kept for L once its 5h window is past the reserve."""
    if backend == "codex" and tier in ("S", "M") and CODEX_SUB and (CODEX_SUB[0] != "claude" or claude_grunts_ready()) and (CODEX_RESERVED.is_set() or CODEX_PAUSED.is_set()):
        return CODEX_SUB
    return backend, model


def work(ea, db, key, ladder, out_dir=None, example=None, review_over=None, evidence_budget=0, prior=None,
         tier=None):
    """ladder: [(backend, model, attempts), ...] tried in order; the conversation (with gate errors) carries over.
    example: ea of a done clone sibling (its file is shown). review_over: Codex-review DeepSeek outputs above N bytes.
    evidence_budget: read-only evidence requests the model may make (tools/evidence.py); they aren't attempts.
    prior: (answer text, gate error, usage) of this function's first attempt inside a batch (work_batch): the
    conversation continues from it and that attempt counts as the ladder's first step."""
    _tl.ea = ea
    packet = build(db, ea) + (example_section(db, example) if example else "")
    ev_left, n_ev = evidence_budget, 0
    if evidence_budget:
        packet += "\n" + evidence.PROMPT % evidence_budget
    msgs = [{"role": "system", "content": SYSTEM}, {"role": "user", "content": packet}]
    path = os.path.join(out_dir or os.path.join(ROOT, "src", "auto"), f"{ea:08x}.cpp")
    os.makedirs(os.path.dirname(path), exist_ok=True)
    tmp = os.path.join(out_dir, "tmp", f"{ea:08x}.cpp") if out_dir else \
        os.path.join(ROOT, "work", "grunt", ladder[0][0], f"{ea:08x}.cpp")  # A/B arms run the same ea in parallel
    os.makedirs(os.path.dirname(tmp), exist_ok=True)
    usage_all, err, meta, text = [], "", {}, ""
    steps = [(b, m) for b, m, n in ladder for _ in range(n)]
    dead, reviews = set(), []  # dead: rungs that errored (backend, or go:model); reviews: True = reviewer rejected
    quota_hit = False  # a rung was skipped for quota (Codex / Claude paused, limit hit): defer instead of escalating
    start = 0
    if prior:
        text, err, u = prior
        usage_all.append(u)
        msgs += [{"role": "assistant", "content": text},
                 {"role": "user", "content": f"The gate rejected the file:\n{err}\nFix it and answer again in the same format."}]
        start = 1
    for attempt, (backend, model) in enumerate(steps):
        if attempt < start:
            continue
        # first attempt per rung at high effort, retries at max (a pinned model@effort ignores both)
        first = attempt == 0 or steps[attempt - 1] != (backend, model)
        backend, model = substitute(backend, model, tier)
        dk = f"{backend}:{model.split('@')[0]}" if backend == "claude" else backend
        if dk in dead:
            continue
        try:
            _tl.step = "escalate" if first and attempt else "retry"
            text, usage = ask(backend, model, msgs, key, "high" if first else "max")
            usage_all.append(usage)
            code, meta = parse(text)
            while not code and isinstance(meta.get("actions"), list) and meta["actions"] and ev_left > 0:
                _tl.step = "evidence"
                reply, used = evidence.answer(db, meta["actions"], ev_left)
                ev_left, n_ev = ev_left - used, n_ev + used
                msgs += [{"role": "assistant", "content": text}, {"role": "user", "content": reply}]
                text, usage = ask(backend, model, msgs, key, "high" if first else "max")
                usage_all.append(usage)
                code, meta = parse(text)
        except Exception as e:  # out of credit / quota / CLI failure: fall through to the next rung
            dead.add(dk)
            quota_hit |= bool(re.search(r"paused by the quota guard|usage limit|rate limit|hit your limit", str(e), re.I))
            err = f"{backend} error: {e}"
            continue
        if code:
            code = autofix(code)
        if not code:
            ok, err = False, "no ```cpp block found in your answer"
        else:
            open(tmp, "w").write(code)
            ok, err = gate(ea, tmp, db)
        if ok and backend in ("deepseek", "claude") and review_over is not None and review_over >= 0 \
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
    if quota_hit:  # not a real failure: a rung was out of quota — retry in a later run
        return dict(ea=ea, ok=False, deferred=True, attempts=len(usage_all), err=err[:500], usage=usage_all,
                    reviews=reviews)
    qdir = os.path.join(out_dir, "queue") if out_dir else os.path.join(ROOT, "queue")
    os.makedirs(qdir, exist_ok=True)
    with open(os.path.join(qdir, f"{ea:08x}.md"), "w") as f:
        f.write(f"# Escalation 0x{ea:08X}\n\n## Last gate error\n```\n{err}\n```\n\n## Last answer\n{text}\n\n"
                f"## Packet\n{packet}\n")
    return dict(ea=ea, ok=False, attempts=len(steps), err=err[:500], usage=usage_all, reviews=reviews, evidence=n_ev)


# Batches get their own system prompt: as the real top-level instructions (direct transport) the single-file output
# rule overrides a batch note in the user message (the model answered only the first function).
SYSTEM_BATCH = SYSTEM.replace(
    "Output exactly two fenced blocks: first ```cpp (the whole file), then ```json (metadata). Nothing else.",
    "A request holds one or more functions. For EACH function, in the order given, output a line `### 0xADDR`, then\n"
    "its own ```cpp block (one whole, separate file), then its own ```json block (metadata). Nothing else. Answer\n"
    "every function; the file rules below apply to each file on its own.").replace(
    "Exactly one HOOK line, for the target:", "Exactly one HOOK line per file, for that file's target:")
assert SYSTEM_BATCH != SYSTEM

BATCH_NOTE = """This request holds {n} functions, each under its own "# FUNCTION 0x..." heading. Treat each one
exactly as if it came alone (same rules, one separate file each). Answer for every function, in the order given:
a line `### 0xADDR` (the function's address), then its ```cpp block, then its ```json block. Nothing else."""


def work_batch(eas, db, key, model, out_dir=None, review_over=None, ev=None):
    """Several small functions in one Codex call (the per-call harness + system prompt, ~5.8k tokens, is paid once).
    Each file is gated alone; a failed or missing one continues as a single-function conversation (work(prior=...))
    from its own part of the answer, with its evidence budget (ev: ea -> requests). Returns a list of work()
    results; batch usage is split evenly."""
    ev = ev or {}
    _tl.ea, _tl.batch = eas[0], eas
    packets = {ea: build(db, ea) for ea in eas}
    body = "\n\n".join(f"# FUNCTION 0x{ea:08X}\n\n{packets[ea]}" for ea in eas)
    msgs = [{"role": "system", "content": SYSTEM_BATCH},
            {"role": "user", "content": BATCH_NOTE.format(n=len(eas)) + "\n\n" + body}]
    ladder = [("codex", model, 2)]
    try:
        text, usage = ask("codex", model, msgs, key, "high")
    except Exception as e:
        _tl.batch = ()
        if CODEX_PAUSED.is_set():
            return [dict(ea=ea, ok=False, deferred=True, attempts=0, err=f"codex error: {e}", usage=[]) for ea in eas]
        return [work(ea, db, key, ladder, out_dir, None, review_over, ev.get(ea, 0)) for ea in eas]
    _tl.batch = ()
    share = {k: (v // len(eas) if isinstance(v, int) else v) for k, v in usage.items()}
    share["batch"] = len(eas)
    parts = {}
    for m in re.finditer(r"^###\s*0x([0-9A-Fa-f]+)\s*$(.*?)(?=^###\s*0x|\Z)", text, re.M | re.S):
        parts[int(m.group(1), 16)] = m.group(2)
    # by the HOOK address inside each file (the model sometimes drops the ### markers): cpp block + following json
    for m in re.finditer(r"```(?:cpp|c\+\+)\s*\n.*?```(?:\s*```json\s*\n.*?```)?", text, re.S):
        h = re.search(r"HOOK(?:_METHOD)?\(\s*0x([0-9A-Fa-f]+)", m.group(0))
        if h and int(h.group(1), 16) in packets and int(h.group(1), 16) not in parts:
            parts[int(h.group(1), 16)] = m.group(0)
    out = []
    for ea in eas:
        _tl.ea = ea
        part = parts.get(ea, "")
        code, meta = parse(part)
        if code:
            code = autofix(code)
            tmp = os.path.join(ROOT, "work", "grunt", "codex", f"{ea:08x}.cpp")
            os.makedirs(os.path.dirname(tmp), exist_ok=True)
            open(tmp, "w").write(code)
            ok, err = gate(ea, tmp, db)
        else:
            ok, err = False, "no ```cpp block found in your answer"
        if ok:
            path = os.path.join(out_dir or os.path.join(ROOT, "src", "auto"), f"{ea:08x}.cpp")
            os.makedirs(os.path.dirname(path), exist_ok=True)
            os.replace(tmp, path)
            out.append(dict(ea=ea, ok=True, attempts=1, meta=meta, usage=[share], by=f"codex:{model}", example=None,
                            reviews=[], evidence=0, batch=len(eas)))
        else:
            r = work(ea, db, key, ladder, out_dir, None, review_over, ev.get(ea, 0),
                     prior=(part.strip() or "(no answer for this function)", err, share))
            r["batch"] = len(eas)
            out.append(r)
    return out


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


def module_ids(spec):
    """--module: comma list of module ids (m4F9190) or names (dev_console) from db/modules.yaml -> set of ids."""
    if not spec:
        return None
    import modules
    mods = modules.read_yaml()
    out = set()
    for k in spec.split(","):
        m = next((m for m in mods if k in (m["id"], m.get("name"))), None)
        if not m:
            sys.exit(f"unknown module {k!r} (modules.py list)")
        out.add(m["id"])
    return out


def select(db, n, max_size, min_size=6, exclude=(), modules=None):
    """Leaf-first: fewest not-done game callees, then smallest. Returns [(ea, example_ea|None)].
    modules: only functions of these module ids (a module wave).
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
        if modules is not None and r["module"] not in modules:
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
SMOKE_SHORT = (SMOKE_SECS, {})  # lanes --smoke-menu: short smokes run the menu tour instead (GUI waves)
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
        env = SMOKE_STARTS[(_smokes // SMOKE_LONG_EVERY) % len(SMOKE_STARTS)] if long else SMOKE_SHORT[1]
        _last_smoke = (secs or (SMOKE_LONG_SECS if long else SMOKE_SHORT[0]), env)
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


def build_culprits(out, eas):
    """Files of this batch named by a failed build: compile errors (src/auto/<ea>.cpp) and unresolved symbols
    (the batch file that uses the symbol; a gate compile is per file, so link errors only show up here)."""
    bad = {int(m.group(1), 16) for m in re.finditer(r"src[/\\]auto[/\\]([0-9a-f]{8})\.cpp[^\n]*?\berror\b", out)}
    for sym in set(re.findall(r"undefined symbol: (\S+)", out)):
        plain = re.sub(r"^[_@?]|@\d+$", "", sym)
        for ea in eas:
            p = os.path.join(ROOT, "src", "auto", f"{ea:08x}.cpp")
            if os.path.exists(p) and re.search(rf"\b{re.escape(plain)}\b", open(p).read()):
                bad.add(ea)
    return sorted(bad & set(eas))


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


def run_config(args, lanes=False):
    """The cost/speed settings of this run, for the monitor."""
    return dict(transport=os.environ.get("CSF_CODEX_TRANSPORT", "direct"), effort=CODEX_EFFORT["high"],
                retry_effort=CODEX_EFFORT["max"], class_view=os.environ.get("CSF_CLASS_VIEW", "compact"),
                batch_fns=getattr(args, "batch_fns", 1), batch_bytes=getattr(args, "batch_bytes", 0),
                batch_max=getattr(args, "batch_max", 300), tiny=getattr(args, "tiny", None),
                review_over=args.review_over, codex_cap=getattr(args, "codex_cap", None),
                codex_reserve=getattr(args, "codex_reserve", None), claude_cap=getattr(args, "claude_cap", None), codex_sub=getattr(args, "codex_sub", None),
                backend=getattr(args, "backend", "lanes"), wave=getattr(args, "module", None),
                **(dict(tiers={t: dict(ladder=getattr(args, f"{t.lower()}_ladder"), jobs=getattr(args, f"{t.lower()}_jobs"))
                               for t in "SML"}, s_max=args.s_max, m_max=args.m_max, review_effort=args.review_effort) if lanes else {}))


def pack(db, eas, fns, budget=0, max_size=300):
    """Codex batches: same class (else module) together, at most `fns` functions and, with a byte budget, at most
    `budget` bytes of original code per call (many tiny functions share a call, mid-size ones get 2-3). Functions over
    `max_size` go alone."""
    groups = collections.defaultdict(list)
    for ea in sorted(eas):
        groups[db.class_of(ea) or db.rows[ea]["module"]].append(ea)
    out = []
    for g in groups.values():
        cur, nb = [], 0
        for ea in g:
            sz = db.F[ea]["size"]
            if sz > max_size:
                out.append([ea])
                continue
            if cur and (len(cur) >= fns or (budget and nb + sz > budget)):
                out.append(cur)
                cur, nb = [], 0
            cur.append(ea)
            nb += sz
        if cur:
            out.append(cur)
    return out


def run(args):
    db = DB()
    key = load_key()
    picks = [(int(x, 16), None) for x in args.only.split(",")] if args.only else select(db, args.n, args.max_size, args.min_size, modules=module_ids(args.module))
    targets = [ea for ea, _ in picks]
    example = dict(picks)
    live(mode="run" + (" (dry)" if args.dry else ""), phase="grunting", config=run_config(args),
         counts=dict(total=len(targets), submitted=len(targets)))
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
        out_dir = os.path.join(ROOT, "work", "ab", args.backend + (f"-{os.environ['CSF_CODEX_EFFORT']}"
                               if os.environ.get("CSF_CODEX_EFFORT") else "")) if args.dry else None
        if args.batch_fns > 1 and args.backend in ("codex", "auto"):  # several functions per Codex call
            solo = [ea for ea in targets if args.backend == "auto" and db.F[ea]["size"] < args.tiny]
            chunks = pack(db, [ea for ea in targets if ea not in solo], args.batch_fns, args.batch_bytes,
                          args.batch_max)
            futs = {ex.submit(work_batch, ch, db, key, args.codex_model, out_dir, args.review_over,
                              {e: ev_budget(args, db, e) for e in ch}) if len(ch) > 1
                    else ex.submit(work, ch[0], db, key, codex + esc, out_dir, None, args.review_over,
                                   ev_budget(args, db, ch[0])): ch[0] for ch in chunks}
            futs.update({ex.submit(work, ea, db, key, ladder_of(ea), out_dir, example.get(ea), args.review_over,
                                   ev_budget(args, db, ea)): ea for ea in solo})
        else:
            futs = {ex.submit(work, ea, db, key, ladder_of(ea), out_dir, example.get(ea), args.review_over,
                              ev_budget(args, db, ea)): ea
                    for ea in targets}
        for fu in cf.as_completed(futs):
            try:
                rs = fu.result()
            except Exception as e:
                rs = dict(ea=futs[fu], ok=False, err=f"exception: {e}", usage=[])
            for r in (rs if isinstance(rs, list) else [rs]):
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


TIERS = ("S", "M", "L")


def ladder_spec(spec):
    """"claude:claude-haiku-5-5@low:2,codex:gpt-6.1-sol:1" -> [("claude", "claude-haiku-5-5@low", 2), ("codex", ...)]."""
    out = []
    for part in spec.split(","):
        backend, model, n = part.strip().split(":")
        out.append((backend, model, int(n)))
    return out


def lanes(args):
    """Continuous grunting in one lane. Each function's size picks its tier and the tier its ladder of models (tried
    in order, the conversation with the gate errors carried over): S = under --s-max bytes, or a clone sibling with a
    done example up to --m-max; M = up to --m-max; L = the rest. Each tier has its own in-flight cap, so slow L calls
    never hold up the S stream. Quota: Codex pauses at --codex-cap of its 5h window (L gets no new work, S/M lose their
    Codex rung); the grunts' Claude account pauses at --claude-cap of its 5h or weekly window. A function whose only
    remaining rungs were out of quota is deferred, not escalated. Every --batch gate passes: build + smoke + commit."""
    if args.module and not args.no_captain:
        captain_prep(args.module)
    global REVIEW_EFFORT, CODEX_SUB
    REVIEW_EFFORT = args.review_effort
    CODEX_SUB = tuple(args.codex_sub.split(":", 1)) if args.codex_sub else None
    db = DB()
    wave = module_ids(args.module)
    if args.smoke_menu:
        global SMOKE_SHORT
        SMOKE_SHORT = (40, SMOKE_STARTS[1])  # the tour opens ~25 menus at 1.5 s each
    key = None
    esc = [("claude", args.claude_model, 2)] if args.escalate_claude else []
    ladders = {"S": ladder_spec(args.s_ladder) + esc, "M": ladder_spec(args.m_ladder) + esc,
               "L": ladder_spec(args.l_ladder) + esc}
    if any(b == "deepseek" for lad in ladders.values() for b, _, _ in lad):
        key = load_key()
    caps = {"S": args.s_jobs, "M": args.m_jobs, "L": args.l_jobs}
    pool = cf.ThreadPoolExecutor(sum(caps.values()))
    inflight, good, submitted, total_ok, total_esc, total_def = {}, [], 0, 0, 0, 0
    stopping, quota_t = False, 0
    quarantined = set()  # in-flight functions quarantined by a batch build (their result is then an escalation)
    if os.path.exists(STOP):
        os.remove(STOP)
    live(mode="lanes", phase="grunting", config=run_config(args, lanes=True),
         counts=dict(total=args.total, batch=args.batch, ok=0, esc=0, pending=0, submitted=0, committed=0))

    def tier_of(ea, ex):
        size = db.F[ea]["size"]
        if size < args.s_max or (ex is not None and size <= args.m_max):
            return "S"
        return "M" if size <= args.m_max else "L"

    def codex_only(t):
        return all(b == "codex" for b, _, _ in ladders[t])

    def quota():
        """Pause/resume Codex on its 5h window and the grunts' Claude account on its 5h/7d windows (every 2 min)."""
        nonlocal quota_t
        if time.time() - quota_t < 120:
            return
        quota_t = time.time()
        if args.claude_cap > 0 and claude_grunts_ready():
            try:
                u = claude_usage("grunts")
                pct = max(v[0] for k, v in u.items() if k != "_age")
                if pct >= args.claude_cap and not CLAUDE_PAUSED.is_set():
                    CLAUDE_PAUSED.set()
                    print(f"grunts' Claude account at {pct:.0f}% (cap {args.claude_cap}): Claude paused", flush=True)
                elif pct < args.claude_cap and CLAUDE_PAUSED.is_set():
                    CLAUDE_PAUSED.clear()
                    print(f"grunts' Claude account at {pct:.0f}%: Claude resumed", flush=True)
                live(claude_paused=CLAUDE_PAUSED.is_set(), claude_pct=pct)
            except Exception:
                pass  # endpoint down: keep the current state
        if args.codex_cap <= 0:
            return
        try:
            pct, reset = codex_usage()["5h"]
        except Exception:
            return  # endpoint down: keep the current state
        if CODEX_SUB and pct >= args.codex_reserve and not CODEX_RESERVED.is_set():
            CODEX_RESERVED.set()
            print(f"Codex 5h window at {pct:.0f}% (reserve {args.codex_reserve}): S/M Codex rungs -> "
                  f"{':'.join(CODEX_SUB)}, Codex kept for L", flush=True)
        elif pct < args.codex_reserve and CODEX_RESERVED.is_set():
            CODEX_RESERVED.clear()
            print(f"Codex 5h window at {pct:.0f}%: S/M Codex rungs back on Codex", flush=True)
        if pct >= args.codex_cap and not CODEX_PAUSED.is_set():
            CODEX_PAUSED.set()
            print(f"Codex 5h window at {pct:.0f}% (cap {args.codex_cap}): Codex paused, resets in {reset // 60} min",
                  flush=True)
        elif pct < args.codex_cap and CODEX_PAUSED.is_set():
            CODEX_PAUSED.clear()
            print(f"Codex 5h window at {pct:.0f}%: Codex resumed", flush=True)
        live(codex_paused=CODEX_PAUSED.is_set(), codex_reserved=CODEX_RESERVED.is_set(), codex_5h=pct)

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
        cnt = collections.Counter(t for _, t in inflight.values())
        for t in TIERS:  # a tier that can only use Codex waits while Codex is paused
            if CODEX_PAUSED.is_set() and codex_only(t):
                cnt[t] = caps[t]
        if submitted >= args.total or all(cnt[t] >= caps[t] for t in TIERS):
            return
        # wide window: the leaf-first head is almost all small (S); M and L need larger candidates
        for ea, ex in select(db, 4000, args.max_size, args.min_size, exclude=busy, modules=wave):
            if submitted >= args.total or all(cnt[t] >= caps[t] for t in TIERS):
                break
            t = tier_of(ea, ex)
            if cnt[t] >= caps[t]:
                continue
            fut = pool.submit(work, ea, db, key, ladders[t], None, ex, args.review_over, ev_budget(args, db, ea),
                              tier=t)
            inflight[fut] = (ea, t)
            cnt[t] += 1
            submitted += 1
        live(counts=dict(submitted=submitted), inflight_tiers=dict(cnt))

    def flush():
        nonlocal good, total_ok, total_esc
        if not good:
            return True
        live(phase="smoke")
        if not args.no_smoke:
            ok, out = smoke()
            errs = [l for l in out.splitlines() if re.search(r"\berror\b|FAILED|undefined symbol", l)][:20]
            print("\n".join(errs + [out[-1200:]]), flush=True)
            if not ok and out.startswith("build/deploy failed"):
                # the build sees every file in src/auto: also those of in-flight functions that passed the gate
                # since the batch was cut (not logged yet) — a culprit can be one of them
                pending = [ea for ea, _ in inflight.values()]
                culprits = build_culprits(out, [r["ea"] for r in good] + pending)
                if culprits:  # compile/link error in this batch's own files (the gate compiles each one alone)
                    print(f"build failed: culprits {' '.join(f'{c:x}' for c in culprits)}", flush=True)
                    for c in culprits:
                        quarantine(c, "build/link failure in the lanes batch build", out)
                        db.rows[c]["status"] = "todo"
                    quarantined.update(c for c in culprits if c in pending)
                    culprits = [c for c in culprits if c not in pending]
                    good = [r for r in good if r["ea"] not in culprits]
                    total_ok, total_esc = total_ok - len(culprits), total_esc + len(culprits)
                    live(counts=dict(ok=total_ok, esc=total_esc, pending=len(good)))
                    return flush()
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

    def handle(r, tier):
        nonlocal total_ok, total_esc, total_def
        ea = r["ea"]
        if r["ok"] and ea in quarantined:  # its file broke a batch build while it was in flight
            r = dict(r, ok=False, err="quarantined: broke the batch build while in flight")
        with open(LOG, "a") as f:
            f.write(json.dumps(dict(r, ts=time.time(), tier=tier, size=db.F[ea]["size"])) + "\n")
        if r["ok"]:
            db.rows[ea]["status"] = "done"  # in memory: siblings may now use it as their example
            good.append(r)
            total_ok += 1
        elif r.get("deferred"):
            db.rows[ea]["status"] = "deferred"  # in memory only: not re-picked this run
            total_def += 1
        else:
            total_esc += 1
        live(counts=dict(ok=total_ok, esc=total_esc, deferred=total_def, pending=len(good)))
        print(f"  [{tier}] {ea:08x} {'OK ' if r['ok'] else 'DEF' if r.get('deferred') else 'ESC'} "
              f"{(r.get('by') or '').split(':')[-1] + ' ' if r['ok'] else ''}"
              f"{r.get('meta', {}).get('name', '') if r['ok'] else r.get('err', '')[:90]}"
              f"{'  (example %08x)' % r['example'] if r.get('example') else ''}", flush=True)

    refill()
    while inflight:
        done, _ = cf.wait(list(inflight), timeout=60, return_when=cf.FIRST_COMPLETED)
        for fu in done:
            ea, tier = inflight.pop(fu)
            try:
                r = fu.result()
            except Exception as e:
                r = dict(ea=ea, ok=False, err=f"exception: {e}", usage=[])
            handle(r, tier)
        if len(good) >= args.batch and not flush():
            pool.shutdown(wait=True, cancel_futures=True)
            return
        refill()
    if flush():
        live(phase="stopped" if stopping else "finished")
        if wave and not args.no_captain and total_ok:
            captain_after(wave)
    print(f"lanes done: {total_ok} ok, {total_esc} escalated, {total_def} deferred", flush=True)


def captain(*argv):
    """tools/captain.py as a step of a module wave (it holds its own one-instance lock; busy = skipped)."""
    r = subprocess.run([sys.executable, os.path.join(ROOT, "tools", "captain.py"), *argv],
                       capture_output=True, text=True)
    out = (r.stdout + r.stderr).strip()
    print("\n".join("  [captain] " + l for l in out.splitlines()[-20:]), flush=True)
    return r.returncode == 0


def captain_prep(module_arg):
    """Wave prep: name the wave's unnamed classes (agentic captain session), then the rename pass so this wave's
    packets and the files already done use the names. Runs only when classes.yaml changed."""
    names = [m.strip() for m in module_arg.split(",") if m.strip()]
    yml = os.path.join(ROOT, "db", "types", "classes.yaml")
    before = open(yml).read()
    live(mode="lanes", phase="captain: naming classes")
    print(f"wave prep: captain family {module_arg}", flush=True)
    captain("family", *names)
    if open(yml).read() == before:
        return
    subprocess.run([sys.executable, os.path.join(ROOT, "tools", "rename_classes.py")], check=False)
    if subprocess.run(["ninja", "-C", os.path.join(ROOT, "build")], capture_output=True).returncode:
        print("wave prep: build failed after the class rename pass: reverting it", flush=True)
        subprocess.run(["git", "-C", ROOT, "checkout", "--", "src", "db/functions.csv"], check=False)
        return
    subprocess.run(["git", "-C", ROOT, "add", "-A", "--", "src", "db/functions.csv"], check=False)
    subprocess.run(["git", "-C", ROOT, "commit", "-q", "-m", "Class rename pass (wave prep: captain family)\n\n"
                    "Co-Authored-By: Claude Opus 5.5 <noreply@anthropic.com>", "--", "src", "db/functions.csv"],
                   check=False)


def captain_after(wave):
    """After the wave: placeholder function names (GetField0050, ...) of its modules, now that callers are named."""
    print("wave end: captain functions", flush=True)
    live(phase="captain: placeholder names")
    captain("functions", "--module", ",".join(sorted(wave)))


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
    r.add_argument("--module", help="module wave: comma list of module ids/names (tools/modules.py list)")
    r.add_argument("--model", default="deepseek-flash")
    r.add_argument("--backend", choices=["auto", "deepseek", "codex", "claude"], default="auto")
    r.add_argument("--codex-model", default="gpt-6.1-sol")
    r.add_argument("--tiny", type=int, default=48, help="auto: functions smaller than this go to DeepSeek flash")
    r.add_argument("--claude-model", default="sonnet")
    r.add_argument("--escalate-claude", action="store_true", help="after 2 DeepSeek failures, try Claude twice")
    r.add_argument("--min-size", type=int, default=6)
    r.add_argument("--batch-fns", type=int, default=1, help="Codex: max functions per call")
    r.add_argument("--batch-bytes", type=int, default=0, help="Codex: max bytes of original code per call (0: off)")
    r.add_argument("--batch-max", type=int, default=300, help="only functions up to N bytes are batched")
    r.add_argument("--dry", action="store_true", help="write to work/ab/<backend>/, no smoke/commit/status")
    r.add_argument("--no-smoke", action="store_true")
    r.add_argument("--evidence", type=int, default=3, help="evidence requests per function (tools/evidence.py)")
    r.add_argument("--evidence-over", type=int, default=150, help="offer evidence requests above N bytes")
    r.add_argument("--review-over", type=int, default=100, help="Codex-review DeepSeek outputs above N bytes (-1: off)")
    l = sub.add_parser("lanes", help="continuous grunting: one lane, size tiers S/M/L each with its own model ladder")
    l.add_argument("--total", type=int, default=200)
    l.add_argument("--module", help="module wave: comma list of module ids/names (tools/modules.py list)")
    l.add_argument("--batch", type=int, default=60, help="smoke + commit every N gate passes")
    l.add_argument("--s-max", type=int, default=150, help="S tier: functions under N bytes (and clone siblings with "
                   "a done example up to --m-max)")
    l.add_argument("--m-max", type=int, default=400, help="M tier: functions up to N bytes; L: the rest")
    # 2026-10-08 after the first tiered menu batches: MiMo dropped (6-12 min thinks, 3 rescues in 140), no Codex xhigh
    l.add_argument("--s-ladder", default="claude:claude-haiku-5-5@low:2,claude:claude-haiku-5-5@medium:1,codex:gpt-6.1-sol@high:1",
                   help="backend:model[@effort]:attempts,...")
    l.add_argument("--m-ladder", default="claude:claude-haiku-5-5@high:2,codex:gpt-6.1-sol@high:2")
    l.add_argument("--l-ladder", default="codex:gpt-6.1-sol@high:2")
    l.add_argument("--review-effort", default="medium", help="Codex review effort")
    l.add_argument("--s-jobs", type=int, default=12, help="S tier: functions in flight")
    l.add_argument("--m-jobs", type=int, default=6)
    l.add_argument("--l-jobs", type=int, default=3)
    l.add_argument("--max-size", type=int, default=600)
    l.add_argument("--min-size", type=int, default=6)
    l.add_argument("--claude-model", default="sonnet")
    l.add_argument("--escalate-claude", action="store_true", help="append Claude x2 to every ladder")
    l.add_argument("--evidence", type=int, default=3, help="evidence requests per function (tools/evidence.py)")
    l.add_argument("--evidence-over", type=int, default=150, help="offer evidence requests above N bytes")
    l.add_argument("--review-over", type=int, default=150, help="Codex reviews non-Codex answers above N bytes "
                   "(-1: off)")
    l.add_argument("--no-smoke", action="store_true")
    l.add_argument("--no-bisect", action="store_true", help="stop on a smoke failure instead of bisecting it")
    l.add_argument("--smoke-menu", action="store_true", help="short smokes run the menu tour (GUI module waves)")
    l.add_argument("--no-captain", action="store_true", help="module waves: skip the captain steps (class naming "
                   "before, placeholder function names after)")
    l.add_argument("--codex-cap", type=float, default=90, help="pause Codex at this 5h-window %% (0: off)")
    l.add_argument("--claude-cap", type=float, default=95,
                   help="pause the grunts' Claude account at this %% of its 5h or 7d window (0: off)")
    l.add_argument("--codex-reserve", type=float, default=50,
                   help="from this Codex 5h-window %%, S/M Codex rungs run on --codex-sub (Codex kept for L)")
    l.add_argument("--codex-sub", default="claude:claude-opus-5-5@medium",
                   help="backend:model[@effort] standing in for Codex in S/M (empty: off)")
    sub.add_parser("cost")
    a = ap.parse_args()
    {"run": run, "lanes": lanes, "cost": cost}[a.cmd](a)
