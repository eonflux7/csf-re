# Mission loading: offline investigation (2026-10-15)

## Parser stop: catalog text mistaken for code

The reported chain is `504970 -> 503000 -> 4FE010 -> 46BC40 -> 46EAD0 ->
46E910 -> 46F970 -> 46F6C0 -> 46F210`. The faulting read compares the byte
at `candidate->value->payload` with `target->value->payload`:

- entry +2 is `FFFF` (leaf); entry +4 points at the 12-byte value object;
- value +8 is `-1` (unresolved constant); value +4 must point at its typed
  payload. `46F210` checks the value pointer, **not** the payload pointer.
- Original instructions `46F265..46F26D` load both payload pointers and read
  their first bytes. A null guard here would hide the earlier parser defect.

The catalog constructor `76A540` used `CSF_ADDR_DATA(8718C4)` for the name
`VAR`. The bytes `56 41 52 00` look like the code address `00524156`.
The pre-fix generated `data.cpp` contained:

```
put(280772, fn_slot<0x00524156>::p); // image offset of 8718C4
put(280668, fn_slot<0x004D4E41>::p); // 87185C: ANM
put(280224, fn_slot<0x00534246>::p); // 8716A0: FBS
```

Thus catalog registration stored a pointer to relocated pointer bytes, not
`VAR`. `7692F0` falls back to the first definition (`NO_FUNCTION`, kind 2,
opcode 0). `46E590`'s nonzero-kind/default branch calls `46F2B0(0)` only;
that constructs a value with +8=-1 and +4=null, without `4725C0` to allocate
its payload. A child finalized through `46F970` can then reach the failing
comparison. A recognized VAR instead uses opcode 10, finds its variable
definition, reserves the typed payload, and sets value +8 to the variable ID.

Fix: `gen_script_catalog.py` now reads the known name argument of each
registration directly from original bytes, validates printable text, and
emits `CSF_STR`. Regenerated `0076a540.cpp` covers every catalog name (not
just VAR); standalone literals bypass ambiguous data fixups. Original-hook
builds still use the original string address. No change to exports or db.
The generator also retains a TU-independent ABI for the reserve callee,
whose current exported signature refers to a local `Vector` alias.

`tests/mission_catalog_offline.py` runs both actual x86 constructors, mocks
registration calls, and compares all 422 names. Before running the compiled
constructor it overwrites VAR/ANM/FBS's original data words, reproducing the
relocation hazard. It then runs the original lookup and `46E590` branches:
corrupt VAR name -> fallback -> null payload; correct VAR -> typed payload.
Only allocation, component/variable lookup and registration are doubled.
This establishes the failure mechanism offline; it does not establish that
this was the only malformed expression in the Windows run.

### Assets and paths

`4FE010` replaces the scene filename extension with `.gsc`, calls `46BC40`,
then uses `.csc` for `444890`. The assets are not expected as loose GSCs in
GlobalEK: `/mnt/e/dev/csf-builds/csf-pc-retail/Maps/Parachut.pak` contains
`Maps/FR01/Parachut.scn` (387143 bytes), `.gsc` (953175), `.csc` (14929), and
`Maps/FR01/FR01.rws` (11029370).

Independently decoded the GSC from the retail PAK index and 233 zlib blocks;
it exactly matches `work/mission1/Parachut.gsc`, SHA256
`5a04d8cb6b8c2bb04767eaa6975656826d4efe74dccab846551fbf19bf4413e9`.
The packed-tree decoder consumes all 953175 bytes, and `check_expr.py` checks
the valid scripts against 422 catalog definitions: no unknown expressions,
wrong arities, or non-expression children. This argues against missing/short
retail data, but does not measure the Windows reader. Retained `SCRIPT loaded`
path/size/root, invalid-leaf context, and payload-allocation-failure diagnostics
allow that remaining possibility to be checked on the next run.

## Orphan stop: 537490 and tail 536F20

