#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"

// Global read by this accessor (referenced as a dword at 0x008B056C).
CSF_GLOBAL(int, g_value_8B056C, 0x008B056C);

// Returns the current value of the global at 0x008B056C.
static int __cdecl GetGlobalValue_8B056C()
{
    return g_value_8B056C;
}

HOOK(0x00506BD0, GetGlobalValue_8B056C, "auto");
