#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"

CSF_GLOBAL(float, g_HalfDurationScale, 0x0082D444);
CSF_GLOBAL(float, g_AxisX,            0x008A7D6C);
CSF_GLOBAL(float, g_AxisX_Y,          0x008A7D70);
CSF_GLOBAL(float, g_AxisX_Z,          0x008A7D74);
CSF_GLOBAL(float, g_AxisY,            0x008A7D78);
CSF_GLOBAL(float, g_AxisY_Y,          0x008A7D7C);
CSF_GLOBAL(float, g_AxisY_Z,          0x008A7D80);
CSF_GLOBAL(float, g_AxisZ,            0x008A7D84);
CSF_GLOBAL(float, g_AxisZ_Y,          0x008A7D88);
CSF_GLOBAL(float, g_AxisZ_Z,          0x008A7D8C);

static float* __fastcall GfxMatrix_SetBoundsAndDefaultAxes(
    char* self, void* /*edx*/,
    float maxX, float minX,
    float maxY, float minY,
    float maxZ, float minZ)
{
    CSF_AT(float, self, 0x00) = (maxX + minX) * g_HalfDurationScale;
    CSF_AT(float, self, 0x04) = (maxY + minY) * g_HalfDurationScale;
    CSF_AT(float, self, 0x08) = (maxZ + minZ) * g_HalfDurationScale;
    CSF_AT(float, self, 0x0C) = (maxX - minX) * g_HalfDurationScale;
    CSF_AT(float, self, 0x10) = (maxY - minY) * g_HalfDurationScale;
    CSF_AT(float, self, 0x14) = (maxZ - minZ) * g_HalfDurationScale;

    CSF_AT(uint32_t, self, 0x18) = 0;
    CSF_AT(uint32_t, self, 0x1C) = 0;
    CSF_AT(uint32_t, self, 0x20) = 0;
    CSF_AT(uint32_t, self, 0x24) = 0;
    CSF_AT(uint32_t, self, 0x28) = 0;
    CSF_AT(uint32_t, self, 0x2C) = 0;
    CSF_AT(uint32_t, self, 0x30) = 0;
    CSF_AT(uint32_t, self, 0x34) = 0;
    CSF_AT(uint32_t, self, 0x38) = 0;

    CSF_AT(float, self, 0x18) = g_AxisX;
    CSF_AT(float, self, 0x1C) = g_AxisX_Y;
    CSF_AT(float, self, 0x20) = g_AxisX_Z;
    CSF_AT(float, self, 0x24) = g_AxisY;
    CSF_AT(float, self, 0x28) = g_AxisY_Y;
    CSF_AT(float, self, 0x2C) = g_AxisY_Z;
    CSF_AT(float, self, 0x30) = g_AxisZ;
    CSF_AT(float, self, 0x34) = g_AxisZ_Y;
    CSF_AT(float, self, 0x38) = g_AxisZ_Z;

    return reinterpret_cast<float*>(self);
}

HOOK(0x006ABAA0, GfxMatrix_SetBoundsAndDefaultAxes, "auto");
