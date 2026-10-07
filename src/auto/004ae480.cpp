#include <cstdint>
#include "hook.h"
#include "types/auto_classes.h"

// Returns a constant value 24 (0x18).
static int __fastcall C_00860c88_GetSize(C_00860c88* self, void* /*edx*/)
{
    (void)self;
    return 24;
}

HOOK(0x4AE480, C_00860c88_GetSize, "auto");
