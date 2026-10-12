#include <cstdint>
#include <cstddef>
#include <cstring>
#include <cmath>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

CSF_FUNC(void*, __fastcall, Path_ReservePoints, 0x00425DA0,
         (void* self, void* edx, int a2));
CSF_FUNC(int, __fastcall, Path_Finalize, 0x00425EA0,
         (void* self, void* edx, float a2));
CSF_FUNC(float*, __fastcall, GetObjectPositionVector, 0x0048E430,
         (void* self, void* edx));
CSF_FUNC(int, __fastcall, C_0085e7b8_GetPathPointRecord, 0x004A0F30,
         (C_0085e7b8* self, void*, int index));
CSF_FUNC(void*, __fastcall, GetCachedObjectProvider, 0x00686C10,
         (void* self, void* edx));

CSF_GLOBAL(float, g_MovementAcceleration, 0x0082E074);
CSF_GLOBAL(float, g_PathDistanceLimit, 0x0082FEA4);

static void Path_AppendPoint(void* points, float x, float y, float z,
                             float acceleration, float distance)
{
    if (CSF_AT(std::uint32_t, points, 0x08) >=
        CSF_AT(std::uint32_t, points, 0x04))
    {
        Path_ReservePoints(points, nullptr, CSF_AT(int, points, 0x0C));
    }

    const std::uint32_t index = CSF_AT(std::uint32_t, points, 0x08);
    auto* record = CSF_AT(std::uint8_t*, points, 0x00) + 20 * index;
    CSF_AT(std::uint32_t, points, 0x08) = index + 1;

    CSF_AT(float, record, 0x00) = x;
    CSF_AT(float, record, 0x04) = y;
    CSF_AT(float, record, 0x08) = z;
    CSF_AT(float, record, 0x0C) = acceleration;
    CSF_AT(float, record, 0x10) = distance;
}

static float Path_GetDistanceFromLastPoint(void* points,
                                           float x, float y, float z)
{
    const std::uint32_t count = CSF_AT(std::uint32_t, points, 0x08);
    auto* previous = CSF_AT(std::uint8_t*, points, 0x00) + 20 * count - 20;
    const float dx = CSF_AT(float, previous, 0x00) - x;
    const float dy = CSF_AT(float, previous, 0x04) - y;
    const float dz = CSF_AT(float, previous, 0x08) - z;
    const float distance = std::sqrt(dz * dz + dy * dy + dx * dx);
    return distance < g_PathDistanceLimit ? distance : 500.0f;
}

static int __fastcall Path_BuildFromPathObject(
    void* self, void* /*edx*/, float pathArgument, C_008342e8* object)
{
    CSF_AT(std::uint32_t, self, 0x1C) = 0;
    CSF_AT(std::uint8_t, self, 0x28) = 0;

    const float* position = GetObjectPositionVector(object, nullptr);
    const float startX = position[0];
    const float startY = position[1];
    const float startZ = position[2];

    auto* points = static_cast<std::uint8_t*>(self) + 0x14;
    Path_AppendPoint(points, startX, startY, startZ, 0.0f, -1.0f);

    // The original float parameter carries a pointer's raw bits.
    std::uint32_t pathBits;
    std::memcpy(&pathBits, &pathArgument, sizeof(pathBits));
    auto* path = reinterpret_cast<C_0085e7b8*>(
        static_cast<std::uintptr_t>(pathBits));

    std::uint32_t index = 0;
    if (reinterpret_cast<std::uintptr_t>(
            GetCachedObjectProvider(path, nullptr)) != 1)
    {
        do
        {
            const auto* record = reinterpret_cast<const float*>(
                static_cast<std::uintptr_t>(
                    C_0085e7b8_GetPathPointRecord(
                        path, nullptr, static_cast<int>(index))));
            const float x = record[0];
            const float y = record[1];
            const float z = record[2];

            float distance = Path_GetDistanceFromLastPoint(points, x, y, z);
            if (distance == g_MovementAcceleration)
                distance = -1.0f;

            Path_AppendPoint(points, x, y, z, 300.0f, distance);
            ++index;
        }
        while (index <
               static_cast<std::uint32_t>(
                   reinterpret_cast<std::uintptr_t>(
                       GetCachedObjectProvider(path, nullptr))) - 1);
    }

    const std::uint32_t count = static_cast<std::uint32_t>(
        reinterpret_cast<std::uintptr_t>(
            GetCachedObjectProvider(path, nullptr)));
    const auto* finalRecord = reinterpret_cast<const float*>(
        static_cast<std::uintptr_t>(
            C_0085e7b8_GetPathPointRecord(
                path, nullptr, static_cast<int>(count - 1))));
    const float finalX = finalRecord[0];
    const float finalY = finalRecord[1];
    const float finalZ = finalRecord[2];
    const float finalDistance =
        Path_GetDistanceFromLastPoint(points, finalX, finalY, finalZ);

    Path_AppendPoint(points, finalX, finalY, finalZ, 0.0f, finalDistance);

    const std::uint32_t objectBits = static_cast<std::uint32_t>(
        reinterpret_cast<std::uintptr_t>(object));
    float objectArgument;
    std::memcpy(&objectArgument, &objectBits, sizeof(objectArgument));
    return Path_Finalize(self, nullptr, objectArgument);
}

HOOK(0x00426310, Path_BuildFromPathObject, "auto");
