#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"

CSF_FUNC(int, __cdecl, RpPrtStdPropTabGetPropOffset, 0x0057C8B0, (int a1, int a2));

static int __cdecl GfxParticleRender_UpdateParticleAlpha(
    int emitterAddress, int batchAddress, std::uint32_t** outputContext)
{
    std::uint32_t* output = *outputContext;
    auto* emitter = reinterpret_cast<std::uint8_t*>(emitterAddress);
    auto* batch = reinterpret_cast<std::uint8_t*>(batchAddress);
    auto* particleClass = CSF_AT(std::uint8_t*, emitter, 0x18);

    const int particleCount = CSF_AT(int, batch, 0x8);
    const int particleStride = CSF_AT(int, particleClass, 0xC);
    auto* particles = batch + CSF_AT(int, batch, 0x14);

    const int propertyOffset = RpPrtStdPropTabGetPropOffset(
        CSF_AT(int, particleClass, 0x10), 2);

    if (particleCount <= 0)
        return batchAddress;

    auto* particleFactor = particles + 12;
    auto* alphaProperty = particles + propertyOffset + 12;
    const int ageOffset = -8 - propertyOffset;
    const int lifetimeOffset = -4 - propertyOffset;

    int remaining = particleCount;
    do
    {
        if (CSF_AT(float, alphaProperty, ageOffset) <
            CSF_AT(float, alphaProperty, lifetimeOffset))
        {
            const float alpha =
                CSF_AT(float, alphaProperty, 0x10) *
                CSF_AT(float, alphaProperty, ageOffset) *
                CSF_AT(float, particleFactor, 0) +
                CSF_AT(float, alphaProperty, 0);

            CSF_AT(std::uint8_t,
                reinterpret_cast<std::uint8_t*>(CSF_AT(std::uint32_t, output, 0)),
                0x3) = static_cast<std::uint8_t>(static_cast<std::int64_t>(alpha));
        }

        alphaProperty += particleStride;
        particleFactor += particleStride;
        CSF_AT(std::uint32_t, output, 0) += CSF_AT(std::uint32_t, output, 0x4);
        --remaining;
    }
    while (remaining);

    return batchAddress;
}

HOOK(0x00703980, GfxParticleRender_UpdateParticleAlpha, "auto");
