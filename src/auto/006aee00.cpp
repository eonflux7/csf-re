#include <cmath>
#include "globals.h"
#include "hook.h"

CSF_GLOBAL(float, g_fZero, 0x82D4CC);
CSF_GLOBAL(float, g_TransitionOne, 0x82D4D4);

// Normalizes the 2D vector {x, y} stored at self. Returns self.
// Left untouched when the length is zero (or NaN, which the original's
// unordered compare also skips).
static float* __fastcall NormalizeVector2(float* self, void* /*edx*/)
{
    const float length = std::sqrt(self[0] * self[0] + self[1] * self[1]);
    float* result = self;
    if (length > g_fZero)
    {
        const float inverseLength = g_TransitionOne / length;
        self[0] = inverseLength * self[0];
        self[1] = inverseLength * self[1];
    }
    return result;
}

HOOK_KEEP_ECX(0x006AEE00, NormalizeVector2, 0, "auto");
