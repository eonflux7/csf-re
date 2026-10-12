#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

static int __fastcall C_00867260_SetValue(C_00867260* self, void* /*edx*/, int value)
{
    if (CSF_AT(int, self, 0x54) != value)
    {
        CSF_AT(uint8_t, self, 0x20) = 1;
        CSF_AT(int, self, 0x54) = value;
    }
    return value;
}

HOOK(0x007370D0, C_00867260_SetValue, "auto");

