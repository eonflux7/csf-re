#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

CSF_GLOBAL(float, g_fZero, 0x0082D4CC);
CSF_GLOBAL(float, g_MinimumQueryHeight, 0x0082DF18);

static void __fastcall S_006b0320_EnsureCounterclockwiseWinding(
    S_006b0320* self, void* /*edx*/)
{
    float maximum = g_MinimumQueryHeight;
    const int count = static_cast<int32_t>(self->field_0008);
    int maximumIndex = -1;

    if (count > 0)
    {
        const auto* points =
            reinterpret_cast<const uint8_t*>(self->field_0000);
        for (int index = 0; index < count; ++index)
        {
            if (maximum < CSF_AT(float, points, index * 8))
            {
                maximumIndex = index;
                maximum = CSF_AT(float, points, index * 8);
            }
        }
    }

    if (maximumIndex == -1)
        return;

    const int previousIndex = (count + maximumIndex - 1) % count;
    const int nextIndex = (maximumIndex + 1) % count;
    const auto* points =
        reinterpret_cast<const uint8_t*>(self->field_0000);

    const float incomingX =
        CSF_AT(float, points, maximumIndex * 8) -
        CSF_AT(float, points, previousIndex * 8);
    const float incomingY =
        CSF_AT(float, points, maximumIndex * 8 + 4) -
        CSF_AT(float, points, previousIndex * 8 + 4);
    const float outgoingX =
        CSF_AT(float, points, nextIndex * 8) -
        CSF_AT(float, points, maximumIndex * 8);
    const float outgoingY =
        CSF_AT(float, points, nextIndex * 8 + 4) -
        CSF_AT(float, points, maximumIndex * 8 + 4);

    const float reversedCross =
        outgoingX * incomingY - outgoingY * incomingX;
    if (reversedCross < g_fZero)
        return;

    const int halfCount = count / 2;
    uint32_t endOffset = static_cast<uint32_t>(count) * 8;

    for (int index = 0; index < halfCount; ++index, endOffset -= 8)
    {
        auto* buffer = reinterpret_cast<uint8_t*>(self->field_0000);
        const uint32_t frontOffset = static_cast<uint32_t>(index) * 8;

        const uint32_t lastX = CSF_AT(uint32_t, buffer, endOffset - 8);
        const uint32_t firstY = CSF_AT(uint32_t, buffer, frontOffset + 4);
        const uint32_t firstX = CSF_AT(uint32_t, buffer, frontOffset);

        CSF_AT(uint32_t, buffer, frontOffset) = lastX;
        const uint32_t lastY = CSF_AT(uint32_t, buffer, endOffset - 4);
        CSF_AT(uint32_t, buffer, frontOffset + 4) = lastY;

        buffer = reinterpret_cast<uint8_t*>(self->field_0000);
        CSF_AT(uint32_t, buffer, endOffset - 8) = firstX;
        CSF_AT(uint32_t, buffer, endOffset - 4) = firstY;
    }
}

HOOK(0x006AFEF0, S_006b0320_EnsureCounterclockwiseWinding, "auto");
