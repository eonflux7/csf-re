#include <cmath>
#include <cstdint>

#include "globals.h"
#include "hook.h"

// Epsilon constant used by all six comparisons (0.000001f in the original).
CSF_GLOBAL(float, g_ZeroEpsilon, 0x008603A0);

// Returns 1 when all six floats at this+0x00..this+0x14 have magnitude <= epsilon, else 0.
static int __fastcall IsSixFloatBlockNearZero(float* self, void* /*edx*/) {
    for (int i = 0; i < 6; ++i) {
        if (!(std::fabs(self[i]) <= g_ZeroEpsilon)) {
            return 0;
        }
    }
    return 1;
}

HOOK(0x006B8F40, IsSixFloatBlockNearZero, "auto");
