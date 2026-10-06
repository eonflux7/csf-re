#include <cstdint>
#include "hook.h"

// Returns the constant 9. The original does not touch ecx or read any stack
// arguments, so it is modelled as a plain no-argument function.
static int __cdecl C_0083b118_GetTypeId()
{
    return 9;
}

HOOK(0x004A6F00, C_0083b118_GetTypeId, "auto");
