#include <cstdint>
#include "globals.h"
#include <cstddef>
#include <cstring>
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_FUNC(void*, __cdecl, Mem_Alloc, 0x006910F0, (std::size_t size));
CSF_FUNC(int, __cdecl, Mem_Free, 0x00691110, (void* block));

static std::uint32_t* __fastcall S_004f7d10_GrowEntryCapacity(
    S_004f7d10* self, void* /*edx*/, int additionalCapacity)
{
    const std::uint32_t newCapacity =
        self->field_0004 + static_cast<std::uint32_t>(additionalCapacity);

    auto* allocation = static_cast<std::uint8_t*>(
        Mem_Alloc(48u * newCapacity + 4u));
    std::uint8_t* newEntries = nullptr;

    if (allocation)
    {
        newEntries = allocation + 4;
        CSF_AT(std::uint32_t, newEntries, -4) = newCapacity;

        if (static_cast<std::int32_t>(newCapacity - 1u) >= 0)
        {
            auto* entry = newEntries;
            std::uint32_t remaining = newCapacity;
            do
            {
                CSF_AT(std::int32_t, entry, 0x00) = -1;
                CSF_AT(void*, entry, 0x04) = CSF_VTBL(0x0082D448);
                CSF_AT(std::int32_t, entry, 0x08) = -1;
                CSF_AT(std::int32_t, entry, 0x0C) = -1;
                CSF_AT(void*, entry, 0x10) = CSF_VTBL(0x0082D448);
                CSF_AT(std::int32_t, entry, 0x14) = -1;
                CSF_AT(std::int32_t, entry, 0x18) = -1;
                CSF_AT(std::uint32_t, entry, 0x1C) = 0;
                CSF_AT(std::uint32_t, entry, 0x20) = 0;
                CSF_AT(std::uint32_t, entry, 0x24) = 0;
                CSF_AT(std::uint32_t, entry, 0x28) = 0;
                CSF_AT(std::uint32_t, entry, 0x2C) = 0;
                entry += 48;
            } while (--remaining);
        }
    }

    auto* result = reinterpret_cast<std::uint32_t*>(newEntries);

    const std::uint32_t usedCount = self->field_0008;
    if (usedCount)
    {
        std::memcpy(newEntries,
                    reinterpret_cast<const void*>(self->field_0000),
                    48u * usedCount);
    }

    self->field_0004 += static_cast<std::uint32_t>(additionalCapacity);

    auto* oldEntries = reinterpret_cast<std::uint8_t*>(self->field_0000);
    if (oldEntries)
    {
        const std::uint32_t oldCapacity =
            CSF_AT(std::uint32_t, oldEntries, -4);
        auto* entry = oldEntries + 48u * oldCapacity;

        if (static_cast<std::int32_t>(oldCapacity - 1u) >= 0)
        {
            std::uint32_t remaining = oldCapacity;
            do
            {
                entry -= 48;
                CSF_AT(void*, entry, 0x10) = CSF_VTBL(0x0082D410);
                CSF_AT(void*, entry, 0x04) = CSF_VTBL(0x0082D410);
            } while (--remaining);
        }

        result = reinterpret_cast<std::uint32_t*>(
            static_cast<std::uintptr_t>(
                static_cast<std::uint32_t>(Mem_Free(oldEntries - 4))));
        self->field_0000 = 0;
        self->field_0000 = reinterpret_cast<std::uintptr_t>(newEntries);
    }
    else
    {
        self->field_0000 = reinterpret_cast<std::uintptr_t>(result);
    }

    return result;
}

HOOK(0x004F7D10, S_004f7d10_GrowEntryCapacity, "auto");
