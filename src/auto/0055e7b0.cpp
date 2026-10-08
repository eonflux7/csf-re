#include <cstdint>
#include "hook.h"
#include "core/memory.h"
#include "globals.h"

CSF_FUNC(void*, __cdecl, MenuMpAccounts_CreateListSentinel, 0x00552DD0, ());
CSF_FUNC(void*, __cdecl, MenuMpAccounts_GetAccountManager, 0x0073C020, ());
CSF_FUNC(int, __fastcall, MenuMpAccounts_FillPlayerList, 0x0073F3B0,
         (void* self, void* edx, void* list));
CSF_FUNC(int, __fastcall, MenuMpAccounts_GetStoredSelection, 0x00714CA0,
         (void* self, void* edx, int index));
CSF_FUNC(void*, __fastcall, MenuMpAccounts_GetListControl, 0x0072FF80,
         (void* self, void* edx));
CSF_FUNC(int, __fastcall, MenuMpAccounts_ClearList, 0x00715660,
         (void* self, void* edx));
CSF_FUNC(void, __fastcall, MenuMpAccounts_SelectItem, 0x00716A60,
         (void* self, void* edx, int index));
CSF_FUNC(void*, __fastcall, MenuMpAccounts_GetPlayerName, 0x00741010,
         (void* self, void* edx));
CSF_FUNC(int, __fastcall, MenuMpAccounts_CheckName, 0x00696BB0,
         (void* self, void* edx, const char* text));
CSF_FUNC(int, __fastcall, MenuMpAccounts_CompareNames, 0x005158F0,
         (void* self, void* edx, unsigned int offset, unsigned int length,
          const uint16_t* text, unsigned int textLength));
CSF_FUNC(int, __stdcall, MenuMpAccounts_FormatPlayerStatus, 0x0055DFC0,
         (void* player, uint16_t* buffer));
CSF_FUNC(void, __cdecl, MenuMpAccounts_ReportInvalidPlayerId, 0x00646D30,
         (int first, int second, int error, int fourth));
CSF_FUNC(int, __cdecl, MenuMpAccounts_FreeListNode, 0x00690E60,
         (void* node));

CSF_GLOBAL(uint8_t, g_EmptyAccountName, 0x0082E65C);

