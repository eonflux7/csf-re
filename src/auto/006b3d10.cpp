#include <cstdint>
#include "hook.h"
#include "types/auto_classes.h"

// Returns a constant value (2). Uses no `this` state and takes no arguments.
static int __fastcall C_00861b58_GetTwo(C_00861b58* /*self*/, void* /*edx*/)
{
    return 2;
}

HOOK(0x6B3D10, C_00861b58_GetTwo, "auto");
