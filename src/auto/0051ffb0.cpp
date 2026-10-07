#include <cstdint>
#include "hook.h"

// Returns the incoming value unchanged, except that the value 4 is replaced by 0.
static int __fastcall MapIdReplacingFourWithZero(int value, void* /*edx*/)
{
    return value != 4 ? value : 0;
}

HOOK(0x51FFB0, MapIdReplacingFourWithZero, "auto");
