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

// Returns the identifier string "TAlerta" (likely the type-alert class name).
static const char* C_0083a368_GetName()
{
    return CSF_STR(0x0083A3EC, "TAlerta");
}

HOOK(0x4B2680, C_0083a368_GetName, "auto");
