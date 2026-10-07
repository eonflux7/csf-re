#include <cstdint>
#include "hook.h"
#include "types/auto_classes.h"

// Returns the constant value 25 (0x19).
// __thiscall with no stack arguments (callee pops 0 bytes) -> modeled as __fastcall(self, edx).
static int __fastcall C_0083b850_GetLimit25(C_0083b850* self, void* /*edx*/)
{
    (void)self;
    return 25;
}

HOOK(0x4AE5E0, C_0083b850_GetLimit25, "auto");
