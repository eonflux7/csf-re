#include <cmath>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_FUNC(float*, __fastcall, NormalizeVector3, 0x006AB8D0, (C_00833b88* self, void*));

CSF_GLOBAL(float, g_HalfDurationScale, 0x0082D444);
CSF_GLOBAL(float, g_fZero, 0x0082D4CC);
CSF_GLOBAL(float, g_AxisX, 0x008A7D6C);
CSF_GLOBAL(float, g_AxisX_Y, 0x008A7D70);
CSF_GLOBAL(float, g_AxisX_Z, 0x008A7D74);
CSF_GLOBAL(float, g_AxisY, 0x008A7D78);
CSF_GLOBAL(float, g_AxisY_Y, 0x008A7D7C);
CSF_GLOBAL(float, g_AxisY_Z, 0x008A7D80);

static float* __stdcall GfxWater_ComputeCenterAndBasis(
    float* geometry, float* center,
    C_00833b88* tangent, C_00833b88* bitangent)
{
    const float halfZ = geometry[17] * g_HalfDurationScale;
    const float centerX = geometry[15] * g_HalfDurationScale + geometry[12];
    const float centerY = geometry[16] * g_HalfDurationScale + geometry[13];
    const float centerZ = halfZ + geometry[14];

    center[0] = centerX;
    center[1] = centerY;
    center[2] = centerZ;

    // These arguments are three-float vectors, despite their recovered class type.
    float& tangentX = *reinterpret_cast<float*>(&tangent->vtbl);
    float& tangentY = *reinterpret_cast<float*>(&tangent->field_0004);
    float& tangentZ = CSF_AT(float, tangent, 0x8);

    bool useYAxis =
        geometry[15] == g_AxisX &&
        geometry[16] == g_AxisX_Y &&
        geometry[17] == g_AxisX_Z;

    if (!useYAxis)
    {
        const float x = g_AxisX_Y * geometry[17] - g_AxisX_Z * geometry[16];
        const float y = g_AxisX_Z * geometry[15] - g_AxisX * geometry[17];
        const float z = g_AxisX * geometry[16] - g_AxisX_Y * geometry[15];

        tangentX = x;
        tangentY = y;
        tangentZ = z;

        useYAxis = g_fZero == std::sqrt(
            tangentX * tangentX + tangentY * tangentY + tangentZ * tangentZ);
    }

    if (useYAxis)
    {
        const float x = g_AxisY_Y * geometry[17] - g_AxisY_Z * geometry[16];
        const float y = g_AxisY_Z * geometry[15] - g_AxisY * geometry[17];
        const float z = g_AxisY * geometry[16] - g_AxisY_Y * geometry[15];

        tangentX = x;
        tangentY = y;
        tangentZ = z;
    }

    const float x = tangentY * geometry[17] - geometry[16] * tangentZ;
    const float y = tangentZ * geometry[15] - tangentX * geometry[17];
    const float z = geometry[16] * tangentX - tangentY * geometry[15];

    *reinterpret_cast<float*>(&bitangent->vtbl) = x;
    *reinterpret_cast<float*>(&bitangent->field_0004) = y;
    CSF_AT(float, bitangent, 0x8) = z;

    NormalizeVector3(tangent, nullptr);
    return NormalizeVector3(bitangent, nullptr);
}

HOOK(0x0070B380, GfxWater_ComputeCenterAndBasis, "auto");
