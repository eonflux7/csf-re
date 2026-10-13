#include <cmath>
#include "globals.h"
#include "hook.h"

CSF_GLOBAL(float, g_HalfDurationScale, 0x0082D444);
CSF_GLOBAL(float, g_fZero, 0x0082D4CC);

static float* __fastcall GfxPtankCreate_AccumulateSphere(
    float* self, void* /*edx*/, float* sphere)
{
    const float dx = CSF_AT(float, sphere, 0x00) - CSF_AT(float, self, 0x00);
    const float dy = CSF_AT(float, sphere, 0x04) - CSF_AT(float, self, 0x04);
    const float dz = CSF_AT(float, sphere, 0x08) - CSF_AT(float, self, 0x08);
    const float radiusDifference =
        CSF_AT(float, sphere, 0x0C) - CSF_AT(float, self, 0x0C);
    const float distanceSquared = dz * dz + dy * dy + dx * dx;

    // The original comparison also takes this branch for unordered operands.
    if (!(radiusDifference * radiusDifference >= distanceSquared))
    {
        const float distance = std::sqrt(distanceSquared);
        const float centerX = CSF_AT(float, self, 0x00);
        const float centerY = CSF_AT(float, self, 0x04);
        const float centerZ = CSF_AT(float, self, 0x08);
        const float fraction =
            (distance + CSF_AT(float, sphere, 0x0C) -
             CSF_AT(float, self, 0x0C)) / (distance + distance);

        const float shiftY = dy * fraction;
        const float shiftZ = dz * fraction;
        CSF_AT(float, self, 0x00) = centerX + dx * fraction;
        CSF_AT(float, self, 0x04) = centerY + shiftY;

        const float radius = CSF_AT(float, self, 0x0C);
        CSF_AT(float, self, 0x08) = centerZ + shiftZ;
        CSF_AT(float, self, 0x0C) =
            (radius + CSF_AT(float, sphere, 0x0C) + distance) *
            g_HalfDurationScale;
    }
    else if (radiusDifference >= g_fZero)
    {
        CSF_AT(float, self, 0x00) = CSF_AT(float, sphere, 0x00);
        CSF_AT(float, self, 0x04) = CSF_AT(float, sphere, 0x04);
        CSF_AT(float, self, 0x08) = CSF_AT(float, sphere, 0x08);
        CSF_AT(float, self, 0x0C) = CSF_AT(float, sphere, 0x0C);
    }

    return self;
}

HOOK(0x006EE8C0, GfxPtankCreate_AccumulateSphere, "auto");
