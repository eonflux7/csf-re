#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"

static std::uint32_t* __fastcall Cheats_UpdateDirectionalEntryStates(
    std::uint32_t* self, void* /*edx*/, int direction)
{
    auto* cursor = reinterpret_cast<std::uint8_t*>(self) + 0xC;

    for (int remaining = 31; remaining != 0; --remaining)
    {
        if (CSF_AT(std::uint32_t, cursor, -0x8) == 2 && direction != 0)
        {
            if (CSF_AT(std::uint32_t, cursor, -0x4) ==
                (direction > 0 ? 0u : 1u))
            {
                CSF_AT(std::uint32_t, cursor, 0x0) = 1;
                CSF_AT(std::uint32_t, cursor, 0x8) = 0;
            }
            else
            {
                CSF_AT(std::uint32_t, cursor, 0x0) = 3;
            }
        }

        if (CSF_AT(std::uint32_t, cursor, 0xC) == 2 && direction != 0)
        {
            if (CSF_AT(std::uint32_t, cursor, 0x10) ==
                (direction > 0 ? 0u : 1u))
            {
                CSF_AT(std::uint32_t, cursor, 0x14) = 1;
                CSF_AT(std::uint32_t, cursor, 0x1C) = 0;
            }
            else
            {
                CSF_AT(std::uint32_t, cursor, 0x14) = 3;
            }
        }

        if (CSF_AT(std::uint32_t, cursor, 0x20) == 2 && direction != 0)
        {
            if (CSF_AT(std::uint32_t, cursor, 0x24) ==
                (direction > 0 ? 0u : 1u))
            {
                CSF_AT(std::uint32_t, cursor, 0x28) = 1;
                CSF_AT(std::uint32_t, cursor, 0x30) = 0;
            }
            else
            {
                CSF_AT(std::uint32_t, cursor, 0x28) = 3;
            }
        }

        cursor += 0x3C;
    }

    return reinterpret_cast<std::uint32_t*>(cursor);
}

HOOK(0x00428730, Cheats_UpdateDirectionalEntryStates, "auto");
