#include <cstdint>
#include "globals.h"
#include "hook.h"

// Global dword read by this accessor.
CSF_GLOBAL(int32_t, g_Value_8B0674, 0x8B0674);

// Returns the current value of the global at 0x8B0674.
static int __cdecl GetGlobalValue()
{
    return g_Value_8B0674;
}

HOOK(0x521BB0, GetGlobalValue, "auto");
