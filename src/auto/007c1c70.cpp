#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"

static CSF_GLOBAL(float, g_fZero, 0x0082D4CC);
static CSF_GLOBAL(float, g_TransitionOne, 0x0082D4D4);
static CSF_GLOBAL(float, g_SectorQueryToleranceScale, 0x00845DB0);

static float GfxClipHelpers_CoordinateMagnitude(float coordinate)
{
    return coordinate >= g_fZero ? coordinate : -coordinate;
}

static int __cdecl GfxClipHelpers_InitializeSectorQuery(
    void* query,
    const void* sector,
    const void* segment,
    const uint32_t* tolerance)
{
    CSF_AT(uint32_t, query, 0x00) = CSF_AT(uint32_t, segment, 0x00);
    CSF_AT(uint32_t, query, 0x04) = CSF_AT(uint32_t, segment, 0x04);
    CSF_AT(uint32_t, query, 0x08) = CSF_AT(uint32_t, segment, 0x08);

    CSF_AT(float, query, 0x0C) =
        CSF_AT(float, segment, 0x0C) - CSF_AT(float, segment, 0x00);
    CSF_AT(float, query, 0x10) =
        CSF_AT(float, segment, 0x10) - CSF_AT(float, segment, 0x04);
    CSF_AT(float, query, 0x14) =
        CSF_AT(float, segment, 0x14) - CSF_AT(float, segment, 0x08);

    CSF_AT(float, query, 0x18) = CSF_AT(float, query, 0x0C) == g_fZero
        ? g_fZero : g_TransitionOne / CSF_AT(float, query, 0x0C);
    CSF_AT(float, query, 0x1C) = CSF_AT(float, query, 0x10) == g_fZero
        ? g_fZero : g_TransitionOne / CSF_AT(float, query, 0x10);
    CSF_AT(float, query, 0x20) = CSF_AT(float, query, 0x14) == g_fZero
        ? g_fZero : g_TransitionOne / CSF_AT(float, query, 0x14);

    if (tolerance)
    {
        CSF_AT(uint32_t, query, 0x24) = tolerance[0];
        CSF_AT(uint32_t, query, 0x28) = tolerance[1];
        CSF_AT(uint32_t, query, 0x2C) = tolerance[2];
    }
    else
    {
        float startMagnitude =
            GfxClipHelpers_CoordinateMagnitude(CSF_AT(float, segment, 0x00));
        float endMagnitude =
            GfxClipHelpers_CoordinateMagnitude(CSF_AT(float, segment, 0x0C));
        const float xMagnitude = GfxClipHelpers_CoordinateMagnitude(
            !(startMagnitude >= endMagnitude)
                ? CSF_AT(float, segment, 0x0C)
                : CSF_AT(float, segment, 0x00));
        CSF_AT(float, query, 0x24) = xMagnitude;

        startMagnitude =
            GfxClipHelpers_CoordinateMagnitude(CSF_AT(float, segment, 0x04));
        endMagnitude =
            GfxClipHelpers_CoordinateMagnitude(CSF_AT(float, segment, 0x10));
        const float yMagnitude = GfxClipHelpers_CoordinateMagnitude(
            !(startMagnitude >= endMagnitude)
                ? CSF_AT(float, segment, 0x10)
                : CSF_AT(float, segment, 0x04));
        CSF_AT(float, query, 0x28) = yMagnitude;

        startMagnitude =
            GfxClipHelpers_CoordinateMagnitude(CSF_AT(float, segment, 0x08));
        endMagnitude =
            GfxClipHelpers_CoordinateMagnitude(CSF_AT(float, segment, 0x14));
        const float zMagnitude = GfxClipHelpers_CoordinateMagnitude(
            !(startMagnitude >= endMagnitude)
                ? CSF_AT(float, segment, 0x14)
                : CSF_AT(float, segment, 0x08));

        CSF_AT(float, query, 0x24) = xMagnitude * g_SectorQueryToleranceScale;
        CSF_AT(float, query, 0x28) = yMagnitude * g_SectorQueryToleranceScale;
        CSF_AT(float, query, 0x2C) = zMagnitude * g_SectorQueryToleranceScale;
    }

    const int result = CSF_AT(int, sector, 0x1C);
    CSF_AT(int, query, 0x30) = result;
    CSF_AT(uint32_t, query, 0x34) = 0;
    CSF_AT(uint32_t, query, 0x38) = 1065353216;
    CSF_AT(uint8_t*, query, 0x33C) =
        reinterpret_cast<uint8_t*>(query) + 60;
    return result;
}

HOOK(0x007C1C70, GfxClipHelpers_InitializeSectorQuery, "auto");
