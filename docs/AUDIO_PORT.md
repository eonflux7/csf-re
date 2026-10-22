# RenderWare Audio cutover (2026-10-14)

## Finding / acceptance status

**Partial implementation, not a runtime-ready cutover.** All 40 demanded RWA
addresses have providers (5 filesystem, 34 audio/object/dictionary, one DirectSound
import). A silent voice timeline, optional DirectSound PCM output, synchronous
filesystem adapters and bounds-checked bank metadata loading are implemented.
Compressed bank payloads are consumed but not decoded; their cursor timing is
only an estimate from the declared target format. Bank writing/conversion,
complete device/class enumeration and resource scheduling are not complete.
Resolving a slot is not proof that its contract has been fulfilled. Do not deploy
this as a runtime-verified null audio replacement.

No RWA library code or SDK source was used. Sources of the contract: exported
pseudocode, game callers, `db/functions.csv`, boundary REPORT, and packets for
00712920, 0070F630 and 00710BA0. Original exports are read-only. DB names are
not reliable: `Rt2dFontGet` is an FS lookup; `AudioEffects_Enumerate` constructs
an object; `AudioEffect_Release` toggles a voice, not its lifetime.

## Address contract (40 slots)

Prototypes below are **normalized pointer types**, not new DB declarations.
`P=void*`, `W=uint32_t`, `F=int (__cdecl*)(int,int,char*,int)` (allocation
factory), `C=int (__cdecl*)(P,int)` (destruction callback). All entries are
**cdecl**, except the final DirectSound import (**stdcall**, two stack words).
Variadic floats are one raw word, not promoted doubles. Factory callbacks are
not supplied by the observed game calls. Extra ignored stack words at cdecl
sites (notably 007F1850) are not extra required arguments.