`537490` occurs in .rdata slots `842034` and `84371C`. It is a leave/cleanup
slot of the page's secondary interface, whose
`this` is four bytes past the main page. It resets application and scene
state, releases managers, resets controls on `this-4`, selects the next
scenario and tail-jumps to `536F20` with the main page pointer. `536F20`
closes the loading page and selects the subsequent ready/intro/return/
special/confirmation/default page. Both entries were absent from IDA's
functions and are now hooks in `standalone/game_plain.cpp`.

Argument evidence: `542720` is argumentless cdecl despite its unused ECX;
`501990 ret 8` consumes scenario and flag pushed before the getter;
`69BD90 ret 8` consumes 4 and the empty path pushed before `69B700`;
the intro args pointer survives the menu Find call (`ret 4`) and belongs
to `6F2850 ret 4`. Original addresses use binding macros; there are no new
dynamic initializers.

`tests/mission1_offline.py` compares original bytes with compiled bodies for
both entries, eight modes each, using doubles for direct/virtual callees.
It checks call order/arguments, data writes, ESP and callee-saved registers.
All 16 cases pass. This is not a Windows leave-level/playthrough test.

## Remaining static mission-load boundary

`initreach.py --root 0x504970 --vtables` reaches 6774 functions and 80 trap
slots: 47 RW, 32 NetZ, one CRT (`__purecall`). No game trap is in this upper
bound. Direct bindings in the new leave implementation also have no trap
targets. `--root 0x46BC40 --vtables` reaches 392 functions and only the
`__purecall` error path (`46BC40 > 694770 > 698350 > 6A02B0 > 6A0500`).

The complete address/name/path list follows below. It includes conditional,
cleanup, rendering and multiplayer branches, and taking callback addresses;
it is **not** a claim all 80 execute during first-mission loading. Conversely,
initreach only follows source bindings and explicitly installed vtables, not
arbitrary boundary-to-game callbacks or data-resident function pointers.

Streaming specifically follows `504970 > 503EF0 > 6CDAA0 > 6BBFF0 >
6BF020 > 6BEC60`. `6BEC60` switches on stream chunk IDs: WORLD (0xB)
calls still-trapping `59FEA0 RpWorldStreamRead`; CLUMP (0x10) calls implemented
`58FFC0`, or queues buffered reads. `6BF020` creates an empty world when
needed, then calls still-trapping `6329C0 RwFrameSetStaticPlugins`.
`world.cpp` supports empty-world creation/membership, not streamed BSP worlds.
`clump.cpp` reads on librw, adopts facade objects, publishes plugins, calls
registered game read callbacks via `readPluginPayload`, and rolls back failures;
only Read/GetSize are implemented, not Write. The read path itself contains no
explicit unresolved game call; actual native/plugin resource use remains a
runtime boundary to exercise. `58E680` is an atomic render callback, not
necessarily a load-time call. Library code has not been reimplemented here.

## Next Windows run

Run `tools/dryrun.py --full --input mission` with the fresh exe/map. Expected:
VAR/ANM/FBS catalog names remain text, the diagnosed null-leaf parser fault
disappears, and neither 537490 nor 536F20 traps. Mission entry itself is **not
verified**; the next stop can be a boundary trap (notably 6329C0, a conditional
59FEA0, animation/skin/PVS/collision operations), another parser issue, or a
resource failure. Capture the SCRIPT lines and first new exception/trap.
For any remaining parser fault, retain parent/child, nodeType, opcode,
definition and allocation diagnostic lines; the loaded path/size should be
`Maps/FR01/Parachut.gsc`, 953175 for the retail script investigated here.

## Checks

- `tools/gate.py EA src/auto/EA.cpp`: 46BC40, 46E910, 4725C0, 76A540: all ok.
- `tools/emudiff.py 0076a540 --src src/auto/0076a540.cpp`: same, 7/8 profiles.
- `tools/linktest.py -j 8`: succeeds, no raw-image-address files;
  `cmake --build build --target csf_standalone -j 8`: links successfully.
- `tests/mission1_offline.py`, `tests/mission_catalog_offline.py`,
  `tests/wmrest_offline.py`, `tests/audio1_offline.py`: all pass (16 orphan
  cases, 422 catalog names plus original-parser regression, 9 wmrest cases,
  18 audio cases).
