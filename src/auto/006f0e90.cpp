#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

CSF_GLOBAL(float, g_fZero, 0x0082D4CC);

// Accumulates a2 into field_0030 while field_002c is zero, and once the
// accumulator exceeds the threshold stored at +0x54 of the sub-object at
// field_0018, fires the virtual slot 0x2C (vSlot2C(0)) and resets the accumulator.
static void __fastcall C_00862390_AccumulateTimerAndFire(C_00862390* self, void* /*edx*/, float a2)
{
    void* thresholdOwner = reinterpret_cast<void*>(self->field_0018);

    if (CSF_AT(float, thresholdOwner, 0x54) > g_fZero)
    {
        if (self->field_002c == g_fZero)
            self->field_0030 = a2 + self->field_0030;

        if (self->field_0030 > CSF_AT(float, thresholdOwner, 0x54))
        {
            self->vSlot2C(0);
            self->field_0030 = 0.0f;
        }
    }
}

HOOK(0x6F0E90, C_00862390_AccumulateTimerAndFire, "auto");
