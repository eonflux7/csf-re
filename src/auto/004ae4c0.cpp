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

// Returns the literal type/procedure name "TPrcDetrasDelEnemigo".
// Original: mov eax, 0x839B78 ; ret
static const char* __cdecl GetTPrcDetrasDelEnemigoName()
{
    return CSF_STR(0x00839B78, "TPrcDetrasDelEnemigo");
}

HOOK(0x4AE4C0, GetTPrcDetrasDelEnemigoName, "auto");
