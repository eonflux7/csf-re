#include <cstdint>
#include <cstddef>
#include <cstring>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_FUNC(std::uint32_t*, __fastcall, MenuProgressInput_AssignText, 0x00696F90, (std::uint32_t* self, void* edx, std::int16_t* text));
CSF_FUNC(uint32_t*, __fastcall, SaveSlots_ApplyIndex, 0x00697020, (void* self, void* edx, int16_t a2));

static void* __fastcall MenuMpAccounts_GetPlayerName(void* self, void* /*edx*/)
{
    uint8_t* base = reinterpret_cast<uint8_t*>(self);
    void* nameObj = base + 1832;

    if (CSF_AT(uint8_t, self, 1829) && CSF_AT(int, self, 1860) == CSF_AT(signed char, self, 1100))
        return nameObj;

    CSF_AT(int, self, 1860) = CSF_AT(signed char, self, 1100);
    CSF_AT(uint8_t, self, 1829) = 1;
    MenuProgressInput_AssignText(reinterpret_cast<std::uint32_t*>(nameObj), nullptr,
                                 reinterpret_cast<std::int16_t*>(base + 1380));

    signed char value = CSF_AT(signed char, self, 1100);
    if (value != -1)
    {
        SaveSlots_ApplyIndex(nameObj, nullptr, 40);

        int hundreds = value / 100;
        if (hundreds)
        {
            SaveSlots_ApplyIndex(nameObj, nullptr, (int16_t)(signed char)(hundreds + 48));
            value = (signed char)(value % 100);
        }

        int tens = value / 10;
        if (tens)
        {
            SaveSlots_ApplyIndex(nameObj, nullptr, (int16_t)(signed char)(tens + 48));
            value = (signed char)(value % 10);
        }

        SaveSlots_ApplyIndex(nameObj, nullptr, (int16_t)(signed char)(value + 48));
        SaveSlots_ApplyIndex(nameObj, nullptr, 41);
    }
    return nameObj;
}

HOOK(0x741010, MenuMpAccounts_GetPlayerName, "auto");
