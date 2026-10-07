#include <cstdint>
#include "hook.h"

// Returns a constant type/id tag (19) for this class.
static int __cdecl C_0082dd78_GetTypeTag()
{
    return 19;
}

HOOK(0x004D7900, C_0082dd78_GetTypeTag, "auto");
