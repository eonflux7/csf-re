#include <cstdint>
#include "globals.h"
#include "hook.h"

// Returns the constant name string "TGirar" (the "turn" action identifier).
static const char* __cdecl GetTGirarName()
{
    return CSF_STR(0x0083895C, "TGirar");
}

HOOK(0x004AA280, GetTGirarName, "auto");
