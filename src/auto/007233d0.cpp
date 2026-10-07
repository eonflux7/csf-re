#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"

CSF_FUNC(int, __cdecl, Mem_FreeDebug, 0x00690E60, (void* block));

static int __cdecl ResetWideStringsInRecordRange(int begin, int end)
{
    std::uint32_t current = static_cast<std::uint32_t>(begin);

    if (current != static_cast<std::uint32_t>(end))
    {
        auto* capacityField =
            reinterpret_cast<std::uint8_t*>(current + 0x30u);

        do
        {
            if (CSF_AT(std::uint32_t, capacityField, 0) >= 8u)
            {
                void* buffer = reinterpret_cast<void*>(
                    CSF_AT(std::uint32_t, capacityField, -0x14));

                if (buffer != nullptr)
                    Mem_FreeDebug(buffer);
            }

            CSF_AT(std::uint32_t, capacityField, 0) = 7;
            CSF_AT(std::uint32_t, capacityField, -4) = 0;
            CSF_AT(std::uint16_t, capacityField, -0x14) = 0;

            current += 0x3Cu;
            capacityField += 0x3C;
        }
        while (current != static_cast<std::uint32_t>(end));
    }

    return end;
}

HOOK(0x007233D0, ResetWideStringsInRecordRange, "auto");
