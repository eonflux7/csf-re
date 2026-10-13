#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_FUNC(int, __cdecl, GetGlobalValue_558150, 0x00558150, ());
CSF_FUNC(int64_t, __fastcall, MpSpawn_CommitSpawnRecord, 0x006472B0, (void* self, void* edx));
CSF_FUNC(uint32_t*, __cdecl, MpSpawn_FetchSpawnRecord, 0x006483D0, (uint32_t* a1));
CSF_FUNC(int, __fastcall, Sub_64B250, 0x0064B250, (void* self, void* edx));
CSF_FUNC(char, __fastcall, C_00858180_SetBombActivated, 0x0074B730, (void* self, void* edx, char a2));

static int __fastcall CMenuMpStats_SetBombActivated(
    CMenuMpStats* self, void* /*edx*/, char activated)
{
    // Invoke the original thiscall ABI without introducing an EDX argument.
    if (static_cast<uint8_t>(
            reinterpret_cast<int (__thiscall*)(void*)>(Sub_64B250)(self)) == 0)
    {
        return reinterpret_cast<char (__thiscall*)(void*, char)>(
            C_00858180_SetBombActivated)(self, activated);
    }

    if (CSF_AT(char, self, 0x44) != activated)
        CSF_AT(char, self, 0x44) = activated;

    void* settings = reinterpret_cast<void*>(GetGlobalValue_558150());

    if (activated)
    {
        alignas(8) uint8_t spawnRecordBytes[0x8];
        uint32_t* spawnRecord = MpSpawn_FetchSpawnRecord(
            reinterpret_cast<uint32_t*>(spawnRecordBytes));

        const int64_t spawnTime =
            reinterpret_cast<int64_t (__thiscall*)(void*)>(
                MpSpawn_CommitSpawnRecord)(spawnRecord);
        const uint32_t delay =
            1000 * CSF_AT(uint32_t, settings, 0x1C0);
        const uint64_t deadline = static_cast<uint64_t>(spawnTime) + delay;
        const uint32_t low = static_cast<uint32_t>(deadline);
        const uint32_t high = static_cast<uint32_t>(deadline >> 32);

        if (CSF_AT(uint32_t, self, 0x68) != low ||
            CSF_AT(uint32_t, self, 0x6C) != high)
        {
            CSF_AT(uint32_t, self, 0x68) = low;
            CSF_AT(uint32_t, self, 0x6C) = high;
        }
        return static_cast<int32_t>(low);
    }

    if (CSF_AT(int32_t, self, 0x68) != -1 ||
        CSF_AT(int32_t, self, 0x6C) != -1)
    {
        CSF_AT(int32_t, self, 0x68) = -1;
        CSF_AT(int32_t, self, 0x6C) = -1;
    }
    return -1;
}

HOOK(0x0073BE00, CMenuMpStats_SetBombActivated, "auto");