| Address | DB name (or descriptive working name) | Prototype | Game-visible contract / caller examples |
|---|---|---|---|
| 007C6040 | Rt2dFontGet [FS lookup] | `P(const char* name)` | Look up registered FS by name, null if absent. 0069FC00, 006A0020. |
| 007C6350 | Rtfs_RegisterFileSystem | `int(P fs)` | Link caller-owned FS; 1 success, 0 duplicate/capacity failure. 0069FC00, 006A0020, 006A01D0. |
| 007C66D0 | RwNodeRelease [FS event] | `int(P fs,int event)` | Event 1 invokes +32 callback, event 2 +36; string at +80. Shutdown 0050D1A0. |
| 007C67D0 | Rtfs_InitializeFileSystem | `int(P fs,int count,const char* name,const char* root)` | Set name/root and initialize file records through +20 accessor. Success exactly 1. 0069FC00, 006A0020. |
| 007C6E90 | RtFs_SetDefaultFileSystem [disk FS factory] | `P(int count,const char* root,const char* name)` | Allocate disk FS and records; caller registers it. 006A01D0. Replacement uses synchronous Windows I/O. |
| 007DB830 | audio engine close | `int()` | Release engine modules/resources; shutdown 0070F2D0. |
| 007DB8B0 | RwResourcesEmptyArena [audio service] | `int()` | Service/reclaim audio resources. 00710DF0, 00710D80; replacement advances voice state. |
| 007DBBA0 | Audio_CreateEffectDictionary | `P(P storage)` | Create empty sound dictionary; observed storage null. 00710AC0, 00710BA0, 0070F4A0. |
| 007DBC20 | AudioEffects_DestroyDictionary | `int(P dict)` | Unlink dictionary and release unshared entries. Replacement currently retains sounds until shutdown. 00710BA0. |
| 007DBD90 | RtDictInsertEntry | `P(P dict,P sound)` | Insert once, return dict or null. Merge 0070F500. |
| 007DBE40 | _rtDictRemoveEntryFromAllDictionaries | `P(P sound)` | Remove sound from every dictionary. 0070F4A0 ignores return. |
| 007DBE90 | GetAudioEffectsDictionary | `P()` | Current sound dictionary (not an effect/DSP dictionary). Used by eight game callers. |
| 007DC410 | obtain class defaults | `P(P class,P output)` | Invoke class +60 defaults callback; output is 40-byte device probe record. 0070EFF0. |
| 007DC630 | find class by GUID | `P(P parent,const W* guid,P* ownerOut)` | Return registered implementation class, optionally containing object. 0070EFF0 uses DirectSound GUID. Replacement simplified, not full class registry. |
| 007DD4C0 | AudioEffect_Create [listener factory] | `P(int flags,int callback,int context)` | Create listener/master effect object. 0070F0F0 writes listener parameters. |
| 007DE320 | prepare resource pools | `int(P device,P* channels,unsigned count)` | Build dispatch/resource pools for device and channel objects. 0070F160 ignores result. Null implementation no-op success. |
| 007DE4E0 | destroy resource pools | `int()` | Release previous pools before channel destruction. 0070F220. |
| 007DEF60 | AudioEffect_ReleaseHandle [object destroy] | `int(P object,C callback,int context)` | Invoke class destructor, unlink object, deallocate appropriately. 00712B10, 0070F280/2A0/2D0. Replacement owns its allocations; callback path not supported. |
| 007DF2B0 | Audio_CreatePlaybackObject | `P(P parent,char* config,int flags,F factory,int context)` | Construct game-visible voice (not a PCM sound). 00712920. |
| 007DF700 | AudioEffect_Release [activate voice] | `void(P voice,int active)` | Enable/disable scheduler; zero stops, nonzero requests playback. 00712310, 007123E0, 00712410. |
| 007DF740 | AudioEffect_ReleaseHandle [attach sound] | `P(P voice,P sound)` | Replace voice's sound +80, copy rate to +60 when appropriate; null detaches. 00712260/290, 00712920. |
| 007DFBD0 | Audio_SetSoundIdentity | `P(P sound,const W* guid,const char* name)` | Copy GUID and owned name; input buffers are stack locals. 0070F630. |
| 007DFF20 | Audio_SetSoundData | `P(P sound,unsigned offset,const void* bytes,unsigned count)` | Upload/convert sound data; caller may free source after load poll. 0070F630. Replacement copies source PCM synchronously; no conversion. |
| 007E0170 | Audio_PollSoundLoad | `int(P sound)` | Nonzero while loading; busy-loop in 0070F630. Replacement returns 0 after synchronous copy. Not a playing-state query. |
| 007E0200 | Audio_WriteEffectEntry | `P(P sound,P stream,P sourceFormat,P outputFormat,int flags)` | Write one serialized sound entry, null failure. 0070F9C0; not implemented, honest failure. |
| 007E0540 | AudioEffects_ReadDictionary [read one entry] | `P(P stream,P class,P device,P format,unsigned flags)` | Consume metadata chunk 2051 and payload chunk 2052; create/insert sound. 00710BA0 already consumes outer 2050. Supports observed flags=0 metadata/name/format loading; compressed payloads remain silent. |
| 007E0D10 | RtGUIDGenerate | `W(W* guid)` | Write 16-byte GUID. 0070F630 ignores return. Uses CoCreateGuid. |
| 007E1320 | Audio_DispatchEffectCommand | `P(P control,int id,int mode,const W* args)` | Dispatch packed parameter block (not integer value argument). 00712A50 passes position triple for id 0/mode 1. |
| 007E1380 | Audio_SetEffectParameter | `P(P control,int id,int mode,...)` | Variadic setter; mode 1, one raw word for scalar parameters. 00712920, 00712680/6C0/6F0, 007127D0, 00712840. |
| 007E13D0 | RpMTEffectGetInt | `int(P control,int id,int fallback,W* valid)` | Get scalar, optional success flag. 00712410/4D0 and 00712630 query cursor id 7. |
| 007E1420 | Audio_FindEffectHandle [control factory] | `P(P parent,char* config,int flags,F factory,int context)` | Construct control object; 00710AC0 creates then destroys, not a named lookup. |
| 007EE4C0 | AudioEffects_Enumerate [channel factory] | `P(P parent,char* config,int flags,F factory,int context)` | Construct channel object, mode 2 in 0070ECD0; not enumeration callback. |
| 007EF3C0 | attach device sound/resource | `P(P channel,P sound)` | Detach previous resource and attach new one; reset multipliers +56/+60. 00710D80 passes null. Simplified replacement. |
| 007EFD80 | DirectSound device factory | `P(P parent,char* config,int flags,F factory,int context)` | Create device from probe record. 0070EFF0. +124 contains borrowed real DS device or failure-only COM facade. |
| 007F0E80 | SoundMax_SetChannelGain | `P(P device,float gain)` | Store master gain at +56 and update active voices. 0070F3B0. Replacement applies a single engine master multiplier. |
| 007F0EE0 | AudioEffect_Register [set listener] | `P(P device,P listener)` | Store listener at +60, update backend if present. 0070F0F0. |
| 007F0F00 | set device output mode | `P(P device,int mode)` | Store +116 and dirty +120 bit 0x100; 0070ECF0. Speaker routing is not implemented. |
| 007F0F30 | _dsoundGetSpeakerConfiguration | `int(P device)` | Query device +124 COM object, map DS speaker constants to game mode. 007108B0. Replacement reports RWA stereo **1**, not 2 (RWA mono). 007108B0 maps 1 to game layout 1; 2 would fall through to invalid layout 0. |
| 007F1850 | Audio_CreateSound | `P(P device,P sourceFormat,P targetFormat,char flags,P storage)` | Create sound using two 28-byte format records; auto-insert in current dictionary. 0070F630 passes additional ignored zero words. Replacement preserves source PCM format. |
| 007F78E0 | DirectSoundEnumerateA | `HRESULT __stdcall(P callback,P context)` | Enumerate DS devices via Windows callback. 0070EF60 tests HRESULT; replacement fails, invokes no callback. |

