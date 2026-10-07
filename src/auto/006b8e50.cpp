#include <cstdint>
#include <cstddef>
#include "hook.h"
#include "types/auto_classes.h"

// Returns the constant 8. No arguments, no stack cleanup (ret 0).
// Class C_00861b40 (vtable 0x00861B40) — likely a virtual "size/count" accessor.
static int __fastcall C_00861b40_GetSize(C_00861b40* /*self*/, void* /*edx*/)
{
    return 8;
}

HOOK(0x6B8E50, C_00861b40_GetSize, "auto");
