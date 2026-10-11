#include <cstdint>
#include <cmath>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

CSF_GLOBAL(const float, g_RotationResetThreshold, 0x00899058);
CSF_GLOBAL(const float, g_SecondaryYRotationScale, 0x0082D54C);
CSF_GLOBAL(const float, g_SecondaryXRotationScale, 0x0082D550);
CSF_GLOBAL(float, g_AxisX, 0x008A7D6C);
CSF_GLOBAL(float, g_AxisY, 0x008A7D78);

static void __fastcall C_0082d58c_ApplyRotationDeltas(
    C_0082d58c* self, void* /*edx*/, float deltaY, float deltaX)
{
    const float accumulatedY = deltaY + self->field_001c;
    self->field_001c = accumulatedY;

    const float accumulatedX = deltaX + self->field_0020;
    self->field_0020 = accumulatedX;

    if (std::fabs(accumulatedY) > g_RotationResetThreshold)
    {
        self->field_001c = 0.0f;
        self->field_0010 = 1;
    }
    if (std::fabs(accumulatedX) > g_RotationResetThreshold)
    {
        self->field_0020 = 0.0f;
        self->field_0010 = 1;
    }

    void* target = reinterpret_cast<void*>(self->field_0028);
    if (target)
    {
        ((void(__fastcall*)(void*, void*, float*, float))
            CSF_AT(void**, target, 0)[0x48 / 4])(
                target, nullptr, &g_AxisY, deltaY);
    }

    target = reinterpret_cast<void*>(self->field_002c);
    if (target)
    {
        const float scaledDelta = deltaY * g_SecondaryYRotationScale;
        ((void(__fastcall*)(void*, void*, float*, float))
            CSF_AT(void**, target, 0)[0x48 / 4])(
                target, nullptr, &g_AxisY, scaledDelta);
    }

    target = reinterpret_cast<void*>(self->field_0030);
    if (target)
    {
        ((void(__fastcall*)(void*, void*, float*, float))
            CSF_AT(void**, target, 0)[0x48 / 4])(
                target, nullptr, &g_AxisX, deltaX);
    }

    target = reinterpret_cast<void*>(self->field_0034);
    if (target)
    {
        const float scaledDelta = deltaX * g_SecondaryXRotationScale;
        ((void(__fastcall*)(void*, void*, float*, float))
            CSF_AT(void**, target, 0)[0x48 / 4])(
                target, nullptr, &g_AxisX, scaledDelta);
    }
}

HOOK(0x004055E0, C_0082d58c_ApplyRotationDeltas, "auto");
