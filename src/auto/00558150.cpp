#include <cstdint>
#include "globals.h"
#include "hook.h"

// Global dword read and returned by this getter.
CSF_GLOBAL(int, g_Value_8B08F4, 0x8B08F4);

// 0x00558150 (__cdecl, ret 0): returns the value of the global at 0x8B08F4.
static int __cdecl GetGlobalValue()
{
    return g_Value_8B08F4;
}

HOOK(0x558150, GetGlobalValue, "auto");
