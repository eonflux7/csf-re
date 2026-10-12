#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"

CSF_FUNC(int, __cdecl, RpPrtStdPropTabGetPropOffset, 0x0057C8B0, (int a1, int a2));
CSF_FUNC(std::uint32_t*, __cdecl, RwFrameGetLTM, 0x00623720, (std::uint32_t frame));
CSF_GLOBAL(float, g_UnsignedIntegerBias, 0x0082D744);

static int __cdecl GfxParticleRender_UpdateEmitterAnimation(
    int atomicAddress, int emitterAddress, float* elapsedTime)
{
    auto* atomic = reinterpret_cast<std::uint8_t*>(atomicAddress);
    auto* emitter = reinterpret_cast<std::uint8_t*>(emitterAddress);
    void* emitterClass = CSF_AT(void*, emitter, 0x0C);

    const int propertyOffset = RpPrtStdPropTabGetPropOffset(
        CSF_AT(int, emitterClass, 0x10), 9);
    auto* animation = emitter + propertyOffset;

    if (CSF_AT(std::uint8_t, animation, 0x2A))
        return emitterAddress;

    std::uint32_t* transform = RwFrameGetLTM(
        CSF_AT(std::uint32_t, atomic, 0x04));
    const std::uint8_t looping = CSF_AT(std::uint8_t, animation, 0x10);
    void* definition = CSF_AT(void*, animation, 0x00);
    CSF_AT(std::uint32_t*, animation, 0x2C) = transform;

    const float advancedTime =
        *elapsedTime + CSF_AT(float, animation, 0x18);
    CSF_AT(float, animation, 0x18) = advancedTime;

    if ((!looping || CSF_AT(std::uint8_t, animation, 0x29)) &&
        advancedTime >= CSF_AT(float, definition, 0x24))
    {
        CSF_AT(std::uint8_t, animation, 0x2A) = 1;
        return emitterAddress;
    }

    if (looping && !CSF_AT(std::uint8_t, animation, 0x29))
    {
        if (advancedTime >= CSF_AT(float, animation, 0x1C))
        {
            do
            {
                CSF_AT(std::uint8_t, animation, 0x28) = 1;
                CSF_AT(float, animation, 0x18) =
                    CSF_AT(float, animation, 0x18) -
                    CSF_AT(float, animation, 0x1C);
            }
            while (CSF_AT(float, animation, 0x18) >=
                   CSF_AT(float, animation, 0x1C));
        }
    }
    else
    {
        if (advancedTime >= CSF_AT(float, definition, 0x24))
        {
            do
            {
                CSF_AT(float, animation, 0x18) =
                    CSF_AT(float, animation, 0x18) -
                    CSF_AT(float, definition, 0x24);
            }
            while (CSF_AT(float, animation, 0x18) >=
                   CSF_AT(float, definition, 0x24));
        }
    }

    const std::uint32_t framesBegin =
        CSF_AT(std::uint32_t, definition, 0x04);
    const float normalizedTime =
        CSF_AT(float, animation, 0x18) /
        CSF_AT(float, definition, 0x24);
    const std::int32_t frameCount = framesBegin
        ? static_cast<std::int32_t>(
              CSF_AT(std::uint32_t, definition, 0x08) - framesBegin) >> 4
        : 0;

    double unsignedFrameCount = frameCount;
    if (frameCount < 0)
        unsignedFrameCount += g_UnsignedIntegerBias;

    const std::uint8_t hasNextFrame =
        CSF_AT(std::uint8_t, animation, 0x28);
    CSF_AT(float, animation, 0x20) =
        static_cast<float>(unsignedFrameCount * normalizedTime);

    if (hasNextFrame)
    {
        const float nextTime =
            CSF_AT(float, animation, 0x1C) +
            CSF_AT(float, animation, 0x18);

        if (nextTime >= CSF_AT(float, definition, 0x24))
        {
            CSF_AT(std::uint8_t, animation, 0x28) = 0;
            return emitterAddress;
        }

        const std::uint32_t nextFramesBegin =
            CSF_AT(std::uint32_t, definition, 0x04);
        const float normalizedNextTime =
            nextTime / CSF_AT(float, definition, 0x24);
        const std::int32_t nextFrameCount = nextFramesBegin
            ? static_cast<std::int32_t>(
                  CSF_AT(std::uint32_t, definition, 0x08) - nextFramesBegin) >> 4
            : 0;

        double unsignedNextFrameCount = nextFrameCount;
        if (nextFrameCount < 0)
            unsignedNextFrameCount += g_UnsignedIntegerBias;

        CSF_AT(float, animation, 0x24) =
            static_cast<float>(unsignedNextFrameCount * normalizedNextTime);
    }

    return emitterAddress;
}

HOOK(0x00704200, GfxParticleRender_UpdateEmitterAnimation, "auto");
