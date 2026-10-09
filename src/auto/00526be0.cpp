#include <cstdint>
#include <cstddef>
#include <cstring>
#include <cmath>
#include <climits>
#include <cfloat>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

// Original calls __ftol2 (float -> int64 truncation) and the vtable slot at +0x50
// of the object stored at +0x60. Both are bound here by address/semantics.

static void __fastcall C_008411a0_AdvanceLoopingTimer(C_008411a0* self, void* /*edx*/, float deltaTime)
{
    if (self->field_0050 > 0.0f)
    {
        float elapsed = deltaTime + self->field_0048;
        self->field_0048 = elapsed;

        if (self->field_0054)
        {
            if (elapsed >= self->field_004c && self->field_004c > 0.0f)
            {
                float remainingBefore = self->field_0050;
                int wholeLoops = (int)(elapsed / self->field_004c);
                float wrappedAmount = (float)wholeLoops * self->field_004c;
                self->field_0048 = elapsed - wrappedAmount;
                if (remainingBefore != 3.4028235e38f)
                {
                    self->field_0050 = self->field_0050 - wrappedAmount;
                }
            }
        }
    }

    void* tickSink = reinterpret_cast<void*>(self->field_0060);
    if (tickSink)
    {
        reinterpret_cast<void(__fastcall*)(void*, void*, float)>(CSF_AT(void**, tickSink, 0)[0x50 / 4])(tickSink, nullptr, deltaTime);
    }
}

HOOK(0x526BE0, C_008411a0_AdvanceLoopingTimer, "auto");
