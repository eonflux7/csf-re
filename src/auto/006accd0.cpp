#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_FUNC(float*, __fastcall, NormalizeVector3, 0x006AB8D0, (void* self, void* edx));
CSF_GLOBAL(float, g_fZero, 0x0082D4CC);

static int __fastcall S_00526d40_IntersectPlane(
    S_00526d40* self, void* /*edx*/, int planeAddress, float* intersection)
{
    void* plane = reinterpret_cast<void*>(planeAddress);
    float normal[3] = {
        CSF_AT(float, plane, 0x0C),
        CSF_AT(float, plane, 0x10),
        CSF_AT(float, plane, 0x14)
    };

    if (g_fZero ==
        normal[1] * CSF_AT(float, self, 0x04) +
        normal[0] * *reinterpret_cast<const float*>(&self->field_0000) +
        normal[2] * CSF_AT(float, self, 0x08))
    {
        return 0;
    }

    NormalizeVector3(normal, nullptr);

    if (CSF_AT(uint8_t, plane, 0x28))
    {
        const float nx = CSF_AT(float, plane, 0x0C);
        CSF_AT(uint8_t, plane, 0x28) = 0;
        CSF_AT(float, plane, 0x18) = nx;

        const float ny = CSF_AT(float, plane, 0x10);
        CSF_AT(float, plane, 0x1C) = ny;

        const float nz = CSF_AT(float, plane, 0x14);
        CSF_AT(float, plane, 0x20) = nz;

        CSF_AT(float, plane, 0x24) =
            -(nz * CSF_AT(float, plane, 0x08) +
              ny * CSF_AT(float, plane, 0x04) +
              nx * CSF_AT(float, plane, 0x00));
    }

    const float distance =
        (-CSF_AT(float, plane, 0x24) -
         (normal[1] * CSF_AT(float, self, 0x10) +
          normal[0] * CSF_AT(float, self, 0x0C) +
          normal[2] * CSF_AT(float, self, 0x14))) /
        (normal[1] * CSF_AT(float, self, 0x04) +
         normal[0] * *reinterpret_cast<const float*>(&self->field_0000) +
         normal[2] * CSF_AT(float, self, 0x08));

    const float offsetX =
        distance * *reinterpret_cast<const float*>(&self->field_0000);
    const float offsetY = distance * CSF_AT(float, self, 0x04);
    const float offsetZ = distance * CSF_AT(float, self, 0x08);

    const float x = offsetX + CSF_AT(float, self, 0x0C);
    const float y = offsetY + CSF_AT(float, self, 0x10);
    const float z = offsetZ + CSF_AT(float, self, 0x14);

    intersection[0] = x;
    intersection[1] = y;
    intersection[2] = z;
    return 1;
}

HOOK(0x006ACCD0, S_00526d40_IntersectPlane, "auto");
