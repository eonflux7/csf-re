#include <cstdint>

#include "hook.h"

// Returns the fixed size (0x4C = 76 bytes) of the C_00861ce0 object type.
// Plain __cdecl function with no arguments; the original is `mov eax,0x4c; ret`.
static int __cdecl GetObjectSize()
{
    return 0x4c;
}

HOOK(0x6E54B0, GetObjectSize, "auto");
