#include <cstdint>
#include "globals.h"
#include <cstddef>
#include <cstring>
#include "hook.h"
#include "core/memory.h"

CSF_FUNC(void*, __cdecl, Mem_Alloc, 0x006910F0, (std::size_t size));
CSF_FUNC(int, __cdecl, Mem_Free, 0x00691110, (void* block));

static int __fastcall GfxSkin_GrowEntries(void* self, void* /*edx*/, int additionalCapacity)
{
    const std::uint32_t capacity =
        CSF_AT(std::uint32_t, self, 0x04) +
        static_cast<std::uint32_t>(additionalCapacity);

    auto* newEntries = static_cast<std::uint8_t*>(Mem_Alloc(12u * capacity));
    int result = 0;

    if (newEntries && static_cast<std::int32_t>(capacity - 1u) >= 0)
    {
        auto* entry = newEntries;
        std::uint32_t remaining = capacity;
        do
        {
            CSF_AT(std::uint32_t, entry, 0x00) = 0;
            CSF_AT(float, entry, 0x04) = -1.0f;
            CSF_AT(std::uint16_t, entry, 0x08) = 0;
            entry += 12;
        } while (--remaining);
    }

    const std::uint32_t entryCount = CSF_AT(std::uint32_t, self, 0x08);
    if (entryCount > 0)
    {
        std::memcpy(newEntries, CSF_AT(void*, self, 0x00), 12u * entryCount);
    }

    const std::uint32_t updatedCapacity =
        CSF_AT(std::uint32_t, self, 0x04) +
        static_cast<std::uint32_t>(additionalCapacity);
    void* oldEntries = CSF_AT(void*, self, 0x00);
    CSF_AT(std::uint32_t, self, 0x04) = updatedCapacity;

    if (oldEntries)
    {
        result = Mem_Free(oldEntries);
        CSF_AT(void*, self, 0x00) = nullptr;
    }

    CSF_AT(void*, self, 0x00) = newEntries;
    return result;
}

HOOK(0x006C5B30, GfxSkin_GrowEntries, "auto");
