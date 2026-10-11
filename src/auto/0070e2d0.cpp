#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

CSF_GLOBAL(float, g_fZero, 0x0082D4CC);
CSF_GLOBAL(float, g_TransitionOne, 0x0082D4D4);

// Stores a transition progress value clamped to [0, 1] and marks the flag at +0x44.
// NaN compares false against value <= 1.0, so it takes the upper branch and stores 1.0.
static void __fastcall C_008639b0_SetTransitionProgress(C_008639b0* self, void* /*edx*/, float value)
{
    if (value <= g_TransitionOne)
    {
        float clamped = value;
        if (clamped < g_fZero)
            clamped = g_fZero;
        self->field_0008 = clamped;
        self->field_0044 = 1;
    }
    else
    {
        self->field_0044 = 1;
        self->field_0008 = g_TransitionOne;
    }
}

HOOK(0x0070E2D0, C_008639b0_SetTransitionProgress, "auto");
