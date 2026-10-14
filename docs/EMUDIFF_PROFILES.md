# Additional emudiff inputs and terminal boundaries

`tools/emuprofiles.py` supplies extra inputs for the audited blocked routines in
its `AUDITED` set. The original eight profiles and comparison tolerances are
unchanged. At most two distinct graphs are appended; `same 1/9` still means
only one usable input, **not** complete equivalence or gameplay verification.

## Input construction (original-only)

* Dispatch provenance comes from `stackabi.call_evidence`, not candidate code.
  Nested input fields, argument objects, globals and returned objects are backed
  by deterministic zero-initialized pages. Root class tables come from the
  existing class inventory. Unknown virtual receivers enumerate actual image
  vtables supporting every observed slot, respecting known class restrictions
  and uniquely proved caller pops; the whole original stack CFG must also be
  consistent. Enumeration is not a universal inference of that receiver's type.
* A discovery execution tracks dword loads and register copies. An unconditional
  low-address dereference can justify replacing its **initial input field** with
  a backed pointer. A selected vtable is installed only at a traced input field.
  Every such change discards the execution and restarts the original. The final
  original must complete with no repairs. A field written by the function or a
  modeled callee is never repaired; locals, instruction bytes, arbitrary register
  values and conditional branches are never patched. Unsupported provenance,
  switches or non-progressing loops remain inconclusive.
  Partial-register writes invalidate the whole pointer's load provenance.
* Both runs receive the final graph before executing, including the same backing
  pages and profile-specific pointer results at matched original call sites.
  Discovery does not inspect the candidate. A candidate-only fault remains a
  difference. There is a bounded discovery budget, not a cap-to-pass conversion.

The profiles exercise the emulator's existing boundary model. They are not
snapshots of a real game session, and do not establish all constructor invariants
or implement callee behavior. Unwritten out-parameters can still prevent a run.

## Audited finite shapes

| Original routine | Input invariant and instruction justification |
|---|---|
| `0041F6C0` | Write mode at archive argument `+14`; count is loaded from vector `+8` at `41F6E6`. An empty vector avoids inventing an archive read-count. |
| `00440F50` | Write mode at archive `+14`, empty sentinel tree at owner `+4`. The serializer compares its first node with that head. |
| `004DE9B0`, `006E7000` | Data pointer at `this+0`, capacity at `+4`, used at `+8`; capacity 2, used 0/1 and extra 1. Record sizes are 12/20 bytes, respectively. |
| `00533040` | Both wide strings have capacity 7 (inline storage) and length 0/1; range positions are zero and counts fit those lengths. The original checks both ranges before replacement. |
| `006AA880` | Source list head at `this+44`, next/previous equal the head: the original exits when `*head == head`. |
| `006B7670` | Nil iterator argument with byte `+15` equal to 1, tested at `6B767D`. This is the actual invalid-iterator exception path, not a forced jump. |
| `006D6800` | Zero child count at `this+5C`; scene update/optimization calls still execute. |
| `007215F0`, `00721900` | Wide-string objects at `+150` and `+16C`, lengths zero, capacities seven, cursor `+1B0 = -1`. The original's SSO checks use `+18 < 8`; its decrement loop checks the signed cursor. |
| `00729710` | Positive width 1/2 at `this+58`. The strip loop advances by this width; zero causes non-progress. |
| `00738070`, `00738530` | State `this+8 = 1`; for `738530`, work field `+1C = 1` enters the draw body, and mode `+34 = 0/1` covers both draws. |
| `00782330` | Empty sentinel list at `this+8` and insertion flag zero: the original tests `next == head` before traversing. |

Other audited routines receive dispatch-derived graphs and bounded pointer
discovery, without function-specific branch bypasses or guessed mock writes.

## Calls and terminal states

* An actual runtime callee's `ret N` still outranks metadata. Caller stack proofs
  now include bounded MSVC switches. `CMP index,limit; JA default` bounds a direct
  dword table, or a following byte-remap table. All indices and destinations are
  read from image bytes and must lead to decoded original instructions. Unknown
  table lengths/forms still abort inference. `6E3B17` is a regression: its five
  case edges let the caller prove zero cleanup for RenderWare callbacks.
  The guard instructions must be contiguous, with no known incoming edge
  bypassing CMP/JA; table cases cannot reenter inside that guard.
* Named callback-table casts are paired with a straight-line reaching load of
  the exact global and the machine slot offset. This distinguishes RenderWare
  cdecl callbacks from virtual methods in the same routine. Typed cleanup is
  admitted only if it satisfies the original stack CFG; unknown ECX conventions
  are not globally ignored.
  Reaching loads are discarded at branch joins, chunk gaps and partial-register
  writes. Unknown computed jumps prevent a straight-line callback proof.
* Explicit float/double casts can supply ST0 only when they cover **every**
  indirect call and their slot sequence matches the binary. `7377F0` has eight
  double-returning calls at slot `+20`. This changes mock outputs, not rounding
  tolerance or memory comparison.
* `_CxxThrowException`, abort/exit and documented process/thread termination
  boundaries stop at the call. `RaiseException` stops only with
  `EXCEPTION_NONCONTINUABLE`; a resumable exception is not treated as terminal.
  Both completed terminal prefixes still compare call targets, arguments and
  non-stack writes. There is no return value, restored ESP or callee-saved-register
  contract at a nonreturning call. Existing stack-pointee observation limitations
  remain; this does not prove exception payload contents equal.
* Late register saves are not new arguments. Actual image tables resolve
  `721624`/`721934` to `ret 4` (one argument), and `6F11A7` to `ret` (zero).
  The original's saved EBX/EDI remains for the later POP. No new argument mask
  was added to hide it.

Validation commands and all 32 before/after rows are in the job's
`work/emufix4/report.md`; benchmark JSONs preserve the paired sources, object
fingerprints, controls and exact mutation verdicts.
