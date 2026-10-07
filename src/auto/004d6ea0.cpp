#include <cstdint>
#include <cstddef>
#include <cstring>
#include "globals.h"
#include "hook.h"

// Returns the constant class-name string for the T_LANZAR (thrower) entity type.
static const char* __cdecl GetTLanzarName()
{
    return CSF_STR(0x838FEC, "TLanzar");
}

HOOK(0x4D6EA0, GetTLanzarName, "auto");
