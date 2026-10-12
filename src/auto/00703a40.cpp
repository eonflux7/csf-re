#include <cstdint>
#include <cmath>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

CSF_FUNC(int, __cdecl, RpPTankAtomicLock, 0x005821D0, (int a1, uint32_t* a2, unsigned int a3, int a4));
CSF_FUNC(int, __cdecl, RpPTankAtomicUnlock, 0x005823E0, (int a1));
CSF_FUNC(unsigned int, __cdecl, RtRandom, 0x005867D0, ());
CSF_FUNC(int, __cdecl, RwV3dTransformPoint, 0x00623010, (int a1, int a2, int a3));

CSF_GLOBAL(float, g_HalfDurationScale, 0x0082D444);
CSF_GLOBAL(float, g_TransitionOne, 0x0082D4D4);
CSF_GLOBAL(float, g_SceneLoadMillisecondsToSeconds, 0x0082D4F0);
CSF_GLOBAL(float, g_UnsignedIntegerBias, 0x0082D744);

static int __cdecl GfxParticleRender_InitializeParticlePositions(
    int /*atomicAddress*/, int emitterAddress, int contextAddress)
{
    void* emitter = reinterpret_cast<void*>(emitterAddress);
    void* context = reinterpret_cast<void*>(contextAddress);
    void* batch = CSF_AT(void*, emitter, 0x20);
    void* properties = CSF_AT(void*, context, 0x4);

    if (!batch)
        return emitterAddress;

    const int firstParticle = CSF_AT(int, batch, 0x10);
    if (firstParticle == -1)
        return emitterAddress;

    void* tank = CSF_AT(void*, properties, 0x10);
    int remaining = CSF_AT(int, batch, 0x8) - firstParticle;
    const bool matrixOutput = (CSF_AT(uint32_t, tank, 0x0) & 8) != 0;
    void* distribution = CSF_AT(void*, properties, 0x24);

    alignas(8) uint8_t lockBytes[0x18] = {};
    RpPTankAtomicLock(
        CSF_AT(int, tank, 0x18),
        reinterpret_cast<uint32_t*>(lockBytes),
        matrixOutput ? 8 : 1,
        0x40000000);

    CSF_AT(uint8_t*, lockBytes, 0x0) +=
        CSF_AT(int, lockBytes, 0x4) *
        (CSF_AT(int, tank, 0x8) - remaining);

    const int transform =
        CSF_AT(int, CSF_AT(void*, properties, 0x0), 0xA8);

    alignas(8) uint8_t positionBytes[0xC];
    float* position = reinterpret_cast<float*>(positionBytes);
    position[0] = 0.0f;
    position[1] = 0.0f;
    position[2] = 0.0f;

    if (remaining > 0)
    {
        do
        {
            const unsigned int angleSample = RtRandom() % 0x3E8u;
            const float angle =
                (static_cast<float>(angleSample) *
                     g_SceneLoadMillisecondsToSeconds -
                 g_HalfDurationScale) *
                CSF_AT(float,
                       CSF_AT(void*, CSF_AT(void*, properties, 0x1C), 0x0),
                       0xF0);

            const unsigned int radiusSample = RtRandom() % 0x3E8u;
            const float radiusFactor =
                static_cast<float>(radiusSample) *
                    g_SceneLoadMillisecondsToSeconds -
                g_TransitionOne;

            position[0] = std::sin(angle) *
                (radiusFactor * CSF_AT(float, distribution, 0x4) +
                 CSF_AT(float, distribution, 0x0));
            position[2] = std::cos(angle) *
                (radiusFactor * CSF_AT(float, distribution, 0x4) +
                 CSF_AT(float, distribution, 0x0));

            const unsigned int heightSample = RtRandom() % 0x3E8u;
            void* origin = CSF_AT(void*, properties, 0x0);
            position[1] = static_cast<float>(heightSample) *
                g_SceneLoadMillisecondsToSeconds *
                CSF_AT(float, distribution, 0x8);

            position[0] = CSF_AT(float, origin, 0x1C) + position[0];
            position[1] = CSF_AT(float, origin, 0x20) + position[1];
            position[2] = CSF_AT(float, origin, 0x24) + position[2];

            if (transform)
            {
                const int positionAddress = reinterpret_cast<int>(position);
                RwV3dTransformPoint(positionAddress, positionAddress, transform);
            }

            if (matrixOutput)
            {
                CSF_AT(float, CSF_AT(uint8_t*, lockBytes, 0x0), 0x30) = position[0];
                CSF_AT(float, CSF_AT(uint8_t*, lockBytes, 0x0), 0x34) = position[1];
                CSF_AT(float, CSF_AT(uint8_t*, lockBytes, 0x0), 0x38) = position[2];
            }
            else
            {
                CSF_AT(float, CSF_AT(uint8_t*, lockBytes, 0x0), 0x0) = position[0];
                CSF_AT(float, CSF_AT(uint8_t*, lockBytes, 0x0), 0x4) = position[1];
                CSF_AT(float, CSF_AT(uint8_t*, lockBytes, 0x0), 0x8) = position[2];
            }

            CSF_AT(uint8_t*, lockBytes, 0x0) += CSF_AT(int, lockBytes, 0x4);
            --remaining;
        } while (remaining);
    }

    RpPTankAtomicUnlock(CSF_AT(int, tank, 0x18));
    return emitterAddress;
}

HOOK(0x00703A40, GfxParticleRender_InitializeParticlePositions, "auto");
