#include <cstdint>
#include <cstddef>
#include "hook.h"

// Scales the 3-float vector at `self` by `scale` and writes the result to `out`.
// Returns `out`.
static float* __fastcall IndexedTransform_ScaleVector(float* self, void* /*edx*/, float* out, float scale) {
    // All source components are read before any write, so out == self is safe.
    const float scaledX = scale * self[0];
    const float scaledY = scale * self[1];
    const float scaledZ = scale * self[2];

    out[0] = scaledX;
    out[1] = scaledY;
    out[2] = scaledZ;
    return out;
}

HOOK(0x00401030, IndexedTransform_ScaleVector, "auto");
