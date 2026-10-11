#include <cstdint>
#include <cstring>
#include <cmath>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_GLOBAL(float, g_fZero, 0x0082D4CC);
CSF_GLOBAL(float, g_TransitionOne, 0x0082D4D4);
CSF_GLOBAL(float, g_BoxIntersectionEpsilon, 0x00860070);

static float GfxMatrix_DecodeFloat(std::uint32_t bits)
{
    float value;
    std::memcpy(&value, &bits, sizeof(value));
    return value;
}

static int __fastcall S_00402350_IntersectOrientedBox(
    S_00402350* self, void* /*edx*/, const void* box,
    float* firstDistance, float* secondDistance)
{
    const float deltaX =
        CSF_AT(float, box, 0x00) - GfxMatrix_DecodeFloat(self->field_000c);
    const float deltaY =
        CSF_AT(float, box, 0x04) - GfxMatrix_DecodeFloat(self->field_0010);
    const float deltaZ =
        CSF_AT(float, box, 0x08) - GfxMatrix_DecodeFloat(self->field_0014);

    const float halfExtents[3] = {
        CSF_AT(float, box, 0x0C),
        CSF_AT(float, box, 0x10),
        CSF_AT(float, box, 0x14)
    };

    float nearDistance = -3.4028235e38f;
    float farDistance = 3.4028235e38f;
    const auto* axis = reinterpret_cast<const std::uint8_t*>(box) + 0x18;

    for (int i = 0; i < 3; ++i, axis += 0x0C)
    {
        const float offset =
            deltaY * CSF_AT(float, axis, 0x04) +
            deltaX * CSF_AT(float, axis, 0x00) +
            deltaZ * CSF_AT(float, axis, 0x08);
        const float direction =
            GfxMatrix_DecodeFloat(self->field_0004) * CSF_AT(float, axis, 0x04) +
            GfxMatrix_DecodeFloat(self->field_0000) * CSF_AT(float, axis, 0x00) +
            GfxMatrix_DecodeFloat(self->field_0008) * CSF_AT(float, axis, 0x08);

        float forwardLimit;
        if (!(std::fabs(direction) > g_BoxIntersectionEpsilon))
        {
            if (-offset - halfExtents[i] > g_fZero)
                return 0;
            forwardLimit = halfExtents[i] - offset;
        }
        else
        {
            const float inverseDirection = g_TransitionOne / direction;
            float entry = (offset + halfExtents[i]) * inverseDirection;
            float exit = (offset - halfExtents[i]) * inverseDirection;
            if (entry > exit)
            {
                const float temporary = entry;
                entry = exit;
                exit = temporary;
            }

            if (entry > nearDistance)
                nearDistance = entry;
            if (exit < farDistance)
                farDistance = exit;
            if (nearDistance > farDistance)
                return 0;
            forwardLimit = farDistance;
        }

        if (forwardLimit < g_fZero)
            return 0;
    }

    if (!(nearDistance > g_fZero))
    {
        *firstDistance = farDistance;
        *secondDistance = nearDistance;
    }
    else
    {
        *firstDistance = nearDistance;
        *secondDistance = farDistance;
    }

    if (!(*secondDistance > g_fZero))
        return 1;
    if (*firstDistance == *secondDistance)
        return 1;
    return 2;
}

HOOK(0x006ACB20, S_00402350_IntersectOrientedBox, "auto");
