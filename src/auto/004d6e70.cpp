#include <cstdint>
#include "globals.h"
#include "hook.h"

// Returns the constant name of the idle task/state (string at 0x83D2C0: "TIdle").
static const char* __cdecl GetIdleStateName()
{
    return CSF_STR(0x83D2C0, "TIdle");
}

HOOK(0x4D6E70, GetIdleStateName, "auto");
