#include <cstdint>
#include "globals.h"
#include "hook.h"

// Global flag/pointer tested by the original.
CSF_GLOBAL(int32_t, g_Value_8B0674, 0x8B0674);

// Returns non-zero (1) when the global at 0x8B0674 is non-zero, else 0.
static int __cdecl IsGlobalValueNonZero()
{
    return g_Value_8B0674 != 0;
}

HOOK(0x521BF0, IsGlobalValueNonZero, "auto");
