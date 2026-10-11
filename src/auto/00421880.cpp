#include "globals.h"
#include "hook.h"

// Float constants referenced by the original (read through x87 fld, so returned as double).
CSF_GLOBAL(float, g_TransitionHalfPi, 0x0082FAD0);
CSF_GLOBAL(float, g_HudNegativeHalfPi, 0x0082FACC);
CSF_GLOBAL(float, g_PiFloat, 0x0082E88C);

// Returns the rotation angle (radians) associated with an AI task state code.
// States 5 -> +pi/2, 6 -> -pi/2, 2 and 4 -> pi, anything else -> 0.0.
static double __stdcall AiTaskStates_GetRotationForState(int stateCode)
{
    switch (stateCode)
    {
    case 5:
        return g_TransitionHalfPi;
    case 6:
        return g_HudNegativeHalfPi;
    case 2:
    case 4:
        return g_PiFloat;
    }
    return 0.0;
}

HOOK(0x421880, AiTaskStates_GetRotationForState, "auto");
