#include <cstdint>
#include "hook.h"
#include "types/auto_classes.h"

// Returns the constant 30. No arguments, no side effects.
static int __fastcall C_0083a788_GetThirty(C_0083a788* self, void* /*edx*/)
{
    (void)self;
    return 30;
}

HOOK(0x4B2A00, C_0083a788_GetThirty, "auto");
