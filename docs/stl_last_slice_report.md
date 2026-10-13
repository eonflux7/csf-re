# STL last slice: 194/224 admitted

194 new instances pass both gate (`ok`) and emudiff (`same`). 30 rejected candidates were removed from `src/auto/`. Database statuses remain unchanged for orchestrator adoption.

## Per-cluster coverage (assigned todo slice only)

| Cluster | Done / total | Template |
|---|---:|---|
| 16 | 0 / 5 | `tree_erase (rejected)` |
| 21 | 4 / 4 | `vector_insert_n` |
| 22 | 4 / 4 | `vector_insert_n` |
| 25 | 4 / 4 | `vector_insert_n` |
| 27 | 4 / 4 | `vector_insert_n` |
| 28 | 3 / 3 | `vector_insert_n` |
| 31 | 3 / 3 | `vector_insert_n` |
| 32 | 3 / 3 | `vector_insert_n` |
| 47 | 2 / 2 | `vector_insert_n` |
| 48 | 2 / 2 | `vector_insert_n` |
| 51 | 2 / 2 | `vector_insert_n` |
| 56 | 2 / 2 | `vector_insert_n` |
| 60 | 2 / 2 | `vector_insert_n` |
| 61 | 2 / 2 | `vector_insert_n` |
| 66 | 30 / 30 | `vector_ufill` |
| 79 | 2 / 2 | `vector_insert_n` |
| 81 | 2 / 2 | `vector_insert_n` |
| 105 | 23 / 23 | `vector_ufill` |
| 120 | 20 / 20 | `vector_ufill` |
| 137 | 17 / 17 | `vector_ufill` |
| 157 | 18 / 18 | `vector_ufill` |
| 186 | 0 / 2 | `ordinary network enumeration (rejected)` |
| 201 | 0 / 9 | `vector_assign_value (rejected)` |
| 439 | 0 / 2 | `vector_destroy_range (rejected)` |
| 505 | 5 / 5 | `vector_ufill` |
| 548 | 0 / 2 | `vector_xlen (rejected)` |
| 715 | 0 / 1 | `vector_fill_assign (rejected)` |
| 782 | 0 / 2 | `vector_copy_backward (rejected)` |
| 827 | 0 / 2 | `vector_fill_assign (rejected)` |
| unclustered | 40 / 45 | `vector_insert_n; tree_erase, string_replace_range, ordinary game functions rejected` |

## Verification

Representative packets and typed pseudocode were read, with per-instance Capstone code-head extraction. Every wrapper has a binary-disassembly manifest in `work/stlrest/wrappers.json`. Allocation and signed-difference arithmetic cross-checks for all 82 insertion candidates agree with fill-helper strides (`work/stlrest/stride-evidence.json`, including the rejected 0047ef80). Constructor and normal-cleanup traits come from original code heads; original helper boundaries are retained.

The candidate sweep (224 new + 127 older `vc71_vector.h` users) was:
```sh
STL_RESULTS=work/stlrest/final-results.json .venv/bin/python work/stlrest/check.py $(cat work/stlrest/all-files.txt) > work/stlrest/final-check.log 2>&1
```
Totals: 351 checked; 321 passed both; 30 rejected. Older instances: 127/127 passed both. Each checker invocation runs `.venv/bin/python tools/gate.py EA src/auto/EA.cpp`, then `.venv/bin/python tools/emudiff.py EA --src src/auto/EA.cpp`, using freshly compiled sources.

The **last command over all retained files**, after removing rejected candidates, was:
```sh
STL_RESULTS=work/stlrest/retained-results.json .venv/bin/python work/stlrest/check.py $(cat work/stlrest/retained-files.txt) > work/stlrest/retained-check.log 2>&1
```
Exact totals: **321 checked, 321 gate `ok`, 321 emudiff `same`, 0 failures** (194 new instances + 127 older header users). All 194 new source files present in the final worktree occur in that passing result set. The original byte sizes of the admitted functions total 65,067 bytes. `git diff --check` passed, and the final status audit found only the header, this report, and the 194 new source files changed.


## Throw-reaching failures (apart)

All these differing profiles cross `KERNEL32.dll!RaiseException`. The known zero-vs-16-byte cleanup limitation is implicated; this is not proof that it is the sole cause, especially for 0043efc0. No emulator changes or workarounds were made.

- **0043efc0**: `0043efc0 DIFF         1/1 profiles; profile 6: call 4 to 00690e60: arguments ['002b43c1'] vs ['002efd51']`
  - Gate: `your code accesses field offsets the original never uses: 0x18 (if they are members of a LOCAL object the original keeps on its stack, e.g. a temporary string, access the local directly in the function body or mark the helper `static inline __attribute__((always_inline))`: a helper reaching it through a pointer shows up as a field access)`
- **0046fac0**: `0046fac0 DIFF         8/8 profiles; profile 0: 23 calls vs 3`
  - Gate: `8/8 profiles; profile 0: 23 calls vs 3`
- **0054b640**: `0054b640 DIFF         1/1 profiles; profile 6: ours changes ebx/esi/edi/ebp`
  - Gate: `1/1 profiles; profile 6: ours changes ebx/esi/edi/ebp`
