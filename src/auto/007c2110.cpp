#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"

CSF_GLOBAL(float, g_fZero, 0x0082D4CC);
CSF_GLOBAL(float, g_TransitionOne, 0x0082D4D4);
CSF_GLOBAL(float, g_TriangleQueryToleranceScale, 0x00845DB0);

static float GfxClipHelpers_AbsoluteCoordinate(float value)
{
    return !(value >= g_fZero) ? -value : value;
}

static void __cdecl GfxClipHelpers_InitializeTriangleQuery(
    void* query, const void* bounds, const void* segment,
    const uint32_t* tolerance)
{
    auto* queryBytes = static_cast<uint8_t*>(query);

    CSF_AT(const void*, query, 0x00) = bounds;
    CSF_AT(uint16_t, query, 0x36) = 0;
    CSF_AT(uint8_t, query, 0x35) = 0xFF;
    if (CSF_AT(uint16_t, bounds, 0x1E) == 0)
        CSF_AT(uint8_t, query, 0x35) = CSF_AT(uint8_t, bounds, 0x1C);

    CSF_AT(uint32_t, query, 0x04) = CSF_AT(uint32_t, segment, 0x00);
    CSF_AT(uint32_t, query, 0x08) = CSF_AT(uint32_t, segment, 0x04);
    CSF_AT(uint32_t, query, 0x0C) = CSF_AT(uint32_t, segment, 0x08);

    CSF_AT(float, query, 0x10) =
        CSF_AT(float, segment, 0x0C) - CSF_AT(float, segment, 0x00);
    CSF_AT(float, query, 0x14) =
        CSF_AT(float, segment, 0x10) - CSF_AT(float, segment, 0x04);
    CSF_AT(float, query, 0x18) =
        CSF_AT(float, segment, 0x14) - CSF_AT(float, segment, 0x08);

    const float inverseX = CSF_AT(float, query, 0x10) == g_fZero
        ? g_fZero : g_TransitionOne / CSF_AT(float, query, 0x10);
    CSF_AT(float, query, 0x1C) = inverseX;

    const float inverseY = CSF_AT(float, query, 0x14) == g_fZero
        ? 0.0f : g_TransitionOne / CSF_AT(float, query, 0x14);
    CSF_AT(float, query, 0x20) = inverseY;

    const float inverseZ = CSF_AT(float, query, 0x18) == g_fZero
        ? 0.0f : g_TransitionOne / CSF_AT(float, query, 0x18);
    CSF_AT(float, query, 0x24) = inverseZ;

    if (tolerance)
    {
        CSF_AT(uint32_t, query, 0x28) = tolerance[0];
        CSF_AT(uint32_t, query, 0x2C) = tolerance[1];
        CSF_AT(uint32_t, query, 0x30) = tolerance[2];
    }
    else
    {
        const float startX =
            GfxClipHelpers_AbsoluteCoordinate(CSF_AT(float, segment, 0x00));
        const float endX =
            GfxClipHelpers_AbsoluteCoordinate(CSF_AT(float, segment, 0x0C));
        const float maximumX = GfxClipHelpers_AbsoluteCoordinate(
            !(startX >= endX) ? CSF_AT(float, segment, 0x0C)
                             : CSF_AT(float, segment, 0x00));
        CSF_AT(float, query, 0x28) = maximumX;

        const float startY =
            GfxClipHelpers_AbsoluteCoordinate(CSF_AT(float, segment, 0x04));
        const float endY =
            GfxClipHelpers_AbsoluteCoordinate(CSF_AT(float, segment, 0x10));
        const float maximumY = GfxClipHelpers_AbsoluteCoordinate(
            !(startY >= endY) ? CSF_AT(float, segment, 0x10)
                             : CSF_AT(float, segment, 0x04));
        CSF_AT(float, query, 0x2C) = maximumY;

        const float startZ =
            GfxClipHelpers_AbsoluteCoordinate(CSF_AT(float, segment, 0x08));
        const float endZ =
            GfxClipHelpers_AbsoluteCoordinate(CSF_AT(float, segment, 0x14));
        const float maximumZ = GfxClipHelpers_AbsoluteCoordinate(
            !(startZ >= endZ) ? CSF_AT(float, segment, 0x14)
                             : CSF_AT(float, segment, 0x08));

        CSF_AT(float, query, 0x28) = maximumX * g_TriangleQueryToleranceScale;
        CSF_AT(float, query, 0x2C) = maximumY * g_TriangleQueryToleranceScale;
        CSF_AT(float, query, 0x30) = maximumZ * g_TriangleQueryToleranceScale;
    }

    CSF_AT(void*, query, 0x3C) = queryBytes + 0x34;
    CSF_AT(float, query, 0x40) = 0.0f;
    CSF_AT(float, query, 0x44) = 1.0f;
    CSF_AT(void*, query, 0x1C8) = queryBytes + 0x48;

    const float minimumX =
        CSF_AT(float, bounds, 0x10) - CSF_AT(float, query, 0x28);
    const float minimumY =
        CSF_AT(float, bounds, 0x14) - CSF_AT(float, query, 0x2C);
    const float minimumZ =
        CSF_AT(float, bounds, 0x18) - CSF_AT(float, query, 0x30);
    const float maximumX =
        CSF_AT(float, bounds, 0x04) + CSF_AT(float, query, 0x28);
    const float maximumY =
        CSF_AT(float, bounds, 0x08) + CSF_AT(float, query, 0x2C);
    const float maximumZ =
        CSF_AT(float, bounds, 0x0C) + CSF_AT(float, query, 0x30);

    if (inverseX == 0.0f)
    {
        if (CSF_AT(float, query, 0x04) < minimumX ||
            CSF_AT(float, query, 0x04) > maximumX)
            goto rejected;
    }
    else
    {
        float enter = (minimumX - CSF_AT(float, query, 0x04)) * inverseX;
        float leave = (maximumX - CSF_AT(float, query, 0x04)) * inverseX;
        if (leave < enter)
        {
            const float temporary = leave;
            leave = enter;
            enter = temporary;
        }
        if (leave < 0.0f || enter > 1.0f)
            goto rejected;

        CSF_AT(float, query, 0x40) = enter > 0.0f ? enter : 0.0f;
        CSF_AT(float, query, 0x44) = leave < 1.0f ? leave : 1.0f;
    }

    if (inverseY == 0.0f)
    {
        if (CSF_AT(float, query, 0x08) < minimumY ||
            CSF_AT(float, query, 0x08) > maximumY)
            goto rejected;
    }
    else
    {
        float enter = (minimumY - CSF_AT(float, query, 0x08)) * inverseY;
        float leave = (maximumY - CSF_AT(float, query, 0x08)) * inverseY;
        if (leave < enter)
        {
            const float temporary = leave;
            leave = enter;
            enter = temporary;
        }
        if (CSF_AT(float, query, 0x40) > leave ||
            CSF_AT(float, query, 0x44) < enter)
            goto rejected;

        CSF_AT(float, query, 0x40) =
            CSF_AT(float, query, 0x40) < enter
                ? enter : CSF_AT(float, query, 0x40);
        CSF_AT(float, query, 0x44) =
            CSF_AT(float, query, 0x44) > leave
                ? leave : CSF_AT(float, query, 0x44);
    }

    if (inverseZ == 0.0f)
    {
        if (CSF_AT(float, query, 0x0C) >= minimumZ)
        {
            if (CSF_AT(float, query, 0x0C) > maximumZ)
                goto rejected;
            return;
        }
        goto rejected;
    }
    else
    {
        float enter = (minimumZ - CSF_AT(float, query, 0x0C)) * inverseZ;
        float leave = (maximumZ - CSF_AT(float, query, 0x0C)) * inverseZ;
        if (leave < enter)
        {
            const float temporary = leave;
            leave = enter;
            enter = temporary;
        }
        if (CSF_AT(float, query, 0x40) > leave ||
            CSF_AT(float, query, 0x44) < enter)
            goto rejected;

        CSF_AT(float, query, 0x40) =
            CSF_AT(float, query, 0x40) < enter
                ? enter : CSF_AT(float, query, 0x40);
        CSF_AT(float, query, 0x44) =
            CSF_AT(float, query, 0x44) > leave
                ? leave : CSF_AT(float, query, 0x44);
    }
    return;

rejected:
    CSF_AT(void*, query, 0x1C8) = queryBytes + 0x3C;
}

HOOK(0x007C2110, GfxClipHelpers_InitializeTriangleQuery, "auto");
