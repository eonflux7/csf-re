#include <cstdint>
#include <cstddef>
#include <cstring>
#include <cmath>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

CSF_FUNC(float*, __fastcall, NormalizeVector3, 0x006AB8D0, (C_00833b88* self, void*));
CSF_FUNC(char, __fastcall, GfxCameraProjection_TestViewOrigin, 0x006F6330, (void* self, void* edx, int a2));

CSF_GLOBAL(float, g_fZero, 0x0082D4CC);

static void GfxCameraProjection_BuildEdgeNormal(
    const void* origin, const void* first, const void* second, void* normal)
{
    const float ax = CSF_AT(float, first, 0x00) - CSF_AT(float, origin, 0x00);
    const float ay = CSF_AT(float, first, 0x04) - CSF_AT(float, origin, 0x04);
    const float az = CSF_AT(float, first, 0x08) - CSF_AT(float, origin, 0x08);
    const float bx = CSF_AT(float, second, 0x00) - CSF_AT(float, origin, 0x00);
    const float by = CSF_AT(float, second, 0x04) - CSF_AT(float, origin, 0x04);
    const float bz = CSF_AT(float, second, 0x08) - CSF_AT(float, origin, 0x08);

    CSF_AT(float, normal, 0x00) = by * az - bz * ay;
    CSF_AT(float, normal, 0x04) = bz * ax - az * bx;
    CSF_AT(float, normal, 0x08) = ay * bx - by * ax;
    NormalizeVector3(reinterpret_cast<C_00833b88*>(normal), nullptr);
}

static void GfxCameraProjection_StorePlane(
    void* plane, const void* normal, const void* point)
{
    const float x = CSF_AT(float, normal, 0x00);
    const float y = CSF_AT(float, normal, 0x04);
    const float z = CSF_AT(float, normal, 0x08);
    const float distance =
        x * CSF_AT(float, point, 0x00) +
        y * CSF_AT(float, point, 0x04) +
        z * CSF_AT(float, point, 0x08);

    CSF_AT(float, plane, 0x00) = x;
    CSF_AT(float, plane, 0x04) = y;
    CSF_AT(float, plane, 0x08) = z;
    CSF_AT(float, plane, 0x0C) = distance;
}

