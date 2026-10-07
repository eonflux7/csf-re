#include <cstdint>
#include "globals.h"
#include "hook.h"

// Global variable returned by the getter.
CSF_GLOBAL(int32_t, g_Value_8B08F8, 0x8B08F8);

// Returns the current value of the global at 0x8B08F8.
static int __cdecl GetValue_8B08F8()
{
    return g_Value_8B08F8;
}

HOOK(0x55A9C0, GetValue_8B08F8, "auto");
