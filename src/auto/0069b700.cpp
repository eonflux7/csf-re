#include <cstdint>
#include "globals.h"
#include "hook.h"

// Global counter/value returned by this getter.
CSF_GLOBAL(int32_t, g_Value_8baf94, 0x008BAF94);

// Returns the current value of the global at 0x8BAF94.
static int32_t __cdecl GetGlobalValue()
{
    return g_Value_8baf94;
}

HOOK(0x0069B700, GetGlobalValue, "auto");
