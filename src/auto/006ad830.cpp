#include <cstdint>
#include <cstddef>
#include <cstring>
#include <cmath>
#include <climits>
#include <cfloat>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

// Projects point a3 onto the plane whose normal is the vector at +0x0C/+0x10/+0x14.
// When the dirty flag at +0x28 is set, the plane offset is recomputed from the
// vector at +0x00/+0x04/+0x08 and the normal is cached at +0x18/+0x1C/+0x20.
// Writes the projected point to a2 and returns a2.
static float* __fastcall S_00526d40_ProjectPointToPlane(S_00526d40* self, void* /*edx*/, float* a2, float* a3)
{
    if (CSF_AT(uint8_t, self, 0x28))
    {
        float normalX = CSF_AT(float, self, 0x0C);
        CSF_AT(uint8_t, self, 0x28) = 0;
        CSF_AT(float, self, 0x18) = normalX;

        float normalY = CSF_AT(float, self, 0x10);
        CSF_AT(float, self, 0x1C) = normalY;

        float normalZ = CSF_AT(float, self, 0x14);
        CSF_AT(float, self, 0x20) = normalZ;

        float baseX = *reinterpret_cast<float*>(&self->field_0000);
        float baseY = CSF_AT(float, self, 0x04);
        float baseZ = CSF_AT(float, self, 0x08);
        CSF_AT(float, self, 0x24) = -(normalZ * baseZ + normalY * baseY + normalX * baseX);
    }

    float planeOffset = CSF_AT(float, self, 0x24);
    float normalX = CSF_AT(float, self, 0x0C);
    float normalY = CSF_AT(float, self, 0x10);
    float normalZ = CSF_AT(float, self, 0x14);

    float signedDistance = normalY * a3[1] + normalX * a3[0] + normalZ * a3[2] + planeOffset;

    float offsetX = signedDistance * normalX;
    float offsetY = signedDistance * normalY;
    float offsetZ = a3[2] - signedDistance * normalZ;
    float resultY = a3[1] - offsetY;

    a2[0] = a3[0] - offsetX;
    a2[1] = resultY;
    a2[2] = offsetZ;
    return a2;
}

HOOK(0x006AD830, S_00526d40_ProjectPointToPlane, "auto");
