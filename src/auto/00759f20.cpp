#include <cstdint>
#include <cstddef>
#include <cstring>

#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

// Callees, bound by address (signatures as given in the packet).
CSF_FUNC(void*, __fastcall, AssignStringRange_429B60, 0x00429B60, (void* self, void* edx, char* text, unsigned int length));
CSF_FUNC(char, __fastcall, sub_69DB80, 0x0069DB80, (void* self, void* edx, int a2, int a3, unsigned int a4));
CSF_FUNC(uint32_t*, __fastcall, sub_756920, 0x00756920, (void* self, void* edx));
CSF_FUNC(char, __fastcall, sub_756950, 0x00756950, (void* self, void* edx, int a2));
CSF_FUNC(int, __fastcall, sub_756C80, 0x00756C80, (void* a1, int a2));

// Data referenced by the original.
CSF_GLOBAL(const char*, g_pszConfig_sonidos, 0x008A8AEC);

// Initializes the sound configuration object at this: stores the config
// name string, resets the value buffer, and writes the result flag to *a4.
// Returns this (eax).
static C_0086a3d8* __fastcall C_0086a3d8_InitSoundConfig(C_0086a3d8* self, int edxArg, const char* a3, char* a4)
{
    self->field_0004 = 0;
    *reinterpret_cast<void***>(self) = CSF_VTBL(0x0086A3D8);

    sub_756C80(self->pad_0008, edxArg);

    CSF_AT(uint32_t, self, 0x229C) = 0;
    CSF_AT(uint32_t, self, 0x22A0) = 15;
    CSF_AT(uint8_t, self, 0x228C) = 0;

    void* stringField = reinterpret_cast<uint8_t*>(self) + 0x22A4;
    sub_756920(stringField, nullptr);

    AssignStringRange_429B60(
        static_cast<void*>(reinterpret_cast<uint8_t*>(self) + 0x2288),
        nullptr,
        const_cast<char*>(a3),
        static_cast<unsigned int>(strlen(a3)));

    sub_756950(
        stringField,
        nullptr,
        static_cast<int>(reinterpret_cast<uintptr_t>(g_pszConfig_sonidos)));

    char* valueText;
    if (CSF_AT(uint32_t, self, 0x22A0) < 0x10u)
        valueText = reinterpret_cast<char*>(self) + 0x228C;
    else
        valueText = CSF_AT(char*, self, 0x228C);

    *a4 = sub_69DB80(
        self,
        nullptr,
        static_cast<int>(reinterpret_cast<uintptr_t>(valueText)),
        static_cast<int>(reinterpret_cast<uintptr_t>(self->pad_0008)),
        1u);

    return self;
}

HOOK(0x00759F20, C_0086a3d8_InitSoundConfig, "auto");
