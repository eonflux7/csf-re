#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_FUNC(void*, __cdecl, MenuMpAccounts_CreateListSentinel, 0x00552DD0, ());
CSF_FUNC(void*, __stdcall, MpSpawnPoints_ProcessInactivePlayerComponent, 0x005536C0, (int a1));
CSF_FUNC(int, __cdecl, Mem_FreeDebug, 0x00690E60, (void* block));
CSF_FUNC(int, __cdecl, GetGlobalDword_73C020, 0x0073C020, ());
CSF_FUNC(int, __fastcall, MenuMpAccounts_FillPlayerList, 0x0073F3B0, (void* self, void* edx, void* list));
CSF_FUNC(int, __fastcall, MpSpawnPoints_PrepareObjectForRemoval, 0x0073F750, (void* self, void* edx));
CSF_FUNC(int, __fastcall, GetValueIfNotFlagged, 0x007408C0, (void* self, void*));
CSF_FUNC(char, __fastcall, MpSpawnPoints_SetPlayerStateFlag, 0x00740F80, (void* self, void* edx, char a2));

CSF_GLOBAL(uint32_t, g_CharacterSelectState8B0694, 0x008B0694);

static void** __fastcall S_00555390_ProcessPlayerList(
    S_00555390* self, void* /*edx*/)
{
    alignas(4) uint8_t playerList[0x18] = {};
    CSF_AT(void*, playerList, 0x4) = MenuMpAccounts_CreateListSentinel();
    CSF_AT(int, playerList, 0x8) = 0;

    void* accounts = reinterpret_cast<void*>(GetGlobalDword_73C020());
    MenuMpAccounts_FillPlayerList(accounts, nullptr, playerList);

    uint8_t updatePlayerState = 0;
    const uint32_t selectionState = g_CharacterSelectState8B0694;
    if (selectionState != 0)
    {
        const uint32_t selectionComponent =
            CSF_AT(uint32_t, reinterpret_cast<void*>(selectionState), 0x4);
        if (selectionComponent != 0)
        {
            const uint32_t selectionOwner = selectionComponent - 776;
            if (selectionOwner != 0)
            {
                void* component = reinterpret_cast<void*>(selectionOwner + 776);
                updatePlayerState =
                    reinterpret_cast<uint8_t(__fastcall*)(void*, void*)>(
                        CSF_AT(void**, component, 0)[2])(component, nullptr);
            }
        }
    }

    void* node = CSF_AT(void*, CSF_AT(void*, playerList, 0x4), 0);
    while (node != CSF_AT(void*, playerList, 0x4))
    {
        void* player = CSF_AT(void*, node, 0x8);
        void* component = reinterpret_cast<uint8_t*>(player) + 0x6B0;

        const bool flagged =
            reinterpret_cast<bool(__fastcall*)(void*, void*)>(
                CSF_AT(void**, component, 0)[2])(component, nullptr);

        if (!flagged)
        {
            CSF_AT(uint8_t, component, 0x18) = 0;
            void* attachedObject =
                reinterpret_cast<void*>(GetValueIfNotFlagged(component, nullptr));
            if (attachedObject != nullptr)
            {
                MpSpawnPoints_PrepareObjectForRemoval(attachedObject, nullptr);
                reinterpret_cast<void*(__fastcall*)(void*, void*, int)>(
                    CSF_AT(void**, attachedObject, 0)[0])(
                        attachedObject, nullptr, 1);
            }

            MpSpawnPoints_ProcessInactivePlayerComponent(
                static_cast<int>(reinterpret_cast<uintptr_t>(component)));
        }

        if (updatePlayerState != 0)
            MpSpawnPoints_SetPlayerStateFlag(player, nullptr, 0);

        node = CSF_AT(void*, node, 0);
    }

    void* sentinel = CSF_AT(void*, playerList, 0x4);
    void* result = CSF_AT(void*, sentinel, 0);
    CSF_AT(void*, sentinel, 0) = sentinel;

    sentinel = CSF_AT(void*, playerList, 0x4);
    CSF_AT(void*, sentinel, 0x4) = sentinel;
    sentinel = CSF_AT(void*, playerList, 0x4);
    CSF_AT(int, playerList, 0x8) = 0;

    while (result != sentinel)
    {
        void* next = CSF_AT(void*, result, 0);
        Mem_FreeDebug(result);
        sentinel = CSF_AT(void*, playerList, 0x4);
        result = next;
    }

    if (sentinel != nullptr)
        return reinterpret_cast<void**>(Mem_FreeDebug(sentinel));

    return reinterpret_cast<void**>(result);
}

HOOK(0x00555390, S_00555390_ProcessPlayerList, "auto");
