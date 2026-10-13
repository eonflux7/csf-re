#!/usr/bin/env python3
"""Grunt driver: packet -> model ladder -> gate -> src/auto/<ea>.cpp, or escalate to queue/.

Usage:
  grunt.py wave <module> | --next | --waves N    the loop for one module (see wave()); lanes + verifier + link test
                                                 --waves: rolling, --overlap modules open at once (see lanes())
  grunt.py lanes [--total 200] [--batch 60] [--module m1,m2]   continuous, size tiers S/M/L/X (db/lanes.yaml)
  grunt.py run --only ea,ea [--backend codex|claude] [--dry] [--no-smoke]   single functions (redo, A/B)
  grunt.py cost                      token usage, review hit rate from work/grunt_log.jsonl

Stop a lanes run cleanly: `touch work/grunt_stop` (in-flight calls finish, the batch is smoked and committed).

After a batch: link, deploy, smoke test (unless --no-smoke); on success mark functions done and git commit.
"""
import hashlib, argparse, collections, concurrent.futures as cf, glob, itertools, json, os, re, subprocess, sys, threading, time, urllib.error, urllib.request
sys.path.insert(0, os.path.dirname(__file__))
from packet import DB, build
from gate import gate
import gate as gate_mod
from triage import load_csv, save_csv
from limits import codex_usage, codex_homes, codex_label
import evidence

ROOT = os.path.dirname(os.path.dirname(os.path.abspath(__file__)))
LOG = os.path.join(ROOT, "work", "grunt_log.jsonl")
PROPOSALS = os.path.join(ROOT, "db", "proposals.jsonl")

