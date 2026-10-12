#include <cstdint>
#include <cstddef>
#include <cstring>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"

CSF_FUNC(int, __cdecl, RtIntersectionLineTriangle, 0x00620640, (float* a1, float* a2, float* a3, float* a4, float* a5, float* a6));
CSF_FUNC(double, __cdecl, _rwInvSqrt, 0x00622F40, (float a1));
CSF_FUNC(int, __cdecl, GfxCollision_InitializeSectorQuery, 0x007C1C70, (int a1, int a2, int a3, uint32_t* a4));
CSF_FUNC(int, __cdecl, GfxCollision_NextSector, 0x007C1E90, (int a1));
CSF_FUNC(unsigned int, __cdecl, GfxCollision_ClipSectorQuery, 0x007C2090, (uint32_t* a1, float a2, int a3, int a4));
CSF_FUNC(void, __cdecl, GfxCollision_InitializeTriangleQuery, 0x007C2110, (int a1, int a2, int a3, uint32_t* a4));
CSF_FUNC(int16_t, __cdecl, GfxCollision_NextTriangleRange, 0x007C2640, (uint32_t* a1, uint16_t* a2, uint16_t* a3));
CSF_FUNC(unsigned int, __cdecl, GfxCollision_ClipTriangleQuery, 0x007C2870, (uint32_t* a1, float a2));

CSF_GLOBAL(uint32_t, g_CollisionPluginOffset, 0x008B0930);

