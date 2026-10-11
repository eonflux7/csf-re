#include <cstdint>
#include <cstddef>
#include <cstring>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"

CSF_FUNC(void*, __cdecl, Mem_Alloc, 0x006910F0, (std::size_t size));
CSF_FUNC(int, __cdecl, Mem_Free, 0x00691110, (void* block));

static const void* __fastcall FxParticleManager_GrowParticleSlots(
    void* self, void* /*edx*/, int additionalCapacity)
{
    const std::uint32_t newCapacity =
        CSF_AT(std::uint32_t, self, 0x04) +
        static_cast<std::uint32_t>(additionalCapacity);

    auto* newSlots = static_cast<std::uint8_t*>(
        Mem_Alloc(20 * newCapacity));

    if (newSlots &&
        static_cast<std::int32_t>(newCapacity - 1) >= 0)
    {
        std::uint8_t* slot = newSlots;
        std::uint32_t remaining = newCapacity;
        do
        {
            CSF_AT(std::int32_t, slot, 0x00) = -1;
            CSF_AT(std::int32_t, slot, 0x04) = -1;
            CSF_AT(std::uint32_t, slot, 0x08) = 0;
            CSF_AT(std::int32_t, slot, 0x0C) = -1;
            CSF_AT(std::uint8_t, slot, 0x10) = 0;
            slot += 20;
        }
        while (--remaining);
    }

    const std::uint32_t usedCount = CSF_AT(std::uint32_t, self, 0x08);
    if (usedCount != 0)
        std::memcpy(newSlots, CSF_AT(void*, self, 0x00), 20 * usedCount);

    void* oldSlots = CSF_AT(void*, self, 0x00);
    CSF_AT(std::uint32_t, self, 0x04) +=
        static_cast<std::uint32_t>(additionalCapacity);

    const void* result = oldSlots;
    if (oldSlots)
    {
        result = reinterpret_cast<const void*>(
            static_cast<std::uintptr_t>(Mem_Free(oldSlots)));
        CSF_AT(void*, self, 0x00) = nullptr;
    }

    CSF_AT(void*, self, 0x00) = newSlots;
    return result;
}

HOOK(0x0051FB70, FxParticleManager_GrowParticleSlots, "auto");
