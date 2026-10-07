#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "legacy/string.h"

// Returns the name string of the TCutScene class (pointer to the original
// constant data at 0x0083C9A8).
static const char* __cdecl C_TCutScene_GetName()
{
    return CSF_STR(0x0083C9A8, "TCutScene");
}

HOOK(0x004D6DE0, C_TCutScene_GetName, "auto");
