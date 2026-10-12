#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

CSF_FUNC(float*, __fastcall, GetObjectPositionVector, 0x0048E430,
         (void* self, void* edx));
CSF_FUNC(uint32_t*, __fastcall, AiSearchOrders_InitializePointFromWaypoint, 0x004A9DE0,
         (void* self, void* edx, uint32_t* a2));
CSF_FUNC(uint32_t*, __fastcall, AiDistracted_GrowPointList, 0x004ADDB0,
         (void* self, void* edx, int a2));
CSF_FUNC(int, __cdecl, GetValue8B0554, 0x004E8C50, ());
CSF_FUNC(int, __cdecl, Mem_FreeDebug, 0x00690E60, (void* block));
CSF_FUNC(int, __fastcall, WaypointGraph_FindPointsInRadiusRange, 0x0075DCD0,
         (void* self, void* , int groupId, float* position, float minimumRadius, float maximumRadius, uint32_t* results));

static char __fastcall S_004cb490_AppendSearchPointsInRadiusRange(
    S_004cb490* self, void* /*edx*/, int groupId,
    float minimumRadius, float maximumRadius, int* points)
{
    alignas(8) uint8_t pointBytes[0x14];
    alignas(8) uint8_t resultsBytes[0x10];

    CSF_AT(uint32_t, resultsBytes, 0x04) = 0;
    CSF_AT(uint32_t, resultsBytes, 0x08) = 0;
    CSF_AT(uint32_t, resultsBytes, 0x0C) = 0;

    void* context = reinterpret_cast<void*>(
        static_cast<uint32_t>(GetValue8B0554()));
    void* graph = CSF_AT(void*, context, 0x1FD8);
    float* position = GetObjectPositionVector(
        reinterpret_cast<void*>(self->field_002c), nullptr);

    char found = static_cast<char>(WaypointGraph_FindPointsInRadiusRange(
        graph, nullptr, groupId, position, minimumRadius, maximumRadius,
        reinterpret_cast<uint32_t*>(resultsBytes)));

    if (found)
    {
        uint32_t pointCount = 0;
        if (CSF_AT(void*, resultsBytes, 0x04))
        {
            uint32_t byteCount =
                CSF_AT(uint32_t, resultsBytes, 0x08) -
                CSF_AT(uint32_t, resultsBytes, 0x04);
            pointCount = static_cast<uint32_t>(
                static_cast<int32_t>(byteCount) >> 3);
        }

        uint32_t capacity = static_cast<uint32_t>(points[1]);
        if (pointCount > capacity)
        {
            AiDistracted_GrowPointList(
                points, nullptr, static_cast<int>(pointCount - capacity));
        }

        for (uint32_t i = 0; i < pointCount; ++i)
        {
            auto* waypoint = reinterpret_cast<uint32_t*>(
                CSF_AT(uint8_t*, resultsBytes, 0x04) + 8 * i);
            AiSearchOrders_InitializePointFromWaypoint(
                pointBytes, nullptr, waypoint);

            if (static_cast<uint32_t>(points[2]) >=
                static_cast<uint32_t>(points[1]))
            {
                AiDistracted_GrowPointList(points, nullptr, points[3]);
            }

            uint32_t index = static_cast<uint32_t>(points[2]);
            auto* destination = reinterpret_cast<uint8_t*>(
                static_cast<uint32_t>(points[0])) + 20 * index;
            points[2] = static_cast<int32_t>(index + 1);

            CSF_AT(uint32_t, destination, 0x00) = CSF_AT(uint32_t, pointBytes, 0x00);
            CSF_AT(uint32_t, destination, 0x04) = CSF_AT(uint32_t, pointBytes, 0x04);
            CSF_AT(uint32_t, destination, 0x08) = CSF_AT(uint32_t, pointBytes, 0x08);
            CSF_AT(uint32_t, destination, 0x0C) = CSF_AT(uint32_t, pointBytes, 0x0C);
            CSF_AT(uint32_t, destination, 0x10) = CSF_AT(uint32_t, pointBytes, 0x10);
        }
    }

    if (CSF_AT(void*, resultsBytes, 0x04))
        Mem_FreeDebug(CSF_AT(void*, resultsBytes, 0x04));

    return found;
}

HOOK(0x004CB490, S_004cb490_AppendSearchPointsInRadiusRange, "auto");
