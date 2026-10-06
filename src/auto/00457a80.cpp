#include <cstdint>
#include "globals.h"
#include "hook.h"

// Global dword read by the target function at 0x008AFD9C.
CSF_GLOBAL(int32_t, g_GlobalValue, 0x8AFD9C);

// Returns the value of the global dword at 0x008AFD9C.
static int __cdecl GetGlobalValue()
{
    return g_GlobalValue;
}

HOOK(0x457A80, GetGlobalValue, "auto");
