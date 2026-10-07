#include <cstdint>
#include "globals.h"
#include "hook.h"

// Returns the literal animation/action name "TAproximarse" (Spanish: "to approach").
// Original: mov eax, 0x83883C ; ret  (0x004AA270, 6 bytes)
static const char* __cdecl GetAproximarseName()
{
    return CSF_STR(0x83883C, "TAproximarse");
}

HOOK(0x4AA270, GetAproximarseName, "auto");
