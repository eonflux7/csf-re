#include <cstdint>
#include "hook.h"

// 0x006BC660 - writes 1 to the byte pointed to by the third argument and returns 0.
static int __cdecl SetFlagByteReturnZero(int /*a1*/, int /*a2*/, uint8_t* out_flag)
{
    *out_flag = 1;
    return 0;
}

HOOK(0x6BC660, SetFlagByteReturnZero, "auto");