## Directly accessed layouts

These are game ABI buffers, **not C++ polymorphic classes**. The adapter uses
320-byte zero-initialized buffers with private sidecars; no original vtable is
installed. Pointer offsets assume the mandatory x86 build. The table separates
sound, voice, listener and filesystem: overlapping offsets mean different things.

| Object | Direct fields / game callers |
|---|---|
| Sound entry | +4 owned name (0070F500, 0070FB20); +0x2C..0x47 seven-word format: rate +0x2C, codec pointer +0x30, byte count +0x34, bits byte +0x38, channels byte +0x39, extra data +0x3C and size +0x40; read by 00710920, 0070F9C0, 007127D0, 00712840, 007128F0, 00712590/5D0. Source format +0x10..0x2B. |
| Voice (game wrapper C_00863DD0 +4 points here) | +0x30 control handle; +0x34 flags bit 1 active/physical, bit 2 looping; +0x3C playback rate; +0x40 gain; +0x48 byte cursor; +0x50 sound; +0x58/+0x5C/+0x60 position; +0x70/+0x74 distance limits; +0x88 additional parameter. Writes/reads in 00712920, 007122C0/2D0, 00712310/3C0/E0, 00712410/4D0, 00712630/680/6C0/6F0, 007127D0, 00712840/A0, 00712A50. |
| Listener | +0x64=1, +0x68=0, +0x6C=.01; +0x70 dirty bits 0x10/0x20/0x40, written by 0070F0F0. |
| Probe output | 40 bytes; +8 unavailable flag, +12 usable, +20 output Hz, +24 bits, +26 voice capacity, +36 set to 1 by 0070EFF0. Replacement initializes every byte, sets 48000/16/32. **+26 is not speaker mode**: 0070EFF0 copies it to wrapper +0x34, 0070ECC0 subtracts six to obtain channel count used by 0070F160. The original probe 007F0C80 also has a 32-channel fallback. |
| Device/channel | Device +0x38 master gain, +0x3C listener, +0x74 output mode, +0x78 dirty bits; channel +0x84 attached sound and +0x38/+0x3C multipliers. Game 00710FF0 reads device **+0x7C** and calls its COM vtable slot +12 (`CreateSoundBuffer`) directly, independently of the 40 functions. Null returns a process-lifetime failure-only IDirectSound8 facade here (not null); real mode supplies its borrowed device. No fake successful buffer is returned. |
| Custom FS | 0x5C bytes, +0 linked list, +4 count, +8 state, +12 owned root, +16 mode, callbacks +20 accessor/+24 destroy/+28 filename/+32 register-event/+36 unregister-event/+40 open/+44 close/+48 read/+52 write/+56 seek/+60 sync/+64 abort/+68 EOF/+72 status/+76 exists; +80 name, +88 records. 0069FC00 fills 64-byte records; 006A0020 fills 72-byte records; each +56 owner, +32 state. 0050D1A0 walks list, calls +24 destructor, frees +12 and FS via RW allocator. Root allocation uses matching RW engine +264 allocator. |
| Default disk FS | 96-byte header, records pointer +92, sector alignment +88; 96-byte records, +0/+4 size low/high, +8/+12 cursor low/high, +24 async mode, +32 state, +56 owner, +64 HANDLE. Replacement reads/writes synchronously (including async requests), returns completed status, retains size/cursor. +72 is status, confirmed by original binary disassembly at 007C6CC0. |
| Dictionary | Returned objects are opaque to emitted game code: access via named/index/count helpers. Original embedded list +12/+16 is initialized empty, but the replacement helpers use sidecar lists. Never mix with original RW dictionary implementations. |

