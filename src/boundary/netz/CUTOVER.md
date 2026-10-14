# Second NetZ boundary audit — partial completion

Follow-up: [INIT_CUTOVER.md](INIT_CUTOVER.md) records the separate **46/46
initializer-slot** cutover. The below is the historical second audit; its totals
are not current. NetZ's overall online cutover remains deliberately incomplete.

Finding: 29 further demanded slots can be covered without invented universal
objects. 193 remain unresolved. This is **not** completion of the requested NetZ
cutover or evidence that single player starts. In particular the target set
contains non-network helpers called from movement, resource lookup, collision,
rendering, diagnostics, and exit registration, not just Vorbis imports.

## Supplied family counts and behaviour

| Family | Generated count | Offline answer |
|---|---:|---|
| Byte/boolean network operations | 63 | false/rejected |
| Void network operations | 20 | no-op |
| Explicit count/predicate | 4 | zero |
| Last-result | 1 | nonzero error (-1) |
| Owned String primitives | 4 | real owned length-prefix storage |
| Gathering base constructor | 1 | 0x30-byte base; eight private virtual slots |
| Newly audited support | 20 | ownership/layout-preserving offline data (below); includes one corrected old no-op |
| Newly audited rejections | 10 | false/zero; no successful request or callback |
| Allocator-owned (not generated here) | 5 | existing game heap shim |
| **Total supplied** | **128** | **123 generated + 5 allocator-owned** |

The 20 reviewed support entries comprise: allocator token (1), circular list
head/node/clear/count (6), owned String output (2), error scratch (1), Gathering
ownership/name/type (3), terminal failed request object (1), base connection (1),
session String primitives (3), and borrowed endpoints (2). The session String
destructor replaces an old emitted no-op, so these supply 19 newly resolved slots.
Exact addresses,
stack cleanup, caller aliases, owner functions and use lines are in manifest.json;
reviewed offset/ownership evidence is in audit.json.

Ten rejections: 00799220 StartRequest; 00798FB0 LoadResource; 00798FF0 gathering
resource initiation; 0079FF30 gathering transition predicate; 00648770 endpoint
configure; 00658240 endpoint value; 0068F740 endpoint attribute; 00676700 prudp mode
command; 006575D0 connection completion; 00799020 gathering enumeration.
Their callers either inspect the low byte for failure, discard the
setter result, or forward status. **None is represented as proven MP-only.**
A net-prefixed name, or a direct network caller, is not a complete menu/session
reachability proof. The manifest preserves this distinction.

## Dereference and lifetime checklist for later game checking

* **0066A440 request:** every direct caller supplies 112 bytes. 006a7520,
  006a83f0, 006a8ed0, 006a90b0, 006a9c30 read +0x0c before any null check:
  1 means pending (wait), 2 successful, 3 failed. Offline uses 3, so no pending
  wait is deliberately introduced. +0 vtable has exactly six slots (next image
  word is text); cleanup is 4/0/0/0/8/0. Slot 0 receives delete flags and frees
  through the same game heap as allocation. Error scratch at +0x44/+0x48/+0x4c
  is failed status/borrowed diagnostic/line. No embedded allocation survives.
  006a8bf0/006a8d00/006a8320 delete slot 0 immediately on rejected initiation.
* **00656DE0/00653E90 list heads:** caller stores return into list+4; list+8
  is count; head+0 next and +4 previous must both point to the head when empty.
  006a7060 stores at owner+0x64 (count +0x68); 006a9f10 at owner+0x1c
  (count +0x20). 006aa900 traverses head+0. 006aa970/006aaae0 read head+0/+4,
  insert nodes and explicitly free sentinel via 006470e0 after 00652810 clears
  nodes. Node+8 owns an empty URL String. Plain-list 006508a0 frees node only;
  String-list 00652810 frees both payload and node. Neither clear frees head.
* **0066FBB0 URL output:** 006aa970/006aaae0 dereference returned pointer,
  then output+0 to obtain char data for fprintf. Return is the supplied stack
  output, initialized with valid owned empty text. Caller frees via 0064A6F0.
* **0064A8F0 format output:** first cdecl argument is an initialized String,
  replaced with owned empty text. All 19 caller bindings discard EAX; actual
  output is read afterwards (e.g. 0078cea0) and destroyed via 0064A6F0. Diagnostic
  formatting is intentionally lost. Never return a static String that callers
  would free as length-prefixed heap data.
* **00798E80 Gathering name:** +0x24 embedded owned String; 006a8bf0 and
  006a9660 immediately destroy their temporary input, hence a deep copy is
  necessary. 00798E40 frees +0x28 and +0x24; 006a8550 optionally frees the whole
  caller-owned object afterwards. 00798E60 updates +0x14/+0x18. 006a9660 also
  touches the game-derived object's +0x10/+0x12, +0x1c, +0x30..+0x54 and +0x254;
  caller allocates 600 bytes and replaces the base vtable with its game vtable.
  The offline base constructor only initializes its own 0x30-byte base.
