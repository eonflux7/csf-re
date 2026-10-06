#include <cstdint>
#include "globals.h"
#include "hook.h"

// Global dword read by this accessor (purpose unknown; likely a cached setting/flag).
CSF_GLOBAL(int, g_Value8AFC84, 0x008AFC84);

// Returns the current value of the global at 0x008AFC84.
static int __cdecl GetGlobalValue()
{
    return g_Value8AFC84;
}

HOOK(0x0044ACE0, GetGlobalValue, "auto");
