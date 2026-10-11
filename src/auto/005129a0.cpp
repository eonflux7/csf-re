#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"

CSF_FUNC(int, __cdecl, Mem_FreeDebug, 0x00690E60, (void* block));

static int __cdecl ProfilesSavegames_ReleaseSlotBuffer(int firstSlot, int endSlot)
{
    std::uint32_t slotAddress = static_cast<std::uint32_t>(firstSlot);
    if (slotAddress != static_cast<std::uint32_t>(endSlot))
    {
        auto* capacity = reinterpret_cast<std::uint8_t*>(slotAddress + 160);
        do
        {
            if (CSF_AT(std::uint32_t, capacity, 0) >= 0x10u)
            {
                void* block = CSF_AT(void*, capacity, -0x14);
                if (block)
                    Mem_FreeDebug(block);
            }
            CSF_AT(std::uint32_t, capacity, 0) = 15;
            CSF_AT(std::uint32_t, capacity, -4) = 0;
            CSF_AT(std::uint8_t, capacity, -0x14) = 0;

            if (CSF_AT(std::uint32_t, capacity, -0x1C) >= 0x10u)
            {
                void* block = CSF_AT(void*, capacity, -0x30);
                if (block)
                    Mem_FreeDebug(block);
            }
            CSF_AT(std::uint32_t, capacity, -0x1C) = 15;
            CSF_AT(std::uint32_t, capacity, -0x20) = 0;
            CSF_AT(std::uint8_t, capacity, -0x30) = 0;

            if (CSF_AT(std::uint32_t, capacity, -0x38) >= 0x10u)
            {
                void* block = CSF_AT(void*, capacity, -0x4C);
                if (block)
                    Mem_FreeDebug(block);
            }
            CSF_AT(std::uint32_t, capacity, -0x38) = 15;
            CSF_AT(std::uint32_t, capacity, -0x3C) = 0;
            CSF_AT(std::uint8_t, capacity, -0x4C) = 0;

            if (CSF_AT(std::uint32_t, capacity, -0x88) >= 8u)
            {
                void* block = CSF_AT(void*, capacity, -0x9C);
                if (block)
                    Mem_FreeDebug(block);
            }
            CSF_AT(std::uint32_t, capacity, -0x88) = 7;
            CSF_AT(std::uint32_t, capacity, -0x8C) = 0;
            CSF_AT(std::uint16_t, capacity, -0x9C) = 0;

            slotAddress += 200;
            capacity += 200;
        }
        while (slotAddress != static_cast<std::uint32_t>(endSlot));
    }
    return endSlot;
}

HOOK(0x005129A0, ProfilesSavegames_ReleaseSlotBuffer, "auto");
