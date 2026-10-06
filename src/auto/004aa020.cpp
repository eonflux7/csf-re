#include <cstdint>
#include "globals.h"
#include "hook.h"

// Returns the name of the TCover class.
static const char* __cdecl TCover_GetName()
{
    return CSF_STR(0x836C64, "TCover");
}

HOOK(0x4AA020, TCover_GetName, "auto");
