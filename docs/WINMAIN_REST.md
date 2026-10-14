# Remaining WinMain slots: binary findings

Worker scope: CRT wide entry points, two shared NetZ getters, zlib allocation,
and three targets absent from functions.csv. No Windows execution.

## Supplied slots

* **007B02E2**: cdecl wide fopen wrapper. Original pushes sharing flag 0x40
  (`_SH_DENYNO`), mode and filename, calls 007B0286, cleans 12 bytes and returns.
  Boundary forwards to host `_wfopen`, retaining wide paths/modes and default
  sharing; no lossy narrow conversion.
* **007B00E9**: cdecl `_wsystem`, forwarded to host `_wsystem`, including its
  null-command availability query and wide command processor environment.
* **006683F0**, **00652010**: `mov eax,[ecx+4/5c]; ret` respectively.
  Register boundary returns the exact borrowed pointer, including null; no
  allocation, ownership change, invented scene or networking-success result.
* **006A2F90**: cdecl `zalloc(opaque, uint32_t items, uint32_t size)`;
  **opaque is ignored**, not a this pointer. 006A30A0 and 006A3150 install it at
  stream+0x20. Binary 006A1961..006A1969 pushes size=24, items=1, stream+0x28
  and caller-cleans 12 bytes. The callback computes a wrapping 32-bit product,
  reuses the first available exact-size record, or allocates and appends a
  12-byte record (even a failed/null allocation). A null reusable record is
  marked busy before falling through to allocation. Growth can relocate the
  array, so the table pointer/count are reloaded afterward. Only the low byte
  of the final record word is defined by the binary; the upper three bytes
  are uninitialized stack padding and are intentionally not reproduced.
  The queue crash's address 0x79 is consistent with an invalid opaque/ABI
  interpretation, but the rejected source is absent: this is not a proven
  diagnosis of that crash and requires a new Windows verifier run.

## Targets IDA missed

All three have independently balanced entry/return paths and are not jump
thunks. None lies inside any chunk recorded in export/disasm/*.json. They
are address-taken callbacks; initreach follows CSF_ADDR_FN as well as declared
calls, so its displayed paths do **not** establish direct invocation.

| Address / extent (exclusive) | Binary and registration contract | Standalone disposition |
|---|---|---|
| **0050F920..0050F972** (82 bytes) | Game cdecl load-complete callback, no stack args. 0050FBD0 takes its address and registers through 006973B0. If manager+F0 is nonnull, invokes box virtual +40, reloads box, copies box+78 to manager+F4, clears manager+F0, reloads global manager, and dispatches 0050F790 with `(0x47323037,8,0x1800,0)`. | Real address-keyed implementation in standalone/game_plain.cpp, not a success/no-op stub. Hook build leaves the original callback in place. |
| **0057ED00..0057ED96** (150 bytes) | RW particle emitter standard-create callback. cdecl; emitter is stack argument 2 (`push esi; mov esi,[esp+0xc]`); other arguments unused. Calls 0057C8B0 on emitter-class+10 with property ID 1. If offset >=0, initializes emitter property defaults and flags. Returns emitter. Registered by 006E0670 and 006E0920 at callback-array+14. | RenderWare particle boundary work, **not game code**. Keep fatal slot until that boundary owns the contract; returning an emitter without initialization is unsafe. |
| **0057F0B0..0057F0F0** (64 bytes) | RW particle emitter end-update callback. Same cdecl second-argument emitter ABI. If emitter+10 >0, looks up property ID 6 through 0057C8B0; missing property returns null. A positive property+8 invokes property+18's callback at +48 with that object as a cdecl argument; failure returns null. Otherwise returns emitter. Registered by 006E0670/006E0920 at callback-array+0C and/or +38. | RenderWare/PTank boundary work. Keep fatal slot: a blanket identity/no-op loses failure and callback semantics. |

The latter two lie in the existing **rw** region 00566000..005A1D50.
`not-a-function` is an inventory classification artifact, not evidence they
are game functions or harmless addresses. No RW library implementation was
copied or reimplemented here.

### Metadata follow-up for the orchestrator

Worker instructions prohibit modifying db/ or IDA. Add function facts for
0050F920 (game, size 82), 0057ED00 (rw, size 150), 0057F0B0 (rw, size 64),
then perform the normal IDA/export sync. The descriptive callback names above
are inferred from registration and behavior, not recovered symbol names.
RW slots remain deliberately unresolved pending the particle boundary job.

## Offline test scope

`tests/wmrest_offline.py` executes original bytes and compiled COFF with explicit
allocation/growth/virtual/dispatch doubles. It compares six allocator cases
(reuse/skip/null/growth/wrap/failure) and three game-callback cases (no box,
box, box replaced by virtual), defined state, ordered calls, cdecl stack and
nonvolatile registers. It deliberately excludes undefined record padding.
This is not an integration or in-game test. Actual run results are in the
worker report; logs/disassembly are under work/wmrest-review/.

Resumed-worker results: gate passes for 006A2F90; linktest compiles
14799/14799 files, no failures/duplicates/raw image addresses. NetZ COFF
checks pass (218 cleanup, 196 entry sequences), and test_offline.py passes
607 calls. The targeted binary/COFF test passes all nine cases. Both
csf_standalone.exe and dinput8.dll link. Stock emudiff remains inconclusive
(`UC_ERR_WRITE`, eight cases); the explicit heap/table fixtures above are
the available equivalence evidence, not a claimed stock emudiff pass.

After regenerating actual-link traps, initreach from 00509250 reaches 1217
addresses: 58 rw traps and the two unclassified RW callbacks above; **no
CRT, NetZ or game trap** in that static walk. All six supplied addresses
are absent from the regenerated trap inventory. This does not bound
computed/virtual calls or prove successful startup.
