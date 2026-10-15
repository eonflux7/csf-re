# Mission-load NetZ-region slots (offline audit)

Finding: the 32 listed slots comprise **15 shared plain-data helpers** and
**17 genuine NetZ boundary operations**. All have address-keyed implementations
or explicit offline boundary policies. One extra shared helper, 0x007A2990, is
needed by the Euler helper. No networking, serializer or class registry is
reimplemented. This is an offline audit, not a successful mission-load playtest.

## Address decisions

`A` means exact plain-data implementation in `src/auto/<lowercase address>.cpp`.
`B` means an offline policy in `src/boundary/netz/mission_support.h`, emitted by
`tools/netzstub.py`. All rows are implemented; checks and limitations follow.
Caller addresses below refer to the original instructions as well as the
corresponding source's CSF_FUNC declarations, not just guessed function names.

| Address | What it actually does | Decision / offline result | Original/caller evidence |
|---|---|---|---|
| 0068E2B0 | Zero-initialize three-component vector | A | Three dword stores; game 4FF020 uses its own vector |
| 00660630 | VC7.1 tree iterator decrement, nil flag +15 | A | Game map walkers 6B7D20, 6B7C60; preserves incidental EAX too |
| 00665E40 | VC7.1 tree iterator increment, nil flag +15 | A | Game map walkers 6B7670, 6B7D20 |
| 00672AC0 | Size of embedded dword vector at +10/+14 | A | `(end-begin) sar 2`, null begin -> 0; scene 464720 and six other callers |
| 007A2D60 | Set position interpolation mode at object+3E0 | B: false, no remote mode update | Ownership check 64B5C0 then 690C40; 740A60/741460 ignore or forward byte result |
| 0068E2E0 | Three-dword vector copy constructor | A | Game vectors in 4D1AE0/4D1B50/4D1BC0 as well as 7409D0 |
| 007A2A40 | Euler nearest-equivalent-angle update (y,x,z storage) | A, **not a no-op** | Only float arithmetic and range-wrap 7A2990; 7409D0 passes game object's +48 triple |
| 0068E2C0 | Initialize vector from three scalar bit patterns | A | Three stores, ret12; stack vectors in 7409D0/741460 |
| 007A2AD0 | Degrees -> radians | A | Float multiplication by image float 85798C = 0.01745329238474369; 7409D0 |
| 007A2D90 | Set rotation interpolation mode at object+160 | B: false, no remote mode update | Ownership check 64B5C0 then 690C40; 7409D0 ignores byte result |
| 0068E320 | Vector subtraction into hidden-result buffer | A | Float loads/subtracts/stores, ret8; 741460 temporary vectors |
| 0068E360 | Rescale vector to requested length | A | x87 squared norm, sqrt; zero norm -> false, otherwise scale and true; 741460 |
| 0068E450 | Assign three vector dwords | A | Three stores; EAX contains final source dword, not destination pointer; 741460 |
| 00656380 | Serialize a boolean byte | B: discard, return sink/self | Calls serializer 656090; 7475F0 builds outgoing replicated property payload |
| 00656480 | Serialize an eight-byte scalar | B: discard, return sink/self | Calls 6562C0 with count 1; ret8; 7475F0 |
| 006563B0 | Serialize a byte | B: discard, return sink/self | 656090, width 1/count 1; 7475F0 and game DDL property setters |
| 0068E430 | Copy vector to hidden-result buffer | A | Calls 68E2C0 with the three components, no reset; game 4D1B50 etc. |
| 00651A60 | Construct NetZ object reference | B: empty 12-byte-layout reference, return self | Null input branch stores ptr=0, ID=0, flag(+8)=1; 73E420 enumeration is empty offline |
| 00651D30 | Copy NetZ object reference | B: empty ptr, preserve ID, flag=1, return self | Real nonempty path acquires reference through 6519E0; 73E420/74A9C0 |
| 00652070 | Get object's low-22-bit NetZ ID | B: 0 for absent offline object | Original reads object+2C; 73D0F0 explicitly supplies null when empty lookup fails |
| 00652290 | Per-context owner ID output | B: write 0, return output pointer | Reads TLS-context global 8B9A58/8B9A5C, not a generic lookup; 73D0F0 constructs empty reference from it |
| 0064AD60 | Begin outgoing replicated property payload | B: allocate opaque discard sink | Original goes through object+2C property stream 65C4F0/65BFC0; 74B6E0 immediately writes then commits |
| 006563F0 | Serialize/backpatch a dword | B: discard, return sink/self | Uses NetZ stream 658450/686C10; not a plain game-field write; 74B6E0 |
| 0064CEB0 | Submit replicated payload, consuming ownership | B: consume/free sink, false | Every original success/rejection path destroys/frees payload; 74B6E0/7475F0 pass it as second argument |
| 0066B590 | Resolve registered replication property key | B: invalid ID 0; destroy by-value String | Registry tree 8BAA74/8BAA78, returns registered word or 0 on missing key; destroys String on both branches; not a generic hash |
| 006483D0 | Per-context replication scope output | B: write two zero dwords, return output pointer | Original no-context branch does exactly this; 5516E0 branches on empty scope |
| 0064B620 | Resolve valid NetZ object reference | B: null for empty reference | Original invalid branch returns null; 74A9C0 requires valid nonempty input, bypassed by empty enumeration in 73E420 |
| 0064B110 | Get class/station diagnostic C string | B: borrowed static empty C string | Extracts class ID, registry lookup then virtual+40; 73E0A0 uses %s, 74A9C0 ignores return |
| 007A2A20 | Export Euler triple with first two components swapped | A | Calls vector constructor with self[1],self[0],self[2]; 740AA0/740AF0/740B40 |
| 007A2AE0 | Radians -> degrees | A | Float multiplication by image float 875B98 = 57.2957763671875; game angle getters |
| 006563D0 | Serialize a word | B: discard, return sink/self | 656090, width 2/count 1; 750D00 and other DDL property setters |
| 0068E300 | Squared length of three-float vector | A | x87 sum of squares; used by game geometry 44F3E0, 4503E0, 494250, 48B650, 496E00 |
| 007A2990 (dependency) | Wrap scalar into inclusive float interval | A | Arithmetic + CRT floor 7AFD90; dependency of Euler update; CRT itself is not reimplemented |

