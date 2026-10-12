#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"

CSF_FUNC(int, __cdecl, Mem_FreeDebug, 0x00690E60, (void* block));

static int __cdecl LevelBlock44_DestroyRange(
    std::uint32_t first,
    std::uint32_t last,
    int /*unusedA3*/,
    int /*unusedA4*/)
{
    std::uint32_t current = first;
    int result = static_cast<int>(last);

    if (current != last)
    {
        auto* entryCursor =
            reinterpret_cast<std::uint8_t*>(current + 0x64u);

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

            if (CSF_AT(std::uint32_t, entryCursor, -0x4C) >= 0x10u)
            {
                const std::uint32_t allocation =
                    CSF_AT(std::uint32_t, entryCursor, -0x60);

                if (allocation != 0)
                    Mem_FreeDebug(reinterpret_cast<void*>(allocation));
            }

            result = static_cast<int>(last);
            CSF_AT(std::uint32_t, entryCursor, -0x4C) = 15;
            CSF_AT(std::uint32_t, entryCursor, -0x50) = 0;
            CSF_AT(std::uint8_t, entryCursor, -0x60) = 0;

            current += 0xA8u;
            entryCursor += 0xA8;
        }
        while (current != last);
    }

    return result;
}

HOOK(0x007752E0, LevelBlock44_DestroyRange, "auto");
