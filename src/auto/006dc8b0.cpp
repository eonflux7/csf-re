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

// Returns the constant value 0x98 (152).
static int __cdecl GetSizeConstant()
{
    return 152;
}

HOOK(0x006DC8B0, GetSizeConstant, "auto");
