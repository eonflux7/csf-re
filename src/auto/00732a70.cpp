#include <cstdint>
#include <cstring>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

CSF_FUNC(int, __fastcall, SaveGame_ValidateName, 0x00696BB0, (void* name, void* edx, const uint8_t* separator));
CSF_FUNC(int, __cdecl, MenuExtras_ParseDescriptionId, 0x00698D90, (const char* text));
CSF_FUNC(int, __cdecl, _tolower, 0x007B1905, (int C));

CSF_GLOBAL(uint8_t, g_byte_82E65C, 0x0082E65C);
CSF_GLOBAL(uint8_t, g_DescriptionIdTerminator, 0x008670EC);

static int __fastcall C_008670f0_ParseAndDispatchDescription(
    C_008670f0* self, void* /*edx*/, const uint8_t* text, int argument)
{
    int result = SaveGame_ValidateName(
        reinterpret_cast<uint8_t*>(self) + 0x20, nullptr, &g_byte_82E65C);
    if (static_cast<uint8_t>(result) != 0)
        return result;

    alignas(8) uint8_t descriptionBytes[0xC];
    char* description = reinterpret_cast<char*>(descriptionBytes);
    std::memcpy(description, CSF_STR(0x008670E8, "AAAA"), 4);
    descriptionBytes[4] = g_DescriptionIdTerminator;

    unsigned int index = 0;
    do
    {
        if (*text == 0)
            break;
        descriptionBytes[index] = static_cast<uint8_t>(
            _tolower(static_cast<int>(static_cast<int8_t>(*text))));
        ++index;
        ++text;
    } while (index < 4);

    int descriptionId = MenuExtras_ParseDescriptionId(description);
    return self->vSlot30(descriptionId, argument);
}

HOOK(0x00732A70, C_008670f0_ParseAndDispatchDescription, "auto");
