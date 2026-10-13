#include <cstdint>
#include <cstddef>
#include <cstring>
#include <cmath>
#include <climits>
#include <cfloat>
#include "globals.h"
#include "hook.h"

// Float 1.0 constant used as the EWMA threshold and the (1 - w) base.
CSF_GLOBAL(float, g_TransitionOne, 0x82D4D4);

// Records one sample into a running perf-counter block:
//   +0x00 int   sample count
//   +0x04 float running sum
//   +0x08 float max
//   +0x0C float min
//   +0x10 float exponentially weighted average
//   +0x14 float per-sample weight
//   +0x18 float accumulated weight
// Returns the new sample count.
static int __fastcall NetPerfCounters_RecordSample(void* self, void* /*edx*/, float sample)
{
    if (CSF_AT(int, self, 0x00) == 0)
    {
        CSF_AT(float, self, 0x08) = sample;
        CSF_AT(float, self, 0x0C) = sample;
    }
    else
    {
        if (sample > CSF_AT(float, self, 0x08))
            CSF_AT(float, self, 0x08) = sample;
        if (sample < CSF_AT(float, self, 0x0C))
            CSF_AT(float, self, 0x0C) = sample;
    }

    float weight = CSF_AT(float, self, 0x14);
    float blend = weight;
    float accumulated = weight + CSF_AT(float, self, 0x18);

    if (accumulated < g_TransitionOne)
        blend = weight / accumulated;

    CSF_AT(float, self, 0x18) = accumulated;
    CSF_AT(float, self, 0x10) =
        (g_TransitionOne - blend) * CSF_AT(float, self, 0x10) + blend * sample;

    int newCount = CSF_AT(int, self, 0x00) + 1;
    float newSum = sample + CSF_AT(float, self, 0x04);
    CSF_AT(int, self, 0x00) = newCount;
    CSF_AT(float, self, 0x04) = newSum;
    return newCount;
}

HOOK(0x0078D040, NetPerfCounters_RecordSample, "auto");
