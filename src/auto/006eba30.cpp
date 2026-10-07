#include <cstdint>
#include "hook.h"

// Writes 1 to the byte pointed to by the 4th argument and returns 0.
// The first three arguments are ignored; the callee is __cdecl (ret 0).
static int __cdecl SetOutByteAndReturnZero(int /*a1*/, int /*a2*/, int /*a3*/, uint8_t* out_flag)
{
    *out_flag = 1;
    return 0;
}

HOOK(0x006EBA30, SetOutByteAndReturnZero, "auto");