* **00657590 result:** 006a7060 supplies embedded +0x94/+0xa0; 006a8780 supplies
  a 12-byte stack buffer. +0 status, +4 borrowed diagnostic, +8 line. These are
  not constructors of polymorphic objects; no free of the static diagnostic.
* **00646FF0 allocator token:** observed callers pass result to existing alloc/free
  shims, which ignore identity. Token is static, not owned/freed by caller.
  Existing malformed heap caller declarations (notably 006a7000) are not fixed.
* **00651770 connection:** 0073cb80/0073d350 allocate exactly 92 bytes and store
  the returned object at owner+0xcc/+0xd0. They read +0xc and call delete slot 0.
  0073d550 copies +0x44/+0x48/+0x4c into 12-byte result scratch. Offline supplies
  state 3, failed status, borrowed diagnostic and zero line. Six slots at original
  00858c3c end before text at 00858c54; cleanup 4/0/0/0/8/0, independently proved
  before sharing request virtual implementations. Never initialize 112 bytes in
  this 92-byte allocation. Its embedded lists are not allocated or used by the
  private offline virtuals; this does not emulate original refcount/list methods.
* **0066F130/0066F150/0066F180 session Strings:** 0073d2a0/0073d410 construct,
  compare, destroy a stack String each iteration; 0073dc30 constructs and destroys
  around StartSession. 0073cb80/0073d350 assign into session+0x160 on their success
  branches. Self+0 owns char data, data-4 length, same game heap as other Strings.
  The old 0066f150 no-op would leak these new owned results; now it frees them.
* **006486C0/006486D0 endpoints:** 0073c530 unconditionally reads +0 vtable and
  calls slot +4; 0073c5e0 passes pointers to rejecting setters. Original getters
  return transport-internal objects at +0x1a8/+0x1c4 after loading transport+0xc.
  Original endpoint constructor 00648720 writes 28 bytes, vtable 00858244, flag
  +4=0, +8=0, +0xc=-1, +0x10/+0x14/+0x18=0. Two slots (ret 4/0), then text.
  Return two distinct borrowed static objects with that layout; offline reset
  rejects/no-ops and deleting destructor does not free process-owned memory.
  Registry getter 006487d0 remains unresolved, so this is not a runnable chain.

## Remaining 193: caller-based triage, not completed data-flow proof

| Triage group | Count | Why unresolved |
|---|---:|---|
| Shared/non-network direct game callers | 53 | failure can corrupt SP; must audit/reassign boundary, not fabricate network object |
| Network constructors/layout/ownership | 23 | embedded object/virtual/output contracts still unaudited |
| Network pointer-like results | 33 | dereference/sentinel/ownership not proved |
| Network operation or integer result | 75 | integer may be pointer/ID/status; not proven MP-only, no guessed failure |
| Audio imports | 6 | actual Vorbis entry points; must go to audio boundary |
| Exit callbacks | 3 | atexit references missed by earlier CSF_FUNC-only caller scan |

Manifest enumerates every address under these groups and preserves every caller
binding. Grouping is triage: for example an integer-typed result may still be a
pointer, and the 53 shared-caller entries are not necessarily wrongly attributed
library bodies (COMDAT/inlined utilities may be shared). No db region was changed.

Concrete reasons not to blindly finish all 321:

* **00652010:** binary is `mov eax,[ecx+0x5c]; ret`, not network initialization.
  00417020 movement/collision obtains a scene controller;
  dozens of scene/AI/gameplay callers use it. **00652020** clump manager and
  **00652160/006683F0/00686C10** object/provider lookup similarly appear across
  gameplay. Null/dummy NetZ objects are not substitutes for those SP objects.
* **00672AC0:** 00464510 computes a time-indexed scene-entry lookup, not a lobby
  answer. **0068E300** is used in spatial vector calculations; binary loads three
  floats and computes their squared length on x87. Zeroing shared
  scalar utilities is not safe SP cutover policy.
* **0081B990/0081BA20/0081C0B0/0081CA00:** 006b23e0 loads collision shapes from
  RW streams; **0081BCE0/0081CFD0** are imported 2D object pairs in 006d6fc0.
  These need real rendering/resource boundary behaviour, not offline networking.
* **006487D0:** indirect dispatch through global 008b99cc has no statically proved
  cleanup. The now supplied endpoints do not resolve the registry/transport
  ownership problem; returning null from FindTransport alone would not satisfy
  the unconditional endpoint dereferences in 0073c530.
* **00662BA0:** 0073d710 unconditionally invokes sentinel vtable slot 2; its
  selection scope/reference helpers have their own layouts and ownership. It
  cannot use the six-slot request object or an unproven uniform vtable.
* **007A18E0:** despite the operation-constructor name, binary constructs a
  nonvirtual 16-byte tree container (source +0, sentinel +8, count +0xc), with
  sentinel links +0/+4/+8 and marker byte +0x15. 006a7520/006a8110 allocate only
  16 bytes. A request-style 112-byte initialization would overrun this allocation;
  its tree ownership/destructor cannot be guessed from the name.
