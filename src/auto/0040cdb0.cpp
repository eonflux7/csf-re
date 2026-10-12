#include <cstdint>
#include <cstddef>
#include <cstring>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"

CSF_FUNC(void*, __cdecl, Mem_Alloc, 0x006910F0, (std::size_t size));
CSF_FUNC(int, __cdecl, Mem_Free, 0x00691110, (void* block));

static int __fastcall VehicleWheel_GrowArray(
    void* self, void* /*edx*/, int extraCapacity)
{
    const std::uint32_t newCapacity =
        CSF_AT(std::uint32_t, self, 0x04) +
        static_cast<std::uint32_t>(extraCapacity);

    auto* newArray = static_cast<std::uint8_t*>(
        Mem_Alloc(24u * newCapacity));

    if (newArray &&
        static_cast<std::int32_t>(newCapacity - 1u) >= 0)
    {
        auto* element = newArray;
        std::uint32_t remaining = newCapacity;
        do
        {
            CSF_AT(std::uint32_t, element, 0x00) = 0;
            CSF_AT(std::uint32_t, element, 0x04) = 0;
            CSF_AT(std::uint32_t, element, 0x08) = 0;
            CSF_AT(std::uint8_t, element, 0x0C) = 0;
            CSF_AT(std::uint8_t, element, 0x0D) = 0;
            CSF_AT(std::int32_t, element, 0x10) = -1;
            CSF_AT(std::int32_t, element, 0x14) = -1;
            element += 24;
        }
        while (--remaining);
    }

    const std::uint32_t usedCount = CSF_AT(std::uint32_t, self, 0x08);
    if (usedCount > 0)
    {
        std::memcpy(
            newArray, CSF_AT(void*, self, 0x00), 24u * usedCount);
    }

    const std::uint32_t updatedCapacity =
        CSF_AT(std::uint32_t, self, 0x04) +
        static_cast<std::uint32_t>(extraCapacity);
    void* oldArray = CSF_AT(void*, self, 0x00);
    CSF_AT(std::uint32_t, self, 0x04) = updatedCapacity;

    int result = 0;
    if (oldArray)
    {
        result = Mem_Free(oldArray);
        CSF_AT(void*, self, 0x00) = nullptr;
    }
    CSF_AT(void*, self, 0x00) = newArray;
    return result;
}

HOOK(0x0040CDB0, VehicleWheel_GrowArray, "auto");
