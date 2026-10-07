#include <cstdint>
#include "globals.h"
#include "hook.h"

// Global dword read by the target getter.
CSF_GLOBAL(int32_t, g_value_008B0670, 0x8B0670);

// int sub_51F740() -- returns the global dword at 0x8B0670.
static int32_t __cdecl GetGlobalValue()
{
    return g_value_008B0670;
}

HOOK(0x51F740, GetGlobalValue, "auto");
