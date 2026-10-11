#include <cstdint>
#include <cstddef>
#include <cstring>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_FUNC(void*, __cdecl, Mem_Alloc, 0x006910F0, (std::size_t size));
CSF_FUNC(int, __cdecl, Mem_Free, 0x00691110, (void* block));

static std::uint32_t* __fastcall S_004bdcd0_GrowCapacity(
    S_004bdcd0* self, void* /*edx*/, int additionalCapacity)
{
    const std::uint32_t capacity = self->field_0004 + additionalCapacity;
    auto* allocation = static_cast<std::uint8_t*>(
        Mem_Alloc(52u * capacity + 4u));
    std::uint32_t* newEntries = nullptr;

    if (allocation)
    {
        CSF_AT(std::uint32_t, allocation, 0) = capacity;
        newEntries = reinterpret_cast<std::uint32_t*>(allocation + 4);

        if (static_cast<std::int32_t>(capacity - 1u) >= 0)
        {
            auto* entry = reinterpret_cast<std::uint8_t*>(newEntries);
            std::uint32_t remaining = capacity;
            do
            {
                CSF_AT(void*, entry, 0x0C) = CSF_VTBL(0x0082D448);
                CSF_AT(std::int32_t, entry, 0x10) = -1;
                CSF_AT(std::int32_t, entry, 0x14) = -1;
                CSF_AT(std::uint32_t, entry, 0x18) = 0;
                CSF_AT(std::uint32_t, entry, 0x1C) = 0;
                CSF_AT(std::uint32_t, entry, 0x20) = 0;
                CSF_AT(std::int32_t, entry, 0x00) = -1;
                CSF_AT(std::int32_t, entry, 0x04) = -1;
                CSF_AT(std::int32_t, entry, 0x08) = -1;
                CSF_AT(std::int32_t, entry, 0x10) = -1;
                CSF_AT(std::int32_t, entry, 0x14) = -1;
                CSF_AT(void*, entry, 0x24) = CSF_VTBL(0x0082D448);
                CSF_AT(std::int32_t, entry, 0x28) = -1;
                CSF_AT(std::int32_t, entry, 0x2C) = -1;
                CSF_AT(std::uint32_t, entry, 0x30) = 0;
                entry += 52;
            } while (--remaining);
        }
    }

    std::uint32_t* result = newEntries;
    const std::uint32_t usedCount = self->field_0008;
    if (usedCount)
    {
        std::memcpy(newEntries,
                    reinterpret_cast<const void*>(self->field_0000),
                    52u * usedCount);
    }

    self->field_0004 += additionalCapacity;

    auto* oldEntries = reinterpret_cast<std::uint8_t*>(self->field_0000);
    if (oldEntries)
    {
        auto* oldAllocation = oldEntries - 4;
        const std::uint32_t oldCapacity =
            CSF_AT(std::uint32_t, oldAllocation, 0);

        if (static_cast<std::int32_t>(oldCapacity - 1u) >= 0)
        {
            auto* entry = oldEntries + 52u * oldCapacity;
            std::uint32_t remaining = oldCapacity;
            do
            {
                entry -= 52;
                CSF_AT(void*, entry, 0x24) = CSF_VTBL(0x0082D410);
                CSF_AT(void*, entry, 0x0C) = CSF_VTBL(0x0082D410);
            } while (--remaining);
        }

        result = reinterpret_cast<std::uint32_t*>(
            static_cast<std::uintptr_t>(Mem_Free(oldAllocation)));
        self->field_0000 = 0;
        self->field_0000 = reinterpret_cast<std::uintptr_t>(newEntries);
    }
    else
    {
        self->field_0000 = reinterpret_cast<std::uintptr_t>(result);
    }

    return result;
}

HOOK(0x004BDCD0, S_004bdcd0_GrowCapacity, "auto");
