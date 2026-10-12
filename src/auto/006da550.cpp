#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

CSF_FUNC(void*, __fastcall, Polygon_GrowEdgeArray, 0x006D9D80, (void* self, void* edx, int a2));

static uint32_t* __fastcall Polygon_InsertEdgeAtCursor(
    void* self, void* /*edx*/, uint32_t* edgeData)
{
    void* edgeArray = CSF_AT(void*, self, 0x4);
    const uint32_t newIndex = CSF_AT(uint32_t, edgeArray, 0x8);
    if (newIndex >= CSF_AT(uint32_t, edgeArray, 0x4))
        Polygon_GrowEdgeArray(
            edgeArray, nullptr, CSF_AT(int, edgeArray, 0xC));

    const uint32_t count = CSF_AT(uint32_t, edgeArray, 0x8);
    uint8_t* storage = CSF_AT(uint8_t*, edgeArray, 0x0);
    const uint32_t allocationOffset = 16 * count;
    CSF_AT(uint32_t, edgeArray, 0x8) = count + 1;
    CSF_AT(uint32_t, storage, allocationOffset) = 0;
    CSF_AT(uint32_t, storage, allocationOffset + 0x4) = 0;
    CSF_AT(uint32_t, storage, allocationOffset + 0x8) = 0;
    CSF_AT(uint32_t, storage, allocationOffset + 0xC) = 0;

    edgeArray = CSF_AT(void*, self, 0x4);
    const uint32_t firstValue = edgeData[0];
    storage = CSF_AT(uint8_t*, edgeArray, 0x0);
    const uint32_t newOffset = 16 * newIndex;
    CSF_AT(uint32_t, storage, newOffset) = firstValue;
    CSF_AT(uint32_t, storage, newOffset + 0x4) = edgeData[1];

    if (newIndex == 0)
    {
        CSF_AT(uint32_t, CSF_AT(void*, self, 0x4), 0x10) = 0;
        CSF_AT(uint32_t, CSF_AT(void*, self, 0x4), 0x14) = 0;

        storage = CSF_AT(uint8_t*, CSF_AT(void*, self, 0x4), 0x0);
        CSF_AT(int32_t, storage, newOffset + 0x8) = -1;
        storage = CSF_AT(uint8_t*, CSF_AT(void*, self, 0x4), 0x0);
        CSF_AT(int32_t, storage, newOffset + 0xC) = -1;

        // The original returns the byte offset on this path, not an edge pointer.
        return reinterpret_cast<uint32_t*>(newOffset);
    }

    const int32_t cursor = CSF_AT(int32_t, self, 0x0);
    if (cursor == -1)
    {
        storage = CSF_AT(uint8_t*, CSF_AT(void*, self, 0x4), 0x0);
        CSF_AT(int32_t, storage, newOffset + 0x8) = -1;

        edgeArray = CSF_AT(void*, self, 0x4);
        storage = CSF_AT(uint8_t*, edgeArray, 0x0);
        const uint32_t tail = CSF_AT(uint32_t, edgeArray, 0x14);
        CSF_AT(uint32_t, storage, newOffset + 0xC) = tail;

        edgeArray = CSF_AT(void*, self, 0x4);
        const uint32_t previousTail = CSF_AT(uint32_t, edgeArray, 0x14);
        storage = CSF_AT(uint8_t*, edgeArray, 0x0);
        CSF_AT(uint32_t, storage, 16 * previousTail + 0x8) = newIndex;
        CSF_AT(uint32_t, CSF_AT(void*, self, 0x4), 0x14) = newIndex;
        return reinterpret_cast<uint32_t*>(storage);
    }

    storage = CSF_AT(uint8_t*, CSF_AT(void*, self, 0x4), 0x0);
    CSF_AT(int32_t, storage, newOffset + 0x8) = cursor;

    storage = CSF_AT(uint8_t*, CSF_AT(void*, self, 0x4), 0x0);
    const uint32_t cursorOffset =
        16 * CSF_AT(uint32_t, self, 0x0);
    const uint32_t previous =
        CSF_AT(uint32_t, storage, cursorOffset + 0xC);
    CSF_AT(uint32_t, storage, newOffset + 0xC) = previous;

    const uint32_t currentOffset =
        16 * CSF_AT(uint32_t, self, 0x0);
    storage = CSF_AT(uint8_t*, CSF_AT(void*, self, 0x4), 0x0);
    if (CSF_AT(int32_t, storage, currentOffset + 0xC) == -1)
    {
        CSF_AT(uint32_t, CSF_AT(void*, self, 0x4), 0x10) = newIndex;
    }
    else
    {
        edgeArray = CSF_AT(void*, self, 0x4);
        storage = CSF_AT(uint8_t*, edgeArray, 0x0);
        const uint32_t previousIndex =
            CSF_AT(uint32_t, storage, currentOffset + 0xC);
        CSF_AT(uint32_t, storage, 16 * previousIndex + 0x8) = newIndex;
    }

    const uint32_t finalCursor = CSF_AT(uint32_t, self, 0x0);
    edgeArray = CSF_AT(void*, self, 0x4);
    storage = CSF_AT(uint8_t*, edgeArray, 0x0);
    CSF_AT(uint32_t, storage, 16 * finalCursor + 0xC) = newIndex;
    return reinterpret_cast<uint32_t*>(edgeArray);
}

HOOK(0x006DA550, Polygon_InsertEdgeAtCursor, "auto");
