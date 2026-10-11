#include "hook.h"

// Clamps *value into the range [*lowerBound, *upperBound]. If the bounds are
// given inverted (lowerBound > upperBound), the comparisons are swapped so the
// result still lands on one of the two bounds, matching the original branches.
static void __cdecl ClampFloatToBounds(float* value, float* lowerBound, float* upperBound) {
    if (*lowerBound <= *upperBound) {
        if (*value < *lowerBound) {
            *value = *lowerBound;
        } else if (*value > *upperBound) {
            *value = *upperBound;
        }
    } else {
        if (*value < *upperBound) {
            *value = *upperBound;
        } else if (*value > *lowerBound) {
            *value = *lowerBound;
        }
    }
}

HOOK(0x406B60, ClampFloatToBounds, "auto");
