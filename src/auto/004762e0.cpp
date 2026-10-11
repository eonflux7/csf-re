#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"

static int __fastcall WeaponFirearms_SetCurrentAndPreviousValue(void* self, void* /*edx*/, int value)
{
    CSF_AT(int, self, 0x50) = value;
    CSF_AT(int, self, 0x4C) = value;
    return value;
}

HOOK(0x004762E0, WeaponFirearms_SetCurrentAndPreviousValue, "auto");
