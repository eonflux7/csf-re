#include <cstdint>

#include "globals.h"
#include "hook.h"
#include "core/memory.h"

CSF_GLOBAL(float, g_fZero, 0x0082D4CC);
CSF_GLOBAL(float, g_MissingMetricValue, 0x0082D4C4);

// Plane layout as used here: point at +0x00..+0x08, normal at +0x0C..+0x14,
// cached copy of the normal at +0x18..+0x20, cached offset d at +0x24, dirty flag at +0x28.
// Returns the ray parameter t where the ray (rayOrigin + t * rayDirection) hits the plane,
// or g_MissingMetricValue when the ray is parallel to the plane.
static double __fastcall GfxPlane_RayIntersectionT(float* self, void* /*edx*/, float* rayOrigin, float* rayDirection)
{
    if (CSF_AT(uint8_t, self, 0x28) != 0) {
        float normalX = CSF_AT(float, self, 0x0C);
        float normalY = CSF_AT(float, self, 0x10);
        float normalZ = CSF_AT(float, self, 0x14);
        CSF_AT(uint8_t, self, 0x28) = 0;
        CSF_AT(float, self, 0x18) = normalX;
        CSF_AT(float, self, 0x1C) = normalY;
        CSF_AT(float, self, 0x20) = normalZ;
        CSF_AT(float, self, 0x24) = -(normalZ * CSF_AT(float, self, 0x08)
                                    + normalY * CSF_AT(float, self, 0x04)
                                    + normalX * CSF_AT(float, self, 0x00));
    }

    float denominator = CSF_AT(float, self, 0x10) * rayDirection[1]
                      + rayDirection[0] * CSF_AT(float, self, 0x0C)
                      + CSF_AT(float, self, 0x14) * rayDirection[2];
    if (denominator == g_fZero) {
        return g_MissingMetricValue;
    }

    float numerator = CSF_AT(float, self, 0x10) * rayOrigin[1]
                    + CSF_AT(float, self, 0x0C) * rayOrigin[0]
                    + CSF_AT(float, self, 0x14) * rayOrigin[2]
                    + CSF_AT(float, self, 0x24);
    return -(numerator / denominator);
}

HOOK(0x006AD8C0, GfxPlane_RayIntersectionT, "auto");
