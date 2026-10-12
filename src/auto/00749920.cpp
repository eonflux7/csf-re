#include <cstdint>
#include <cstddef>
#include <cstring>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

CSF_FUNC(void*, __fastcall, AssignStringRange_429B60, 0x00429B60, (void* self, void* edx, char* text, unsigned int length));
CSF_FUNC(void*, __fastcall, Profiles_AppendStringRange, 0x0043EB30, (void* self, int a2, void* a3, unsigned int a4));
CSF_FUNC(void*, __fastcall, StringAppendFill, 0x00512630, (void* self, void* edx, unsigned int count, char ch));

CSF_GLOBAL(uint8_t, g_byte_82E65C, 0x0082E65C);

static char* __fastcall C_008310b8_GetSettingsSelection(
    C_008310b8* self, void* /*edx*/)
{
    auto* bytes = reinterpret_cast<uint8_t*>(self);
    auto* selection = reinterpret_cast<C_0082d410*>(bytes + 0x1A0);

    AssignStringRange_429B60(
        selection, nullptr, reinterpret_cast<char*>(&g_byte_82E65C), 0);

    StringAppendFill(
        selection, nullptr, 1,
        *reinterpret_cast<uint8_t*>(&self->field_0004) ? 76 : 87);

    StringAppendFill(
        selection, nullptr, 1,
        CSF_AT(uint8_t, self, 0x107) ? 80 : 65);

    switch (CSF_AT(int8_t, self, 0x6))
    {
    case 0:
        StringAppendFill(selection, nullptr, 1, 68);
        break;
    case 1:
        StringAppendFill(selection, nullptr, 1, 84);
        break;
    case 3:
        StringAppendFill(selection, nullptr, 1, 83);
        break;
    }

    StringAppendFill(
        selection, nullptr, 1,
        CSF_AT(uint8_t, self, 0x3) ? 70 : 88);
    StringAppendFill(selection, nullptr, 1, 80);

    char combination = 49;
    if (CSF_AT(uint8_t, self, 0x0))
    {
        const uint8_t secondEnabled = CSF_AT(uint8_t, self, 0x1);
        if (!secondEnabled || !CSF_AT(uint8_t, self, 0x2))
        {
            if (CSF_AT(uint8_t, self, 0x2))
                combination = 53;
            else
                combination = static_cast<char>(4 * (secondEnabled != 0) + 50);
        }
    }
    else
    {
        const uint8_t secondEnabled = CSF_AT(uint8_t, self, 0x1);
        const uint8_t thirdEnabled = CSF_AT(uint8_t, self, 0x2);
        if (secondEnabled)
            combination = static_cast<char>(4 * (thirdEnabled != 0) + 51);
        else if (thirdEnabled)
            combination = 52;
    }
    StringAppendFill(selection, nullptr, 1, combination);

    char* suffix = reinterpret_cast<char*>(bytes + 0x87);
    Profiles_AppendStringRange(
        selection, 0, suffix, static_cast<unsigned int>(std::strlen(suffix)));

    const char* text;
    if (CSF_AT(uint32_t, selection, 0x18) < 0x10)
        text = reinterpret_cast<const char*>(&selection->field_0004);
    else
        text = reinterpret_cast<const char*>(selection->field_0004);

    char* result = reinterpret_cast<char*>(bytes + 0x1BC);
    std::strcpy(result, text);
    return result;
}

HOOK(0x00749920, C_008310b8_GetSettingsSelection, "auto");
