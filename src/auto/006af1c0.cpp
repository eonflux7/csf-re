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

CSF_GLOBAL(float, g_SegmentInterpolationStep, 0x008A7DFC);

CSF_FUNC(int, __fastcall, AiSubStateObject_SampleCurve, 0x006AEF40,
         (void* self, void* edx, float a2, float* a3));

static int __fastcall AiSubStateObject_Advance(S_006aef40* self, void* /*edx*/, float elapsed)
{
    // Accumulate elapsed time; the comparison result decides whether we are inside the curve.
    float accumulatedTime = elapsed + self->field_004c;
    self->field_004c = accumulatedTime;

    float curveParameter;
    if (accumulatedTime < self->field_0048)
    {
        float scaledProgress = accumulatedTime * self->field_0044;
        uintptr_t timesAddr = static_cast<uintptr_t>(self->field_0034);
        float* segmentTimes = reinterpret_cast<float*>(timesAddr);

        // Advance the segment index while the scaled progress has passed the segment's end time.
        if (scaledProgress >= segmentTimes[self->field_0050])
        {
            do
            {
                uint32_t nextIndex = self->field_0050 + 1;
                self->field_0050 = nextIndex;
            } while (scaledProgress >= reinterpret_cast<float*>(static_cast<uintptr_t>(self->field_0034))[self->field_0050]);
        }

        uint32_t segmentIndex = self->field_0050;
        float segmentStart = segmentTimes[segmentIndex - 1];
        uint32_t previousIndex = segmentIndex - 1;
        float segmentEnd = segmentTimes[segmentIndex];

        float withinSegment = (scaledProgress - segmentStart) / (segmentEnd - segmentStart);
        curveParameter = withinSegment * g_SegmentInterpolationStep
                       + static_cast<float>(static_cast<int>(previousIndex)) * g_SegmentInterpolationStep;
    }
    else
    {
        curveParameter = 1.0f;
    }

    int result = AiSubStateObject_SampleCurve(
        self, nullptr, curveParameter,
        reinterpret_cast<float*>(reinterpret_cast<uint8_t*>(self) + 0x58));
    self->field_0054 = static_cast<uint32_t>(result);
    return result;
}

HOOK(0x006AF1C0, AiSubStateObject_Advance, "auto");
