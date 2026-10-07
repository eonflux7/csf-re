#include <cstdint>
#include "hook.h"

// Returns the size in bytes of the C_00861e98 object (0xA0 = 160).
static int __cdecl C_00861e98_GetSize()
{
    return 160;
}

HOOK(0x6E84B0, C_00861e98_GetSize, "auto");
