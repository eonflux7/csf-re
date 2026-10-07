#include <cstdint>
#include "globals.h"
#include "hook.h"

// Global dword read by the accessor; its meaning is unknown from this function alone.
CSF_GLOBAL(int, g_008bb400, 0x8BB400);

// Returns the value of the module-level variable at 0x008BB400.
static int __cdecl GetGlobalValue()
{
    return g_008bb400;
}

HOOK(0x69FA10, GetGlobalValue, "auto");
