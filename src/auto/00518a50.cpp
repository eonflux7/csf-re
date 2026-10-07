#include <cstdint>
#include "globals.h"
#include "hook.h"

// Global returned by the target function.
CSF_GLOBAL(int, g_Value, 0x008B0580);

// Returns the global value stored at 0x008B0580.
static int __cdecl GetGlobalValue()
{
    return g_Value;
}

HOOK(0x518A50, GetGlobalValue, "auto");
