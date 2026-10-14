#include <cstdint>
#include <cmath>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"

CSF_FUNC(float*, __fastcall, Vec3_NormalizeHelper, 0x006AB830, (void* self, void* edx, float a2));
CSF_GLOBAL(float, g_fZero, 0x0082D4CC);

static float* __fastcall Geometry_ProjectPointOntoSegment(
    float* self, void* /*edx*/, float* output, float* point)
{
    const float deltaX = CSF_AT(float, point, 0x00) - CSF_AT(float, self, 0x00);
    const float deltaY = CSF_AT(float, point, 0x04) - CSF_AT(float, self, 0x04);
    const float deltaZ = CSF_AT(float, point, 0x08) - CSF_AT(float, self, 0x08);

    alignas(8) uint8_t directionBytes[0x0C];
    float& directionX = CSF_AT(float, directionBytes, 0x00);
    float& directionY = CSF_AT(float, directionBytes, 0x04);
    float& directionZ = CSF_AT(float, directionBytes, 0x08);

    directionX = CSF_AT(float, self, 0x0C) - CSF_AT(float, self, 0x00);
    directionY = CSF_AT(float, self, 0x10) - CSF_AT(float, self, 0x04);
    directionZ = CSF_AT(float, self, 0x14) - CSF_AT(float, self, 0x08);

    // The original squares and sums in the x87 register file (80-bit extended); only the
    // sqrt result is stored as float (fsqrt; fstp dword ptr [ebp-4]). Float squares would
    // underflow for tiny directions where the extended sum is still non-zero.
    const long double lengthSquared =
        static_cast<long double>(directionZ) * directionZ +
        static_cast<long double>(directionY) * directionY +
        static_cast<long double>(directionX) * directionX;
    const float length = static_cast<float>(std::sqrt(lengthSquared));

    Vec3_NormalizeHelper(directionBytes, nullptr, length);

    // Kept extended for the sign test (fcomp on st0 right after fst); the stored float copy is
    // what the in-segment and past-end branches reload (fld dword ptr [ebp+0Ch]).
    const long double projectionExtended =
        static_cast<long double>(directionY) * deltaY +
        static_cast<long double>(directionX) * deltaX +
        static_cast<long double>(directionZ) * deltaZ;
    const float projection = static_cast<float>(projectionExtended);

    if (projectionExtended < g_fZero)
    {
        CSF_AT(float, output, 0x00) = CSF_AT(float, self, 0x00);
        CSF_AT(float, output, 0x04) = CSF_AT(float, self, 0x04);
        CSF_AT(float, output, 0x08) = CSF_AT(float, self, 0x08);
    }
    else if (projection > length)
    {
        CSF_AT(float, output, 0x00) = CSF_AT(float, self, 0x0C);
        CSF_AT(float, output, 0x04) = CSF_AT(float, self, 0x10);
        CSF_AT(float, output, 0x08) = CSF_AT(float, self, 0x14);
    }
    else
    {
        directionX *= projection;
        directionY *= projection;

        // fld dirZ; fmul proj; fadd [self+8]; fstp: the product is not rounded to float first.
        const float projectedZ = static_cast<float>(
            static_cast<long double>(directionZ) * projection + CSF_AT(float, self, 0x08));
        const float projectedY = directionY + CSF_AT(float, self, 0x04);
        const float projectedX = directionX + CSF_AT(float, self, 0x00);

        CSF_AT(float, output, 0x00) = projectedX;
        CSF_AT(float, output, 0x04) = projectedY;
        CSF_AT(float, output, 0x08) = projectedZ;
    }

    return output;
}

HOOK(0x006B1240, Geometry_ProjectPointOntoSegment, "auto");
