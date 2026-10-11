#include <cstdint>
#include <cstddef>
#include <cstring>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_FUNC(void*, __cdecl, Mem_Alloc, 0x006910F0, (std::size_t size));
CSF_FUNC(int, __cdecl, Mem_Free, 0x00691110, (void* block));

static std::uint32_t* __fastcall S_00446340_GrowCapacity(
    S_00446340* self, void* /*edx*/, int additionalCapacity)
{
    const std::uint32_t newCapacity =
        self->field_0004 + static_cast<std::uint32_t>(additionalCapacity);

    auto* allocation = static_cast<std::uint8_t*>(
        Mem_Alloc(36u * newCapacity + 4u));
    std::uint32_t* newData = nullptr;

    if (allocation)
    {
        CSF_AT(std::uint32_t, allocation, 0) = newCapacity;
        newData = reinterpret_cast<std::uint32_t*>(allocation + 4);

        if (static_cast<std::int32_t>(newCapacity - 1u) >= 0)
        {
            auto* element = reinterpret_cast<std::uint8_t*>(newData);
            std::uint32_t remaining = newCapacity;
            do
            {
                CSF_AT(void**, element, 0x04) = CSF_VTBL(0x0082D448);
                CSF_AT(std::int32_t, element, 0x08) = -1;
                CSF_AT(std::int32_t, element, 0x0C) = -1;
                CSF_AT(std::uint32_t, element, 0x10) = 0;
                CSF_AT(std::uint32_t, element, 0x14) = 0;
                CSF_AT(std::uint32_t, element, 0x18) = 0;
                CSF_AT(std::int32_t, element, 0x00) = -1;
                CSF_AT(float, element, 0x20) = 1.0f;
                CSF_AT(std::uint32_t, element, 0x1C) = 0;
                element += 0x24;
            } while (--remaining);
        }
    }

    if (self->field_0008 != 0)
    {
        std::memcpy(
            newData,
            reinterpret_cast<const void*>(self->field_0000),
            36u * self->field_0008);
    }

    const std::uint32_t updatedCapacity =
        self->field_0004 + static_cast<std::uint32_t>(additionalCapacity);
    auto* oldData = reinterpret_cast<std::uint8_t*>(self->field_0000);
    self->field_0004 = updatedCapacity;

    if (!oldData)
    {
        self->field_0000 = reinterpret_cast<std::uintptr_t>(newData);
        return newData;
    }

    auto* oldAllocation = oldData - 4;
    const std::uint32_t oldCapacity =
        CSF_AT(std::uint32_t, oldAllocation, 0);
    auto* destructorCursor = oldData + 36u * oldCapacity;

    if (static_cast<std::int32_t>(oldCapacity - 1u) >= 0)
    {
        destructorCursor += 4;
        std::uint32_t remaining = oldCapacity;
        do
        {
            destructorCursor -= 0x24;
            CSF_AT(void**, destructorCursor, 0) = CSF_VTBL(0x0082D410);
        } while (--remaining);
    }

    const int freeResult = Mem_Free(oldAllocation);
    self->field_0000 = 0;
    self->field_0000 = reinterpret_cast<std::uintptr_t>(newData);
    return reinterpret_cast<std::uint32_t*>(
        static_cast<std::uintptr_t>(static_cast<std::uint32_t>(freeResult)));
}

HOOK(0x00446340, S_00446340_GrowCapacity, "auto");
