#include <cstdint>

#include "globals.h"
#include "hook.h"

// Global dword at 0x008BC1DC, read verbatim (and returned as-is) by 0x00753A30.
CSF_GLOBAL(int, g_GlobalValue, 0x008BC1DC);

// 0x00753A30: trivial accessor that returns the global int stored at 0x008BC1DC.
static int __cdecl GetGlobalValue()
{
    return g_GlobalValue;
}

HOOK(0x00753A30, GetGlobalValue, "auto");
