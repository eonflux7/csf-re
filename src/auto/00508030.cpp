#include <cstdint>
#include "globals.h"
#include <cstddef>
#include <cstring>
#include "hook.h"
#include "core/memory.h"

CSF_FUNC(void*, __cdecl, Mem_Alloc, 0x006910F0, (std::size_t size));
CSF_FUNC(int, __fastcall, AppMain_ReleaseStringArray, 0x00507EA0,
         (void* self, void* edx, char a2));

static std::uint32_t __fastcall AppMain_GrowStringArray(
    void* self, void* /*edx*/, int additionalCapacity)
{
    const std::uint32_t newCapacity =
        CSF_AT(std::uint32_t, self, 0x04) +
        static_cast<std::uint32_t>(additionalCapacity);

    void* allocation = Mem_Alloc(36 * newCapacity + 4);
    std::uint8_t* newElements = nullptr;

    if (allocation)
    {
        CSF_AT(std::uint32_t, allocation, 0x00) = newCapacity;
        newElements = static_cast<std::uint8_t*>(allocation) + 4;

        if (static_cast<std::int32_t>(newCapacity - 1) >= 0)
        {
            std::uint8_t* element = newElements;
            std::uint32_t remaining = newCapacity;
            do
            {
                CSF_AT(std::uint32_t, element, 0x18) = 15;
                CSF_AT(std::uint32_t, element, 0x14) = 0;
                CSF_AT(std::uint8_t, element, 0x04) = 0;
                element += 36;
            }
            while (--remaining);
        }
    }

    std::uint32_t result = CSF_AT(std::uint32_t, self, 0x08);
    if (result)
    {
        result *= 36;
        std::memcpy(newElements, CSF_AT(void*, self, 0x00), result);
    }

    const std::uint32_t updatedCapacity =
        CSF_AT(std::uint32_t, self, 0x04) +
        static_cast<std::uint32_t>(additionalCapacity);
    void* oldElements = CSF_AT(void*, self, 0x00);
    CSF_AT(std::uint32_t, self, 0x04) = updatedCapacity;

    if (oldElements)
    {
        result = static_cast<std::uint32_t>(
            AppMain_ReleaseStringArray(oldElements, nullptr, 3));
        CSF_AT(void*, self, 0x00) = nullptr;
    }

    CSF_AT(void*, self, 0x00) = newElements;
    return result;
}

HOOK(0x00508030, AppMain_GrowStringArray, "auto");
