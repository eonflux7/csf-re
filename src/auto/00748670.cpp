#include <cstdint>
#include "globals.h"
#include <cstddef>
#include "hook.h"
#include "types/auto_classes.h"

// Original callees, bound by address.
CSF_FUNC(uint32_t*, __cdecl, MpSpawn_FetchSpawnRecord, 0x006483D0, (uint32_t* a1));
CSF_FUNC(int64_t, __fastcall, MpSpawn_CommitSpawnRecord, 0x006472B0, (void* self, void* edx));

// Compares the current slot against the reference slot and the spawn timing window.
// Returns 0 when the slot is inactive/out of window, 3 when both slots are equal,
// 1 when the current slot is below the reference slot, and 2 otherwise.
static int __fastcall S_00747f20_GetRelativeSpawnSlotState(S_00747f20* self, void* /*edx*/)
{
    alignas(8) uint8_t spawnRecordBytes[8];
    uint32_t* spawnRecord = MpSpawn_FetchSpawnRecord(reinterpret_cast<uint32_t*>(spawnRecordBytes));
    uint64_t commitTime = static_cast<uint64_t>(MpSpawn_CommitSpawnRecord(spawnRecord, nullptr));

    if (self->field_01f0 == 0)
        goto checkLimit;
    {
        uint64_t spawnTime = (static_cast<uint64_t>(self->field_02ec) << 32) | self->field_02e8;
        if (spawnTime < commitTime)
        {
            uint16_t currentSlot = self->field_02dc;
            uint16_t referenceSlot = self->field_02d6;
            if (currentSlot == referenceSlot)
                return 3;
            return 2 - (currentSlot < referenceSlot ? 1 : 0);
        }
    }

checkLimit:
    {
        uint16_t limit = self->field_0200;
        if (limit == 0 || (self->field_02d6 < limit && self->field_02dc < limit))
            return 0;
    }

    uint16_t currentSlot = self->field_02dc;
    uint16_t referenceSlot = self->field_02d6;
    if (currentSlot == referenceSlot)
        return 3;
    return 2 - (currentSlot < referenceSlot ? 1 : 0);
}

HOOK(0x00748670, S_00747f20_GetRelativeSpawnSlotState, "auto");
