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
    // The original keeps every intermediate on the x87 stack (80-bit extended):
    // only the fstp dword stores round to float. Hence the long double locals.
    const long double radiusDifference =
        static_cast<long double>(CSF_AT(float, sphere, 0x0C)) - CSF_AT(float, self, 0x0C);
    const float distanceSquared = static_cast<float>(
        static_cast<long double>(dz) * dz + static_cast<long double>(dy) * dy +
        static_cast<long double>(dx) * dx);

    // The original comparison also takes this branch for unordered operands.
    if (!(radiusDifference * radiusDifference >= distanceSquared))
    {
        const long double distance = std::sqrt(static_cast<long double>(distanceSquared));
        const long double centerX = CSF_AT(float, self, 0x00);
        const long double centerY = CSF_AT(float, self, 0x04);
        const long double centerZ = CSF_AT(float, self, 0x08);
        const long double fraction =
            (distance + CSF_AT(float, sphere, 0x0C) -
             CSF_AT(float, self, 0x0C)) / (distance + distance);

        const float shiftY = static_cast<float>(dy * fraction);
        const float shiftZ = static_cast<float>(dz * fraction);
        CSF_AT(float, self, 0x00) = static_cast<float>(centerX + dx * fraction);
        CSF_AT(float, self, 0x04) = static_cast<float>(centerY + shiftY);

        const long double radius = CSF_AT(float, self, 0x0C);
        CSF_AT(float, self, 0x08) = static_cast<float>(centerZ + shiftZ);
        CSF_AT(float, self, 0x0C) = static_cast<float>(
            (radius + CSF_AT(float, sphere, 0x0C) + distance) *
            g_HalfDurationScale);
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
