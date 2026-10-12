#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_GLOBAL(float, g_ColorChannelScale, 0x0082FEA8);

// Accumulates deltaValue into field_0010. Below the threshold in field_0008 it stores the
// scaled ratio into field_000c and notifies the target object; once the threshold is reached
// it latches field_0024 to 255.0, notifies, marks field_0000 and resets the accumulator.
static int __fastcall FadeAccumulator_AdvanceAndNotify(S_004382f0* self, void* /*edx*/, float deltaValue)
{
    float accumulated = deltaValue + self->field_0010;
    void* targetObject = reinterpret_cast<void*>(self->field_0004);
    self->field_0010 = accumulated;

    if (accumulated < self->field_0008)
    {
        self->field_000c = accumulated / self->field_0008 * g_ColorChannelScale;
        return reinterpret_cast<int(__fastcall*)(void*, void*, uint8_t*)>(
            CSF_AT(void**, targetObject, 0)[24])(
            targetObject, nullptr, reinterpret_cast<uint8_t*>(self) + 0x18);
    }

    self->field_0024 = 0x437F0000u;
    int result = reinterpret_cast<int(__fastcall*)(void*, void*, uint8_t*)>(
        CSF_AT(void**, targetObject, 0)[24])(
        targetObject, nullptr, reinterpret_cast<uint8_t*>(self) + 0x18);
    self->field_0000 = 1;
    self->field_0010 = 0.0f;
    return result;
}

HOOK(0x4382F0, FadeAccumulator_AdvanceAndNotify, "auto");
