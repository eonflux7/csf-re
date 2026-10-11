#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"

CSF_FUNC(int, __cdecl, GetGlobalState_50D190, 0x0050D190, ());
CSF_FUNC(int, __fastcall, GetStateValue, 0x0050DD30, (void* self, void* edx, int a2));

CSF_GLOBAL(float, g_fZero, 0x0082D4CC);
CSF_GLOBAL(float, g_ProgressScale, 0x0082E03C);

static char __fastcall AiAttackTasks_UpdateAlternatingTimers(
    float* self, void* /*edx*/, float elapsed)
{
    if (CSF_AT(float, self, 0x08) < g_fZero)
    {
        const float remaining = CSF_AT(float, self, 0x0C) - elapsed;
        CSF_AT(float, self, 0x0C) = remaining;

        if (remaining < g_fZero)
        {
            const float duration = CSF_AT(float, self, 0x00);
            CSF_AT(float, self, 0x0C) = 0.0f;

            void* state = reinterpret_cast<void*>(GetGlobalState_50D190());
            const int value = GetStateValue(state, nullptr, 100);
            CSF_AT(float, self, 0x08) =
                static_cast<float>(value) * duration * g_ProgressScale;
            return 0;
        }
        return 1;
    }

    const float remaining = CSF_AT(float, self, 0x08) - elapsed;
    CSF_AT(float, self, 0x08) = remaining;

    if (remaining < g_fZero)
    {
        const float duration = CSF_AT(float, self, 0x04);
        void* state = reinterpret_cast<void*>(GetGlobalState_50D190());
        const int value = GetStateValue(state, nullptr, 100);
        CSF_AT(float, self, 0x0C) =
            static_cast<float>(value) * duration * g_ProgressScale;
        return 1;
    }
    return 0;
}

HOOK(0x0047AED0, AiAttackTasks_UpdateAlternatingTimers, "auto");
