#include <cstdint>

#include "hook.h"

// Returns the constant 27 (0x1B).
static int __cdecl GetValue27()
{
    return 27;
}

HOOK(0x4B2960, GetValue27, "auto");
