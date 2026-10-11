#include <cmath>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"

// Oriented box containment test. The box layout is:
//   +0x00..0x08  origin (x, y, z)
//   +0x0C..0x14  half extents along the three axes
//   +0x18..0x28  three axis direction vectors (3 floats each)
// Returns true when the point lies within all three slabs.
static bool __fastcall Bounds_ContainsPoint(uint8_t* self, void* /*edx*/, float* point)
{
    float localDelta[3];
    localDelta[0] = point[0] - CSF_AT(float, self, 0x00);
    localDelta[1] = point[1] - CSF_AT(float, self, 0x04);
    localDelta[2] = point[2] - CSF_AT(float, self, 0x08);

    for (int axisIndex = 0; axisIndex < 3; ++axisIndex)
    {
        int axisOffset = 0x18 + axisIndex * 12;
        int extentOffset = 0x0C + axisIndex * 4;

        float projection =
            localDelta[0] * CSF_AT(float, self, axisOffset) +
            localDelta[1] * CSF_AT(float, self, axisOffset + 4) +
            localDelta[2] * CSF_AT(float, self, axisOffset + 8);

        if (std::fabs(projection) > CSF_AT(float, self, extentOffset))
            return false;
    }
    return true;
}

HOOK(0x006AC9A0, Bounds_ContainsPoint, "auto");
