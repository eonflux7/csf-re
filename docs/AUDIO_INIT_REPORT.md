# Standalone audio failure follow-up

## Findings

The intermittent cause is a generated-data relocation of **"win\0"** into a
function pointer. Two downstream defects are fixed. The remaining fix is one
text-manifest row, supplied below rather than applied because workers cannot
edit db/. No Windows execution is claimed.

### Garbage message: destination layout, not a demonstrated language-table bug

0069CE50 passes a 28-byte VC7.1 wstring at original frame `ebp-20` to
006966E0. The old C++ instead passed `&formatDestination`, a four-byte local,
and assumed separate `lpText` and `bufferCapacity` locals followed it. That
layout is not guaranteed. The formatter writes beyond that local and the
dialog reads different storage. A real `legacy::wstring` now owns the inline
buffer/pointer, length and capacity together. Formatting, MessageBoxW flags,
heap cleanup and original return behavior are retained.

The targeted fixture fails the pre-change COFF's destination contract and
passes the replacement for inline, heap and null-heap results. This proves
the layout defect independently of localization data. No language-table or
datagen change is justified by this evidence; actual g208 text still needs
the next Windows run.

### UNRESOLVED 00523920: secondary-base deleting destructor

The original bytes are `sub ecx,4; jmp 005240F0`. 00523930 is the corresponding
`sub ecx,8` entry. They belong to the sound manager's secondary-base vtables
at 0084106C/00841064. 005240F0 calls 005238B0 to reset/release the manager,
frees the complete object through 00691110 only when `flags & 1`, and returns
the complete-object pointer, with `ret 4`.

All three slots are supplied in standalone/game_plain.cpp. The adjustors are
literal naked tail thunks, preserving the incoming arguments/registers. No
success/no-op substitute was used. Metadata follow-up (not edited here): game
entries 00523920 and 00523930, size 8 each; 005240F0, size 31, ending 0052410F.

### Intermittent initialization: ASLR-dependent filesystem name corruption

Original 008A6BA4 points at 0085EC30, the string `77 69 6E 00` ("win\0").
Its four bytes also read as the code address **006E6977**. IDA's exported
data-xref marks it as a pointer. The generated `datagen-pointers.json`
explicitly reports `source=85EC30, target=6E6977, confirmed=true, applied=true`;
generated initialization therefore replaces all four string bytes with the
address of a fatal trap. This overwrites the NUL terminator too.

The next word at 0085EC34 is the first vtable pointer (006A0280), also relocated.
ASLR thus determines where the first NUL occurs in these adjacent pointer
bytes. The linked map and two simulated load bases demonstrate:
at base 00400000 the corrupted name has length 3; at base 00C00000 it has
length at least 8. RtFS `initFS` rejects names of length >=8. The "win" setup
006A01D0 returns false, 006971B0 propagates false, and 00710AC0 fails before
any audio-device probe. Successful runs can register a garbage but short name,
which explains why the same image can sometimes continue.

**Required fix for the orchestrator:** append to db/datagen_text.yaml, then
rebuild/regenerate the standalone data:

```yaml
- address: 0x0085EC30
  text: "win"  # 0x006E6977 is text, not a code pointer; RtFS system name.
```

This uses the existing explicit short-string policy, just like bmp/png; do not
globally exempt all text-shaped words (real pointer-table entries can look
printable too). `tests/audio1_datagen.py` generates both the current image and
an in-memory version of this exact override and checks that the latter does
not apply the erroneous fixup. No db/ file was changed.

The audit also independently excludes a normal DirectSound failure:

00710AC0 checks engine open, 006971B0 filesystem setup, device creation,
listener creation, and 0070F160's positive voice count. In the current boundary:

* Engine open always returns 1; the two following support initializers are
  unconditional success and their results are not tested by the game.
* Device/listener creation returns owned objects for these null-factory calls.
  DirectSound backend failure falls back to the silent COM facade; it does
  not return a null RWA device. The import enumeration is a deterministic
  E_NOTIMPL stub, not a timing-dependent device enumeration.
* Defaults write voice count 32; 0070ECC0 subtracts 6, yielding positive 26.
* The speaker query is **after all failure tests**, so it cannot explain the
  false return. The RWA adapter has no worker thread; RtFS host IO is synchronous.

The ASLR model proves a concrete corruption and its failure mechanism, not the
actual load bases of the user's failing/successful runs or the one-in-three
frequency. Runtime confirmation remains useful; additional unrelated failures
are not excluded. No speculative retry or forced success was added.

## Next Windows run / requested evidence

Run `CSF_RW_TRACE=1 .venv/bin/python tools/dryrun.py --full` and retain the
lines from `RWA startup ... open` through the message box, both failing and
successful if available. CSF_RW_TRACE is already forwarded by dryrun.

Expected successful sequence: RtFS manager open; win/vfs/asyn init and
registration returning 1; default set; RWA defaults voices=32; nonnull device
and listener; prepare detail=26. A failure before defaults identifies
filesystem setup. Traces show requested name/device/count, init result,
registration state, duplicate manager open and boundary allocation failures.
If a vfs/asyn name lookup is followed by failure without its init call, capture
the return of game allocator 00690EB0: whether 0069E3A0's wait succeeded and
whether 0069E020 returned null. If listener exists but prepare is missing,
capture 0070ECC0's EAX and the effect object's +34 voice-count field.

With this worker patch alone, the failing run should show the corrupted win
name, readable localized g208 with the same ERROR caption, and teardown past
00523920/00523930/005240F0. **After applying the manifest row**, the trace should
show `name="win"`, init/register returning 1 regardless of ASLR; this particular
false return should disappear. These are expectations, not Windows results.

## Checks

* `tools/gate.py 0069ce50 src/auto/0069ce50.cpp`: ok.
* `tools/emudiff.py 0069ce50 --src src/auto/0069ce50.cpp`: same, 8/8 profiles.
* `tests/audio1_offline.py`: 3 dialog and 15 destructor original-byte/COFF
  cases pass (explicit formatter/reset/free doubles; stack/nonvolatiles,
  displayed text, call order and returns). Pre-change dialog COFF fails.
* `tests/audio1_datagen.py`: verifies the bad current fixup, protection from
  the proposed manifest row, and the linked-map ASLR name-length model.
* All nine `csf_rw_{cpu,life,image,object,device,toolkit9,plugins,particles,fs}_link`
  native fixtures pass with ASan/UBSan; fs also passes with tracing enabled.
* RWA timeline and bank native helpers pass with ASan/UBSan. These do not
  exercise the complete Windows/x86 audio adapter or real devices.
* `tools/linktest.py -j 8`: 14869/14869, no compilation failures, duplicates,
  raw image addresses or raw-address asm thunks. Changed TUs compiled locally;
  10596 unchanged COFFs reused from the parent cache after matching source,
  normalized flags, headers and parent freshness.
* `cmake --build build --target csf_standalone -j 8`: links the exe; rebuilt
  after final source state. Supplied destructor slots absent from fatal traps.
* Extra `tests/test_datagen.py`: 4/6 pass; two assertions still assume no
  reinterpret_cast anywhere in generated source and confirmed-only fixups,
  contrary to the current IAT/default-heuristic generator. Neither generator
  nor those tests was changed. This is not a datagen-suite pass.

No Windows/game execution, library-source port, db/queue/PLAN/JOURNAL edit or
commit. Logs are under work/audio1/; focused fixtures are maintained source.
