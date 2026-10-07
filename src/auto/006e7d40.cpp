#include <cstdint>
#include "hook.h"

// Returns the size (60 bytes) of the C_00861dd8 object layout.
static int __cdecl C_00861dd8_GetSize()
{
    return 60;
}

HOOK(0x006E7D40, C_00861dd8_GetSize, "auto");
