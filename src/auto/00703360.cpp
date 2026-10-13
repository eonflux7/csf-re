#include <cstdint>
#include "globals.h"
#include <cstddef>
#include <cstring>
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_FUNC(int, __cdecl, CoreMemory_AllocUntagged, 0x006910F0, (std::size_t size));
CSF_FUNC(int, __cdecl, CoreMemory_Free, 0x00691110, (void* block));

static char __fastcall S_00703360_GrowParticleStorage(
    S_00703360* self, void* /*edx*/, int additionalCapacity)
{
    const uint32_t newCapacity =
        self->field_0004 + static_cast<uint32_t>(additionalCapacity);
    auto* allocation = reinterpret_cast<uint32_t*>(
        CoreMemory_AllocUntagged(16u * newCapacity + 4u));

    uint8_t* newEntries = nullptr;
    if (allocation)
    {
        *allocation = newCapacity;
        newEntries = reinterpret_cast<uint8_t*>(allocation + 1);

        if (static_cast<int32_t>(newCapacity - 1u) >= 0)
        {
            uint8_t* entry = newEntries;
            uint32_t remaining = newCapacity;
            do
            {
                CSF_AT(uint32_t, entry, 0x00) = 0;
                CSF_AT(uint32_t, entry, 0x04) = 0;
                CSF_AT(uint32_t, entry, 0x08) = 0;
                CSF_AT(uint32_t, entry, 0x0C) = 10;
                entry += 16;
            } while (--remaining);
        }
    }

    const uint32_t usedCount = self->field_0008;
    if (usedCount)
    {
        std::memcpy(
            newEntries,
            reinterpret_cast<const void*>(self->field_0000),
            16u * usedCount);
    }

    const uint32_t oldStorage = self->field_0000;
    self->field_0004 += static_cast<uint32_t>(additionalCapacity);

    char result = 0;
    if (oldStorage)
    {
        auto* oldHeader = reinterpret_cast<void*>(oldStorage - 4u);
        const uint32_t oldCapacity = CSF_AT(uint32_t, oldHeader, 0x00);
        auto* entry = reinterpret_cast<uint8_t*>(
            oldStorage + 16u * oldCapacity);

        if (static_cast<int32_t>(oldCapacity - 1u) >= 0)
        {
            uint32_t remaining = oldCapacity;
            do
            {
                const uint32_t block = CSF_AT(uint32_t, entry, -0x10);
                entry -= 16;
                if (block)
                {
                    CoreMemory_Free(reinterpret_cast<void*>(block));
                    CSF_AT(uint32_t, entry, 0x00) = 0;
                }
            } while (--remaining);
        }

        result = static_cast<char>(CoreMemory_Free(oldHeader));
        self->field_0000 = 0;
    }

    self->field_0000 = reinterpret_cast<uint32_t>(newEntries);
    return result;
}

HOOK(0x00703360, S_00703360_GrowParticleStorage, "auto");
