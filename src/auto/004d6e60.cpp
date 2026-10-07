#include <cstdint>
#include "hook.h"
#include "globals.h"

// Returns the name of the "TSaludo" (salute) type.
static const char* __cdecl GetTSaludoName()
{
    return CSF_STR(0x83D200, "TSaludo");
}

HOOK(0x4D6E60, GetTSaludoName, "auto");
