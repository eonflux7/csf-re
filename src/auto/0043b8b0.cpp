#include <cstdint>
#include <cmath>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

CSF_FUNC(float*, __fastcall, NormalizeVector2, 0x006AEE00, (float* self, void*));
CSF_GLOBAL(float, g_HalfDurationScale, 0x0082D444);
CSF_GLOBAL(float, g_fZero, 0x0082D4CC);

static float* __fastcall C_00830e5c_ComputeMinimapOffset(
    C_00830e5c* self, void* /*edx*/,
    float* result, float* position, float* origin,
    float worldRadius, char outsideFlag, uint8_t* flag)
{
    const float positionX = position[0];
    const float positionZ = position[2];
    const float originX = origin[0];
    const float originZ = origin[2];

    *flag = 1;

    alignas(8) uint8_t offsetBytes[0x8];
    float* offset = reinterpret_cast<float*>(offsetBytes);
    offset[0] = positionX - originX;
    offset[1] = positionZ - originZ;

    const uint32_t right = self->field_002c;
    const uint32_t left = self->field_0024;
    const double distance = std::sqrt(
        static_cast<double>(offset[1]) * offset[1] +
        static_cast<double>(offset[0]) * offset[0]);
    const double radius =
        static_cast<double>(static_cast<int32_t>(right - left)) *
        g_HalfDurationScale;
    const float mapRadius = static_cast<float>(radius);
    const float scaledDistance =
        static_cast<float>(radius * distance / worldRadius);

    if (distance != g_fZero)
    {
        NormalizeVector2(offset, nullptr);
        offset[0] *= scaledDistance;
        offset[1] *= scaledDistance;
    }

    if (std::sqrt(
            static_cast<double>(offset[1]) * offset[1] +
            static_cast<double>(offset[0]) * offset[0]) > mapRadius)
    {
        *flag = static_cast<uint8_t>(outsideFlag);
        NormalizeVector2(offset, nullptr);
        offset[0] *= mapRadius;
        offset[1] *= mapRadius;
    }

    const float offsetX = offset[0];
    const float offsetZ = offset[1];
    result[0] = offsetX;
    result[1] = offsetZ;
    return result;
}

HOOK(0x0043B8B0, C_00830e5c_ComputeMinimapOffset, "auto");
