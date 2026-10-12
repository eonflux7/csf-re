#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"

CSF_FUNC(int, __cdecl, RpPTankAtomicLock, 0x005821D0, (int a1, uint32_t* a2, unsigned int a3, int a4));
CSF_FUNC(int, __cdecl, RpPTankAtomicUnlock, 0x005823E0, (int a1));

CSF_GLOBAL(float, g_TransitionOne, 0x0082D4D4);
CSF_GLOBAL(uint32_t, g_DefaultParamA_8BB42C, 0x008BB42C);

static int __cdecl GfxParticleRender_RenderInterpolatedParticles(
    void* emitter, int batch, void* context)
{
    void* renderContext = CSF_AT(void*, context, 0x4);
    void* tank = CSF_AT(void*, renderContext, 0x10);
    void* state = CSF_AT(void*, renderContext, 0x18);

    if (CSF_AT(uint8_t, state, 0x2A))
        return batch;

    void* animation = CSF_AT(void*, state, 0x0);
    const int currentFrame =
        static_cast<int>(static_cast<int64_t>(CSF_AT(float, state, 0x20)));
    const float fraction = CSF_AT(float, state, 0x20) - static_cast<float>(currentFrame);
    const float currentWeight = g_TransitionOne - fraction;
    const int additionalFrame =
        static_cast<int>(static_cast<int64_t>(CSF_AT(float, state, 0x24)));

    int additionalCount = 0;
    if (CSF_AT(uint8_t, state, 0x28))
    {
        auto* frame = static_cast<uint8_t*>(CSF_AT(void*, animation, 0x4))
                    + 16 * additionalFrame;
        const uint32_t begin = CSF_AT(uint32_t, frame, 0x4);
        if (begin)
        {
            additionalCount =
                static_cast<int32_t>(CSF_AT(uint32_t, frame, 0x8) - begin) / 28;
        }
    }

    auto* currentFrameEntry = static_cast<uint8_t*>(CSF_AT(void*, animation, 0x4))
                            + 16 * currentFrame;
    const uint32_t currentBegin = CSF_AT(uint32_t, currentFrameEntry, 0x4);
    CSF_AT(int, state, 0x14) = currentBegin
        ? static_cast<int32_t>(
              CSF_AT(uint32_t, currentFrameEntry, 0x8) - currentBegin) / 28
        : 0;

    alignas(8) uint8_t positionLock[0x8];
    alignas(8) uint8_t sizeLock[0x8];
    alignas(8) uint8_t colorLock[0x8];
    alignas(8) uint8_t rotationLock[0x70];

    RpPTankAtomicLock(CSF_AT(int, tank, 0x18),
                     reinterpret_cast<uint32_t*>(positionLock), 1, 0x40000000);
    RpPTankAtomicLock(CSF_AT(int, tank, 0x18),
                     reinterpret_cast<uint32_t*>(sizeLock), 4, 0x40000000);
    RpPTankAtomicLock(CSF_AT(int, tank, 0x18),
                     reinterpret_cast<uint32_t*>(colorLock), 2, 0x40000000);
    RpPTankAtomicLock(CSF_AT(int, tank, 0x18),
                     reinterpret_cast<uint32_t*>(rotationLock), 32, 0x40000000);

    auto* positions = CSF_AT(uint8_t*, positionLock, 0x0);
    auto* sizes = CSF_AT(uint8_t*, sizeLock, 0x0);
    auto* colors = CSF_AT(uint8_t*, colorLock, 0x0);
    auto* rotations = CSF_AT(uint8_t*, rotationLock, 0x0);

    const int currentCount = CSF_AT(int, state, 0x14);
    const int totalCount = currentCount + additionalCount;
    if (totalCount > CSF_AT(int, animation, 0x10))
        return 0;

    int currentOffset = 0;
    int additionalOffset = -28 * currentCount;
    for (int particleIndex = 0; particleIndex < totalCount; ++particleIndex)
    {
        const bool fromCurrentFrame = particleIndex < currentCount;
        const int frameIndex = fromCurrentFrame ? currentFrame : additionalFrame;
        const int particleOffset = fromCurrentFrame ? currentOffset : additionalOffset;
        auto* frames = static_cast<uint8_t*>(CSF_AT(void*, animation, 0x4));
        auto* particle = static_cast<uint8_t*>(
            CSF_AT(void*, frames + 16 * frameIndex, 0x4)) + particleOffset;
        const int nextParticleIndex = CSF_AT(int, particle, 0x18);

        float x, y, z, size, rotation, colorBlend;
        if (nextParticleIndex == -1)
        {
            size = CSF_AT(float, particle, 0xC);
            rotation = CSF_AT(float, particle, 0x10);
            x = CSF_AT(float, particle, 0x0);
            y = CSF_AT(float, particle, 0x4);
            z = CSF_AT(float, particle, 0x8);
            colorBlend = CSF_AT(float, particle, 0x14);
        }
        else
        {
            auto* nextParticle = static_cast<uint8_t*>(
                CSF_AT(void*, frames + 16 * (frameIndex + 1), 0x4))
                + 28 * nextParticleIndex;

            x = currentWeight * CSF_AT(float, particle, 0x0)
              + fraction * CSF_AT(float, nextParticle, 0x0);
            y = currentWeight * CSF_AT(float, particle, 0x4)
              + fraction * CSF_AT(float, nextParticle, 0x4);
            z = currentWeight * CSF_AT(float, particle, 0x8)
              + fraction * CSF_AT(float, nextParticle, 0x8);
            colorBlend = fraction * CSF_AT(float, nextParticle, 0x14)
                       + currentWeight * CSF_AT(float, particle, 0x14);
            size = fraction * CSF_AT(float, nextParticle, 0xC)
                 + currentWeight * CSF_AT(float, particle, 0xC);
            rotation = fraction * CSF_AT(float, nextParticle, 0x10)
                     + currentWeight * CSF_AT(float, particle, 0x10);
        }

        if (!CSF_AT(uint8_t, state, 0x4))
        {
            void* transform = CSF_AT(void*, state, 0x2C);
            const float translationX = CSF_AT(float, transform, 0x30);
            const float translationY = CSF_AT(float, transform, 0x34);
            const float translationZ = CSF_AT(float, transform, 0x38);
            x = translationX + x;
            y = translationY + y;
            z = translationZ + z;
        }

        uint32_t color;
        if (CSF_AT(uint8_t, state, 0x5))
        {
            const float inverseBlend = g_TransitionOne - colorBlend;
            const uint8_t alpha = static_cast<uint8_t>(static_cast<int64_t>(
                CSF_AT(uint8_t, state, 0xE) * inverseBlend
                + CSF_AT(uint8_t, state, 0xF) * colorBlend));
            const uint8_t red = static_cast<uint8_t>(static_cast<int64_t>(
                CSF_AT(uint8_t, state, 0x6) * inverseBlend
                + CSF_AT(uint8_t, state, 0xA) * colorBlend));
            const uint8_t green = static_cast<uint8_t>(static_cast<int64_t>(
                CSF_AT(uint8_t, state, 0x7) * inverseBlend
                + CSF_AT(uint8_t, state, 0xB) * colorBlend));
            const uint8_t blue = static_cast<uint8_t>(static_cast<int64_t>(
                CSF_AT(uint8_t, state, 0x8) * inverseBlend
                + CSF_AT(uint8_t, state, 0xC) * colorBlend));
            color = uint32_t(red) | (uint32_t(green) << 8)
                  | (uint32_t(blue) << 16) | (uint32_t(alpha) << 24);
        }
        else
        {
            color = g_DefaultParamA_8BB42C;
        }

        CSF_AT(float, positions, 0x0) = x;
        CSF_AT(float, positions, 0x4) = y;
        CSF_AT(float, positions, 0x8) = z;
        CSF_AT(float, sizes, 0x0) = size;
        CSF_AT(float, sizes, 0x4) = size;
        CSF_AT(uint32_t, colors, 0x0) = color;
        CSF_AT(float, rotations, 0x0) = rotation;

        sizes += CSF_AT(uint32_t, sizeLock, 0x4);
        positions += CSF_AT(uint32_t, positionLock, 0x4);
        colors += CSF_AT(uint32_t, colorLock, 0x4);
        rotations += CSF_AT(uint32_t, rotationLock, 0x4);
        currentOffset += 28;
        additionalOffset += 28;
    }

    RpPTankAtomicUnlock(CSF_AT(int, tank, 0x18));
    const int activeCount = additionalCount + CSF_AT(int, state, 0x14);
    CSF_AT(int, state, 0x14) = activeCount;
    CSF_AT(int, tank, 0x8) = activeCount;
    CSF_AT(int, emitter, 0x10) = CSF_AT(int, state, 0x14);
    return batch;
}

HOOK(0x00704D60, GfxParticleRender_RenderInterpolatedParticles, "auto");
