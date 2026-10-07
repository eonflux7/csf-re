#include <cstdint>
#include <cstddef>
#include <cstring>
#include <cmath>
#include <climits>
#include <cfloat>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"
#include "legacy/string.h"

// Returns the fixed type-name string of C_008393f0 ("TMovilAInteractivo").
// Original: mov eax, 0x839474 ; ret
static const char* __cdecl C_008393f0_GetTypeName()
{
    return CSF_STR(0x00839474, "TMovilAInteractivo");
}

HOOK(0x004AE330, C_008393f0_GetTypeName, "auto");
