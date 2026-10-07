#include <cstdint>
#include "globals.h"
#include "hook.h"

// Global value read by this accessor. Purpose unknown from the packet.
CSF_GLOBAL(int, g_Value_8BB450, 0x8BB450);

// Returns the value of the global at 0x008BB450.
static int __cdecl GetGlobalInt()
{
    return g_Value_8BB450;
}

HOOK(0x6B1380, GetGlobalInt, "auto");
