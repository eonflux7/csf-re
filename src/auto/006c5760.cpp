#include <cstdint>
#include "globals.h"
#include "hook.h"

// 32-bit value read by the original at 0x008BB4B8 (mov eax, [0x8bb4b8]).
CSF_GLOBAL(int, g_Value_8BB4B8, 0x8BB4B8);

// Returns the global 32-bit value stored at 0x008BB4B8.
static int __cdecl GetGlobalValue()
{
    return g_Value_8BB4B8;
}

HOOK(0x6C5760, GetGlobalValue, "auto");
