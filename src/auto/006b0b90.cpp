#include <cstdint>
#include <cstddef>
#include <cstring>
#include "globals.h"
#include "hook.h"

CSF_FUNC(void*, __cdecl, Mem_Alloc, 0x006910F0, (std::size_t size));
CSF_FUNC(int, __cdecl, Mem_Free, 0x00691110, (void* block));
CSF_FUNC(char, __fastcall, Geometry_IsMergedPolygonValid, 0x006B0030, (void* self, void* edx, int* a2));
CSF_FUNC(char, __fastcall, Geometry_BuildMergeCandidates, 0x006B04B0, (void* self, void* edx, uint32_t* a2));
CSF_FUNC(void, __cdecl, _qsort, 0x007AEAE0, (void* base, std::size_t count, std::size_t size, int (__cdecl* compare)(const void*, const void*)));
CSF_FUNC(void*, __cdecl, _memcpy, 0x007AEE60, (void* destination, const void* source, std::size_t byteCount));
CSF_FUNC(int, __cdecl, CompareDwordPtrsDescending, 0x006AFDF0, (const uint32_t* a1, const uint32_t* a2));

CSF_GLOBAL(float, g_HalfDurationScale, 0x0082D444);
CSF_GLOBAL(float, g_MinimumQueryHeight, 0x0082DF18);

