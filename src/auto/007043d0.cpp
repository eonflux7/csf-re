#include <cstdint>
#include "globals.h"
#include "hook.h"

CSF_FUNC(double, __cdecl, ClampContactComponent, 0x0040C920, (float* a1, int a2, int a3));
CSF_FUNC(int, __cdecl, RpPrtAdvEmtSphereEmitCB, 0x0057BBD0, (int a1, int a2));
CSF_FUNC(int, __cdecl, RpPrtStdPropTabGetPropOffset, 0x0057C8B0, (int a1, int a2));
CSF_FUNC(unsigned int, __cdecl, RtRandom, 0x005867D0, ());
CSF_FUNC(int, __cdecl, GfxParticleRender_EmitParticles, 0x00703A40, (int a1, int a2, int a3));

CSF_GLOBAL(float, g_fZero, 0x0082D4CC);
CSF_GLOBAL(float, g_TransitionOne, 0x0082D4D4);
CSF_GLOBAL(float, g_SceneLoadMillisecondsToSeconds, 0x0082D4F0);
CSF_GLOBAL(float, g_UnsignedIntegerBias, 0x0082D744);
CSF_GLOBAL(float, g_ZeroVector2X, 0x008BB448);
CSF_GLOBAL(float, g_ZeroVector2Y, 0x008BB44C);

