#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_FUNC(uint32_t*, __cdecl, MpSpawn_FetchSpawnRecord, 0x006483D0, (uint32_t* a1));
CSF_FUNC(int64_t, __fastcall, MpSpawn_CommitSpawnRecord, 0x006472B0, (void* self, void* edx));

static int __fastcall S_00747f20_GetRelativeSpawnSlotState(
    S_00747f20* self, void* /*edx*/)
{
    alignas(8) uint8_t spawnRecordBytes[8];
    uint32_t* spawnRecord =
        MpSpawn_FetchSpawnRecord(reinterpret_cast<uint32_t*>(spawnRecordBytes));
    // The original sets ECX only. The getter overwrites EDX with the high word.
    uint64_t committedTime = static_cast<uint64_t>(
        reinterpret_cast<int64_t(__thiscall*)(void*)>(
            MpSpawn_CommitSpawnRecord)(spawnRecord));

    if (self->field_01f0 != 0)
    {
        uint32_t stampHigh = self->field_02ec;
        uint32_t committedHigh = static_cast<uint32_t>(committedTime >> 32);

        if (stampHigh < committedHigh)
            goto compareSlots;

        if (stampHigh == committedHigh)
        {
            if (self->field_02e8 < static_cast<uint32_t>(committedTime))
                goto compareSlots;
        }
    }

    {
        uint16_t windowLimit = self->field_0200;
        if (windowLimit == 0)
            return 0;

        if (self->field_02d6 < windowLimit)
        {
            if (self->field_02dc < windowLimit)
                return 0;
        }
    }

compareSlots:
    uint16_t currentSlot = self->field_02dc;
    uint16_t referenceSlot = self->field_02d6;
    if (currentSlot == referenceSlot)
        return 3;

    return currentSlot < referenceSlot ? 1 : 2;
}

HOOK(0x00748670, S_00747f20_GetRelativeSpawnSlotState, "auto");
