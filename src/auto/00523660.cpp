#include <cstdint>
#include <cstddef>
#include <cstring>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_FUNC(void*, __cdecl, Mem_Alloc, 0x006910F0, (std::size_t size));
CSF_FUNC(int, __cdecl, Mem_Free, 0x00691110, (void* block));

static unsigned int __fastcall S_00523660_GrowCapacity(
    S_00523660* self, void* /*edx*/, int additionalCapacity)
{
    const std::uint32_t newCapacity =
        self->field_0004 + static_cast<std::uint32_t>(additionalCapacity);

    auto* allocation = static_cast<std::uint8_t*>(
        Mem_Alloc(24 * newCapacity + 4));
    std::uint8_t* newEntries = nullptr;

    if (allocation)
    {
        CSF_AT(std::uint32_t, allocation, 0) = newCapacity;
        newEntries = allocation + 4;

        if (static_cast<std::int32_t>(newCapacity - 1) >= 0)
        {
            auto* entry = newEntries;
            std::uint32_t remaining = newCapacity;
            do
            {
                CSF_AT(std::uint32_t, entry, 0x00) = 0;
                CSF_AT(std::uint32_t, entry, 0x04) = 0;
                CSF_AT(void**, entry, 0x08) = CSF_VTBL(0x0082D448);
                CSF_AT(std::int32_t, entry, 0x0C) = -1;
                CSF_AT(std::int32_t, entry, 0x10) = -1;
                CSF_AT(float, entry, 0x14) = -1.0f;
                entry += 24;
            } while (--remaining);
        }
    }

    unsigned int result = self->field_0008;
    if (result)
    {
        result *= 24;
        std::memcpy(
            newEntries,
            reinterpret_cast<const void*>(self->field_0000),
            result);
    }

    auto* oldEntries = reinterpret_cast<std::uint8_t*>(self->field_0000);
    self->field_0004 += static_cast<std::uint32_t>(additionalCapacity);

    if (oldEntries)
    {
        const std::uint32_t oldCapacity =
            CSF_AT(std::uint32_t, oldEntries, -4);
        auto* end = oldEntries + 24 * oldCapacity;

        if (static_cast<std::int32_t>(oldCapacity - 1) >= 0)
        {
            std::uint32_t remaining = oldCapacity;
            do
            {
                end -= 24;
                CSF_AT(void**, end, 0x08) = CSF_VTBL(0x0082D410);
            } while (--remaining);
        }

        result = static_cast<unsigned int>(Mem_Free(oldEntries - 4));
        self->field_0000 = 0;
    }

    self->field_0000 = reinterpret_cast<std::uint32_t>(newEntries);
    return result;
}

HOOK(0x00523660, S_00523660_GrowCapacity, "auto");
