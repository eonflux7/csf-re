#include <cstdint>
#include <cstring>
#include <cmath>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_FUNC(void, __fastcall, C_0082d58c_ApplyMotionDelta, 0x004055E0,
         (void* self, void* edx, float a2, float a3));
CSF_FUNC(char, __cdecl, ClampMotionDelta, 0x004056A0,
         (float* a1, float* a2));
CSF_FUNC(int, __cdecl, DispatchMotionEvent, 0x00405AB0,
         (int a1, uint32_t* arg4, char arg8));

CSF_GLOBAL(float, g_MotionEpsilon, 0x0082D56C);

static void __fastcall C_0082d58c_UpdateMotion(
    C_0082d58c* self, void* /*edx*/, float elapsedTime)
{
    const uint8_t wasActive = self->field_0011;
    self->field_0010 = 0;

    if (!wasActive)
        return;

    float deltaX = elapsedTime * self->field_000c;
    elapsedTime = deltaX;

    const char changedX = ClampMotionDelta(
        reinterpret_cast<float*>(&self->field_0014), &deltaX);
    const char changedY = ClampMotionDelta(
        &self->field_0018, &elapsedTime);
    const char changed = changedY | changedX;

    C_0082d58c_ApplyMotionDelta(self, nullptr, deltaX, elapsedTime);

    if (changed)
    {
        float remainingX;
        std::memcpy(&remainingX, &self->field_0014, sizeof(remainingX));

        const uint8_t stillActive =
            std::fabs(remainingX) > g_MotionEpsilon ||
            std::fabs(self->field_0018) > g_MotionEpsilon;

        self->field_0011 = stillActive;
        if (!stillActive)
        {
            const int eventTarget = static_cast<int>(self->field_0008 + 28);
            uint32_t event = 47;
            DispatchMotionEvent(eventTarget, &event, 0);
        }
    }
}

HOOK(0x00405BF0, C_0082d58c_UpdateMotion, "auto");