Additional **misclassified rw audio** providers are therefore installed for
007DB6F0/820/920/960, 007DBD20/70/80, 007DBE70/80, 007DE550/590, 007F17C0/1810 and
007F78E6/F2. These are not implementations of RenderWare graphics. The database
is deliberately unchanged. DirectSoundCaptureCreate (007F78EC, caller 007116F0)
remains unresolved under rw; null capture and audio targets reached only through
generated vtables need a later audit. 007F7900 is a graphics Rt2dObjectDestroy,
not audio despite being next to the DS import thunks.

## Polling, threads and callbacks

007123C0 directly tests voice +0x34 bit 1; 00712630 and 00712410/4D0 query id 7
when active. 00710DF0 calls audio service before 00712050 polls each game sound
wrapper's virtual finished predicate (+0x14) and destroys finished sounds.
Silent PCM voices expire after byte-count / (playback-Hz * channels * bits/8); looping
voices wrap. Clock advances at service/query, not a worker thread. The game must
continue its normal audio service calls for direct flag polling to see completion.
Timeline tests cover completion, looping, pause/resume, rate changes, loop exit,
end-offset clamping and 32-bit clock wrap. Parameter 4 retimes without losing the
current cursor; parameter 8 rebases at the current loop cursor before changing
loop mode. DirectSound buffer frequency is updated too. Unsupported compressed
formats do not yet have codec-correct cursor/duration semantics.

The music path is separately threaded: 0070EA70 calls `_beginthread` with entry
0070EA60, which forwards to 0070E970. 0070E970 opens a decoder and enters
0070E830, with stop/pause/loop flags and 15 ms sleeps; 0070E4A0 decodes PCM,
0070E550 feeds a buffer. RWA itself contains a semaphore/threaded resource
dispatcher (e.g. 007F0E80); a complete callback callgraph audit is still open.
Adapter object operations are serialized by a recursive mutex; FS globals are
not thread-safe yet. No replacement audio worker is started. Voice enumeration
007DE550/590 invokes game callbacks synchronously on the calling thread, stops
on zero return, and tolerates callback mutation via snapshot iteration.
Original factory/destructor callbacks, where non-null, require a later
ownership audit. FS registration/unregistration does invoke supplied callbacks.
Do not assert the game's whole audio subsystem is single-threaded.

## Files / codecs

* 0070F630 opens a game file with `rb`, calls 00712B70's RIFF/WAVE parser,
  requires 16-bit input, reads PCM into aligned memory, creates a sound, uploads,
  busy-polls load completion, and frees input. Source channels/rate/bytes come
  from the WAV header; requested target rate/codec come from 008A85A8/B0/B4.
  Target conversion is not implemented. DirectSound output uses source PCM.
* 00710BA0 loads RenderWare streams, outer chunk **2050 (0x802)**, with metadata
  **2051 (0x803)** and payload **2052 (0x804)** per sound; 0070F9C0 writes entries.
  Original metadata contains two 28-byte format records plus optional codec GUID,
  extra format bytes, sound GUID, name, device/class GUID. This is not just WAV.
  The null reader validates variable GUID/extra/name sizes and keeps both declared
  formats and owned identity/name. It consumes the whole payload to keep subsequent
  entry alignment. The real backend retains payload only for identical source/target
  PCM formats whose serialized codec GUID matches the WAV loader's PCM identity.
  No extracted bank fixtures were supplied; synthetic metadata tests do not prove
  compatibility with retail banks. Compressed decoding and writing are deferred.
