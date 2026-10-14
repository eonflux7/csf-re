# Global initializers: completed follow-up, 2026-10-14

**The remaining 189/189 are now finished offline: every literal gate invocation
printed `ok`, and every fresh-source emudiff invocation printed `same 8/8 profiles`.**
Together with the earlier 204, this completes the original 393-entry selection:
382 generated files and eleven manual files. No unfinished addresses/failing lines
remain in this task. Database/queue status and in-game installation are not changed
by this source completion.

## Follow-up verification

Last command over all 189 remaining entries (invokes both literal CLI tools, not
stale build objects):

```sh
.venv/bin/python tools/initfngen.py --check --jobs 6 --timeout 180 --results work/initfn-cli-final.json > work/initfn-cli-final.log 2>&1
```

Final line: `total 189, supported 188, passed 189`. All 189 gate outputs are exactly
`ok`; all 189 emulator comparisons cover 8/8 profiles. The earlier shared-cache
retry also passed all 189 (`work/initfn-finished.json`). Generation used
`tools/initfngen.py --write`; all 188 emitted files were compared against fresh
generator output. The generator itself is unchanged.

The original first-entry reproduction was:

```sh
.venv/bin/python tools/emudiff.py 008289f0 --src src/auto/008289f0.cpp -v
```

It returned `inconclusive cap x8` before the harness change. A block/event trace
shows `_atexit -> __onexit -> __lock -> __mtinitlocknum -> malloc`, with hundreds
of repeated mocked `HeapAlloc(..., 32)` events, rather than a loop in the tiny
initializer. Running process CRT startup machinery against the emulator's
uninitialized/profile-filled state is the blocker. `tools/emudiff.py` now treats
CRT `_atexit` (selected by symbol, not initializer address) as a cdecl, one-callback
boundary event. Its argument uses existing pointer normalization and is compared
exactly, without unread-argument/stack-garbage/float tolerances. Other CRT helpers
still execute normally. No gate or semantic-check policy was weakened.

Acceptance evidence:

* `.venv/bin/python tests/test_emudiff.py`: 24 tests pass, including the new
  registration-boundary/callback-identity/self-pointer-normalization regression.
* Unchanged-tool baseline:
  `.venv/bin/python tools/emumut.py run --jobs 6 --out work/emumut-before.json`.
* Replay:
  `.venv/bin/python tools/emumut.py run --jobs 6 --replay work/emumut-before.json --out work/emumut-after.json`.
* Comparison:
  `.venv/bin/python tools/emumut.py compare work/emumut-before.json work/emumut-after.json`.
  All 400 mutants pair with unchanged fingerprints. Both runs catch 292/400
  (73.0%), miss 108, and have zero mutant errors/inconclusives. No lost catches
  and no new control DIFF; controls remain 448 same / 12 inconclusive out of 460.
* Mutating `008289f0` to register `0082a5e0` instead of `0082a5c0`:
  `DIFF 8/8 profiles; profile 0: call 1 to 007aee43: arguments ['0082a5c0'] vs ['0082a5e0']`.
* Mutating its constructed global to `008afc68` instead of `008afc64`:
  `DIFF 8/8 profiles; profile 0: call 0 to 006b27e0: arguments ['008afc64'] vs ['008afc68']`.
  Mutant sources and full verbose outputs are in `work/initfn-wrong-*.cpp/log`.

`0082c404` is handwritten, without inline assembly. FXSAVE captures the raw CW;
native `_controlfp` supplies the waiting query and 24-bit/masked-exception API;
FXRSTOR applies the exact modified raw CW, including the denormal mask and
reserved bits, while preserving the rest of the FP state, including MXCSR. `_controlfp` alone is not
equivalent: its abstract flags omit raw reserved bits and cannot set the denormal
mask. Unlike the original FWAIT/FNSTCW/FLDCW sequence, this implementation requires
FXSR and uses native CRT calls and the normal void-cdecl volatile-register ABI.
No game or CPU-minimum compatibility test was performed. Startup order and native
CRT/data initialization requirements below still apply.

