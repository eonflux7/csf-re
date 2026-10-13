#include <cstdint>
#include <cstddef>
#include <cstring>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_FUNC(int, __fastcall, MenuMpAccounts_CompareNames, 0x005158F0, (void* self, void* edx, unsigned int offset, unsigned int length, const uint16_t* text, unsigned int textLength));
CSF_FUNC(uint32_t*, __cdecl, MenuMpAccounts_CreateListSentinel, 0x00552DD0, ());
CSF_FUNC(int, __stdcall, CMenuPlayerList_BuildPlayerStatusIcons, 0x0055E110, (const std::uint16_t* playerName, int connectionId, int playerId, std::uint16_t* output, int));
CSF_FUNC(void, __cdecl, Game_RaiseStatusNotice, 0x00646D30, (int arg0, int arg1, uint32_t code, int arg3));
CSF_FUNC(int, __cdecl, Mem_FreeDebug, 0x00690E60, (void* block));
CSF_FUNC(unsigned int, __cdecl, CopyTrucoStringToWide, 0x00695590, (const char* src, uint16_t* dst, int maxCount));
CSF_FUNC(unsigned int, __cdecl, ConvertWideTextToNarrow, 0x00695620, (std::uint16_t* source, char* destination, unsigned int length));
CSF_FUNC(int, __cdecl, WideString_Compare, 0x00695760, (uint16_t* a1, uint16_t* a2));
CSF_FUNC(int, __fastcall, SaveGame_ValidateName, 0x00696BB0, (void* self, void* , char* a2));
CSF_FUNC(int, __fastcall, GetStoredValueAtIndex, 0x00714CA0, (void* self, void* , int index));
CSF_FUNC(int, __fastcall, CGuiListBoxBase_ClearStorageAndSendEvent, 0x00715660, (CGuiListBoxBase* self, void*));
CSF_FUNC(void, __fastcall, CGuiListBoxBase_AddSelectedItem, 0x00716A60, (CGuiListBoxBase* self, void* , int index));
CSF_FUNC(int, __fastcall, GetCellStringAddress, 0x00723030, (void* self, void* , int firstIndex, int secondIndex));
CSF_FUNC(int, __fastcall, CGuiStatic_GetField0134, 0x0072FF80, (CGuiStatic* self, void*));
CSF_FUNC(int, __cdecl, GetGlobalDword_73C020, 0x0073C020, ());
CSF_FUNC(int, __fastcall, MenuMpAccounts_FillPlayerList, 0x0073F3B0, (void* self, void* edx, void* list));
CSF_FUNC(void*, __fastcall, MenuMpAccounts_GetPlayerName, 0x00741010, (void* self, void*));
CSF_FUNC(char*, __fastcall, MpSpawnPoints_GetPlayerEntry, 0x0074A520, (void* self, void* edx, unsigned int a2));
CSF_FUNC(int, __cdecl, _sprintf, 0x007AF9BB, (char* buffer, const char* format, ...));

CSF_GLOBAL(uint8_t, g_byte_82E65C, 0x0082E65C);

static uint16_t* MenuMpAccounts_GetWideStringData(void* stringObject)
{
    if (CSF_AT(uint32_t, stringObject, 0x18) < 8u)
        return reinterpret_cast<uint16_t*>(
            static_cast<uint8_t*>(stringObject) + 4);
    return reinterpret_cast<uint16_t*>(
        CSF_AT(uint32_t, stringObject, 4));
}

static void* CMenuPlayerList_GetListControl(CMenuPlayerList* self)
{
    return reinterpret_cast<void*>(self->field_0024);
}

static void CMenuPlayerList_SetCell(
    void* control, int row, int column, uint16_t* text, int value)
{
    reinterpret_cast<void (__fastcall*)(
        void*, void*, int, int, uint16_t*, int)>(
            CSF_AT(void**, control, 0)[0x294 / 4])(
                control, nullptr, row, column, text, value);
}

static void CMenuPlayerList_SetRowColor(
    void* control, int row, float* color)
{
    reinterpret_cast<void (__fastcall*)(void*, void*, int, int, float*)>(
        CSF_AT(void**, control, 0)[0x2A4 / 4])(
            control, nullptr, row, 0, color);
}

