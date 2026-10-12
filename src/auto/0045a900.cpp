#include <cmath>

#include "hook.h"

// Returns nonzero when the 3D distance between `self` and `other` is within
// `maxDistance`. A negative bound never matches, because the distance is >= 0
// and the test is dist >= -maxDistance && dist <= maxDistance.
static int __fastcall HudStatusIcons_IsWithinDistance3D(float* self, void* /*edx*/, float* other, float maxDistance) {
    float dx = self[2] - other[2];
    float dy = self[0] - other[0];
    float dz = self[1] - other[1];
    float distance = std::sqrt(dy * dy + dz * dz + dx * dx);

    return distance >= -maxDistance && distance <= maxDistance;
}

HOOK(0x45A900, HudStatusIcons_IsWithinDistance3D, "auto");
