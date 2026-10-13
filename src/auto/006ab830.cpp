#include "globals.h"
#include "hook.h"

CSF_GLOBAL(float, g_fZero, 0x0082D4CC);
CSF_GLOBAL(float, g_TransitionOne, 0x0082D4D4);

// Divides the 3-float vector at 'self' by 'length' (multiplying by the reciprocal)
// when length is not zero. Returns 'self'.
static float* __fastcall GfxBBoxMath_Vec3ScaleByReciprocal(float* self, void* /*edx*/, float length)
{
    float* result = self;
    if (length != g_fZero)
    {
        float reciprocal = g_TransitionOne / length;
        self[0] = reciprocal * self[0];
        self[1] = reciprocal * self[1];
        self[2] = reciprocal * self[2];
    }
    return result;
}

HOOK(0x006AB830, GfxBBoxMath_Vec3ScaleByReciprocal, "auto");
