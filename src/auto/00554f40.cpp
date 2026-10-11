#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"

CSF_FUNC(void*, __cdecl, MenuMpAccounts_CreateListSentinel, 0x00552DD0, ());
CSF_FUNC(int, __cdecl, Mem_FreeDebug, 0x00690E60, (void* block));
CSF_FUNC(int, __cdecl, GetGlobalDword_73C020, 0x0073C020, ());
CSF_FUNC(int, __fastcall, MenuMpAccounts_FillPlayerList, 0x0073F3B0, (void* self, void* edx, void* list));

static bool __stdcall MpSpawnPoints_HasMatchingPlayer(int identifier)
{
    alignas(4) std::uint8_t list[0xC] = {};
    CSF_AT(void*, list, 0x4) = MenuMpAccounts_CreateListSentinel();
    CSF_AT(int, list, 0x8) = 0;

    void* manager = reinterpret_cast<void*>(GetGlobalDword_73C020());
    MenuMpAccounts_FillPlayerList(manager, nullptr, list);

    void* head = CSF_AT(void*, list, 0x4);
    void* first = CSF_AT(void*, head, 0x0);
    void* node = first;
    bool found = false;

    while (node != head)
    {
        if (found)
            break;

        void* player = CSF_AT(void*, node, 0x8);
        if (player &&
            CSF_AT(std::int8_t, player, 0x443) == identifier &&
            CSF_AT(std::uint8_t, player, 0x458) == 1)
        {
            found = true;
        }

        node = CSF_AT(void*, node, 0x0);
    }

    CSF_AT(void*, head, 0x0) = head;
    void* sentinel = CSF_AT(void*, list, 0x4);
    CSF_AT(void*, sentinel, 0x4) = sentinel;
    sentinel = CSF_AT(void*, list, 0x4);
    CSF_AT(int, list, 0x8) = 0;

    while (first != sentinel)
    {
        void* next = CSF_AT(void*, first, 0x0);
        Mem_FreeDebug(first);
        sentinel = CSF_AT(void*, list, 0x4);
        first = next;
    }

    if (sentinel)
        Mem_FreeDebug(sentinel);

    return found;
}

HOOK(0x00554F40, MpSpawnPoints_HasMatchingPlayer, "auto");
