#include <cstdint>
#include "globals.h"
#include <cstddef>
#include <cstring>
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_FUNC(void*, __cdecl, Mem_Alloc, 0x006910F0, (std::size_t size));
CSF_FUNC(int, __cdecl, Mem_Free, 0x00691110, (void* block));

static unsigned int __fastcall S_0051c1b0_GrowEntries(
    S_0051c1b0* self, void* /*edx*/, int additionalCapacity)
{
    const std::uint32_t entryCount =
        self->field_0004 + static_cast<std::uint32_t>(additionalCapacity);
    auto* allocation = static_cast<std::uint8_t*>(
        Mem_Alloc(24 * entryCount + 4));
    std::uint8_t* newEntries = nullptr;

    if (allocation)
    {
        CSF_AT(std::uint32_t, allocation, 0) = entryCount;
        newEntries = allocation + 4;

        if (static_cast<std::int32_t>(entryCount - 1) >= 0)
        {
            auto* entry = newEntries;
            std::uint32_t remaining = entryCount;
            do
            {
                CSF_AT(void**, entry, 0x04) = CSF_VTBL(0x0082D448);
                CSF_AT(std::int32_t, entry, 0x08) = -1;
                CSF_AT(std::int32_t, entry, 0x0C) = -1;
                CSF_AT(std::uint32_t, entry, 0x00) = 0;
                CSF_AT(std::int32_t, entry, 0x10) = -1;
                CSF_AT(std::int32_t, entry, 0x14) = -1;
                entry += 24;
            } while (--remaining);
        }
    }

    unsigned int result = self->field_0008;
    if (result)
    {
        result *= 24;
        std::memcpy(
            newEntries, reinterpret_cast<const void*>(self->field_0000),
            result);
    }

    const std::uint32_t updatedCapacity =
        self->field_0004 + static_cast<std::uint32_t>(additionalCapacity);
    auto* oldEntries = reinterpret_cast<std::uint8_t*>(self->field_0000);
    self->field_0004 = updatedCapacity;

    if (oldEntries)
    {
        auto* oldAllocation = oldEntries - 4;
        const std::uint32_t oldCount =
            CSF_AT(std::uint32_t, oldAllocation, 0);

        if (static_cast<std::int32_t>(oldCount - 1) >= 0)
        {
            auto* entry = oldEntries + 24 * oldCount;
            std::uint32_t remaining = oldCount;
            do
            {
                entry -= 24;
                CSF_AT(void**, entry, 0x04) = CSF_VTBL(0x0082D410);
            } while (--remaining);
        }

        result = static_cast<unsigned int>(Mem_Free(oldAllocation));
        self->field_0000 = 0;
    }

    self->field_0000 = reinterpret_cast<std::uint32_t>(newEntries);
    return result;
}

HOOK(0x0051C1B0, S_0051c1b0_GrowEntries, "auto");
