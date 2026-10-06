#include <cstdint>
#include "globals.h"
#include "hook.h"

// Returns the address of the constant string "TApuntar".
static const char* __cdecl GetTApuntarName()
{
    return CSF_STR(0x8326A0, "TApuntar");
}

HOOK(0x47B100, GetTApuntarName, "auto");
