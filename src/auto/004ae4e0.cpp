#include <cstdint>
#include "globals.h"
#include "hook.h"

// Returns the literal identifier of the "EvitarOclusores" (avoid occluders) process/class.
static const char* __cdecl GetEvitarOclusoresName()
{
    return CSF_STR(0x839C18, "TPrcEvitarOclusores");
}

HOOK(0x4AE4E0, GetEvitarOclusoresName, "auto");
