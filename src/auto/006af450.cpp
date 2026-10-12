#include <cstdint>
#include <cmath>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"

CSF_FUNC(void*, __fastcall, VecGrow_4DE9B0, 0x004DE9B0, (void* self, void* edx, int a2));
CSF_GLOBAL(float, g_PointDistanceTolerance, 0x008600D0);

static void __fastcall AppendDistinctPoint(void* self, void* /*edx*/, const void* point)
{
    if (CSF_AT(std::uint32_t, self, 0x0C) != 0)
    {
        const std::uint32_t count = CSF_AT(std::uint32_t, self, 0x0C);
        const auto* last = CSF_AT(std::uint8_t*, self, 0x04) + (12 * count - 12);

        const float dx = CSF_AT(float, last, 0x00) - CSF_AT(float, point, 0x00);
        const float dy = CSF_AT(float, last, 0x04) - CSF_AT(float, point, 0x04);
        const float dz = CSF_AT(float, last, 0x08) - CSF_AT(float, point, 0x08);

        if (!(std::sqrt(dz * dz + dx * dx + dy * dy) > g_PointDistanceTolerance))
            return;
    }

    if (CSF_AT(std::uint32_t, self, 0x0C) >= CSF_AT(std::uint32_t, self, 0x08))
    {
        VecGrow_4DE9B0(static_cast<std::uint8_t*>(self) + 0x04,
                      nullptr, CSF_AT(int, self, 0x10));
    }

    const std::uint32_t count = CSF_AT(std::uint32_t, self, 0x0C);
    auto* data = CSF_AT(std::uint8_t*, self, 0x04);
    CSF_AT(std::uint32_t, self, 0x0C) = count + 1;

    auto* destination = data + 12 * count;
    CSF_AT(std::uint32_t, destination, 0x00) = CSF_AT(std::uint32_t, point, 0x00);
    CSF_AT(std::uint32_t, destination, 0x04) = CSF_AT(std::uint32_t, point, 0x04);
    CSF_AT(std::uint32_t, destination, 0x08) = CSF_AT(std::uint32_t, point, 0x08);
}

HOOK(0x006AF450, AppendDistinctPoint, "auto");