static void** __fastcall CMenuPlayerList_RefreshPlayers(
    CMenuPlayerList* self, void* /*edx*/)
{
    alignas(8) uint8_t offlineIconsBytes[0x200];
    alignas(8) uint8_t textBytes[0x200];
    alignas(8) uint8_t formattedNameBytes[0x100];
    alignas(8) uint8_t narrowNameBytes[0x10C];
    alignas(8) uint8_t colorBytes[0x14];
    alignas(8) uint8_t listBytes[0x18];

    auto* offlineIcons = reinterpret_cast<uint16_t*>(offlineIconsBytes);
    auto* text = reinterpret_cast<uint16_t*>(textBytes);
    auto* formattedName = reinterpret_cast<char*>(formattedNameBytes);
    auto* narrowName = reinterpret_cast<char*>(narrowNameBytes);
    auto* color = reinterpret_cast<float*>(colorBytes);

    int rowCount = 0;
    int selectedValue = GetStoredValueAtIndex(
        CMenuPlayerList_GetListControl(self), nullptr, 0);

    void* scrollControl = reinterpret_cast<void*>(
        CGuiStatic_GetField0134(
            reinterpret_cast<CGuiStatic*>(self->field_0024), nullptr));
    int scrollPosition = reinterpret_cast<int (__fastcall*)(void*, void*)>(
        CSF_AT(void**, scrollControl, 0)[0x270 / 4])(
            scrollControl, nullptr);

    void* control = CMenuPlayerList_GetListControl(self);
    reinterpret_cast<void (__fastcall*)(void*, void*)>(
        CSF_AT(void**, control, 0)[0x298 / 4])(control, nullptr);

    CGuiListBoxBase_ClearStorageAndSendEvent(
        reinterpret_cast<CGuiListBoxBase*>(self->field_0024), nullptr);

    uint32_t* sentinel = MenuMpAccounts_CreateListSentinel();
    CSF_AT(uint32_t*, listBytes, 4) = sentinel;
    CSF_AT(int, listBytes, 8) = 0;
    void* accounts = reinterpret_cast<void*>(GetGlobalDword_73C020());
    MenuMpAccounts_FillPlayerList(accounts, nullptr, listBytes);

    void* node = CSF_AT(void*, sentinel, 0);
    int liveRows = 0;
    while (node != sentinel)
    {
        void* player = CSF_AT(void*, node, 8);
        if (player)
        {
            accounts = reinterpret_cast<void*>(GetGlobalDword_73C020());
            uint16_t* localName = MenuMpAccounts_GetWideStringData(
                static_cast<uint8_t*>(accounts) + 12);
            uint16_t* playerName = MenuMpAccounts_GetWideStringData(
                MenuMpAccounts_GetPlayerName(player, nullptr));

            if (WideString_Compare(playerName, localName))
            {
                int playerId = CSF_AT(int, player, 0x2C);
                if ((playerId & 0x3FFFFF) == 0)
                {
                    Game_RaiseStatusNotice(0, 0, -536870898, 0);
                    playerId = 0;
                }

                int connectionId = CSF_AT(int, player, 0x674);
                playerName = MenuMpAccounts_GetWideStringData(
                    MenuMpAccounts_GetPlayerName(player, nullptr));
                int icons = CMenuPlayerList_BuildPlayerStatusIcons(
                    playerName, connectionId, playerId, text, 0);

                playerId = CSF_AT(int, player, 0x2C);
                if ((playerId & 0x3FFFFF) == 0)
                {
                    Game_RaiseStatusNotice(0, 0, -536870898, 0);
                    playerId = 0;
                }

                playerName = MenuMpAccounts_GetWideStringData(
                    MenuMpAccounts_GetPlayerName(player, nullptr));
                CMenuPlayerList_SetCell(
                    CMenuPlayerList_GetListControl(self),
                    liveRows, 0, playerName, playerId);
                CMenuPlayerList_SetCell(
                    CMenuPlayerList_GetListControl(self),
                    liveRows, 1, text, icons);

                control = CMenuPlayerList_GetListControl(self);
                color[0] = 200.0f;
                color[1] = 200.0f;
                color[2] = 200.0f;
                color[3] = 255.0f;
                CMenuPlayerList_SetRowColor(control, liveRows, color);
                ++liveRows;
            }
        }
        node = CSF_AT(void*, node, 0);
    }
    rowCount = liveRows;

    accounts = reinterpret_cast<void*>(GetGlobalDword_73C020());
    void* spawnPoints = CSF_AT(void*, accounts, 0xC8);
    if (spawnPoints)
    {
        for (int index = 0; index < 30; ++index)
        {
            char* entry = MpSpawnPoints_GetPlayerEntry(
                spawnPoints, nullptr, index);
            if (!entry)
                continue;

            void* entryName = entry + 4;
            // This caller tests AL, despite the shared binding's int return.
            if (static_cast<uint8_t>(SaveGame_ValidateName(
                    entryName, nullptr,
                    reinterpret_cast<char*>(&g_byte_82E65C))) == 0)
                continue;

            accounts = reinterpret_cast<void*>(GetGlobalDword_73C020());
            uint32_t localCapacity = CSF_AT(uint32_t, accounts, 0x24);
            uint32_t localLength = CSF_AT(uint32_t, accounts, 0x20);
            uint16_t* localName;
            if (localCapacity < 8u)
                localName = reinterpret_cast<uint16_t*>(
                    static_cast<uint8_t*>(accounts) + 16);
            else
                localName = CSF_AT(uint16_t*, accounts, 0x10);

            if (!MenuMpAccounts_CompareNames(
                    entryName, nullptr, 0,
                    CSF_AT(uint32_t, entryName, 0x14),
                    localName, localLength))
                continue;

            bool alreadyListed = false;
            int row = 0;
            for (;;)
            {
                control = CMenuPlayerList_GetListControl(self);
                int count = reinterpret_cast<int (__fastcall*)(void*, void*)>(
                    CSF_AT(void**, control, 0)[0x274 / 4])(
                        control, nullptr);
                if (row >= count)
                    break;

                uint16_t* name = MenuMpAccounts_GetWideStringData(entryName);
                uint16_t* cell = reinterpret_cast<uint16_t*>(
                    GetCellStringAddress(
                        CMenuPlayerList_GetListControl(self), nullptr, row, 0));
                int comparison = WideString_Compare(cell, name);
                alreadyListed = comparison == 0;
                ++row;
                if (!comparison)
                    break;
            }

            if (alreadyListed)
                continue;

            std::memset(formattedName, 0, 253);
            CSF_AT(uint16_t, formattedNameBytes, 253) = 0;
            formattedNameBytes[255] = 0;
            std::memset(narrowName, 0, 253);
            CSF_AT(uint16_t, narrowNameBytes, 253) = 0;

            uint16_t* name = MenuMpAccounts_GetWideStringData(entry + 4);
            int icons = CMenuPlayerList_BuildPlayerStatusIcons(
                name, CSF_AT(int, entry, 0x24),
                CSF_AT(int, entry, 0), offlineIcons,
                static_cast<int>(reinterpret_cast<uintptr_t>(entry)));
            icons |= 0x40;

            text[0] = 0;
            name = MenuMpAccounts_GetWideStringData(entryName);
            ConvertWideTextToNarrow(name, narrowName, 256);
            _sprintf(formattedName, CSF_STR(0x008456D4, "(%s)"), narrowName);
            CopyTrucoStringToWide(formattedName, text, 256);

            int playerId = CSF_AT(int, entry, 0);
            int newRow = rowCount;
            CMenuPlayerList_SetCell(
                CMenuPlayerList_GetListControl(self),
                newRow, 0, text, playerId);
            CMenuPlayerList_SetCell(
                CMenuPlayerList_GetListControl(self),
                newRow, 1, offlineIcons, icons);

            control = CMenuPlayerList_GetListControl(self);
            color[0] = 150.0f;
            color[1] = 150.0f;
            color[2] = 150.0f;
            color[3] = 255.0f;
            CMenuPlayerList_SetRowColor(control, newRow, color);
            rowCount = newRow + 1;
        }
    }

    scrollControl = reinterpret_cast<void*>(
        CGuiStatic_GetField0134(
            reinterpret_cast<CGuiStatic*>(self->field_0024), nullptr));
    reinterpret_cast<void (__fastcall*)(void*, void*, int, int)>(
        CSF_AT(void**, scrollControl, 0)[0x26C / 4])(
            scrollControl, nullptr, scrollPosition, 0);

    CGuiListBoxBase_AddSelectedItem(
        reinterpret_cast<CGuiListBoxBase*>(self->field_0024),
        nullptr, selectedValue);

    control = CMenuPlayerList_GetListControl(self);
    reinterpret_cast<void (__fastcall*)(void*, void*, int, int)>(
        CSF_AT(void**, control, 0)[0x2A8 / 4])(
            control, nullptr, 0, 0);

    void** result = CSF_AT(void**, sentinel, 0);
    CSF_AT(uint32_t*, sentinel, 0) = sentinel;
    CSF_AT(uint32_t*, sentinel, 4) = sentinel;
    CSF_AT(int, listBytes, 8) = 0;
    while (result != reinterpret_cast<void**>(sentinel))
    {
        void** next = CSF_AT(void**, result, 0);
        Mem_FreeDebug(result);
        result = next;
    }

    if (sentinel)
        return reinterpret_cast<void**>(Mem_FreeDebug(sentinel));
    return result;
}

HOOK(0x0055E340, CMenuPlayerList_RefreshPlayers, "auto");
