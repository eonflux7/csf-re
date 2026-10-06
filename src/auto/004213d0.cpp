#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"

// Returns the name of the strafe animation: pointer to the constant "TStrafe".
static const char* __cdecl GetStrafeAnimName()
{
    return CSF_STR(0x0082F790, "TStrafe");
}

HOOK(0x004213D0, GetStrafeAnimName, "auto");
