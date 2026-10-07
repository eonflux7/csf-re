#include <cstdint>
#include "globals.h"
#include "hook.h"

// Returns the flag byte stored at offset +0x0C of the object referenced by the
// pointer at this+0x10, or 0 when that pointer is null.
static int __fastcall GetInnerByteFlag(void* self, void* /*edx*/)
{
    void* inner = CSF_AT(void*, self, 0x10);
    if (!inner)
        return 0;
    return CSF_AT(uint8_t, inner, 0x0C);
}

HOOK(0x6B81E0, GetInnerByteFlag, "auto");
