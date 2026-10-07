#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

// Global dword returned by this getter.
CSF_GLOBAL(int32_t, g_Value_8BC1E4, 0x8BC1E4);

// 0x0075FDF0 (6 bytes) -- __cdecl, no arguments.
// Simply loads and returns the global dword at 0x008BC1E4.
static int __cdecl GetGlobalValue()
{
    return g_Value_8BC1E4;
}

HOOK(0x75FDF0, GetGlobalValue, "auto");
