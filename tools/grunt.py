#!/usr/bin/env python3
"""Grunt driver: packet -> DeepSeek -> gate (1 retry) -> src/auto/<ea>.cpp, or escalate to queue/.

Usage:
  grunt.py run [--n 10] [--jobs 8] [--max-size 200] [--only ea,ea] [--model deepseek-flash] [--no-smoke]
  grunt.py cost                      token usage + projected cost from work/grunt_log.jsonl

After a batch: link, deploy, smoke test (unless --no-smoke); on success mark functions done and git commit.
"""
import argparse, concurrent.futures as cf, json, os, re, subprocess, sys, threading, time, urllib.request
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
  "core/memory.h" "types/auto_classes.h" "legacy/string.h". No inline asm. No malloc/free/new/delete:
  use game::Mem_Alloc(size)/game::Mem_Free(p) from core/memory.h.
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
- Memory reads: re-read a field/array element at every point the original reads it (check the disassembly).
  Never cache a value in a local across a call if the original reloads it after the call — callees may modify
  the object (e.g. a child removing itself from the list being iterated). Don't add checks or reads the
  original doesn't have.
- Give functions descriptive names in PascalCase (English), prefixed by the class id for methods, e.g.
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


def chat_codex(model, messages, effort="high"):
    """GPT via the Codex CLI headless (user's ChatGPT subscription). Ephemeral, read-only sandbox, no user config
    (no MCP servers), run outside the repo. The whole conversation goes in as one prompt."""
    os.makedirs(CODEX_CWD, exist_ok=True)
    convo = [messages[0]["content"], "Answer directly from the packet below. Do not run commands or use tools."]
    for m in messages[1:]:
        tag = "PACKET / REQUEST" if m["role"] == "user" else "YOUR PREVIOUS ANSWER"
        convo.append(f"=== {tag} ===\n{m['content']}")
    out = os.path.join(CODEX_CWD, f"out-{os.getpid()}-{threading.get_ident()}.txt")
    r = subprocess.run(["codex", "exec", "--ephemeral", "--skip-git-repo-check", "--ignore-user-config",
                        "-s", "read-only", "-m", model, "-c", f'model_reasoning_effort="{CODEX_EFFORT.get(effort, effort)}"',
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
    return text, dict(prompt_tokens=u.get("input_tokens", 0), completion_tokens=u.get("output_tokens", 0) +
                      u.get("reasoning_output_tokens", 0), backend="codex", model=model)


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


def work(ea, db, key, ladder, out_dir=None):
    """ladder: [(backend, model, attempts), ...] tried in order; the conversation (with gate errors) carries over."""
    packet = build(db, ea)
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
        if ok:
            os.replace(tmp, path)
            return dict(ea=ea, ok=True, attempts=attempt + 1, meta=meta, usage=usage_all, by=f"{backend}:{model}")
        msgs += [{"role": "assistant", "content": text},
                 {"role": "user", "content": f"The gate rejected the file:\n{err}\nFix it and answer again in the same format."}]
    qdir = os.path.join(out_dir, "queue") if out_dir else os.path.join(ROOT, "queue")
    os.makedirs(qdir, exist_ok=True)
    with open(os.path.join(qdir, f"{ea:08x}.md"), "w") as f:
        f.write(f"# Escalation 0x{ea:08X}\n\n## Last gate error\n```\n{err}\n```\n\n## Last answer\n{text}\n\n"
                f"## Packet\n{packet}\n")
    return dict(ea=ea, ok=False, attempts=len(steps), err=err[:500], usage=usage_all)


def select(db, n, max_size, min_size=6):
    """Leaf-first: fewest not-done game callees, then smallest. One representative per clone cluster."""
    rows = db.rows
    clones = json.load(open(os.path.join(ROOT, "export", "clones.json")))
    rep = {}
    for c in clones:
        for m in c["members"][1:]:
            rep[int(m, 16)] = int(c["members"][0], 16)
    eas = sorted(db.F)
    room = {eas[i]: eas[i + 1] - eas[i] for i in range(len(eas) - 1)}
    cands = []
    for ea, r in rows.items():
        if r["status"] != "todo" or r["region"] != "game" or ea in rep or r["module"] == "stl":  # stl: template impls
            continue
        f = db.F.get(ea)
        if not f or f["size"] > max_size or f["size"] < min_size or room.get(ea, 99) < 5 or not os.path.exists(os.path.join(ROOT, "export", "funcs", f"{ea:08x}.c")):
            continue
        if os.path.exists(os.path.join(ROOT, "queue", f"{ea:08x}.md")):
            continue
        pending = sum(1 for c in f["callees"] if rows.get(c, {}).get("status") == "todo")
        cands.append((pending, f["size"], ea))
    cands.sort()
    return [ea for _, _, ea in cands[:n]]


def smoke():
    r = subprocess.run([os.path.join(ROOT, "tools", "deploy.sh")], capture_output=True, text=True)
    if r.returncode:
        return False, "build/deploy failed:\n" + (r.stdout + r.stderr)[-3000:]
    r = subprocess.run([os.path.join(ROOT, "tools", "smoke.sh"), "30"], capture_output=True, text=True)
    return r.returncode == 0, r.stdout[-2000:]


def run(args):
    db = DB()
    key = load_key()
    targets = [int(x, 16) for x in args.only.split(",")] if args.only else select(db, args.n, args.max_size, args.min_size)
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
        futs = {ex.submit(work, ea, db, key, ladder_of(ea), out_dir): ea for ea in targets}
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
    paths = [p for p in ("src/auto", "db", "queue") if os.path.exists(os.path.join(ROOT, p))]
    subprocess.run(["git", "-C", ROOT, "add", "-A", *paths], check=True)
    msg = f"grunt batch: {len(good)} functions reimplemented ({args.backend})\n\nCo-Authored-By: Claude Opus 5.5 <noreply@anthropic.com>"
    subprocess.run(["git", "-C", ROOT, "commit", "-q", "-m", msg], check=False)
    print("committed", flush=True)


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
    sub.add_parser("cost")
    a = ap.parse_args()
    {"run": run, "cost": cost}[a.cmd](a)
