#include <cstdint>
#include <cmath>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_FUNC(float*, __fastcall, NormalizeVector3, 0x006AB8D0, (void* self, void* edx));
CSF_FUNC(int, __fastcall, S_006add20_SetAxis, 0x006ADD20, (void* self, void* edx, int a2, uint32_t* a3));

CSF_GLOBAL(float, g_MinimumAxisCrossLength, 0x00860090);

static double (__cdecl* s_SquareRoot)(double) =
    static_cast<double (__cdecl*)(double)>(&std::sqrt);

static void __fastcall S_006add20_AlignAxesToDirection(
    S_006add20* self, void* /*edx*/,
    int fixedAxis, int alignedAxis, float* point, float* origin)
{
    (void)CSF_VTBL(0x006ADD20);

    int remainingAxis;
    switch (fixedAxis)
    {
    case 1:
        remainingAxis = (alignedAxis == 2) + 2;
        break;
    case 2:
        remainingAxis = 2 * (alignedAxis == 1) + 1;
        break;
    case 3:
        remainingAxis = (alignedAxis == 1) + 1;
        break;
    default:
        remainingAxis = alignedAxis;
        break;
    }

    float* fixedVector = reinterpret_cast<float*>(self->field_0000);
    if (fixedAxis != 1)
    {
        if (fixedAxis == 2)
            fixedVector += 4;
        else
            fixedVector += 8;
    }

    float direction[3];
    direction[0] = point[0] - origin[0];
    direction[1] = point[1] - origin[1];
    direction[2] = point[2] - origin[2];
    NormalizeVector3(direction, nullptr);

    float remainingVector[3];
    remainingVector[0] = direction[2] * fixedVector[1]
                       - direction[1] * fixedVector[2];
    remainingVector[1] = direction[0] * fixedVector[2]
                       - direction[2] * fixedVector[0];
    remainingVector[2] = direction[1] * fixedVector[0]
                       - direction[0] * fixedVector[1];

    if (s_SquareRoot(remainingVector[2] * remainingVector[2]
                   + remainingVector[1] * remainingVector[1]
                   + remainingVector[0] * remainingVector[0])
        >= g_MinimumAxisCrossLength)
    {
        NormalizeVector3(remainingVector, nullptr);

        float alignedVector[3];
        alignedVector[0] = remainingVector[2] * fixedVector[1]
                        - remainingVector[1] * fixedVector[2];
        alignedVector[1] = remainingVector[0] * fixedVector[2]
                        - remainingVector[2] * fixedVector[0];
        alignedVector[2] = remainingVector[1] * fixedVector[0]
                        - remainingVector[0] * fixedVector[1];
        NormalizeVector3(alignedVector, nullptr);

        S_006add20_SetAxis(
            self, nullptr, remainingAxis,
            reinterpret_cast<uint32_t*>(remainingVector));
        S_006add20_SetAxis(
            self, nullptr, alignedAxis,
            reinterpret_cast<uint32_t*>(alignedVector));
    }
}

HOOK(0x006AE040, S_006add20_AlignAxesToDirection, "auto");
