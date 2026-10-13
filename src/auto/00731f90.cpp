#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_GLOBAL(float, g_fZero, 0x0082D4CC);
CSF_GLOBAL(float, g_TransitionOne, 0x0082D4D4);

CSF_FUNC(float*, __fastcall, GfxLights_AddVector4ToDest, 0x006D1150, (float* self, void* , float* dest, float* src));

static uint32_t* __fastcall C_0086709c_InterpolateVector4(
    C_0086709c* self, void* /*edx*/, uint32_t* output, float time)
{
    const uint32_t begin = self->field_000c;
    if (!begin ||
        !(static_cast<int32_t>(self->field_0010 - begin) >> 4))
    {
        output[3] = 0;
        output[2] = 0;
        output[1] = 0;
        output[0] = 0;
        return output;
    }

    if (time < g_fZero)
    {
        const auto* first = reinterpret_cast<const uint32_t*>(self->field_000c);
        output[0] = first[0];
        output[1] = first[1];
        output[2] = first[2];
        output[3] = first[3];
        return output;
    }

    if (((bool(__fastcall*)(C_0086709c*, void*, float))
             self->vtbl[0x10 / 4])(self, nullptr, time))
    {
        const auto* last =
            reinterpret_cast<const uint32_t*>(self->field_0010) - 4;
        output[0] = last[0];
        output[1] = last[1];
        output[2] = last[2];
        output[3] = last[3];
        return output;
    }

    uint32_t lowerIndex = 0;
    uint32_t upperIndex = 0;
    const uint32_t currentBegin = self->field_000c;
    const uint32_t count = currentBegin
        ? static_cast<uint32_t>(
              static_cast<int32_t>(self->field_0010 - currentBegin) >> 4)
        : 0;

    for (uint32_t index = 0; index < count; )
    {
        const uint32_t nextIndex = index + 1;
        if (nextIndex >= count)
        {
            lowerIndex = index;
            upperIndex = index;
        }
        else if (time <=
                 reinterpret_cast<const float*>(self->field_001c)[nextIndex])
        {
            lowerIndex = index;
            upperIndex = nextIndex;
            break;
        }
        index = nextIndex;
    }

    uint32_t x, y, z, w;
    if (lowerIndex != upperIndex)
    {
        const auto* times =
            reinterpret_cast<const float*>(self->field_001c);
        const float numerator = time - times[lowerIndex];
        const uint32_t vectors = self->field_000c;
        const float denominator = times[upperIndex] - times[lowerIndex];
        const auto* lower = reinterpret_cast<const float*>(
            vectors + 16 * lowerIndex);
        const auto* upper = reinterpret_cast<const float*>(
            vectors + 16 * upperIndex);
        const float upperWeight = numerator / denominator;
        const float lowerWeight = g_TransitionOne - upperWeight;

        alignas(8) uint8_t resultAndUpperBytes[0x20];
        alignas(8) uint8_t lowerBytes[0x14];
        auto* result = reinterpret_cast<float*>(resultAndUpperBytes);
        auto* weightedUpper =
            reinterpret_cast<float*>(resultAndUpperBytes + 0x10);
        auto* weightedLower = reinterpret_cast<float*>(lowerBytes);

        weightedLower[3] = lowerWeight * lower[3];
        weightedLower[2] = lowerWeight * lower[2];
        weightedLower[1] = lowerWeight * lower[1];
        weightedLower[0] = lowerWeight * lower[0];

        weightedUpper[3] = upperWeight * upper[3];
        weightedUpper[2] = upperWeight * upper[2];
        weightedUpper[1] = upperWeight * upper[1];
        weightedUpper[0] = upperWeight * upper[0];

        float* sum = GfxLights_AddVector4ToDest(
            weightedUpper, nullptr, result, weightedLower);
        x = CSF_AT(uint32_t, sum, 0x00);
        y = CSF_AT(uint32_t, sum, 0x04);
        z = CSF_AT(uint32_t, sum, 0x08);
        w = CSF_AT(uint32_t, sum, 0x0C);
    }
    else
    {
        const auto* selected = reinterpret_cast<const uint32_t*>(
            self->field_000c + 16 * lowerIndex);
        x = selected[0];
        y = selected[1];
        z = selected[2];
        w = selected[3];
    }

    output[0] = x;
    output[1] = y;
    output[2] = z;
    output[3] = w;
    return output;
}

HOOK(0x00731F90, C_0086709c_InterpolateVector4, "auto");
