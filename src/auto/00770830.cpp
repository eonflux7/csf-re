#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"

CSF_GLOBAL(float, g_fZero, 0x0082D4CC);

static int __fastcall SoundAreas_NormalizeEntryPositions(void* self, void* /*edx*/)
{
    if (CSF_AT(float, self, 0x54) < g_fZero)
    {
        const float extent = -CSF_AT(float, self, 0x54);
        CSF_AT(float, self, 0x54) = extent;
        CSF_AT(float, self, 0x50) =
            CSF_AT(float, self, 0x50) - extent;
    }

    std::uint32_t offset = CSF_AT(std::uint32_t, self, 0x24);
    std::int32_t entryCount = 0;
    if (offset != 0)
    {
        const std::int32_t byteCount = static_cast<std::int32_t>(
            CSF_AT(std::uint32_t, self, 0x28) - offset);
        entryCount = byteCount / 12;
        offset = static_cast<std::uint32_t>(715827883LL * byteCount);
    }

    std::uint32_t processed = 0;
    if (entryCount >= 4)
    {
        std::uint32_t groups =
            (static_cast<std::uint32_t>(entryCount - 4) >> 2) + 1;
        processed = 4 * groups;
        offset = 0;
        do
        {
            CSF_AT(std::uint32_t,
                reinterpret_cast<void*>(
                    CSF_AT(std::uint32_t, self, 0x24) + offset), 4) =
                CSF_AT(std::uint32_t, self, 0x50);
            CSF_AT(std::uint32_t,
                reinterpret_cast<void*>(
                    CSF_AT(std::uint32_t, self, 0x24) + offset), 16) =
                CSF_AT(std::uint32_t, self, 0x50);
            CSF_AT(std::uint32_t,
                reinterpret_cast<void*>(
                    CSF_AT(std::uint32_t, self, 0x24) + offset), 28) =
                CSF_AT(std::uint32_t, self, 0x50);
            CSF_AT(std::uint32_t,
                reinterpret_cast<void*>(
                    CSF_AT(std::uint32_t, self, 0x24) + offset), 40) =
                CSF_AT(std::uint32_t, self, 0x50);
            offset += 48;
        } while (--groups != 0);
    }

    if (processed < static_cast<std::uint32_t>(entryCount))
    {
        offset = 12 * processed;
        std::uint32_t remaining =
            static_cast<std::uint32_t>(entryCount) - processed;
        do
        {
            CSF_AT(std::uint32_t,
                reinterpret_cast<void*>(
                    CSF_AT(std::uint32_t, self, 0x24) + offset), 4) =
                CSF_AT(std::uint32_t, self, 0x50);
            offset += 12;
        } while (--remaining != 0);
    }

    return static_cast<std::int32_t>(offset);
}

HOOK(0x00770830, SoundAreas_NormalizeEntryPositions, "auto");
