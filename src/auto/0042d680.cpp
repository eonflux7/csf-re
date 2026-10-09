#include "hook.h"

// Returns nonzero when all four float components of `other` equal those of `self`.
// Plain float == matches the original: NaN compares unequal, and comparisons short-circuit in order.
static int __fastcall Hud_ColorsMatchRgba(float* self, void* /*edx*/, float* other) {
    if (other[0] != self[0]) return 0;
    if (other[1] != self[1]) return 0;
    if (other[2] != self[2]) return 0;
    if (other[3] != self[3]) return 0;
    return 1;
}

HOOK(0x42D680, Hud_ColorsMatchRgba, "auto");
