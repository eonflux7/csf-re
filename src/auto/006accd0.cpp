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

    // The original keeps every sum, product and quotient here on the x87 stack (extended precision) and rounds
    // only where it stores a float. Products of tiny components underflow to zero in float but not there, which
    // changes the sign of a zero result or whether a test is taken, so the arithmetic is done in double and
    // rounded to float only at the original's stores.
    const double dot =
        static_cast<double>(normal[1]) * CSF_AT(float, self, 0x04) +
        static_cast<double>(normal[0]) * *reinterpret_cast<const float*>(&self->field_0000) +
        static_cast<double>(normal[2]) * CSF_AT(float, self, 0x08);
    if (g_fZero == dot)
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

        CSF_AT(float, plane, 0x24) = static_cast<float>(
            -(static_cast<double>(nz) * CSF_AT(float, plane, 0x08) +
              static_cast<double>(ny) * CSF_AT(float, plane, 0x04) +
              static_cast<double>(nx) * CSF_AT(float, plane, 0x00)));
    }

    const double numerator =
        -static_cast<double>(CSF_AT(float, plane, 0x24)) -
        (static_cast<double>(normal[1]) * CSF_AT(float, self, 0x10) +
         static_cast<double>(normal[0]) * CSF_AT(float, self, 0x0C) +
         static_cast<double>(normal[2]) * CSF_AT(float, self, 0x14));
    const double denominator =
        static_cast<double>(normal[1]) * CSF_AT(float, self, 0x04) +
        static_cast<double>(normal[0]) * *reinterpret_cast<const float*>(&self->field_0000) +
        static_cast<double>(normal[2]) * CSF_AT(float, self, 0x08);
    const double distance = numerator / denominator;

    // offsetX stays on the stack until the final sum; offsetY and offsetZ are stored as float first.
    const double offsetX = distance * *reinterpret_cast<const float*>(&self->field_0000);
    const float offsetY = static_cast<float>(distance * CSF_AT(float, self, 0x04));
    const float offsetZ = static_cast<float>(distance * CSF_AT(float, self, 0x08));

    const float x = static_cast<float>(offsetX + CSF_AT(float, self, 0x0C));
    const float y = static_cast<float>(static_cast<double>(offsetY) + CSF_AT(float, self, 0x10));
    const float z = static_cast<float>(static_cast<double>(offsetZ) + CSF_AT(float, self, 0x14));

    intersection[0] = x;
    intersection[1] = y;
    intersection[2] = z;
    return 1;
}

HOOK(0x006ACCD0, S_00526d40_IntersectPlane, "auto");
