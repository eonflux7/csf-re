#include <cstdint>
#include <cstring>
#include <cmath>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_FUNC(const void*, __fastcall, ResizePairBuffer, 0x0045BBA0, (void* self, void* , int extraCount));
CSF_FUNC(int, __fastcall, SetMarkedFlagAndIncrementCounter, 0x0075BF50, (void* self, void*));
CSF_FUNC(void, __fastcall, WaypointGraph_ClearMarks, 0x0075BF60, (void* self, void* edx));
CSF_FUNC(int, __fastcall, WaypointGraph_FindPoint, 0x0075CF20, (void* self, void* edx, uint32_t* a2));
CSF_FUNC(int, __fastcall, WaypointGraph_MarkPoint, 0x0075D1D0, (void* self, void* edx, uint32_t* a2));
CSF_FUNC(char, __fastcall, WaypointGraph_GrowSearchRecords, 0x0075F0B0, (void* self, void* edx, int a2));
CSF_FUNC(int*, __fastcall, WaypointGraph_EraseFrontierNode, 0x0075F1E0, (uint32_t* a1, int a2, int* a3, int a4));
CSF_FUNC(int*, __fastcall, WaypointGraph_ClearFrontierRange, 0x0075F4A0, (uint32_t* a1, int a2, int* a3, int a4, int a5));
CSF_FUNC(int, __fastcall, WaypointGraph_InsertFrontierRecord, 0x0075F700, (void* self, void* edx, int a2, float* a3));
CSF_FUNC(uint32_t*, __fastcall, WaypointGraph_ExpandSearchRecord, 0x0075F7C0, (void* self, void* edx, int a2, int a3, float* a4, int a5));

