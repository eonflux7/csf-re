#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

CSF_GLOBAL(float, g_HalfDurationScale, 0x0082D444);
CSF_GLOBAL(float, g_AxisX, 0x008A7D6C);
CSF_GLOBAL(float, g_AxisX_Y, 0x008A7D70);
CSF_GLOBAL(float, g_AxisX_Z, 0x008A7D74);
CSF_GLOBAL(float, g_AxisY, 0x008A7D78);
CSF_GLOBAL(float, g_AxisY_Y, 0x008A7D7C);
CSF_GLOBAL(float, g_AxisY_Z, 0x008A7D80);
CSF_GLOBAL(float, g_AxisZ, 0x008A7D84);
CSF_GLOBAL(float, g_AxisZ_Y, 0x008A7D88);
CSF_GLOBAL(float, g_AxisZ_Z, 0x008A7D8C);

static float* __fastcall S_006abb80_InitializeBounds(
    S_006abb80* self, void* /*edx*/,
    float xFirst, float xSecond,
    float yFirst, float ySecond,
    float zFirst, float zSecond)
{
    self->field_0018 = g_AxisX;
    CSF_AT(float, self, 0x1C) = g_AxisX_Y;
    CSF_AT(float, self, 0x20) = g_AxisX_Z;
    self->field_0024 = g_AxisY;
    CSF_AT(float, self, 0x28) = g_AxisY_Y;
    CSF_AT(float, self, 0x2C) = g_AxisY_Z;
    self->field_0030 = g_AxisZ;

    const float axisZY = g_AxisZ_Y;
    const float centerX = (xFirst + xSecond) * g_HalfDurationScale;
    CSF_AT(float, self, 0x34) = axisZY;
    const float axisZZ = g_AxisZ_Z;
    self->field_0000 = centerX;
    CSF_AT(float, self, 0x38) = axisZZ;

    self->field_0004 = (yFirst + ySecond) * g_HalfDurationScale;
    self->field_0008 = (zFirst + zSecond) * g_HalfDurationScale;
    self->field_000c = (xFirst - xSecond) * g_HalfDurationScale;
    self->field_0010 = (yFirst - ySecond) * g_HalfDurationScale;
    self->field_0014 = (zFirst - zSecond) * g_HalfDurationScale;

    return &self->field_0030;
}

HOOK(0x006ABB80, S_006abb80_InitializeBounds, "auto");