The remainder of this document is the **historical partial-completion record**;
its unfinished table describes the earlier attempt, not the current sources.

# Historical partial completion, 2026-10-14

**204 of 393 selected functions are finished: gate `ok` and fresh-source emudiff `same`.**
194 are generated, ten are by hand. The 188 atexit initializers and one x87-control-word helper remain unfinished, and have no files under `src/auto/`. This is not standalone startup readiness.

Selection is exactly the requested game/todo range `[008289f0,0082c410)`: 393 functions, 9,873 original bytes, maximum 355 bytes. PE bytes were decoded with Capstone using the exported function/chunk boundaries. The classification was performed before source generation (initial coarse groups were then refined into the table below). Shared chunks are not new library implementations.

## Shape table

Columns are disjoint completed generated / completed by hand / unfinished, not attempted generation counts.

| Shape | Count | Generated | By hand | Unfinished |
|---|---:|---:|---:|---:|
| cdecl construct + atexit | 106 | 0 | 0 | 106 |
| atexit only | 13 | 0 | 0 | 13 |
| x87 helper | 8 | 0 | 7 | 1 |
| thiscall construct + atexit | 36 | 0 | 0 | 36 |
| compound initializer | 22 | 0 | 0 | 22 |
| array loop | 3 | 2 | 0 | 1 |
| construct + vtable override + atexit | 8 | 0 | 0 | 8 |
| global stores + atexit | 2 | 0 | 0 | 2 |
| packed byte constant | 5 | 5 | 0 | 0 |
| global destructor tail | 151 | 149 | 2 | 0 |
| compound destructor / helper | 30 | 29 | 1 | 0 |
| empty destructor | 5 | 5 | 0 | 0 |
| global vtable store | 4 | 4 | 0 | 0 |
| **Total** | **393** | **194** | **10** | **189** |

## Verified commands and evidence

Last all-files command:

```sh
.venv/bin/python tools/initfngen.py --check --shared --prior work/initfn/combined.json --results work/initfn/final.json > work/initfn/final.log 2>&1
```

Final line: `total 393, supported 382, passed 204`. This command freshly recompiles and runs BOTH `gate.gate` and `emudiff.check(src=...)` for all 204 retained files. The other 189 have no sources: their earlier failures are retained by `--prior`, NOT newly rechecked or credited as passes. `supported` counts generator shapes, not completion (ten manual files are also checked).

199 have 8/8 compared profiles. Five have partial coverage: `0082ab20` 4/8, `0082adf0` 5/8, `0082b680` 2/8, `0082c000` 4/8, `0082c07f` 4/8. The other profiles are inconclusive, not passes. These checks do not prove game execution, every caller ABI, or bit-identical x87 rounding outside the emulator's float-store contract.

Earlier full-set command:

```sh
.venv/bin/python tools/initfngen.py --check --jobs 8 --timeout 20 --results work/initfn/check-all.json > work/initfn/check-all.log 2>&1
```

It attempted both literal CLI tools for every candidate. Process/index setup caused false timeouts even for destructor thunks. Every completed emulator comparison whose gate timed out was subsequently retried with unbounded shared caches; all surviving files passed again in the last command above. First representative `008289f0` also completed an unbounded gate (`ok`), followed by a separate fresh-source emulator comparison returning `inconclusive cap x8` (`work/initfn/check-first.log`), NOT `same`.

Additional checks: `.venv/bin/python -m py_compile tools/initfngen.py`; `git diff --check`; exact regenerated-text comparison for all 194 generated survivors; check that every surviving `CSF_ADDR_FN` denotes a real exported function start. Explicit library-address input (`--write 007aee43`) was rejected without emitting a source. No module assumption blocked the gate: the completed functions have no CSV module and use hook group `initfn`.

Raw classification, complete diagnostics, ordered table, callback edges, and rejected candidate text are under `work/initfn/` (scratch, not repository metadata). Durable unfinished addresses and reasons are below.

## Changed files / generator behavior

