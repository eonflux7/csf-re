#include "hook.h"

// Returns the byte stored at offset +8 of the object passed as the second
// argument. The original loads the full 32-bit pointer into eax and only
// overwrites the low byte, so the declared return type is a byte/char.
static char __stdcall GetByteFieldAt8(int /*a1*/, int a2, int /*a3*/, int /*a4*/)
{
    return *reinterpret_cast<char*>(a2 + 8);
}

HOOK(0x0074B830, GetByteFieldAt8, "auto");
