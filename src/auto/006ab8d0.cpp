#include <cmath>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_GLOBAL(float, g_fZero, 0x0082D4CC);
CSF_GLOBAL(float, g_TransitionOne, 0x0082D4D4);

static float* __fastcall C_00833b88_NormalizeVector3(
    C_00833b88* self, void* /*edx*/)
{
    // The supplied class type actually represents three float components here.
    float& x = reinterpret_cast<float&>(self->vtbl);
    float& y = reinterpret_cast<float&>(self->field_0004);
    float& z = CSF_AT(float, self, 0x008);

    // The original keeps the length on the x87 stack (extended precision): squares of very small components
    // don't underflow to zero there, so the sums are done in double, not float.
    const double length = std::sqrt(static_cast<double>(x) * x + static_cast<double>(y) * y
                                    + static_cast<double>(z) * z);
    if (length != g_fZero)
    {
        const double reciprocalLength = g_TransitionOne / length;
        x = static_cast<float>(reciprocalLength * x);
        y = static_cast<float>(reciprocalLength * y);
        z = static_cast<float>(reciprocalLength * z);
    }

    return reinterpret_cast<float*>(self);
}

// The original leaves ecx alone and callers go on using it after the call.
HOOK_KEEP_ECX(0x006AB8D0, C_00833b88_NormalizeVector3, 0, "auto");
