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

static uint8_t __fastcall C_00858180_ApplyDdlSettings(C_00858180* self, void* /*edx*/,
    char a2, int arg4, int a4, int a5, int a6, int a7, int a8, int a9, int a10,
    int a11, int a12, int a13, int a14, int a15, int a16, int a17,
    char a18, int a19, int a3)
{
    if (IsDdlActive(self)) {
        DdlApplySettingsA(self, nullptr, a2, (char)arg4, (char)a4, (char)a5, (char)a6, (char)a7, (char)a8, (char)a9, (char)a10, (char)a11, (char)a12, (char)a13, (char)a14, (char)a15, (char)a16, (char)a17, 1, (char)a19, a3);
    } else if (!a18) {
        DdlApplySettingsB(self, nullptr, a2, (char)arg4, (char)a4, (char)a5, (char)a6, (char)a7, (char)a8, (char)a9, (char)a10, (char)a11, (char)a12, (char)a13, (char)a14, (char)a15, (char)a16, (char)a17, 0, (char)a19, a3);
    }

    if (!a18) {
        if (!IsDdlActive(self)) {
            return 0;
        }
    }

    CSF_AT(char, self, 0xD4) = a2;
    CSF_AT(int, self, 0xD8) = arg4;
    CSF_AT(int, self, 0xDC) = a4;
    CSF_AT(int, self, 0xE0) = a5;
    CSF_AT(int, self, 0xE4) = a6;
    CSF_AT(int, self, 0xE8) = a7;
    CSF_AT(int, self, 0xEC) = a8;
    CSF_AT(int, self, 0xF0) = a9;
    CSF_AT(int, self, 0xF4) = a10;
    CSF_AT(int, self, 0xF8) = a11;
    CSF_AT(int, self, 0xFC) = a12;
    CSF_AT(int, self, 0x100) = a13;
    CSF_AT(int, self, 0x104) = a14;
    CSF_AT(int, self, 0x108) = a15;
    CSF_AT(int, self, 0x10C) = a16;
    CSF_AT(int, self, 0x110) = a17;
    CSF_AT(char, self, 0x114) = 0;
    CSF_AT(char, self, 0x118) = 1;
    CSF_AT(int, self, 0x120) = a19;
    CSF_AT(int, self, 0x124) = a3;
    return (uint8_t)a15;
}

HOOK(0x00740190, C_00858180_ApplyDdlSettings, "auto");
