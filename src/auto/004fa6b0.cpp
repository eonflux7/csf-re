#include <cstdint>
#include "globals.h"
#include "hook.h"

// Global dword read (and returned) by this accessor.
CSF_GLOBAL(int, g_globalValue_8B0560, 0x8B0560);

// 0x004FA6B0 - trivial accessor: returns the global dword stored at 0x8B0560.
static int __cdecl GetGlobalValue_8B0560()
{
    return g_globalValue_8B0560;
}

HOOK(0x4FA6B0, GetGlobalValue_8B0560, "auto");
