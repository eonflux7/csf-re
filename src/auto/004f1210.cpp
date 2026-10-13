#include <cstdint>
#include <cstddef>
#include <cstring>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_FUNC(void*, __cdecl, Mem_Alloc, 0x006910F0, (std::size_t size));
CSF_FUNC(int, __cdecl, Mem_Free, 0x00691110, (void* block));

static std::uint32_t* __fastcall S_004f1210_ReserveAdditionalSlots(
    S_004f1210* self, void* /*edx*/, int additionalSlots)
{
    const std::uint32_t capacity =
        self->field_0004 + static_cast<std::uint32_t>(additionalSlots);
    void* allocation = Mem_Alloc(40 * capacity + 4);
    std::uint32_t* replacement = nullptr;

    if (allocation)
    {
        replacement = reinterpret_cast<std::uint32_t*>(
            static_cast<std::uint8_t*>(allocation) + 4);
        CSF_AT(std::uint32_t, replacement, -4) = capacity;

        if (static_cast<std::int32_t>(capacity - 1) >= 0)
        {
            auto* element = reinterpret_cast<std::uint8_t*>(replacement);
            std::uint32_t remaining = capacity;
            do
            {
                CSF_AT(void**, element, 0x08) = CSF_VTBL(0x0082D448);
                CSF_AT(std::int32_t, element, 0x0C) = -1;
                CSF_AT(std::int32_t, element, 0x10) = -1;
                CSF_AT(std::uint32_t, element, 0x14) = 0;
                CSF_AT(std::uint32_t, element, 0x18) = 0;
                CSF_AT(std::uint32_t, element, 0x1C) = 0;
                CSF_AT(std::int32_t, element, 0x00) = -1;
                CSF_AT(std::int32_t, element, 0x04) = -1;
                CSF_AT(std::uint32_t, element, 0x20) = 0x7F7FFFFFu;
                CSF_AT(std::uint32_t, element, 0x24) = 0;
                element += 40;
            } while (--remaining);
        }
    }

    if (self->field_0008)
    {
        std::memcpy(replacement,
                    reinterpret_cast<const void*>(self->field_0000),
                    40 * self->field_0008);
    }

    auto* oldElements = reinterpret_cast<std::uint8_t*>(self->field_0000);
    self->field_0004 += static_cast<std::uint32_t>(additionalSlots);

    if (!oldElements)
    {
        self->field_0000 = static_cast<std::uint32_t>(
            reinterpret_cast<std::uintptr_t>(replacement));
        return replacement;
    }

    const std::uint32_t oldCapacity = CSF_AT(std::uint32_t, oldElements, -4);
    std::uint8_t* elementEnd = oldElements + 40 * oldCapacity;
    if (static_cast<std::int32_t>(oldCapacity - 1) >= 0)
    {
        std::uint32_t remaining = oldCapacity;
        do
        {
            elementEnd -= 40;
            CSF_AT(void**, elementEnd, 0x08) = CSF_VTBL(0x0082D410);
        } while (--remaining);
    }

    const int freeResult = Mem_Free(oldElements - 4);
    self->field_0000 = 0;
    self->field_0000 = static_cast<std::uint32_t>(
        reinterpret_cast<std::uintptr_t>(replacement));
    return reinterpret_cast<std::uint32_t*>(
        static_cast<std::uintptr_t>(freeResult));
}

HOOK(0x004F1210, S_004f1210_ReserveAdditionalSlots, "auto");
