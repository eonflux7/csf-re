#include <cstdint>
#include "globals.h"
#include "hook.h"

// Returns the class identifier string for the NetPlayer class.
// Original: 00744600  mov eax, 0x867b68 ; ret
static const char* __cdecl NetPlayer_GetName()
{
    return CSF_STR(0x00867B68, "NetPlayer");
}

HOOK(0x00744600, NetPlayer_GetName, "auto");
