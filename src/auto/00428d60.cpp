#include <cstdint>
#include "globals.h"
#include <cstddef>
#include <cstring>
#include "hook.h"
#include "core/memory.h"

CSF_FUNC(void*, __cdecl, Mem_Alloc, 0x006910F0, (std::size_t size));
CSF_FUNC(int, __cdecl, Mem_Free, 0x00691110, (void* block));

static const void* __fastcall Cheats_GrowEntryStorage(
    void* self, void* /*edx*/, int additionalCapacity)
{
    const std::uint32_t newCapacity =
        CSF_AT(std::uint32_t, self, 0x4) +
        static_cast<std::uint32_t>(additionalCapacity);

    auto* newEntries = static_cast<std::uint8_t*>(
        Mem_Alloc(516 * newCapacity));

    if (newEntries &&
        static_cast<std::int32_t>(newCapacity - 1) >= 0)
    {
        auto* entry = newEntries;
        std::uint32_t remaining = newCapacity;
        do
        {
            CSF_AT(std::int32_t, entry, 0x0) = -1;
            CSF_AT(std::uint16_t, entry, 0x4) = 0;
            entry += 516;
            --remaining;
        }
        while (remaining);
    }

    const std::uint32_t usedCount = CSF_AT(std::uint32_t, self, 0x8);
    if (usedCount > 0)
    {
        std::memcpy(newEntries, CSF_AT(void*, self, 0x0),
                    516 * usedCount);
    }

    const std::uint32_t updatedCapacity =
        CSF_AT(std::uint32_t, self, 0x4) +
        static_cast<std::uint32_t>(additionalCapacity);
    void* oldEntries = CSF_AT(void*, self, 0x0);
    const void* result = oldEntries;
    CSF_AT(std::uint32_t, self, 0x4) = updatedCapacity;

    if (oldEntries)
    {
        result = reinterpret_cast<const void*>(
            static_cast<std::uintptr_t>(Mem_Free(oldEntries)));
        CSF_AT(void*, self, 0x0) = nullptr;
    }

    CSF_AT(void*, self, 0x0) = newEntries;
    return result;
}

HOOK(0x00428D60, Cheats_GrowEntryStorage, "auto");
