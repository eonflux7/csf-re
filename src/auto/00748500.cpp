#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"

CSF_FUNC(int64_t, __fastcall, MpSpawn_CommitSpawnRecord, 0x006472B0, (void* self));
CSF_FUNC(uint32_t*, __cdecl, MpSpawn_FetchSpawnRecord, 0x006483D0, (uint32_t* a1));

static int64_t __fastcall MpSpawn_ResetExpiredSpawnState(
    void* self, void* /*edx*/, int /*unused*/)
{
    alignas(8) uint8_t spawnRecordBytes[0x8];
    uint32_t* spawnRecord =
        MpSpawn_FetchSpawnRecord(reinterpret_cast<uint32_t*>(spawnRecordBytes));
    const int64_t currentTime = MpSpawn_CommitSpawnRecord(spawnRecord);
    const uint64_t elapsed =
        static_cast<uint64_t>(currentTime) - CSF_AT(uint64_t, self, 0x2E8);

    CSF_AT(uint32_t, spawnRecordBytes, 0x4) =
        static_cast<uint32_t>(elapsed >> 32);

    if (elapsed > 0x3A98)
    {
        CSF_AT(uint8_t, self, 0x5C) = 2;
        CSF_AT(uint8_t, self, 0x300) = 0;

        if (CSF_AT(uint16_t, self, 0x2D6))
            CSF_AT(uint16_t, self, 0x2D6) = 0;
        if (CSF_AT(uint16_t, self, 0x2DC))
            CSF_AT(uint16_t, self, 0x2DC) = 0;
        if (CSF_AT(uint16_t, self, 0x2C4))
            CSF_AT(uint16_t, self, 0x2C4) = 0;

        uint32_t delay;
        switch (CSF_AT(int8_t, self, 0x6A))
        {
        case 0:
            delay = CSF_AT(uint32_t, self, 0x1EC);
            break;
        case 1:
            delay = CSF_AT(uint32_t, self, 0x1F4);
            break;
        case 3:
            delay = CSF_AT(uint32_t, self, 0x1F0);
            break;
        default:
            return currentTime;
        }

        CSF_AT(uint64_t, self, 0x2E8) =
            static_cast<uint64_t>(currentTime) + delay;
    }

    return currentTime;
}

HOOK(0x00748500, MpSpawn_ResetExpiredSpawnState, "auto");
