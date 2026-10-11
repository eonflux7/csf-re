#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_FUNC(void*, __cdecl, MenuMpAccounts_CreateListSentinel, 0x00552DD0, ());
CSF_FUNC(int, __cdecl, Mem_FreeDebug, 0x00690E60, (void* block));
CSF_FUNC(void*, __cdecl, Mem_Alloc, 0x006910F0, (std::size_t size));
CSF_FUNC(int, __cdecl, GetGlobalDword_73C020, 0x0073C020, ());
CSF_FUNC(int, __fastcall, MenuMpAccounts_FillPlayerList, 0x0073F3B0, (void* self, void* edx, void* list));
CSF_FUNC(void*, __fastcall, MpSpawnPoints_CreateNameNode, 0x0073F700, (void* self, void* edx));
CSF_FUNC(int, __fastcall, MpSpawnPoints_AttachNameNode, 0x0073F730, (void* self, void* edx, int a2));
CSF_FUNC(int, __fastcall, MpSpawnPoints_ReleaseNameNode, 0x0073F750, (void* self, void* edx));
CSF_FUNC(int, __fastcall, GetValueIfNotFlagged, 0x007408C0, (void* self, void*));
CSF_FUNC(int, __fastcall, MpSpawnPoints_UpdatePlayerState, 0x00740F40, (void* self, void* edx, int a2));

CSF_GLOBAL(uint32_t, g_CharacterSelectState8B0694, 0x008B0694);

static void** __cdecl MpSpawnPoints_PreparePlayerNameNodes()
{
    uint8_t list[0x10]{};
    CSF_AT(void*, list, 0x4) = MenuMpAccounts_CreateListSentinel();
    CSF_AT(int, list, 0x8) = 0;

    void* accounts = reinterpret_cast<void*>(
        static_cast<uintptr_t>(GetGlobalDword_73C020()));
    MenuMpAccounts_FillPlayerList(accounts, nullptr, list);

    uint8_t selectionFlag = 0;
    if (g_CharacterSelectState8B0694)
    {
        const uint32_t selectionAddress = CSF_AT(
            uint32_t,
            reinterpret_cast<void*>(g_CharacterSelectState8B0694),
            0x4);
        if (selectionAddress && selectionAddress - 776 != 0)
        {
            void* selection = reinterpret_cast<void*>(selectionAddress);
            selectionFlag =
                ((uint8_t(__fastcall*)(void*, void*))
                    CSF_AT(void**, selection, 0)[2])(selection, nullptr);
        }
    }
    CSF_AT(uint8_t, list, 0xF) = selectionFlag;

    void* node = CSF_AT(void*, CSF_AT(void*, list, 0x4), 0);
    while (node != CSF_AT(void*, list, 0x4))
    {
        void* player = CSF_AT(void*, node, 0x8);
        void* nameComponent = static_cast<uint8_t*>(player) + 0x6B0;

        if (!((bool(__fastcall*)(void*, void*))
                  CSF_AT(void**, nameComponent, 0)[2])(nameComponent, nullptr))
        {
            if (GetValueIfNotFlagged(nameComponent, nullptr))
            {
                const int existingNode = GetValueIfNotFlagged(nameComponent, nullptr);
                MpSpawnPoints_ReleaseNameNode(
                    reinterpret_cast<void*>(static_cast<uintptr_t>(existingNode)),
                    nullptr);
            }

            void* nameNode = Mem_Alloc(8);
            if (nameNode)
                nameNode = MpSpawnPoints_CreateNameNode(nameNode, nullptr);

            MpSpawnPoints_AttachNameNode(
                nameNode, nullptr,
                static_cast<int>(reinterpret_cast<uintptr_t>(nameComponent)));
            CSF_AT(uint8_t, nameComponent, 0x18) = 1;
        }

        if (CSF_AT(uint8_t, list, 0xF))
            MpSpawnPoints_UpdatePlayerState(player, nullptr, 0);

        node = CSF_AT(void*, node, 0);
    }

    void* sentinel = CSF_AT(void*, list, 0x4);
    void* result = CSF_AT(void*, sentinel, 0);
    CSF_AT(void*, sentinel, 0) = sentinel;

    sentinel = CSF_AT(void*, list, 0x4);
    CSF_AT(void*, sentinel, 0x4) = sentinel;
    sentinel = CSF_AT(void*, list, 0x4);
    CSF_AT(int, list, 0x8) = 0;

    while (result != sentinel)
    {
        void* next = CSF_AT(void*, result, 0);
        Mem_FreeDebug(result);
        sentinel = CSF_AT(void*, list, 0x4);
        result = next;
    }

    if (sentinel)
        return reinterpret_cast<void**>(
            static_cast<uintptr_t>(Mem_FreeDebug(sentinel)));

    return static_cast<void**>(result);
}

HOOK(0x00555270, MpSpawnPoints_PreparePlayerNameNodes, "auto");
