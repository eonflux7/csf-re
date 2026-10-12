#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"

CSF_FUNC(int, __cdecl, Mem_FreeDebug, 0x00690E60, (void* block));

static int __cdecl ResetCollisionShapeRange(int firstShape, int lastShape)
{
    const std::uint32_t first = static_cast<std::uint32_t>(firstShape);
    const std::uint32_t last = static_cast<std::uint32_t>(lastShape);
    std::uint32_t current = first;

    if (current != last)
    {
        auto* entryCursor =
            reinterpret_cast<std::uint8_t*>(current + 0x38u);

        do
        {
            if (CSF_AT(std::uint32_t, entryCursor, 0) >= 0x10u)
            {
                const std::uint32_t allocation =
                    CSF_AT(std::uint32_t, entryCursor, -0x14);

                if (allocation != 0)
                    Mem_FreeDebug(reinterpret_cast<void*>(allocation));
            }

            CSF_AT(std::uint32_t, entryCursor, 0) = 15;
            CSF_AT(std::uint32_t, entryCursor, -0x04) = 0;
            CSF_AT(std::uint8_t, entryCursor, -0x14) = 0;

            if (CSF_AT(std::uint32_t, entryCursor, -0x1C) >= 0x10u)
            {
                const std::uint32_t allocation =
                    CSF_AT(std::uint32_t, entryCursor, -0x30);

                if (allocation != 0)
                    Mem_FreeDebug(reinterpret_cast<void*>(allocation));
            }

            CSF_AT(std::uint32_t, entryCursor, -0x1C) = 15;
            CSF_AT(std::uint32_t, entryCursor, -0x20) = 0;
            CSF_AT(std::uint8_t, entryCursor, -0x30) = 0;

            current += 0x64u;
            entryCursor += 0x64;
        }
        while (current != last);
    }

    return static_cast<int>(last);
}

HOOK(0x006CE270, ResetCollisionShapeRange, "auto");
