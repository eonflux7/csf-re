#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

CSF_FUNC(int, __cdecl, GetGlobal8B0688, 0x00550CC0, ());
CSF_FUNC(int64_t, __fastcall, MpSpawn_CommitSpawnRecord, 0x006472B0, (void* self, void* edx));
CSF_FUNC(uint32_t*, __cdecl, MpSpawn_FetchSpawnRecord, 0x006483D0, (uint32_t* a1));

static int __fastcall S_00747f20_ComputeStateMode1(
    S_00747f20* self, void* /*edx*/)
{
    alignas(8) uint8_t spawnRecordBytes[0x8];
    uint32_t* spawnRecord = MpSpawn_FetchSpawnRecord(
        reinterpret_cast<uint32_t*>(spawnRecordBytes));
    // This boundary has no EDX input: 6472B0 loads EAX/EDX from [ECX]/[ECX+4].
    // Call its actual thiscall ABI rather than zeroing the dummy fastcall EDX.
    const uint64_t currentTime = static_cast<uint64_t>(
        reinterpret_cast<int64_t(__thiscall*)(void*)>(
            MpSpawn_CommitSpawnRecord)(spawnRecord));

    const bool elapsed =
        self->field_01f4 &&
        ((static_cast<uint64_t>(self->field_02ec) << 32) |
         static_cast<uint64_t>(self->field_02e8)) < currentTime;

    if (GetGlobal8B0688() == -3240)
        return 0;

    if (!elapsed)
    {
        const uint16_t threshold = self->field_01fc;
        if (!threshold ||
            (self->field_02d6 < threshold &&
             self->field_02dc < threshold))
            return 0;
    }

    const uint16_t secondState = self->field_02dc;
    const uint16_t firstState = self->field_02d6;
    if (secondState == firstState)
        return 3;

    return 2 - (secondState < firstState);
}

HOOK(0x007485C0, S_00747f20_ComputeStateMode1, "auto");
