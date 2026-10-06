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

// Returns the literal name of the "Girar" (turn) function.
static const char* __cdecl GetTFunctionGirarName()
{
    return "TFunctionGirar";
}

HOOK(0x47B0D0, GetTFunctionGirarName, "auto");
