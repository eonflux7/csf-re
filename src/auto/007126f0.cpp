#include <cstdint>
#include <cstddef>
#include <cstring>
#include <cmath>
#include <climits>
#include <cfloat>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"
#include "legacy/string.h"

CSF_GLOBAL(float, g_fZero, 0x0082D4CC);
CSF_GLOBAL(float, g_TransitionOne, 0x0082D4D4);
CSF_GLOBAL(float, g_ScaleA, 0x008A85DC);
CSF_GLOBAL(float, g_ScaleB, 0x008A85D8);

CSF_FUNC(uint32_t*, __cdecl, sub_7E1380, 0x007E1380, (uint32_t* a1, int a2, int a3, ...));

// Clamps the requested gain to [0, 1], stores it on the object, rescales the
// effect's level in the owned audio channel, and forwards the scaled value to
// the effect dispatcher when the channel handle is present.
static int __fastcall C_00863dd0_SetClampedGain(C_00863dd0* self, void* /*edx*/, float requestedGain)
{
    float clampedGain = requestedGain;
    if (requestedGain <= g_TransitionOne) {
        if (clampedGain < g_fZero)
            clampedGain = g_fZero;
    } else {
        clampedGain = g_TransitionOne;
    }
    self->field_0008 = clampedGain;

    uint8_t* channel = reinterpret_cast<uint8_t*>(self->field_0004);
    CSF_AT(float, channel, 0x40) = g_ScaleA * g_ScaleB * clampedGain;

    int result = CSF_AT(int, reinterpret_cast<uint8_t*>(self->field_0004), 0x50);
    if (result) {
        int effectHandle = CSF_AT(int, reinterpret_cast<uint8_t*>(self->field_0004), 0x30);
        result = effectHandle;
        if (result) {
            float scaledGain = g_ScaleA * self->field_0008 * g_ScaleB;
            uint32_t scaledBits;
            std::memcpy(&scaledBits, &scaledGain, sizeof(scaledBits));
            uintptr_t ret = reinterpret_cast<uintptr_t>(
                sub_7E1380(reinterpret_cast<uint32_t*>(effectHandle), 5, 1, scaledBits));
            return static_cast<int>(ret);
        }
    }
    return result;
}

HOOK(0x7126F0, C_00863dd0_SetClampedGain, "auto");
