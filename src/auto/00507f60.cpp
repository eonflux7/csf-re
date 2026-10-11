#include <cstdint>
#include "globals.h"
#include <cstddef>
#include <cstring>
#include "hook.h"
#include "core/memory.h"

CSF_FUNC(void*, __cdecl, Mem_Alloc, 0x006910F0, (std::size_t size));
CSF_FUNC(int, __fastcall, sub_507DE0, 0x00507DE0, (void* self, void* edx, char a2));

static int __fastcall AppMain_GrowEntryCapacity(void* self, void* /*edx*/, int additionalCapacity)
{
    const std::uint32_t newCapacity =
        CSF_AT(std::uint32_t, self, 0x4) +
        static_cast<std::uint32_t>(additionalCapacity);

    auto* allocation = static_cast<std::uint8_t*>(
        Mem_Alloc(44 * newCapacity + 4));

    std::uint8_t* newEntries = nullptr;
    if (allocation)
    {
        CSF_AT(std::uint32_t, allocation, 0x0) = newCapacity;
        newEntries = allocation + 4;

        if (static_cast<std::int32_t>(newCapacity - 1) >= 0)
        {
            std::uint32_t remaining = newCapacity;
            std::uint8_t* entry = newEntries;
            do
            {
                CSF_AT(std::uint32_t, entry, 0x18) = 15;
                CSF_AT(std::uint32_t, entry, 0x14) = 0;
                CSF_AT(std::uint8_t, entry, 0x4) = 0;
                CSF_AT(std::uint32_t, entry, 0x1C) = 0;
                CSF_AT(std::uint32_t, entry, 0x20) = 0;
                CSF_AT(std::uint32_t, entry, 0x24) = 0;
                CSF_AT(float, entry, 0x28) = 1.0f;
                entry += 44;
            } while (--remaining);
        }
    }

    const std::uint32_t entryCount = CSF_AT(std::uint32_t, self, 0x8);
    if (entryCount > 0)
    {
        std::memcpy(
            newEntries,
            CSF_AT(void*, self, 0x0),
            44 * entryCount);
    }

    const std::uint32_t updatedCapacity =
        CSF_AT(std::uint32_t, self, 0x4) +
        static_cast<std::uint32_t>(additionalCapacity);
    void* oldEntries = CSF_AT(void*, self, 0x0);
    CSF_AT(std::uint32_t, self, 0x4) = updatedCapacity;

    int result;
    if (oldEntries)
    {
        result = sub_507DE0(oldEntries, nullptr, 3);
        CSF_AT(void*, self, 0x0) = nullptr;
    }
    else
    {
        result = static_cast<int>(reinterpret_cast<std::uintptr_t>(newEntries));
    }

    CSF_AT(void*, self, 0x0) = newEntries;
    return result;
}

HOOK(0x00507F60, AppMain_GrowEntryCapacity, "auto");
