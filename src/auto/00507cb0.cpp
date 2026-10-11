#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_FUNC(float*, __fastcall, S_005080e0_ParseColor, 0x00507B20, (void* self, void* edx, float* a2, char* a3));
CSF_FUNC(int, __fastcall, S_005080e0_ParseFontId, 0x00507C10, (void* self, void* edx, char* a2));
CSF_FUNC(int, __cdecl, MenuExtras_ParseDescriptionId, 0x00698D90, (const char* text));
CSF_FUNC(uint32_t, __cdecl, AppMain_ParseLanguageValue, 0x0069C310, (const char* text));
CSF_FUNC(int, __cdecl, _strncmp, 0x007B00B0, (const char* name, const char* prefix, std::size_t length));

static char __fastcall S_005080e0_ParseFormattingDirective(
    S_005080e0* self, void* /*edx*/, uint32_t* attributes, char* text)
{
    char* value = text + 4;

    if (_strncmp(text, CSF_STR(0x0083F784, "FLI:"), 4u) == 0)
    {
        attributes[0] |= 8u;
        attributes[6] = MenuExtras_ParseDescriptionId(value);
        return 1;
    }

    if (_strncmp(text, CSF_STR(0x0083F77C, "CLR:"), 4u) == 0)
    {
        attributes[0] |= 0x10u;
        alignas(8) uint8_t colorBytes[0x14];
        float* color = S_005080e0_ParseColor(
            self, nullptr, reinterpret_cast<float*>(colorBytes), value);
        attributes[2] = CSF_AT(uint32_t, color, 0x0);
        attributes[3] = CSF_AT(uint32_t, color, 0x4);
        attributes[4] = CSF_AT(uint32_t, color, 0x8);
        attributes[5] = CSF_AT(uint32_t, color, 0xC);
        return 1;
    }

    if (_strncmp(text, CSF_STR(0x0083F774, "FNT:"), 4u) == 0)
    {
        attributes[0] |= 0x20u;
        attributes[1] = S_005080e0_ParseFontId(self, nullptr, value);
        return 1;
    }

    if (_strncmp(text, CSF_STR(0x0083F76C, "LAN:"), 4u) == 0)
    {
        self->field_0064 = AppMain_ParseLanguageValue(value);
        return 1;
    }

    if (_strncmp(text, CSF_STR(0x0083F764, "LFF:"), 4u) == 0)
    {
        self->field_0064 = 0;
        return 1;
    }

    return 0;
}

HOOK(0x00507CB0, S_005080e0_ParseFormattingDirective, "auto");
