#include <cstdint>
#include <cstring>
#include <cmath>
#include "globals.h"
#include "hook.h"

CSF_GLOBAL(float, g_fZero, 0x0082D4CC);
CSF_GLOBAL(float, g_ScoreboardRefreshInterval, 0x0082DAF0);

CSF_FUNC(void, __cdecl, ClampFloatToBounds, 0x00406B60, (float* value, float* lowerBound, float* upperBound));
CSF_FUNC(float*, __cdecl, SelectMovementRateBound, 0x0040A640, (float* a1, float* a2));
CSF_FUNC(int, __fastcall, MovementRates_GrowSegmentArray, 0x0040AD70, (void* self, void* edx, int a2));

static int __stdcall MovementRates_AppendSegment(void* movement, void* segment)
{
    const uint32_t firstValueBits = CSF_AT(uint32_t, segment, 0x00);
    float firstValue;
    std::memcpy(&firstValue, &firstValueBits, sizeof(firstValue));

    float rate = CSF_AT(float, segment, 0x08);
    float start = CSF_AT(float, segment, 0x0C);

    if (CSF_AT(uint32_t, movement, 0x14) != 0 &&
        CSF_AT(float, segment, 0x08) != g_fZero)
    {
        if (CSF_AT(int, segment, 0x0C) == -1082130432)
        {
            auto* previous = reinterpret_cast<uint8_t*>(
                CSF_AT(uint32_t, movement, 0x0C) +
                16u * CSF_AT(uint32_t, movement, 0x14) - 16u);

            if (CSF_AT(float, previous, 0x08) == g_fZero)
            {
                start = CSF_AT(float, previous, 0x04);
            }
            else
            {
                const float coefficient =
                    CSF_AT(float, previous, 0x08) * g_fZero;
                const float previousStart = CSF_AT(float, previous, 0x0C);
                const float discriminantRoot = std::sqrt(
                    previousStart * previousStart -
                    -CSF_AT(float, previous, 0x00) *
                        coefficient * g_ScoreboardRefreshInterval);

                float firstRoot =
                    (discriminantRoot - previousStart) /
                    (coefficient + coefficient);
                float secondRoot =
                    (-previousStart - discriminantRoot) /
                    (coefficient + coefficient);

                float selectedRoot =
                    firstRoot < secondRoot ? firstRoot : secondRoot;
                if (selectedRoot < g_fZero)
                    selectedRoot = *SelectMovementRateBound(&firstRoot, &secondRoot);

                secondRoot = selectedRoot * CSF_AT(float, segment, 0x08) +
                             CSF_AT(float, segment, 0x0C);
                ClampFloatToBounds(
                    &secondRoot,
                    &CSF_AT(float, segment, 0x0C),
                    &CSF_AT(float, segment, 0x04));
                start = secondRoot;
            }
        }
        else
        {
            start = CSF_AT(float, segment, 0x0C);
        }
    }

    const bool reverseTarget =
        firstValue * CSF_AT(float, segment, 0x04) < g_fZero;
    float target = CSF_AT(float, segment, 0x04);
    if (reverseTarget)
        target = -target;

    if (rate == g_fZero)
    {
        start = target;
    }
    else if ((target - start) * CSF_AT(float, segment, 0x08) < g_fZero)
    {
        rate = -CSF_AT(float, segment, 0x08);
    }
    else
    {
        rate = CSF_AT(float, segment, 0x08);
    }

    if (CSF_AT(uint32_t, movement, 0x14) >=
        CSF_AT(uint32_t, movement, 0x10))
    {
        MovementRates_GrowSegmentArray(
            static_cast<uint8_t*>(movement) + 0x0C,
            nullptr,
            CSF_AT(int, movement, 0x18));
    }

    const uint32_t count = CSF_AT(uint32_t, movement, 0x14);
    auto* destination = reinterpret_cast<uint8_t*>(
        CSF_AT(uint32_t, movement, 0x0C) + 16u * count);
    CSF_AT(uint32_t, movement, 0x14) = count + 1;

    CSF_AT(uint32_t, destination, 0x00) = firstValueBits;
    CSF_AT(float, destination, 0x04) = target;
    CSF_AT(float, destination, 0x08) = rate;
    CSF_AT(float, destination, 0x0C) = start;

    if (CSF_AT(uint32_t, movement, 0x14) == 1)
    {
        CSF_AT(float, movement, 0x00) = start;
        CSF_AT(uint32_t, movement, 0x04) = 0;
        CSF_AT(uint32_t, movement, 0x08) = firstValueBits;

        int result;
        std::memcpy(&result, &start, sizeof(result));
        return result;
    }

    return static_cast<int>(reinterpret_cast<uintptr_t>(destination));
}

HOOK(0x0040B240, MovementRates_AppendSegment, "auto");
