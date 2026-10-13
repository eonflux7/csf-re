#include <cstdint>
#include <cstring>
#include <cmath>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"

CSF_FUNC(double, __cdecl, GfxMatrix_ComputeArcCosine, 0x0040C980, (float a1));
CSF_FUNC(const void*, __fastcall, Buffer_GrowCapacity, 0x00425CE0, (const void** self, void* , int additionalCapacity));

CSF_GLOBAL(float, g_HalfDurationScale, 0x0082D444);
CSF_GLOBAL(float, g_fZero, 0x0082D4CC);
CSF_GLOBAL(float, g_TransitionOne, 0x0082D4D4);
CSF_GLOBAL(float, g_PathPi, 0x0082FE80);

static std::uint32_t __fastcall Path_PrepareSegments(
    std::uint32_t* self, void* /*edx*/)
{
    const std::uint32_t pointIndex = CSF_AT(std::uint32_t, self, 0x24);
    auto* points = CSF_AT(std::uint8_t*, self, 0x14);
    auto* first = points + 20 * pointIndex;
    auto* middle = first + 20;
    auto* last = first + 40;

    const float startX = CSF_AT(float, first, 0x00);
    const float startZ = CSF_AT(float, first, 0x08);
    const float cornerX = CSF_AT(float, middle, 0x00);
    const float cornerZ = CSF_AT(float, middle, 0x08);
    const double incomingX = static_cast<double>(cornerX) - startX;
    const double incomingZ = static_cast<double>(cornerZ) - startZ;
    const float endX = CSF_AT(float, last, 0x00);
    const float endZ = CSF_AT(float, last, 0x08);
    const float trimDistance = CSF_AT(float, first, 0x20);

    const float incomingLength = static_cast<float>(
        std::sqrt(incomingZ * incomingZ + incomingX * incomingX));
    const double incomingScale =
        static_cast<double>(g_TransitionOne) / incomingLength;
    const float directionX = static_cast<float>(incomingScale * incomingX);
    const float directionZ = static_cast<float>(incomingScale * incomingZ);

    const double outgoingX = static_cast<double>(endX) - cornerX;
    const double outgoingZ = static_cast<double>(endZ) - cornerZ;
    const double outgoingScale = g_TransitionOne /
        std::sqrt(outgoingZ * outgoingZ + outgoingX * outgoingX);
    const float outgoingDirectionX =
        static_cast<float>(outgoingScale * outgoingX);
    const float outgoingDirectionZ =
        static_cast<float>(outgoingScale * outgoingZ);
    const float backwardX = -directionX;
    const float backwardZ = -directionZ;

    const double angle = GfxMatrix_ComputeArcCosine(static_cast<float>(
        static_cast<double>(backwardZ) * outgoingDirectionZ +
        static_cast<double>(backwardX) * outgoingDirectionX));
    const double halfAngle = g_HalfDurationScale * angle;
    const float radius = static_cast<float>(
        std::sin(halfAngle) * trimDistance / std::cos(halfAngle));

    const bool appendStraight = !(trimDistance >= incomingLength);
    float tangentX;
    float tangentZ;
    if (appendStraight)
    {
        tangentX = backwardX * trimDistance;
        tangentX = tangentX + cornerX;
        tangentZ = static_cast<float>(
            static_cast<double>(backwardZ) * trimDistance + cornerZ);
    }
    else
    {
        tangentX = startX;
        tangentZ = startZ;
    }

    float normalX = directionZ;
    float normalZ = backwardX;
    const bool oppositeTurn =
        !(static_cast<double>(outgoingDirectionZ) * backwardX +
          static_cast<double>(outgoingDirectionX) * directionZ >= g_fZero);
    if (oppositeTurn)
    {
        normalX = backwardZ;
        normalZ = -backwardX;
    }

    const float centerX = static_cast<float>(
        static_cast<double>(normalX) * radius + tangentX);
    const float centerZ = static_cast<float>(
        tangentZ + static_cast<double>(normalZ) * radius);
    const float arcLength = static_cast<float>((g_PathPi - angle) * radius);
    float startAngle = static_cast<float>(
        GfxMatrix_ComputeArcCosine(static_cast<float>(
            -static_cast<double>(normalZ) * g_fZero - normalX)));
    if (normalZ > g_fZero)
        startAngle = -startAngle;

    alignas(8) std::uint8_t segmentBytes[0x28] = {};
    std::memset(segmentBytes + 0x10, 0, 16);
    CSF_AT(std::uint8_t, segmentBytes, 0x20) = 1;

    if (appendStraight)
    {
        CSF_AT(std::uint32_t, segmentBytes, 0x00) = 1;
        CSF_AT(float, segmentBytes, 0x04) = incomingLength - trimDistance;
        CSF_AT(float, segmentBytes, 0x08) = startX;
        CSF_AT(float, segmentBytes, 0x0C) = startZ;
        CSF_AT(float, segmentBytes, 0x10) = directionX;
        CSF_AT(float, segmentBytes, 0x14) = directionZ;

        if (CSF_AT(std::uint32_t, self, 0x0C) >=
            CSF_AT(std::uint32_t, self, 0x08))
        {
            Buffer_GrowCapacity(
                reinterpret_cast<const void**>(
                    reinterpret_cast<std::uint8_t*>(self) + 0x04),
                nullptr, CSF_AT(int, self, 0x10));
        }

        const std::uint32_t count = CSF_AT(std::uint32_t, self, 0x0C);
        auto* destination = CSF_AT(std::uint8_t*, self, 0x04) + 36 * count;
        CSF_AT(std::uint32_t, self, 0x0C) = count + 1;
        std::memcpy(destination, segmentBytes, 0x24);
    }

    CSF_AT(float, segmentBytes, 0x04) = arcLength;
    CSF_AT(float, segmentBytes, 0x18) = radius;
    CSF_AT(float, segmentBytes, 0x08) = centerX;
    CSF_AT(std::uint32_t, segmentBytes, 0x00) =
        2 * static_cast<std::uint32_t>(!oppositeTurn) + 2;
    CSF_AT(float, segmentBytes, 0x0C) = centerZ;
    CSF_AT(float, segmentBytes, 0x1C) = startAngle;

    if (CSF_AT(std::uint32_t, self, 0x0C) >=
        CSF_AT(std::uint32_t, self, 0x08))
    {
        Buffer_GrowCapacity(
            reinterpret_cast<const void**>(
                reinterpret_cast<std::uint8_t*>(self) + 0x04),
            nullptr, CSF_AT(int, self, 0x10));
    }

    const std::uint32_t count = CSF_AT(std::uint32_t, self, 0x0C);
    auto* destination = CSF_AT(std::uint8_t*, self, 0x04) + 36 * count;
    const std::uint32_t result = count + 1;
    CSF_AT(std::uint32_t, self, 0x0C) = result;
    std::memcpy(destination, segmentBytes, 0x24);
    return result;
}

HOOK(0x00426A20, Path_PrepareSegments, "auto");
