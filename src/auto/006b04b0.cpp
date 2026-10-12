#include <cstdint>
#include <cstddef>
#include <cstring>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

CSF_FUNC(void*, __fastcall, S_0041e5f0_GrowArrayCapacity, 0x0041E5F0, (S_0041e5f0* self, void* , int additionalCapacity));
CSF_FUNC(void*, __cdecl, Mem_Alloc, 0x006910F0, (std::size_t size));
CSF_FUNC(int, __cdecl, Mem_Free, 0x00691110, (void* block));
CSF_FUNC(bool, __cdecl, GfxMatrix_IsPointInsideTriangle_6AD120, 0x006AD120, (float* point, float* vertexB, float* vertexC, float* vertexD));
CSF_FUNC(void*, __fastcall, Polygon_GrowEdgeArray, 0x006D9D80, (void* self, void* edx, int a2));
CSF_FUNC(void*, __cdecl, _memcpy, 0x007AEE60, (void* destination, const void* source, std::size_t byteCount));

CSF_GLOBAL(float, g_fZero, 0x0082D4CC);

static void Polygon_ReserveDwordSlot(void* array)
{
    const std::uint32_t capacity = CSF_AT(std::uint32_t, array, 0x04);
    if (CSF_AT(std::uint32_t, array, 0x08) >= capacity)
    {
        const std::uint32_t increment = CSF_AT(std::uint32_t, array, 0x0C);
        void* replacement = Mem_Alloc(4 * (capacity + increment));
        const std::uint32_t count = CSF_AT(std::uint32_t, array, 0x08);
        if (count)
            std::memcpy(replacement, CSF_AT(void*, array, 0x00), 4 * count);

        void* oldStorage = CSF_AT(void*, array, 0x00);
        CSF_AT(std::uint32_t, array, 0x04) += increment;
        if (oldStorage)
        {
            Mem_Free(oldStorage);
            CSF_AT(void*, array, 0x00) = nullptr;
        }
        CSF_AT(void*, array, 0x00) = replacement;
    }
}

static void Polygon_AppendDword(void* array, std::uint32_t value)
{
    Polygon_ReserveDwordSlot(array);
    const std::uint32_t count = CSF_AT(std::uint32_t, array, 0x08);
    auto* destination = CSF_AT(std::uint32_t*, array, 0x00) + count;
    CSF_AT(std::uint32_t, array, 0x08) = count + 1;
    *destination = value;
}

static void* Polygon_CreateTriangle()
{
    void* triangle = Mem_Alloc(24);
    if (triangle)
    {
        CSF_AT(void*, triangle, 0x00) = nullptr;
        CSF_AT(std::uint32_t, triangle, 0x04) = 0;
        CSF_AT(std::uint32_t, triangle, 0x08) = 0;
        CSF_AT(std::uint32_t, triangle, 0x0C) = 10;
        CSF_AT(std::uint32_t, triangle, 0x10) = 0;
        CSF_AT(std::uint32_t, triangle, 0x14) = 0;
    }
    return triangle;
}

static bool Polygon_TriangleContainsIndex(void* triangle, std::uint32_t vertex)
{
    const std::uint32_t count = CSF_AT(std::uint32_t, triangle, 0x08);
    if (count)
    {
        auto* indices = CSF_AT(std::uint32_t*, triangle, 0x00);
        for (std::uint32_t index = 0; index < count; ++index)
        {
            if (indices[index] == vertex)
                return true;
        }
    }
    return false;
}

