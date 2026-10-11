#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

static CSF_GLOBAL(float, g_fZero, 0x0082D4CC);

static void __fastcall C_0082dfd0_UpdateChannelLevels(
    C_0082dfd0* self, void* /*edx*/, float deltaTime)
{
    const float limit0 = self->field_00e0 * self->field_0110;
    const float limit1 = self->field_00e4 * self->field_0110;
    const float limit2 = self->field_00e8 * self->field_0110;
    const float limit3 = self->field_00ec * self->field_0110;

    if (self->field_010c)
    {
        const float level = deltaTime * self->field_0100 + self->field_00f0;
        self->field_00f0 = level;
        if (level > limit0)
            self->field_00f0 = limit0;
    }
    else
    {
        const float level = self->field_00f0 - deltaTime * self->field_0100;
        self->field_00f0 = level;
        if (level < g_fZero)
            self->field_00f0 = 0.0f;
    }

    const float step1 = deltaTime * self->field_0104;
    if (self->field_010d)
    {
        const float level = step1 + self->field_00f4;
        self->field_00f4 = level;
        if (level > limit1)
            self->field_00f4 = limit1;
    }
    else
    {
        const float level = self->field_00f4 - step1;
        self->field_00f4 = level;
        if (level < g_fZero)
            self->field_00f4 = 0.0f;
    }

    const float sharedStep = deltaTime * self->field_0108;
    if (self->field_010e)
    {
        const float level = sharedStep + self->field_00f8;
        self->field_00f8 = level;
        if (level > limit2)
            self->field_00f8 = limit2;
    }
    else
    {
        const float level = self->field_00f8 - sharedStep;
        self->field_00f8 = level;
        if (level < g_fZero)
            self->field_00f8 = 0.0f;
    }

    if (self->field_010f)
    {
        const float level = sharedStep + self->field_00fc;
        self->field_00fc = level;
        if (level > limit3)
            self->field_00fc = limit3;
    }
    else
    {
        const float level = self->field_00fc - sharedStep;
        self->field_00fc = level;
        if (level < g_fZero)
            self->field_00fc = 0.0f;
    }
}

HOOK(0x00411C00, C_0082dfd0_UpdateChannelLevels, "auto");
