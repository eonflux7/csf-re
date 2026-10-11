#include <cstdint>
#include <cstddef>
#include <cstring>
#include <cmath>
#include <climits>
#include <cfloat>
#include "globals.h"
#include "hook.h"

// Clamped setter for the audio effects unit gain (flt_8A85DC), clamped to [0.0, 1.0].
// The original is __cdecl with no stack cleanup; a2/a3 are not used.
// The comparisons use the original's x87 fcomp semantics: NaN is not greater than 1.0
// and is not less than 0.0, so it is stored unchanged.
CSF_GLOBAL(float, g_AudioUnitGain, 0x8A85DC);
CSF_GLOBAL(float, g_fZero, 0x82D4CC);
CSF_GLOBAL(float, g_TransitionOne, 0x82D4D4);

static void __cdecl Audio_SetUnitGainClamped(float gain, int /*a2*/, int /*a3*/)
{
    if (!(gain > g_TransitionOne))
    {
        if (!(gain < g_fZero))
            g_AudioUnitGain = gain;
        else
            g_AudioUnitGain = 0.0f;
    }
    else
    {
        g_AudioUnitGain = 1.0f;
    }
}

HOOK(0x712770, Audio_SetUnitGainClamped, "auto");
