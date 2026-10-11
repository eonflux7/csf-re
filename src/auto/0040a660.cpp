#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"

CSF_FUNC(void*, __cdecl, _memcpy, 0x007AEE60, (void* destination, const void* source, std::size_t byteCount));
CSF_GLOBAL(float, g_fZero, 0x0082D4CC);

static double __stdcall Motion_AdvanceQueuedSegment(void* state, float elapsedTime)
{
    if (CSF_AT(std::uint32_t, state, 0x14) == 0)
        return g_fZero;

    auto* segments = CSF_AT(std::uint8_t*, state, 0x0C);
    const float acceleration = CSF_AT(float, segments, 0x08);

    if ((acceleration > g_fZero &&
         CSF_AT(float, state, 0x00) < CSF_AT(float, segments, 0x04)) ||
        (acceleration < g_fZero &&
         CSF_AT(float, state, 0x00) > CSF_AT(float, segments, 0x04)))
    {
        CSF_AT(float, state, 0x00) =
            elapsedTime * CSF_AT(float, segments, 0x08) +
            CSF_AT(float, state, 0x00);
    }

    float displacement = elapsedTime * CSF_AT(float, state, 0x00);
    bool completed = false;

    if (CSF_AT(float, state, 0x04) == CSF_AT(float, state, 0x08) &&
        displacement == g_fZero)
    {
        completed = true;
    }
    else if ((CSF_AT(float, state, 0x04) <= CSF_AT(float, state, 0x08) &&
              displacement + CSF_AT(float, state, 0x04) >=
                  CSF_AT(float, state, 0x08)) ||
             (CSF_AT(float, state, 0x04) >= CSF_AT(float, state, 0x08) &&
              displacement + CSF_AT(float, state, 0x04) <=
                  CSF_AT(float, state, 0x08)))
    {
        displacement =
            CSF_AT(float, state, 0x08) - CSF_AT(float, state, 0x04);
        completed = true;
    }

    CSF_AT(float, state, 0x04) =
        displacement + CSF_AT(float, state, 0x04);

    if (completed)
    {
        const std::uint32_t remaining =
            CSF_AT(std::uint32_t, state, 0x14) - 1;

        if (remaining == 0)
        {
            CSF_AT(std::uint32_t, state, 0x14) = 0;
        }
        else
        {
            segments = CSF_AT(std::uint8_t*, state, 0x0C);
            CSF_AT(std::uint32_t, state, 0x14) = remaining;
            _memcpy(segments, segments + 16,
                    static_cast<std::size_t>(16U * remaining));
        }

        if (CSF_AT(std::uint32_t, state, 0x14) != 0)
        {
            segments = CSF_AT(std::uint8_t*, state, 0x0C);
            const std::uint32_t initialSpeed =
                CSF_AT(std::uint32_t, segments, 0x0C);
            CSF_AT(std::uint32_t, state, 0x04) = 0;
            CSF_AT(std::uint32_t, state, 0x00) = initialSpeed;
            CSF_AT(std::uint32_t, state, 0x08) =
                CSF_AT(std::uint32_t, segments, 0x00);
            return displacement;
        }

        CSF_AT(std::uint32_t, state, 0x00) = 0;
        CSF_AT(std::uint32_t, state, 0x04) = 0;
        CSF_AT(std::uint32_t, state, 0x08) = 0;
    }

    return displacement;
}

HOOK(0x0040A660, Motion_AdvanceQueuedSegment, "auto");
