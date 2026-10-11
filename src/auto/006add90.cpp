#include <cstdint>
#include <cstring>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_FUNC(uint32_t*, __fastcall, Vector_ConvertDirection, 0x006ABA10, (void* self, void* edx, uint32_t* a2));

CSF_GLOBAL(float, g_BasisDegeneracyThreshold, 0x00860090);
CSF_GLOBAL(float, g_AxisX, 0x008A7D6C);
CSF_GLOBAL(float, g_AxisX_Y, 0x008A7D70);
CSF_GLOBAL(float, g_AxisX_Z, 0x008A7D74);
CSF_GLOBAL(float, g_AxisY, 0x008A7D78);
CSF_GLOBAL(float, g_AxisY_Y, 0x008A7D7C);
CSF_GLOBAL(float, g_AxisY_Z, 0x008A7D80);

static int __cdecl GfxMatrix_BuildDirectionBasis(
    S_00526d40* source, float* direction, float* perpendicular, int outputAddress)
{
    uint32_t convertedStorage[0x18 / sizeof(uint32_t)];
    uint32_t* converted =
        Vector_ConvertDirection(source, nullptr, convertedStorage);

    std::memcpy(direction, converted, sizeof(float));
    std::memcpy(direction + 1, converted + 1, sizeof(float));
    std::memcpy(direction + 2, converted + 2, sizeof(float));

    float x = g_AxisY_Z * direction[1] - g_AxisY_Y * direction[2];
    float y = g_AxisY * direction[2] - g_AxisY_Z * direction[0];
    float z = g_AxisY_Y * direction[0] - g_AxisY * direction[1];

    perpendicular[0] = x;
    perpendicular[1] = y;
    perpendicular[2] = z;

    if (__builtin_sqrtf(perpendicular[0] * perpendicular[0] +
                        perpendicular[1] * perpendicular[1] +
                        perpendicular[2] * perpendicular[2]) <
        g_BasisDegeneracyThreshold)
    {
        x = g_AxisX_Z * direction[1] - g_AxisX_Y * direction[2];
        y = g_AxisX * direction[2] - g_AxisX_Z * direction[0];
        z = g_AxisX_Y * direction[0] - g_AxisX * direction[1];

        perpendicular[0] = x;
        perpendicular[1] = y;
        perpendicular[2] = z;
    }

    x = perpendicular[1] * direction[2] - perpendicular[2] * direction[1];
    y = perpendicular[2] * direction[0] - perpendicular[0] * direction[2];
    z = direction[1] * perpendicular[0] - perpendicular[1] * direction[0];

    int result;
    std::memcpy(&result, &z, sizeof(result));

    float* output = reinterpret_cast<float*>(outputAddress);
    output[0] = x;
    output[1] = y;
    std::memcpy(output + 2, &result, sizeof(result));
    return result;
}

HOOK(0x006ADD90, GfxMatrix_BuildDirectionBasis, "auto");
