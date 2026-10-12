#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"

CSF_FUNC(int, __cdecl, Mem_FreeDebug, 0x00690E60, (void* block));

static int __cdecl GfxScreenEffects_ResetEntryRange(int begin, int end)
{
    uint32_t current = static_cast<uint32_t>(begin);
    if (current != static_cast<uint32_t>(end))
    {
        auto* capacityField = reinterpret_cast<uint8_t*>(current + 28);
        do
        {
            if (CSF_AT(uint32_t, capacityField, 0) >= 0x10u)
            {
                void* block = CSF_AT(void*, capacityField, -20);
                if (block)
                    Mem_FreeDebug(block);
            }

            CSF_AT(uint32_t, capacityField, 0) = 15;
            CSF_AT(uint32_t, capacityField, -4) = 0;
            CSF_AT(uint8_t, capacityField, -20) = 0;
            current += 44;
            capacityField += 44;
        }
        while (current != static_cast<uint32_t>(end));
    }
    return end;
}

HOOK(0x006F8810, GfxScreenEffects_ResetEntryRange, "auto");
