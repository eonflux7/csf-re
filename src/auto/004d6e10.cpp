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

// Returns the literal string identifier "TAgachandose" (crouching state name).
static const char* __cdecl GetAgachandoseString()
{
    return CSF_STR(0x83CBF8, "TAgachandose");
}

HOOK(0x4D6E10, GetAgachandoseString, "auto");