SYSTEM = r"""You reimplement single functions of a 2006 MSVC 7.1 x86 game (Commandos: Strike Force, Pyro Studios;
identifiers/strings are Spanish) as C++20, to be compiled with clang-cl for 32-bit MSVC ABI into a hook DLL that
replaces the original function inside the running game, and later builds standalone. Goal: the same observable
behavior (calls, memory writes, return value), written as the readable C++ a programmer would have written.

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
  Use the callee's real arg count (from its prototype / ret N). When the packet gives a callee's
  "bind as" line, copy it exactly (one name and signature per address across the codebase).
- Virtual calls: when the packet lists a typed wrapper for the slot (`obj->vName(args)`), use it. Otherwise
  ((RET(__fastcall*)(void*, void*, ARGS...))obj->vtbl[SLOT])(obj, nullptr, args...) where SLOT = byte offset / 4;
  if obj's type is unknown use CSF_AT(void**, p, 0)[SLOT].
- Globals: CSF_GLOBAL(type, g_Name, 0xADDR); then use g_Name. Never write an original address as a plain number:
  a vtable is CSF_VTBL(0xADDR) (e.g. `self->vtbl = CSF_VTBL(0x830030);`), an address used as a value is
  CSF_ADDR_FN(0xADDR) (function) / CSF_ADDR_DATA(0xADDR) (data) / CSF_ADDR_VT(0xADDR) (vtable compare). The
  standalone build resolves these by address; a bare number would point into the old exe.
- String constants of the original: ALWAYS CSF_STR(0xADDR, "text") with the address from the packet, never a
  bare string literal (callers may compare pointers). Same for other constant data: bind by address.
- Numbers: write the original's literals as the pseudocode shows them (case 203, not case 3 after `x - 200`;
  0x24C bytes, not 0x93 dwords); keep consecutive calls in the original's order. The gate checks both.
- Fields: use the declared struct fields (types/auto_classes.h; never redefine those structs). For an offset
  that is not a declared field (inside a pad_ array, or no class given) use CSF_AT(type, ptr, 0xOFF).
  CSF_AT(T, p, off) IS the T stored at p+off (an lvalue): `x = CSF_AT(int, p, 0x10)`, never `*CSF_AT(int*, ...)`.
  Pointer-sized fields declared uint32_t: cast as needed, e.g. reinterpret_cast<C_xxx*>(self->field_0010).
- Mistakes that crashed the game or failed review (check yours against each):
  * `mov eax, [obj]; call [eax+N]`: obj's first dword is the VTABLE pointer, the function is vtable[N / 4]. With
    `void** obj`, `obj[0](...)` calls the vtable itself (crash: EIP inside .rdata); write CSF_AT(void**, obj, 0)[N / 4].
    Never pass the byte offset N as an index, and never wrap virtual calls in a helper taking the slot at runtime.
  * `call A; mov ecx, eax; call B`: A's result is B's `this`. Pass it; don't call A for nothing and give B nullptr.
  * `push X` before a call to a no-argument getter / a forwarder: X belongs to the NEXT call that pops it (the ret N
    in the packet decides who takes what), never to the getter.
  * A field holding a value is CSF_AT(int, p, off); `*CSF_AT(int*, p, off)` dereferences the value as a pointer.
  * A callee whose result the original tests with `test al, al` returns bool/char: bind it returning bool.
  * __ftol2 / fistp are SIGNED conversions (float -> int / int64): never go through an unsigned type.
- Fidelity is behavioral, not bit-for-bit code generation. Floating point: write the natural expression the
  source had (`(float)x / (float)g_ScreenWidth`, a plain float compare); never emulate x87 80-bit intermediates,
  rounding steps, or float bit patterns - last-bit differences are accepted by the in-game diff. Don't reproduce
  register contents, stack layout or compiler idioms (MSVC helpers like __ftol2 are a plain cast). No
  `volatile`: a load whose value the original never uses is not behavior (skip it), and an uninitialized
  stack value it passes along is garbage (pass 0).
- Preserve exact semantics: signed vs unsigned compares (check the disassembly when given: jl/jg = signed,
  jb/ja = unsigned), integer widths and truncation, float vs double, evaluation and call order, side effects.
  Do not "fix" bugs. Do not drop calls whose results are unused.
- `add ecx, N` / `lea reg, [ecx+N]` computes the ADDRESS of a sub-object embedded at +N (e.g. a member object
  or secondary base): use (uint8_t*)self + N. Only `mov reg, [ecx+N]` loads a pointer stored at +N.
- Memory reads: re-read a field/array element at every point the original reads it (check the disassembly).
  Never cache a value in a local across a call if the original reloads it after the call — callees may modify
  the object (e.g. a child removing itself from the list being iterated). Don't add checks or reads the
  original doesn't have.
- An object the original keeps on its own stack (`lea ecx, [ebp-X]` passed as `this` / an out argument): never
  declare it as a by-value local of an S_/C_ struct type (their sizes are lower bounds; callees write the real
  size). Reserve the bytes the original's frame gives it: `alignas(8) uint8_t nameBytes[N];` (N: up to the next
  local in the frame; when unsure the whole `sub esp, N`) and `T& name = *reinterpret_cast<T*>(nameBytes);`.
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
  (indirect jmp through a pointer / vtable slot), with HOOK_TAIL as the packet's "Indirect tail jump" section says
  (`hook::Tail __fastcall Fn(void* self, const uint32_t* args)` returning hook::tail_jump(obj, target)); no asm.
- Return width: `xor eax, eax` / `movzx eax, ...` before `setcc al` (or any full-eax result) means a 32-bit
  int/BOOL return; declare it `int`, never `bool` (bool leaves eax's upper bytes undefined; callers test eax).
  Only a bare `mov al, ...` / `setcc al` without zeroing eax is a genuine byte/bool return.
"""


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
            c.update(error=None, at=time.time(), u={k: (d[w]["utilization"] or 0, iso(d[w]["resets_at"]) if d[w].get("resets_at")
                                                         else time.time()) for k, w in  # idle window: no reset time
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


# Haiku 5.5 at low effort thinks ~2x longer via claude -p than it did over the API (Go) for the same request
# settings (adaptive thinking + effort low): ~5k vs ~2.6k output tokens on 40-150 B functions. Adaptive thinking
# follows the prompt: this note halves it on a median packet (7.1k -> 4.0k tokens, 30 -> 17 s), gate still passes.
CLAUDE_BRIEF = ("\n\nThinking: keep it short. Work from the packet directly; reason only about the parts that are "
                "unclear, then write the answer.")


def chat_claude(model, messages, effort=None):
    """Claude via the Claude Code CLI in headless mode, on the grunts' own account (never the captain's). No tools."""
    if not claude_grunts_ready():
        raise RuntimeError(f"grunts' Claude account not logged in: CLAUDE_CONFIG_DIR={CLAUDE_GRUNTS} claude, /login")
    os.makedirs(CLAUDE_CWD, exist_ok=True)
    env = dict(os.environ, CLAUDE_CONFIG_DIR=CLAUDE_GRUNTS)
    eff = ["--effort", effort] if effort in ("low", "medium", "high", "xhigh", "max") else []
    system = messages[0]["content"] + (CLAUDE_BRIEF if effort == "low" else "")
    convo = []
    for m in messages[1:]:
        tag = "PACKET / REQUEST" if m["role"] == "user" else "YOUR PREVIOUS ANSWER"
        convo.append(f"=== {tag} ===\n{m['content']}")
    # no tools/settings/MCP/skills: ~560 fixed tokens instead of ~4k (the account's claude.ai connectors alone ~3.4k)
    r = subprocess.run(["claude", "-p", "--tools", "", "--model", model, "--no-session-persistence",
                        "--strict-mcp-config", "--setting-sources", "", "--disable-slash-commands",
                        "--output-format", "json", "--system-prompt", system, *eff],
                       input="\n\n".join(convo), capture_output=True, text=True, cwd=CLAUDE_CWD, timeout=1800, env=env)
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
# Several ChatGPT subscriptions (limits.codex_homes): each call takes the next login that isn't paused; a login
# whose 5h window is spent (429 usage_limit, or the quota guard's cap) pauses alone, and CODEX_PAUSED is set only
# when every login is paused.
CODEX_HOME_PAUSED = set()
CODEX_REFRESH, CODEX_REFRESHED = set(), set()  # logins whose token the backend called expired: one CLI refresh each
CODEX_HOME_OUT = set()  # logins the backend refuses (token rejected, model not on the account): out for this run
_codex_rr = itertools.count()


def codex_pick():
    pin = getattr(_tl, "codex_home", None)  # a Claude stand-in pinned to its login (substitute)
    if pin and pin not in CODEX_HOME_PAUSED:
        return pin
    homes = [h for h in codex_homes() if h not in CODEX_HOME_PAUSED]
    if not homes:
        CODEX_PAUSED.set()
        raise RuntimeError("codex backend HTTP 429: every Codex login is paused (usage_limit)")
    return homes[next(_codex_rr) % len(homes)]


def codex_pause(home, paused=True):
    (CODEX_HOME_PAUSED.add if paused else CODEX_HOME_PAUSED.discard)(home)
    if all(h in CODEX_HOME_PAUSED for h in codex_homes()):
        CODEX_PAUSED.set()
    else:
        CODEX_PAUSED.clear()


def _codex_token(home):
    """ChatGPT-subscription bearer token the Codex CLI keeps in <home>/auth.json (valid ~10 days). Near expiry,
    one trivial CLI call (with CODEX_HOME=home) makes the CLI refresh it."""
    import base64
    auth = os.path.join(home, "auth.json")
    t = json.load(open(auth))["tokens"]
    p = t["access_token"].split(".")[1]
    exp = json.loads(base64.urlsafe_b64decode(p + "=" * (-len(p) % 4)))["exp"]
    if exp - time.time() < 3600 or home in CODEX_REFRESH:
        CODEX_REFRESH.discard(home)
        subprocess.run(["codex", "exec", "--ephemeral", "--skip-git-repo-check", "-s", "read-only", "-"],
                       input="Reply OK.", capture_output=True, text=True, cwd=CODEX_CWD, timeout=300,
                       env=dict(os.environ, CODEX_HOME=home))
        t = json.load(open(auth))["tokens"]
    return t["access_token"], t["account_id"]


def chat_codex_direct(model, messages, effort="high", _try=0):
    """GPT via the ChatGPT Codex backend directly (same subscription and endpoint as the Codex CLI, no CLI harness:
    ~1.4k instead of ~5.7k fixed tokens per call). A fixed session id per system prompt makes the backend cache
    the shared prefix reliably (the CLI's per-run session id hit the cache about half the time)."""
    home = codex_pick()
    token, account = _codex_token(home)
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
            codex_pause(home)  # this login's window is spent: the others carry on; all spent -> defer, not escalate
        if e.code == 401 and home not in CODEX_REFRESHED:
            # expired on the server while the token's own exp is days away (2026-10-12, after an account change):
            # the CLI refreshes it; only a second refusal takes the login out
            CODEX_REFRESHED.add(home)
            CODEX_REFRESH.add(home)
            return chat_codex_direct(model, messages, effort)
        if e.code in (401, 403) or (e.code == 400 and b"not supported" in msg):
            # the account, not the function (2026-10-12: one login lost gpt-6.1-sol; its calls were escalated to
            # queue/ and M functions fell through to Opus): the login is out, the call goes to another one
            if home not in CODEX_HOME_OUT:
                CODEX_HOME_OUT.add(home)
                print(f"{codex_label(home)} is refused by the backend (HTTP {e.code}: {msg[:160]!r}): out for this "
                      f"run", flush=True)
            codex_pause(home)
            if any(h not in CODEX_HOME_PAUSED for h in codex_homes()):
                return chat_codex_direct(model, messages, effort)
            raise RuntimeError("Codex paused by the quota guard (no login the backend accepts)")
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
            # "servers are currently overloaded", "an error occurred ... you can retry": the backend's state, not
            # the function's (2026-10-12: 109 such errors were escalated to queue/). Two more tries, then defer.
            if _try < 2:
                time.sleep(30 * (_try + 1))
                return chat_codex_direct(model, messages, effort, _try + 1)
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
    home = codex_pick()
    r = subprocess.run(["codex", "exec", "--ephemeral", "--skip-git-repo-check", "--ignore-user-config",
                        "-s", "read-only", "-m", model, "-c", f'model_reasoning_effort="{CODEX_EFFORT.get(effort, effort)}"',
                        *feats,
                        "-o", out, "--json", "-"],
                       input="\n\n".join(convo), capture_output=True, text=True, cwd=CODEX_CWD, timeout=1800,
                       env=dict(os.environ, CODEX_HOME=home))
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


# Live state for tools/webmon.py: in-flight model calls, per-backend token totals, run counters.
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
CLAUDE_SUB = {}  # Claude rung model -> Codex model standing in while the grunts' Claude account is paused (--claude-sub)
CLAUDE_SUB_LOGIN = None  # the Codex login those stand-ins run on (--claude-sub-login, e.g. codex-2)
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
        # rung (first call of a later rung) or evidence (answering the model's own evidence requests)
        kind = ("review" if sysmsg is REVIEW_SYSTEM
                else "write" if len(messages) <= 2 else getattr(_tl, "step", "retry"))
        _live["calls"][cid] = dict(ea=getattr(_tl, "ea", 0), backend=backend, model=model,
                                   effort=CODEX_EFFORT.get(effort, effort) if backend == "codex" else effort,
                                   kind=kind, n=1,
                                   transport=os.environ.get("CSF_CODEX_TRANSPORT", "direct")
                                   if backend == "codex" else "api", since=time.time())
    live()
    try:
        if backend == "claude":
            text, usage = chat_claude(model, messages, effort)
        elif backend == "codex":
            text, usage = chat_codex(model, messages, effort)
        else:
            raise ValueError(f"unknown backend {backend!r}")
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


def normalize(code, ea=None):
    """Mechanical fixes applied before the gate (each used to cost a full retry call): the HOOK line's missing
    semicolon; #include "globals.h" when CSF_* macros are used without it (132 of ~220 retries, 2026-10-07);
    raw original addresses -> CSF_VTBL / CSF_ADDR_* (tools/addrfix.py, needed to link standalone); callee
    bindings renamed to the one canonical name per address (tools/api.py, db/api.json)."""
    import addrfix, api
    code = api.canonicalize(addrfix.fix(code, ea))
    code = re.sub(r"^(\s*HOOK(?:_METHOD|_KEEP_ECX_CDECL|_KEEP_ECX|_REGS|_TAIL)?\(.*\))\s*$", r"\1;", code, flags=re.M)
    hr = {"_DWORD": "std::uint32_t", "_WORD": "std::uint16_t", "_BYTE": "std::uint8_t", "_QWORD": "std::uint64_t"}
    if re.search(r"\b_(DWORD|WORD|BYTE|QWORD)\b", code):  # Hex-Rays types copied from the pseudocode
        code = re.sub(r"\b_(DWORD|WORD|BYTE|QWORD)\b", lambda m: hr[m.group(0)], code)
        if "<cstdint>" not in code:
            code = "#include <cstdint>\n" + code
    # reinterpret_cast can't take nullptr (std::nullptr_t): the commonest compile error left (21, 2026-10-08)
    code = re.sub(r"\breinterpret_cast<([^<>()]*\*)\s*>\(\s*nullptr\s*\)", r"static_cast<\1>(nullptr)", code)
    if re.search(r"\bCSF_(GLOBAL|FUNC|AT|STR)\b", code) and not re.search(r'#\s*include\s*"globals.h"', code):
        incs = list(re.finditer(r'^\s*#\s*include[^\n]*\n', code, re.M))
        at = incs[-1].end() if incs else 0
        code = code[:at] + '#include "globals.h"\n' + code[at:]
    return code


def compile_fix(code, err):
    """Code with clang's own suggestion applied when the gate's compile error is an unknown type name it can name
    (`unknown type name 'C_CMenuPause'; did you mean 'CMenuPause'?`: class names from an older packet), else None.
    The caller re-gates the result: no model call spent on a rename."""
    subs = dict(re.findall(r"error: unknown type name '(\w+)'; did you mean '([\w:]+)'\?", err or ""))
    if not subs:
        return None
    fixed = re.sub(r"\b(" + "|".join(map(re.escape, subs)) + r")\b", lambda m: subs[m.group(1)], code)
    return fixed if fixed != code else None


COMPILE_HINTS = [  # appended to a compile error the model has to fix itself
    (r"array type '[^']*' is not assignable", "C arrays can't be assigned as a whole: copy element by element "
                                              "(the original copies each element / dword too)"),
]


def gate_fixed(ea, tmp, db, code):
    """gate() plus the mechanical compile fixes; returns (ok, err, code as written)."""
    ok, err = gate(ea, tmp, db)
    fixed = None if ok else compile_fix(code, err)
    if fixed:
        open(tmp, "w").write(fixed)
        ok2, err2 = gate(ea, tmp, db)
        if ok2 or "compile error" not in err2:
            with _live_lock:
                _live["counts"]["autofix"] = _live["counts"].get("autofix", 0) + 1
            ok, err, code = ok2, err2, fixed
        else:
            open(tmp, "w").write(code)
    if not ok and "compile error" in (err or ""):
        err += "".join(f"\nhint: {h}" for rx, h in COMPILE_HINTS if re.search(rx, err))
    return ok, err, code


def autofix(code, ea=None):
    """normalize(), counted for the monitor (each fix is a retry call saved)."""
    fixed = normalize(code, ea)
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
double, missing/extra calls or call arguments, calls in another order, wrong virtual slots, wrong loop bounds, wrong
constants, stack objects smaller than the block the original passes to a callee, values cached across calls where
the original reloads.
Not behavior, never report: naming and style; last-bit floating-point rounding (x87 extended precision vs SSE) and
NaN ordering; float-to-int results outside the int32 range; integer overflow corner cases (INT_MIN); the order of
loads and stores with no call between them, unless a store can change what a later load reads; register and
stack-layout artifacts. A register the packet's NOTE calls a callee-save artifact is not an argument.
Output exactly one ```json block:
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
    """The rung that really runs: Codex is kept for L once its 5h window is past the reserve; while the grunts'
    Claude account is paused, its Haiku rungs run on Codex (--claude-sub, pinned to --claude-sub-login)."""
    _tl.codex_home = None
    if backend == "claude" and CLAUDE_PAUSED.is_set() and model in CLAUDE_SUB:
        _tl.codex_home = next((h for h in codex_homes() if codex_label(h) == CLAUDE_SUB_LOGIN), None)
        return "codex", CLAUDE_SUB[model]
    # a Claude stand-in only while that account has room: paused, the rung stays on Codex (2026-10-11: with Codex
    # past its reserve and Claude at its cap every M function was deferred while Codex stood at 70%)
    if backend == "codex" and tier in ("S", "M") and CODEX_SUB and (CODEX_RESERVED.is_set() or CODEX_PAUSED.is_set()) \
            and (CODEX_SUB[0] != "claude" or (claude_grunts_ready() and not CLAUDE_PAUSED.is_set())):
        return CODEX_SUB
    return backend, model


def work(ea, db, key, ladder, out_dir=None, example=None, review_over=None, evidence_budget=0, tier=None):
    """ladder: [(backend, model, attempts), ...] tried in order; the conversation (with gate errors) carries over.
    example: ea of a done clone sibling (its file is shown). review_over: Codex reviews Claude answers above N bytes.
    evidence_budget: read-only evidence requests the model may make (tools/evidence.py); they aren't attempts."""
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
    fails = []  # per failed attempt: the rung and what failed (gate.fail_kinds), for `grunt.py cost`
    steps = [(b, m) for b, m, n in ladder for _ in range(n)]
    dead, reviews = set(), []  # dead: rungs that errored (backend, or go:model); reviews: True = reviewer rejected
    quota_hit = False  # a rung was skipped for quota (Codex / Claude paused, limit hit): defer instead of escalating
    for attempt, (backend, model) in enumerate(steps):
        # first attempt per rung at high effort, retries at max (a pinned model@effort ignores both)
        first = attempt == 0 or steps[attempt - 1] != (backend, model)
        backend, model = substitute(backend, model, tier)
        dk = f"{backend}:{model.split('@')[0]}" if backend == "claude" else backend
        if dk in dead:
            continue
        try:
            _tl.step = "rung" if first and attempt else "retry"  # "rung": first call of a later rung of the ladder
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
            quota_hit |= bool(re.search(r"paused by the quota guard|usage.limit|rate.limit|session limit|hit your (session |usage )?limit"
                                        r"|overloaded|server_error|service_unavailable|you can retry|stream ended without", str(e), re.I))
            err = f"{backend} error: {e}"
            fails.append(dict(by=f"{backend}:{model}", kinds=["backend"]))
            continue
        if code:
            code = autofix(code, ea)
        if not code:
            ok, err = False, "no ```cpp block found in your answer"
        else:
            open(tmp, "w").write(code)
            ok, err, code = gate_fixed(ea, tmp, db, code)
        if ok and backend == "claude" and review_over is not None and review_over >= 0 \
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
                        example=example, reviews=reviews, evidence=n_ev, fails=fails)
        fails.append(dict(by=f"{backend}:{model}", kinds=gate_mod.fail_kinds(err)))
        msgs += [{"role": "assistant", "content": text},
                 {"role": "user", "content": f"The gate rejected the file:\n{err}\nFix it and answer again in the same format."}]
    if quota_hit:  # not a real failure: a rung was out of quota — retry in a later run
        return dict(ea=ea, ok=False, deferred=True, attempts=len(usage_all), err=err[:500], usage=usage_all,
                    reviews=reviews, fails=fails)
    qdir = os.path.join(out_dir, "queue") if out_dir else os.path.join(ROOT, "queue")
    os.makedirs(qdir, exist_ok=True)
    with open(os.path.join(qdir, f"{ea:08x}.md"), "w") as f:
        f.write(f"# Escalation 0x{ea:08X}\n\n## Last gate error\n```\n{err}\n```\n\n## Last answer\n{text}\n\n"
                f"## Packet\n{packet}\n")
    return dict(ea=ea, ok=False, attempts=len(steps), err=err[:500], usage=usage_all, reviews=reviews, evidence=n_ev,
                fails=fails)


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


def callers_outside(db):
    """{ea: number of game functions in OTHER modules that call it} (centrality: what other code waits on)."""
    out = collections.Counter()
    for ea, f in db.F.items():
        r = db.rows.get(ea)
        if not r or r["region"] != "game":
            continue
        for c in f["callees"]:
            rc = db.rows.get(c)
            if rc and rc["region"] == "game" and rc["module"] != r["module"]:
                out[c] += 1
    return out


def rank_modules(db, n=10):
    """Modules to wave next: undone bytes weighted by how much code elsewhere calls into them
    (sum over todo functions of size * log2(2 + outside callers)). Returns [(score, id, name, todo_bytes, done%)]."""
    import math, modules as M
    fanin = callers_outside(db)
    names = {m["id"]: m.get("name") or m["id"] for m in M.read_yaml()}
    score, todo, total = collections.Counter(), collections.Counter(), collections.Counter()
    for ea, r in db.rows.items():
        if r["region"] != "game" or not r["module"] or r["module"] == "stl" or ea not in db.F:
            continue  # stl: VC7.1 container replicas + template impls, written by hand (PLAN), not waved
        size = db.F[ea]["size"]
        total[r["module"]] += size
        if r["status"] == "todo":
            todo[r["module"]] += size
            score[r["module"]] += size * math.log2(2 + fanin.get(ea, 0))
    return [(round(v), m, names.get(m, m), todo[m], 100 * (1 - todo[m] / max(total[m], 1)))
            for m, v in score.most_common(n)]


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
        if rows.get(ea, {}).get("status") == "done":
            done_in.setdefault(i, ea)
        if ea in exclude:
            busy.add(i)
    eas = sorted(db.F)
    room = {eas[i]: eas[i + 1] - eas[i] for i in range(len(eas) - 1)}
    cands = []
    # addresses our hand-written code already hooks (src/ outside auto/: dev mode, handle table, options): a second
    # hook from src/auto replaces it. 2026-10-11: a wave reimplemented 0x508F50, devcmd's window placement hook, and
    # every smoke window came up on screen instead of off it.
    owned = set()
    for p in glob.glob(os.path.join(ROOT, "src", "**", "*.cpp"), recursive=True):
        if os.sep + "auto" + os.sep not in p:
            owned |= {int(m, 16) for m in re.findall(r"^HOOK(?:_METHOD|_TRAMP)?\(\s*0x([0-9A-Fa-f]+)", open(p).read(), re.M)}
    for ea, r in rows.items():
        if r["status"] != "todo" or r["region"] != "game" or ea in exclude or ea in owned:
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


SMOKE_SECS, SMOKE_LONG_SECS, SMOKE_LONG_EVERY = 30, 60, 5  # seconds after ready (smoke.sh); every 5th smoke runs long
# 15 -> 30 s (2026-10-09): a verified script handler (0x45B570) first ran 17 s into FR01 and crashed there; 54 passing
# 15 s smokes had installed it without ever executing it. Parallel smokes (tools/pool.py) make the longer run cheap.
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


def set_disabled(eas, play=None):
    """Disable exactly these hooks in a play copy's ini (lines tagged BISECT_TAG; others are left alone)."""
    ini = os.path.join(play or PLAY, "csf_hook.ini")
    lines = [l for l in open(ini).read().splitlines() if not l.endswith(BISECT_TAG)]
    lines += [f"disable 0x{ea:X} {BISECT_TAG}" for ea in sorted(eas)]
    open(ini, "w").write("\n".join(lines) + "\n")


DLL_MAP = os.path.join(ROOT, "build", "dinput8.map")


def crash_suspects(out, batch):
    """Functions of `batch` named by the smoke's crash report: the faulting EIP, then return addresses on the dumped
    stack, that fall inside our DLL, resolved through the linker map of the deployed build to src/auto/<ea>.cpp.
    Most likely first; [] when the crash is elsewhere (game code, no report)."""
    # the log is CRLF and the DLL base is printed with %p (upper case): both made this miss every crash until
    # 2026-10-08, so every bisect fell back to the blind binary search
    m = re.search(r"CRASH code=\w+ eip=([0-9a-f]{8}).*?\n\s*eip module ([0-9a-f]+) \(ours ([0-9a-f]{8})\); stack:\n"
                  r"((?:[ \t]+[0-9a-f]{8}(?: [0-9a-f]{8})*\n)*)", out.replace("\r", ""), re.I)
    if not m or not os.path.exists(DLL_MAP):
        return []
    if int(m.group(2), 16) == 0:
        # EIP in no module: a smashed return address, so the stack is corrupt and the return addresses left on it
        # are stale frames of functions that already returned fine (2026-10-09: four innocent script handlers
        # named in a row, bisect gave up). The blind binary search is the reliable path here.
        return []
    base, pref, syms = int(m.group(3), 16), 0x10000000, []
    for l in open(DLL_MAP, errors="replace"):
        if l.strip().startswith("Preferred load address is"):
            pref = int(l.split()[-1], 16)
        f = re.match(r"\s*\d{4}:[0-9a-f]{8}\s+\S+\s+([0-9a-f]{8,16})\s+(?:f\s+)?(?:i\s+)?(\S+)", l)
        if f:
            syms.append((int(f.group(1), 16), f.group(2)))
    syms.sort()
    addrs = [int(m.group(1), 16)] + [int(w, 16) for w in m.group(4).split()]
    out_eas, want = [], set(batch)
    for a in addrs:
        va = a - base + pref
        i = max((k for k in range(len(syms)) if syms[k][0] <= va), default=None) if base <= a < base + 0x1000000 \
            else None
        o = re.match(r"([0-9a-f]{8})\.cpp\.obj$", syms[i][1]) if i is not None else None
        if o and int(o.group(1), 16) in want and int(o.group(1), 16) not in out_eas:
            out_eas.append(int(o.group(1), 16))
    return out_eas


def fail_cap(out):
    """Seconds a bisect smoke needs when the failure it reproduces happened `fail_at` s after ready (smoke.sh): a run
    that survives twice that (+3 s) passes. None when the failure has no time (a hang, never ready)."""
    m = re.search(r"\bfail_at=(\d+)s", out or "")
    return max(5, 2 * int(m.group(1)) + 3) if m else None


def bisect(batch, max_culprits=4, first_out=""):
    """The deployed DLL holds `batch` (uncommitted) and its smoke failed (its output: first_out). Find the culprits
    by toggling hooks in the ini (no rebuild). Each failed run's crash report is read first (crash_suspects): the
    named function is taken and the rest re-run, ~1 smoke per culprit. When a crash names none of the batch: a
    control run with the whole batch off and a re-run of the rest (side by side on the smoke pool), then a search
    with the rest off: with a pool of k copies each round runs k parts at once (k-way; plain binary search with one
    copy, where a half that passes clears it and the other half is taken without its own smoke). A run of the rest
    that passes ends it. Runs are capped at ~2x the failure's time after ready (fail_cap), not the full smoke.
    Returns (culprits, why): culprits=None when the batch is not to blame or the result is unreliable."""
    import pool
    n, controlled = 0, False
    cap = fail_cap(first_out)
    secs0, env0 = _last_smoke[0] or SMOKE_SECS, _last_smoke[1]
    k = pool.size()
    if k > 1:
        pool.sync()

    def job(on):
        secs = min(secs0, cap) if cap else secs0
        env = {**env0, "CSF_SMOKE_ADAPTIVE": "0"} if cap else env0  # survive the whole cap, not a quiet end
        return secs, env, lambda play: set_disabled(set(batch) - set(on), play)

    def runs(ons):
        nonlocal n
        n += len(ons)
        live(phase=f"bisect: smokes {n - len(ons) + 1}..{n} ({'/'.join(str(len(o)) for o in ons)} on)")
        return [(not ok, out) for ok, out in pool.run([job(on) for on in ons])]

    def run(on):
        return runs([on])[0]

    try:
        culprits, rest, out, named = [], list(batch), first_out, 0
        while True:
            sus = crash_suspects(out, rest)
            if sus:
                c, named = sus[0], named + 1
            else:
                if not controlled:
                    controlled = True
                    if k > 1 and not culprits:
                        (ctl_fail, _), (rest_fail, out) = runs([[], rest])
                    else:
                        ctl_fail, _ = run([])
                        rest_fail = True if culprits else run(rest)[0]
                    if ctl_fail:
                        return None, "control run (whole batch disabled) also fails: crash is not from this batch"
                    if not rest_fail:
                        return None, "the full batch passed on re-run: flaky crash, not bisectable"
                    sus = crash_suspects(out, rest)
                    if sus:
                        c, named = sus[0], named + 1
                if not sus:
                    cur = rest
                    while len(cur) > 1:
                        if k > 1:
                            parts = [cur[i * len(cur) // k:(i + 1) * len(cur) // k] for i in range(k)]
                            parts = [p for p in parts if p]
                            failing = [p for p, (f, _) in zip(parts, runs(parts)) if f]
                            if not failing:
                                return None, (f"no single part of {len(cur)} fails alone ({n} smokes): an "
                                              "interaction between parts or a flaky crash")
                            cur = failing[0]
                        else:
                            a, b = cur[:len(cur) // 2], cur[len(cur) // 2:]
                            cur = a if run(a)[0] else b
                    c = cur[0]
                    # a culprit no crash report names is run once more alone: one slow start (never ready within
                    # the limit on a loaded machine) at any step above would otherwise blame a correct function
                    # (2026-10-13: four of a group, one of them instruction for instruction the original)
                    if not run([c])[0]:
                        return None, (f"{c:x} found by search passes when on alone ({n} smokes): a flaky failure "
                                      "(slow start?) or an interaction")
            culprits.append(c)
            rest.remove(c)
            failed, out = run(rest)
            if not failed:
                return culprits, f"{n} smokes ({named} named by crash reports{', k=' + str(k) if k > 1 else ''})"
            if len(culprits) >= max_culprits:
                return None, f"more than {max_culprits} culprits: {' '.join(f'{c:x}' for c in culprits)} ..."
    finally:
        for play in pool.copies():
            set_disabled([], play)


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
    # the function's own lines first (DIFF / MISMATCH / T3): the tail alone rarely holds them
    own = [l for l in smoke_out.splitlines() if f"{ea:08X}" in l.upper()]
    own = "\n".join(sorted(set(own), key=own.index)[:40])
    with open(os.path.join(ROOT, "queue", f"{ea:08x}.md"), "w") as f:
        f.write(f"# Escalation 0x{ea:08X}: {why}\n\n## Its lines in the smoke log\n```\n{own}\n```\n\n"
                f"## Smoke log tail\n```\n{smoke_out[-2500:]}\n```\n\n"
                f"## Rejected file\n```cpp\n{code}\n```\n")
    if os.path.exists(src):
        os.remove(src)


STOP = os.path.join(ROOT, "work", "grunt_stop")


def run_config(args, lanes=False):
    """The cost/speed settings of this run, for the monitor."""
    return dict(transport=os.environ.get("CSF_CODEX_TRANSPORT", "direct"), effort=CODEX_EFFORT["high"],
                retry_effort=CODEX_EFFORT["max"],
                review_over=args.review_over, codex_cap=getattr(args, "codex_cap", None),
                codex_reserve=getattr(args, "codex_reserve", None), claude_cap=getattr(args, "claude_cap", None), codex_sub=getattr(args, "codex_sub", None),
                backend=getattr(args, "backend", "lanes"), wave=getattr(args, "module", None),
                **(dict(tiers={t: dict(ladder=getattr(args, f"{t.lower()}_ladder"), jobs=getattr(args, f"{t.lower()}_jobs"))
                               for t in TIERS}, s_max=args.s_max, m_max=args.m_max, l_max=args.l_max, review_effort=args.review_effort) if lanes else {}))


def run(args):
    """Single functions outside the lanes: redo a queue item (--only), or an A/B arm (--dry writes to work/ab/)."""
    db = DB()
    picks = [(int(x, 16), None) for x in args.only.split(",")] if args.only else select(db, args.n, args.max_size, args.min_size, modules=module_ids(args.module))
    targets = [ea for ea, _ in picks]
    example = dict(picks)
    live(mode="run" + (" (dry)" if args.dry else ""), phase="grunting", config=run_config(args),
         counts=dict(total=len(targets), submitted=len(targets)))
    ladder = [("claude", args.claude_model, 2)] if args.backend == "claude" else [("codex", args.codex_model, 2)]
    print(f"batch of {len(targets)}: {' '.join(f'{t:x}' for t in targets)}", flush=True)
    results = []
    with cf.ThreadPoolExecutor(args.jobs) as ex:
        out_dir = os.path.join(ROOT, "work", "ab", args.backend + (f"-{os.environ['CSF_CODEX_EFFORT']}"
                               if os.environ.get("CSF_CODEX_EFFORT") else "")
                               + (f"-{os.environ['CSF_AB_ARM']}" if os.environ.get("CSF_AB_ARM") else "")) \
            if args.dry else None
        futs = {ex.submit(work, ea, db, None, ladder, out_dir, example.get(ea), args.review_over,
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
                    f.write(json.dumps(dict(r, ts=time.time(), **({"arm": os.environ["CSF_AB_ARM"]}
                                                                   if os.environ.get("CSF_AB_ARM") else {}))) + "\n")
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
    subprocess.run([sys.executable, os.path.join(ROOT, "tools", "api.py"), "build"], check=False)  # canonical bindings
    files = [f"src/auto/{r['ea']:08x}.cpp" for r in good] + ["db/functions.csv", "db/proposals.jsonl", "db/globals.yaml",
                                                       "db/verify.json", "db/api.json"]
    files += [os.path.relpath(p, ROOT) for p in glob.glob(os.path.join(ROOT, "queue", "*.md"))]
    if subprocess.run(["git", "-C", ROOT, "ls-files", "--", "queue"], capture_output=True, text=True).stdout:
        files.append("queue")  # also stages escalations resolved (deleted) since the last commit
    import repo  # one git lock with tools/verifier.py, which commits concurrently
    repo.commit(files, title)
    print("committed", flush=True)


TIERS = ("S", "M", "L", "X")


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
    done example up to --m-max; M = up to --m-max; L = up to --l-max; X = the rest (Opus: the captain's model writes
    the big ones). Each tier has its own in-flight cap, so slow L/X calls
    never hold up the S stream. Quota: Codex pauses at --codex-cap of its 5h window (L gets no new work, S/M lose their
    Codex rung); the grunts' Claude account pauses at --claude-cap of its 5h or weekly window. A function whose only
    remaining rungs were out of quota is deferred, not escalated. Every --batch gate passes: build + smoke + commit.

    Rolling waves (args.roll, from `wave --waves N`): modules are taken in rank order and up to --overlap are open at
    once. The next one is admitted (IDB prep, then its functions join the candidates behind the older modules') as
    soon as the open ones have handed out all their S/M work and an S/M slot is free, so those slots don't idle
    through a module's L/X tail. A module with
    nothing in flight and nothing left to submit is closed: its last results are committed and its end-of-wave steps
    (verifier --module, link test, report, stop rule: wave_close) run on one background worker, one module at a
    time, while production goes on. A stop rule that fires ends the run like the stop file does (exit 3). A module
    with work that waits for quota (L/X while Codex is paused, functions deferred) stays open, so no new module
    comes in past --overlap; once nothing can run the loop waits, and deferred functions are offered again when
    both accounts are back."""
    lane_config(args)
    rolling = getattr(args, "roll", 0)
    if args.module and not rolling:
        ida_refresh(args.module)
    global REVIEW_EFFORT, CODEX_SUB, CLAUDE_SUB, CLAUDE_SUB_LOGIN
    REVIEW_EFFORT = args.review_effort
    CODEX_SUB = tuple(args.codex_sub.split(":", 1)) if args.codex_sub else None
    CLAUDE_SUB = dict(x.split("=", 1) for x in args.claude_sub.split(",") if x) if args.claude_sub else {}
    CLAUDE_SUB_LOGIN = args.claude_sub_login or None
    db = DB()
    wave = module_ids(args.module)
    key = None
    ladders = {"S": ladder_spec(args.s_ladder), "M": ladder_spec(args.m_ladder),
               "L": ladder_spec(args.l_ladder), "X": ladder_spec(args.x_ladder)}
    caps = {"S": args.s_jobs, "M": args.m_jobs, "L": args.l_jobs, "X": args.x_jobs}
    pool = cf.ThreadPoolExecutor(sum(caps.values()))
    inflight, good, submitted, total_ok, total_esc, total_def = {}, [], 0, 0, 0, 0
    stopping, quota_t = False, 0
    quarantined = set()  # in-flight functions quarantined by a batch build (their result is then an escalation)
    open_mods, seen, closings, stops = [], set(), [], []  # rolling waves: open modules in admission order
    admitted, closed, broken = 0, 0, False
    closer = cf.ThreadPoolExecutor(1) if rolling else None
    if os.path.exists(STOP):
        os.remove(STOP)
    live(mode="lanes", phase="grunting", config=run_config(args, lanes=True),
         counts=dict(total=args.total, batch=args.batch, ok=0, esc=0, pending=0, submitted=0, committed=0))

    fanin = callers_outside(db) if args.hub_callers else {}

    def tier_of(ea, ex):
        size = db.F[ea]["size"]
        if args.hub_callers and fanin.get(ea, 0) >= args.hub_callers and size >= 40:
            return "L"  # a hub: many functions elsewhere depend on it, so the strong ladder writes it
        if size < args.s_max or (ex is not None and size <= args.m_max):
            return "S"
        return "M" if size <= args.m_max else "L" if size <= args.l_max else "X"

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
        per = {}
        for h in codex_homes():
            try:
                per[h] = codex_usage(h)["5h"]
            except Exception:
                continue  # endpoint down: keep this login's current state
            hp, hr = per[h]
            if hp >= args.codex_cap and h not in CODEX_HOME_PAUSED:
                print(f"{codex_label(h)} 5h window at {hp:.0f}% (cap {args.codex_cap}): paused, resets in "
                      f"{hr // 60} min", flush=True)
                codex_pause(h)
            elif hp < args.codex_cap and h in CODEX_HOME_PAUSED and h not in CODEX_HOME_OUT:
                print(f"{codex_label(h)} 5h window at {hp:.0f}%: resumed", flush=True)
                codex_pause(h, False)
        if not per:
            return
        pct, reset = min(per.values())  # the reserve follows the login with the most room
        if CODEX_SUB and pct >= args.codex_reserve and not CODEX_RESERVED.is_set():
            CODEX_RESERVED.set()
            print(f"Codex 5h window at {pct:.0f}% (reserve {args.codex_reserve}): S/M Codex rungs -> "
                  f"{':'.join(CODEX_SUB)}, Codex kept for L", flush=True)
        elif pct < args.codex_reserve and CODEX_RESERVED.is_set():
            CODEX_RESERVED.clear()
            print(f"Codex 5h window at {pct:.0f}%: S/M Codex rungs back on Codex", flush=True)
        live(codex_paused=CODEX_PAUSED.is_set(), codex_reserved=CODEX_RESERVED.is_set(), codex_5h=pct,
             codex_logins={codex_label(h): p for h, (p, _) in per.items()})


    def refill():
        nonlocal submitted, stopping
        if not stopping and os.path.exists(STOP):
            stopping = True
            print("stop file found: finishing in-flight tasks, then smoke + commit", flush=True)
            live(phase="stopping")
        for fu in [f for f in closings if f.done()]:  # a closed module's stop rule (or a failed regression check)
            closings.remove(fu)
            stops.extend(fu.result())
        if stops and not stopping:
            stopping = True
            print("stop rule fired: finishing in-flight tasks", flush=True)
            live(phase="stopping")
        if stopping:
            return
        quota()
        if rolling and not CODEX_PAUSED.is_set() and not CLAUDE_PAUSED.is_set():
            for r in db.rows.values():  # deferred on quota earlier in this run: the windows are open again
                if r["status"] == "deferred":
                    r["status"] = "todo"
        while True:
            busy = {ea for ea, _ in inflight.values()}
            cnt = collections.Counter(t for _, t in inflight.values())
            blocked = {t for t in TIERS if CODEX_PAUSED.is_set() and codex_only(t)}
            for t in blocked:  # a tier that can only use Codex waits while Codex is paused
                cnt[t] = caps[t]
            if submitted >= args.total or all(cnt[t] >= caps[t] for t in TIERS):
                break
            # wide window: the leaf-first head is almost all small (S); M and L need larger candidates
            sel = [] if rolling and not open_mods else select(
                db, 4000, args.max_size, args.min_size, exclude=busy,
                modules={o["id"] for o in open_mods} if rolling else wave)
            if rolling:  # the older module first: its tail is what its verification waits for
                order = {o["id"]: i for i, o in enumerate(open_mods)}
                sel.sort(key=lambda c: order[db.rows[c[0]]["module"]])
            # per module: what it still has to hand out (candidates whose tier is full or waits for Codex, and
            # functions deferred on quota): it stays open
            left = collections.Counter(r["module"] for r in db.rows.values() if r["status"] == "deferred") \
                if rolling else collections.Counter()
            small = 0  # S/M candidates among them
            for ea, ex in sel:
                t = tier_of(ea, ex)
                if cnt[t] >= caps[t]:
                    left[db.rows[ea]["module"]] += 1  # also of a tier waiting for Codex: the module stays open
                    small += t in "SM"
                    continue
                if submitted >= args.total:
                    break
                fut = pool.submit(work, ea, db, key, ladders[t], None, ex, args.review_over, ev_budget(args, db, ea),
                                  tier=t)
                inflight[fut] = (ea, t)
                cnt[t] += 1
                submitted += 1
            if not rolling:
                break
            flying = collections.Counter(db.rows[ea]["module"] for ea, _ in inflight.values())
            for o in [o for o in open_mods if not flying[o["id"]] and not left[o["id"]]]:
                close(o)
            # the next module starts once the open ones have handed out all their S/M work and a small slot idles:
            # what is left of them is the L/X tail (it keeps its slots; a later module's functions queue behind)
            if broken or submitted >= args.total or small or all(cnt[t] >= caps[t] for t in "SM") or not admit():
                break
        live(counts=dict(submitted=submitted), inflight_tiers=dict(cnt),
             **(dict(open=[o["name"] for o in open_mods]) if rolling else {}))

    def admit():
        """Rolling waves: open the next module of the ranking. False: the limits (--waves, --overlap) or the ranking
        say no."""
        nonlocal admitted, broken
        if (rolling > 0 and admitted >= rolling) or len(open_mods) >= args.overlap:
            return False
        ranked = [(m, nm) for _, m, nm, *_ in rank_modules(db, 60) if m not in seen]
        if not ranked:
            if not open_mods and not inflight:
                print("waves: ranking exhausted", flush=True)
            return False
        if not flush():  # names of what is done reach functions.csv before the IDB sync reads it
            broken = True
            return False
        m, name = ranked[0]
        seen.add(m)
        admitted += 1
        print(f"\n==== waves: {admitted}{'/' + str(rolling) if rolling > 0 else ''} {name} ({m})"
              + (f"  [still open: {', '.join(o['name'] for o in open_mods)}]" if open_mods else ""), flush=True)
        ida_refresh(m)
        live(phase="grunting")
        open_mods.append(dict(id=m, name=name, t0=time.time()))
        args.module = ",".join(o["id"] for o in open_mods)
        live(config=run_config(args, lanes=True))
        return True

    def close(o):
        """Rolling waves: the module has nothing in flight and nothing left to submit. Commit, then its end-of-wave
        steps in the background."""
        nonlocal closed, broken
        open_mods.remove(o)
        if not flush():
            broken = True
            return
        closed += 1
        print(f"wave {o['name']} ({o['id']}): production done in {time.time() - o['t0']:.0f}s; verify + link test "
              f"in the background", flush=True)
        closings.append(closer.submit(wave_close, args, o, closed))

    def flush():
        """Produce side only: link the batch (the gate compiles each file alone; link errors show up here), commit
        it as pending. The game-side checks (smoke, bisect, diff) are tools/verifier.py's, run apart (the wave's
        verify step, or by hand next to a long lanes run): production never waits on the game."""
        nonlocal good, total_ok, total_esc
        if not good:
            return True
        live(phase="link")
        r = subprocess.run(["ninja", "-C", os.path.join(ROOT, "build")], capture_output=True, text=True)
        if r.returncode:
            out = "build/deploy failed:\n" + (r.stdout + r.stderr)[-3000:]
            # the build sees every file in src/auto: also in-flight ones that passed the gate since the batch was cut
            pending = [ea for ea, _ in inflight.values()]
            culprits = build_culprits(out, [r["ea"] for r in good] + pending)
            if not culprits:
                print(out, flush=True)
                live(phase="BUILD FAILED")
                return False
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
        commit(good, f"grunt lanes: {len(good)} functions reimplemented (pending verification)")
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
    waiting = False
    while inflight or (rolling and open_mods and not stopping and not broken
                       and (CODEX_PAUSED.is_set() or CLAUDE_PAUSED.is_set())):
        if not inflight:  # rolling waves: every open module waits for an account; the quota guard resumes it
            if getattr(args, "stop_on_quota", False):
                print("waves: nothing can run until a quota window reopens; stopping (--stop-on-quota)", flush=True)
                stopping = True
                break
            if not waiting:
                print("waves: nothing can run until a quota window reopens; waiting", flush=True)
                live(phase="waiting for quota")
            waiting = True
            flush()
            time.sleep(60)
            refill()
            continue
        if waiting:
            waiting = False
            live(phase="grunting")
        done, _ = cf.wait(list(inflight), timeout=60, return_when=cf.FIRST_COMPLETED)
        for fu in done:
            ea, tier = inflight.pop(fu)
            try:
                r = fu.result()
            except Exception as e:
                import traceback
                tb = "".join(traceback.format_exception(e))
                print(f"  [{tier}] {ea:08x} tooling exception:\n{tb}", file=sys.stderr, flush=True)
                r = dict(ea=ea, ok=False, err=f"exception: {e} | {tb.strip().splitlines()[-3].strip()}", usage=[])
            handle(r, tier)
        if len(good) >= args.batch and not flush():
            broken = True
        if broken:
            pool.shutdown(wait=True, cancel_futures=True)
            break
        refill()
    for o in list(open_mods):  # stopped with modules open: what they have is still checked in game
        if not broken:
            close(o)
    if not broken and flush():
        live(phase="stopped" if stopping else "finished")
    print(f"lanes done: {total_ok} ok, {total_esc} escalated, {total_def} deferred", flush=True)
    if rolling:
        closer.shutdown(wait=True)
        for fu in closings:
            stops.extend(fu.result())
        for x in stops:
            print(f"waves: stopped: {x}", flush=True)
        if stops:
            sys.exit(STOP_EXIT)
        if broken:
            sys.exit(1)


def ida_refresh(module_arg):
    """Wave prep: push db/ into the IDB (tools/ida/sync.py, ~1 min: function names the earlier waves produced,
    classes, typed vtables, member types) and re-decompile the module's open functions (export_pseudo.py --only).
    Names of classes, fields and modules only change in tools/naming.py (milestones, never inside a wave); the
    whole-program type pass (~45 min) is tools/ida/propagate.sh, run between waves."""
    ids = module_ids(module_arg)
    eas = [f"{ea:x}" for ea, r in load_csv().items() if r["module"] in ids
           and r["status"] not in ("done", "lib")]
    if not eas:
        return
    live(mode="lanes", phase="ida: sync + re-decompile")
    run = lambda *a: subprocess.run([sys.executable, os.path.join(ROOT, "tools", "ida", a[0]), *a[1:]],  # noqa: E731
                                    capture_output=True, text=True)
    r = run("sync.py")
    if r.returncode:
        print(f"wave prep: IDB sync failed, packets keep the old pseudocode: {r.stderr.strip()[-300:]}", flush=True)
        return
    r = run("export_pseudo.py", "--only", ",".join(eas))
    print(f"wave prep: IDB synced, {len(eas)} functions re-decompiled "
          f"({(r.stdout.strip().splitlines() or ['?'])[-1]})", flush=True)


WAVES_LOG = os.path.join(ROOT, "work", "waves.jsonl")
STOP_EXIT = 3  # wave's exit code when the stop rule fires


def wave(args):
    """The RE loop for one module in one command:
      1. prep (lanes' ida_refresh): the IDB gets what db/ holds, the module is re-decompiled
      2. produce: lanes over the module (gate + link + commit as pending)
      3. verify: tools/verifier.py --module (baseline, then the module's pending functions, in game)
      4. tools/linktest.py summary (what is left before the standalone exe links)
      5. stop rule (wave_report): too many quarantines or functions never installed in a passing smoke -> exit 3
    The Opus review is not a wave step: tools/review.py sample is one rolling sample over all done code.
    `wave --next` ranks modules by centrality instead (rank_modules); `wave --waves N` runs N waves in rank order."""
    if args.waves:
        return waves(args)
    if args.next or not args.module_name:
        db = DB()
        print(f"{'score':>8}  {'module':10s} {'name':28s} {'todo KB':>8} {'done':>6}")
        for sc, m, n, todo, pct in rank_modules(db, 15):
            print(f"{sc:8d}  {m:10s} {n:28s} {todo / 1024:8.1f} {pct:5.1f}%")
        print("\nrun: tools/grunt.py wave <module>   (or --waves N: N waves in this order, stop rule between)")
        return
    args.module = args.module_name
    if args.total == 200:  # lanes' default: a wave takes the whole module
        args.total = 100000
    t0 = time.time()
    lanes(args)
    py = sys.executable
    tool = lambda *a: subprocess.run([py, os.path.join(ROOT, "tools", a[0]), *a[1:]])
    if not args.no_verify:
        print("wave: verify", flush=True)
        tool("verifier.py", "--wait", "--module", args.module)
    print("wave: link test", flush=True)
    tool("linktest.py")
    rep = wave_report(args, module_ids(args.module), t0)
    if rep["stop"]:
        sys.exit(STOP_EXIT)


VERIFIER_LOG = os.path.join(ROOT, "work", "verifier_log.jsonl")


def _jsonl(p, skip=0):
    try:
        with open(p) as f:
            lines = f.readlines()[skip:]
    except OSError:
        return []
    out = []
    for l in lines:
        try:
            out.append(json.loads(l))
        except ValueError:
            pass
    return out


def wave_report(args, mods, t0, since=None):
    """What this wave's in-game check found, and whether the wave loop should stop. Counts the verifier groups run
    since the wave's verify step started (`since`; default: since the wave started, t0. Rolling waves overlap, so
    an earlier module's groups may fall after t0) and the module's done functions
    still pending (never installed in a passing smoke). `installed` is exactly that: the hook was on in a smoke
    that passed; whether the function ran or was compared is the ledger's tier (verify.py report). One smoke
    failure the verifier can't attribute is reported, not counted as a defect; it leaves functions pending."""
    v = 0
    notes, quar = [], set()
    for r in _jsonl(VERIFIER_LOG):
        if r.get("ts", 0) >= (since or t0) and r.get("event") in ("group", "baseline"):
            v += r.get("verified", 0)
            quar.update(r.get("quarantined") or [])
            if r.get("note"):
                notes.append(r["note"][:160])
            elif r.get("event") == "baseline" and r.get("ok") is False:
                # the baseline itself failed without a culprit (a flaky crash): the module was never tried
                notes.append("baseline smoke failed, not attributable")
    db = DB()
    # the stop rule counts this module's functions only: a baseline smoke also quarantines older functions of other
    # modules (2026-10-11: five diff mismatches of old code stopped the run as "net_gathering_ddl 5/105")
    own = {e for e in quar if db.rows.get(int(e, 16), {}).get("module") in mods}
    q, q_other = len(own), len(quar - own)
    try:
        led = json.load(open(os.path.join(ROOT, "db", "verify.json")))
    except (OSError, ValueError):
        led = {}
    done = [f"{ea:08x}" for ea, r in db.rows.items() if r["module"] in mods and r["status"] == "done"]
    pend = [ea for ea in done if not led.get(ea, {}).get("ok")]
    stop = []
    if q >= 2 and q / max(1, v + q) > args.stop_quarantine:
        stop.append(f"quarantined {q}/{v + q} in game (> {args.stop_quarantine:.0%})")
    # not after a smoke failure the verifier could not attribute: its group is pending because of that failure
    # (2026-10-13: a control run that failed in 8 s, with a git index.lock error just before, stopped the run as
    # "41/44 never installed"); a later verify pass takes the group again
    if not args.no_verify and not notes and len(done) >= 10 and len(pend) / len(done) > args.stop_pending:
        stop.append(f"{len(pend)}/{len(done)} done functions never installed in a passing smoke (> {args.stop_pending:.0%})")
    eas = set(done)
    import verify
    depth = collections.Counter(t for ea, t in verify.tiers().items() if ea in eas)
    rep = dict(ts=time.time(), modules=sorted(mods), secs=round(time.time() - t0), installed=v, quarantined=q,
               quarantined_other=q_other,
               diffed=depth["diffed"], executed=depth["executed"], never_ran=depth["gate"],
               done=len(done), pending=len(pend), notes=notes, stop=stop)
    with open(WAVES_LOG, "a") as f:
        f.write(json.dumps(rep) + "\n")
    print(f"wave report: installed {v}, quarantined {q}" + (f" (+{q_other} of other modules)" if q_other else "") +
          f", {len(pend)}/{len(done)} still pending; of the module's "
          f"done functions: diffed {depth['diffed']}, executed {depth['executed']}, never ran {depth['gate']}" + (f"\n  unattributed smoke failures: {len(notes)}" if notes else ""),
          flush=True)
    for x in stop:
        print(f"  STOP: {x}", flush=True)
    return rep


def wave_close(args, o, n):
    """End-of-wave steps of one module of a rolling run (lanes' background worker, one module at a time): verify its
    pending functions in game, link test, report; every --regress-every modules the installed baseline is smoked
    with the widest coverage (verifier --baseline-only --full: what FR01's short start never reaches; a failure
    without a culprit is run once more before it stops the run). Returns the
    reasons to stop the run ([] = go on)."""
    py = sys.executable
    tool = lambda *a: subprocess.run([py, os.path.join(ROOT, "tools", a[0]), *a[1:]]).returncode  # noqa: E731
    try:
        since = time.time()
        try:
            led = json.load(open(os.path.join(ROOT, "db", "verify.json")))
        except (OSError, ValueError):
            led = {}
        if not any(r["module"] == o["id"] and r["status"] == "done" and not led.get(f"{ea:08x}", {}).get("ok")
                   for ea, r in load_csv().items()):
            # nothing written and nothing left over from an earlier run (e.g. every open function is in queue/)
            print(f"wave {o['name']}: nothing pending, no verify", flush=True)
            wave_report(args, {o["id"]}, o["t0"], since)
            return []
        if not args.no_verify:
            print(f"wave {o['name']}: verify", flush=True)
            tool("verifier.py", "--wait", "--module", o["id"])
        if not args.no_linktest:
            print(f"wave {o['name']}: link test", flush=True)
            tool("linktest.py")
        stop = [f"{o['name']}: {x}" for x in wave_report(args, {o["id"]}, o["t0"], since)["stop"]]
        if not stop and args.regress_every and n % args.regress_every == 0 and not args.no_verify:
            # the verifier quarantines the culprits it can name and passes; it fails when it can't name any (a flaky
            # crash, an interaction) or dies. One such failure is run again: of 9 on 2026-10-11, 5 were single
            # flakes of the savegame start and 4 came in a row from code that was really broken
            for attempt in (1, 2):
                print(f"wave {o['name']}: regression check (installed baseline, full coverage)"
                      + (", second run" if attempt == 2 else ""), flush=True)
                rc = tool("verifier.py", "--wait", "--baseline-only", "--full")
                if not rc:
                    break
                print(f"wave {o['name']}: regression check failed without a culprit (exit {rc})", flush=True)
            else:
                stop.append("regression check failed twice in a row without a culprit (work/baseline_fail_*.log)")
        return stop
    except Exception as e:  # a tooling error here must not pass for a clean wave
        import traceback
        traceback.print_exc()
        return [f"{o['name']}: end-of-wave steps raised {e!r}"]


def waves(args):
    """Waves in rank order as one rolling lanes run (see lanes): the next module starts while the previous one's
    slow tail finishes, and each module's verify + link test + report run in the background when it closes. Stops
    when a module's stop rule fires, the regression check fails, the ranking runs out, the stop file appears, or
    after --waves N modules (-1: no limit)."""
    args.roll, args.module = args.waves, None
    if args.total == 200:
        args.total = 100000
    lanes(args)


def cost(_):
    """Token totals per writing backend, and how often the in-lane Codex review rejected an answer (logged
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
    # throughput: latency per model from the call transcripts (work/calls/<ea>/*.json), and what the calls bought
    lat = collections.defaultdict(list)
    seen_err = set()
    for fp in glob.glob(os.path.join(ROOT, "work", "calls", "*", "*.json")):
        try:
            c = json.load(open(fp))
        except (OSError, ValueError):
            continue
        if c.get("secs") and not c.get("error"):
            lat[(c.get("kind") or "write", f"{c.get('backend')}:{c.get('model')}@{c.get('effort')}")].append(c["secs"])
        for m in c.get("messages") or []:  # gate feedback carried in the conversation (each once per function)
            t = m.get("content") if m.get("role") == "user" else None
            if isinstance(t, str) and t.startswith("The gate rejected the file:\n"):
                seen_err.add((c.get("ea"), t[28:].rsplit("\nFix it", 1)[0]))
    if lat:
        print("latency per call (s): kind / model           n     p50     p95")
        for (kind, mdl), xs in sorted(lat.items(), key=lambda x: -len(x[1])):
            xs.sort()
            print(f"  {kind:7} {mdl:42} {len(xs):5} {xs[len(xs) // 2]:7.1f} {xs[min(len(xs) - 1, int(len(xs) * .95))]:7.1f}")
    # what retries are spent on: gate.fail_kinds of every rejection (logged `fails`; transcripts for older runs)
    kinds, by_kind = collections.Counter(), collections.defaultdict(collections.Counter)
    logged = set()
    for l in open(LOG):
        r = json.loads(l)
        for f in r.get("fails") or []:
            logged.add(r["ea"])
            for k in f["kinds"]:
                kinds[k] += 1
                by_kind[k][re.sub(r"@.*", "", f["by"])] += 1
    import gate as gate_mod
    old = collections.Counter()
    for ea, e in seen_err:
        if (int(ea, 16) if isinstance(ea, str) else ea) not in logged:
            for k in gate_mod.fail_kinds(e):
                old[k] += 1
    for title, kc in (("rejections by kind (logged runs)", kinds), ("rejections by kind (older runs, transcripts)", old)):
        if kc:
            tot = sum(kc.values())
            print(f"{title}: {tot}")
            for k, n in kc.most_common():
                who = ", ".join(f"{w} {c}" for w, c in by_kind[k].most_common(3)) if kc is kinds else ""
                print(f"  {k:16} {n:5} {n / tot:5.0%}  {who}")
    try:
        import verify
        led = verify.load()
        ok = sum(1 for e in led.values() if e.get("ok"))
        diffed = sum(1 for e in led.values() if e.get("diffed") and not e.get("mismatched"))
        calls = sum(len(v) for v in lat.values())
        print(f"in game: {ok} functions installed in a passing smoke, {diffed} diffed clean; "
              f"{calls / max(ok, 1):.2f} logged model calls per installed function")
    except Exception as e:
        print(f"in game: unavailable ({e})")


LANES_YAML = os.path.join(ROOT, "db", "lanes.yaml")


def lane_args(l):
    """Arguments of `lanes` (and `wave`, which runs lanes for one module). The model policy (tiers, ladders, jobs,
    quota guards, stand-ins, in-lane review) is db/lanes.yaml, read into the same namespace by lane_config()."""
    l.add_argument("--total", type=int, default=200)
    l.add_argument("--module", help="module wave: comma list of module ids/names (tools/modules.py list)")
    l.add_argument("--batch", type=int, default=60, help="link + commit every N gate passes")
    l.add_argument("--config", default=LANES_YAML, help="model policy file (default db/lanes.yaml)")
    l.add_argument("--max-size", type=int, default=100000, help="skip functions over N bytes (default: none)")
    l.add_argument("--min-size", type=int, default=6)
    l.add_argument("--evidence", type=int, default=3, help="evidence requests per function (tools/evidence.py)")
    l.add_argument("--evidence-over", type=int, default=150, help="offer evidence requests above N bytes")


LANE_KEYS = ("s_max m_max l_max s_ladder m_ladder l_ladder x_ladder hub_callers s_jobs m_jobs l_jobs x_jobs "
             "review_over review_effort codex_cap claude_cap codex_reserve codex_sub claude_sub claude_sub_login emudiff_gate").split()


def lane_config(args):
    import yaml
    cfg = yaml.safe_load(open(args.config)) or {}
    if set(cfg) != set(LANE_KEYS):
        sys.exit(f"{args.config}: missing {sorted(set(LANE_KEYS) - set(cfg))}, unknown {sorted(set(cfg) - set(LANE_KEYS))}")
    for k, v in cfg.items():
        setattr(args, k, v)


if __name__ == "__main__":
    ap = argparse.ArgumentParser()
    sub = ap.add_subparsers(dest="cmd", required=True)
    r = sub.add_parser("run", help="single functions: redo (--only) or an A/B arm (--dry)")
    r.add_argument("--n", type=int, default=10)
    r.add_argument("--jobs", type=int, default=8)
    r.add_argument("--max-size", type=int, default=200)
    r.add_argument("--min-size", type=int, default=6)
    r.add_argument("--only")
    r.add_argument("--module", help="comma list of module ids/names (tools/modules.py list)")
    r.add_argument("--backend", choices=["codex", "claude"], default="codex")
    r.add_argument("--codex-model", default="gpt-6.1-sol")
    r.add_argument("--claude-model", default="sonnet")
    r.add_argument("--dry", action="store_true", help="write to work/ab/<backend>/, no smoke/commit/status")
    r.add_argument("--no-smoke", action="store_true")
    r.add_argument("--evidence", type=int, default=3, help="evidence requests per function (tools/evidence.py)")
    r.add_argument("--evidence-over", type=int, default=150, help="offer evidence requests above N bytes")
    r.add_argument("--review-over", type=int, default=150, help="Codex reviews Claude answers above N bytes (-1: off)")
    l = sub.add_parser("lanes", help="continuous grunting: one lane, size tiers S/M/L/X each with its own model ladder")
    lane_args(l)
    w = sub.add_parser("wave", help="one module, the whole loop: types first, produce, verify, review, link test")
    lane_args(w)
    w.add_argument("module_name", nargs="?", help="module id/name; omitted: --next shows the ranking")
    w.add_argument("--next", action="store_true", help="rank modules by centrality (what to wave next) and exit")
    w.add_argument("--no-verify", action="store_true")
    w.add_argument("--waves", type=int, default=0, help="run N waves in rank order (-1: until the stop rule or the "
                   "ranking runs out); each wave's report -> work/waves.jsonl")
    w.add_argument("--overlap", type=int, default=3, help="with --waves: modules open at once (the next one starts "
                   "while the earlier ones' slow tails finish; 1: one after another)")
    w.add_argument("--no-linktest", action="store_true", help="with --waves: skip the link test at a module's close")
    w.add_argument("--stop-on-quota", action="store_true", help="with --waves: end the run when every open module "
                   "waits for a quota window (default: wait and resume)")
    w.add_argument("--regress-every", type=int, default=3, help="with --waves: full regression smoke of the installed "
                   "baseline every N waves (0: off)")
    w.add_argument("--stop-quarantine", type=float, default=0.03, help="stop rule: the module's quarantined / installed+quarantined")
    w.add_argument("--stop-pending", type=float, default=0.25, help="stop rule: module's done functions never installed "
                   "in a passing smoke")
    sub.add_parser("cost")
    a = ap.parse_args()
    {"run": run, "lanes": lanes, "wave": wave, "cost": cost}[a.cmd](a)
