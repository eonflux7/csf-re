#include <cstdint>
#include <cstddef>
#include <cstring>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_FUNC(int, __fastcall, Sub_64B250, 0x0064B250, (void* self, void* edx));
CSF_FUNC(char, __fastcall, DdlApplySettingsA, 0x00742430, (void* self, void* edx, char a2, char arg4, char a4, char a5, char a6, char a7, char a8, char a9, char a10, char a11, char a12, char a13, char a14, char a15, char a16, char a17, char a18, char a19, int a3));
CSF_FUNC(char, __fastcall, DdlApplySettingsB, 0x007475F0, (void* self, void* edx, char a2, char arg4, char a4, char a5, char a6, char a7, char a8, char a9, char a10, char a11, char a12, char a13, char a14, char a15, char a16, char a17, char a18, char a19, int a3));

// The original tests only AL of Sub_64B250's result.
static bool IsDdlActive(C_00858180* self)
{
    return (Sub_64B250(self, nullptr) & 0xFF) != 0;
}

static uint8_t __fastcall C_00858180_SetSettings(C_00858180* self, void* /*edx*/,
    char a2, int arg4, int a4, int a5, int a6, int a7, int a8, int a9, int a10,
    int a11, int a12, int a13, int a14, int a15, int a16, int a17,
    char a18, int a19, int a3)
{
    bool active = IsDdlActive(self);

    if (active) {
        DdlApplySettingsA(self, nullptr, a2, arg4, a4, a5, a6, a7, a8, a9, a10, a11, a12, a13, a14, a15, a16, a17, 1, a19, a3);
    } else if (!a18) {
        DdlApplySettingsB(self, nullptr, a2, arg4, a4, a5, a6, a7, a8, a9, a10, a11, a12, a13, a14, a15, a16, a17, 0, a19, a3);
    }

    if (!a18) {
        if (!IsDdlActive(self)) {
            return 0;
        }
    }

    CSF_AT(char, self, 0xB8) = a2;
    CSF_AT(int, self, 0xBC) = arg4;
    CSF_AT(int, self, 0xC0) = a4;
    CSF_AT(int, self, 0xC4) = a5;
    CSF_AT(int, self, 0xC8) = a6;
    CSF_AT(int, self, 0xCC) = a7;
    CSF_AT(int, self, 0xD0) = a8;
    CSF_AT(int, self, 0xD4) = a9;
    CSF_AT(int, self, 0xD8) = a10;
    CSF_AT(int, self, 0xDC) = a11;
    CSF_AT(int, self, 0xE0) = a12;
    CSF_AT(int, self, 0xE4) = a13;
    CSF_AT(int, self, 0xE8) = a14;
    CSF_AT(int, self, 0x100) = a19;
    CSF_AT(int, self, 0x104) = a3;
    CSF_AT(int, self, 0xEC) = a15;
    CSF_AT(int, self, 0xF0) = a16;
    CSF_AT(int, self, 0xF4) = a17;
    CSF_AT(char, self, 0xF8) = 0;
    CSF_AT(char, self, 0xFC) = 1;
    return (uint8_t)a15;
}

HOOK(0x0073F870, C_00858180_SetSettings, "auto");
