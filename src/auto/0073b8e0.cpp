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

// Zeroes the first three dwords of the block and sets the fourth to 1.0f (0x3F800000).
// Returns the block pointer. Stdcall, pops one argument (ret 4); ecx is never read.
static uint32_t* __stdcall InitVec4ZeroWithUnitW(uint32_t* block)
{
    block[0] = 0;
    block[1] = 0;
    block[2] = 0;
    block[3] = 0x3f800000u;
    return block;
}

HOOK(0x0073B8E0, InitVec4ZeroWithUnitW, "auto");
