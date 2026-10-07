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

// 0x00720A60 - trivial constant getter, returns 10.
static int __cdecl GetConstantTen()
{
    return 10;
}

HOOK(0x720A60, GetConstantTen, "auto");
