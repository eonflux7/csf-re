#include <cstdint>
#include <cstddef>
#include <cstring>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_FUNC(void*, __cdecl, Mem_Alloc, 0x006910F0, (std::size_t size));
CSF_FUNC(void*, __fastcall, GfxScene_DestroyEntryArray, 0x006CD210,
         (void* self, void* edx, char a2));

static void* __fastcall S_006cd2d0_GrowCapacity(
    S_006cd2d0* self, void* /*edx*/, int additionalCapacity)
{
    const std::uint32_t newCapacity =
        self->field_0004 + static_cast<std::uint32_t>(additionalCapacity);
    void* allocation = Mem_Alloc(28 * newCapacity + 4);
    void* newEntries = nullptr;

    if (allocation)
    {
        CSF_AT(std::uint32_t, allocation, 0) = newCapacity;
        newEntries = static_cast<std::uint8_t*>(allocation) + 4;

        if (static_cast<std::int32_t>(newCapacity - 1) >= 0)
        {
            auto* entry = static_cast<std::uint8_t*>(newEntries);
            std::uint32_t remaining = newCapacity;
            do
            {
                CSF_AT(std::uint32_t, entry, 0x18) = 15;
                CSF_AT(std::uint32_t, entry, 0x14) = 0;
                CSF_AT(std::uint8_t, entry, 0x04) = 0;
                entry += 28;
            }
            while (--remaining);
        }
    }

    const std::uint32_t entryCount = self->field_0008;
    if (entryCount)
    {
        std::memcpy(newEntries,
                    reinterpret_cast<const void*>(self->field_0000),
                    28 * entryCount);
    }

    const std::uint32_t updatedCapacity =
        self->field_0004 + static_cast<std::uint32_t>(additionalCapacity);
    void* oldEntries = reinterpret_cast<void*>(self->field_0000);
    self->field_0004 = updatedCapacity;

    void* result = newEntries;
    if (oldEntries)
    {
        result = GfxScene_DestroyEntryArray(oldEntries, nullptr, 3);
        self->field_0000 = 0;
        self->field_0000 = reinterpret_cast<std::uintptr_t>(newEntries);
    }
    else
    {
        self->field_0000 = reinterpret_cast<std::uintptr_t>(newEntries);
    }
    return result;
}

HOOK(0x006CD2D0, S_006cd2d0_GrowCapacity, "auto");