static int __fastcall Polygon_TriangulateAndLinkNeighbors(
    void* self, void* /*edx*/, void* edges)
{
    // The packet also identifies the capacity routine as a vtable reference.
    (void)CSF_VTBL(0x0041E5F0);

    alignas(8) std::uint8_t remainingBytes[0x38];
    CSF_AT(void*, remainingBytes, 0x00) = nullptr;
    CSF_AT(std::uint32_t, remainingBytes, 0x04) = 0;
    CSF_AT(std::uint32_t, remainingBytes, 0x08) = 0;
    CSF_AT(std::uint32_t, remainingBytes, 0x0C) = 10;

    const std::uint32_t vertexCount = CSF_AT(std::uint32_t, self, 0x08);
    if (vertexCount)
    {
        S_0041e5f0_GrowArrayCapacity(
            reinterpret_cast<S_0041e5f0*>(remainingBytes),
            nullptr, static_cast<int>(vertexCount));
    }

    CSF_AT(std::uint32_t, remainingBytes, 0x08) = vertexCount;
    for (std::uint32_t index = 0; index < vertexCount; ++index)
        CSF_AT(std::uint32_t*, remainingBytes, 0x00)[index] = index;

    std::int32_t remainingCount = static_cast<std::int32_t>(vertexCount);
    if (vertexCount > 3)
    {
        do
        {
            auto* indices = CSF_AT(std::uint32_t*, remainingBytes, 0x00);
            std::int32_t ear = -1;

            for (std::int32_t candidate = 0;
                 candidate < remainingCount && ear == -1; ++candidate)
            {
                const std::int32_t previous =
                    (candidate + 1 + remainingCount - 2) % remainingCount;
                const std::int32_t next = (candidate + 1) % remainingCount;

                auto* vertices = CSF_AT(std::uint8_t*, self, 0x00);
                auto* currentPoint =
                    reinterpret_cast<float*>(vertices + 8 * indices[candidate]);
                auto* previousPoint =
                    reinterpret_cast<float*>(vertices + 8 * indices[previous]);
                auto* nextPoint =
                    reinterpret_cast<float*>(vertices + 8 * indices[next]);

                const float incomingX = currentPoint[0] - previousPoint[0];
                const float incomingY = currentPoint[1] - previousPoint[1];
                const float outgoingX = nextPoint[0] - currentPoint[0];
                const float outgoingY = nextPoint[1] - currentPoint[1];

                if (outgoingX * incomingY - outgoingY * incomingX < g_fZero)
                {
                    bool containsPoint = false;
                    for (std::int32_t point = 0;
                         point < remainingCount && !containsPoint; ++point)
                    {
                        if (point != previous && point != candidate && point != next)
                        {
                            auto* position = reinterpret_cast<float*>(
                                CSF_AT(std::uint8_t*, self, 0x00) + 8 * indices[point]);
                            containsPoint = GfxMatrix_IsPointInsideTriangle_6AD120(
                                position, previousPoint, currentPoint, nextPoint);
                        }
                    }
                    if (!containsPoint)
                        ear = candidate;
                }
            }

            const std::uint32_t previousVertex =
                indices[(remainingCount + ear - 1) % remainingCount];
            const std::uint32_t earVertex = indices[ear];
            auto* removedSlot = indices + ear;
            const std::uint32_t nextVertex =
                indices[(ear + 1) % remainingCount];

            void* triangle = Polygon_CreateTriangle();
            Polygon_AppendDword(triangle, previousVertex);
            Polygon_AppendDword(triangle, earVertex);
            Polygon_AppendDword(triangle, nextVertex);
            Polygon_AppendDword(
                static_cast<std::uint8_t*>(self) + 0x10,
                static_cast<std::uint32_t>(reinterpret_cast<std::uintptr_t>(triangle)));

            if (CSF_AT(std::uint32_t, edges, 0x08) >=
                CSF_AT(std::uint32_t, edges, 0x04))
            {
                Polygon_GrowEdgeArray(
                    edges, nullptr, CSF_AT(int, edges, 0x0C));
            }

            const std::uint32_t edgeCount = CSF_AT(std::uint32_t, edges, 0x08);
            auto* edge = CSF_AT(std::uint8_t*, edges, 0x00) + 16 * edgeCount;
            CSF_AT(std::uint32_t, edges, 0x08) = edgeCount + 1;
            CSF_AT(std::uint32_t, edge, 0x00) = previousVertex;
            CSF_AT(std::uint32_t, edge, 0x04) = nextVertex;
            CSF_AT(void*, edge, 0x08) = triangle;
            CSF_AT(void*, edge, 0x0C) = nullptr;

            --remainingCount;
            CSF_AT(std::uint32_t, remainingBytes, 0x08) =
                static_cast<std::uint32_t>(remainingCount);
            if (ear != remainingCount)
                _memcpy(removedSlot, removedSlot + 1, 4 * (remainingCount - ear));
        }
        while (static_cast<std::uint32_t>(remainingCount) > 3);
    }

    void* finalTriangle = Polygon_CreateTriangle();
    for (std::uint32_t vertex = 0; vertex < 3; ++vertex)
    {
        Polygon_ReserveDwordSlot(finalTriangle);
        const std::uint32_t count = CSF_AT(std::uint32_t, finalTriangle, 0x08);
        auto* destination = CSF_AT(std::uint32_t*, finalTriangle, 0x00) + count;
        CSF_AT(std::uint32_t, finalTriangle, 0x08) = count + 1;
        *destination = CSF_AT(std::uint32_t*, remainingBytes, 0x00)[vertex];
    }
    Polygon_AppendDword(
        static_cast<std::uint8_t*>(self) + 0x10,
        static_cast<std::uint32_t>(reinterpret_cast<std::uintptr_t>(finalTriangle)));

    if (CSF_AT(std::uint32_t, edges, 0x08))
    {
        std::uint32_t edgeIndex = 0;
        std::uint32_t edgeOffset = 0;
        do
        {
            auto* edge = CSF_AT(std::uint8_t*, edges, 0x00) + edgeOffset;
            if (CSF_AT(std::uint32_t, self, 0x18))
            {
                std::uint32_t triangleIndex = 0;
                do
                {
                    void* triangle = CSF_AT(void**, self, 0x10)[triangleIndex];
                    if (triangle != CSF_AT(void*, edge, 0x08))
                    {
                        const bool containsFirst = Polygon_TriangleContainsIndex(
                            triangle, CSF_AT(std::uint32_t, edge, 0x00));
                        const bool containsSecond = Polygon_TriangleContainsIndex(
                            triangle, CSF_AT(std::uint32_t, edge, 0x04));
                        if (containsFirst && containsSecond)
                            CSF_AT(void*, edge, 0x0C) = triangle;
                    }
                    ++triangleIndex;
                }
                while (triangleIndex < CSF_AT(std::uint32_t, self, 0x18));
            }
            ++edgeIndex;
            edgeOffset += 16;
        }
        while (edgeIndex < CSF_AT(std::uint32_t, edges, 0x08));
    }

    return Mem_Free(CSF_AT(void*, remainingBytes, 0x00));
}

HOOK(0x006B04B0, Polygon_TriangulateAndLinkNeighbors, "auto");
