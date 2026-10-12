#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"

CSF_FUNC(int, __cdecl, RpPrtStdPropTabGetPropOffset, 0x0057C8B0, (int a1, int a2));
CSF_FUNC(int, __cdecl, RpPTankAtomicLock, 0x005821D0, (int a1, uint32_t* a2, unsigned int a3, int a4));
CSF_FUNC(int, __cdecl, RpPTankAtomicUnlock, 0x005823E0, (int a1));

CSF_GLOBAL(float, g_TransitionOne, 0x0082D4D4);

static int __cdecl GfxParticleRender_ClampParticleAlpha(int /*context*/, int emitterAddress)
{
    auto* emitter = reinterpret_cast<uint8_t*>(
        static_cast<uintptr_t>(emitterAddress));

    auto* emitterClass = reinterpret_cast<void*>(
        CSF_AT(uint32_t, emitter, 0x0C));
    const int alphaOffset = RpPrtStdPropTabGetPropOffset(
        CSF_AT(int, emitterClass, 0x10), 10);

    emitterClass = reinterpret_cast<void*>(
        CSF_AT(uint32_t, emitter, 0x0C));
    const int tankOffset = RpPrtStdPropTabGetPropOffset(
        CSF_AT(int, emitterClass, 0x10), 6);

    uint8_t* alphaProperty =
        alphaOffset == -1 ? nullptr : emitter + alphaOffset;
    uint8_t* tankProperty =
        tankOffset == -1 ? nullptr : emitter + tankOffset;

    if (CSF_AT(float, alphaProperty, 0x04) < g_TransitionOne)
    {
        auto* alphaSettings = reinterpret_cast<uint8_t*>(
            CSF_AT(uint32_t, alphaProperty, 0x00));

        const float* maximumAlpha =
            CSF_AT(float, alphaSettings, 0x50) <
                    CSF_AT(float, alphaSettings, 0x60)
                ? &CSF_AT(float, alphaSettings, 0x60)
                : &CSF_AT(float, alphaSettings, 0x50);

        const int32_t alphaLimit = static_cast<int32_t>(
            static_cast<int64_t>(
                CSF_AT(float, alphaProperty, 0x04) * *maximumAlpha));

        alignas(8) uint8_t lockBytes[0x08]{};
        auto* lock = reinterpret_cast<uint32_t*>(lockBytes);

        RpPTankAtomicLock(
            CSF_AT(int, tankProperty, 0x18), lock, 2, 0x40000000);

        uint32_t index = 0;
        if (CSF_AT(uint32_t, tankProperty, 0x08) != 0)
        {
            uint32_t cursor = lock[0];
            do
            {
                auto* color = reinterpret_cast<void*>(cursor);
                if (static_cast<int32_t>(CSF_AT(uint8_t, color, 0x03)) >
                    alphaLimit)
                {
                    CSF_AT(uint8_t, color, 0x03) =
                        static_cast<uint8_t>(alphaLimit);
                    cursor = lock[0];
                }

                cursor += lock[1];
                const uint32_t particleCount =
                    CSF_AT(uint32_t, tankProperty, 0x08);
                ++index;
                lock[0] = cursor;
                if (index >= particleCount)
                    break;
            } while (true);
        }

        RpPTankAtomicUnlock(CSF_AT(int, tankProperty, 0x18));
    }

    return emitterAddress;
}

HOOK(0x00703C40, GfxParticleRender_ClampParticleAlpha, "auto");
