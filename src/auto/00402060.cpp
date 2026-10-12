#include <cstdint>
#include <cstddef>
#include "hook.h"

// Cross product: out = self x other. Both inputs are read before the
// result is written, so out may alias self or other.
static float* __fastcall Vec3_CrossProductInto(float* self, void* /*edx*/, float* out, float* other) {
    float x = self[1] * other[2] - self[2] * other[1];
    float y = self[2] * other[0] - self[0] * other[2];
    float z = other[1] * self[0] - self[1] * other[0];

    out[0] = x;
    out[1] = y;
    out[2] = z;
    return out;
}

HOOK(0x402060, Vec3_CrossProductInto, "auto");
