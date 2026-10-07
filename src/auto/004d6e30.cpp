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

// Returns the name of the TAccesoObjeto type.
static const char* __cdecl GetAccesoObjetoTypeName()
{
    return CSF_STR(0x0083CEF8, "TAccesoObjeto");
}

HOOK(0x004D6E30, GetAccesoObjetoTypeName, "auto");
