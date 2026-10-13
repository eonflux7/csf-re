#include <cstdint>
#include <cstddef>

#include "globals.h"
#include "hook.h"

CSF_GLOBAL(float, g_fZero, 0x0082D4CC);
CSF_GLOBAL(float, g_TransitionOne, 0x0082D4D4);
CSF_GLOBAL(float, g_ZeroVectorX, 0x008BB420);
CSF_GLOBAL(float, g_ZeroVectorY, 0x008BB424);
CSF_GLOBAL(float, g_ZeroVectorZ, 0x008BB428);

// Divides the source vector (this, float[3]) by a scalar, writing into dest.
// When the divisor is exactly zero, dest receives the global zero vector instead.
static float* __fastcall GfxObjectInstances_DivideVector(float* self, void* /*edx*/, float* dest, float divisor)
{
    if (divisor == g_fZero) {
        dest[0] = g_ZeroVectorX;
        dest[1] = g_ZeroVectorY;
        dest[2] = g_ZeroVectorZ;
    } else {
        float reciprocal = g_TransitionOne / divisor;
        dest[0] = self[0] * reciprocal;
        dest[1] = self[1] * reciprocal;
        dest[2] = self[2] * reciprocal;
    }
    return dest;
}

HOOK(0x006AB870, GfxObjectInstances_DivideVector, "auto");