- Whole-script Unicorn attempts were capped/incomplete; not counted as passes.

## Static trap inventory

```
from 0x00504970: 6774 functions reached; trap slots {'crt': 1, 'rw': 47, 'netz': 32}
crt            0x007AEA58 __purecall                               via 504970 > 504570 > 44F380 > 4DEE90 > 6B1E50 > 6B1AF0 > 7AEA58
netz           0x0068E2B0 NetCogibleDdl_InitializeVector           via 504970 > 503EF0 > 4FF500 > 4FF020 > 68E2B0
netz           0x00660630 TreeMap_MoveIteratorToPrevious_660630    via 504970 > 503EF0 > 501360 > 6B7F70 > 6B7E80 > 6B7D20 > 660630
netz           0x00665E40 GfxWorld_IncrementTreeIterator           via 504970 > 503EF0 > 6CDAA0 > 6B6AD0 > 6EB950 > 6B7F00 > 6B7A00 > 6B7670 > 665E40
netz           0x00672AC0 ResolveSceneEntryValue                   via 504970 > 4565D0 > 455F40 > 46ADA0 > 469550 > 468310 > 467BC0 > 465310 > 464720 > 672AC0
netz           0x007A2D60 <unnamed>                                via 504970 > 500AE0 > 5004E0 > 4EEEF0 > 4EEB90 > 4E8EA0 > 49C3D0 > 49C030 > 4A2490 > 740A60 > 7A2D60
netz           0x0068E2E0 ScratchObj_CopyFromSource                via 504970 > 500AE0 > 5004E0 > 4EEEF0 > 4EEB90 > 4E8EA0 > 49C3D0 > 49C030 > 4A2490 > 7409D0 > 68E2E0
netz           0x007A2A40 <unnamed>                                via 504970 > 500AE0 > 5004E0 > 4EEEF0 > 4EEB90 > 4E8EA0 > 49C3D0 > 49C030 > 4A2490 > 7409D0 > 7A2A40
netz           0x0068E2C0 <unnamed>                                via 504970 > 500AE0 > 5004E0 > 4EEEF0 > 4EEB90 > 4E8EA0 > 49C3D0 > 49C030 > 4A2490 > 7409D0 > 68E2C0
netz           0x007A2AD0 <unnamed>                                via 504970 > 500AE0 > 5004E0 > 4EEEF0 > 4EEB90 > 4E8EA0 > 49C3D0 > 49C030 > 4A2490 > 7409D0 > 7A2AD0
netz           0x007A2D90 <unnamed>                                via 504970 > 500AE0 > 5004E0 > 4EEEF0 > 4EEB90 > 4E8EA0 > 49C3D0 > 49C030 > 4A2490 > 7409D0 > 7A2D90
netz           0x0068E320 <unnamed>                                via 504970 > 500AE0 > 5004E0 > 4EEEF0 > 4EEB90 > 4E8EA0 > 49C3D0 > 49C030 > 4A2490 > 741460 > 68E320
netz           0x0068E360 <unnamed>                                via 504970 > 500AE0 > 5004E0 > 4EEEF0 > 4EEB90 > 4E8EA0 > 49C3D0 > 49C030 > 4A2490 > 741460 > 68E360
netz           0x0068E450 <unnamed>                                via 504970 > 500AE0 > 5004E0 > 4EEEF0 > 4EEB90 > 4E8EA0 > 49C3D0 > 49C030 > 4A2490 > 741460 > 68E450
netz           0x00656380 WriteConfigFlag                          via 504970 > 500AE0 > 5004E0 > 4EEEF0 > 4EEB90 > 4E8EA0 > 49E280 > 49E3C0 > 740410 > 740190 > 7475F0 > 656380
netz           0x00656480 WriteConfigPair                          via 504970 > 500AE0 > 5004E0 > 4EEEF0 > 4EEB90 > 4E8EA0 > 49E280 > 49E3C0 > 740410 > 740190 > 7475F0 > 656480
netz           0x006563B0 GameConfig_WriteByteField                via 504970 > 500AE0 > 5004E0 > 4EEEF0 > 4EEB90 > 4E8EA0 > 49E280 > 49E3C0 > 740410 > 740190 > 7475F0 > 6563B0
netz           0x0068E430 ScratchObj_Reset                         via 504970 > 500AE0 > 5004E0 > 4EEEF0 > 4EEB90 > 490E00 > 4D1180 > 4DC440 > 4D1C30 > 4D3480 > 4D1B50 > 68E430
netz           0x00651A60 NetSession_InitializeObjectReference     via 504970 > 500AE0 > 5004E0 > 4EEEF0 > 4EEB90 > 4EE420 > 490E70 > 4E0FE0 > 4E5130 > 4E50D0 > 4E2970 > 73E420 > 651A60
netz           0x00651D30 NetSession_CopyObjectReference           via 504970 > 500AE0 > 5004E0 > 4EEEF0 > 4EEB90 > 4EE420 > 490E70 > 4E0FE0 > 4E5130 > 4E50D0 > 4E2970 > 73E420 > 651D30
netz           0x00652070 SpawnPoints_ResolveEntryId               via 504970 > 500AE0 > 5004E0 > 4EEEF0 > 4EEB90 > 490E00 > 4D1180 > 4DC440 > 4D67C0 > 4D6B70 > 4D6A10 > 73D0F0 > 652070
netz           0x00652290 MpRecords_ResolveOwnerSlot               via 504970 > 500AE0 > 5004E0 > 4EEEF0 > 4EEB90 > 490E00 > 4D1180 > 4DC440 > 4D67C0 > 4D6B70 > 4D6A10 > 73D0F0 > 652290
netz           0x0064AD60 FindOrCreateConfigEntry                  via 504970 > 500AE0 > 5004E0 > 4EEEF0 > 4EEB90 > 4EB7B0 > 491050 > 48C570 > 4891E0 > 488590 > 48D610 > 73BD20 > 74B6E0 > 64AD60
netz           0x006563F0 GameConfig_WriteDwordField               via 504970 > 500AE0 > 5004E0 > 4EEEF0 > 4EEB90 > 4EB7B0 > 491050 > 48C570 > 4891E0 > 488590 > 48D610 > 73BD20 > 74B6E0 > 6563F0
netz           0x0064CEB0 AddConfigEntry                           via 504970 > 500AE0 > 5004E0 > 4EEEF0 > 4EEB90 > 4EB7B0 > 491050 > 48C570 > 4891E0 > 488590 > 48D610 > 73BD20 > 74B6E0 > 64CEB0
netz           0x0066B590 HashTemporaryString                      via 504970 > 500AE0 > 5004E0 > 4EEEF0 > 4EEB90 > 4EB7B0 > 491050 > 48C570 > 4891E0 > 488590 > 48D610 > 73BD20 > 74B6E0 > 66B590
netz           0x006483D0 MpSpawn_FetchSpawnRecord                 via 504970 > 500AE0 > 5004E0 > 4EEEF0 > 4EEB90 > 4EB7B0 > 491050 > 48C570 > 4891E0 > 488A20 > 484E90 > 48DEE0 > 5516E0 > 6483D0
netz           0x0064B620 NetDdlObjectManager_GetReferencedObject  via 504970 > 500AE0 > 5004E0 > 4EEEF0 > 4EEB90 > 4EE420 > 490E70 > 4E0FE0 > 4E5130 > 4E50D0 > 4E2970 > 73E420 > 74A9C0 > 64B620
netz           0x0064B110 NetSession_GetStationName                via 504970 > 500AE0 > 5004E0 > 4EEEF0 > 4EEB90 > 4EE420 > 490E70 > 4E0FE0 > 4E5130 > 4E50D0 > 4E2970 > 73E420 > 74A9C0 > 64B110
netz           0x007A2A20 Sub48_FillScratchFromSubobject           via 504970 > 500AE0 > 5004E0 > 4EEEF0 > 4EEB90 > 490E00 > 4D1180 > 4DC440 > 4D1C30 > 4D3480 > 4D2C40 > 4D26E0 > 740AA0 > 7A2A20
netz           0x007A2AE0 Scalar_PassThroughDouble                 via 504970 > 500AE0 > 5004E0 > 4EEEF0 > 4EEB90 > 490E00 > 4D1180 > 4DC440 > 4D1C30 > 4D3480 > 4D2C40 > 4D26E0 > 740AA0 > 7A2AE0
netz           0x006563D0 GameConfig_WriteWordField                via 504970 > 500AE0 > 5004E0 > 4EEEF0 > 4EEB90 > 4EE420 > 490E70 > 4E0FE0 > 4E5130 > 4E50D0 > 4E27D0 > 551680 > 749660 > 750D00 > 6563D0
netz           0x0068E300 CalculateSquaredVectorLength             via 504970 > 500AE0 > 5004E0 > 4EEEF0 > 4EEB90 > 4EB7B0 > 490EE0 > 4E10D0 > 4E4630 > 4E4D60 > 4E4CE0 > 4E4910 > 48B280 > 453070 > 451250 > 44F3E0 > 68E300
rw             0x0061EE90 RtAnimInterpolatorDestroy                via 504970 > 502250 > 6CB3A0 > 61EE90
rw             0x007FF0E0 Gfx2d_CreateVisibilityProxy              via 504970 > 503EF0 > 6CDAA0 > 6D6EE0 > 7FF0E0
rw             0x007FDA30 RtDictAddEntry                           via 504970 > 503EF0 > 6CDAA0 > 6D6EE0 > 7FDA30
rw             0x007FDA80 RtDictRemoveEntry                        via 504970 > 503EF0 > 6CBA40 > 6D6A40 > 7FDA80
rw             0x005870B0 RpUserDataArrayGetName                   via 504970 > 503EF0 > 4FEE70 > 6BB8A0 > 6BADA0 > 5870B0
rw             0x005863D0 RpPVSWorldGetProgressCallBack            via 504970 > 503EF0 > 6CDAA0 > 6BBFF0 > 6BC0B0 > 5863D0
rw             0x0056EFF0 RpLtMapGetRenderStyle                    via 504970 > 503EF0 > 6CDAA0 > 6BBFF0 > 6BC830 > 56EFF0
rw             0x0056F2B0 RpLtMapSetRenderStyle                    via 504970 > 503EF0 > 6CDAA0 > 6BBFF0 > 6BC830 > 56F2B0
rw             0x006329C0 RwFrameSetStaticPlugins                  via 504970 > 503EF0 > 6CDAA0 > 6BBFF0 > 6BF020 > 6329C0
rw             0x0056B9B0 RpWorldForAllWorldSectorIntersections    via 504970 > 503EF0 > 6CDAA0 > 6BD3D0 > 6BCE70 > 56B9B0
rw             0x0059FEA0 RpWorldStreamRead                        via 504970 > 503EF0 > 6CDAA0 > 6BBFF0 > 6BF020 > 6BEC60 > 59FEA0
rw             0x0063BF20 RwMatrixStreamRead                       via 504970 > 503EF0 > 6CDAA0 > 6BBFF0 > 6BF020 > 6BEC60 > 63BF20
rw             0x0056F380 RpLtMapWorldLightMapsQuery               via 504970 > 503EF0 > 6CDAA0 > 6BBFF0 > 6EE510 > 6BE050 > 6BD830 > 56F380
rw             0x0063C9B0 RwBBoxAddPoint                           via 504970 > 503EF0 > 6CDAA0 > 6BBFF0 > 6BF020 > 6BEC60 > 6BDB50 > 63C9B0
rw             0x0056CF80 RpHAnimHierarchyGetMatrixArray           via 504970 > 503000 > 6CD0E0 > 6CCB70 > 6B4330 > 6B4A50 > 6E9FF0 > 56CF80
rw             0x0056DB50 RpHAnimHierarchyAttach                   via 504970 > 503000 > 6CD0E0 > 6CCB70 > 6B4330 > 6B4A50 > 6E9FF0 > 56DB50
rw             0x0061F050 RtAnimInterpolatorSetCurrentAnim         via 504970 > 503000 > 6CD0E0 > 6CCB70 > 6B4330 > 6B4A50 > 6E9FF0 > 61F050
rw             0x0056D3F0 RpHAnimHierarchyUpdateMatrices           via 504970 > 503000 > 6CD0E0 > 6CCB70 > 6B4330 > 6B4A50 > 6E9FF0 > 56D3F0
rw             0x0056BAF0 RpCollisionWorldForAllIntersections      via 504970 > 503EF0 > 6CDAA0 > 6BBFF0 > 6EE4C0 > 6EE2A0 > 6EDF60 > 6EDED0 > 56BAF0
rw             0x007DA4E0 RtIntersectionLineSphere                 via 504970 > 503EF0 > 6CDAA0 > 6BBFF0 > 6EE4C0 > 6EE2A0 > 6EDF60 > 6EDE50 > 7DA4E0
rw             0x00587020 RpSkinAtomicSetHAnimHierarchy            via 504970 > 503000 > 6CD0E0 > 6CCB70 > 6B4330 > 6B4A50 > 6E9FF0 > 6E80C0 > 587020
rw             0x0061F590 RtAnimInterpolatorSetCurrentTime         via 504970 > 503000 > 6CD0E0 > 6CCB70 > 6B4330 > 6B4A50 > 6E8F50 > 6EA4D0 > 61F590
rw             0x0061F120 RtAnimInterpolatorCopy                   via 504970 > 503000 > 6CD0E0 > 6CCB70 > 6B4330 > 6B4A50 > 6E8F50 > 6EA4D0 > 61F120
rw             0x0061EFB0 RtAnimAnimationGetNumNodes               via 504970 > 503000 > 6CD0E0 > 6CCB70 > 6B4330 > 6B4A50 > 6E8F50 > 6EA4D0 > 61EFB0
rw             0x0056CDB0 RpHAnimHierarchyCreateSubHierarchy       via 504970 > 503000 > 6CD0E0 > 6CCB70 > 6B4330 > 6B4A50 > 6E8F50 > 6EA430 > 56CDB0
rw             0x0056CFA0 RpHAnimFrameGetID                        via 504970 > 503000 > 5021A0 > 501F90 > 4E9370 > 6CCD70 > 6CB8C0 > 6E81A0 > 56CFA0
rw             0x007C5D00 RtCompressedKeyConvertFromStd            via 504970 > 503000 > 5021A0 > 501F90 > 4E9370 > 6CCD70 > 6CB8C0 > 6E81A0 > 7C5D00
rw             0x0061EE10 RtAnimAnimationCreate                    via 504970 > 503000 > 5021A0 > 501F90 > 4E9370 > 6CCD70 > 6CB8C0 > 6E81A0 > 61EE10
rw             0x00620640 RtIntersectionLineTriangle               via 504970 > 503EF0 > 6CDAA0 > 6BBFF0 > 6EE4C0 > 6EE2A0 > 6EDF60 > 6EDED0 > 6EBA90 > 620640
rw             0x0056BDF0 RpAtomicForAllIntersections              via 504970 > 503EF0 > 6CDAA0 > 6BBFF0 > 6EE4C0 > 6EE2A0 > 6EDF60 > 6EDE50 > 6EDC00 > 56BDF0
rw             0x005712A0 RpMatFXAtomicQueryEffects                via 504970 > 503000 > 6CD0E0 > 6CCB70 > 6B4330 > 6B3FE0 > 6B3B40 > 6B3230 > 6E6A20 > 5712A0
rw             0x0056CF90 RpHAnimFrameGetHierarchy                 via 504970 > 503000 > 6CD0E0 > 6CCB70 > 6B4330 > 6B4A50 > 6E9FF0 > 6E8090 > 6E8010 > 56CF90
rw             0x0056CED0 RpHAnimHierarchyCreateFromHierarchy      via 504970 > 503000 > 6CD0E0 > 6CCB70 > 6B4330 > 6B4A50 > 6E9FF0 > 6F7960 > 6E80F0 > 56CED0
rw             0x0056CD30 RpHAnimHierarchyDestroy                  via 504970 > 503000 > 6CD0E0 > 6CCB70 > 6B4330 > 6B4A50 > 6E8F50 > 6EB3E0 > 6EA230 > 56CD30
rw             0x0056CF50 RpHAnimHierarchyDetach                   via 504970 > 503000 > 6CD0E0 > 6CCB70 > 6B4330 > 6B4A50 > 6E8F50 > 6EB3E0 > 6EA230 > 56CF50
rw             0x0058E680 <unnamed>                                via 504970 > 503EF0 > 6CDAA0 > 6BBFF0 > 6EE510 > 6BE050 > 6BD830 > 6BD690 > 6C6D40 > 6C68F0 > 58E680
rw             0x005712B0 RpMatFXMaterialGetEffects                via 504970 > 503000 > 6CD0E0 > 6CCB70 > 6B4330 > 6B3FE0 > 6B3B40 > 6B3230 > 6E6A20 > 6C07D0 > 5712B0
rw             0x00571610 RpMatFXMaterialSetEnvMapFrameBufferAlpha via 504970 > 503000 > 6CD0E0 > 6CCB70 > 6B4330 > 6B3FE0 > 6B3B40 > 6B3230 > 6E6A20 > 6C07D0 > 571610
rw             0x005715D0 RpMatFXMaterialSetEnvMapFrame            via 504970 > 503000 > 6CD0E0 > 6CCB70 > 6B4330 > 6B3FE0 > 6B3B40 > 6B3230 > 6E6A20 > 6C07D0 > 5715D0
rw             0x005870F0 RpSkinAtomicGetType                      via 504970 > 503EF0 > 6CDAA0 > 6BBFF0 > 6EE510 > 6BE050 > 6BD830 > 6BD690 > 6C6D40 > 6C68F0 > 6C4DF0 > 5870F0
rw             0x00587050 RpSkinGeometryGetSkin                    via 504970 > 503EF0 > 6CDAA0 > 6BBFF0 > 6EE510 > 6BE050 > 6BD830 > 6BD690 > 6C6D40 > 6C68F0 > 6C4DF0 > 587050
rw             0x005870E0 RpSkinIsSplit                            via 504970 > 503EF0 > 6CDAA0 > 6BBFF0 > 6EE510 > 6BE050 > 6BD830 > 6BD690 > 6C6D40 > 6C68F0 > 6C4DF0 > 5870E0
rw             0x007C73A0 RtSkinSplitAtomicSplit                   via 504970 > 503EF0 > 6CDAA0 > 6BBFF0 > 6EE510 > 6BE050 > 6BD830 > 6BD690 > 6C6D40 > 6C68F0 > 6C4DF0 > 7C73A0
rw             0x00571920 RpMatFXMaterialSetUVTransformMatrices    via 504970 > 503EF0 > 6CDAA0 > 6BBFF0 > 6EE510 > 6BE050 > 6BD830 > 6BD690 > 6C6D40 > 6C6200 > 6EFB20 > 571920
rw             0x0063C8B0 RwBBoxCalculate                          via 504970 > 503EF0 > 6CDAA0 > 6BBFF0 > 6EE510 > 6BE050 > 6BD830 > 6BD690 > 6C6D40 > 6C62D0 > 6C55B0 > 6C0570 > 63C8B0
rw             0x00620840 RtIntersectionSphereTriangle             via 504970 > 500AE0 > 5004E0 > 4EEEF0 > 4EEB90 > 4ED030 > 48E2B0 > 41DDC0 > 415AE0 > 415860 > 415770 > 412B10 > 4121B0 > 44FDD0 > 620840
rw             0x0056BBD0 RpWorldForAllAtomicIntersections         via 504970 > 500AE0 > 5004E0 > 4EEEF0 > 4EEB90 > 4ED030 > 48E2B0 > 41DDC0 > 415AE0 > 415860 > 415770 > 4154D0 > 413DE0 > 405370 > 405180 > 4048F0 > 404390 > 404320 > 56BBD0

```