static int __fastcall Geometry_MergeAdjacentPolygons(void* self, void* /*edx*/)
{
    alignas(8) std::uint8_t candidateBytes[0x10];
    alignas(8) std::uint8_t temporaryBytes[0x38];

    auto& candidates = CSF_AT(std::uint8_t*, candidateBytes, 0);
    auto& candidateCount = CSF_AT(std::int32_t, candidateBytes, 8);
    candidates = nullptr;
    CSF_AT(std::uint32_t, candidateBytes, 4) = 0;
    candidateCount = 0;
    CSF_AT(std::uint32_t, candidateBytes, 12) = 10;
    Geometry_BuildMergeCandidates(
        self, nullptr, reinterpret_cast<uint32_t*>(candidateBytes));

    auto& temporaryData = CSF_AT(std::uint32_t*, temporaryBytes, 0);
    auto& temporaryCapacity = CSF_AT(std::uint32_t, temporaryBytes, 4);
    auto& temporaryCount = CSF_AT(std::uint32_t, temporaryBytes, 8);
    auto& temporaryGrowth = CSF_AT(std::uint32_t, temporaryBytes, 12);
    temporaryData = nullptr;
    temporaryCapacity = 0;
    temporaryCount = 0;
    temporaryGrowth = 10;

    int mergeCount;
    do
    {
        mergeCount = 0;
        std::int32_t candidateIndex = candidateCount - 1;
        if (candidateIndex < 0)
            break;

        std::uint8_t* candidate = candidates + 16 * candidateIndex;
        do
        {
            void* retainedPolygon = CSF_AT(void*, candidate, 8);
            void* removedPolygon = CSF_AT(void*, candidate, 12);

            if (CSF_AT(std::uint32_t, retainedPolygon, 8) > temporaryCapacity)
            {
                void* allocation = Mem_Alloc(
                    4u * CSF_AT(std::uint32_t, retainedPolygon, 4));
                temporaryCapacity = CSF_AT(std::uint32_t, retainedPolygon, 4);
                if (temporaryData)
                    Mem_Free(temporaryData);
                temporaryData = static_cast<std::uint32_t*>(allocation);
            }

            std::uint32_t copyCount = CSF_AT(std::uint32_t, retainedPolygon, 8);
            if (copyCount)
            {
                std::memcpy(temporaryData,
                            CSF_AT(void*, retainedPolygon, 0),
                            4u * copyCount);
            }

            temporaryGrowth = CSF_AT(std::uint32_t, retainedPolygon, 12);
            std::uint32_t combinedCount = CSF_AT(std::uint32_t, retainedPolygon, 8);
            std::uint32_t requiredCapacity =
                combinedCount + CSF_AT(std::uint32_t, removedPolygon, 8);

            std::uint32_t* combinedData;
            if (requiredCapacity > temporaryCapacity)
            {
                void* allocation = Mem_Alloc(4u * requiredCapacity);
                if (combinedCount)
                    std::memcpy(allocation, temporaryData, 4u * combinedCount);
                temporaryCapacity = requiredCapacity;
                if (temporaryData)
                    Mem_Free(temporaryData);
                combinedData = static_cast<std::uint32_t*>(allocation);
                temporaryData = combinedData;
            }
            else
            {
                combinedData = temporaryData;
            }

            std::uint32_t appendIndex = 0;
            if (CSF_AT(std::uint32_t, removedPolygon, 8))
            {
                do
                {
                    std::uint32_t* sourceElement =
                        CSF_AT(std::uint32_t*, removedPolygon, 0) + appendIndex;
                    if (combinedCount >= temporaryCapacity)
                    {
                        std::uint32_t newCapacity = temporaryGrowth + temporaryCapacity;
                        void* allocation = Mem_Alloc(4u * newCapacity);
                        if (combinedCount)
                            std::memcpy(allocation, temporaryData, 4u * combinedCount);
                        temporaryCapacity = newCapacity;
                        if (temporaryData)
                            Mem_Free(temporaryData);
                        temporaryData = static_cast<std::uint32_t*>(allocation);
                        combinedData = temporaryData;
                    }

                    combinedData[combinedCount++] = *sourceElement;
                    ++appendIndex;
                }
                while (appendIndex < CSF_AT(std::uint32_t, removedPolygon, 8));
            }

            std::uint32_t firstVertex = CSF_AT(std::uint32_t, candidate, 0);
            std::int32_t firstIndex = 0;
            while (static_cast<std::uint32_t>(firstIndex) < combinedCount &&
                   combinedData[firstIndex] != firstVertex)
            {
                ++firstIndex;
            }
            if (static_cast<std::uint32_t>(firstIndex) >= combinedCount)
                firstIndex = -1;

            std::uint32_t remainingCount = combinedCount - 1;
            if (static_cast<std::uint32_t>(firstIndex) != remainingCount)
            {
                _memcpy(combinedData + firstIndex,
                        combinedData + firstIndex + 1,
                        4u * (remainingCount - static_cast<std::uint32_t>(firstIndex)));
            }

            std::uint32_t secondVertex = CSF_AT(std::uint32_t, candidate, 4);
            std::int32_t secondIndex = 0;
            while (static_cast<std::uint32_t>(secondIndex) < remainingCount &&
                   combinedData[secondIndex] != secondVertex)
            {
                ++secondIndex;
            }
            if (static_cast<std::uint32_t>(secondIndex) >= remainingCount)
                secondIndex = -1;

            std::uint32_t mergedCount = remainingCount - 1;
            temporaryCount = mergedCount;
            if (static_cast<std::uint32_t>(secondIndex) != mergedCount)
            {
                _memcpy(combinedData + secondIndex,
                        combinedData + secondIndex + 1,
                        4u * (mergedCount - static_cast<std::uint32_t>(secondIndex)));
            }

            _qsort(combinedData, mergedCount, 4u,
                   reinterpret_cast<int (__cdecl*)(const void*, const void*)>(
                       CompareDwordPtrsDescending));

            if (Geometry_IsMergedPolygonValid(
                    self, nullptr, reinterpret_cast<int*>(temporaryBytes)))
            {
                std::int32_t recordsToUpdate = candidateCount - 1;
                bool isLastCandidate = candidateIndex == recordsToUpdate;
                ++mergeCount;
                --candidateCount;
                if (!isLastCandidate)
                {
                    _memcpy(candidate, candidate + 16,
                            16u * static_cast<std::uint32_t>(
                                recordsToUpdate - candidateIndex));
                }

                if (mergedCount > CSF_AT(std::uint32_t, retainedPolygon, 4))
                {
                    void* allocation = Mem_Alloc(4u * temporaryCapacity);
                    void* previousData = CSF_AT(void*, retainedPolygon, 0);
                    CSF_AT(std::uint32_t, retainedPolygon, 4) = temporaryCapacity;
                    if (previousData)
                    {
                        Mem_Free(previousData);
                        CSF_AT(void*, retainedPolygon, 0) = nullptr;
                    }
                    CSF_AT(void*, retainedPolygon, 0) = allocation;
                }

                if (mergedCount)
                {
                    std::memcpy(CSF_AT(void*, retainedPolygon, 0),
                                temporaryData, 4u * mergedCount);
                    recordsToUpdate = candidateCount;
                }
                CSF_AT(std::uint32_t, retainedPolygon, 8) = mergedCount;
                CSF_AT(std::uint32_t, retainedPolygon, 12) = temporaryGrowth;

                if (recordsToUpdate)
                {
                    std::uint8_t* record = candidates;
                    do
                    {
                        if (CSF_AT(void*, record, 8) == removedPolygon)
                            CSF_AT(void*, record, 8) = retainedPolygon;
                        else if (CSF_AT(void*, record, 12) == removedPolygon)
                            CSF_AT(void*, record, 12) = retainedPolygon;
                        record += 16;
                        --recordsToUpdate;
                    }
                    while (recordsToUpdate);
                }

                std::uint32_t polygonCount = CSF_AT(std::uint32_t, self, 24);
                std::uint32_t polygonIndex = 0;
                std::int32_t removalIndex = -1;
                if (polygonCount)
                {
                    void** polygon = CSF_AT(void**, self, 16);
                    do
                    {
                        if (*polygon == removedPolygon)
                        {
                            removalIndex = static_cast<std::int32_t>(polygonIndex);
                            break;
                        }
                        ++polygonIndex;
                        ++polygon;
                    }
                    while (polygonIndex < polygonCount);
                }

                std::uint32_t newPolygonCount = CSF_AT(std::uint32_t, self, 24) - 1;
                CSF_AT(std::uint32_t, self, 24) = newPolygonCount;
                if (static_cast<std::uint32_t>(removalIndex) != newPolygonCount)
                {
                    void** polygonList = CSF_AT(void**, self, 16);
                    _memcpy(polygonList + removalIndex,
                            polygonList + removalIndex + 1,
                            4u * (newPolygonCount -
                                  static_cast<std::uint32_t>(removalIndex)));
                }

                if (CSF_AT(void*, removedPolygon, 0))
                {
                    Mem_Free(CSF_AT(void*, removedPolygon, 0));
                    CSF_AT(void*, removedPolygon, 0) = nullptr;
                }
                Mem_Free(removedPolygon);
            }

            --candidateIndex;
            candidate -= 16;
        }
        while (candidateIndex >= 0);
    }
    while (mergeCount);

    std::uint32_t polygonIndex = 0;
    if (CSF_AT(std::uint32_t, self, 24))
    {
        do
        {
            void* polygon = CSF_AT(void**, self, 16)[polygonIndex];
            std::uint32_t vertexCount = CSF_AT(std::uint32_t, polygon, 8);
            float minX = 100000000.0f;
            float maxX = -100000000.0f;
            float minY = 100000000.0f;
            float maxY = g_MinimumQueryHeight;

            if (vertexCount)
            {
                std::uint32_t* vertex = CSF_AT(std::uint32_t*, polygon, 0);
                do
                {
                    float* coordinates = CSF_AT(float*, self, 0) + 2 * *vertex;
                    if (minX >= coordinates[0])
                        minX = coordinates[0];
                    if (maxX < coordinates[0])
                        maxX = coordinates[0];
                    if (!(minY < coordinates[1]))
                        minY = coordinates[1];
                    if (maxY < coordinates[1])
                        maxY = coordinates[1];
                    ++vertex;
                    --vertexCount;
                }
                while (vertexCount);
            }

            ++polygonIndex;
            CSF_AT(float, polygon, 16) = (maxX + minX) * g_HalfDurationScale;
            CSF_AT(float, polygon, 20) = (maxY + minY) * g_HalfDurationScale;
        }
        while (polygonIndex < CSF_AT(std::uint32_t, self, 24));
    }

    if (temporaryData)
        Mem_Free(temporaryData);
    if (candidates)
        return Mem_Free(candidates);
    return 0;
}

HOOK(0x006B0B90, Geometry_MergeAdjacentPolygons, "auto");
