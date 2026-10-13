#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

CSF_GLOBAL(float, g_fZero, 0x0082D4CC);
CSF_GLOBAL(float, g_TransitionOne, 0x0082D4D4);

static double __fastcall C_0086243c_EvaluateLinearCurve(
    C_0086243c* self, void* /*edx*/, float position)
{
    const uint32_t initialBegin = self->field_000c;
    if (!initialBegin ||
        !(static_cast<int32_t>(self->field_0010 - initialBegin) >> 2))
    {
        return g_fZero;
    }

    if (position < g_fZero)
        return reinterpret_cast<const float*>(self->field_000c)[0];

    if (reinterpret_cast<bool(__fastcall*)(C_0086243c*, void*, float)>(
            self->vtbl[0x10 / 4])(self, nullptr, position))
    {
        return reinterpret_cast<const float*>(self->field_0010)[-1];
    }

    const uint32_t begin = self->field_000c;
    const uint32_t count = begin
        ? static_cast<uint32_t>(
              static_cast<int32_t>(self->field_0010 - begin) >> 2)
        : 0;

    uint32_t lowerIndex = 0;
    uint32_t upperIndex = 0;
    uint32_t index = 0;

    while (index < count)
    {
        const uint32_t nextIndex = index + 1;
        if (nextIndex >= count)
        {
            lowerIndex = index;
            upperIndex = index;
        }
        else if (position <=
                 reinterpret_cast<const float*>(self->field_001c)[nextIndex])
        {
            lowerIndex = index;
            upperIndex = nextIndex;
            break;
        }
        index = nextIndex;
    }

    if (lowerIndex == upperIndex)
        return reinterpret_cast<const float*>(self->field_000c)[lowerIndex];

    const float* times =
        reinterpret_cast<const float*>(self->field_001c);
    const float fraction =
        (position - times[lowerIndex]) /
        (times[upperIndex] - times[lowerIndex]);
    const float* values =
        reinterpret_cast<const float*>(self->field_000c);

    return (g_TransitionOne - fraction) * values[lowerIndex]
         + fraction * values[upperIndex];
}

HOOK(0x006F1530, C_0086243c_EvaluateLinearCurve, "auto");