* **0064B110/00690A10:** indirect tail dispatch, no unique cleanup proven by CFG.
* **0082ADF0:** 0069d530 registers address with atexit; **0082B040/0082B050:**
  006db160 does likewise. No CSF_FUNC binding is necessary to demand a slot.

### Audio evidence (never stubbed)

| Address | Import | Game caller |
|---|---|---|
| 0081DD16 | ov_read | 0070e4a0 audio decode/read |
| 0081DD1C | ov_clear | 0070e600, 0070e970 decoder cleanup |
| 0081DD22 | ov_time_total | 0070e600, 0070e970 stream duration |
| 0081DD28 | ov_open_callbacks | 0070e600, 0070e970 callback-based decoder open |
| 0081DD2E | ov_pcm_seek_lap | 0070e830 seek |
| 0081DD34 | ov_info | 0070e970 checks pointer then reads channels +4/rate +8 |

Boundary contract names and caller packets identify audio usage. Original bytes
are import-table indirect jumps, not NetZ method bodies. No fake Vorbis info or
empty network object is returned.

## Verification

Fresh baseline obtained by temporarily restoring **only** HEAD's generated
stubs.cpp, running `.venv/bin/python tools/linktest.py -j 8`, then restoring the
resumed generated source with a shell EXIT trap. Other task-tree sources were
unchanged (the older REPORT.md's game counts do not describe this newer tree):

```
compiled 14363/14363 files standalone; 14515 functions implemented
unresolved functions: game 99, netz 222, not-a-function 7, png 1, rw 400, rwa 40
data to generate: 2645 globals, 1113 vtables; raw image addresses in code: 0 in 0 files; asm thunks with original addresses: 0 files; other undefined symbols: 50
```

After `.venv/bin/python tools/netzstub.py` and
`.venv/bin/python tools/linktest.py -j 8`:

```
compiled 14363/14363 files standalone; 14544 functions implemented
unresolved functions: game 99, netz 193, not-a-function 7, png 1, rw 400, rwa 40
data to generate: 2645 globals, 1113 vtables; raw image addresses in code: 0 in 0 files; asm thunks with original addresses: 0 files; other undefined symbols: 50
```

`work/linktest.json` has **zero duplicates, zero compile failures**. The resumed
22-slot changes were also built/tested before extension (NetZ 200 unresolved).
Final checks:

* `.venv/bin/python tools/netzstub.py --check --object build/standalone/src_boundary_netz_stubs.cpp.obj`:
  reproducible; **145 cleanup checks, 123 entry-sequence checks**. Includes
  separately proved base-connection slot cleanups before reusing request methods.
* `.venv/bin/python src/boundary/netz/test_offline.py build/standalone/src_boundary_netz_stubs.cpp.obj`:
  **PASS, 154 emulated calls**. ESP/nonvolatile preservation, all 98 scalar/rejection
  entries, owned String deep-copy/self-assignment/destruction, circular-list
  ownership, request/connection virtuals, 92-byte connection canary, borrowed
  endpoints and Gathering factory/delete; no leaked tracked test allocations.
  The heap/CRT are test doubles, not original allocator execution.
* `ninja -C build -j 8 csf_hook`: **exit 0**, linked `build/dinput8.dll`.
* Boundary source compiled with linktest flags minus `-DCSF_STANDALONE`;
  `linktest.symbols('/tmp/netz-hook-guard.obj')` gives no defined/undefined program
  symbols (llvm-nm still lists its harmless absolute `@feat.00` COFF metadata).
* `linktest.raw_addresses('src/boundary/netz/stubs.cpp')`: empty;
  `git diff --check`: clean.
* Original instruction bytes read through `packet.disasm`; game packets read for
  0073cb80, 0073d550 and 006a95d0. `pefile.PE` import-directory inspection verifies
  all six Vorbis DLL/name/IAT pairings above. NetZ library pseudocode/disasm exports
  for the newly examined library addresses are absent here, so the original image
  supplied their vtable/offset/cleanup facts instead.

## Conclusions not execution-verified / still open

No single-player startup or multiplayer-exclusive caller-chain proof is claimed.
The 193 entries still need real per-family boundary reassignment or layout/result
audits; the manifest's grouping and use lines are triage, not exhaustive data-flow
proof. The original 89 name-selected scalar/no-op policies also remain policy,
not proof that every path is SP-safe. Other existing allocator/binding problems
in REPORT.md remain outside this task. This partial result must **not** be used
to declare NetZ cutover complete or to install invented universal objects.

Changed files: tools/netzstub.py and src/boundary/netz/{stubs.cpp, manifest.json,
audit.json, test_offline.py, README.md, CUTOVER.md}; targets.json is unchanged.
No game, smoke,
verifier, IDA writes, commit or network access was used. Hooked DLL sources exclude
/boundary/ in CMake; generated C++ is also guarded by CSF_STANDALONE.
