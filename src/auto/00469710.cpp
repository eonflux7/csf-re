#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"

CSF_FUNC(int, __cdecl, Mem_FreeDebug, 0x00690E60, (void* block));

static int __cdecl MissionScript_ResetEntryRange(int begin, int end)
{
    uint32_t current = static_cast<uint32_t>(begin);
    if (current != static_cast<uint32_t>(end))
    {
        auto* capacityField = reinterpret_cast<uint8_t*>(current + 0x30);
        do
        {
            if (CSF_AT(uint32_t, capacityField, 0) >= 0x10u)
            {
                void* block = CSF_AT(void*, capacityField, -0x14);
                if (block)
                    Mem_FreeDebug(block);
            }

            CSF_AT(uint32_t, capacityField, 0) = 15;
            CSF_AT(uint32_t, capacityField, -4) = 0;
            CSF_AT(uint8_t, capacityField, -0x14) = 0;
            current += 0x44;
            capacityField += 0x44;
        }
        while (current != static_cast<uint32_t>(end));
    }
    return end;
}

HOOK(0x00469710, MissionScript_ResetEntryRange, "auto");
