#include <cstdint>
#include <cmath>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

CSF_FUNC(int, __fastcall, WaypointGraph_GetPoint, 0x0075CF20,
         (void* self, void* edx, uint32_t* a2));
CSF_FUNC(bool, __fastcall, WaypointGraph_IsPointMarked, 0x0075D1B0,
         (void* self, void* edx, uint32_t* a2));
CSF_FUNC(int, __fastcall, WaypointGraph_MarkPoint, 0x0075D1D0,
         (void* self, void* edx, uint32_t* a2));
CSF_FUNC(int, __fastcall, WaypointGraph_InsertFrontierRecord, 0x0075F700,
         (void* self, void* edx, int a2, float* a3));

static uint32_t* __fastcall S_0075f7c0_ExpandNeighbors(
    S_0075f7c0* self, void* /*edx*/, int pointIndex, int /*unused*/,
    float* destination, int frontierAddress)
{
    auto* current = reinterpret_cast<uint8_t*>(
        self->field_0004 + 36 * static_cast<uint32_t>(pointIndex));
    auto* graph = reinterpret_cast<C_0086c288*>(self->field_0000);
    auto* neighbors = reinterpret_cast<uint32_t*>(
        graph->vSlot28(reinterpret_cast<uint32_t*>(current + 4)));

    if (neighbors[2] == 0)
        return neighbors;

    alignas(8) uint8_t candidate[0x24];
    alignas(8) uint8_t insertionResult[0x14];
    CSF_AT(int, candidate, 0x00) = pointIndex;

    auto* frontier = reinterpret_cast<void*>(frontierAddress);
    uint32_t neighborIndex = 0;
    for (;;)
    {
        auto* key = CSF_AT(uint32_t*, neighbors, 0x00) + 2 * neighborIndex;
        auto* point = reinterpret_cast<uint8_t*>(
            WaypointGraph_GetPoint(
                reinterpret_cast<void*>(self->field_0000), nullptr, key));

        const float x = CSF_AT(float, point, 0x04);
        const float y = CSF_AT(float, point, 0x08);
        const float z = CSF_AT(float, point, 0x0C);
        CSF_AT(float, candidate, 0x0C) = x;
        CSF_AT(float, candidate, 0x10) = y;
        CSF_AT(float, candidate, 0x14) = z;

        const float dx = x - CSF_AT(float, current, 0x0C);
        const float dy = y - CSF_AT(float, current, 0x10);
        const float dz = z - CSF_AT(float, current, 0x14);
        const uint32_t depth = CSF_AT(uint32_t, current, 0x20);
        const uint32_t secondKey = key[1];
        CSF_AT(uint32_t, candidate, 0x04) = key[0];
        CSF_AT(uint32_t, candidate, 0x08) = secondKey;
        CSF_AT(uint32_t, candidate, 0x20) = depth + 1;
        CSF_AT(float, candidate, 0x18) =
            std::sqrt(dz * dz + dy * dy + dx * dx) +
            CSF_AT(float, current, 0x18);

        const float goalX = destination[0] - x;
        const float goalY = destination[1] - y;
        const float goalZ = destination[2] - z;
        CSF_AT(float, candidate, 0x1C) =
            std::sqrt(goalZ * goalZ + goalY * goalY + goalX * goalX);

        bool insert = false;
        if (!WaypointGraph_IsPointMarked(
                reinterpret_cast<void*>(self->field_0000), nullptr, key))
        {
            WaypointGraph_MarkPoint(
                reinterpret_cast<void*>(self->field_0000), nullptr, key);
            insert = true;
        }
        else
        {
            const uint32_t closedCount = CSF_AT(uint32_t, self, 0x0C);
            if (closedCount != 0)
            {
                const uint32_t firstKey = key[0];
                auto* closed = reinterpret_cast<uint8_t*>(self->field_0004);
                for (uint32_t index = 0; index < closedCount; ++index, closed += 36)
                {
                    // The original compares the saved key to the neighbor,
                    // not to the key in the closed record.
                    if (CSF_AT(uint32_t, candidate, 0x04) == firstKey &&
                        secondKey == key[1])
                    {
                        insert = CSF_AT(float, candidate, 0x18) <
                                 CSF_AT(float, closed, 0x18);
                        break;
                    }
                }
            }

            if (!insert)
            {
                void* sentinel = CSF_AT(void*, frontier, 0x04);
                void* node = CSF_AT(void*, sentinel, 0x00);
                if (node != sentinel)
                {
                    const uint32_t firstKey = key[0];
                    for (;;)
                    {
                        if (CSF_AT(uint32_t, node, 0x10) == firstKey &&
                            CSF_AT(uint32_t, node, 0x14) == key[1])
                        {
                            insert = CSF_AT(float, candidate, 0x18) <
                                     CSF_AT(float, node, 0x24);
                            break;
                        }

                        if (CSF_AT(uint8_t, node, 0x31) == 0)
                        {
                            void* right = CSF_AT(void*, node, 0x08);
                            if (CSF_AT(uint8_t, right, 0x31) == 0)
                            {
                                node = right;
                                void* left = CSF_AT(void*, node, 0x00);
                                while (CSF_AT(uint8_t, left, 0x31) == 0)
                                {
                                    node = left;
                                    left = CSF_AT(void*, node, 0x00);
                                }
                            }
                            else
                            {
                                void* parent = CSF_AT(void*, node, 0x04);
                                while (CSF_AT(uint8_t, parent, 0x31) == 0)
                                {
                                    if (node != CSF_AT(void*, parent, 0x08))
                                        break;
                                    node = parent;
                                    parent = CSF_AT(void*, parent, 0x04);
                                }
                                node = parent;
                            }
                        }
                        if (node == sentinel)
                            break;
                    }
                }
            }
        }

        if (insert)
        {
            WaypointGraph_InsertFrontierRecord(
                frontier, nullptr,
                static_cast<int>(reinterpret_cast<uintptr_t>(insertionResult)),
                reinterpret_cast<float*>(candidate));
        }

        const uint32_t count = neighbors[2];
        ++neighborIndex;
        if (neighborIndex >= count)
            return reinterpret_cast<uint32_t*>(
                static_cast<uintptr_t>(neighborIndex));
    }
}

HOOK(0x0075F7C0, S_0075f7c0_ExpandNeighbors, "auto");
