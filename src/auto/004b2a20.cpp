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

// 0x004B2A20 - empty __stdcall function; the original body is just `ret 4`.
static void __stdcall C_0085ec18_NullSub(int /*a1*/)
{
    // Intentionally empty (nullsub).
}

HOOK(0x4B2A20, C_0085ec18_NullSub, "auto");