* Added `tools/initfngen.py`, 194 generated `src/auto/*.cpp`, ten manual `src/auto/*.cpp`, and this document.
* Database, queue, exports, PLAN, JOURNAL, all existing tools, hook headers, and boundary code are unchanged.
* Generator uses a closed instruction whitelist, snapshotting pushed values at their original positions, accounting for callee/caller stack cleanup, and preserving store/call/branch/loop order. Unsupported operands, register conventions, non-function tails, or unsafe five-byte patch spans are rejected. Its bindings come from the same canonical API used by packets; `String`/`Char` aliases match the existing string implementation.
* Addresses are expressed through CSF macros; known vtables have VT bindings and known strings have STR bindings. Simple destructor tails preserve untouched caller registers/stack in a naked indirect-jump thunk.
* `0082a930` collapses the shared unconditional jump label `00672640` to actual callee `00653840`; there is no nonexistent standalone function slot for the shared label. The two game-owned shared destructor bodies reached by `0082ad80` and `0082ad90` are implemented in C++, also without fabricated function slots.
* Manual: `00828a90`, `0082ad80`, `0082ad90`, `0082adf0`, and the six matrix/vector helpers `0082c000`, `0082c07f`, `0082c0ef`, `0082c160`, `0082c1c4`, `0082c2a0`.
* `--write` never overwrites files. `--check` removes failed candidates, retaining rejected text in scratch. `--shared` uses the actual tool functions and fresh source compilation, with one worker and no timeout; use it for retries of already-completed comparisons rather than to hide inconclusive verdicts.

## Initialization order and startup requirements (static conclusions, NOT runtime verified)

Static image/source inspection (not runtime verification):
__xc table [00898000,00898450) is 276 dwords: 196 nonzero entries; first
00898004 -> CRT 007B90C8 ___security_init_cookie, then 195 ascending game
entries 008289F0..0082A590. Do not rebuild startup by executing all 393
in address order: 188 callbacks are registered by these initializers and
must execute once, in reverse registration order on exit. One callback,
0082A670, is already done and outside this todo task. 187 callbacks belong
to the 393 selection. Eleven other selected entries are helpers/unregistered
cleanup/math/control-word routines. Full table and registration edges:
work/initfn/xc-order.json, work/initfn/callbacks.json.

Early allocation: 00828AB0 initializes g_BlockList4 at 008B02E8 via
00471340 -> 0069B250 -> 0069B100 -> 006910F0 -> 00690DC0. It occurs
BEFORE allocator object construction 00829420 (008BAD98 via 0069DD50)
and mutex construction 00829440 (008BAEB4 via 0069E3E0). Zero handle is
explicitly accepted by 0069E3A0; zero field_0118 sends 0069DEF0 to CRT
malloc instead of pools. Do not prematurely enable pools or require a live
mutex for these early allocations. The original 0069DD50 resets allocator
state later. 00829460 constructs and allocates the ReferenceListPool and
PairBlockPool (008BAEC0,008BAF04), then registers their cleanup.

NetZ global node creators 0066B390,007AA3C0 allocate 24-byte sentinel
nodes through 00647070, before/after allocator construction respectively;
initializers 008290B0 and 0082A500 immediately dereference their returned
pointers, set byte +0x15 and self-links at +0,+4,+8. A null-return NetZ
stub cannot substitute these. Other node factories 0066D400,006882D0,
00796490,0079C090 feed additional global containers. No library code was
reimplemented here; boundary implementations must provide these objects.

CSF data must be initialized, imports/vtables/addresses fixed up, and CRT
atexit/allocation functional before invoking the original game table.
Generated callbacks use CSF_ADDR_FN, not independent native C++ statics.
0082C404 loads the x87 control word after updating 008C1000/008C1004;
a scalar-store-only approximation is not correct. It is not a table entry.

The CRT entry `007b90c8` is a library security-cookie initializer, not another game function to reimplement; the standalone native CRT must provide its own equivalent startup. Original game initialization is explicit and must follow native binding initialization/data fixup, not arbitrary C++ translation-unit order. Callbacks must not also be eagerly invoked as initializers.

