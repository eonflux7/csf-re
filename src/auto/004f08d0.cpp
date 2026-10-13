#include <cstdint>
#include <cstddef>
#include <cstring>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"

CSF_FUNC(void*, __cdecl, Mem_Alloc, 0x006910F0, (std::size_t size));
CSF_FUNC(int, __fastcall, SceneLightsObjects_DestroyEntryArray, 0x004F0810,
         (void* self, void* edx, char a2));

static unsigned int __fastcall SceneLightsObjects_GrowObjectArray(
    void* self, void* /*edx*/, int additionalCapacity)
{
    const std::uint32_t capacity =
        CSF_AT(std::uint32_t, self, 0x4) +
        static_cast<std::uint32_t>(additionalCapacity);

    auto* allocation = static_cast<std::uint8_t*>(
        Mem_Alloc(40u * capacity + 4u));
    std::uint8_t* entries = nullptr;

    if (allocation)
    {
        CSF_AT(std::uint32_t, allocation, 0x0) = capacity;
        entries = allocation + 4;

        if (static_cast<std::int32_t>(capacity - 1u) >= 0)
        {
            auto* entry = entries;
            std::uint32_t remaining = capacity;
            do
            {
                CSF_AT(std::uint32_t, entry, 0x18) = 15;
                CSF_AT(std::uint32_t, entry, 0x14) = 0;
                CSF_AT(std::uint8_t, entry, 0x4) = 0;
                CSF_AT(std::int32_t, entry, 0x1C) = -1;
                CSF_AT(std::uint32_t, entry, 0x20) = 0;
                CSF_AT(std::uint32_t, entry, 0x24) = 0;
                entry += 40;
            } while (--remaining);
        }
    }

    unsigned int result = CSF_AT(std::uint32_t, self, 0x8);
    if (result)
    {
        result *= 40;
        std::memcpy(entries, CSF_AT(void*, self, 0x0), result);
    }

    const std::uint32_t updatedCapacity =
        CSF_AT(std::uint32_t, self, 0x4) +
        static_cast<std::uint32_t>(additionalCapacity);
    void* oldEntries = CSF_AT(void*, self, 0x0);
    CSF_AT(std::uint32_t, self, 0x4) = updatedCapacity;

    if (oldEntries)
    {
        result = SceneLightsObjects_DestroyEntryArray(oldEntries, nullptr, 3);
        CSF_AT(void*, self, 0x0) = nullptr;
    }

    CSF_AT(void*, self, 0x0) = entries;
    return result;
}

HOOK(0x004F08D0, SceneLightsObjects_GrowObjectArray, "auto");
