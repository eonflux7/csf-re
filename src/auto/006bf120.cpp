#include <cstdint>
#include "hook.h"

// Returns the fixed object size (0x20 = 32 bytes) for class C_00863180.
static int __cdecl C_00863180_GetSize()
{
    return 32;
}

HOOK(0x6BF120, C_00863180_GetSize, "auto");
