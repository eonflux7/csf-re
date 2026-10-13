#include <cstdint>
#include <cstddef>
#include <cstring>
#include <cmath>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

CSF_FUNC(long double, __cdecl, GfxMatrix_ComputeArcCosine, 0x0040C980, (float a1));
CSF_FUNC(void*, __fastcall, Buffer_PadToMinimumLength, 0x00425CE0, (void* self, void* edx, int a2));
CSF_FUNC(void*, __cdecl, _memcpy, 0x007AEE60, (void* destination, const void* source, std::size_t byteCount));

CSF_GLOBAL(float, g_HalfDurationScale, 0x0082D444);
CSF_GLOBAL(float, g_fZero, 0x0082D4CC);
CSF_GLOBAL(float, g_TransitionOne, 0x0082D4D4);
CSF_GLOBAL(double, g_PathPi, 0x0082FE80);

static void __fastcall S_00426550_RunInitStep(S_00426550* self, void* /*edx*/)
{
    const uint32_t index = self->field_0024;
    auto* current = reinterpret_cast<uint8_t*>(self->field_0014 + 20 * index);
    const float startX = CSF_AT(float, current, 0);
    const float startZ = CSF_AT(float, current, 8);
    const float cornerX = CSF_AT(float, current, 20);
    const float cornerZ = CSF_AT(float, current, 28);
    const float deltaX = cornerX - startX;
    const float deltaZ = cornerZ - startZ;
    const float startTrim = CSF_AT(float, current, 12);
    const float length = std::sqrt(deltaZ * deltaZ + deltaX * deltaX);
    const float inverseLength = g_TransitionOne / length;
    const float directionX = inverseLength * deltaX;
    const float directionZ = inverseLength * deltaZ;
    const float lineStartX = directionX * startTrim + startX;
    const float lineStartZ = directionZ * startTrim + startZ;

    alignas(8) uint8_t segment[0x24]{};

    if (index >= self->field_001c - 2)
    {
        if (startTrim < length)
        {
            CSF_AT(uint32_t, segment, 0) = 1;
            CSF_AT(float, segment, 4) = length - startTrim;
            CSF_AT(float, segment, 8) = lineStartX;
            CSF_AT(float, segment, 12) = lineStartZ;
            CSF_AT(float, segment, 16) = directionX;
            CSF_AT(float, segment, 20) = directionZ;
            std::memset(segment + 24, 0, 9);

            if (self->field_000c >= CSF_AT(uint32_t, self, 0x08))
                Buffer_PadToMinimumLength(
                    &self->field_0004, nullptr, CSF_AT(int, self, 0x10));

            const uint32_t count = self->field_000c;
            const uint32_t data = self->field_0004;
            self->field_000c = count + 1;
            std::memcpy(reinterpret_cast<void*>(data + 36 * count), segment, 0x24);
        }
        return;
    }

    const uint32_t nextIndex = index + 2;
    auto* next = reinterpret_cast<uint8_t*>(self->field_0014 + 20 * nextIndex);
    float nextDeltaX = CSF_AT(float, next, 0) - cornerX;
    float nextDeltaZ = CSF_AT(float, next, 8) - cornerZ;
    float endTrim = CSF_AT(float, current, 32);
    double nextLength = std::sqrt(
        static_cast<double>(nextDeltaZ) * nextDeltaZ +
        static_cast<double>(nextDeltaX) * nextDeltaX);
    float nextDirectionX = static_cast<float>(nextDeltaX * (g_TransitionOne / nextLength));
    float nextDirectionZ = static_cast<float>((g_TransitionOne / nextLength) * nextDeltaZ);

    if (endTrim > nextLength)
    {
        bool foundLongEnough = false;
        while (nextIndex < self->field_001c - 1)
        {
            const uint32_t newCount = self->field_001c - 1;
            self->field_001c = newCount;
            if (nextIndex != newCount)
            {
                auto* destination =
                    reinterpret_cast<uint8_t*>(self->field_0014 + 20 * nextIndex);
                _memcpy(destination, destination + 20, 20 * (newCount - nextIndex));
            }

            next = reinterpret_cast<uint8_t*>(self->field_0014 + 20 * nextIndex);
            nextDeltaX = CSF_AT(float, next, 0) - cornerX;
            nextDeltaZ = CSF_AT(float, next, 8) - cornerZ;
            nextLength = std::sqrt(
                static_cast<double>(nextDeltaZ) * nextDeltaZ +
                static_cast<double>(nextDeltaX) * nextDeltaX);
            nextDirectionX = static_cast<float>(nextDeltaX * (g_TransitionOne / nextLength));
            nextDirectionZ = static_cast<float>((g_TransitionOne / nextLength) * nextDeltaZ);
            if (!(endTrim > nextLength))
            {
                foundLongEnough = true;
                break;
            }
        }
        if (!foundLongEnough)
            endTrim = static_cast<float>(nextLength);
    }

    auto* followingTrim = reinterpret_cast<uint8_t*>(
        self->field_0014 + 20 * self->field_0024 + 52);
    if (endTrim + CSF_AT(float, followingTrim, 0) >= nextLength)
    {
        if (g_HalfDurationScale * nextLength < endTrim)
            endTrim = static_cast<float>(g_HalfDurationScale * nextLength);
        CSF_AT(float, followingTrim, 0) = static_cast<float>(nextLength - endTrim);
    }

    CSF_AT(float, reinterpret_cast<void*>(self->field_0014),
           20 * self->field_0024 + 32) = endTrim;

    const bool omitStraight = !(endTrim + startTrim < length);
    const float tangentX = omitStraight ? lineStartX : -directionX * endTrim + cornerX;
    const float tangentZ = omitStraight ? lineStartZ : -directionZ * endTrim + cornerZ;
    const float reverseX = -directionX;
    const float reverseZ = -directionZ;
    const double angle = static_cast<double>(GfxMatrix_ComputeArcCosine(
        reverseZ * nextDirectionZ + reverseX * nextDirectionX));
    float normalX = directionZ;
    float normalZ = reverseX;
    bool clockwise = true;
    const float radius = static_cast<float>(
        std::sin(g_HalfDurationScale * angle) * endTrim /
        std::cos(g_HalfDurationScale * angle));

    if (nextDirectionZ * reverseX + nextDirectionX * directionZ < g_fZero)
    {
        normalX = reverseZ;
        normalZ = -reverseX;
        clockwise = false;
    }

    const float centerX = normalX * radius + tangentX;
    const float centerZ = tangentZ + normalZ * radius;
    const float arcLength = static_cast<float>((g_PathPi - angle) * radius);
    float startAngle = static_cast<float>(
        GfxMatrix_ComputeArcCosine(-normalZ * g_fZero - normalX));
    if (normalZ > g_fZero)
        startAngle = -startAngle;

    std::memset(segment + 16, 0, 17);
    if (!omitStraight)
    {
        CSF_AT(uint32_t, segment, 0) = 1;
        CSF_AT(float, segment, 4) = length - startTrim - endTrim;
        CSF_AT(float, segment, 8) = lineStartX;
        CSF_AT(float, segment, 12) = lineStartZ;
        CSF_AT(float, segment, 16) = directionX;
        CSF_AT(float, segment, 20) = directionZ;

        if (self->field_000c >= CSF_AT(uint32_t, self, 0x08))
            Buffer_PadToMinimumLength(
                &self->field_0004, nullptr, CSF_AT(int, self, 0x10));

        const uint32_t count = self->field_000c;
        void* destination = reinterpret_cast<void*>(self->field_0004 + 36 * count);
        self->field_000c = count + 1;
        std::memcpy(destination, segment, 0x24);
    }

    CSF_AT(uint32_t, segment, 0) = 2 * (clockwise != 0) + 2;
    CSF_AT(float, segment, 4) = arcLength;
    CSF_AT(float, segment, 8) = centerX;
    CSF_AT(float, segment, 12) = centerZ;
    CSF_AT(float, segment, 24) = radius;
    CSF_AT(float, segment, 28) = startAngle;

    if (self->field_000c >= CSF_AT(uint32_t, self, 0x08))
        Buffer_PadToMinimumLength(
            &self->field_0004, nullptr, CSF_AT(int, self, 0x10));

    const uint32_t count = self->field_000c;
    void* destination = reinterpret_cast<void*>(self->field_0004 + 36 * count);
    self->field_000c = count + 1;
    std::memcpy(destination, segment, 0x24);
}

HOOK(0x00426550, S_00426550_RunInitStep, "auto");
