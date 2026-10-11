#include <cstdint>
#include <cstddef>
#include <cstring>
#include <cmath>
#include <climits>
#include <cfloat>
#include "globals.h"
#include "hook.h"

// Returns non-zero when any of the three components differs.
// NaN compares as different, matching the original's unordered-compare branch.
static int __fastcall GfxMatrix_AreVectorsDifferent(float* self, void* /*edx*/, float* other)
{
    if (self[0] != other[0]) {
        return 1;
    }
    if (self[1] != other[1]) {
        return 1;
    }
    if (self[2] != other[2]) {
        return 1;
    }
    return 0;
}

HOOK(0x0040FB20, GfxMatrix_AreVectorsDifferent, "auto");
