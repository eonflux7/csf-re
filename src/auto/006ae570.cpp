#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

CSF_GLOBAL(float, g_FullRotationDegrees, 0x00830D04);
CSF_GLOBAL(float, g_PositiveHalfRotationDegrees, 0x00830D08);
CSF_GLOBAL(float, g_NegativeHalfRotationDegrees, 0x0083F18C);

static void __fastcall S_006ae570_NormalizeAngles(
    S_006ae570* self, void* /*edx*/)
{
    if (self->field_0004 >= g_PositiveHalfRotationDegrees)
    {
        double angle = self->field_0004;
        do
        {
            angle -= g_FullRotationDegrees;
        } while (angle >= g_PositiveHalfRotationDegrees);
        self->field_0004 = static_cast<float>(angle);
    }

    if (self->field_0004 < g_NegativeHalfRotationDegrees)
    {
        double angle = self->field_0004;
        do
        {
            angle += g_FullRotationDegrees;
        } while (angle < g_NegativeHalfRotationDegrees);
        self->field_0004 = static_cast<float>(angle);
    }

    if (self->field_0000 >= g_PositiveHalfRotationDegrees)
    {
        double angle = self->field_0000;
        do
        {
            angle -= g_FullRotationDegrees;
        } while (angle >= g_PositiveHalfRotationDegrees);
        self->field_0000 = static_cast<float>(angle);
    }

    if (self->field_0000 < g_NegativeHalfRotationDegrees)
    {
        double angle = self->field_0000;
        do
        {
            angle += g_FullRotationDegrees;
        } while (angle < g_NegativeHalfRotationDegrees);
        self->field_0000 = static_cast<float>(angle);
    }
}

HOOK(0x006AE570, S_006ae570_NormalizeAngles, "auto");
