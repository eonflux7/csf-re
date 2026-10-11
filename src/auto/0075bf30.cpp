#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

// Clamp helper called from the timer sub-object's clamp routine (ecx = sub-object + 4).
CSF_FUNC(char, __fastcall, TimerSubObject_AdjustTimeout, 0x0075F0B0, (void* self, void* edx, int a2));

static unsigned int __fastcall C_0086c288_ResetTimerState(C_0086c288* self, void* /*edx*/) {
    self->field_0018 = 0;
    self->field_001c = 0;
    self->field_0030 = 0;

    // Original: v1 = this->field_0014; the clamp body at 0x75F1B0 is inlined here (it is part of this function's flow).
    uint8_t* timerSub = reinterpret_cast<uint8_t*>(self->field_0014);
    unsigned int result = CSF_AT(uint32_t, timerSub, 0x08);
    if (result < 0x1F4) {
        result = static_cast<unsigned int>(
            TimerSubObject_AdjustTimeout(timerSub + 4, nullptr, static_cast<int>(0x1F4 - result)));
    }
    CSF_AT(uint32_t, timerSub, 0x10) = 0x1F4;
    return result;
}

HOOK(0x0075BF30, C_0086c288_ResetTimerState, "auto");