## Still open / why files were left out

188 atexit initializers did not produce the required emulator `same`. Their bounded CLI attempts timed out in both tools; the full first representative produced `cap x8`. Inspection shows emudiff executes original CRT code, including atexit/onexit and its CRT locking machinery, against profile-filled state. This is a likely harness/startup-state blocker, NOT proof that all 188 bodies are correct, nor a measured claim that every one yields precisely `cap x8`. Existing tools were not changed, as instructed. The generator can emit their recognized C++ candidates for a future retry after the orchestrator resolves the check environment.

`0082c404` executes `fwait`/`fldcw`, copying `008c1000` to `008c1004`, masking/or-ing the stored control word, and loading it into x87. No scalar-only approximation was kept. The generator rejects `0082c405: wait : instruction wait`; the gate's permitted naked-tail asm whitelist does not allow an x87 control-word body. An approved primitive/boundary or gate policy is still needed. No candidate was submitted to the gate for this helper, so this is a generator rejection / inspected restriction, not a claimed gate-run failure.

### Every unfinished address and failing line

For the 188 entries below, the observed line in BOTH CLI checks was `timeout after 20s (no verdict)`. They are absent from `src/auto/`. No timeout is counted as `same`.

| Address | Failing line |
|---|---|
| `008289f0` | gate and emudiff: timeout after 20s (no verdict) |
| `00828a10` | gate and emudiff: timeout after 20s (no verdict) |
| `00828a30` | gate and emudiff: timeout after 20s (no verdict) |
| `00828a40` | gate and emudiff: timeout after 20s (no verdict) |
| `00828a60` | gate and emudiff: timeout after 20s (no verdict) |
| `00828a80` | gate and emudiff: timeout after 20s (no verdict) |
| `00828aa0` | gate and emudiff: timeout after 20s (no verdict) |
| `00828ab0` | gate and emudiff: timeout after 20s (no verdict) |
| `00828ad0` | gate and emudiff: timeout after 20s (no verdict) |
| `00828ae0` | gate and emudiff: timeout after 20s (no verdict) |
| `00828af0` | gate and emudiff: timeout after 20s (no verdict) |
| `00828b10` | gate and emudiff: timeout after 20s (no verdict) |
| `00828ba0` | gate and emudiff: timeout after 20s (no verdict) |
| `00828bc0` | gate and emudiff: timeout after 20s (no verdict) |
| `00828be0` | gate and emudiff: timeout after 20s (no verdict) |
| `00828c40` | gate and emudiff: timeout after 20s (no verdict) |
| `00828ca0` | gate and emudiff: timeout after 20s (no verdict) |
| `00828cc0` | gate and emudiff: timeout after 20s (no verdict) |
| `00828ce0` | gate and emudiff: timeout after 20s (no verdict) |
| `00828d00` | gate and emudiff: timeout after 20s (no verdict) |
| `00828d20` | gate and emudiff: timeout after 20s (no verdict) |
| `00828d40` | gate and emudiff: timeout after 20s (no verdict) |
| `00828da0` | gate and emudiff: timeout after 20s (no verdict) |
| `00828dd0` | gate and emudiff: timeout after 20s (no verdict) |
| `00828df0` | gate and emudiff: timeout after 20s (no verdict) |
| `00828e30` | gate and emudiff: timeout after 20s (no verdict) |
| `00828e50` | gate and emudiff: timeout after 20s (no verdict) |
| `00828eb0` | gate and emudiff: timeout after 20s (no verdict) |
| `00828f00` | gate and emudiff: timeout after 20s (no verdict) |
| `00828f20` | gate and emudiff: timeout after 20s (no verdict) |
| `00828f80` | gate and emudiff: timeout after 20s (no verdict) |
| `00828fd0` | gate and emudiff: timeout after 20s (no verdict) |
| `00829030` | gate and emudiff: timeout after 20s (no verdict) |
| `00829040` | gate and emudiff: timeout after 20s (no verdict) |
| `00829060` | gate and emudiff: timeout after 20s (no verdict) |
| `00829080` | gate and emudiff: timeout after 20s (no verdict) |
| `008290b0` | gate and emudiff: timeout after 20s (no verdict) |
| `008290f0` | gate and emudiff: timeout after 20s (no verdict) |
| `00829110` | gate and emudiff: timeout after 20s (no verdict) |
| `00829140` | gate and emudiff: timeout after 20s (no verdict) |
| `00829150` | gate and emudiff: timeout after 20s (no verdict) |
| `00829180` | gate and emudiff: timeout after 20s (no verdict) |
| `008291e0` | gate and emudiff: timeout after 20s (no verdict) |
| `00829240` | gate and emudiff: timeout after 20s (no verdict) |
| `00829270` | gate and emudiff: timeout after 20s (no verdict) |
| `00829290` | gate and emudiff: timeout after 20s (no verdict) |
| `008292b0` | gate and emudiff: timeout after 20s (no verdict) |
| `008292e0` | gate and emudiff: timeout after 20s (no verdict) |
| `00829300` | gate and emudiff: timeout after 20s (no verdict) |
| `00829320` | gate and emudiff: timeout after 20s (no verdict) |
| `00829340` | gate and emudiff: timeout after 20s (no verdict) |
| `00829360` | gate and emudiff: timeout after 20s (no verdict) |
| `00829380` | gate and emudiff: timeout after 20s (no verdict) |
| `008293a0` | gate and emudiff: timeout after 20s (no verdict) |
| `008293c0` | gate and emudiff: timeout after 20s (no verdict) |
| `008293e0` | gate and emudiff: timeout after 20s (no verdict) |
| `00829400` | gate and emudiff: timeout after 20s (no verdict) |
| `00829420` | gate and emudiff: timeout after 20s (no verdict) |
| `00829440` | gate and emudiff: timeout after 20s (no verdict) |
| `00829460` | gate and emudiff: timeout after 20s (no verdict) |
| `008294b0` | gate and emudiff: timeout after 20s (no verdict) |
| `008294c0` | gate and emudiff: timeout after 20s (no verdict) |
| `00829570` | gate and emudiff: timeout after 20s (no verdict) |
| `00829590` | gate and emudiff: timeout after 20s (no verdict) |
| `008295b0` | gate and emudiff: timeout after 20s (no verdict) |
| `008295d0` | gate and emudiff: timeout after 20s (no verdict) |
| `008295f0` | gate and emudiff: timeout after 20s (no verdict) |
| `00829610` | gate and emudiff: timeout after 20s (no verdict) |
| `00829630` | gate and emudiff: timeout after 20s (no verdict) |
| `00829650` | gate and emudiff: timeout after 20s (no verdict) |
| `00829660` | gate and emudiff: timeout after 20s (no verdict) |
| `00829680` | gate and emudiff: timeout after 20s (no verdict) |
| `008296a0` | gate and emudiff: timeout after 20s (no verdict) |
| `008296c0` | gate and emudiff: timeout after 20s (no verdict) |
| `008296e0` | gate and emudiff: timeout after 20s (no verdict) |
| `00829700` | gate and emudiff: timeout after 20s (no verdict) |
| `00829720` | gate and emudiff: timeout after 20s (no verdict) |
| `00829740` | gate and emudiff: timeout after 20s (no verdict) |
| `00829760` | gate and emudiff: timeout after 20s (no verdict) |
| `00829780` | gate and emudiff: timeout after 20s (no verdict) |
| `008297a0` | gate and emudiff: timeout after 20s (no verdict) |
| `008297c0` | gate and emudiff: timeout after 20s (no verdict) |
| `008297e0` | gate and emudiff: timeout after 20s (no verdict) |
| `00829800` | gate and emudiff: timeout after 20s (no verdict) |
| `00829820` | gate and emudiff: timeout after 20s (no verdict) |
| `00829840` | gate and emudiff: timeout after 20s (no verdict) |
| `00829860` | gate and emudiff: timeout after 20s (no verdict) |
| `00829880` | gate and emudiff: timeout after 20s (no verdict) |
| `008298a0` | gate and emudiff: timeout after 20s (no verdict) |
| `008298c0` | gate and emudiff: timeout after 20s (no verdict) |
| `008298e0` | gate and emudiff: timeout after 20s (no verdict) |
| `00829900` | gate and emudiff: timeout after 20s (no verdict) |
| `00829920` | gate and emudiff: timeout after 20s (no verdict) |
| `00829940` | gate and emudiff: timeout after 20s (no verdict) |
| `00829960` | gate and emudiff: timeout after 20s (no verdict) |
| `00829980` | gate and emudiff: timeout after 20s (no verdict) |
| `008299a0` | gate and emudiff: timeout after 20s (no verdict) |
| `008299c0` | gate and emudiff: timeout after 20s (no verdict) |
| `008299e0` | gate and emudiff: timeout after 20s (no verdict) |
| `00829a00` | gate and emudiff: timeout after 20s (no verdict) |
| `00829a20` | gate and emudiff: timeout after 20s (no verdict) |
| `00829a40` | gate and emudiff: timeout after 20s (no verdict) |
| `00829a60` | gate and emudiff: timeout after 20s (no verdict) |
| `00829a80` | gate and emudiff: timeout after 20s (no verdict) |
| `00829aa0` | gate and emudiff: timeout after 20s (no verdict) |
| `00829ac0` | gate and emudiff: timeout after 20s (no verdict) |
| `00829ae0` | gate and emudiff: timeout after 20s (no verdict) |
| `00829b00` | gate and emudiff: timeout after 20s (no verdict) |
| `00829b20` | gate and emudiff: timeout after 20s (no verdict) |
| `00829b40` | gate and emudiff: timeout after 20s (no verdict) |
| `00829b60` | gate and emudiff: timeout after 20s (no verdict) |
| `00829b80` | gate and emudiff: timeout after 20s (no verdict) |
| `00829ba0` | gate and emudiff: timeout after 20s (no verdict) |
| `00829bc0` | gate and emudiff: timeout after 20s (no verdict) |
| `00829be0` | gate and emudiff: timeout after 20s (no verdict) |
| `00829c00` | gate and emudiff: timeout after 20s (no verdict) |
| `00829c20` | gate and emudiff: timeout after 20s (no verdict) |
| `00829c40` | gate and emudiff: timeout after 20s (no verdict) |
| `00829c60` | gate and emudiff: timeout after 20s (no verdict) |
| `00829c80` | gate and emudiff: timeout after 20s (no verdict) |
| `00829ca0` | gate and emudiff: timeout after 20s (no verdict) |
| `00829cc0` | gate and emudiff: timeout after 20s (no verdict) |
| `00829ce0` | gate and emudiff: timeout after 20s (no verdict) |
| `00829d00` | gate and emudiff: timeout after 20s (no verdict) |
| `00829d20` | gate and emudiff: timeout after 20s (no verdict) |
| `00829d40` | gate and emudiff: timeout after 20s (no verdict) |
| `00829d60` | gate and emudiff: timeout after 20s (no verdict) |
| `00829d80` | gate and emudiff: timeout after 20s (no verdict) |
| `00829da0` | gate and emudiff: timeout after 20s (no verdict) |
| `00829dc0` | gate and emudiff: timeout after 20s (no verdict) |
| `00829dd0` | gate and emudiff: timeout after 20s (no verdict) |
| `00829de0` | gate and emudiff: timeout after 20s (no verdict) |
| `00829e00` | gate and emudiff: timeout after 20s (no verdict) |
| `00829e20` | gate and emudiff: timeout after 20s (no verdict) |
| `00829e40` | gate and emudiff: timeout after 20s (no verdict) |
| `00829e60` | gate and emudiff: timeout after 20s (no verdict) |
| `00829e80` | gate and emudiff: timeout after 20s (no verdict) |
| `00829ea0` | gate and emudiff: timeout after 20s (no verdict) |
| `00829ec0` | gate and emudiff: timeout after 20s (no verdict) |
| `00829f00` | gate and emudiff: timeout after 20s (no verdict) |
| `00829f20` | gate and emudiff: timeout after 20s (no verdict) |
| `00829f40` | gate and emudiff: timeout after 20s (no verdict) |
| `00829f60` | gate and emudiff: timeout after 20s (no verdict) |
| `00829f80` | gate and emudiff: timeout after 20s (no verdict) |
| `00829fa0` | gate and emudiff: timeout after 20s (no verdict) |
| `00829fc0` | gate and emudiff: timeout after 20s (no verdict) |
| `00829fe0` | gate and emudiff: timeout after 20s (no verdict) |
| `0082a000` | gate and emudiff: timeout after 20s (no verdict) |
| `0082a020` | gate and emudiff: timeout after 20s (no verdict) |
| `0082a040` | gate and emudiff: timeout after 20s (no verdict) |
| `0082a060` | gate and emudiff: timeout after 20s (no verdict) |
| `0082a080` | gate and emudiff: timeout after 20s (no verdict) |
| `0082a0a0` | gate and emudiff: timeout after 20s (no verdict) |
| `0082a0c0` | gate and emudiff: timeout after 20s (no verdict) |
| `0082a0e0` | gate and emudiff: timeout after 20s (no verdict) |
| `0082a100` | gate and emudiff: timeout after 20s (no verdict) |
| `0082a120` | gate and emudiff: timeout after 20s (no verdict) |
| `0082a140` | gate and emudiff: timeout after 20s (no verdict) |
| `0082a160` | gate and emudiff: timeout after 20s (no verdict) |
| `0082a180` | gate and emudiff: timeout after 20s (no verdict) |
| `0082a1a0` | gate and emudiff: timeout after 20s (no verdict) |
| `0082a1c0` | gate and emudiff: timeout after 20s (no verdict) |
| `0082a1e0` | gate and emudiff: timeout after 20s (no verdict) |
| `0082a200` | gate and emudiff: timeout after 20s (no verdict) |
| `0082a220` | gate and emudiff: timeout after 20s (no verdict) |
| `0082a240` | gate and emudiff: timeout after 20s (no verdict) |
| `0082a260` | gate and emudiff: timeout after 20s (no verdict) |
| `0082a280` | gate and emudiff: timeout after 20s (no verdict) |
| `0082a2a0` | gate and emudiff: timeout after 20s (no verdict) |
| `0082a2b0` | gate and emudiff: timeout after 20s (no verdict) |
| `0082a2d0` | gate and emudiff: timeout after 20s (no verdict) |
| `0082a2f0` | gate and emudiff: timeout after 20s (no verdict) |
| `0082a310` | gate and emudiff: timeout after 20s (no verdict) |
| `0082a330` | gate and emudiff: timeout after 20s (no verdict) |
| `0082a360` | gate and emudiff: timeout after 20s (no verdict) |
| `0082a380` | gate and emudiff: timeout after 20s (no verdict) |
| `0082a3a0` | gate and emudiff: timeout after 20s (no verdict) |
| `0082a3d0` | gate and emudiff: timeout after 20s (no verdict) |
| `0082a400` | gate and emudiff: timeout after 20s (no verdict) |
| `0082a420` | gate and emudiff: timeout after 20s (no verdict) |
| `0082a440` | gate and emudiff: timeout after 20s (no verdict) |
| `0082a470` | gate and emudiff: timeout after 20s (no verdict) |
| `0082a4a0` | gate and emudiff: timeout after 20s (no verdict) |
| `0082a4d0` | gate and emudiff: timeout after 20s (no verdict) |
| `0082a500` | gate and emudiff: timeout after 20s (no verdict) |
| `0082a540` | gate and emudiff: timeout after 20s (no verdict) |
| `0082a570` | gate and emudiff: timeout after 20s (no verdict) |
| `0082a590` | gate and emudiff: timeout after 20s (no verdict) |
| `0082c404` | generator: 0082c405: wait : instruction wait; no gate candidate |
