#include <cstdint>
#include "globals.h"
#include "hook.h"

// Returns the constant action/label string "TApuntar" (Spanish for "to aim").
// Original: mov eax, 0x8326A0 ; ret   (pointer to the string in the game image)
static const char* __cdecl GetTApuntarName()
{
    return "TApuntar";
}

HOOK(0x47B100, GetTApuntarName, "auto");
