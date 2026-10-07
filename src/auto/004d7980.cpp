#include <cstdint>
#include "hook.h"

// Returns the constant 17 (0x11). No arguments, no side effects.
static int __cdecl GetSeventeen()
{
    return 17;
}

HOOK(0x4D7980, GetSeventeen, "auto");
