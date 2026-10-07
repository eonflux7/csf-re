#include <cstdint>
#include "hook.h"

// Returns the byte stored at offset 0x4C of the object passed as the second argument.
// Note: the original leaves the upper 24 bits of eax untouched (mov al, [eax+0x4c]),
// but the declared return type is a byte, so only al is meaningful to callers.
static char __stdcall GetObjectByte4C(int /*unused1*/, int object, int /*unused3*/, int /*unused4*/)
{
    return static_cast<char>(*reinterpret_cast<const uint8_t*>(static_cast<uintptr_t>(object) + 0x4C));
}

HOOK(0x0074EAB0, GetObjectByte4C, "auto");
