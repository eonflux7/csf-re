#include <cstdint>
#include <cmath>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_FUNC(char, __fastcall, S_004ddf20_GetCellCoordinates, 0x004DDF80,
         (void* self, void* edx, float a2, float a3, uint32_t* a4, uint32_t* a5));
CSF_FUNC(bool, __fastcall, ActorTargetMatchesPoint, 0x004E0BF0,
         (void* self, void* edx, float* a2));
CSF_FUNC(double, __fastcall, SubObjectDistanceToPoint, 0x004E0F60,
         (void* self, void* edx, float* a2));

static int __fastcall S_004ddf20_FindTargetAtPoint(
    S_004ddf20* self, void* /*edx*/, unsigned int pointAddress)
{
    float* point = reinterpret_cast<float*>(pointAddress);
    if (!(point[0] >= self->field_000c) ||
        !(point[0] <= self->field_0010) ||
        !(point[2] >= self->field_0014) ||
        !(point[2] <= self->field_0018))
    {
        return 0;
    }

    uint32_t column;
    uint32_t row;
    S_004ddf20_GetCellCoordinates(
        self, nullptr, point[0], point[2], &column, &row);

    uint8_t* cell = reinterpret_cast<uint8_t*>(
        self->field_001c + 16 * (column + row * self->field_0004));

    S_004e0c20* matches[50];
    uint32_t matchCount = 0;
    uint32_t index = 0;

    if (CSF_AT(uint32_t, cell, 0x8) > 0)
    {
        do
        {
            S_004e0c20* candidate =
                CSF_AT(S_004e0c20**, cell, 0x0)[index];
            if (ActorTargetMatchesPoint(candidate, nullptr, point))
                matches[matchCount++] = candidate;

            ++index;
        }
        while (index < CSF_AT(uint32_t, cell, 0x8));

        if (matchCount == 1)
            return reinterpret_cast<int>(matches[0]);
    }

    float nearestDistance = 100000000.0f;
    S_004e0c20* nearest = nullptr;
    for (uint32_t i = 0; i < matchCount; ++i)
    {
        S_004e0c20* candidate = matches[i];
        double distance = std::fabs(
            SubObjectDistanceToPoint(candidate, nullptr, point));
        if (distance < nearestDistance)
        {
            nearestDistance = static_cast<float>(distance);
            nearest = candidate;
        }
    }

    return reinterpret_cast<int>(nearest);
}

HOOK(0x004DE100, S_004ddf20_FindTargetAtPoint, "auto");
