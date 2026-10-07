#include <cstdint>
#include "globals.h"
#include "hook.h"

// Returns the constant string "TRestaurar" (localized label used by the UI).
static const char* __cdecl GetRestaurarString()
{
    return CSF_STR(0x83D140, "TRestaurar");
}

HOOK(0x4D6E50, GetRestaurarString, "auto");