static int __cdecl GfxCollision_IntersectWorldLine(
    int world, int intersection,
    int (__cdecl* callback)(int, int, float*, uint32_t, int),
    int userData)
{
    // The line and its sector traversal state occupy one contiguous stack object.
    alignas(8) uint8_t sectorQuery[0x340];
    alignas(8) uint8_t triangleQuery[0x1E0];
    alignas(8) uint8_t hit[0x34];
    alignas(8) uint8_t rangeCount[0x8];
    alignas(8) uint8_t rangeStart[0x4];
    alignas(8) uint8_t distanceBytes[0x4];

    auto* line = reinterpret_cast<float*>(sectorQuery);
    auto* lineState = reinterpret_cast<float*>(sectorQuery + 0xC);
    auto* sectorWords = reinterpret_cast<uint32_t*>(sectorQuery);
    auto* triangleWords = reinterpret_cast<uint32_t*>(triangleQuery);
    auto& sector = CSF_AT(int, triangleQuery, 0x1D8);
    auto& collisionTree = CSF_AT(int, triangleQuery, 0x1DC);
    auto& triangleData = CSF_AT(uint8_t*, hit, 0x28);
    auto& vertices = CSF_AT(uint8_t*, rangeCount, 0x4);
    auto& triangleCursor = CSF_AT(uint32_t, hit, 0x2C);
    auto& distance = CSF_AT(float, distanceBytes, 0);

    GfxCollision_InitializeSectorQuery(
        reinterpret_cast<int>(sectorQuery), world, intersection, nullptr);

    while (CSF_AT(uint32_t, sectorQuery, 0x33C) >
           reinterpret_cast<uintptr_t>(sectorQuery + 0x30))
    {
        const int nextSector =
            GfxCollision_NextSector(reinterpret_cast<int>(sectorQuery));
        const int tree = CSF_AT(
            int, reinterpret_cast<void*>(nextSector), g_CollisionPluginOffset);
        sector = nextSector;
        collisionTree = tree;
        if (!tree)
            continue;

        triangleData = CSF_AT(uint8_t*, reinterpret_cast<void*>(nextSector), 0x4);
        vertices = CSF_AT(uint8_t*, reinterpret_cast<void*>(nextSector), 0x8);
        GfxCollision_InitializeTriangleQuery(
            reinterpret_cast<int>(triangleQuery), tree, intersection, nullptr);

        while (CSF_AT(uint32_t, triangleQuery, 0x1C8) >
               reinterpret_cast<uintptr_t>(triangleQuery + 0x3C))
        {
            CSF_AT(uint32_t, rangeStart, 0) = 0;
            CSF_AT(uint32_t, rangeCount, 0) = 0;
            GfxCollision_NextTriangleRange(
                triangleWords, reinterpret_cast<uint16_t*>(rangeStart),
                reinterpret_cast<uint16_t*>(rangeCount));
            triangleCursor = CSF_AT(uint32_t, rangeStart, 0);

            while (static_cast<uint16_t>(triangleCursor) <
                   static_cast<int>(CSF_AT(uint16_t, rangeStart, 0)) +
                   CSF_AT(uint16_t, rangeCount, 0))
            {
                auto* remap = CSF_AT(uint16_t*,
                                    reinterpret_cast<void*>(collisionTree), 0x24);
                distance = 0.0f;
                const uint16_t triangleIndex =
                    remap ? remap[static_cast<uint16_t>(triangleCursor)]
                          : static_cast<uint16_t>(triangleCursor);

                float* vertex0 = reinterpret_cast<float*>(
                    vertices + 12 * CSF_AT(uint16_t, triangleData, 8 * triangleIndex));
                float* vertex1 = reinterpret_cast<float*>(
                    vertices + 12 * CSF_AT(uint16_t, triangleData, 8 * triangleIndex + 2));
                CSF_AT(uint32_t, hit, 0x30) = triangleIndex;
                float* vertex2 = reinterpret_cast<float*>(
                    vertices + 12 * CSF_AT(uint16_t, triangleData, 8 * triangleIndex + 4));

                if (RtIntersectionLineTriangle(
                        line, lineState, vertex0, vertex1, vertex2, &distance))
                {
                    std::memcpy(hit + 0xC, vertex0, 12);
                    CSF_AT(uint32_t, hit, 0x18) = CSF_AT(uint32_t, hit, 0x30);
                    CSF_AT(float*, hit, 0x1C) = vertex0;
                    CSF_AT(float*, hit, 0x20) = vertex1;
                    CSF_AT(float*, hit, 0x24) = vertex2;

                    const float edge1X = vertex1[0] - vertex0[0];
                    const float edge1Y = vertex1[1] - vertex0[1];
                    const float edge1Z = vertex1[2] - vertex0[2];
                    CSF_AT(float, triangleQuery, 0x1CC) = vertex2[0] - vertex0[0];
                    CSF_AT(float, triangleQuery, 0x1D0) = vertex2[1] - vertex0[1];
                    const float edge2Z = vertex2[2] - vertex0[2];

                    auto& nx = CSF_AT(float, hit, 0);
                    auto& ny = CSF_AT(float, hit, 0x4);
                    auto& nz = CSF_AT(float, hit, 0x8);
                    nx = edge2Z * edge1Y -
                         CSF_AT(float, triangleQuery, 0x1D0) * edge1Z;
                    ny = CSF_AT(float, triangleQuery, 0x1CC) * edge1Z -
                         edge2Z * edge1X;
                    nz = CSF_AT(float, triangleQuery, 0x1D0) * edge1X -
                         CSF_AT(float, triangleQuery, 0x1CC) * edge1Y;
                    CSF_AT(float, hit, 0x30) = ny * ny + nx * nx + nz * nz;
                    const double inverseLength = _rwInvSqrt(CSF_AT(float, hit, 0x30));
                    nx = static_cast<float>(nx * inverseLength);
                    ny = static_cast<float>(ny * inverseLength);
                    nz = static_cast<float>(nz * inverseLength);

                    if (!callback(intersection, sector, reinterpret_cast<float*>(hit),
                                  CSF_AT(uint32_t, distanceBytes, 0), userData))
                        return world;

                    GfxCollision_ClipTriangleQuery(triangleWords, distance);
                    GfxCollision_ClipSectorQuery(sectorWords, distance, 0, 0);
                }
                ++triangleCursor;
            }
        }
    }
    return world;
}

HOOK(0x006EBA90, GfxCollision_IntersectWorldLine, "auto");
