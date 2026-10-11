#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"

CSF_FUNC(int, __cdecl, Mem_FreeDebug, 0x00690E60, (void* block));

static int __cdecl ResetConfigRecordStringRange(
    std::uint32_t first,
    std::uint32_t last)
{
    std::uint32_t current = first;

    if (current != last)
    {
        auto* fieldCursor =
            reinterpret_cast<std::uint8_t*>(current + 0x48u);

        do
        {
            if (CSF_AT(std::uint32_t, fieldCursor, 0) >= 0x10u)
            {
                const std::uint32_t allocation =
                    CSF_AT(std::uint32_t, fieldCursor, -0x14);

                if (allocation != 0)
                    Mem_FreeDebug(reinterpret_cast<void*>(allocation));
            }

            CSF_AT(std::uint32_t, fieldCursor, 0) = 15;
            CSF_AT(std::uint32_t, fieldCursor, -0x04) = 0;
            CSF_AT(std::uint8_t, fieldCursor, -0x14) = 0;

            if (CSF_AT(std::uint32_t, fieldCursor, -0x2C) >= 0x10u)
            {
                const std::uint32_t allocation =
                    CSF_AT(std::uint32_t, fieldCursor, -0x40);

                if (allocation != 0)
                    Mem_FreeDebug(reinterpret_cast<void*>(allocation));
            }

            CSF_AT(std::uint32_t, fieldCursor, -0x2C) = 15;
            CSF_AT(std::uint32_t, fieldCursor, -0x30) = 0;
            CSF_AT(std::uint8_t, fieldCursor, -0x40) = 0;

            current += 0x50u;
            fieldCursor += 0x50;
        }
        while (current != last);
    }

    return static_cast<int>(last);
}

HOOK(0x00755BE0, ResetConfigRecordStringRange, "auto");
