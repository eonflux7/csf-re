#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

static CSF_FUNC(unsigned int, __fastcall, UpdateParticleTankSetting, 0x006D90A0,
               (void* self, void* edx, int a2));

static int __fastcall C_008611d4_SetParticleTankSetting(
    C_008611d4* self, void* /*edx*/, int setting)
{
    if (CSF_AT(int, self, 0xA4) == setting)
        return setting;

    CSF_AT(int, self, 0xA4) = setting;
    UpdateParticleTankSetting(self, nullptr, setting);
    CSF_AT(int, self, 0xE8) = 0;
    CSF_AT(int, self, 0xEC) = 0;
    CSF_AT(int, self, 0xF0) = 0;
    CSF_AT(int, self, 0xF4) = 0;
    return 0;
}

HOOK(0x006D93F0, C_008611d4_SetParticleTankSetting, "auto");
