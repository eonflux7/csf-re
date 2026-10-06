#include <cstdint>
#include "hook.h"
#include "globals.h"
#include "types/auto_classes.h"

// Returns the constant Spanish string "truco" stored in the game's read-only data.
static const char* __cdecl GetTrucoString()
{
    return CSF_STR(0x00830084, "truco");
}

HOOK(0x00428B10, GetTrucoString, "auto");
