#include <cstdint>
#include <cstddef>
#include <cstring>
#include <cmath>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_FUNC(double, __cdecl, GfxMatrix_ComputeArcCosine, 0x0040C980, (float a1));
CSF_FUNC(const void*, __fastcall, Buffer_GrowCapacity, 0x00425CE0, (const void** self, void* , int additionalCapacity));
CSF_FUNC(void*, __cdecl, _memcpy, 0x007AEE60, (void* destination, const void* source, std::size_t byteCount));

CSF_GLOBAL(float, g_HalfDurationScale, 0x0082D444);
CSF_GLOBAL(float, g_fZero, 0x0082D4CC);
CSF_GLOBAL(float, g_TransitionOne, 0x0082D4D4);
CSF_GLOBAL(float, g_PathPi, 0x0082FE80);

static void __fastcall S_00426550_BuildCurrentPathSegments(
    S_00426550* self, void* /*edx*/)
{
    const std::uint32_t index = self->field_0024;
    auto* points = reinterpret_cast<std::uint8_t*>(self->field_0014);
    auto* current = points + 20 * index;

    const float startX = CSF_AT(float, current, 0x00);
    const float startZ = CSF_AT(float, current, 0x08);
    const float cornerX = CSF_AT(float, current, 0x14);
    const float cornerZ = CSF_AT(float, current, 0x1C);
    const double deltaX = static_cast<double>(cornerX) - startX;
    const double deltaZ = static_cast<double>(cornerZ) - startZ;
    const float startTrim = CSF_AT(float, current, 0x0C);
    const float edgeLength = static_cast<float>(std::sqrt(deltaX * deltaX + deltaZ * deltaZ));
    const double inverseLength = static_cast<double>(g_TransitionOne) / edgeLength;
    const float directionX = inverseLength * deltaX;
    const float directionZ = inverseLength * deltaZ;
    const float trimmedStartX = directionX * startTrim + startX;
    const float trimmedStartZ = directionZ * startTrim + startZ;

    alignas(8) std::uint8_t segment[0x24] = {};

    if (index >= self->field_001c - 2)
    {
        if (startTrim < edgeLength)
        {
            CSF_AT(std::uint32_t, segment, 0x00) = 1;
            CSF_AT(float, segment, 0x04) = edgeLength - startTrim;
            CSF_AT(float, segment, 0x08) = trimmedStartX;
            CSF_AT(float, segment, 0x0C) = trimmedStartZ;
            CSF_AT(float, segment, 0x10) = directionX;
            CSF_AT(float, segment, 0x14) = directionZ;
            std::memset(segment + 0x18, 0, 9);

            if (self->field_000c >= CSF_AT(std::uint32_t, self, 0x08))
            {
                Buffer_GrowCapacity(
                    reinterpret_cast<const void**>(
                        reinterpret_cast<std::uint8_t*>(self) + 0x04),
                    nullptr, CSF_AT(int, self, 0x10));
            }
            const std::uint32_t count = self->field_000c;
            auto* destination =
                reinterpret_cast<std::uint8_t*>(self->field_0004) + 36 * count;
            self->field_000c = count + 1;
            std::memcpy(destination, segment, 0x24);
        }
        return;
    }

    const std::uint32_t nextIndex = index + 2;
    auto* next = points + 20 * nextIndex;
    float outgoingX = CSF_AT(float, next, 0x00) - cornerX;
    float outgoingZ = CSF_AT(float, next, 0x08) - cornerZ;
    float cornerTrim = CSF_AT(float, current, 0x20);
    double outgoingLength =
        std::sqrt(static_cast<double>(outgoingX) * outgoingX +
                  static_cast<double>(outgoingZ) * outgoingZ);
    double inverseOutgoingLength = g_TransitionOne / outgoingLength;
    outgoingX *= inverseOutgoingLength;
    outgoingZ *= inverseOutgoingLength;

    if (cornerTrim > outgoingLength)
    {
        for (;;)
        {
            if (nextIndex >= self->field_001c - 1)
            {
                cornerTrim = outgoingLength;
                break;
            }

            const std::uint32_t newCount = self->field_001c - 1;
            self->field_001c = newCount;
            if (nextIndex != newCount)
            {
                auto* destination =
                    reinterpret_cast<std::uint8_t*>(self->field_0014) +
                    20 * nextIndex;
                _memcpy(destination, destination + 20,
                        20 * (newCount - nextIndex));
            }

            next = reinterpret_cast<std::uint8_t*>(self->field_0014) +
                   20 * nextIndex;
            outgoingX = CSF_AT(float, next, 0x00) - cornerX;
            outgoingZ = CSF_AT(float, next, 0x08) - cornerZ;
            outgoingLength =
                std::sqrt(static_cast<double>(outgoingX) * outgoingX +
                          static_cast<double>(outgoingZ) * outgoingZ);
            inverseOutgoingLength = g_TransitionOne / outgoingLength;
            outgoingX *= inverseOutgoingLength;
            outgoingZ *= inverseOutgoingLength;
            if (!(cornerTrim > outgoingLength))
                break;
        }
    }

    auto* followingTrim =
        reinterpret_cast<std::uint8_t*>(self->field_0014) +
        20 * self->field_0024 + 0x34;
    if (cornerTrim + CSF_AT(float, followingTrim, 0x00) >= outgoingLength)
    {
        if (g_HalfDurationScale * outgoingLength < cornerTrim)
            cornerTrim = g_HalfDurationScale * outgoingLength;
        CSF_AT(float, followingTrim, 0x00) = outgoingLength - cornerTrim;
    }
    CSF_AT(float,
           reinterpret_cast<std::uint8_t*>(self->field_0014) +
               20 * self->field_0024,
           0x20) = cornerTrim;

    const bool omitStraight = !(cornerTrim + startTrim < edgeLength);
    float arcStartX;
    float arcStartZ;
    if (!omitStraight)
    {
        arcStartX = -directionX * cornerTrim + cornerX;
        arcStartZ = -directionZ * cornerTrim + cornerZ;
    }
    else
    {
        arcStartX = trimmedStartX;
        arcStartZ = trimmedStartZ;
    }

    const float reverseX = -directionX;
    const float reverseZ = -directionZ;
    const double cornerAngle = GfxMatrix_ComputeArcCosine(
        reverseZ * outgoingZ + reverseX * outgoingX);

    float perpendicularX = directionZ;
    float perpendicularZ = reverseX;
    bool positiveTurn = true;
    const float radius = static_cast<float>(
        std::sin(g_HalfDurationScale * cornerAngle) * cornerTrim /
        std::cos(g_HalfDurationScale * cornerAngle));

    if (outgoingZ * reverseX + outgoingX * directionZ < g_fZero)
    {
        perpendicularX = reverseZ;
        perpendicularZ = -reverseX;
        positiveTurn = false;
    }

    const float centerX = perpendicularX * radius + arcStartX;
    const float centerZ = arcStartZ + perpendicularZ * radius;
    const float arcLength =
        static_cast<float>((g_PathPi - cornerAngle) * radius);
    float startAngle = static_cast<float>(GfxMatrix_ComputeArcCosine(
        -perpendicularZ * g_fZero - perpendicularX));
    if (perpendicularZ > g_fZero)
        startAngle = -startAngle;

    std::memset(segment + 0x10, 0, 17);
    if (!omitStraight)
    {
        CSF_AT(std::uint32_t, segment, 0x00) = 1;
        CSF_AT(float, segment, 0x04) = edgeLength - startTrim - cornerTrim;
        CSF_AT(float, segment, 0x08) = trimmedStartX;
        CSF_AT(float, segment, 0x0C) = trimmedStartZ;
        CSF_AT(float, segment, 0x10) = directionX;
        CSF_AT(float, segment, 0x14) = directionZ;

        if (self->field_000c >= CSF_AT(std::uint32_t, self, 0x08))
        {
            Buffer_GrowCapacity(
                reinterpret_cast<const void**>(
                    reinterpret_cast<std::uint8_t*>(self) + 0x04),
                nullptr, CSF_AT(int, self, 0x10));
        }
        const std::uint32_t count = self->field_000c;
        auto* destination =
            reinterpret_cast<std::uint8_t*>(self->field_0004) + 36 * count;
        self->field_000c = count + 1;
        std::memcpy(destination, segment, 0x24);
    }

    CSF_AT(std::uint32_t, segment, 0x00) = 2 * (positiveTurn != 0) + 2;
    CSF_AT(float, segment, 0x04) = arcLength;
    CSF_AT(float, segment, 0x08) = centerX;
    CSF_AT(float, segment, 0x0C) = centerZ;
    CSF_AT(float, segment, 0x18) = radius;
    CSF_AT(float, segment, 0x1C) = startAngle;

    if (self->field_000c >= CSF_AT(std::uint32_t, self, 0x08))
    {
        Buffer_GrowCapacity(
            reinterpret_cast<const void**>(
                reinterpret_cast<std::uint8_t*>(self) + 0x04),
            nullptr, CSF_AT(int, self, 0x10));
    }
    const std::uint32_t count = self->field_000c;
    auto* destination =
        reinterpret_cast<std::uint8_t*>(self->field_0004) + 36 * count;
    self->field_000c = count + 1;
    std::memcpy(destination, segment, 0x24);
}

HOOK(0x00426550, S_00426550_BuildCurrentPathSegments, "auto");
