#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

// Returns the name of the "moving to point while taking cover" state/action.
// Original: mov eax, 0x83973C ; ret
static const char* __cdecl C_008396b8_GetTypeName()
{
    return CSF_STR(0x0083973C, "TMovilAPuntoParapetandose");
}

HOOK(0x004AE2B0, C_008396b8_GetTypeName, "auto");
