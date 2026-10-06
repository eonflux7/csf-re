#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"
#include "legacy/string.h"

// Returns the name of the "TInitImpacto" class/type.
static const char* __cdecl GetInitImpactoName()
{
    return CSF_STR(0x0082F0C8, "TInitImpacto");
}

HOOK(0x00420710, GetInitImpactoName, "auto");
