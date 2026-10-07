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

// 0x006A7180 - __stdcall stub that unconditionally returns -1.
static int __stdcall ReturnMinusOne(int /*a1*/, int /*a2*/)
{
    return -1;
}

HOOK(0x6A7180, ReturnMinusOne, "auto");
