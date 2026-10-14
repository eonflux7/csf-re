# Initializer-reached NetZ boundary

**46 / 46 supplied.** This is process-object lifetime/data support, not NetZ
online implementation. Original vtable identities are retained; unprovided
methods still trap. `init_support.h` is standalone-only through `stubs.cpp`.
No library source, game source, exports, database or queue was changed.

## Objects and observed consumer layout

Addresses below are original function slots. Constructors return the caller's
object (node factories return owned nodes); binary `ret N`, not guessed C++
prototype cleanup, governs every generated wrapper. Exact instructions and
per-address ownership/offset evidence are in `audit.json`; direct game caller
files and use lines are in `manifest.json`. Many functions are unnamed: the
descriptive labels below are not claims of recovered class names.

| Slots | Object / what its consumers read |
|---|---|
| 646E10, 646EF0, 649AC0, 649BA0 | Three thread-indexed DWORD default-value globals: `+4` default, `+8` owned native mutex, `+0x10` tree sentinel, `+0x14` count. One real current-thread node: key `+0xc`, value `+0x10`; black/sentinel bytes `+0x14/+0x15`. Independent mutexes and a real shared registration list. |
| 648FA0, 648FB0 | **MutexPrimitive.cpp**: nine direct global mutexes plus mutexes embedded above; `self+0` points to an owned, initialized 24-byte Windows `CRITICAL_SECTION`. Delete before free. |
| 651D70, 651D80, 653730, 653740, 653750, 6537B0 | **PseudoGlobalVariable.h** registry base/registry/append/unlink: original vtable at `+0`, next at `+4`, global first/count `8B9A88/8B9A8C`. Five derived tables independently have slots 4/5 as `+4` get/set, so offline append/unlink uses these exact fields without dispatching unaudited methods. |
| 647840, 6523A0, 67AC40, 657810 | Pseudo-global context-array initialization: owned array at `+8`, copied DWORD default `+0x10` or WORD default `+0xe`. Original context count at `8A6544` is **1**. The actual startup branches skip these array calls at count 1; static closure includes them anyway. |
| 652680 | Pseudo-global two-DWORD clock pair: current at `+0x10`, default at `+0x18`, independent optional array at `+8`; the stack input is copied, not borrowed. |
| 647200, 647220, 6587B0, 658610 | Sixteen 0x50-byte pseudo-globals with current/default 32-byte values at `+0x10/+0x30`. Flags at value `+0/+0x10`, DWORDs `+4/+0x14`, embedded vtable `+8`, pair `+0x18/+0x1c`; optional owned 32-byte-stride array at owner `+8`. Exact field copies leave unrelated padding alone. |
| 668450, 66D400, 6882D0, 796490, 79C090 | Five 12-byte circular-list sentinel factories: `head+0/+4` self links, payload `+8`; game initializers store sentinel at container `+4`, zero count at `+8`, and callbacks clear nodes then separately free sentinel. Never return null or a shared sentinel. |
| 669FE0, 6694A0 | Pseudo-global DWORD lists: current/default containers `+0xc/+0x1c`, independent heads `+0x10/+0x20`; optional 16-byte-stride context list array `+8`. Deep-copy nodes/payload DWORDs; release every head and array. |
| 66B390, 7AA3C0, 66BBE0, 7AA730 | Two 24-byte tree-node factories / empty range cleanup: factory links zero, black byte `+0x14=1`, marker `+0x15=0`; initializer makes marker 1 and three self-links. Callback fills its output iterator then frees the caller-owned sentinel. Populated online trees are rejected, not silently lost. |
| 64A310, 64A290 | **WaterMark.cpp**, named **PRUDP Dispatch Queue** / **PRUDP Sliding Window**: borrowed name `+0xc`, millisecond sample `+0x10`, threshold 60000 at `+0x14`, float 1 at `+0x20`, zero counters. Real process registration list at `8B9A24`; last destructor releases it. These are records, not offline network services. |
| 6474D0 | Real local millisecond clock: fills caller's 8-byte output using QPC/Frequency (multiply by 1000 then unsigned divide as original); delegates optional provider at `8B99B0` through slot `+4` with one output argument. `78CEA0/78CF40` use this shared clock as well as the initializer. |
| 672620, 653840 | Unnamed settings: eight zero words, byte `+0x20=1`, DWORD `+0x24=0`; destruction zeros only the eight words. |
| 672960, 6727A0 | **Log.cpp** base objects (two globals): original parent vtable, owned 4-byte child `+4`, owned 32-byte timestamped child `+8`, extension `+0xc=null`. Child tables `85B8E0/85D93C` retain their identities and unaudited operations remain traps. Destructor accepts only its own offline children. |
| 67CFE0 | Seeded random-generator state: 624 DWORDs, seed 1 and successive multiply-by-69069 modulo 2^32; index `+0x9c0=624`, twist constants `+0x9c4/+0x9c8=0/9908B0DF`. No random-generation algorithm supplied. |
| 67D010, 67D030 | **RootDO** base of game-derived descriptors (nearby original string **IDGeneratorDDL.cpp**): original base table, zero `+4/+0xc`, zero byte `+8`, input flag byte `+0x10`. Game initializers apply their actual derived tables afterwards. |
| 67D100 | Unnamed reference/token globals: null DWORD `+0`, zero bytes `+4/+5`; game's later metadata and padding are not overwritten. |
| 686C10 | **GetCachedObjectProvider**, shared by **252 direct caller declarations**: return precisely `[self+0xc]`, preserving stored pointer identity. Not a null/fake networking result. Scene-cleanup, rendering and gameplay consumers are retained in the manifest. |
| 68ECE0 | Unnamed timeout/default parameters: float 4 and -1 at `+0/+4`, 1000/250 at `+8/+0xc`, enabled byte `+0x10`, 3500 at `+0x14`. |
| 68F540 | Unnamed interpolation/default state: seven zero DWORDs, float -1 at `+0x1c`. |

