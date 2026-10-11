#include "hook.h"

// Returns a2 only when *a1 < *a2 (strictly, x87 C0-only result).
// Otherwise (greater, equal, or unordered/NaN) returns a1.
static float* __cdecl SelectMovementRateBound(float* a1, float* a2) {
    if (*a1 < *a2) {
        return a2;
    }
    return a1;
}

HOOK(0x0040A640, SelectMovementRateBound, "auto");
