#include <cstdint>
#include "hook.h"

// Returns the constant type/kind identifier 0x21 (33).
static int __cdecl C_0083a948_GetTypeId()
{
    return 33;
}

HOOK(0x4B9540, C_0083a948_GetTypeId, "auto");
