#include <cstdint>
#include "globals.h"
#include "hook.h"

// Returns the type name for the "desplazamiento" (offset/displacement) type descriptor.
static const char* __cdecl GetDesplazamientoName()
{
    return CSF_STR(0x837734, "TDesplazamiento");
}

HOOK(0x4AA0E0, GetDesplazamientoName, "auto");
