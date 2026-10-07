#include <cstdint>
#include "globals.h"
#include "hook.h"

// A single 32-bit global read and returned unchanged.
CSF_GLOBAL(int, g_Value_8BAF98, 0x8BAF98);

// Returns the current value of the global at 0x8BAF98.
static int __cdecl GetValue_8BAF98()
{
    return g_Value_8BAF98;
}

HOOK(0x69C370, GetValue_8BAF98, "auto");