static int __fastcall S_0075f7c0_FindPath(
    S_0075f7c0* self, void* /*edx*/,
    float* start, uint32_t* destination, int outputAddress)
{
    alignas(8) uint8_t initialRecord[0x24];
    alignas(8) uint8_t iteratorResult[0x4];
    alignas(8) uint8_t insertionResult[0xC];
    alignas(8) uint8_t copiedRecord[0x24];

    auto* frontier = reinterpret_cast<uint32_t*>(
        reinterpret_cast<uint8_t*>(self) + 0x14);
    uint32_t head = self->field_0018;
    uint32_t first = CSF_AT(uint32_t, reinterpret_cast<void*>(head), 0);

    CSF_AT(uint8_t, insertionResult, 0xB) = 0;
    WaypointGraph_ClearFrontierRange(
        frontier, static_cast<int>(first),
        reinterpret_cast<int*>(iteratorResult),
        static_cast<int>(first), static_cast<int>(head));

    CSF_AT(uint32_t, self, 0xC) = 0;
    auto* startPosition = reinterpret_cast<float*>(
        static_cast<uint32_t>(WaypointGraph_FindPoint(
            reinterpret_cast<void*>(self->field_0000), nullptr,
            reinterpret_cast<uint32_t*>(start))) + 4);

    auto* destinationPosition = reinterpret_cast<float*>(
        static_cast<uint32_t>(WaypointGraph_FindPoint(
            reinterpret_cast<void*>(self->field_0000), nullptr, destination)) + 4);
    CSF_AT(float*, iteratorResult, 0) = destinationPosition;

    SetMarkedFlagAndIncrementCounter(
        reinterpret_cast<void*>(self->field_0000), nullptr);

    float dx = destinationPosition[0] - startPosition[0];
    CSF_AT(float, initialRecord, 0x18) = 0.0f;
    float dy = destinationPosition[1] - startPosition[1];
    float dz = destinationPosition[2] - startPosition[2];

    std::memcpy(initialRecord + 4, start, 8);
    CSF_AT(float, initialRecord, 0x1C) =
        std::sqrt(dz * dz + dy * dy + dx * dx);

    auto* position = reinterpret_cast<float*>(
        static_cast<uint32_t>(WaypointGraph_FindPoint(
            reinterpret_cast<void*>(self->field_0000), nullptr,
            reinterpret_cast<uint32_t*>(start))) + 4);
    std::memcpy(initialRecord + 0xC, position, 12);
    CSF_AT(uint32_t, initialRecord, 0) = 0xFFFFFFFF;
    CSF_AT(float, initialRecord, 0x20) = 0.0f;

    WaypointGraph_MarkPoint(
        reinterpret_cast<void*>(self->field_0000), nullptr,
        reinterpret_cast<uint32_t*>(start));
    WaypointGraph_InsertFrontierRecord(
        frontier, nullptr,
        static_cast<int>(reinterpret_cast<uintptr_t>(insertionResult)),
        reinterpret_cast<float*>(initialRecord));

    uint8_t* currentRecord = initialRecord;
    while (CSF_AT(uint32_t, self, 0x1C) != 0)
    {
        uint32_t node = CSF_AT(
            uint32_t, reinterpret_cast<void*>(self->field_0018), 0);
        std::memcpy(copiedRecord, reinterpret_cast<uint8_t*>(node) + 0xC, 0x24);
        start = reinterpret_cast<float*>(node);

        if (CSF_AT(uint32_t, self, 0xC) >= CSF_AT(uint32_t, self, 0x8))
        {
            WaypointGraph_GrowSearchRecords(
                &self->field_0004, nullptr,
                CSF_AT(int, self, 0x10));
        }

        uint32_t count = CSF_AT(uint32_t, self, 0xC);
        currentRecord = reinterpret_cast<uint8_t*>(self->field_0004 + 36 * count);
        CSF_AT(uint32_t, self, 0xC) = count + 1;
        std::memcpy(currentRecord, copiedRecord, 0x24);
        int recordIndex = static_cast<int>(CSF_AT(uint32_t, self, 0xC) - 1);

        WaypointGraph_EraseFrontierNode(
            frontier, static_cast<int>(reinterpret_cast<uintptr_t>(start)),
            reinterpret_cast<int*>(&start),
            static_cast<int>(reinterpret_cast<uintptr_t>(start)));

        if (CSF_AT(uint32_t, currentRecord, 4) == destination[0] &&
            CSF_AT(uint32_t, currentRecord, 8) == destination[1])
        {
            CSF_AT(uint8_t, insertionResult, 0xB) = 1;
            break;
        }

        CSF_AT(uint8_t, insertionResult, 0xB) = 0;
        WaypointGraph_ExpandSearchRecord(
            self, nullptr, recordIndex,
            static_cast<int>(reinterpret_cast<uintptr_t>(destination)),
            CSF_AT(float*, iteratorResult, 0),
            static_cast<int>(reinterpret_cast<uintptr_t>(frontier)));
    }

    WaypointGraph_ClearMarks(
        reinterpret_cast<void*>(self->field_0000), nullptr);

    if (CSF_AT(uint8_t, insertionResult, 0xB) != 0)
    {
        auto* output = reinterpret_cast<void*>(static_cast<uint32_t>(outputAddress));
        uint32_t length = CSF_AT(uint32_t, currentRecord, 0x20);
        uint32_t capacity = CSF_AT(uint32_t, output, 4);
        if (length > capacity)
            ResizePairBuffer(output, nullptr, static_cast<int>(length - capacity));
        CSF_AT(uint32_t, output, 8) = length;

        while (CSF_AT(uint32_t, currentRecord, 0) != 0xFFFFFFFF)
        {
            uint32_t firstKey = CSF_AT(uint32_t, currentRecord, 4);
            uint32_t step = CSF_AT(uint32_t, currentRecord, 0x20);
            uint32_t data = CSF_AT(uint32_t, output, 0);
            CSF_AT(uint32_t, reinterpret_cast<void*>(data + 8 * step - 8), 0) =
                firstKey;
            CSF_AT(uint32_t, reinterpret_cast<void*>(data + 8 * step - 4), 0) =
                CSF_AT(uint32_t, currentRecord, 8);

            uint32_t parent = CSF_AT(uint32_t, currentRecord, 0);
            currentRecord = reinterpret_cast<uint8_t*>(
                self->field_0004 + 36 * parent);
        }
    }

    return 0;
}

HOOK(0x0075FA10, S_0075f7c0_FindPath, "auto");
