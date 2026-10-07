#include <cstdint>
#include "hook.h"

// Writes 1 into the byte pointed to by the fifth argument and returns 0.
// The first four arguments are ignored.
static int __cdecl SetFlagByteAndReturnZero(int /*a1*/, int /*a2*/, int /*a3*/, int /*a4*/, uint8_t* outFlag)
{
    *outFlag = 1;
    return 0;
}

HOOK(0x6EBA20, SetFlagByteAndReturnZero, "auto");