static char __fastcall S_006f6330_UpdateClumpView(
    S_006f6330* self, void* /*edx*/, int originAddress,
    int lateralAddress, float directionArgument)
{
    // The packet also identifies the predicate's address as a vtable reference.
    (void)CSF_VTBL(0x006F6330);

    const int initialCount = static_cast<int>(self->field_0008);
    if (!self->field_0069)
    {
        const char rejected =
            GfxCameraProjection_TestViewOrigin(self, nullptr, originAddress);
        if (rejected)
        {
            self->field_006a = 0;
            return rejected;
        }
    }

    const void* origin = reinterpret_cast<const void*>(originAddress);
    const void* lateral = reinterpret_cast<const void*>(lateralAddress);
    uint32_t directionAddress;
    std::memcpy(&directionAddress, &directionArgument, sizeof(directionAddress));
    const void* direction = reinterpret_cast<const void*>(directionAddress);

    const float lateralX = CSF_AT(float, lateral, 0x00);
    const float lateralZ = CSF_AT(float, lateral, 0x08);
    const float directionX = CSF_AT(float, direction, 0x00);
    const float directionZ = CSF_AT(float, direction, 0x08);
    const float originX = CSF_AT(float, origin, 0x00);
    const float originZ = CSF_AT(float, origin, 0x08);

    bool found = false;
    int minimumIndex = 0;
    int maximumIndex = 0;
    int farthestIndex = 0;
    float minimumRatio = 0.0f;
    float maximumRatio = directionArgument;
    float farthestDistance = 0.0f;
    int index = 0;
    char traversalResult = 0;

    const auto considerVertex = [&](const float* vertex, int vertexIndex,
                                    bool trailingComparison)
    {
        const float dx = vertex[0] - originX;
        const float dz = vertex[1] - originZ;
        const float distance = dx * directionX + dz * directionZ;
        if (!(distance > g_fZero))
            return;

        const float ratio = (dx * lateralX + dz * lateralZ) / distance;
        if (!found)
        {
            found = true;
            minimumRatio = maximumRatio = ratio;
            farthestDistance = distance;
            minimumIndex = maximumIndex = farthestIndex = vertexIndex;
            return;
        }

        if (farthestDistance < distance)
        {
            farthestDistance = distance;
            farthestIndex = vertexIndex;
        }
        if (ratio < minimumRatio)
        {
            minimumRatio = ratio;
            minimumIndex = vertexIndex;
        }
        if (trailingComparison ? !(ratio <= maximumRatio) : ratio > maximumRatio)
        {
            maximumRatio = ratio;
            maximumIndex = vertexIndex;
        }
    };

    if (initialCount >= 4)
    {
        const int groupedLimit = initialCount - 3;
        const float* firstPair =
            reinterpret_cast<const float*>(self->field_0000 + 8);
        const float* secondPair =
            reinterpret_cast<const float*>(self->field_0000 + 20);
        int middleIndex = 2;

        do
        {
            const float first[2] = {firstPair[-2], firstPair[-1]};
            considerVertex(first, index, false);

            const float second[2] = {firstPair[0], secondPair[-2]};
            considerVertex(second, middleIndex - 1, false);

            const float third[2] = {firstPair[2], secondPair[0]};
            considerVertex(third, middleIndex, false);

            const float fourth[2] = {firstPair[4], secondPair[2]};
            considerVertex(fourth, middleIndex + 1, true);

            firstPair += 8;
            secondPair += 8;
            traversalResult = static_cast<char>(groupedLimit);
            index += 4;
            middleIndex += 4;
        }
        while (index < groupedLimit);
    }

    if (index < static_cast<int>(self->field_0008))
    {
        const float* vertex =
            reinterpret_cast<const float*>(self->field_0000 + 8 * index);
        int currentCount;
        do
        {
            considerVertex(vertex, index, true);
            currentCount = static_cast<int>(self->field_0008);
            traversalResult = static_cast<char>(currentCount);
            ++index;
            vertex += 2;
        }
        while (index < currentCount);
    }

    if (minimumIndex == maximumIndex)
    {
        self->field_006a = 0;
        return traversalResult;
    }

    uint32_t vertices = self->field_0000;
    float y = CSF_AT(float, self, 0x10);
    self->field_006a = 1;
    float x = CSF_AT(float, reinterpret_cast<const void*>(vertices + 8 * minimumIndex), 0x00);
    const float* minimumZ =
        reinterpret_cast<const float*>(vertices + 8 * minimumIndex + 4);
    float z = *minimumZ;
    CSF_AT(float, self, 0x6C) = x;
    CSF_AT(float, self, 0x70) = y;
    CSF_AT(float, self, 0x74) = z;

    y = CSF_AT(float, self, 0x10);
    const float* maximumZ =
        reinterpret_cast<const float*>(self->field_0000 + 8 * maximumIndex + 4);
    x = CSF_AT(float, reinterpret_cast<const void*>(self->field_0000 + 8 * maximumIndex), 0x00);
    z = *maximumZ;
    CSF_AT(float, self, 0x78) = x;
    CSF_AT(float, self, 0x7C) = y;
    CSF_AT(float, self, 0x80) = z;

    vertices = self->field_0000;
    x = CSF_AT(float, reinterpret_cast<const void*>(self->field_0000 + 8 * maximumIndex), 0x00);
    y = CSF_AT(float, self, 0x14);
    z = *maximumZ;
    CSF_AT(float, self, 0x84) = x;
    CSF_AT(float, self, 0x88) = y;
    CSF_AT(float, self, 0x8C) = z;

    z = *minimumZ;
    x = CSF_AT(float, reinterpret_cast<const void*>(vertices + 8 * minimumIndex), 0x00);
    y = CSF_AT(float, self, 0x14);
    CSF_AT(float, self, 0x90) = x;
    CSF_AT(float, self, 0x94) = y;
    CSF_AT(float, self, 0x98) = z;

    alignas(8) uint8_t leftNormalBytes[0xC];
    alignas(8) uint8_t bottomNormalBytes[0xC];
    alignas(8) uint8_t rightNormalBytes[0xC];
    alignas(8) uint8_t topNormalBytes[0xC];
    alignas(8) uint8_t farNormalBytes[0x5C];
    std::memset(bottomNormalBytes, 0, 0xC);
    std::memset(rightNormalBytes, 0, 0xC);
    std::memset(topNormalBytes, 0, 0xC);
    std::memset(farNormalBytes, 0, 0xC);

    auto* bytes = reinterpret_cast<uint8_t*>(self);
    GfxCameraProjection_BuildEdgeNormal(
        origin, bytes + 0x90, bytes + 0x6C, leftNormalBytes);
    GfxCameraProjection_BuildEdgeNormal(
        origin, bytes + 0x6C, bytes + 0x78, bottomNormalBytes);
    GfxCameraProjection_BuildEdgeNormal(
        origin, bytes + 0x78, bytes + 0x84, rightNormalBytes);
    GfxCameraProjection_BuildEdgeNormal(
        origin, bytes + 0x84, bytes + 0x90, topNormalBytes);

    GfxCameraProjection_StorePlane(bytes + 0x18, leftNormalBytes, origin);
    GfxCameraProjection_StorePlane(bytes + 0x28, bottomNormalBytes, origin);
    GfxCameraProjection_StorePlane(bytes + 0x38, rightNormalBytes, origin);
    GfxCameraProjection_StorePlane(bytes + 0x48, topNormalBytes, origin);

    float farDistance;
    if (CSF_AT(uint8_t, self, 0x68))
    {
        GfxCameraProjection_BuildEdgeNormal(
            bytes + 0x6C, bytes + 0x84, bytes + 0x90, farNormalBytes);
        farDistance =
            CSF_AT(float, farNormalBytes, 0x04) * CSF_AT(float, self, 0x70) +
            CSF_AT(float, farNormalBytes, 0x00) * CSF_AT(float, self, 0x6C) +
            CSF_AT(float, farNormalBytes, 0x08) * CSF_AT(float, self, 0x74);
    }
    else
    {
        const float directionY = CSF_AT(float, direction, 0x04);
        const float currentDirectionZ = CSF_AT(float, direction, 0x08);
        CSF_AT(float, farNormalBytes, 0x00) = CSF_AT(float, direction, 0x00);
        CSF_AT(float, farNormalBytes, 0x04) = directionY;
        CSF_AT(float, farNormalBytes, 0x08) = currentDirectionZ;
        NormalizeVector3(reinterpret_cast<C_00833b88*>(farNormalBytes), nullptr);

        const float* farthest =
            reinterpret_cast<const float*>(self->field_0000 + 8 * farthestIndex);
        const float topDistance =
            farthest[0] * CSF_AT(float, farNormalBytes, 0x00) +
            CSF_AT(float, self, 0x14) * CSF_AT(float, farNormalBytes, 0x04) +
            farthest[1] * CSF_AT(float, farNormalBytes, 0x08);
        const float bottomDistance =
            farthest[0] * CSF_AT(float, farNormalBytes, 0x00) +
            CSF_AT(float, self, 0x10) * CSF_AT(float, farNormalBytes, 0x04) +
            farthest[1] * CSF_AT(float, farNormalBytes, 0x08);
        farDistance = bottomDistance >= topDistance ? bottomDistance : topDistance;
    }

    CSF_AT(float, self, 0x58) = CSF_AT(float, farNormalBytes, 0x00);
    CSF_AT(float, self, 0x5C) = CSF_AT(float, farNormalBytes, 0x04);
    CSF_AT(float, self, 0x60) = CSF_AT(float, farNormalBytes, 0x08);
    CSF_AT(float, self, 0x64) = farDistance;

    for (int plane = 0; plane < 5; ++plane)
    {
        CSF_AT(float, bytes + 0xB4 + 12 * plane, 0x00) =
            std::fabs(CSF_AT(float, bytes + 0x18 + 16 * plane, 0x00));
        CSF_AT(float, bytes + 0xB4 + 12 * plane, 0x04) =
            std::fabs(CSF_AT(float, bytes + 0x18 + 16 * plane, 0x04));
        CSF_AT(float, bytes + 0xB4 + 12 * plane, 0x08) =
            std::fabs(CSF_AT(float, bytes + 0x18 + 16 * plane, 0x08));
    }

    return static_cast<char>(reinterpret_cast<uintptr_t>(self) + 88);
}

HOOK(0x006F63C0, S_006f6330_UpdateClumpView, "auto");
