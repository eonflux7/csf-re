#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"

// Returns the name of the "Move" TFnc (function/task function) descriptor.
// 0x004B2AA0: mov eax, offset "TFncMove"; ret
static const char* __cdecl GetTFncMoveString()
{
    return CSF_STR(0x83A8A4, "TFncMove");
}

HOOK(0x4B2AA0, GetTFncMoveString, "auto");
