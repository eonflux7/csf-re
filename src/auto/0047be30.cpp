#include <cstdint>
#include "globals.h"
#include "hook.h"

// Returns the identifier string "TDisparar" ("Shoot"), which lives in the game
// image at 0x00832D58 (the original simply loads that address into eax).
static const char* __cdecl C_00832cc0_GetTDispararString()
{
    return "TDisparar";
}

HOOK(0x0047BE30, C_00832cc0_GetTDispararString, "auto");
