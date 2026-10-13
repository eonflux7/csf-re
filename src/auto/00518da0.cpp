#include <cstdint>
#include "globals.h"
#include <cstddef>
#include <cstring>
#include "hook.h"
#include "core/memory.h"

CSF_FUNC(void*, __cdecl, Mem_Alloc, 0x006910F0, (std::size_t size));
CSF_FUNC(int, __cdecl, Mem_Free, 0x00691110, (void* block));

static std::uint32_t __fastcall NetProfile_GrowEntryArray(
    void* self, void* /*edx*/, int additionalCapacity)
{
    const std::uint32_t newCapacity =
        CSF_AT(std::uint32_t, self, 0x4) +
        static_cast<std::uint32_t>(additionalCapacity);

    auto* allocation = static_cast<std::uint8_t*>(
        Mem_Alloc(24u * newCapacity + 4u));
    std::uint8_t* newEntries = nullptr;

    if (allocation)
    {
        CSF_AT(std::uint32_t, allocation, 0x0) = newCapacity;
        newEntries = allocation + 4;

        if (static_cast<std::int32_t>(newCapacity - 1u) >= 0)
        {
            auto* entry = newEntries;
            std::uint32_t remaining = newCapacity;
            do
            {
                CSF_AT(void**, entry, 0xC) = CSF_VTBL(0x0082D448);
                CSF_AT(std::int32_t, entry, 0x10) = -1;
                CSF_AT(std::int32_t, entry, 0x14) = -1;
                entry += 24;
            } while (--remaining);
        }
    }

    std::uint32_t result = CSF_AT(std::uint32_t, self, 0x8);
    if (result != 0)
    {
        result *= 24u;
        std::memcpy(newEntries, CSF_AT(void*, self, 0x0), result);
    }

    const std::uint32_t updatedCapacity =
        CSF_AT(std::uint32_t, self, 0x4) +
        static_cast<std::uint32_t>(additionalCapacity);
    auto* oldEntries = static_cast<std::uint8_t*>(
        CSF_AT(void*, self, 0x0));
    CSF_AT(std::uint32_t, self, 0x4) = updatedCapacity;

    if (oldEntries)
    {
        const std::uint32_t oldCapacity =
            CSF_AT(std::uint32_t, oldEntries, -4);
        auto* entry = oldEntries + 24u * oldCapacity;

        if (static_cast<std::int32_t>(oldCapacity - 1u) >= 0)
        {
            std::uint32_t remaining = oldCapacity;
            do
            {
                entry -= 24;
                CSF_AT(void**, entry, 0xC) = CSF_VTBL(0x0082D410);
            } while (--remaining);
        }

        result = static_cast<std::uint32_t>(Mem_Free(oldEntries - 4));
        CSF_AT(void*, self, 0x0) = nullptr;
    }

    CSF_AT(void*, self, 0x0) = newEntries;
    return result;
}

HOOK(0x00518DA0, NetProfile_GrowEntryArray, "auto");
