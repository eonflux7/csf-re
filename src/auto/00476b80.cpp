#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_FUNC(void, __cdecl, ClampFloatToBounds, 0x00406B60, (float* value, float* lowerBound, float* upperBound));
CSF_FUNC(int, __cdecl, GetGlobalState_50D190, 0x0050D190, ());
CSF_FUNC(float, __fastcall, MenuSystem_GetTermTransitionScale, 0x0050DA40, (void* self, void*));
CSF_FUNC(int, __cdecl, GetGlobalValue_8BC1E0, 0x00756A10, ());

CSF_GLOBAL(float, g_fZero, 0x0082D4CC);
CSF_GLOBAL(float, g_TransitionOne, 0x0082D4D4);
CSF_GLOBAL(float, g_TransitionEpsilon, 0x008316B0);

static double __fastcall C_008316d8_GetTransitionScale(
    C_008316d8* self, void* /*edx*/)
{
    if (CSF_AT(float, reinterpret_cast<void*>(GetGlobalValue_8BC1E0()), 0xAF4)
        < g_TransitionEpsilon)
    {
        return g_fZero;
    }

    float transitionScale = 1.0f;
    if (CSF_AT(float, reinterpret_cast<void*>(GetGlobalValue_8BC1E0()), 0xAF4)
        > g_fZero)
    {
        void* globalValues = reinterpret_cast<void*>(GetGlobalValue_8BC1E0());
        int globalState = GetGlobalState_50D190();
        float menuScale = MenuSystem_GetTermTransitionScale(
            reinterpret_cast<void*>(globalState), nullptr);
        transitionScale = menuScale * CSF_AT(float, globalValues, 0xAFC);

        float firstBound = 0.0f;
        float secondBound = 1.0f;
        ClampFloatToBounds(&transitionScale, &firstBound, &secondBound);

        globalValues = reinterpret_cast<void*>(GetGlobalValue_8BC1E0());
        float fraction = self->field_0018 / CSF_AT(float, globalValues, 0xAF4);

        firstBound = 1.0f;
        secondBound = 0.0f;
        transitionScale =
            fraction * (g_TransitionOne - transitionScale) + transitionScale;
        ClampFloatToBounds(&transitionScale, &secondBound, &firstBound);
    }

    return transitionScale;
}

HOOK(0x00476B80, C_008316d8_GetTransitionScale, "auto");
