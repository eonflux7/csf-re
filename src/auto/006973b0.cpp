#include <cstdint>

#include "globals.h"
#include "hook.h"

// Global dword written by the original function at 0x008BB3F8.
CSF_GLOBAL(int, g_Value_8BB3F8, 0x008BB3F8);

// 0x006973B0: stores its argument into dword_8BB3F8 and returns it unchanged.
static int __cdecl SetGlobalValue8BB3F8(int value)
{
    g_Value_8BB3F8 = value;
    return value;
}

HOOK(0x6973B0, SetGlobalValue8BB3F8, "auto");
