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

CSF_FUNC(void, __fastcall, S_004a1210_RefreshEmbeddedState, 0x004A12D0, (void* self, void* edx, float a2));

// Advances the embedded timer by a2, then computes an exponential decay factor
// 2^(-elapsed * rate) and stores the two scaled outputs. Tail-calls the refresh routine.
static void __fastcall S_004a1210_UpdateTimedState(S_004a1210* self, void* /*edx*/, float a2)
{
    // Accumulate elapsed time.
    self->field_0014 = a2 + self->field_0014;

    // Decay exponent: -(elapsed * rate) * log2(e).
    double exponent = -(static_cast<double>(self->field_0014) * self->field_0018) * 1.442695040888963407;

    // 2^exponent (the original's frndint / f2xm1 / fscale sequence).
    double decayFactor = std::exp2(exponent);

    float scaledValueB = static_cast<float>(decayFactor) * self->field_0008;
    self->field_000c = static_cast<float>(decayFactor) * self->field_0004;
    self->field_0010 = scaledValueB;

    S_004a1210_RefreshEmbeddedState(self, nullptr, a2);
}

HOOK(0x004A1680, S_004a1210_UpdateTimedState, "auto");
