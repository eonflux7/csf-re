#include <cstdint>
#include "hook.h"
#include "types/auto_classes.h"

// Returns the size (in bytes) of a C_00860bb8 instance: 248 (0xF8).
static int __cdecl C_00860bb8_GetSize()
{
    return 0xf8;
}

HOOK(0x006CA8D0, C_00860bb8_GetSize, "auto");
