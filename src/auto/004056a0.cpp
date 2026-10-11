#include <cmath>
#include "globals.h"
#include "hook.h"

CSF_GLOBAL(float, g_MissingMetricValue, 0x82D4C4);
CSF_GLOBAL(float, g_Epsilon, 0x82D56C);

// Pairs a magnitude (a1) with a companion component (a2).
// Returns 1 when the pair was fully resolved (a1 zero, or a2 dominates),
// returns 0 when the component was combined into a1.
static char __cdecl AxisPair_TransferComponents(float* a1, float* a2)
{
    const float magnitude = std::fabs(*a1);
    if (magnitude <= g_Epsilon)
    {
        *a2 = 0.0f;
        return 1;
    }

    if (std::fabs(*a2) >= magnitude)
    {
        *a2 = *a1;
        *a1 = 0.0f;
        return 1;
    }

    if (*a1 <= g_Epsilon)
    {
        const float original = *a1;
        *a1 = original + *a2;
        *a2 = *a2 * g_MissingMetricValue;
    }
    else
    {
        *a1 = *a1 - *a2;
    }
    return 0;
}

HOOK(0x4056A0, AxisPair_TransferComponents, "auto");