static int __cdecl GfxParticleRender_ApplyEmissionVariation(
    int atomic, int emitterAddress, int contextAddress)
{
    auto* emitter = reinterpret_cast<uint8_t*>(emitterAddress);
    auto* batch = CSF_AT(uint8_t*, emitter, 0x20);
    if (!batch || CSF_AT(int, batch, 0x10) == -1)
        return emitterAddress;

    auto* context = reinterpret_cast<uint8_t*>(contextAddress);
    void* contextData = CSF_AT(void*, context, 0x04);
    void* settingsTable = CSF_AT(void*, contextData, 0x1C);
    void* settings = CSF_AT(void*, settingsTable, 0x00);

    const int emissionMode = CSF_AT(int, settings, 0xD4);
    if (emissionMode == 1)
        GfxParticleRender_EmitParticles(atomic, emitterAddress, contextAddress);
    else if (emissionMode == 2)
        RpPrtAdvEmtSphereEmitCB(atomic, emitterAddress);

    const bool varyScale =
        CSF_AT(float, settings, 0xB4) != g_ZeroVector2X ||
        CSF_AT(float, settings, 0xB8) != g_ZeroVector2Y;
    const bool varyColor = CSF_AT(float, settings, 0xC8) != g_fZero;
    if (!varyScale && !varyColor)
        return emitterAddress;

    void* particleClass = CSF_AT(void*, emitter, 0x18);
    const int firstParticle = CSF_AT(int, batch, 0x10);
    int remaining = CSF_AT(int, batch, 0x08) - firstParticle;
    const int stride = CSF_AT(int, particleClass, 0x0C);
    uint8_t* particle = batch + CSF_AT(int, batch, 0x14) + stride * firstParticle;

    const int scaleOffset = RpPrtStdPropTabGetPropOffset(
        CSF_AT(int, particleClass, 0x10), 5);
    particleClass = CSF_AT(void*, emitter, 0x18);
    const int colorOffset = RpPrtStdPropTabGetPropOffset(
        CSF_AT(int, particleClass, 0x10), 2);

    if (remaining <= 0)
        return emitterAddress;

    uint8_t* scale = particle + scaleOffset;
    uint8_t* color = particle + colorOffset;

    alignas(8) uint8_t candidateBytes[0x04];
    alignas(8) uint8_t redMinimumBytes[0x04];
    alignas(8) uint8_t redMaximumBytes[0x14];
    alignas(8) uint8_t greenMinimumBytes[0x04];
    alignas(8) uint8_t greenMaximumBytes[0x04];
    alignas(8) uint8_t blueMinimumBytes[0x04];
    alignas(8) uint8_t blueMaximumBytes[0x08];

    do
    {
        // Advancing only eligible particles is intentional.
        if (!(CSF_AT(float, color, 4 - colorOffset) > g_fZero))
        {
            if (varyScale)
            {
                const bool horizontalPositive =
                    CSF_AT(float, settings, 0xB4) > g_fZero;
                const bool uniform = CSF_AT(uint8_t, settings, 0xBC) != 0;
                const unsigned int horizontalRandom = RtRandom() % 0x3E8u;
                const double horizontalVariation =
                    static_cast<double>(horizontalRandom) *
                    g_SceneLoadMillisecondsToSeconds *
                    CSF_AT(float, settings, 0xB4);
                double factor = horizontalPositive
                    ? horizontalVariation + g_TransitionOne
                    : g_TransitionOne / (g_TransitionOne - horizontalVariation);

                CSF_AT(float, scale, 0x00) =
                    static_cast<float>(factor * CSF_AT(float, scale, 0x00));

                if (uniform)
                {
                    CSF_AT(float, scale, 0x04) =
                        static_cast<float>(factor * CSF_AT(float, scale, 0x04));
                    CSF_AT(float, scale, 0x08) =
                        static_cast<float>(factor * CSF_AT(float, scale, 0x08));
                }
                else
                {
                    CSF_AT(float, scale, 0x08) =
                        static_cast<float>(factor * CSF_AT(float, scale, 0x08));

                    const bool verticalPositive =
                        CSF_AT(float, settings, 0xB8) > g_fZero;
                    const unsigned int verticalRandom = RtRandom() % 0x3E8u;
                    const double verticalVariation =
                        static_cast<double>(verticalRandom) *
                        g_SceneLoadMillisecondsToSeconds *
                        CSF_AT(float, settings, 0xB8);
                    factor = verticalPositive
                        ? verticalVariation + g_TransitionOne
                        : g_TransitionOne / (g_TransitionOne - verticalVariation);

                    CSF_AT(float, scale, 0x04) =
                        static_cast<float>(factor * CSF_AT(float, scale, 0x04));
                }

                CSF_AT(float, scale, 0x0C) =
                    static_cast<float>(factor * CSF_AT(float, scale, 0x0C));
            }

            if (varyColor)
            {
                const unsigned int random = RtRandom() % 0x3E8u;
                CSF_AT(int, redMaximumBytes, 0) = 1132396544;
                CSF_AT(int, redMinimumBytes, 0) = 0;
                const float variation = static_cast<float>(
                    (static_cast<double>(random) * g_SceneLoadMillisecondsToSeconds +
                     static_cast<double>(random) * g_SceneLoadMillisecondsToSeconds -
                     g_TransitionOne) * CSF_AT(float, settings, 0xC8));

                CSF_AT(float, candidateBytes, 0) =
                    variation + CSF_AT(float, settings, 0x44);
                CSF_AT(float, color, 0x00) = static_cast<float>(
                    ClampContactComponent(
                        reinterpret_cast<float*>(candidateBytes),
                        reinterpret_cast<int>(redMinimumBytes),
                        reinterpret_cast<int>(redMaximumBytes)));

                CSF_AT(float, candidateBytes, 0) =
                    variation + CSF_AT(float, settings, 0x48);
                CSF_AT(int, greenMaximumBytes, 0) = 1132396544;
                CSF_AT(int, greenMinimumBytes, 0) = 0;
                CSF_AT(float, color, 0x04) = static_cast<float>(
                    ClampContactComponent(
                        reinterpret_cast<float*>(candidateBytes),
                        reinterpret_cast<int>(greenMinimumBytes),
                        reinterpret_cast<int>(greenMaximumBytes)));

                CSF_AT(float, candidateBytes, 0) =
                    variation + CSF_AT(float, settings, 0x4C);
                CSF_AT(int, blueMaximumBytes, 0) = 1132396544;
                CSF_AT(int, blueMinimumBytes, 0) = 0;
                CSF_AT(float, color, 0x08) = static_cast<float>(
                    ClampContactComponent(
                        reinterpret_cast<float*>(candidateBytes),
                        reinterpret_cast<int>(blueMinimumBytes),
                        reinterpret_cast<int>(blueMaximumBytes)));
            }

            scale += stride;
            color += stride;
        }
    } while (--remaining);

    return emitterAddress;
}

HOOK(0x007043D0, GfxParticleRender_ApplyEmissionVariation, "auto");