## Offline policy and limits

Empty references preserve the boundary's existing 12-byte layout and its
release/iterator helpers. Unexpected nonempty online references fail closed
(`abort`), rather than silently pretending to implement reference counting.
0x652070 likewise accepts only the absent offline object. Scope/owner outputs
are initialized; no pointer-shaped return is replaced by an uninitialized 0.

Property serialization uses a separately allocated, magic-checked **opaque sink**
per transaction. The writers return that same sink; commit consumes it exactly
once and returns false. There is no successful request, fake NetZ stream, or
registry entry. The owned temporary key String is still destroyed. This is
necessary because the game setters unconditionally call the writers; returning
null from the begin-payload slot would not provide their required boundary
contract. No local game-state update is invented by this boundary: for example,
0x74B6E0 is an outgoing manipulation request and forwards commit's false result.

The object/name operations are not evidence that an empty reference can safely
be passed through all of 0x74A9C0: that game routine dereferences an object+2C
even on invalid input. Its mission-load parent 0x73E420 skips its enumeration
loop with the existing empty offline cursor, so it does not reach that invalid
path. Returning a fake DDL object to make it work would violate the boundary.
The name getter has no stack arguments at either game caller; its original
cleanup is an indirect virtual tail, so the explicit offline wrapper has ret0
instead of treating an unbounded cleanup inference as proof.

0x7A2A40 was incorrectly an offline no-op in the inherited patch. It now has a
real arithmetic implementation and a real generic 0x7A2990 dependency. The
mode setters 0x7A2D60/0x7A2D90 remain NetZ, since they inspect ownership and
modify NetZ interpolation state, unlike the plain angle/vector operations.

## Evidence and checks

Original instructions were decoded from `work/pc/CommXPC.exe` with
`packet.disasm`; the inherited full listing is `work/netzload/disasm.txt`.
An additional direct-call scan joined the original instruction sites to
`export/calls.json` and game CSF_FUNC declarations:
`work/netzload/callsite-evidence.json` (32 targets, 488 direct calls).
Disassembly, not the misleading voted names, determined each classification.

All 16 auto files pass `tools/gate.py EA src/auto/EA.cpp`. Fresh-source
`tools/emudiff.py EA --src src/auto/EA.cpp` reports same for all: the two tree
walks have only 2/8 conclusive default profiles, the other fourteen have 8/8.
`tests/netz_shared_trees.py` adds 20 concrete original/candidate comparisons:
head and every node of both a one-node and a balanced seven-node tree, both
directions. All are same, including incidental EAX and iterator writes.
External collaborators in emudiff are mocked; this is not a serializer or
mission-load integration test. Logs are under `work/netzload/`.

The generator now explicitly records shared-game-helper ownership in its
manifest and does not emit a second boundary slot for those addresses.
Additional checks completed successfully:

- `.venv/bin/python tools/netzstub.py --check --object build/standalone/src_boundary_netz_stubs.cpp.obj`:
  259 entry/virtual cleanup checks, 238 boundary entry-sequence checks;
  387 targets = 238 emitted + 5 allocator-owned + 15 shared helpers + 129 deferred.
- `.venv/bin/python tools/linktest.py -j 8`: 14924/14924 compiled,
  15831 implemented slots; no compile failures, duplicate slots, raw original
  addresses or unresolved game functions. Unrelated unresolved boundaries:
  NetZ 122, PNG 1, RW 162 (and 50 other library undefined symbols).
- `cmake --build build --target csf_standalone -j 8`: successful final link.
  Re-run after the two extra source files were added, so CMake's glob and
  generated trap inventory include them. Final full-image inventory:
  14927/14927 inputs, 15831 implemented function slots.
- Final `build/cutover/stubs.json` contains **none** of the 32 requested
  addresses or extra helper 0x7A2990. Other unrelated image/vtable traps remain;
  the full-image inventory is larger than the direct source-call link test.
- `git diff --check HEAD`: clean.

Logs: `work/netzload/{gate-*,emu-*,tree-test.txt,netzstub-check.txt,linktest.txt,build-final.txt}`.
No Windows execution is possible here; no mission-progress claim is made.

## Requested database changes (not applied)

For the following **16** rows, set `region=game`, `module=stl`, `status=done`,
retaining `ea`, `size`, `name`, `name_src` unchanged:

```
00660630 00665e40 00672ac0
0068e2b0 0068e2c0 0068e2e0 0068e300 0068e320 0068e360 0068e430 0068e450
007a2990 007a2a20 007a2a40 007a2ad0 007a2ae0
```

No changes to NetZ's range boundaries or real-NetZ function rows are requested.
Keep verification evidence separate from `done`; none is Windows-verified.
`db/`, `queue/`, `docs/PLAN.md`, `docs/JOURNAL.md` and `export/` were not edited.
