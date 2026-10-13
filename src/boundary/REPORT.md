# Cutover boundary report

**Partial completion:** all 51 CRT slots demanded by this worktree are resolved;
99/321 NetZ slots are supplied (94 generated, five allocator shims). 222 remain
deliberately unresolved rather than returning unsafe fabricated objects. The
allocator routes ordinary global C++ allocation to the existing game heap, but
known caller ABI and startup-order issues prevent claiming runtime readiness.

## Verified

Fresh baseline (`.venv/bin/python tools/linktest.py -j 8`):

```
compiled 14137/14137 files standalone; 14142 functions implemented
unresolved functions: crt 51, game 211, netz 321, not-a-function 7, png 1, rw 399, rwa 40
data to generate: 2642 globals, 1108 vtables; raw image addresses in code: 2 in 1 files; asm thunks with original addresses: 1 files; other undefined symbols: 21
```

After (`.venv/bin/python tools/linktest.py -j 8`):

```
compiled 14141/14141 files standalone; 14293 functions implemented
unresolved functions: game 211, netz 222, not-a-function 7, png 1, rw 399, rwa 40
data to generate: 2642 globals, 1108 vtables; raw image addresses in code: 2 in 1 files; asm thunks with original addresses: 1 files; other undefined symbols: 50
```

No duplicate definitions or compile failures. The final CRT unresolved count is
zero. The task's cached 49 CRT demand was out of date here: the fresh baseline
also demands the two string error throwers. CRT source supplies 52 address slots
(one additional EH entry is not currently demanded by emitted object references).

Other checks:

* `.venv/bin/python tools/netzstub.py --check --object build/standalone/src_boundary_netz_stubs.cpp.obj`:
  reproducible generation; **102 actual COFF entry/virtual cleanup wrappers**
  agree with the binary's ret N. 94 generated entries, five alloc-owned, 222 deferred.
* `linktest.compile_one(linktest.flags(), ...)` and the full link test compile
  all four boundary translation units using the required standalone flags.
* Compile those four files again with `-DCSF_STANDALONE` removed, then use
  `llvm-nm-21`: each guarded object has no defined/undefined program symbols.
  CMake also excludes `/boundary/` from csf_hook's glob.
* `linktest.raw_addresses(...)`: no raw image addresses in any new C++ file;
  the full report's pre-existing raw/asm counts are unchanged.
* `llvm-nm-21` on actual xwin libcmt/libvcruntime/libcpmt/libucrt archives:
  `_ftol2`, the legacy Cxx handler/throw entry, `_CIasin/_CIpow/_CIfmod`, stack
  prologs/probe, memset and security checker are exported by the assumed CRT.
  **48 of the 50 reported other symbols have real CRT providers.** The remaining
  two are hook::log and hook::ini_string, not CRT routines. The scanner caps the
  other-symbol report at 50; forwarding legitimately adds ordinary CRT refs.
* Packets read with `tools/packet.py` for the game heap frontends/inner alloc-free;
  original binary disassembly used for the NetZ header/free convention and the
  audited Gathering constructor/vtable. Exports and original image were read-only.
* `git diff --check`: clean.

Emudiff was also run after the three guarded auto-file edits:

```
emudiff.py 00411FB0 --src src/auto/00411fb0.cpp: DIFF 5/8 profiles (call/ecx)
emudiff.py 00740AF0 --src src/auto/00740af0.cpp: DIFF 8/8 profiles (6 calls vs 7)
emudiff.py 00740B40 --src src/auto/00740b40.cpp: DIFF 8/8 profiles (6 calls vs 7)
```

These are **not passes** and do not verify standalone math correctness. The new
branches are standalone-only; hooked behaviour was intentionally left unchanged.

## Decisions / conclusions not execution-verified

* CRT: /MT UCRT + vcruntime/libcpmt, not msvcr71. Naked forwarding preserves
  compiler helper ABIs. Modern exception and formatting/locale/FP/FILE behaviour
  has compatibility limits; details and addresses are in crt/README.md.
* Alloc: 10 ordinary global operator overloads supplied; existing game-owned
  allocation/free entry points retained, so no duplicate game slots. Five NetZ
  alloc/free entry points deliberately discard obsolete NetZ headers/callbacks
  and use the game heap together. Backing CRT malloc/free stay real CRT calls,
  avoiding recursion. See alloc/README.md's ownership/routing table.
* NetZ: byte predicates/operations reject with zero, void operations no-op,
  counts/predicates zero, last-result nonzero error. One audited constructor
  builds a genuine offline object with ABI-specific virtuals; its virtual factory
  also constructs an object and its class-name method initializes owned output.
  The family table is in netz/README.md. This is not evidence SP startup works.
* NetZ export/contract/binary disagree in some places; the generator proves the
  actual cleanup or defers. Six alleged NetZ entries are Vorbis audio imports.
  Unknown constructors/pointer/ID returns are not safely covered by one null
  return policy or dummy vtable. Every deferred address/reason/caller is retained.

## Changed files

* `src/boundary/crt/crt.cpp`, `crt/README.md`.
* `src/boundary/alloc/operators.cpp`, `alloc/netz_heap.cpp`, `alloc/README.md`.
* `src/boundary/netz/stubs.cpp`, `targets.json`, `manifest.json`, `README.md`.
* `tools/netzstub.py`; `CMakeLists.txt` excludes boundary sources from hooked DLL.
* `src/auto/00411fb0.cpp`: standalone scalar asin instead of a wrongly
  stack-prototyped x87 helper call.
* `src/auto/00740af0.cpp`, `00740b40.cpp`: omit standalone dummy calls into
  _CIfmod without x87 operands; the scalar fmod computation remains.
* This report. No db/, queue/, PLAN or JOURNAL changes; no commit or exe link.

## Still open

1. **222 NetZ entries**: constructors/layout/ownership, pointer and ID results,
   shared/non-network helpers, indirect tails and the Vorbis imports. 84 result
   hazard addresses and caller lists are marked in manifest.json; this is a
   conservative review list, not complete result data-flow proof.
2. **Existing allocator callers are wrong in places.** For example 006a7000
   groups node/kind into get-manager then passes only the manager to the
   three-argument node free. A correct boundary cannot recover the missing block.
   Audit/fix the recorded allocator caller declarations before those paths run.
3. Game heap/mutex initialization before dynamic C++ static allocations;
   over-aligned C++ operators; the pre-existing inner alignment forwarding.
4. Modern FILE/locale/format/FP differences, original exception object/catch/EH
   metadata compatibility and remaining original-address naked thunks.
5. Standalone providers for hook::log / hook::ini_string, generated data, other
   library boundaries, and runtime/SP testing belong to later cutover work.

No game, smoke, verifier, forbidden IDA writes, network or exe link was used.
