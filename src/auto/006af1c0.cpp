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

// Vtable of S_006aef40 as referenced by the original's call site.
static void* const kSubStateObjectVtable = CSF_VTBL(0x006AEF40);

CSF_GLOBAL(float, g_DistanceSampleStep, 0x008A7DFC);

CSF_FUNC(int, __fastcall, S_006aef40_EvaluateHermite, 0x006AEF40,
         (S_006aef40* self, void* edx, float time, float* result));

static int __fastcall AiSubStateObject_Advance(S_006aef40* self, void* /*edx*/, float elapsed)
{
    // The original keeps every intermediate in the x87 register file (80-bit
    // extended); `fst` only stores a rounded copy, the register stays extended.
    const long double accumulatedTime = static_cast<long double>(elapsed)
                                      + static_cast<long double>(self->field_004c);
    self->field_004c = static_cast<float>(accumulatedTime);

    float curveParameter;
    // fcom + test ah,1: C0 (st < field_0048, or unordered) -> inside the curve.
    if (!(accumulatedTime >= static_cast<long double>(self->field_0048))
        || accumulatedTime != accumulatedTime)
    {
        const long double scaledProgress = accumulatedTime * static_cast<long double>(self->field_0044);
        float* segmentTimes = reinterpret_cast<float*>(static_cast<uintptr_t>(self->field_0034));

        // Advance the segment index while the scaled progress has passed the segment's end time.
        if (!(scaledProgress < static_cast<long double>(segmentTimes[self->field_0050])))
        {
            do
            {
                self->field_0050 = self->field_0050 + 1;
            } while (!(scaledProgress < static_cast<long double>(segmentTimes[self->field_0050])));
        }

        const uint32_t index = self->field_0050;
        const uint32_t previousIndex = index - 1;
        const long double segmentStart = segmentTimes[previousIndex];
        const long double segmentEnd = segmentTimes[index];

        const long double withinSegment = (scaledProgress - segmentStart) / (segmentEnd - segmentStart);
        curveParameter = static_cast<float>(
            withinSegment * static_cast<long double>(g_DistanceSampleStep)
            + static_cast<long double>(static_cast<int>(previousIndex)) * static_cast<long double>(g_DistanceSampleStep));
    }
    else
    {
        curveParameter = 1.0f;
    }

    const int result = S_006aef40_EvaluateHermite(
        self, nullptr, curveParameter,
        reinterpret_cast<float*>(reinterpret_cast<uint8_t*>(self) + 0x58));
    self->field_0054 = static_cast<uint32_t>(result);
    return result;
}

HOOK(0x006AF1C0, AiSubStateObject_Advance, "auto");
