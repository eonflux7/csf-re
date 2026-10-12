#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"

// Plane distance helper: lazily rebuilds the cached plane (offsets 0x18..0x24)
// from the stored direction/offset when the dirty flag at 0x28 is set,
// then returns the signed distance of a point from that plane.
static double __fastcall PlaneDistanceToPoint(float* self, void* /*edx*/, float* point)
{
    if (CSF_AT(uint8_t, self, 0x28))
    {
        float offsetComponent = CSF_AT(float, self, 0xC);
        CSF_AT(uint8_t, self, 0x28) = 0;

        CSF_AT(float, self, 0x18) = offsetComponent;
        float normalY = CSF_AT(float, self, 0x10);
        CSF_AT(float, self, 0x1C) = normalY;
        float normalZ = CSF_AT(float, self, 0x14);
        CSF_AT(float, self, 0x20) = normalZ;

        CSF_AT(float, self, 0x24) = -(normalZ * self[2] + normalY * self[1] + offsetComponent * self[0]);
    }

    return CSF_AT(float, self, 0x20) * point[2]
         + CSF_AT(float, self, 0x1C) * point[1]
         + CSF_AT(float, self, 0x18) * point[0]
         + CSF_AT(float, self, 0x24);
}

HOOK_KEEP_ECX(0x4024D0, PlaneDistanceToPoint, 1, "auto");
