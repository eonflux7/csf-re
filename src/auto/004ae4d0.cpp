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

// 0x004AE4D0: trivially returns the constant 13 (0xD).
// Hex-Rays prototype: int sub_4AE4D0()
static int __cdecl GetConstant13()
{
    return 13;
}

HOOK(0x004AE4D0, GetConstant13, "auto");
