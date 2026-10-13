# Standalone data generation (partial cutover, 2026-10-13)

```
.venv/bin/python tools/datagen.py --compile-check
.venv/bin/python tests/test_datagen.py
```

The first command currently **exits 2**, deliberately: 23 cached linktest references
cannot be defined as non-executable data. It still writes and compiles the valid
subset, and checks its COFF definitions. This is not a completed cutover.
`work/linktest.json` is required; refresh it with linktest.py in the integrating
tree. The worker used the parent tree's cached 2,677 gv / 1,080 vt inventory, not
a new full linktest run. Do not mistake a filtered inventory for full acceptance.

## Layout

One writable, 4096-aligned byte image spans every non-executable PE section, from
original 0x82D000 through 0x8C5000 (exclusive). Original raw section bytes are
copied verbatim, including padding; virtual zero-fill and inter-section gaps are
zero. No executable section is copied or executed. This preserves *all* relative
data offsets, aliases, overlapping globals, array indexing and cross-section
offsets without guessing object sizes. `gv<A>::d` and `vt<A>::t` are constant
byte pointers into this **same** image. Independent arrays cannot implement
overlapping storage; this is why their standalone declarations in globals.h
changed. Hook-mode declarations and macros are untouched. Rebuild all standalone
objects after this header change.

Cost: 622,592 bytes (608 KiB) of static image, including unreferenced and library
data/resources; writable .rdata, with no per-object bounds/type enforcement. The
3,734 emitted address aliases cost another 14,936 bytes on PE32. There is also
fixup code/debug information, not just the original image footprint. Generated
`data.cpp` goes under **build/datagen/**: it is an input-specific build artifact,
not a maintained implementation; keeping it outside src avoids automatic
linktest source discovery and huge generated diffs. Integrators must explicitly
compile/add this TU. No executable link is attempted.

## Pointer evidence and the actual limitation

PE32 FILE_HEADER.Characteristics is 0x10F (RELOCS_STRIPPED set). Base-relocation
directory RVA/size are both zero. There is no loader relocation inventory.
Existing `export/functions.jsonl` drefs contain referenced *targets* from code.
`export/disasm/*.json` contains *instruction*-source drefs, not source locations
of pointer words in .data/.rdata. These cannot prove data-to-data relocations.

The generator validates db/vtables.json against the binary and adds referenced
tables absent from that database by bounded runs of executable addresses. Null
slots inside a run remain zero. Overlapping table starts share storage and fixups
are deduplicated by source location. The table report labels exported versus
binary-inferred tables; inferred boundaries remain an assumption to review.
All recovered slots use `fn_slot<target>::p` in the startup fixup routine,
including unresolved and library slots: there is no fallback to original code.
All exported tables are fixed, not only manifest-referenced starts: traversal
through other copied data can reach them. Consequently this TU introduces
additional function-slot dependencies beyond the current linktest inventory;
the library boundary job must consult the generated unresolved list as well.

For everything else it inventories aligned words that happen to fall into an
image section. Target names from exports improve the evidence label, **not** its
certainty. Default mode does NOT relocate these unproven words. The opt-in
`--candidate-fixups` mode does, but is UNSAFE: numeric constants can look like
addresses, and packed/unaligned pointers can be missed. Candidate counts are
not a count of proven pointers. Strings and vtables are classified by known
starts/slot locations; interior string addresses may be counted as other data.

An IDA job must export actual data-source offset xrefs / fixups, including
unaligned pointers, and audit their coverage. `--data-refs FILE` accepts a
read-only JSON list of `{ "source": "0x...", "target": "0x..." }`; each word is
validated against the PE before emission. This does not automatically certify
the export's completeness. HIGHLOW relocations are used if a non-stripped PE
is supplied; other data relocation types and external targets are rejected.

## Measured inventory

Valid subset: **2,673 gv, 1,061 vt**; **27,131 unique vtable function slots**.
All 3,734 definitions appear exactly once in the compiled standalone object.

Aligned-address inventory (confirmed vtable slots included):

| target kind | sites | confirmed |
|---|---:|---:|
| function | 30,233 | 27,131 |
| code interior / unknown entry | 4,984 | 0 |
| other data | 4,754 | 0 |
| string | 2,472 | 0 |
| vtable | 7 | 0 |

Candidate+confirmed targets in excluded library code: CRT 1,411; NetZ 4,233;
RW 776; D3DX9 674; PNG 24; RWA 183 (7,301 sites total). regions.yaml supplies
range precedence; functions.csv supplies exact-entry labels, notably RWA which
is absent from that coarse region map.

Recovered vtable slots unresolved/library: **3,628 sites, 713 distinct targets**.
Of those sites, library counts are CRT 1,142; NetZ 2,004; RW 4; D3DX9 265;
PNG 1; RWA 0 (3,416 sites); the remaining 212 are game/unknown entries.
Implementation detection scans HOOK variants in every src/ TU, including
TAIL/REGS (api.scan omits those for signature purposes); this is a source
inventory, not proof the eventual link resolves all slots.

Machine-readable output under work/:

- `datagen.json`: counts, elapsed time, completeness/coverage verdicts.
- `datagen-pointers.json`: every source/target, evidence, classification,
  implementation and whether a fixup was emitted.
- `datagen-unresolved.json`: all unresolved/library candidate and confirmed sites.
- `datagen-vtable-unresolved.json`: unresolved/library vtable sites specifically.
- `datagen-vtables.json`: table starts, slot targets, provenance, referenced flag.
- `datagen-rejected.json`: all manifest references not defined.

Rejected gv: 0x718 (unmapped), 0x53C9A0 and 0x560BB8 (embedded .text jump
tables), 0x800000 (.text; needs caller review). Nineteen vt references point to
.text rather than data. Some are unused bindings to initializer functions, e.g.
0x402350. They need caller cleanup, not fake byte arrays or dummy vtables.
Text-embedded data needs a separate reviewed extent/mapping policy. The tests
check that each manifest reference is **either** defined once **or** explicitly
rejected; the summary separately states that full definition coverage fails.

## Startup and remaining work

Call `csf::initialize_data()` explicitly on the main thread, once **after CRT/C++
initialization** (especially HOOK_METHOD's dynamically initialized function slots)
and **before any game/data consumer**. There is deliberately no unordered static
constructor for fixups. Static global bindings may take image addresses earlier;
they must not consume pointer-containing image data yet. The routine is
idempotent but not a concurrent initialization API. Original pointer bytes are
still present until it runs. No original game static constructor is replayed.

Still open: complete pointer relocation evidence, rejected bindings, interior
code targets (no function-entry implementation), library adapters, PE imports
and IAT initialization, original dynamic initialization, and CSF_STR literal
versus copied-image string pointer identity. No runtime/standalone link has been
tested, and default generated data is **not safe to run as a game** yet.
