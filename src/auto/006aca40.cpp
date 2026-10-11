#include <cmath>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"

CSF_GLOBAL(float, g_fZero, 0x0082D4CC);

static bool __fastcall GfxMatrix_RayIntersectsSphere(
    float* self, void* /*edx*/, float* sphere)
{
    const float offsetX = CSF_AT(float, sphere, 0x00) - CSF_AT(float, self, 0x0C);
    const float offsetY = CSF_AT(float, sphere, 0x04) - CSF_AT(float, self, 0x10);
    const float offsetZ = CSF_AT(float, sphere, 0x08) - CSF_AT(float, self, 0x14);
    const float radius = CSF_AT(float, sphere, 0x0C);

    const float distanceSquared =
        offsetZ * offsetZ + offsetY * offsetY + offsetX * offsetX;

    // The original also enters this branch for an unordered comparison.
    if (!(radius * radius > distanceSquared))
    {
        const float projection =
            offsetY * CSF_AT(float, self, 0x04) +
            offsetX * CSF_AT(float, self, 0x00) +
            offsetZ * CSF_AT(float, self, 0x08);

        if (projection < g_fZero)
            return false;

        const float projectedX = projection * CSF_AT(float, self, 0x00);
        const float projectedY = projection * CSF_AT(float, self, 0x04);
        const float projectedZ = projection * CSF_AT(float, self, 0x08);

        const float perpendicularX = offsetX - projectedX;
        const float perpendicularY = offsetY - projectedY;
        const float perpendicularZ = offsetZ - projectedZ;

        if (std::sqrt(perpendicularZ * perpendicularZ +
                      perpendicularY * perpendicularY +
                      perpendicularX * perpendicularX) > radius)
            return false;
    }

    return true;
}

HOOK(0x006ACA40, GfxMatrix_RayIntersectsSphere, "auto");
