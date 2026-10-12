#include <cstdint>
#include <cstddef>
#include <cstring>
#include <cmath>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_FUNC(int, __cdecl, GetGlobalValue_8BC1E0, 0x00756A10, ());

CSF_GLOBAL(float, g_fZero, 0x0082D4CC);
CSF_GLOBAL(float, g_TransitionOne, 0x0082D4D4);
CSF_GLOBAL(float, g_Pi, 0x00830D10);

static float& GlobalFloat(int base, int off)
{
    return CSF_AT(float, reinterpret_cast<uint8_t*>(base), off);
}

static void __fastcall Minimap_UpdateTransitionPulse(void* self, void* /*edx*/, float deltaTime)
{
    // accumulate time, wrap to zero once the period is exceeded
    CSF_AT(float, self, 0x44) = deltaTime + CSF_AT(float, self, 0x44);

    float timer;
    if (CSF_AT(float, self, 0x44) <= GlobalFloat(GetGlobalValue_8BC1E0(), 0x2268))
        timer = CSF_AT(float, self, 0x44);
    else
        timer = g_fZero;
    CSF_AT(float, self, 0x44) = timer;

    float phase = CSF_AT(float, self, 0x44) / GlobalFloat(GetGlobalValue_8BC1E0(), 0x2268);

    float minValue = GlobalFloat(GetGlobalValue_8BC1E0(), 0x226C);
    if (minValue >= g_fZero) {
        if (minValue > g_TransitionOne)
            minValue = g_TransitionOne;
    } else {
        minValue = g_fZero;
    }

    float maxValue = GlobalFloat(GetGlobalValue_8BC1E0(), 0x2270);
    if (maxValue >= g_fZero) {
        if (maxValue > g_TransitionOne)
            maxValue = g_TransitionOne;
    } else {
        maxValue = g_fZero;
    }

    CSF_AT(float, self, 0x40) =
        (float)(std::fabs(std::cos((double)(phase * g_Pi))) * (double)(maxValue - minValue) + (double)minValue);
}

HOOK(0x0043AC60, Minimap_UpdateTransitionPulse, "auto");
