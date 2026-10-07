#include <cstdint>
#include "hook.h"

// Returns the byte stored at offset 0x2C of the second argument.
// Original: return *(BYTE*)(a2 + 44);  (genuine byte return: only AL is written)
static char __stdcall GetSecondArgByteAt2C(int /*a1*/, int a2, int /*a3*/, int /*a4*/)
{
    return *reinterpret_cast<char*>(a2 + 0x2c);
}

HOOK(0x0074EBA0, GetSecondArgByteAt2C, "auto");
