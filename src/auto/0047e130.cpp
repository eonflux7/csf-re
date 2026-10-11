#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

static CSF_GLOBAL(float, g_fZero, 0x0082D4CC);
static CSF_GLOBAL(float, g_TransitionOne, 0x0082D4D4);

static float* __fastcall C_00832eec_InterpolateVector(
    C_00832eec* self, void* /*edx*/, float* output, float parameter)
{
    const uint32_t initialBegin = self->field_000c;
    if (!initialBegin ||
        static_cast<int32_t>(self->field_0010 - initialBegin) / 12 == 0)
    {
        output[0] = 0.0f;
        output[1] = 0.0f;
        output[2] = 0.0f;
        return output;
    }

    if (parameter < g_fZero)
    {
        const float* first = reinterpret_cast<const float*>(self->field_000c);
        output[0] = first[0];
        output[1] = first[1];
        output[2] = first[2];
        return output;
    }

    if (((bool(__fastcall*)(void*, void*, float))
         CSF_AT(void**, self, 0)[0x10 / 4])(self, nullptr, parameter))
    {
        const float* last =
            reinterpret_cast<const float*>(self->field_0010 - 12);
        output[0] = last[0];
        output[1] = last[1];
        output[2] = last[2];
        return output;
    }

    const uint32_t begin = self->field_000c;
    const uint32_t count = begin
        ? static_cast<uint32_t>(
              static_cast<int32_t>(self->field_0010 - begin) / 12)
        : 0;

    uint32_t lower = 0;
    uint32_t upper = 0;
    for (uint32_t index = 0; index < count; ++index)
    {
        const uint32_t next = index + 1;
        if (next >= count)
        {
            lower = index;
            upper = index;
            break;
        }

        const float* parameters =
            reinterpret_cast<const float*>(self->field_001c);
        if (parameter <= parameters[next])
        {
            lower = index;
            upper = next;
            break;
        }
    }

    float x;
    float y;
    float z;
    if (lower != upper)
    {
        const float* parameters =
            reinterpret_cast<const float*>(self->field_001c);
        const float numerator = parameter - parameters[lower];
        const float* vectors =
            reinterpret_cast<const float*>(self->field_000c);
        const float denominator = parameters[upper] - parameters[lower];
        const float fraction = numerator / denominator;
        const float inverseFraction = g_TransitionOne - fraction;
        const float* first = vectors + 3 * lower;
        const float* second = vectors + 3 * upper;

        x = inverseFraction * first[0] + fraction * second[0];
        y = inverseFraction * first[1] + fraction * second[1];
        z = inverseFraction * first[2] + fraction * second[2];
    }
    else
    {
        const float* vector =
            reinterpret_cast<const float*>(self->field_000c) + 3 * lower;
        y = vector[1];
        x = vector[0];
        z = vector[2];
    }

    output[0] = x;
    output[1] = y;
    output[2] = z;
    return output;
}

HOOK(0x0047E130, C_00832eec_InterpolateVector, "auto");
