#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_FUNC(void, __fastcall, OnInterpolationFinished, 0x00409300, (void* self, void* edx));

// Advances the interpolated value toward its target by deltaTime * rate while active.
// When the value reaches or passes the target, it is clamped, the active flag is cleared,
// and the completion callback runs. Overshoot past the target also triggers the callback.
static void __fastcall C_0082d844_StepInterpolation(C_0082d844* self, void* /*edx*/, float deltaTime)
{
    if (self->field_001c == 0)
        return;

    float stepAmount = deltaTime * self->field_0018;

    if (self->field_0014 <= self->field_0010)
    {
        // Value is above the target: decrease it.
        float newValue = self->field_0010 - stepAmount;
        self->field_0010 = newValue;
        if (newValue > self->field_0014)
        {
            OnInterpolationFinished(self, nullptr);
            return;
        }
    }
    else
    {
        // Value is below the target: increase it.
        float newValue = stepAmount + self->field_0010;
        self->field_0010 = newValue;
        if (newValue < self->field_0014)
        {
            OnInterpolationFinished(self, nullptr);
            return;
        }
    }

    // Reached or passed the target: clamp, deactivate, and notify.
    self->field_0010 = self->field_0014;
    self->field_001c = 0;
    OnInterpolationFinished(self, nullptr);
}

HOOK(0x00409390, C_0082d844_StepInterpolation, "auto");
