#include "hook.h"

// Returns the static text identifier used for the "shooting" animation state.
static const char* __cdecl C_0047b110_GetDisparandoText()
{
    return "TDisparando";
}

HOOK(0x47B110, C_0047b110_GetDisparandoText, "auto");
