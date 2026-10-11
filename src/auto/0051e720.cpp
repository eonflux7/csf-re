#include <cstdint>
#include <cstddef>
#include <cstring>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_FUNC(void*, __cdecl, Mem_Alloc, 0x006910F0, (std::size_t size));
CSF_FUNC(int, __cdecl, Mem_Free, 0x00691110, (void* block));

static std::uint32_t* __fastcall FxParticleManager_GrowEntryArray(
    void* self, void* /*edx*/, int additionalCapacity)
{
    const std::uint32_t newCapacity =
        CSF_AT(std::uint32_t, self, 0x4) +
        static_cast<std::uint32_t>(additionalCapacity);

    void* allocation = Mem_Alloc(16u * newCapacity + 4u);
    std::uint32_t* newEntries = nullptr;

    if (allocation)
    {
        CSF_AT(std::uint32_t, allocation, 0x0) = newCapacity;
        newEntries = reinterpret_cast<std::uint32_t*>(
            static_cast<std::uint8_t*>(allocation) + 4);

        if (static_cast<std::int32_t>(newCapacity - 1u) >= 0)
        {
            std::uint32_t* entry = newEntries;
            std::uint32_t remaining = newCapacity;
            do
            {
                CSF_AT(void**, entry, 0x4) = CSF_VTBL(0x0082D448);
                CSF_AT(std::int32_t, entry, 0x8) = -1;
                CSF_AT(std::int32_t, entry, 0xC) = -1;
                CSF_AT(std::uint32_t, entry, 0x0) = 0;
                entry += 4;
            } while (--remaining);
        }
    }

    std::uint32_t* result = newEntries;
    const std::uint32_t usedCount = CSF_AT(std::uint32_t, self, 0x8);
    if (usedCount != 0)
    {
        std::memcpy(newEntries, CSF_AT(void*, self, 0x0), 16u * usedCount);
    }

    CSF_AT(std::uint32_t, self, 0x4) +=
        static_cast<std::uint32_t>(additionalCapacity);

    auto* oldEntries = CSF_AT(std::uint32_t*, self, 0x0);
    if (oldEntries)
    {
        const std::uint32_t oldCapacity =
            CSF_AT(std::uint32_t, oldEntries, -0x4);

        if (static_cast<std::int32_t>(oldCapacity - 1u) >= 0)
        {
            std::uint32_t* entry = oldEntries + 4u * oldCapacity + 1;
            std::uint32_t remaining = oldCapacity;
            do
            {
                entry -= 4;
                --remaining;
                CSF_AT(void**, entry, 0x0) = CSF_VTBL(0x0082D410);
            } while (remaining);
        }

        result = reinterpret_cast<std::uint32_t*>(
            static_cast<std::uintptr_t>(
                static_cast<std::uint32_t>(Mem_Free(oldEntries - 1))));
        CSF_AT(std::uint32_t*, self, 0x0) = nullptr;
        CSF_AT(std::uint32_t*, self, 0x0) = newEntries;
    }
    else
    {
        CSF_AT(std::uint32_t*, self, 0x0) = result;
    }

    return result;
}

HOOK(0x0051E720, FxParticleManager_GrowEntryArray, "auto");
