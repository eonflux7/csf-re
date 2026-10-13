#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_FUNC(uint32_t*, __cdecl, MenuMpAccounts_CreateListSentinel, 0x00552DD0, ());
CSF_FUNC(int, __fastcall, Sub_64B250, 0x0064B250, (void* self, void* edx));
CSF_FUNC(int, __cdecl, Mem_FreeDebug, 0x00690E60, (void* block));
CSF_FUNC(int, __cdecl, GetGlobalDword_73C020, 0x0073C020, ());
CSF_FUNC(int, __fastcall, MenuMpAccounts_FillPlayerList, 0x0073F3B0, (void* self, void* edx, void* list));
CSF_FUNC(char, __fastcall, MpSpawnPoints_SetPlayerStateFlag, 0x00740F80, (void* self, void* , char value));
CSF_FUNC(char, __fastcall, C_00858180_ResetRespawnMemoriesConfig, 0x0074C720, (void* self, void* edx));

static char __fastcall CMenuMpStats_RefreshSpawnStatistics(
    CMenuMpStats* self, void* /*edx*/)
{
    const char ready = static_cast<char>(Sub_64B250(self, nullptr));
    if (!ready)
        return ready;

    const int slotCount = CSF_AT(int8_t, self, 0x69);
    if (CSF_AT(int32_t*, self, 0x350))
    {
        CSF_AT(int32_t, self, 0x354) = 0;
        CSF_AT(uint8_t, self, 0x360) = 0;
        for (int slot = 0; slot < slotCount; ++slot)
            CSF_AT(int32_t*, self, 0x350)[slot] = -1;
    }

    if (CSF_AT(int32_t*, self, 0x358))
    {
        CSF_AT(int32_t, self, 0x35C) = 0;
        CSF_AT(uint8_t, self, 0x361) = 0;
        for (int slot = 0; slot < slotCount; ++slot)
            CSF_AT(int32_t*, self, 0x358)[slot] = -1;
    }

    C_00858180_ResetRespawnMemoriesConfig(self, nullptr);

    alignas(8) uint8_t playerListBytes[0xC];
    CSF_AT(uint32_t, playerListBytes, 0) = 0;
    CSF_AT(void*, playerListBytes, 4) = MenuMpAccounts_CreateListSentinel();
    CSF_AT(uint32_t, playerListBytes, 8) = 0;

    void* accounts = reinterpret_cast<void*>(GetGlobalDword_73C020());
    MenuMpAccounts_FillPlayerList(accounts, nullptr, playerListBytes);

    void* sentinel = CSF_AT(void*, playerListBytes, 4);
    void* node = CSF_AT(void*, sentinel, 0);
    while (node != sentinel)
    {
        void* player = CSF_AT(void*, node, 8);
        if (player)
        {
            MpSpawnPoints_SetPlayerStateFlag(player, nullptr, 0);
            sentinel = CSF_AT(void*, playerListBytes, 4);
        }
        node = CSF_AT(void*, node, 0);
    }

    node = CSF_AT(void*, sentinel, 0);
    CSF_AT(void*, sentinel, 0) = sentinel;
    sentinel = CSF_AT(void*, playerListBytes, 4);
    CSF_AT(void*, sentinel, 4) = sentinel;
    sentinel = CSF_AT(void*, playerListBytes, 4);
    CSF_AT(uint32_t, playerListBytes, 8) = 0;

    while (node != sentinel)
    {
        void* next = CSF_AT(void*, node, 0);
        Mem_FreeDebug(node);
        sentinel = CSF_AT(void*, playerListBytes, 4);
        node = next;
    }

    if (sentinel)
        return static_cast<char>(Mem_FreeDebug(sentinel));

    return static_cast<char>(reinterpret_cast<uintptr_t>(node));
}

HOOK(0x00748AE0, CMenuMpStats_RefreshSpawnStatistics, "auto");
