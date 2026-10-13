#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"

CSF_FUNC(int, __cdecl, RtAnimInterpolatorAddAnimTime, 0x0061F150, (int a1, float a2));
CSF_FUNC(int, __cdecl, RtAnimInterpolatorSetCurrentTime, 0x0061F590, (int a1, float a2));

CSF_GLOBAL(float, g_MissingMetricValue, 0x0082D4C4);

static std::uint32_t __fastcall GfxAnimInterpolator_AdvanceAnimation(
    void* self, void* /*edx*/, float elapsedTime, int /*unused*/, float activity)
{
    const float deltaTime = elapsedTime * CSF_AT(float, self, 0x10);

    std::uint32_t flags = CSF_AT(std::uint32_t, self, 0x14);
    if (activity == g_MissingMetricValue)
        flags &= 0xFFFFFFFD;
    else
        flags |= 2;

    CSF_AT(std::uint32_t, self, 0x14) = flags;

    if ((flags & 6) == 0)
    {
        const std::uint32_t result = flags & 0xFFFFFFF7;
        CSF_AT(std::uint32_t, self, 0x14) = result;
        return result;
    }

    CSF_AT(std::uint32_t, self, 0x14) = flags | 8;

    void* hierarchy = CSF_AT(void*, self, 0x04);
    void* interpolator = CSF_AT(void*, hierarchy, 0x20);
    const float previousTime = CSF_AT(float, interpolator, 0x04);
    void* animation = CSF_AT(void*, interpolator, 0x00);
    const float duration = CSF_AT(float, animation, 0x0C);

    RtAnimInterpolatorAddAnimTime(
        static_cast<int>(reinterpret_cast<std::intptr_t>(
            CSF_AT(void*, hierarchy, 0x20))),
        deltaTime);

    if (previousTime + deltaTime > duration &&
        (CSF_AT(std::uint8_t, self, 0x14) & 0x10) == 0)
    {
        RtAnimInterpolatorSetCurrentTime(
            static_cast<int>(reinterpret_cast<std::intptr_t>(
                CSF_AT(void*, hierarchy, 0x20))),
            duration);
        CSF_AT(std::uint32_t, self, 0x14) |= 0x40;
    }

    std::uint32_t result = CSF_AT(std::uint32_t, self, 0x0C);
    const int nodeCount = CSF_AT(int, CSF_AT(void*, self, 0x04), 0x04);
    if (result)
    {
        for (int index = 0; index < nodeCount; ++index)
        {
            result = CSF_AT(std::uint32_t*, self, 0x0C)[index];
            if (result)
            {
                void* node = reinterpret_cast<void*>(
                    static_cast<std::uintptr_t>(result));
                CSF_AT(std::uint8_t, node, 0x20) = 1;
            }
        }
    }

    return result;
}

HOOK(0x007086D0, GfxAnimInterpolator_AdvanceAnimation, "auto");
