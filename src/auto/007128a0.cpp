#include <cstdint>
#include <cstddef>
#include <cstring>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"
#include "legacy/string.h"

CSF_GLOBAL(float, g_AudioLevelScale, 0x008A85DC);

CSF_FUNC(uint32_t*, __cdecl, AudioEffect_SetParamSlot, 0x007E1380, (uint32_t* a1, int a2, int a3, ...));

static int __cdecl AudioEffect_ApplyScaledLevel(int a1, float* a2)
{
    if (!a2)
        return a1;

    void* effect = reinterpret_cast<void*>(a1);
    if (CSF_AT(int, effect, 0x50))
    {
        int paramTarget = CSF_AT(int, effect, 0x30);
        float scaledLevel = g_AudioLevelScale * *a2;
        CSF_AT(float, effect, 0x40) = scaledLevel;
        if (paramTarget)
        {
            // The original pushes the float's 4 raw bytes as the variadic argument.
            uint32_t levelBits = 0;
            std::memcpy(&levelBits, &scaledLevel, sizeof(levelBits));
            AudioEffect_SetParamSlot(reinterpret_cast<uint32_t*>(paramTarget), 5, 1, levelBits);
        }
    }
    return a1;
}

HOOK(0x7128A0, AudioEffect_ApplyScaledLevel, "auto");
