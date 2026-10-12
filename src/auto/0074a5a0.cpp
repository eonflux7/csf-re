#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_FUNC(void*, __fastcall, AssignStringRange_488830, 0x00488830, (void* self, void* edx, void* source, std::uint32_t position, std::uint32_t count));
CSF_FUNC(void, __cdecl, Game_RaiseStatusNotice, 0x00646D30, (int arg0, int arg1, uint32_t code, int arg3));
CSF_FUNC(void*, __fastcall, TextInputSubobject_Init, 0x00697130, (void* self, void* edx, uint8_t* arg));
CSF_FUNC(void*, __fastcall, MenuMpAccounts_GetPlayerName, 0x00741010, (void* self, void*));
CSF_FUNC(int, __fastcall, C_00867900_FindAvailableEntry, 0x0074A250, (void* self, void* edx));
CSF_FUNC(int, __fastcall, C_00867900_FindEntryByPlayerName, 0x0074A4C0, (void* self, void* edx, uint32_t* a2));

CSF_GLOBAL(uint8_t, g_byte_82E65C, 0x0082E65C);

static void __fastcall C_00867900_BeginPlayerEntry(
    C_00867900* self, void* /*edx*/, C_00867900* account)
{
    CSF_AT(uint8_t, self, 0x5AC) = 1;
    if (!account)
        return;

    void* playerName = MenuMpAccounts_GetPlayerName(account, nullptr);
    int entryIndex = C_00867900_FindEntryByPlayerName(
        self, nullptr, static_cast<uint32_t*>(playerName));

    if (entryIndex == -1)
    {
        entryIndex = C_00867900_FindAvailableEntry(self, nullptr);
        if (entryIndex == -1)
            return;
    }

    uint8_t* entry = reinterpret_cast<uint8_t*>(self) + 48 * entryIndex;

    CSF_AT(uint32_t, entry, 0x04) = 0xFFFFFFFF;
    TextInputSubobject_Init(entry + 8, nullptr, &g_byte_82E65C);
    CSF_AT(uint8_t, entry, 0x2C) = 0;
    CSF_AT(uint8_t, entry, 0x2D) = 0;
    CSF_AT(uint32_t, entry, 0x24) = 0xFFFFFFFF;
    CSF_AT(uint8_t, entry, 0x2E) = 0;
    CSF_AT(uint8_t, entry, 0x2F) = 0;
    CSF_AT(uint8_t, entry, 0x30) = 0xFF;
    CSF_AT(uint32_t, entry, 0x28) = 0;

    CSF_AT(uint8_t, entry, 0x2D) = 1;
    CSF_AT(uint32_t, entry, 0x24) = CSF_AT(uint32_t, self, 0x5A4);

    playerName = MenuMpAccounts_GetPlayerName(account, nullptr);
    AssignStringRange_488830(entry + 8, nullptr, playerName, 0, 0xFFFFFFFF);

    uint32_t accountId = account->field_002c;
    if ((accountId & 0x3FFFFF) == 0)
    {
        Game_RaiseStatusNotice(0, 0, static_cast<uint32_t>(-536870898), 0);
        accountId = 0;
    }

    CSF_AT(uint32_t, entry, 0x04) = accountId;
    CSF_AT(uint32_t, entry, 0x28) = account->field_0674;
    ++CSF_AT(uint32_t, self, 0x5A4);
}

HOOK(0x0074A5A0, C_00867900_BeginPlayerEntry, "auto");
