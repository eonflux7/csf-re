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

// Returns the constant 0x38 (56).
static int __cdecl GetConstant56()
{
    return 56;
}

HOOK(0x006EEAA0, GetConstant56, "auto");
