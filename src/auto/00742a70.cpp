#include <cstdint>
#include "hook.h"

// Reads the byte at offset +1 of the record/buffer passed in the second argument.
// Note: only AL is written by the original (mov al, [eax+1]), so the return is a
// genuine single byte, not a zero-extended 32-bit value.
static char __stdcall GetByteAtOffset1(int /*unused1*/, int record, int /*unused3*/, int /*unused4*/)
{
    return *reinterpret_cast<const char*>(record + 1);
}

HOOK(0x742A70, GetByteAtOffset1, "auto");
