#include <cstdint>
#include "globals.h"
#include "hook.h"

// Returns the literal name of the "combat, moving to A" state.
// Original: mov eax, 0x83CFC0 ; ret  (__cdecl, pops 0)
static const char* __cdecl CombatStateName_YendoA()
{
    return CSF_STR(0x83CFC0, "TCombateYendoA");
}

HOOK(0x4D6E40, CombatStateName_YendoA, "auto");
