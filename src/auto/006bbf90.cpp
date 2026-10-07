#include <cstdint>
#include "hook.h"

// Returns the constant value 404 (0x194). No arguments, no side effects.
static int __cdecl C_008605d8_GetValue()
{
    return 404;
}

HOOK(0x6BBF90, C_008605d8_GetValue, "auto");
