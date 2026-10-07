#include "globals.h"
#include "hook.h"

// Returns the constant string "TRecargar" (Spanish for "TRecargar", likely a
// key used by the game's input/config system).
static const char* __cdecl GetTRecargarString()
{
    return CSF_STR(0x00831CF0, "TRecargar");
}

HOOK(0x004AA220, GetTRecargarString, "auto");
