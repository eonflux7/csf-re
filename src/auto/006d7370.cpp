#include <cstdint>
#include "hook.h"

// Returns the size in bytes of a C_008611d4 instance (0x13C == 316).
static int __cdecl C_008611d4_GetSize()
{
    return 316;
}

HOOK(0x6D7370, C_008611d4_GetSize, "auto");
