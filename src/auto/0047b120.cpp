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

// Returns the localized name string "TRecargando" (reloading).
static const char* __cdecl GetRecargandoString()
{
    return "TRecargando";
}

HOOK(0x47B120, GetRecargandoString, "auto");
