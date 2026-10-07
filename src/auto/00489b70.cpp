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

// 0x00489B70 - __stdcall, pops 4 bytes (one stack argument, unused).
// Always returns 1.
static int __stdcall C_00833708_ReturnOne(int /*a1*/)
{
    return 1;
}

HOOK(0x00489B70, C_00833708_ReturnOne, "auto");
