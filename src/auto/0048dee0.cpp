#include <cstdint>
#include <cstring>
#include "globals.h"
#include "hook.h"

CSF_FUNC(int, __fastcall, CHandleTable_Resolve, 0x0044FC30, (void* self, void* edx, int handle));
CSF_FUNC(int, __cdecl, GetGlobalObjectField184, 0x00475470, ());
CSF_FUNC(uint32_t, __fastcall, DispatchRecordToGlobalObject, 0x004758B0, (void* self, void* edx, void* record, char flag));
CSF_FUNC(int, __cdecl, GetGlobalState_50D190, 0x0050D190, ());
CSF_FUNC(int, __cdecl, GetGlobal8B0688, 0x00550CC0, ());
CSF_FUNC(int64_t, __cdecl, MpSpawn_QueryAndCommitSpawn, 0x005516E0, ());

CSF_GLOBAL(float, g_fZero, 0x0082D4CC);
CSF_GLOBAL(float, g_MaxPercent, 0x0082E120);
CSF_GLOBAL(float, g_CharacterFacingDistance, 0x00833854);
CSF_GLOBAL(int32_t, g_SpawnCountdownOffset, 0x00844624);
CSF_GLOBAL(void*, g_pCurrentScene, 0x008AFC90);

static int __fastcall State_UpdateSpawnCountdown(void* self, void* /*edx*/, float deltaTime)
{
    bool cancel = CSF_AT(int32_t, self, 0x2C) < 0;
    if (!cancel)
    {
        void* owner = CSF_AT(void*, self, 0x00);
        if (!CSF_AT(uint8_t, owner, 0xBC))
        {
            cancel = true;
        }
        else
        {
            void* character = CSF_AT(void*, owner, 0x08);
            cancel = CSF_AT(uint32_t, character, 0x244) !=
                         CSF_AT(uint32_t, self, 0x28) ||
                     CSF_AT(float, character, 0x258) > g_CharacterFacingDistance;
        }
    }

    if (cancel)
    {
        void* owner = CSF_AT(void*, self, 0x00);
        return ((int(__fastcall*)(void*, void*, int, int))
                    CSF_AT(void**, owner, 0x00)[0x134 / 4])
            (owner, nullptr, 0, 0);
    }

    void* resolved = reinterpret_cast<void*>(CHandleTable_Resolve(
        g_pCurrentScene, nullptr,
        reinterpret_cast<int>(static_cast<uint8_t*>(self) + 0x24)));
    uintptr_t interior = CSF_AT(uintptr_t, resolved, 0x18);
    void* spawn = reinterpret_cast<void*>(interior ? interior - 128 : 0);

    alignas(8) uint8_t recordBytes[0x38];
    const int32_t countdownOffset = g_SpawnCountdownOffset;
    const uint32_t spawnTimeLow = CSF_AT(uint32_t, spawn, 0x68);
    const uint32_t spawnTimeHigh = CSF_AT(uint32_t, spawn, 0x6C);
    CSF_AT(int32_t, recordBytes, 0x34) = countdownOffset;

    GetGlobal8B0688();
    const uint64_t committedTime =
        static_cast<uint64_t>(MpSpawn_QueryAndCommitSpawn());
    const uint64_t spawnTime =
        (static_cast<uint64_t>(spawnTimeHigh) << 32) | spawnTimeLow;

    if (spawnTime - committedTime <
        static_cast<uint64_t>(static_cast<int64_t>(countdownOffset)))
    {
        void* owner = CSF_AT(void*, self, 0x00);
        return ((int(__fastcall*)(void*, void*, int, int))
                    CSF_AT(void**, owner, 0x00)[0x134 / 4])
            (owner, nullptr, 0, 0);
    }

    const float remaining = CSF_AT(float, self, 0x1C) - deltaTime;
    CSF_AT(float, self, 0x1C) = remaining;

    if (remaining < g_fZero)
    {
        const uint32_t characterId = CSF_AT(uint32_t, self, 0x28);
        const int32_t spawnIndex = CSF_AT(int32_t, self, 0x2C);

        CSF_AT(void**, recordBytes, 0x08) = CSF_VTBL(0x0082D448);
        CSF_AT(void**, recordBytes, 0x14) = CSF_VTBL(0x0082D448);
        CSF_AT(int32_t, recordBytes, 0x18) = -1;
        CSF_AT(int32_t, recordBytes, 0x1C) = -1;
        CSF_AT(void**, recordBytes, 0x00) = CSF_VTBL(0x0082D478);
        CSF_AT(int32_t, recordBytes, 0x04) = 34;
        std::memset(recordBytes + 0x20, 0, 16);
        CSF_AT(uint8_t, recordBytes, 0x30) = 0;
        CSF_AT(uint32_t, recordBytes, 0x0C) = characterId;
        CSF_AT(int32_t, recordBytes, 0x10) = spawnIndex;

        void* dispatcher = reinterpret_cast<void*>(GetGlobalObjectField184());
        DispatchRecordToGlobalObject(dispatcher, nullptr, recordBytes, 0);

        void* owner = CSF_AT(void*, self, 0x00);
        return ((int(__fastcall*)(void*, void*, int, int))
                    CSF_AT(void**, owner, 0x00)[0x134 / 4])
            (owner, nullptr, 0, 0);
    }

    const float percentage =
        g_MaxPercent - remaining / CSF_AT(float, self, 0x20) * g_MaxPercent;
    const int state = GetGlobalState_50D190();
    CSF_AT(float, reinterpret_cast<void*>(state), 0xFC) = percentage;
    return state;
}

HOOK(0x0048DEE0, State_UpdateSpawnCountdown, "auto");
