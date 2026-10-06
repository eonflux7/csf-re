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

// 0x0040EE70 - trivial accessor that always yields the constant 15.
// Not a __thiscall (no ecx use) and pops 0 stack bytes (cdecl/plain).
static int __cdecl GetConstant15()
{
    return 15;
}

HOOK(0x40EE70, GetConstant15, "auto");
