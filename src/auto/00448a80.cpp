#include <cstdint>
#include <cstddef>
#include <cstring>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_FUNC(void*, __cdecl, Mem_Alloc, 0x006910F0, (std::size_t size));
CSF_FUNC(int, __cdecl, Mem_Free, 0x00691110, (void* block));

static std::uint32_t* __fastcall S_00448a80_GrowCapacity(
    S_00448a80* self, void* /*edx*/, int additionalCapacity)
{
    const std::uint32_t capacity =
        self->field_0004 + static_cast<std::uint32_t>(additionalCapacity);
    auto* allocation = static_cast<std::uint32_t*>(
        Mem_Alloc(24u * capacity + 4u));

    std::uint8_t* newElements = nullptr;
    if (allocation)
    {
        *allocation = capacity;
        newElements = reinterpret_cast<std::uint8_t*>(allocation + 1);

        if (static_cast<std::int32_t>(capacity - 1u) >= 0)
        {
            auto* element = newElements;
            std::uint32_t remaining = capacity;
            do
            {
                CSF_AT(void**, element, 0x00) = CSF_VTBL(0x0082D448);
                CSF_AT(std::int32_t, element, 0x04) = -1;
                CSF_AT(std::int32_t, element, 0x08) = -1;
                CSF_AT(void**, element, 0x0C) = CSF_VTBL(0x0082D448);
                CSF_AT(std::int32_t, element, 0x10) = -1;
                CSF_AT(std::int32_t, element, 0x14) = -1;
                element += 24;
            } while (--remaining);
        }
    }

    const std::uint32_t usedCount = self->field_0008;
    if (usedCount)
    {
        std::memcpy(newElements,
                    reinterpret_cast<const void*>(self->field_0000),
                    24u * usedCount);
    }

    auto* oldElements = reinterpret_cast<std::uint8_t*>(self->field_0000);
    self->field_0004 += static_cast<std::uint32_t>(additionalCapacity);

    std::uint32_t* result = reinterpret_cast<std::uint32_t*>(oldElements);
    if (oldElements)
    {
        auto* oldAllocation = reinterpret_cast<std::uint32_t*>(oldElements) - 1;
        const std::uint32_t oldCapacity = *oldAllocation;
        auto* element = reinterpret_cast<std::uint8_t*>(
            reinterpret_cast<std::uintptr_t>(oldElements) + 24u * oldCapacity);

        if (static_cast<std::int32_t>(oldCapacity - 1u) >= 0)
        {
            std::uint32_t remaining = oldCapacity;
            do
            {
                element -= 24;
                CSF_AT(void**, element, 0x0C) = CSF_VTBL(0x0082D410);
                CSF_AT(void**, element, 0x00) = CSF_VTBL(0x0082D410);
            } while (--remaining);
        }

        result = reinterpret_cast<std::uint32_t*>(
            static_cast<std::uintptr_t>(Mem_Free(oldAllocation)));
        self->field_0000 = 0;
    }

    self->field_0000 = reinterpret_cast<std::uintptr_t>(newElements);
    return result;
}

HOOK(0x00448A80, S_00448a80_GrowCapacity, "auto");
