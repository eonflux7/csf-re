#include "hook.h"

// Clamps *value into the range [*lowerBound, *upperBound].
// When the bounds are inverted (lower > upper), a different precedence applies:
// the upper bound is checked first, then the lower bound.
static double __cdecl ClampContactComponent(float* value, float* lowerBound, float* upperBound) {
    const float originalValue = *value;

    if (!(*lowerBound > *upperBound)) {
        // Bounds are ordered (lower <= upper).
        if (originalValue < *lowerBound) {
            return *lowerBound;
        }
        if (originalValue <= *upperBound) {
            return originalValue;
        }
        return *upperBound;
    }

    // Bounds are inverted (lower > upper).
    if (originalValue < *upperBound) {
        return *upperBound;
    }
    if (originalValue > *lowerBound) {
        return *lowerBound;
    }
    return originalValue;
}

HOOK(0x0040C920, ClampContactComponent, "auto");
