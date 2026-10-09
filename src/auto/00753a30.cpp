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

// LTCG callers (0x401550, 0x4355D0) keep a value in ecx across the call: the original never writes ecx.
HOOK_KEEP_ECX(0x00753A30, GetGlobalValue, 0, "auto");
