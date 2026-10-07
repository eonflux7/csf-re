#include <cstdint>
#include "hook.h"

// Returns the constant value 0xA4 (164). No arguments, no `this`, pops 0 stack bytes.
static int __cdecl GetConstantValue_6D0CA0()
{
    return 0xA4;
}

HOOK(0x006D0CA0, GetConstantValue_6D0CA0, "auto");
