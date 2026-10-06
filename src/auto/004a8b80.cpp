#include <cstdint>
#include "hook.h"
#include "types/auto_classes.h"

// Returns the hard-coded constant 22 (0x16).
static int __fastcall C_00835060_GetConstant22(C_00835060* self, void* /*edx*/)
{
    (void)self; // unused
    return 22;
}

HOOK(0x4A8B80, C_00835060_GetConstant22, "auto");
