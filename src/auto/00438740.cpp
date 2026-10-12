#include <cstdint>
#include <cstddef>
#include <cmath>
#include "globals.h"
#include "hook.h"

// Comparison tolerance stored in the game's data segment (float).
CSF_GLOBAL(float, g_RangeEpsilon, 0x830CAC);

// Returns 1 when `value` lies inside the wrapped range [rangeStart, rangeEnd]
// (going upward from rangeStart to rangeEnd when rangeStart <= rangeEnd, or
// the complementary arc when rangeStart > rangeEnd). Values within epsilon of
// either endpoint count as inside.
static char __cdecl IsValueInWrappedRange(float rangeStart, float rangeEnd, float value)
{
    if (std::fabs(rangeStart - value) <= g_RangeEpsilon ||
        std::fabs(rangeEnd - value) <= g_RangeEpsilon)
    {
        return 1;
    }

    if (rangeStart <= rangeEnd)
    {
        if (rangeStart > value)
            return 0;
        return !(value > rangeEnd) ? 1 : 0;
    }

    return (value < rangeEnd || value > rangeStart) ? 1 : 0;
}

HOOK(0x00438740, IsValueInWrappedRange, "auto");
