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
CSF_GLOBAL(float, g_fEpsilon, 0x00867480);
CSF_GLOBAL(float, g_fPi, 0x0086747C);

// Advances the phase accumulator at +0x10 by elapsed time and writes a
// sine-shaped interpolation between +0x18 and +0x1C into +0x14.
// The phase wraps to zero once it reaches the period stored at +0x20.
static void __fastcall SineOscillator_Advance(void* self, void* /*edx*/, float elapsedTime)
{
    if (CSF_AT(uint8_t, self, 4) == 0)
        return;

    float period = CSF_AT(float, self, 0x20);
    if (!(std::fabs(-period) > g_fEpsilon))
        return;

    float phase = elapsedTime + CSF_AT(float, self, 0x10);
    CSF_AT(float, self, 0x10) = phase;
    if (!(phase < CSF_AT(float, self, 0x20)))
        phase = g_fZero;
    CSF_AT(float, self, 0x10) = phase;

    float minValue = CSF_AT(float, self, 0x18);
    float maxValue = CSF_AT(float, self, 0x1C);
    float wave = std::sin(phase / CSF_AT(float, self, 0x20) * g_fPi);
    CSF_AT(float, self, 0x14) = wave * (maxValue - minValue) + minValue;
}

HOOK(0x0073BA10, SineOscillator_Advance, "auto");
