#include <cstdint>
#include <cstddef>
#include <cstring>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_FUNC(int, __cdecl, CoreMemory_AllocUntagged, 0x006910F0, (std::size_t size));
CSF_FUNC(int, __cdecl, CoreMemory_Free, 0x00691110, (void* block));

static void* __fastcall GrowPolymorphicArray(
    void* self, void* /*edx*/, int additionalCapacity)
{
    const std::uint32_t capacity =
        CSF_AT(std::uint32_t, self, 4) +
        static_cast<std::uint32_t>(additionalCapacity);

    auto* allocation = reinterpret_cast<std::uint8_t*>(
        CoreMemory_AllocUntagged(28 * capacity + 4));

    std::uint8_t* newElements = nullptr;
    if (allocation)
    {
        CSF_AT(std::uint32_t, allocation, 0) = capacity;
        newElements = allocation + 4;

        if (static_cast<std::int32_t>(capacity - 1) >= 0)
        {
            auto* element = newElements;
            std::uint32_t remaining = capacity;
            do
            {
                CSF_AT(void**, element, 0) = CSF_VTBL(0x0082D448);
                CSF_AT(std::int32_t, element, 4) = -1;
                CSF_AT(std::int32_t, element, 8) = -1;
                CSF_AT(std::uint32_t, element, 0x18) = 0;
                element += 28;
            } while (--remaining);
        }
    }

    void* result = newElements;
    const std::uint32_t usedCount = CSF_AT(std::uint32_t, self, 8);
    if (usedCount > 0)
    {
        std::memcpy(
            newElements,
            CSF_AT(void*, self, 0),
            static_cast<std::uint32_t>(28 * usedCount));
    }

    auto* oldElements = CSF_AT(std::uint8_t*, self, 0);
    CSF_AT(std::uint32_t, self, 4) +=
        static_cast<std::uint32_t>(additionalCapacity);

    if (oldElements)
    {
        auto* oldAllocation = oldElements - 4;
        const std::uint32_t oldCapacity =
            CSF_AT(std::uint32_t, oldAllocation, 0);
        auto* element = oldElements +
            static_cast<std::uint32_t>(28 * oldCapacity);

        if (static_cast<std::int32_t>(oldCapacity - 1) >= 0)
        {
            std::uint32_t remaining = oldCapacity;
            do
            {
                element -= 28;
                CSF_AT(void**, element, 0) = CSF_VTBL(0x0082D410);
            } while (--remaining);
        }

        result = reinterpret_cast<void*>(CoreMemory_Free(oldAllocation));
        CSF_AT(void*, self, 0) = nullptr;
        CSF_AT(void*, self, 0) = newElements;
    }
    else
    {
        CSF_AT(void*, self, 0) = newElements;
    }

    return result;
}

HOOK(0x0049F330, GrowPolymorphicArray, "auto");
