#include <cstdint>
#include "hook.h"
#include "globals.h"

// Returns the literal type/class-name string "TAction".
static const char* __cdecl TAction_GetName()
{
    return "TAction";
}

HOOK(0x4206E0, TAction_GetName, "auto");
