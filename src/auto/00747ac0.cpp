#include <cstdint>
#include "globals.h"
#include "hook.h"

// Reads a single byte flag stored at +0x28 of the object passed as the
// second argument. Only AL is written by the original, so the return type
// is a plain byte (char), not a zero-extended 32-bit value.
static char __stdcall GetByteField0x28(int /*a1*/, int a2, int /*a3*/, int /*a4*/)
{
    return CSF_AT(char, reinterpret_cast<void*>(a2), 0x28);
}

HOOK(0x00747AC0, GetByteField0x28, "auto");
