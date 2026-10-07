#include <cstdint>
#include "globals.h"
#include "hook.h"

// Global fallback context value, set to 1000000 (0xF4240).
CSF_GLOBAL(std::int32_t, g_FallbackContext, 0x008A669C);

// Stores the constant 1000000 into the fallback-context global.
static void __cdecl SetFallbackContext()
{
    g_FallbackContext = 1000000;
}

HOOK(0x006912F0, SetFallbackContext, "auto");
