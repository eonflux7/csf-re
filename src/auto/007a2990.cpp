#include <cstdint>
#include "globals.h"
#include "hook.h"
CSF_FUNC(double, __cdecl, Floor, 0x007AFD90, (double value));
// Shared scalar range wrap; both endpoints are retained when already in range.
static double __cdecl SharedHelper(float value, float low, float high) {
    if (value < low) {
        low = static_cast<float>(static_cast<double>(high) - low);
        return Floor((static_cast<double>(high) - value) / low) * low + value;
    }
    if (value > high) {
        const float width = static_cast<float>(static_cast<double>(high) - low);
        return value - Floor((static_cast<double>(value) - low) / width) * width;
    }
    return value;
}
HOOK(0x007A2990, SharedHelper, "stl");
