#include "globals.h"
#include "hook.h"

CSF_GLOBAL(float, g_MissingMetricValue, 0x0082D4C4);
CSF_GLOBAL(float, g_fZero, 0x0082D4CC);
CSF_GLOBAL(float, g_TransitionOne, 0x0082D4D4);

// Returns g_TransitionOne for value > 0, g_MissingMetricValue for value < 0,
// and g_fZero for value == 0 or NaN (the unordered case takes the zero path).
static double __cdecl Water_SignOfFloat(float value) {
    if (value > 0.0f)
        return g_TransitionOne;
    if (value < 0.0f)
        return g_MissingMetricValue;
    return g_fZero;
}

HOOK(0x0070CCD0, Water_SignOfFloat, "auto");