- **006b7670**: `006b7670 DIFF         1/1 profiles; profile 6: ours changes ebx/esi/edi/ebp`
  - Gate: `1/1 profiles; profile 6: ours changes ebx/esi/edi/ebp`
- **006ce410**: `006ce410 DIFF         8/8 profiles; profile 0: 23 calls vs 3`
  - Gate: `8/8 profiles; profile 0: 23 calls vs 3`
- **00735130**: `00735130 DIFF         1/1 profiles; profile 6: ours changes ebx/esi/edi/ebp`
  - Gate: `1/1 profiles; profile 6: ours changes ebx/esi/edi/ebp`
- **0073a0a0**: `0073a0a0 DIFF         1/4 profiles; profile 6: ours changes ebx/esi/edi/ebp`
  - Gate: `1/4 profiles; profile 6: ours changes ebx/esi/edi/ebp`
- **0075f1e0**: `0075f1e0 DIFF         1/1 profiles; profile 6: ours changes ebx/esi/edi/ebp`
  - Gate: `1/1 profiles; profile 6: ours changes ebx/esi/edi/ebp`

## Other unfinished addresses

- **0043f860**: `0043f860 inconclusive cap x8`
- **0043fa20**: `0043fa20 inconclusive cap x8`
- **00449550**: `00449550 inconclusive cap x8`
- **0044c2a0**: `0044c2a0 inconclusive cap x8`
- **00458360**: `00458360 inconclusive cap x8`
- **00458630**: `00458630 inconclusive cap x8`
- **0046eda0**: `0046eda0 inconclusive cap x8`
- **0047ef80**: `0047ef80 DIFF         5/6 profiles; profile 0: call 1 to 0047f240: arguments ['2bd4e100', '28000100', '2c000101', '2a000100', 'cccccccc', '2a400100', '2a300100', '2a200100', 'cccccccc', 'cccccccc', 'cccccccc', 'cccccccc', 'cccccccc', 'cccccccc', '2bd4e100'] vs ['2bd4e100', '28000100', '2c000101', '2a000100', '00000000', 'cccccccc', 'cccccccc', 'fdb02800', '00000001', '00000000', 'fdb02800', 'fdb02800', 'fd17ec00', '2a000100', '2a100100']`
  - Gate: `5/6 profiles; profile 0: call 1 to 0047f240: arguments ['2bd4e100', '28000100', '2c000101', '2a000100', 'cccccccc', '2a400100', '2a300100', '2a200100', 'cccccccc', 'cccccccc', 'cccccccc', 'cccccccc', 'cccccccc', 'cccccccc', '2bd4e100'] vs ['2bd4e100', '28000100', '2c000101', '2a000100', '00000000', 'cccccccc', 'cccccccc', 'fdb02800', '00000001', '00000000', 'fdb02800', 'fdb02800', 'fd17ec00', '2a000100', '2a100100']`
- **004f2ec0**: `004f2ec0 inconclusive cap x8`
- **005134c0**: `005134c0 inconclusive cap x8`
- **00533040**: `00533040 inconclusive fault Unhandled CPU exception (UC_ERR_EX x3, cap x2, fault Invalid memory read (UC_ERR_READ_U x2, fault Invalid memory write (UC_ERR_WRITE x1`
- **006aa880**: `006aa880 inconclusive fault Invalid memory read (UC_ERR_READ_U x7, cap x1`
- **006dcff0**: `006dcff0 inconclusive cap x8`
- **006de100**: `006de100 inconclusive cap x8`
- **00723fd0**: `00723fd0 inconclusive cap x8`
- **0073f3b0**: `0073f3b0 inconclusive fault Invalid memory read (UC_ERR_READ_U x6, fault Invalid memory write (UC_ERR_WRITE x1, cap x1`
- **0073f4f0**: `0073f4f0 inconclusive fault Invalid memory read (UC_ERR_READ_U x6, fault Invalid memory write (UC_ERR_WRITE x1, cap x1`
- **00762d70**: `00762d70 inconclusive cap x8`
- **00773bd0**: `00773bd0 inconclusive cap x8`
- **0077d190**: `0077d190 inconclusive cap x8`
- **00782330**: `00782330 inconclusive fault Invalid memory read (UC_ERR_READ_U x7, cap x1`
- **00784350**: `00784350 inconclusive cap x8`

## Changed files

- `src/types/vc71_vector.h`: `_Ufill` forwarding template and optional operation-trait hooks for insertion shape variants (growth-size boundary, return residues, split arithmetic, cleanup, spare fill/assignment). Existing defaults remain compatible.
- 194 new `src/auto/<address>.cpp` files; exact list: `work/stlrest/done-addresses.txt`. No changes to tools/, db/, queue/, export/, PLAN, or JOURNAL.

## Conclusions and open work

Offline equality is limited to the stock profiles, with external helpers mocked. No valid-container fixture sweep, full EH guarantee, standalone link test, game execution, or smoke test is claimed. Element names remain opaque; dimensions and operations are binary-derived. `_Ufill` dummy dispatch-tag upper bytes are unspecified in the original and follow the existing forwarding-template convention.

Range helpers and four game list/enumeration routines cannot be admitted on inconclusive runs. 0047ef80 is excluded for the 15-argument forwarding-boundary mismatch; the machine call pushes five actual arguments, but current exported/mock ABI observes phantom/unused stack values. No artificial arguments were introduced to force equality.
