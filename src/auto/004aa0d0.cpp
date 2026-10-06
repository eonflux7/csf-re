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

// Returns a pointer to the constant string "TRastrear" (0x83603C).
static const char* __cdecl GetTRastrearString()
{
    return CSF_STR(0x83603C, "TRastrear");
}

HOOK(0x4AA0D0, GetTRastrearString, "auto");