The baseline 321 manifest targets gained 43 newly demanded addresses (three of
the 46 were already tracked). Manifest totals: **169 emitted + 5 allocator-owned,
190 deferred / 364**. This does not eliminate unrelated NetZ traps reached from
vtables/data or implement online operations.

### Explicit offline policy differences

* Thread-map base registration is real and offset-compatible. Its list/mutex are
  released on last map destruction instead of registering the original hidden
  lazy singleton callback `82A820`. All three map destructors remain registered
  by the actual game initializer wrappers. There is no hidden library callback
  or execution of original NetZ code.
* Allocation failure aborts before publishing partial state: the original callers
  immediately dereference results. Nonempty online tree range cleanup, invalid
  context counts, or substituted Log children/extensions fail closed.
* Original virtual tables are address-bound, not replaced with a uniform dummy
  table. Their unprovided methods remain fatal traps with no fabricated success.

## Verification

The worker checkout omitted `docs/INIT_REACH.txt`, its measurement script and
generated trap/initializer inputs. The supplied baseline report/script were read
from the primary tree; the script was copied to worker scratch and only its ROOT
changed. The before line is from the primary tree's existing baseline inventory;
the after line is from this worker's freshly linked standalone inputs:

```
trap slots reachable from initialisers: 51 {'game': 1, 'netz': 46, 'rw': 4}
trap slots reachable from initialisers: 5 {'game': 1, 'rw': 4}
```

Verified offline:

* Boundary compiled with `tools/linktest.flags()`; `netzstub.py --check --object`
  verifies **191 cleanup checks / 169 entry sequences**.
* `test_offline.py build/standalone/src_boundary_netz_stubs.cpp.obj`: **PASS, 528
  emulated calls**. Every new wrapper is exercised. `test_initializers.py`
  additionally loads **109 actual compiled game COFF objects** and executes the
  **54 direct NetZ-using initializer wrappers and 54 registered destructors** in
  table/reverse-registration order. All sixteen array elements are included;
  ESP/nonvolatile registers, independent ownership, canaries and native-sized
  critical-section initialize/delete-before-free are checked. Custom clock-provider
  thiscall output/cleanup and rejection of unowned online states are also tested.
  No tracked leaks.
  `829A20`'s scene/RW graph is excluded; its one shared provider getter is tested
  directly with a non-null stored pointer. Heap, CRT and Win32 imports are doubles.