static void** __fastcall MenuMpAccounts_RefreshPlayerList(
    void* self, void* /*edx*/)
{
    uint16_t statusText[256];

    // The first word is deliberately uninitialized, as in the original.
    uint32_t listState[3];
    listState[1] = reinterpret_cast<uint32_t>(
        MenuMpAccounts_CreateListSentinel());
    listState[2] = 0;
    int row = 0;

    void* manager = MenuMpAccounts_GetAccountManager();
    MenuMpAccounts_FillPlayerList(manager, nullptr, listState);

    int selectedItem = MenuMpAccounts_GetStoredSelection(
        CSF_AT(void*, self, 0x24), nullptr, 0);

    void* control = MenuMpAccounts_GetListControl(
        CSF_AT(void*, self, 0x24), nullptr);
    int savedPosition =
        reinterpret_cast<int(__fastcall*)(void*, void*)>(
            CSF_AT(void**, control, 0)[0x270 / 4])(control, nullptr);

    MenuMpAccounts_ClearList(CSF_AT(void*, self, 0x24), nullptr);

    void* listBox = CSF_AT(void*, self, 0x24);
    reinterpret_cast<void(__fastcall*)(void*, void*)>(
        CSF_AT(void**, listBox, 0)[0x298 / 4])(listBox, nullptr);

    void* node = CSF_AT(void*, reinterpret_cast<void*>(listState[1]), 0);
    while (node != reinterpret_cast<void*>(listState[1]))
    {
        void* player = CSF_AT(void*, node, 0x08);
        void* name = MenuMpAccounts_GetPlayerName(player, nullptr);

        if (static_cast<uint8_t>(MenuMpAccounts_CheckName(
                name, nullptr,
                reinterpret_cast<const char*>(&g_EmptyAccountName))) != 0)
        {
            void* managerName = static_cast<uint8_t*>(
                MenuMpAccounts_GetAccountManager()) + 0x0C;
            name = MenuMpAccounts_GetPlayerName(player, nullptr);

            unsigned int managerNameLength =
                CSF_AT(uint32_t, managerName, 0x14);
            const uint16_t* managerNameText;
            if (CSF_AT(uint32_t, managerName, 0x18) < 8u)
                managerNameText = reinterpret_cast<const uint16_t*>(
                    static_cast<uint8_t*>(managerName) + 4);
            else
                managerNameText = CSF_AT(uint16_t*, managerName, 0x04);

            unsigned int nameLength = CSF_AT(uint32_t, name, 0x14);
            if (MenuMpAccounts_CompareNames(
                    name, nullptr, 0, nameLength,
                    managerNameText, managerNameLength) != 0)
            {
                int status = MenuMpAccounts_FormatPlayerStatus(
                    player, statusText);

                uint32_t playerId = CSF_AT(uint32_t, player, 0x2C);
                if ((playerId & 0x003FFFFFu) == 0)
                {
                    MenuMpAccounts_ReportInvalidPlayerId(
                        0, 0, -536870898, 0);
                    playerId = 0;
                }

                name = MenuMpAccounts_GetPlayerName(player, nullptr);
                const uint16_t* nameText;
                if (CSF_AT(uint32_t, name, 0x18) < 8u)
                    nameText = reinterpret_cast<const uint16_t*>(
                        static_cast<uint8_t*>(name) + 4);
                else
                    nameText = CSF_AT(uint16_t*, name, 0x04);

                int currentRow = row;
                listBox = CSF_AT(void*, self, 0x24);
                reinterpret_cast<void(__fastcall*)(
                    void*, void*, int, int, const uint16_t*, uint32_t)>(
                        CSF_AT(void**, listBox, 0)[0x294 / 4])(
                            listBox, nullptr, currentRow, 0,
                            nameText, playerId);

                listBox = CSF_AT(void*, self, 0x24);
                reinterpret_cast<void(__fastcall*)(
                    void*, void*, int, int, const uint16_t*, int)>(
                        CSF_AT(void**, listBox, 0)[0x294 / 4])(
                            listBox, nullptr, currentRow, 1,
                            statusText, status);

                row = static_cast<int>(
                    static_cast<uint32_t>(currentRow) + 1u);
            }
        }

        node = CSF_AT(void*, node, 0);
    }

    control = MenuMpAccounts_GetListControl(
        CSF_AT(void*, self, 0x24), nullptr);
    reinterpret_cast<void(__fastcall*)(void*, void*, int, int)>(
        CSF_AT(void**, control, 0)[0x26C / 4])(
            control, nullptr, savedPosition, 0);

    MenuMpAccounts_SelectItem(
        CSF_AT(void*, self, 0x24), nullptr, selectedItem);

    listBox = CSF_AT(void*, self, 0x24);
    reinterpret_cast<void(__fastcall*)(void*, void*, int, int)>(
        CSF_AT(void**, listBox, 0)[0x2A8 / 4])(
            listBox, nullptr, 0, 0);

    node = CSF_AT(void*, reinterpret_cast<void*>(listState[1]), 0);
    CSF_AT(void*, reinterpret_cast<void*>(listState[1]), 0) =
        reinterpret_cast<void*>(listState[1]);
    CSF_AT(void*, reinterpret_cast<void*>(listState[1]), 4) =
        reinterpret_cast<void*>(listState[1]);
    listState[2] = 0;

    while (node != reinterpret_cast<void*>(listState[1]))
    {
        void* next = CSF_AT(void*, node, 0);
        MenuMpAccounts_FreeListNode(node);
        node = next;
    }

    void* sentinel = reinterpret_cast<void*>(listState[1]);
    if (sentinel)
        return reinterpret_cast<void**>(
            static_cast<uint32_t>(MenuMpAccounts_FreeListNode(sentinel)));
    return reinterpret_cast<void**>(node);
}

HOOK(0x0055E7B0, MenuMpAccounts_RefreshPlayerList, "auto");
