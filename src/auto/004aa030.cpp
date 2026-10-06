#include "globals.h"
#include "hook.h"

// The original function simply returns the address of the "TRodear" string
// literal, which lives in the game's read-only data at 0x836D7C.
CSF_GLOBAL(char, g_TRodearString, 0x836d7c);

static const char* __cdecl GetRodearString()
{
    return &g_TRodearString;
}

HOOK(0x4AA030, GetRodearString, "auto");
