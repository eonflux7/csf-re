#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

CSF_FUNC(uint32_t*, __cdecl, MenuMpAccounts_CreateListSentinel, 0x00552DD0, ());
CSF_FUNC(int64_t, __fastcall, MpSpawn_CommitSpawnRecord, 0x006472B0, (void* self, void* edx));
CSF_FUNC(uint32_t*, __cdecl, MpSpawn_FetchSpawnRecord, 0x006483D0, (uint32_t* a1));
CSF_FUNC(int, __cdecl, Mem_FreeDebug, 0x00690E60, (void* block));
CSF_FUNC(int, __cdecl, GetGlobalDword_73C020, 0x0073C020, ());
CSF_FUNC(int, __fastcall, MenuMpAccounts_FillPlayerList, 0x0073F3B0, (void* self, void* edx, void* list));
CSF_FUNC(int, __fastcall, GetValueIfNotFlagged, 0x007408C0, (void* self, void*));
CSF_FUNC(int, __cdecl, __stricmp, 0x007B0C41, (const char* left, const char* right));

CSF_GLOBAL(float, g_fZero, 0x0082D4CC);

static uint32_t* __fastcall MpSpawn_UpdateMatchingPlayerCountdown(
    void* self, void* /*edx*/, float elapsed)
{
    alignas(8) uint8_t listBytes[0xC] = {};
    alignas(8) uint8_t spawnBytes[0xC] = {};

    CSF_AT(uint32_t*, listBytes, 0x4) = MenuMpAccounts_CreateListSentinel();
    CSF_AT(int, listBytes, 0x8) = 0;

    void* accounts = reinterpret_cast<void*>(GetGlobalDword_73C020());
    MenuMpAccounts_FillPlayerList(accounts, nullptr, listBytes);

    uint32_t* node = CSF_AT(uint32_t*, CSF_AT(uint32_t*, listBytes, 0x4), 0);
    int8_t matchingPlayers = 0;
    while (node != CSF_AT(uint32_t*, listBytes, 0x4))
    {
        if (matchingPlayers >= 2)
            break;

        void* player = CSF_AT(void*, node, 0x8);
        if (player &&
            GetValueIfNotFlagged(static_cast<uint8_t*>(player) + 0x6B0, nullptr) &&
            __stricmp(
                reinterpret_cast<const char*>(player) + 0x459,
                reinterpret_cast<const char*>(self) + 0x6B) == 0)
        {
            ++matchingPlayers;
        }

        node = CSF_AT(uint32_t*, node, 0);
    }

    if (matchingPlayers < 1)
    {
        CSF_AT(float, self, 0x32C) = 15.0f;
    }
    else
    {
        float remaining = CSF_AT(float, self, 0x32C) - elapsed;
        CSF_AT(float, self, 0x32C) = remaining;

        if (remaining <= g_fZero)
        {
            CSF_AT(uint8_t, self, 0x5C) = 2;
            uint32_t* record =
                MpSpawn_FetchSpawnRecord(reinterpret_cast<uint32_t*>(spawnBytes));
            uint64_t timestamp =
                static_cast<uint64_t>(MpSpawn_CommitSpawnRecord(record, nullptr));

            uint32_t delay = 0;
            bool storeTimestamp = true;
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
                storeTimestamp = false;
                break;
            }

            if (storeTimestamp)
            {
                timestamp += delay;
                CSF_AT(uint32_t, self, 0x2E8) = static_cast<uint32_t>(timestamp);
                CSF_AT(uint32_t, self, 0x2EC) = static_cast<uint32_t>(timestamp >> 32);
            }
        }
    }

    node = CSF_AT(uint32_t*, CSF_AT(uint32_t*, listBytes, 0x4), 0);
    CSF_AT(uint32_t*, CSF_AT(uint32_t*, listBytes, 0x4), 0) =
        CSF_AT(uint32_t*, listBytes, 0x4);
    CSF_AT(uint32_t*, CSF_AT(uint32_t*, listBytes, 0x4), 0x4) =
        CSF_AT(uint32_t*, listBytes, 0x4);
    CSF_AT(int, listBytes, 0x8) = 0;

    while (node != CSF_AT(uint32_t*, listBytes, 0x4))
    {
        uint32_t* next = CSF_AT(uint32_t*, node, 0);
        Mem_FreeDebug(node);
        node = next;
    }

    if (CSF_AT(uint32_t*, listBytes, 0x4))
    {
        return reinterpret_cast<uint32_t*>(static_cast<uintptr_t>(
            Mem_FreeDebug(CSF_AT(uint32_t*, listBytes, 0x4))));
    }
    return node;
}

HOOK(0x00748BF0, MpSpawn_UpdateMatchingPlayerCountdown, "auto");
