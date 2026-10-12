#include <cstdint>
#include <cstring>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

CSF_FUNC(void*, __fastcall, AssignStringRange_429B60, 0x00429B60,
         (void* self, void* edx, char* text, unsigned int length));
CSF_FUNC(char, __fastcall, S_004fb710_FindOptionValue, 0x004FB7A0,
         (void* self, void* edx, char* String2, uint8_t* a3));

static char __fastcall S_004fb710_ParseConsoleOptions(
    S_004fb710* self, void* /*edx*/)
{
    alignas(8) uint8_t optionValue[0x100] = {};

    if (S_004fb710_FindOptionValue(
            self, nullptr,
            const_cast<char*>(CSF_STR(0x0083F35C, "-scene")), optionValue))
    {
        char* text = reinterpret_cast<char*>(optionValue);
        AssignStringRange_429B60(
            reinterpret_cast<uint8_t*>(self) + 0x1C,
            nullptr, text, static_cast<unsigned int>(std::strlen(text)));
    }

    if (S_004fb710_FindOptionValue(
            self, nullptr,
            const_cast<char*>(CSF_STR(0x0083F354, "-log")), optionValue))
    {
        self->field_0038 = 1;
    }

    char result = S_004fb710_FindOptionValue(
        self, nullptr,
        const_cast<char*>(CSF_STR(0x0083F348, "-instance")), optionValue);
    if (result)
        self->field_0039 = 1;

    return result;
}

HOOK(0x004FB850, S_004fb710_ParseConsoleOptions, "auto");
