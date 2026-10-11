#include <cstdint>
#include <cstddef>
#include <cstring>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"
#include "legacy/string.h"

CSF_GLOBAL(float, g_fZero, 0x0082D4CC);
CSF_GLOBAL(float, g_ZeroVectorX, 0x008BB420);

CSF_FUNC(char, __fastcall, NotifyDepleted_49D400, 0x0049D400, (void* self, void* edx, void* a3, int a4));
CSF_FUNC(char, __fastcall, Autonomo_DispatchAction, 0x004D1160, (void* self, void* edx, int a2));

// Advances the depletion countdown by 'elapsed' seconds. When the countdown
// crosses zero the state becomes "depleted"; on the first depleted tick the
// linked object is notified. Always finishes by dispatching the autonomo action.
static char __fastcall C_0083b2e0_TickDepletionTimer(C_0083b2e0* self, void* /*edx*/, float elapsed)
{
    uint8_t enabled = self->field_0038;
    if (!enabled)
        return static_cast<char>(enabled);

    uint32_t depletionState = self->field_0040;
    if (depletionState == 0)
    {
        float remaining = *reinterpret_cast<float*>(&self->field_0044) - elapsed;
        *reinterpret_cast<float*>(&self->field_0044) = remaining;
        if (remaining < g_fZero)
            self->field_0040 = 1;
    }
    else if (depletionState == 1)
    {
        NotifyDepleted_49D400(reinterpret_cast<void*>(self->field_0014), nullptr,
                              &g_ZeroVectorX, 0);
    }

    int32_t elapsedBits = 0;
    std::memcpy(&elapsedBits, &elapsed, sizeof elapsedBits);
    return Autonomo_DispatchAction(self, nullptr, elapsedBits);
}

HOOK(0x004CF5A0, C_0083b2e0_TickDepletionTimer, "auto");
