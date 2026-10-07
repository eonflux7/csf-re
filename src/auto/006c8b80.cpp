#include <cstdint>
#include "globals.h"
#include "hook.h"

// Plain 32-bit global at 0x008BB4CC.
CSF_GLOBAL(int32_t, g_Value_8BB4CC, 0x008BB4CC);

// Returns the current value of the global variable at 0x008BB4CC.
static int __cdecl GetGlobalValue_8BB4CC()
{
    return g_Value_8BB4CC;
}

HOOK(0x006C8B80, GetGlobalValue_8BB4CC, "auto");
