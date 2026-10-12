#include <cstdint>
#include <cmath>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

CSF_FUNC(void*, __fastcall, S_0041e5f0_GrowArrayCapacity, 0x0041E5F0, (S_0041e5f0* self, void* , int additionalCapacity));
CSF_GLOBAL(float, g_TransitionOne, 0x0082D4D4);

static int __fastcall S_006aef40_BuildNormalizedArcLengths(
    S_006aef40* self, void* /*edx*/)
{
    const int pointCount = static_cast<int>(self->field_000c);
    float distance = 0.0f;
    uint32_t pointOffset = 0;

    for (int i = 0; i < pointCount; ++i, pointOffset += 12)
    {
        const float scaledDistance = distance / self->field_0044;

        if (CSF_AT(uint32_t, self, 0x1C) >=
            CSF_AT(uint32_t, self, 0x18))
        {
            // Also retain the address reference listed in the packet.
            (void)CSF_VTBL(0x0041E5F0);
            S_0041e5f0_GrowArrayCapacity(
                reinterpret_cast<S_0041e5f0*>(
                    reinterpret_cast<uint8_t*>(self) + 0x14),
                nullptr,
                CSF_AT(int, self, 0x20));
        }

        const uint32_t count = CSF_AT(uint32_t, self, 0x1C);
        float* destination =
            reinterpret_cast<float*>(self->field_0014) + count;
        CSF_AT(uint32_t, self, 0x1C) = count + 1;
        *destination = scaledDistance;

        if (i != pointCount - 1)
        {
            const uint8_t* point =
                reinterpret_cast<const uint8_t*>(self->field_0004) +
                pointOffset;
            const float dx =
                CSF_AT(float, point, 0x0C) - CSF_AT(float, point, 0x00);
            const float dy =
                CSF_AT(float, point, 0x10) - CSF_AT(float, point, 0x04);
            const float dz =
                CSF_AT(float, point, 0x14) - CSF_AT(float, point, 0x08);

            distance += std::sqrt(dz * dz + dy * dy + dx * dx);
        }
    }

    const float lastDistance =
        reinterpret_cast<float*>(self->field_0014)[pointCount - 1];
    int index = 0;

    if (pointCount >= 4)
    {
        const float reciprocal = g_TransitionOne / lastDistance;
        do
        {
            float* entry = reinterpret_cast<float*>(self->field_0014) + index;
            *entry *= reciprocal;

            entry = reinterpret_cast<float*>(self->field_0014) + index + 1;
            *entry *= reciprocal;

            entry = reinterpret_cast<float*>(self->field_0014) + index + 2;
            *entry *= reciprocal;

            entry = reinterpret_cast<float*>(self->field_0014) + index + 3;
            index += 4;
            *entry *= reciprocal;
        }
        while (index < pointCount - 3);
    }

    if (index < pointCount)
    {
        const float reciprocal = g_TransitionOne / lastDistance;
        do
        {
            float* entry = reinterpret_cast<float*>(self->field_0014) + index;
            ++index;
            *entry *= reciprocal;
        }
        while (index < pointCount);
    }

    return index;
}

HOOK(0x006AF250, S_006aef40_BuildNormalizedArcLengths, "auto");
