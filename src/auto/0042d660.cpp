#include <cstdint>
#include <cstddef>
#include <cstring>
#include <cmath>
#include <climits>
#include <cfloat>
#include "globals.h"
#include "hook.h"

// Returns non-zero when |a2 - a1| <= tolerance. Returns 0 for NaN inputs.
static int __cdecl AreFloatsWithinTolerance(float a1, float a2, float tolerance) {
    return std::fabs(a2 - a1) <= tolerance;
}

HOOK(0x0042D660, AreFloatsWithinTolerance, "auto");
