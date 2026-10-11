#include <cmath>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"

CSF_GLOBAL(float, g_fZero, 0x0082D4CC);
CSF_GLOBAL(float, g_TransitionOne, 0x0082D4D4);

static int __fastcall GfxMatrix_IntersectRaySphere(
    float* self, void* /*edx*/, float* sphere,
    float* nearDistance, float* farDistance)
{
    const float dx = CSF_AT(float, self, 0x0C) - CSF_AT(float, sphere, 0x00);
    const float dy = CSF_AT(float, self, 0x10) - CSF_AT(float, sphere, 0x04);
    const float dz = CSF_AT(float, self, 0x14) - CSF_AT(float, sphere, 0x08);

    const float directionSquared =
        CSF_AT(float, self, 0x00) * CSF_AT(float, self, 0x00) +
        CSF_AT(float, self, 0x04) * CSF_AT(float, self, 0x04) +
        CSF_AT(float, self, 0x08) * CSF_AT(float, self, 0x08);

    const float projection =
        dx * CSF_AT(float, self, 0x00) +
        dy * CSF_AT(float, self, 0x04) +
        dz * CSF_AT(float, self, 0x08);

    const float discriminant =
        projection * projection -
        (dz * dz + dy * dy + dx * dx -
         CSF_AT(float, sphere, 0x0C) * CSF_AT(float, sphere, 0x0C)) *
        directionSquared;

    if (discriminant < g_fZero)
        return 0;

    if (discriminant > g_fZero)
    {
        const float root = std::sqrt(discriminant);
        const float inverseDirectionSquared = g_TransitionOne / directionSquared;

        CSF_AT(float, nearDistance, 0x00) =
            (-projection - root) * inverseDirectionSquared;
        const float farRoot = inverseDirectionSquared * (root - projection);
        CSF_AT(float, farDistance, 0x00) = farRoot;

        if (CSF_AT(float, nearDistance, 0x00) >= g_fZero)
            return 2;

        if (farRoot >= g_fZero)
        {
            CSF_AT(float, nearDistance, 0x00) = farRoot;
            return 1;
        }
        return 0;
    }

    const float tangentDistance = -(projection / directionSquared);
    CSF_AT(float, nearDistance, 0x00) = tangentDistance;
    return tangentDistance >= g_fZero;
}

HOOK(0x006ACDF0, GfxMatrix_IntersectRaySphere, "auto");
