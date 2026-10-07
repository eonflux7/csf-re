#include "globals.h"
#include "hook.h"

// Returns the constant string "NetGame".
// Original: mov eax, 0x8682B4 ; ret  (0x74C4F0, 6 bytes, ret 0)
static const char* __cdecl GetNetGameName()
{
    return CSF_STR(0x8682B4, "NetGame");
}

HOOK(0x74C4F0, GetNetGameName, "auto");
