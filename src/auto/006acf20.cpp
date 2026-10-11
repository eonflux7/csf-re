#include <cmath>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"

CSF_GLOBAL(float, g_fZero, 0x0082D4CC);
CSF_GLOBAL(float, g_TransitionOne, 0x0082D4D4);

static int __fastcall GfxMatrix_IntersectRaySphere(
    float* self, void* /*edx*/, float* sphere, float* firstHit, float* secondHit)
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

    if (!(discriminant >= g_fZero))
        return 0;

    if (discriminant <= g_fZero)
    {
        const float distance = -(projection / directionSquared);
        if (distance >= g_fZero)
        {
            const float scaledX = distance * CSF_AT(float, self, 0x00);
            const float scaledY = distance * CSF_AT(float, self, 0x04);
            const float scaledZ = distance * CSF_AT(float, self, 0x08);
            const float x = scaledX + CSF_AT(float, self, 0x0C);
            const float y = scaledY + CSF_AT(float, self, 0x10);
            const float z = scaledZ + CSF_AT(float, self, 0x14);

            CSF_AT(float, firstHit, 0x00) = x;
            CSF_AT(float, firstHit, 0x04) = y;
            CSF_AT(float, firstHit, 0x08) = z;
            return 1;
        }
        return 0;
    }

    const float root = std::sqrt(discriminant);
    const float inverseDirectionSquared = g_TransitionOne / directionSquared;
    const float nearDistance = (-projection - root) * inverseDirectionSquared;
    const float farDistance = (root - projection) * inverseDirectionSquared;

    if (nearDistance >= g_fZero)
    {
        const float nearScaledZ = nearDistance * CSF_AT(float, self, 0x08);
        const float nearX =
            nearDistance * CSF_AT(float, self, 0x00) + CSF_AT(float, self, 0x0C);
        const float nearY =
            nearDistance * CSF_AT(float, self, 0x04) + CSF_AT(float, self, 0x10);
        const float nearZ = nearScaledZ + CSF_AT(float, self, 0x14);

        CSF_AT(float, firstHit, 0x00) = nearX;
        CSF_AT(float, firstHit, 0x04) = nearY;
        CSF_AT(float, firstHit, 0x08) = nearZ;

        const float farScaledZ = farDistance * CSF_AT(float, self, 0x08);
        const float farX =
            farDistance * CSF_AT(float, self, 0x00) + CSF_AT(float, self, 0x0C);
        const float farY =
            farDistance * CSF_AT(float, self, 0x04) + CSF_AT(float, self, 0x10);
        const float farZ = farScaledZ + CSF_AT(float, self, 0x14);

        CSF_AT(float, secondHit, 0x00) = farX;
        CSF_AT(float, secondHit, 0x04) = farY;
        CSF_AT(float, secondHit, 0x08) = farZ;
        return 2;
    }

    if (farDistance >= g_fZero)
    {
        const float scaledZ = farDistance * CSF_AT(float, self, 0x08);
        const float scaledX = farDistance * CSF_AT(float, self, 0x00);
        const float scaledY = farDistance * CSF_AT(float, self, 0x04);
        const float x = scaledX + CSF_AT(float, self, 0x0C);
        const float y = scaledY + CSF_AT(float, self, 0x10);
        const float z = scaledZ + CSF_AT(float, self, 0x14);

        CSF_AT(float, firstHit, 0x00) = x;
        CSF_AT(float, firstHit, 0x04) = y;
        CSF_AT(float, firstHit, 0x08) = z;
        return 1;
    }

    return 0;
}

HOOK(0x006ACF20, GfxMatrix_IntersectRaySphere, "auto");
