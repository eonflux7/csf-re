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

// Returns the constant string "TEspera" (0x0083D380).
static const char* __cdecl GetEsperaString()
{
    return CSF_STR(0x0083D380, "TEspera");
}

HOOK(0x4D6E80, GetEsperaString, "auto");
