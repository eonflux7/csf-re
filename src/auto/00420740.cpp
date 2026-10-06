#include <cstdint>
#include "globals.h"
#include "hook.h"

// The literal string "TEnd" lives at 0x0082F318 in the game's data section.
CSF_GLOBAL(const char, g_TEndString, 0x0082F318);

// Returns the static identifier string "TEnd".
static const char* __cdecl GetTEndString()
{
    return &g_TEndString;
}

HOOK(0x00420740, GetTEndString, "auto");
