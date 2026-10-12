#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"

CSF_FUNC(int, __cdecl, GetGlobalState_50D190, 0x0050D190, ());
CSF_FUNC(double, __fastcall, MenuSystem_GetIndexedGlobalFloat, 0x0050DA90, (void* self, void*));

CSF_GLOBAL(float, g_fZero, 0x0082D4CC);
CSF_GLOBAL(float, g_TransitionOne, 0x0082D4D4);

static char __fastcall AiDistracted_IsReactionReady(void* self, void* /*edx*/)
{
    if (!(CSF_AT(float, self, 0x60) < g_fZero) ||
        !(CSF_AT(float, self, 0x5C) < g_fZero))
        return 0;

    CSF_AT(float, self, 0x5C) = 0.2f;
    const int globalState = GetGlobalState_50D190();
    const double indexedValue = MenuSystem_GetIndexedGlobalFloat(
        reinterpret_cast<void*>(globalState), nullptr);
    const double reactionScale = -(CSF_AT(float, self, 0x60) / indexedValue);

    if (!(reactionScale > g_TransitionOne))
        return 1;

    const double reactionDelay = CSF_AT(float, self, 0x5C) / reactionScale;
    CSF_AT(float, self, 0x5C) = static_cast<float>(reactionDelay);
    if (reactionDelay <= g_fZero)
        CSF_AT(float, self, 0x5C) = 0.1f;

    return 1;
}

HOOK(0x00448240, AiDistracted_IsReactionReady, "auto");
