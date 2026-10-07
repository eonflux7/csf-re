#include <cstdint>
#include "globals.h"
#include "hook.h"

// Global dword written by this setter (referenced at 0x008BB404).
CSF_GLOBAL(int, g_GlobalValue_8BB404, 0x008BB404);

// Stores the incoming value into global 0x8BB404 and returns it unchanged.
static int __stdcall SetGlobalValue_8BB404(int value)
{
    g_GlobalValue_8BB404 = value;
    return value;
}

HOOK(0x0069FBC0, SetGlobalValue_8BB404, "auto");