* Filename evidence: 0050E7C0 uses `%s\\Sonidos.bdd` (sound definitions/database,
  not itself proven PCM); config registrations 00756C80 `.sMusicaMenus` and
  `.sMusicaCreditos` use `*.rws`; 007770C0 also uses `*.rws`. Music code actually
  invokes Vorbis on a Windows HANDLE supplied by its configurable path, so an
  extension is not a guarantee of the encoding. No disc inspection was performed.
* The six **NetZ-misclassified** imports are 0081DD16 `ov_read`, 0081DD1C
  `ov_clear`, 0081DD22 `ov_time_total`, 0081DD28 `ov_open_callbacks`, 0081DD2E
  `ov_pcm_seek_lap`, 0081DD34 `ov_info`. **Correction to the task premise:**
  PE inspection with `pefile.PE('work/pc/CommXPC.exe')` confirms these are import
  thunks into **vorbisfile.dll**, not a statically linked Vorbis decoder in this
  executable. IAT addresses: ov_clear 0082D3E8, ov_time_total 0082D3EC,
  ov_open_callbacks 0082D3F0, ov_info 0082D3F4, ov_pcm_seek_lap 0082D3F8,
  ov_read 0082D3FC. The game calls them **directly**, in 0070E4A0/600/830/970,
  independent of RWA. Decoder storage is 720 bytes; `ov_info` fields +4 channels,
  +8 rate. Callbacks read/seek/close/tell are game functions 0070DF60/FB0,
  0070E010/020. The null boundary fails open with OV_ENOTVORBIS (-132), never
  fabricates decoder state; observed callers close their HANDLE on failure.

The six auxiliary decode slots are cdecl with the x86 Vorbisfile ABI:
`ov_read(P,char*,int length,int bigEndian,int wordBytes,int signedSamples,int*)`
returns signed long (32-bit); `ov_clear(P)` and `ov_pcm_seek_lap(P,int64_t)` return
int; `ov_time_total(P,int link)` returns double in x87 ST0; `ov_info(P,int link)`
returns a borrowed info pointer; `ov_open_callbacks(P source,P decoder,const char*,
long initialBytes,Callbacks)` returns int and receives the four-pointer callbacks
record **by value**. Read callback is `size_t(P,size_t,size_t,P)`, seek is
`int(P,int64_t,int)`, close is `int(P)`, tell is `long(P)`. The game owns HANDLE and
decoder storage, but a successful decoder takes responsibility for the close
callback at clear; failed open leaves HANDLE closure to the caller. The null
failure path does not invoke any callback or touch decoder storage.

## Backend design and build feasibility

`backend.h` exposes PCM create/destroy, play(offset,loop), stop, gain, frequency;
plus a borrowed device handle for the game's separate direct-COM buffer path. The adapter
owns game ABI state and a separate clock timeline. No backend sees game pointers
or invokes game callbacks. `Null` makes no device and runs the same timeline.
`CSF_AUDIO_BACKEND=dsound` opts into source-PCM one-shot output; default is null.
DSound dynamically loads `dsound.dll`, no new import-library dependency. Device
or buffer failures leave the game timeline silent. Gains use DS logarithmic units.
No spatialization, device loss recovery, compressed decoding, stream mixing or
sample-rate conversion is claimed. Pitch and a single master gain are implemented,
not runtime-tested. Null COM buffer creation fails honestly instead of allowing
the direct-COM music path to dereference a null device.
The backend/device singleton is deliberately process-lifetime (avoids static
teardown ordering with ABI sidecars); voice buffers are released on destruction
or engine close. DLL/backend unloading before process exit is not implemented.

The configured clang-cl sysroot is `~/.xwin`. SDK headers **dsound.h,
xaudio2.h, audioclient.h** and `sdk/lib/um/x86/dsound.lib` are present. DirectSound
was chosen because it provides PCM secondary buffers with this existing toolchain;
WASAPI/XAudio2 would need more device/COM/event plumbing. `mmsystem.h` must precede
`dsound.h` with WIN32_LEAN_AND_MEAN. The actual DSound implementation is compiled
with linktest flags. Header availability alone is not runtime verification.

A later vendoring job may add miniaudio (licensed, pinned source), libogg/libvorbis
or stb_vorbis (include license and revision, build x86 offline). It must replace
the six import adapters with genuine decode state or a sidecar API preserving
caller ownership/callback close behavior; never map modern OggVorbis_File over
the game's 720-byte buffer without checking layout. Supply PCM/WAV, compressed
bank and music fixtures and tests, fix all remaining rw-labeled audio helpers,
then run game startup/menu/mission/pause/load/shutdown checks. No download,
network, game, smoke or verifier was used here.

