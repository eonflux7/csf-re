#include <cstdint>
#include <cstddef>
#include <cstring>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

CSF_FUNC(void*, __cdecl, Mem_Alloc, 0x006910F0, (std::size_t size));
CSF_FUNC(int, __cdecl, Mem_Free, 0x00691110, (void* block));

static std::uint32_t* __fastcall S_004dca10_GrowCapacity(
    S_004dca10* self, void* /*edx*/, int additionalCapacity)
{
    const std::uint32_t capacity =
        self->field_0004 + static_cast<std::uint32_t>(additionalCapacity);

    auto* allocation = static_cast<std::uint8_t*>(
        Mem_Alloc(28u * capacity + 4u));
    std::uint32_t* newData = nullptr;

    if (allocation)
    {
        CSF_AT(std::uint32_t, allocation, 0) = capacity;
        newData = reinterpret_cast<std::uint32_t*>(allocation + 4);

        if (static_cast<std::int32_t>(capacity - 1u) >= 0)
        {
            auto* entry = reinterpret_cast<std::uint8_t*>(newData);
            std::uint32_t remaining = capacity;
            do
            {
                CSF_AT(std::uint32_t, entry, 0x00) = 0;
                CSF_AT(void**, entry, 0x04) = CSF_VTBL(0x0082D448);
                CSF_AT(std::int32_t, entry, 0x08) = -1;
                CSF_AT(std::int32_t, entry, 0x0C) = -1;
                CSF_AT(std::uint32_t, entry, 0x10) = 0;
                CSF_AT(std::uint32_t, entry, 0x14) = 0;
                CSF_AT(std::uint32_t, entry, 0x18) = 0;
                entry += 28;
            } while (--remaining);
        }
    }

    std::uint32_t* result = newData;
    const std::uint32_t size = self->field_0008;
    if (size)
    {
        std::memcpy(newData,
                    reinterpret_cast<const void*>(self->field_0000),
                    28u * size);
    }

    self->field_0004 += static_cast<std::uint32_t>(additionalCapacity);

    auto* oldData = reinterpret_cast<std::uint8_t*>(self->field_0000);
    if (oldData)
    {
        auto* oldAllocation = oldData - 4;
        const std::uint32_t oldCapacity =
            CSF_AT(std::uint32_t, oldAllocation, 0);

        if (static_cast<std::int32_t>(oldCapacity - 1u) >= 0)
        {
            auto* entryVtable = oldData + 28u * oldCapacity + 4;
            std::uint32_t remaining = oldCapacity;
            do
            {
                entryVtable -= 28;
                CSF_AT(void**, entryVtable, 0) = CSF_VTBL(0x0082D410);
            } while (--remaining);
        }

        result = reinterpret_cast<std::uint32_t*>(Mem_Free(oldAllocation));
        self->field_0000 = 0;
        self->field_0000 = reinterpret_cast<std::uintptr_t>(newData);
    }
    else
    {
        self->field_0000 = reinterpret_cast<std::uintptr_t>(result);
    }

    return result;
}

HOOK(0x004DCA10, S_004dca10_GrowCapacity, "auto");
