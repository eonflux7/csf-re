#include <cstdint>
#include "globals.h"
#include <cstddef>
#include <cstring>
#include "hook.h"
#include "core/memory.h"

CSF_FUNC(void*, __cdecl, Mem_Alloc, 0x006910F0, (std::size_t size));
CSF_FUNC(int, __cdecl, Mem_Free, 0x00691110, (void* block));

static void* __fastcall GrowPolymorphicArray(void* self, void* /*edx*/, int additionalCapacity)
{
    const std::uint32_t newCapacity =
        CSF_AT(std::uint32_t, self, 0x4) +
        static_cast<std::uint32_t>(additionalCapacity);

    auto* allocation = static_cast<std::uint8_t*>(
        Mem_Alloc(16 * newCapacity + 4));

    void* newElements = nullptr;
    if (allocation)
    {
        CSF_AT(std::uint32_t, allocation, 0x0) = newCapacity;
        newElements = allocation + 4;

        if (static_cast<std::int32_t>(newCapacity - 1) >= 0)
        {
            auto* element = static_cast<std::uint8_t*>(newElements);
            std::uint32_t remaining = newCapacity;
            do
            {
                CSF_AT(void**, element, 0x0) = CSF_VTBL(0x0082D448);
                CSF_AT(std::int32_t, element, 0x4) = -1;
                CSF_AT(std::int32_t, element, 0x8) = -1;
                element += 16;
            } while (--remaining);
        }
    }

    const std::uint32_t usedCount = CSF_AT(std::uint32_t, self, 0x8);
    if (usedCount != 0)
    {
        std::memcpy(newElements, CSF_AT(void*, self, 0x0),
                    16 * usedCount);
    }

    const std::uint32_t updatedCapacity =
        CSF_AT(std::uint32_t, self, 0x4) +
        static_cast<std::uint32_t>(additionalCapacity);
    auto* oldElements = CSF_AT(std::uint8_t*, self, 0x0);
    CSF_AT(std::uint32_t, self, 0x4) = updatedCapacity;

    if (!oldElements)
    {
        CSF_AT(void*, self, 0x0) = newElements;
        return newElements;
    }

    auto* oldAllocation = oldElements - 4;
    const std::uint32_t oldCapacity =
        CSF_AT(std::uint32_t, oldAllocation, 0x0);
    auto* element = oldElements + 16 * oldCapacity;

    if (static_cast<std::int32_t>(oldCapacity - 1) >= 0)
    {
        std::uint32_t remaining = oldCapacity;
        do
        {
            element -= 16;
            --remaining;
            CSF_AT(void**, element, 0x0) = CSF_VTBL(0x0082D410);
        } while (remaining);
    }

    const int freeResult = Mem_Free(oldAllocation);
    CSF_AT(void*, self, 0x0) = nullptr;
    CSF_AT(void*, self, 0x0) = newElements;
    return reinterpret_cast<void*>(static_cast<std::uintptr_t>(freeResult));
}

HOOK(0x00448600, GrowPolymorphicArray, "auto");
