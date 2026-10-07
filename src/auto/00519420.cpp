#include <cstdint>
#include "globals.h"
#include "hook.h"

// Global dword read by this getter (ida: dword_8B066C)
CSF_GLOBAL(int, g_Value_8B066C, 0x8B066C);

// 0x00519420: returns the current value of the global at 0x008B066C.
static int __cdecl GetGlobalValue()
{
    return g_Value_8B066C;
}

HOOK(0x00519420, GetGlobalValue, "auto");
