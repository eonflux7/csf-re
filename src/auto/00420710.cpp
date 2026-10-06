#include <cstdint>
#include <cstddef>
#include <cstring>

#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"
#include "legacy/string.h"

// Returns the name of the "init impact" animation/resource used by the game.
// Original: mov eax, 0x82f0c8 ; ret   (address of the literal "TInitImpacto").
static const char* __cdecl GetInitImpactoName()
{
    return "TInitImpacto";
}

HOOK(0x420710, GetInitImpactoName, "auto");
