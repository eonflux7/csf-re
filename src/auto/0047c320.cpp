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

// Returns the literal identifier string "TFunctionRecargar" (static data at 0x00832ED8).
static const char* __cdecl C_00832e40_GetTFunctionRecargar()
{
    return "TFunctionRecargar";
}

HOOK(0x47C320, C_00832e40_GetTFunctionRecargar, "auto");
