#include <cstdint>
#include "hook.h"

// Reads the single byte stored at offset +4 of the second argument
// (a2 is used purely as an address base; the other arguments are ignored).
static char __stdcall GetByteAtOffset4OfSecondArg(int /*a1*/, int a2, int /*a3*/, int /*a4*/)
{
    return *reinterpret_cast<const char*>(static_cast<uintptr_t>(static_cast<uint32_t>(a2)) + 4);
}

HOOK(0x00745670, GetByteAtOffset4OfSecondArg, "auto");
