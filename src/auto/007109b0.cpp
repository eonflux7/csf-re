#include <cstdint>
#include <cstddef>
#include <cstring>
#include <cmath>
#include <climits>
#include <cfloat>
#include "globals.h"
#include "hook.h"

// Audio_SetEffectParameter(effect, paramId=11, 1, value): variadic, value is passed as a raw dword.
CSF_FUNC(uint32_t*, __cdecl, Audio_SetEffectParameter, 0x007E1380, (uint32_t* a1, int a2, int a3, ...));

// Bit pattern of 1.0f as stored in the effect gain slot (self[34]) and passed as the effect value.
static constexpr uint32_t kUnitGainBits = 0x3F800000u;

// Callback(void* argBlock, int ctx) returns bool (tested with `test al, al`), cdecl.
using GainPredicate = bool(__cdecl*)(void* argBlock, int ctx);

// params: { +0x00 callback, +0x04 ctx, +0x08 bool useCallback }
static uint32_t* __cdecl AudioEffects_RefreshEffectGain(uint32_t* self, int params)
{
    const int ctx = CSF_AT(int, params, 0x04);
    bool gainOn;

    if (ctx == 0)
    {
        gainOn = false;
    }
    else if (CSF_AT(uint8_t, params, 0x08) != 0)
    {
        // Snapshot three state dwords from the object (self[22..24]) for the callback.
        uint32_t argBlock[3];
        argBlock[0] = self[22];
        argBlock[1] = self[23];
        argBlock[2] = self[24];

        auto predicate = reinterpret_cast<GainPredicate>(CSF_AT(uint32_t, params, 0x00));
        gainOn = predicate(argBlock, ctx);
    }
    else
    {
        gainOn = true;
    }

    // Original re-reads self[12] after any callback.
    const int effectHandle = static_cast<int>(self[12]);

    self[34] = gainOn ? kUnitGainBits : 0u;

    if (effectHandle != 0)
    {
        Audio_SetEffectParameter(reinterpret_cast<uint32_t*>(effectHandle), 11, 1,
                                 gainOn ? static_cast<int>(kUnitGainBits) : 0);
    }

    return self;
}

HOOK(0x007109B0, AudioEffects_RefreshEffectGain, "auto");
