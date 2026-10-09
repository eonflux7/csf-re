#include <cstdint>
#include <cstddef>

#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

CSF_GLOBAL(float, g_fZero, 0x0082D4CC);
CSF_GLOBAL(float, g_FloatMax, 0x00831090);

static double __fastcall C_00840ff0_GetMaxValueIfFlagSet(C_00840ff0* self, void* /*edx*/)
{
    // Byte flag at +0x5C: non-zero yields FLT_MAX, zero yields 0.0
    if (CSF_AT(uint8_t, self, 0x5C) != 0)
        return (double)g_FloatMax;
    return (double)g_fZero;
}

HOOK(0x00526460, C_00840ff0_GetMaxValueIfFlagSet, "auto");
