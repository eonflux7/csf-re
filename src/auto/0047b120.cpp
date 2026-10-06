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

// 0x0047B120: returns the constant string "TRecargando" (0x00832470).
// Bound by address via CSF_STR so pointer comparisons elsewhere keep working.
static const char* __cdecl GetRecargandoString()
{
    return CSF_STR(0x00832470, "TRecargando");
}

HOOK(0x0047B120, GetRecargandoString, "auto");
