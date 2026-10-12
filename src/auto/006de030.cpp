#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"

CSF_GLOBAL(float, g_fZero, 0x0082D4CC);
CSF_GLOBAL(float, g_MessageBoxAnimationScale, 0x0082D7C0);
CSF_GLOBAL(float, g_flt_0082DC84, 0x0082DC84);
CSF_GLOBAL(float, g_PositionHeightOffset, 0x0082DDE8);

static int __fastcall GfxTextureLoader_UpdateScaledParticleParameters(
    void* self, void* /*edx*/)
{
    double scaledValue;

    if (CSF_AT(float, self, 0x14) == g_fZero)
    {
        const auto count = static_cast<std::int32_t>(
            CSF_AT(std::uint32_t, self, 0x88) +
            CSF_AT(std::uint32_t, self, 0x0C));

        scaledValue =
            (CSF_AT(float, self, 0x90) + CSF_AT(float, self, 0x18)) /
            CSF_AT(float, self, 0x10) * static_cast<double>(count);
    }
    else
    {
        const auto count = static_cast<std::int32_t>(
            CSF_AT(std::uint32_t, self, 0x88) +
            CSF_AT(std::uint32_t, self, 0x0C));

        scaledValue = static_cast<double>(count) *
            ((CSF_AT(float, self, 0x10) + CSF_AT(float, self, 0x14)) /
             CSF_AT(float, self, 0x10));
    }

    if (scaledValue < g_fZero)
        scaledValue = g_fZero;
    else if (scaledValue > g_PositionHeightOffset)
        scaledValue = g_PositionHeightOffset;

    const auto quantized = static_cast<std::int64_t>(
        (scaledValue + g_MessageBoxAnimationScale) * g_flt_0082DC84);
    const auto result = static_cast<std::uint32_t>(quantized);

    CSF_AT(std::uint32_t, self, 0x08) = 10 * result;
    return static_cast<std::int32_t>(result);
}

HOOK(0x006DE030, GfxTextureLoader_UpdateScaledParticleParameters, "auto");
