#include <cstdint>
#include <cmath>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"

CSF_FUNC(double, __fastcall, GetPlayerPositionX, 0x004D1AE0, (void* self, void* edx));
CSF_FUNC(double, __fastcall, GetPlayerPositionY, 0x004D1B50, (void* self, void* edx));
CSF_FUNC(double, __fastcall, GetPlayerPositionZ, 0x004D1BC0, (void* self, void* edx));
CSF_FUNC(void*, __cdecl, MenuMpAccounts_CreateListSentinel, 0x00552DD0, ());
CSF_FUNC(int, __cdecl, Mem_FreeDebug, 0x00690E60, (void* block));
CSF_FUNC(int, __cdecl, GetGlobalDword_73C020, 0x0073C020, ());
CSF_FUNC(int, __fastcall, MenuMpAccounts_FillPlayerList, 0x0073F3B0, (void* self, void* edx, void* list));
CSF_FUNC(int, __fastcall, GetValueIfNotFlagged, 0x007408C0, (void* self, void*));
CSF_FUNC(int, __fastcall, GetPlayerExclusionFlag, 0x007411A0, (void* self, void* edx));

CSF_GLOBAL(float, g_Const200_0f, 0x0082DDEC);

static bool __stdcall MpSpawnPoints_IsPlayerWithinSpawnRadius(float x, float y, float z)
{
    alignas(4) std::uint8_t playerList[0x14]{};
    bool nearby = false;

    CSF_AT(void*, playerList, 0x04) = MenuMpAccounts_CreateListSentinel();
    CSF_AT(int, playerList, 0x08) = 0;

    void* accounts = reinterpret_cast<void*>(GetGlobalDword_73C020());
    MenuMpAccounts_FillPlayerList(accounts, nullptr, playerList);

    void* sentinel = CSF_AT(void*, playerList, 0x04);
    void* node = CSF_AT(void*, sentinel, 0x00);
    while (node != sentinel)
    {
        void* player = CSF_AT(void*, node, 0x08);
        GetValueIfNotFlagged(static_cast<std::uint8_t*>(player) + 0x6B0, nullptr);

        if (static_cast<std::uint8_t>(GetPlayerExclusionFlag(player, nullptr)) == 0)
        {
            CSF_AT(float, playerList, 0x0C) =
                static_cast<float>(GetPlayerPositionZ(player, nullptr));
            CSF_AT(float, playerList, 0x10) =
                static_cast<float>(GetPlayerPositionY(player, nullptr));
            const double playerX = GetPlayerPositionX(player, nullptr);

            const double dx = x - playerX;
            const float dy = y - CSF_AT(float, playerList, 0x10);
            const float dz = z - CSF_AT(float, playerList, 0x0C);
            nearby = std::fabs(std::sqrt(dz * dz + dy * dy + dx * dx)) <
                     g_Const200_0f;
        }

        node = CSF_AT(void*, node, 0x00);
        sentinel = CSF_AT(void*, playerList, 0x04);
        if (nearby)
            break;
    }

    node = CSF_AT(void*, sentinel, 0x00);
    CSF_AT(void*, sentinel, 0x00) = sentinel;
    sentinel = CSF_AT(void*, playerList, 0x04);
    CSF_AT(void*, sentinel, 0x04) = sentinel;
    sentinel = CSF_AT(void*, playerList, 0x04);
    CSF_AT(int, playerList, 0x08) = 0;

    while (node != sentinel)
    {
        void* next = CSF_AT(void*, node, 0x00);
        Mem_FreeDebug(node);
        sentinel = CSF_AT(void*, playerList, 0x04);
        node = next;
    }

    if (sentinel)
        Mem_FreeDebug(sentinel);

    return nearby;
}

HOOK(0x00554CD0, MpSpawnPoints_IsPlayerWithinSpawnRadius, "auto");
