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

// Returns the type-name string constant "TOrden" (bound at 0x0083A6E4).
// Original: mov eax, 0x83A6E4 ; ret   -> no `this` is touched, no stack args (ret 0).
static const char* __cdecl C_0083a660_GetTypeName()
{
    return CSF_STR(0x0083A6E4, "TOrden");
}

HOOK(0x004B2950, C_0083a660_GetTypeName, "auto");
