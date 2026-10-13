# Generated offline NetZ boundary — partial, deliberately fail-closed

Regenerate with `.venv/bin/python tools/netzstub.py`; test reproducibility with
`--check`. Initial target capture uses `--report work/linktest.json` **before**
installing these slots. `targets.json` freezes that baseline so regeneration
does not discard functions which became resolved. `manifest.json` lists every
address, the API contract/export prototype, emitted/deferred decision, binary
ret bytes, and every existing game-side caller declaration.
`--check --object build/standalone/src_boundary_netz_stubs.cpp.obj` additionally
checks actual COFF entry sequences and ret instructions against all 123 emitted
entries and 22 virtual-slot ABI checks (the connection shares the request's six
method implementations). Current totals: **123 generated + 5 allocator bindings /
321 demanded; 193 deliberately deferred**.

This audit adds 29 slots, not a completed cutover. See `CUTOVER.md` for
the before/after evidence, family counts, caller offsets, and remaining blockers.
`audit.json` is the reviewed policy/ownership evidence for those slots; generation
requires it. `manifest.json` also includes direct caller function names, call-use
lines, triage groups, and binary cleanup even for deferred entries. Triage groups
are **not** claims of multiplayer-only reachability (`mp_only_proven` is false).
`test_offline.py OBJECT` executes the compiled boundary with a tracked fake heap
in Unicorn; this tests local ABI/ownership, not SP startup.

The generator does not hand-edit exports or infer a universal dummy virtual
ABI. It follows binary branch/tail control flow to prove a unique ret N and
checks that against the contract; indirect tails/ambiguous cleanup are deferred.
Naked result-only stubs preserve non-result registers and implement the proven
stack cleanup regardless of cdecl/thiscall/fastcall spelling. They do not read
parameters. Output-buffer initialization is NOT implicitly supplied.

| Family | Behaviour |
|---|---|
| Bool/byte network queries, send/request/field operations | false / zero; no connection, no callbacks, operation rejected |
| Void named network methods | no-op; no transport/state mutation; cleanup methods may leak pending resources |
| Explicit count/predicate methods | zero count / false |
| Last-result status | -1, nonzero error (zero would mean success) |
| Allocators | five slots supplied by boundary/alloc on the game heap; not emitted twice |
| Owned String default/construct/assign/destroy | compatible length-prefix storage backed by game allocation, forwards byte copy/length work to CRT |
| Audited `00798DF0` Gathering base constructor | constructs 0x30 bytes, two owned empty Strings, default +0x14 = 1, private eight-slot offline vtable |
| Gathering virtual delete/factory/class-name | matched ret 4/0/4; destroy strings and optionally free, allocate another valid offline Gathering, construct `Gathering` name in caller's String output |
| Gathering local name/type/base destruction | deep-copy name; store type/options; free both owned names without freeing caller's object |
| Request constructor `0066A440` | caller's 0x70-byte allocation, state +0x0c = 3 (completed failure), six-slot private vtable with cleanup 4/0/0/0/8/0; slot 0 frees when flag bit 0 is set |
| Connection constructor `00651770` | caller's **92-byte** allocation (not 112), failed state/error scratch and its own six-slot table; same proven method cleanup as request |
| Session String `0066F130/0066F150/0066F180` | construct/destroy/deep-copy owned length-prefix text; replaces a previously leaking destructor no-op |
| Endpoint getters `006486C0/006486D0` | distinct process-owned 28-byte borrowed endpoints, two-slot delete/reset table with ret 4/0; no free of static storage |
| Circular list helpers | owned 12-byte empty head; real self links; insertion creates node with owned empty URL; clear frees nodes/payload, not head; count updated locally |
| URL/format String outputs | valid owned empty String; URL output constructed, format output replaces initialized String; caller destroys via `0064A6F0` |
| Result scratch | 12-byte completed error record with borrowed static diagnostic; not an object/vtable |
| Audited request/load/endpoint operations | ten zero rejections; tested return discarded/byte predicate/forwarded status; no claim of exclusive MP reachability |
| Other Gathering virtual predicates/serialization | zero with each original slot's own proven cleanup; NOT a uniform ret-0 table |
| Unreviewed constructors, pointer/ID results, unknown/shared helpers | deliberately **unresolved**, not fake success and not a blindly null/dereferenceable object |
| `ov_*` functions | deliberately unresolved: these are Vorbis **audio** imports despite the NetZ region label |

## What later work must audit

All entries marked `result_dereference_review` in manifest.json are a conservative
superset of result-dereference hazards (includes pointer-like integer returns
identified by the export). Their caller file/name/signature lists are included.
There are 84 flagged addresses, including the reviewed data/object returns now
emitted. `ov_info` is a concrete deferred example:
`src/auto/0070e970.cpp` reads channels at +4 and sample rate at +8 after checking
the returned pointer. An invented NetZ object is not valid Vorbis info.
Do not interpret this as a proven exhaustive data-flow analysis. Constructor
virtual factory/class-name outputs are now valid objects; allocation failure
still returns null, just as an allocation failure may in the original.

Unaudited pointer-returning stubs are not emitted. These include session object,
endpoint/registry, list/sentinel, resolver and embedded scratch results. Making
up one shared 4-KB object or a uniform dummy vtable would hide use-after-free,
member-offset and ret-N errors. Remaining constructor families need their full
embedded ownership/layout and indirect virtual calls audited individually.

`0064B110` (station name getter) is an indirect virtual tail; no correct cleanup
is established, so even its tempting empty-C-string stub remains unresolved.
`00690A10` likewise has an indirect tail. Some exported userpurge argument
sizes disagree with binary ret N; the binary wins, with contract conflicts
recorded rather than silently copied into C++ signatures.

These choices have been compiled and statically checked, **not played**. A
link-test improvement is not evidence that single-player startup works, or
that failure paths exit waits gracefully. The deferred set is intentionally
visible for the next job rather than made to look complete.
