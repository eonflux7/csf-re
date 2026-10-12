#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_FUNC(int, __fastcall, GetLinkedObjectValue_4DB5C0, 0x004DB5C0, (void* self, void*));
CSF_FUNC(float*, __fastcall, GetObjectPositionVector, 0x0048E430, (void* self, void* edx));
CSF_FUNC(uint32_t*, __fastcall, AiCoverAttackTasks_GetPositionData, 0x004A9CA0, (void* self, void* edx));

CSF_GLOBAL(int, g_AiSearchCostThisFrame, 0x008B044C);

static uint32_t __fastcall S_004abb90_FindClosestPositionIndex(
    S_004abb90* self, void* /*edx*/, int object)
{
    uint32_t linkedAddress = static_cast<uint32_t>(
        GetLinkedObjectValue_4DB5C0(reinterpret_cast<void*>(object), nullptr));
    if (linkedAddress)
        linkedAddress -= 4;

    float* objectPosition = GetObjectPositionVector(
        CSF_AT(C_008342e8*, reinterpret_cast<void*>(linkedAddress), 0x2C),
        nullptr);

    float* position = reinterpret_cast<float*>(
        AiCoverAttackTasks_GetPositionData(
            CSF_AT(void*, self, 0x28), nullptr));

    float dx = position[0] - objectPosition[0];
    float dy = position[1] - objectPosition[1];
    float dz = position[2] - objectPosition[2];
    const uint32_t positionCount = CSF_AT(uint32_t, self, 0x30);
    float closestDistanceSquared = dz * dz + dy * dy + dx * dx;
    uint32_t closestIndex = 0;

    uint32_t positionOffset = 20;
    for (uint32_t index = 1; index < positionCount; ++index)
    {
        position = reinterpret_cast<float*>(
            AiCoverAttackTasks_GetPositionData(
                CSF_AT(uint8_t*, self, 0x28) + positionOffset, nullptr));

        dx = position[0] - objectPosition[0];
        dy = position[1] - objectPosition[1];
        dz = position[2] - objectPosition[2];
        const float distanceSquared = dz * dz + dy * dy + dx * dx;

        if (distanceSquared < closestDistanceSquared)
        {
            closestDistanceSquared = distanceSquared;
            closestIndex = index;
        }

        positionOffset += 20;
    }

    ++g_AiSearchCostThisFrame;
    return closestIndex;
}

HOOK(0x004B0770, S_004abb90_FindClosestPositionIndex, "auto");
