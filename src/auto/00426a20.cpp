#include <cstdint>
#include <cstring>
#include <cmath>
#include "globals.h"
#include "hook.h"

CSF_FUNC(long double, __cdecl, GfxMatrix_ComputeArcCosine, 0x0040C980, (float a1));
CSF_FUNC(void*, __fastcall, Buffer_PadToMinimumLength, 0x00425CE0, (void* self, void* edx, int a2));

CSF_GLOBAL(float, g_HalfDurationScale, 0x0082D444);
CSF_GLOBAL(float, g_fZero, 0x0082D4CC);
CSF_GLOBAL(float, g_TransitionOne, 0x0082D4D4);
CSF_GLOBAL(float, g_PathPi, 0x0082FE80);

static int __fastcall Path_PrepareSegments(void* self, void* /*edx*/)
{
    auto* pointStorage = static_cast<uint8_t*>(self) + 0x14;
    auto* points = reinterpret_cast<uint8_t*>(CSF_AT(uint32_t, pointStorage, 0));
    auto* point = points + 20 * CSF_AT(uint32_t, self, 0x24);

    const float startX = CSF_AT(float, point, 0x00);
    const float startZ = CSF_AT(float, point, 0x08);
    const float cornerX = CSF_AT(float, point, 0x14);
    const float cornerZ = CSF_AT(float, point, 0x1C);
    const float nextX = CSF_AT(float, point, 0x28);
    const float nextZ = CSF_AT(float, point, 0x30);
    const float trimDistance = CSF_AT(float, point, 0x20);

    const float deltaX = cornerX - startX;
    const float deltaZ = cornerZ - startZ;
    const float length = std::sqrt(deltaZ * deltaZ + deltaX * deltaX);
    const float inverseLength = g_TransitionOne / length;
    const float directionX = inverseLength * deltaX;
    const float directionZ = inverseLength * deltaZ;

    const float outgoingX = nextX - cornerX;
    const float outgoingZ = nextZ - cornerZ;
    const float inverseOutgoingLength =
        g_TransitionOne / std::sqrt(outgoingZ * outgoingZ + outgoingX * outgoingX);
    const float outgoingDirectionX = inverseOutgoingLength * outgoingX;
    const float outgoingDirectionZ = inverseOutgoingLength * outgoingZ;

    const float reverseX = -directionX;
    const float reverseZ = -directionZ;
    const float angle = static_cast<float>(GfxMatrix_ComputeArcCosine(
        reverseZ * outgoingDirectionZ + reverseX * outgoingDirectionX));
    const float halfAngle = g_HalfDurationScale * angle;
    const float radius = std::sin(halfAngle) * trimDistance / std::cos(halfAngle);

    const bool hasStraightSegment = trimDistance < length;
    float tangentX = startX;
    float tangentZ = startZ;
    if (hasStraightSegment)
    {
        tangentX = reverseX * trimDistance;
        tangentX += cornerX;
        tangentZ = reverseZ * trimDistance + cornerZ;
    }

    float normalX = directionZ;
    float normalZ = reverseX;
    const bool negativeTurn =
        outgoingDirectionZ * reverseX + outgoingDirectionX * directionZ < g_fZero;
    if (negativeTurn)
    {
        normalX = reverseZ;
        normalZ = -reverseX;
    }

    const float centerX = normalX * radius + tangentX;
    const float centerZ = tangentZ + normalZ * radius;
    const float arcLength = (g_PathPi - angle) * radius;
    float startAngle = static_cast<float>(
        GfxMatrix_ComputeArcCosine(-normalZ * g_fZero - normalX));
    if (normalZ > g_fZero)
        startAngle = -startAngle;

    alignas(8) uint8_t segmentBytes[0x24] = {};
    std::memset(segmentBytes + 0x10, 0, 16);
    CSF_AT(uint8_t, segmentBytes, 0x20) = 1;

    if (hasStraightSegment)
    {
        CSF_AT(uint32_t, segmentBytes, 0x00) = 1;
        CSF_AT(float, segmentBytes, 0x04) = length - trimDistance;
        CSF_AT(float, segmentBytes, 0x08) = startX;
        CSF_AT(float, segmentBytes, 0x0C) = startZ;
        CSF_AT(float, segmentBytes, 0x10) = directionX;
        CSF_AT(float, segmentBytes, 0x14) = directionZ;

        if (CSF_AT(uint32_t, self, 0x0C) >= CSF_AT(uint32_t, self, 0x08))
            Buffer_PadToMinimumLength(
                static_cast<uint8_t*>(self) + 0x04, nullptr,
                CSF_AT(int, self, 0x10));

        const uint32_t index = CSF_AT(uint32_t, self, 0x0C);
        auto* destination = reinterpret_cast<uint8_t*>(CSF_AT(uint32_t, self, 0x04))
                            + 36 * index;
        CSF_AT(uint32_t, self, 0x0C) = index + 1;
        std::memcpy(destination, segmentBytes, 0x24);
    }

    CSF_AT(uint32_t, segmentBytes, 0x00) = 2 * (!negativeTurn) + 2;
    CSF_AT(float, segmentBytes, 0x04) = arcLength;
    CSF_AT(float, segmentBytes, 0x08) = centerX;
    CSF_AT(float, segmentBytes, 0x0C) = centerZ;
    CSF_AT(float, segmentBytes, 0x18) = radius;
    CSF_AT(float, segmentBytes, 0x1C) = startAngle;

    if (CSF_AT(uint32_t, self, 0x0C) >= CSF_AT(uint32_t, self, 0x08))
        Buffer_PadToMinimumLength(
            static_cast<uint8_t*>(self) + 0x04, nullptr,
            CSF_AT(int, self, 0x10));

    const uint32_t index = CSF_AT(uint32_t, self, 0x0C);
    auto* destination = reinterpret_cast<uint8_t*>(CSF_AT(uint32_t, self, 0x04))
                        + 36 * index;
    const uint32_t result = index + 1;
    CSF_AT(uint32_t, self, 0x0C) = result;
    std::memcpy(destination, segmentBytes, 0x24);
    return static_cast<int>(result);
}

HOOK(0x00426A20, Path_PrepareSegments, "auto");
