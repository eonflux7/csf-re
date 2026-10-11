#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"

CSF_GLOBAL(float, g_fZero, 0x0082D4CC);

static void __fastcall WeaponFirearms_SetTimer(void* self, void* /*edx*/, float value)
{
    CSF_AT(float, self, 0x10) = value;
    CSF_AT(uint8_t, self, 0x14) = !(value > g_fZero);
}

HOOK(0x00476190, WeaponFirearms_SetTimer, "auto");
