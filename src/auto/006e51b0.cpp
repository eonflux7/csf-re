#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

CSF_GLOBAL(float, g_fZero, 0x0082D4CC);
CSF_GLOBAL(float, g_TransitionOne, 0x0082D4D4);

// Clamps the value to [0, 1], then if it differs from the object's current
// value (vSlot8), stores the clamped float at +0x0C and the raw 32-bit
// pattern of a3 at +0x10.
static void __fastcall C_00861bd8_SetClampedValueAndBits(C_00861bd8* self, void* /*edx*/, float a2, int a3)
{
    if (a2 < g_fZero)
    {
        a2 = 0.0f;
    }
    else if (a2 > g_TransitionOne)
    {
        a2 = 1.0f;
    }

    if (a2 != self->vSlot8())
    {
        *reinterpret_cast<float*>(&self->field_000c) = a2;
        *reinterpret_cast<int*>(&self->field_0010) = a3;
    }
}

HOOK(0x006E51B0, C_00861bd8_SetClampedValueAndBits, "auto");
