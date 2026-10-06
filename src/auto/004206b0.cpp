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

// 0x004206B0 (6 bytes) — trivially returns 1. __cdecl, `ret` with no operands.
static int __cdecl Sub_4206B0()
{
    return 1;
}

HOOK(0x4206B0, Sub_4206B0, "auto");