* `tools/linktest.py -j 8`: all **14780/14780** src files compile, **15164** supplied
  function slots; zero duplicate definitions, raw image literals or compile errors.
* `cmake --build build --target csf_standalone -j 8`: linked executable, exit 0.
* `cmake --build build --target csf_hook -j 8`: exit 0, linked
  `build/dinput8.dll` (full local hooked build, not a reused DLL).
* `standalone_check.py`: structural inputs consistent, **36023 fixups**, zero
  undefined fixup targets, **zero initializer traps**. Startup readiness is still
  **false** because nine computed calls are unanalysed; not a Windows execution.
  Its 13 regression tests pass; `--require-ready` is expected to return 2.
* Boundary compiled with hooked flags (without `CSF_STANDALONE`): no defined or
  undefined program symbols, confirming that none of these entries replaces the
  original library in the hooked build.
* Python syntax compilation and `git diff --check` pass. Source/header raw-address
  scans are empty.

For the first full src inventory, unchanged COFF cache entries from the primary
tree were reused only after byte comparison of sources/all existing src/hook
headers and path-normalized compiler flags; mismatches were compiled locally.
Imported objects were also newer than the primary tree's headers (its four
older abandoned object files have no worker source and were not imported).
The changed boundary was compiled locally, not taken from that cache.

## Static-closure caveats / next Windows run

`init_reach.py` scans declarations and address tokens, not executed paths. It
includes **atexit** chains and branch-only scalar context allocation helpers;
it is not a claim of 51 startup calls or 46 startup traps. Its five remaining
slots are still **game 69DFA0** and **RW 58DF40/58E310/58EF00/58F740**, all in the
reported exit chains. They were not supplied here.

No C++ table-entry body directly calls a vtable/pointer in the original bytes.
Indirect calls occur in their callees (and in registered exit chains):

* Pseudo-global registry initializers **828BE0, 828C40, 828D40, 828DA0, 828E50,
  828EB0, 828F20, 828F80, 828FD0, 829180, 8291E0** originally reach registry
  get/set virtual slots through `653750` (and `6537B0` on exit). The boundary
  replaces that dispatch with the independently audited `+4` chain fields.
* Thread maps **828BA0, 828BC0, 828CC0**, mutex globals **828CA0, 828CE0, 828D00,
  828E30, 8290F0, 829110, 829240, 82A360, 82A400**, clock/watermark users
  **828DA0, 828DF0, 829150, 829270, 829290** reach Win32 IAT calls inside library
  constructors. Clock provider virtual dispatch is conditional. Log globals
  **828DF0, 829150** originally delete children through virtual slot 0 on exit;
  boundary destruction owns/frees only the recognized offline children.
* Game/allocator closure has computed sites rooted at **828A30, 828A80, 828AB0,
  828AD0, 828AE0, 828AF0, 828B10, 829420, 829440, 829460, 8294B0, 8294C0,
  829650, 829A20, 82A2A0** (includes exit chains). The unanalysed startup-site
  addresses reported by `standalone_check.py` are **690D6C, 690D7B, 6910BB,
  69DF6E, 69DF76, 69E3BC, 69E3D8, 69E3F4**, plus C initializer **7B460F**.
* Original map construction also hides lazy callback `82A820` inside NetZ code:
  declaration-only source closure cannot discover it. Offline last-detach
  ownership above removes that hidden callback rather than pretending it was
  statically proved reachable/resolved.

Expected next dry run: get past `828BA0` and every direct NetZ process-initializer
call, potentially print `csf: dry run completed; WinMain not called` after all
**196 C++ entries**. That is a prediction, not a runtime result. Computed
allocator/CRT paths, the five unrelated exit traps and other boundary/runtime
issues can still stop Windows execution. No game/exe/smoke was run here.
