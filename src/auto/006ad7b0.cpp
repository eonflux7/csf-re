#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

// Plane-side thresholds: +0.05f (0x82DFCC) and -0.05f (0x846170).
CSF_GLOBAL(float, g_PlaneSideUpperThreshold, 0x0082DFCC);
CSF_GLOBAL(float, g_PlaneSideLowerThreshold, 0x00846170);

// Returns 0 if the point is in front of the plane (distance > +0.05),
// 1 if behind it (distance < -0.05), 2 if it lies on the plane within tolerance.
static int __fastcall GfxMatrix_ClassifyPointAgainstPlane(float* self, void* /*edx*/, float* point)
{
    // Lazily refresh the cached plane: copy the base point and normal, then
    // recompute the plane offset term when the dirty flag is set.
    if (CSF_AT(uint8_t, self, 0x28) != 0)
    {
        float baseX = CSF_AT(float, self, 0x0C);
        CSF_AT(uint8_t, self, 0x28) = 0;
        CSF_AT(float, self, 0x18) = baseX;

        float baseY = CSF_AT(float, self, 0x10);
        CSF_AT(float, self, 0x1C) = baseY;

        float baseZ = CSF_AT(float, self, 0x14);
        CSF_AT(float, self, 0x20) = baseZ;

        CSF_AT(float, self, 0x24) =
            -(baseZ * CSF_AT(float, self, 0x08)
            + baseY * CSF_AT(float, self, 0x04)
            + baseX * CSF_AT(float, self, 0x00));
    }

    float signedDistance =
        CSF_AT(float, self, 0x10) * point[1]
        + CSF_AT(float, self, 0x0C) * point[0]
        + CSF_AT(float, self, 0x14) * point[2]
        + CSF_AT(float, self, 0x24);

    if (signedDistance > g_PlaneSideUpperThreshold)
        return 0;
    if (signedDistance < g_PlaneSideLowerThreshold)
        return 1;
    return 2;
}

HOOK(0x006AD7B0, GfxMatrix_ClassifyPointAgainstPlane, "auto");
