# WinMain-reached offline NetZ boundary

Follow-on virtual-slot cutover: see [VTABLE_CUTOVER.md](VTABLE_CUTOVER.md).
All 23 actual virtual trap targets in the 128-address upper bound are supplied;
103 downstream direct dependencies remain. Direct-demand NetZ unresolved in
the follow-on worker tree: 163 -> 153 (this tree also includes two intervening
WinMain boundary additions). Historical results below are unchanged.

The 17 slots in the primary tree's `docs/WINMAIN_REACH.txt` are supplied by
address, with binary CFG cleanup and compiled HOOK_REGS entry checks. This
worker tree lacked that report and the tool's `--root` extension; the primary
report was read without changing it, and the extension was ported locally.
Closure includes address-only callbacks, not just directly executed calls.

This is an offline ownership boundary, **not** an online library implementation.
No NetZ source was used. The original image supplies layout and ABI evidence;
game caller source supplies the consumption/failure paths. No Windows run is
claimed. The static closure is neither a runtime trace nor an exhaustive list
of indirect calls.

## All 17 slots: state, consumers, offline failure

| Original slot | Boundary state / what game reads | Offline caller path |
|---|---|---|
| 6683F0 | Exact borrowed pointer at self+4, including null; binary `mov eax,[ecx+4]; ret`. | 4E9C90 and menu/options consumers need real game object identity. No network dummy, allocation, or refcount change. |
| 652010 | Exact borrowed scene-controller pointer at self+5C, including null; binary `mov eax,[ecx+5c]; ret`. | 6D3010 world setup and 417020 spatial collision consume the actual controller. Not a failed network query. |
| 6684F0 | Allocate independent **0x40-byte** circular sentinels, self-linked at +0/+4; no constructed payload. | 73F080 stores at session+A0/B0 and zeroes counts+A4/B4. Never null: allocation failure aborts before publishing invalid state. |
| 6687B0 | Clear/reset empty container, retain sentinel; populated transport payloads reject. | 73EEA0 clears session+9C/AC then separately frees both heads through 6470E0. |
| 670020 | Owned length-prefix String containing original **prudp:/** default, copied from 85B4E4. No fabricated virtual object. | 73F080 constructs session+160; 73EEA0 destroys through 66F150. Original temporary null URL contributes no fields. |
| 671260 | Borrowed original process runtime record at 8BAA9C; original getter simply returns its address. | 73ECC0 ORs flags+10 with 04000000. Return actual data alias, not a copied dummy. |
| 652020 | Exact stored game-manager pointer at self+64. | 506F70 uses the actual manager. This shared SP helper is **not** a failed network query. |
| 652160 | Exact stored game-object manager pointer at self+70. | 50FBD0/50D270 subsequently use its game vtable. No null/fake manager. |
| 7A7B50 | True, exactly as binary `mov al,1`. | Shared 6B1B80 branches on low byte. Not an offline-network false stub. |
| 646D30 | Store args2/3 in the two initialized current-thread map values (8B9980/8B9968), preserving their ownership. | Failed 743A10 entry checks record notice then return false. Foreign threads reject; no additional nodes/mutexes are fabricated. |
| 64AD00 | Bounds-checked actual class-registry array lookup (count+0, pointer+4), null if absent. Preserve any genuine registered object's identity. | 743A10's missing reference+0 branch records failure and skips this lookup and the otherwise-unconditional returned-object class virtual. No dummy class descriptor. |
| 64B250 | With no online selection context: false for null owner+self+C, true for nonnull owner, matching original branches. | 73DE40 empty selection skips the loop and this predicate. |
| 64B9B0 | Exact empty 0x24-byte selection iterator: context+0 null; link+4 has null object, zero ID, ownership byte+8=1; sentinel+10; first/last+14/+18; mode byte+1C=1; counter+20=0. | Nonnull online contexts reject. Empty construction owns no referenced network object and is not an uninitialized return-only stub. |
| 651C70 | Reset null-object link and copy input DWORD ID into link+4. Reject an unowned live reference. | 73D710/73DE40 pass pointer to zero; iterator+8 stays null, so no online traversal/wait. |
| 651D00 | Construct 12-byte null-object reference, input ID+4, byte+8=1. | 743A70 calls 743A10: null object means false and bypasses object+68 access. |
| 65E250 | Null: no online selection context. | 73D710 explicitly handles null, resets link, skips AdvanceIterator/SetTraversal; sentinel still constructed and released correctly. |
| 662BA0 | Caller-owned **20-byte** sentinel: original seven-slot 859FA0 table, WORD refcount+4=1, cache+8=0, type+10=input. Padding/cache byte+C not spuriously overwritten. | 73D710 allocates 20, assigns/retains then **unconditionally releases slot2**. Clear releases last owned reference. |
| 668AC0 | No callback installed in this offline boundary; return zero, no invocation. | 743A70 discards return and continues cleanup. Additional registration slot 669B40 explicitly rejects, so this is not cancellation of a fabricated successful registration. |
| 7A8660 | Null typed singleton; no session entry exists offline. | Address-only callback 7437E0 checks null and returns before setters or vtable+40 tail. |

Not all failures above are operations that should return false: constructors and
shared local getters must yield valid state/identity. The network selection and
singleton absences have explicit game-side empty/null branches. No pending
request, connection, callback, or transport is created. Missing class lookup is
not safe to dereference; the verified offline caller avoids it, rather than
receiving a universal dummy.

## Indirect calls and additional ownership support

* **73D710** calls sentinel vtable+8 unconditionally. **65E130** calls old
  sentinel+8 (release), new sentinel+4 (retain), +14/+18 (output getters).
  The earlier 65E130 no-op leaked the game allocation and failed to initialize
  cached IDs. It now maintains the original WORD reference count and iterator
  fields. **651B40** validates empty reference release rather than silently
  discarding foreign online state.
* Original sentinel table **859FA0** ends at next table **859FBC**. Its seven
  supplied targets are **662BD0/64FE50/64FE90/662C30/662C00/662C60/662D10**,
  with independently proved cleanup **4/0/0/4/4/4/4**. Delete honors bit0;
  refcount owns lifetime; membership rejects; first/last outputs are -1/0 as
  original empty registry scans produce. Original table identity is retained.
  No request vtable or uniform ret-0 table is substituted.
* **73DE40**'s populated branch would invoke selected object slots +34/+C;
  with null context and null link+0 it is skipped. Existing online traversal
  entries are still rejecting boundary policies, not a working object registry.
* **743A10** would dereference class lookup result and invoke +3C; offline
  651D00 yields null reference and that branch is skipped. **7437E0** would tail
  through singleton+40; null singleton suppresses it. The latter callback is
  captured through source address references, but invocation is computed.
* **743A70** can delete populated services through slot0 and manager+14.
  **73ECC0** can delete transport-owned children/connections through slot0,
  and manipulate an optional clock provider at 8B99B0. Normal freshly
  constructed offline session pointers are null, so those branches are skipped.
  Earlier supplied connection/request/endpoint private tables retain their own
  audited ABI. No claim is made that every later dynamically created service's
  table is supplied by this job.
* Shared managers returned by **652020/652160** dispatch through genuine game
  tables; those targets belong to game/RW work, not invented NetZ virtuals.
* Initializer-indirect caveats in INIT_CUTOVER.md still apply (optional clock
  provider, allocator/CRT calls, original table identities with unsupplied slots).

## Verification scope

`test_winmain.py`, called by `test_offline.py`, exercises every new wrapper,
binary stack cleanup/nonvolatile preservation, 0x40-byte head size and independent
ownership, URL default/deallocation, pointer identity getters, borrowed runtime
write-through, registry bounds, empty-link failure, canaries and all seven
selection virtual contracts. It then runs actual compiled game session
construct/destroy cycles twice, the selection-constructor/processing callers,
entry validation and stopped-service cleanup. Unexpected dependencies trap;
the tests do not mock a universal successful game return. Heap/CRT/Win32 remain
test doubles. Allocation exhaustion and Windows SP startup are not tested.

Commands/results from the worker run are recorded in its report, not inferred
from this documentation. Static no-NetZ reachability is only after regenerating
standalone trap inputs; merely regenerating boundary source does not establish it.

## Resumed-worker verification (2026-10-14)

* `tools/linktest.py -j 8`: **NetZ 183 -> 165** unresolved (HEAD's boundary
  source temporarily restored for the before inventory, then restored to the
  task source). Both inventories compile 14792/14792 files; no raw image
  addresses. The 18 resolved direct demands are the 17 requested slots and
  callback registration 669B40. Seven newly captured original sentinel virtual
  targets bring emitted wrappers from 169 to 194; two old result-only policies
  are replaced with owned-state helpers. Frozen manifest: 371 targets,
  194 emitted + 5 allocator-owned, 172 deferred.
* `tools/netzstub.py --check --object
  build/standalone/src_boundary_netz_stubs.cpp.obj`: 216 cleanup checks and
  194 entry-sequence checks pass. Selection virtuals are included among the
  address-keyed emitted entries (not counted twice as extra virtual wrappers).
* `src/boundary/netz/test_offline.py
  build/standalone/src_boundary_netz_stubs.cpp.obj`: **603 calls pass**;
  includes all 46 initializer and 17 WinMain wrappers, 54 actual initializers
  and registered destructors, 15 actual game session/selection/cleanup wrappers,
  and every selection slot invoked through the relocated original vtable.
  Stack cleanup, nonvolatile registers, canaries and ownership are checked;
  no test allocations or critical sections survive.
* `cmake --build build --target csf_standalone -j 8`: links. Its full
  data/vtable inventory still has 830 NetZ traps outside this direct closure;
  the smaller linktest count above inventories direct source demands.
* `cmake --build build --target csf_hook -j 8`: links `dinput8.dll`.
* `tools/initreach.py --root 0x509250`: 1054 addresses reached, **no NetZ
  trap lines**; remaining traps: CRT 2, game 4, not-a-function 3, RW 31.

Logs are in `work/netzwm-review/`. No Windows execution, successful SP startup,
allocation exhaustion, or populated online-state handling is established.
