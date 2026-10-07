#include <cstdint>
#include "globals.h"
#include "hook.h"

// Global dword read by this getter (0x008B0684).
CSF_GLOBAL(int, g_GlobalValue, 0x8B0684);

// Returns the current value of the global dword at 0x008B0684.
static int __cdecl GetGlobalValue()
{
    return g_GlobalValue;
}

HOOK(0x54ECE0, GetGlobalValue, "auto");
