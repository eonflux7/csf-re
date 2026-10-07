#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"

CSF_FUNC(int, __cdecl, Mem_FreeDebug, 0x00690E60, (void* block));

static int __cdecl ClearRecordStringRange(
    std::uint32_t first,
    std::uint32_t last)
{
    std::uint32_t current = first;

    if (current != last)
    {
        auto* capacityCursor =
            reinterpret_cast<std::uint8_t*>(current + 0x34u);

        do
        {
            if (CSF_AT(std::uint32_t, capacityCursor, 0) >= 0x10u)
            {
                const std::uint32_t allocation =
                    CSF_AT(std::uint32_t, capacityCursor, -0x14);

                if (allocation != 0)
                    Mem_FreeDebug(reinterpret_cast<void*>(allocation));
            }

            CSF_AT(std::uint32_t, capacityCursor, 0) = 15;
            CSF_AT(std::uint32_t, capacityCursor, -0x04) = 0;
            CSF_AT(std::uint8_t, capacityCursor, -0x14) = 0;

            if (CSF_AT(std::uint32_t, capacityCursor, -0x1C) >= 0x10u)
            {
                const std::uint32_t allocation =
                    CSF_AT(std::uint32_t, capacityCursor, -0x30);

                if (allocation != 0)
                    Mem_FreeDebug(reinterpret_cast<void*>(allocation));
            }

            CSF_AT(std::uint32_t, capacityCursor, -0x1C) = 15;
            CSF_AT(std::uint32_t, capacityCursor, -0x20) = 0;
            CSF_AT(std::uint8_t, capacityCursor, -0x30) = 0;

            current += 0x40u;
            capacityCursor += 0x40;
        }
        while (current != last);
    }

    return static_cast<int>(last);
}

HOOK(0x0071B600, ClearRecordStringRange, "auto");