## Remaining blockers

1. Execute disk FS callback tests, confirm synchronous substitution is acceptable
   for async requests; FS registry threading and >4 GB seeks remain limited.
2. Bank writing/conversion and compressed codecs; retail fixture tests. Metadata
   read support is implemented, but codec-specific duration/cursor units are unproven.
3. Complete class/device GUID enumeration, resource scheduling and callback ownership;
   callback factories/destructors with non-null arguments are not supported. All
   observed emitted game callers pass null. Audit indirect/data-only library slots
   before startup is claimed. The replacement class lookup currently ignores GUID.
4. Spatialization, multi-device master gain semantics and state transitions under
   real game polling. Test pitch/loop/pause/seek with actual DirectSound output.
5. Music decode/streaming: null imports intentionally reject it, not silently play
   a finite-duration music stream. DirectSound one-shots are build-tested only.
6. Execute adapter integration tests and real device/game tests. Current native
   tests exercise Timeline and synthetic bank metadata, not the complete address
   ABI/object ownership. No Wine is installed and native x86 C++ execution is blocked
   by missing 32-bit runtime libraries; x86 Windows compilation is available.

`src/boundary/rwa/contract.json` retains the DB names, verbatim exported
prototypes and complete caller-file lists for each of the 40. Counts there are
caller files; they match the reconstructed baseline linktest reference-file
counts for all 40 in this tree (12 for the parameter setter, 8 current dictionary,
6 object destruction, 5 each activation/attachment). 32 demanded slots have DB
names; unnamed slots use descriptive working names in the table.

## Offline verification (this resumed job)

The cached result on entry already included the interrupted job's providers, so
the **before** baseline was reconstructed by calling `linktest.run(8)` with only
`src/boundary/rwa/` filtered out of `linktest.sources()` (no tool/source edits).
Both runs scanned all other sources in this same tree. Full results are in
`work/rwa_baseline.json`, `work/rwa_slots_before.txt`, `work/rwa_linktest_after.txt`.

Before:
```
compiled 14363/14363 files standalone; 14515 functions implemented
unresolved functions: game 99, netz 222, not-a-function 7, png 1, rw 400, rwa 40
```

After (`.venv/bin/python tools/linktest.py -j 8`):
```
compiled 14367/14367 files standalone; 14576 functions implemented
unresolved functions: game 99, netz 216, not-a-function 7, png 1, rw 386
data to generate: 2649 globals, 1113 vtables; raw image addresses in code: 0 in 0 files; asm thunks with original addresses: 0 files; other undefined symbols: 50
```

RWA is **zero unresolved**, no duplicate definitions or compile failures. The 61
supplied slots are the 40 requested plus six Vorbis and 15 shared audio helpers
(one helper is data-only, not demanded by emitted object refs). Every new C++ TU
also compiled individually with `linktest.flags()`; these checks caught/fixed a
pointer cast and an SDK constant spelling before the final full run. The boundary
is standalone-only: compile all four TUs without `-DCSF_STANDALONE` then inspect
with `llvm-nm-21` shows zero defined and undefined program symbols each.

`cmake --build build -j 8` successfully linked `dinput8.dll`; repeat reports
`ninja: no work to do`. CMake already excludes `/boundary/`, so this does not
install the replacement over original library objects in the hooked game.

`g++ -std=c++17 -Wall -Wextra -pedantic tests/rwa_bank.cpp -o /tmp/rwa_bank &&
/tmp/rwa_bank` and the analogous `rwa_timeline.cpp` command pass. Both also pass
ASan/UBSan (`-fsanitize=address,undefined -fno-omit-frame-pointer`, run with
`ASAN_OPTIONS=detect_leaks=0`); LeakSanitizer itself cannot run under this sandbox's
ptrace, so leak checking is not claimed. These are **native helper tests**, not
full x86 adapter/device integration. `git diff HEAD --check` is clean.

No game, smoke, verifier, forbidden IDA writes, network, downloads or commit was
used. No db/, queue/, PLAN or JOURNAL files were changed.
