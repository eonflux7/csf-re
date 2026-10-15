#include <cstdint>
#include "globals.h"
#include "hook.h"
static float* __fastcall SharedHelper(const float* self, void*, float* result, const float* other) {
    const double x = static_cast<double>(self[0]) - other[0];
    const double y = static_cast<double>(self[1]) - other[1];
    const double z = static_cast<double>(self[2]) - other[2];
    result[0] = static_cast<float>(x); result[1] = static_cast<float>(y); result[2] = static_cast<float>(z); return result;
}
HOOK(0x0068E320, SharedHelper, "stl");
