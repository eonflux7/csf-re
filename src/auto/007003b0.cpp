#include <cstdint>
#include "globals.h"
#include <cstddef>
#include <cstring>
#include "hook.h"
#include "core/memory.h"

CSF_FUNC(void*, __cdecl, Mem_Alloc, 0x006910F0, (std::size_t size));
CSF_FUNC(int, __cdecl, Mem_Free, 0x00691110, (void* block));

static int __fastcall GfxTeam_GrowEntries(void* self, void* /*edx*/, int additionalEntries)
{
    const std::uint32_t newCapacity =
        CSF_AT(std::uint32_t, self, 0x04) +
        static_cast<std::uint32_t>(additionalEntries);

    auto* newEntries = static_cast<std::uint8_t*>(Mem_Alloc(32 * newCapacity));
    if (newEntries && static_cast<std::int32_t>(newCapacity - 1) >= 0)
    {
        auto* entry = newEntries;
        std::uint32_t remaining = newCapacity;
        do
        {
            CSF_AT(std::uint32_t, entry, 0x00) = 0;
            CSF_AT(std::uint32_t, entry, 0x04) = 0;
            CSF_AT(std::uint32_t, entry, 0x08) = 0;
            CSF_AT(std::uint32_t, entry, 0x0C) = 0;
            CSF_AT(std::uint32_t, entry, 0x10) = 0;
            CSF_AT(std::uint32_t, entry, 0x14) = 0;
            CSF_AT(std::uint8_t, entry, 0x18) = 1;
            CSF_AT(std::uint32_t, entry, 0x1C) = 0;
            entry += 32;
        } while (--remaining);
    }

    const std::uint32_t usedEntries = CSF_AT(std::uint32_t, self, 0x08);
    if (usedEntries != 0)
        std::memcpy(newEntries, CSF_AT(void*, self, 0x00), 32 * usedEntries);

    const std::uint32_t updatedCapacity =
        CSF_AT(std::uint32_t, self, 0x04) +
        static_cast<std::uint32_t>(additionalEntries);
    void* oldEntries = CSF_AT(void*, self, 0x00);
    CSF_AT(std::uint32_t, self, 0x04) = updatedCapacity;

    int result;
    if (oldEntries)
    {
        result = Mem_Free(oldEntries);
        CSF_AT(void*, self, 0x00) = nullptr;
    }
    else
    {
        result = static_cast<int>(reinterpret_cast<std::intptr_t>(newEntries));
    }

    CSF_AT(void*, self, 0x00) = newEntries;
    return result;
}

HOOK(0x007003B0, GfxTeam_GrowEntries, "auto");
