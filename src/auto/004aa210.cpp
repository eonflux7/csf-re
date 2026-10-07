#include <cstdint>
#include "globals.h"
#include "hook.h"

// Returns the pointer to the constant name string "TAgacharse"
// (used as an animation / action name by the game).
static const char* __cdecl GetTAgacharseName()
{
    return CSF_STR(0x83834C, "TAgacharse");
}

HOOK(0x004AA210, GetTAgacharseName, "auto");
