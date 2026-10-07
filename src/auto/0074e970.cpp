#include <cstdint>
#include "hook.h"

// Reads a byte field at offset 0x10 of the second argument and returns it.
// The original loads the raw pointer into eax and only overwrites al, so the
// return value is a genuine byte (char) result.
static char __stdcall GetByteAtOffset16(int /*a1*/, int a2, int /*a3*/, int /*a4*/)
{
    return *reinterpret_cast<const char*>(static_cast<uintptr_t>(a2) + 0x10);
}

HOOK(0x74E970, GetByteAtOffset16, "auto");
