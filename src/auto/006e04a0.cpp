#include <cstdint>
#include "globals.h"
#include "hook.h"

// Global read by the target: plain 32-bit value at 0x008BB738.
CSF_GLOBAL(int, g_Value_8BB738, 0x8BB738);

// Returns the 32-bit global stored at 0x008BB738.
static int __cdecl GetValue_8BB738()
{
    return g_Value_8BB738;
}

HOOK(0x6E04A0, GetValue_8BB738, "auto");
