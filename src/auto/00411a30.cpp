#include <cstdint>
#include <cstring>
#include <cmath>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

CSF_FUNC(int, __fastcall, GetObjectPositionVector, 0x0048E430, (void* self, void* edx));

CSF_GLOBAL(float, g_fZero, 0x0082D4CC);
CSF_GLOBAL(float, g_VerticalGravity, 0x0082DFB8);
CSF_GLOBAL(float, g_VerticalResetDistance, 0x0082DFC8);
CSF_GLOBAL(float, g_MessageBoxCollapsedScale, 0x0082DFCC);

static void __fastcall C_0082dfd0_UpdateVerticalVelocity(
    C_0082dfd0* self, void* /*edx*/, float deltaTime)
{
    if (deltaTime > g_MessageBoxCollapsedScale)
        deltaTime = 0.050000001f;

    if (self->field_0088)
    {
        if (!self->field_0089 && !self->field_008a)
            self->field_0084 = self->field_013c * g_VerticalGravity * deltaTime;
        else
            self->field_0084 = 0.0f;
        return;
    }

    const uint8_t checkHeight = self->field_008c;
    const float velocity =
        self->field_013c * g_VerticalGravity * deltaTime + self->field_0084;
    self->field_0084 = velocity;

    if (checkHeight && velocity > g_fZero)
    {
        float referenceHeight;
        std::memcpy(&referenceHeight, &self->field_00dc, sizeof(referenceHeight));

        void* position = reinterpret_cast<void*>(
            GetObjectPositionVector(
                reinterpret_cast<void*>(self->field_0004), nullptr));

        if (std::fabs(referenceHeight - CSF_AT(float, position, 0x004))
            >= g_VerticalResetDistance)
        {
            self->field_008c = 0;
            self->field_0084 = self->field_013c * g_VerticalGravity * deltaTime;
        }
    }
}

HOOK(0x00411A30, C_0082dfd0_UpdateVerticalVelocity, "auto");
