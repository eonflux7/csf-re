#include <cstdint>
#include <cstddef>
#include <cstring>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"

CSF_FUNC(int, __cdecl, CoreMemory_AllocUntagged, 0x006910F0, (std::size_t size));
CSF_FUNC(int, __cdecl, CoreMemory_Free, 0x00691110, (void* block));

static std::uint32_t* __fastcall EntryTable_GrowStorage(
    void* self, void* /*edx*/, int additionalCapacity)
{
    const std::uint32_t newCapacity =
        CSF_AT(std::uint32_t, self, 4) +
        static_cast<std::uint32_t>(additionalCapacity);

    auto* allocation = reinterpret_cast<std::uint8_t*>(
        CoreMemory_AllocUntagged(36u * newCapacity + 4u));

    std::uint8_t* newStorage = nullptr;
    if (allocation)
    {
        CSF_AT(std::uint32_t, allocation, 0) = newCapacity;
        newStorage = allocation + 4;

        if (static_cast<std::int32_t>(newCapacity - 1u) >= 0)
        {
            auto* entry = newStorage;
            std::uint32_t remaining = newCapacity;
            do
            {
                CSF_AT(void**, entry, 0) = CSF_VTBL(0x0082D448);
                CSF_AT(std::int32_t, entry, 4) = -1;
                CSF_AT(std::int32_t, entry, 8) = -1;
                CSF_AT(void**, entry, 0x10) = CSF_VTBL(0x0082D448);
                CSF_AT(std::int32_t, entry, 0x14) = -1;
                CSF_AT(std::int32_t, entry, 0x18) = -1;
                CSF_AT(std::uint32_t, entry, 0x0C) = 0;
                entry += 0x24;
            } while (--remaining);
        }
    }

    const std::uint32_t usedCount = CSF_AT(std::uint32_t, self, 8);
    if (usedCount != 0)
    {
        std::memcpy(
            newStorage, CSF_AT(void*, self, 0), 36u * usedCount);
    }

    auto* oldStorage = CSF_AT(std::uint8_t*, self, 0);
    CSF_AT(std::uint32_t, self, 4) +=
        static_cast<std::uint32_t>(additionalCapacity);

    auto* result = reinterpret_cast<std::uint32_t*>(oldStorage);
    if (oldStorage)
    {
        auto* oldAllocation = oldStorage - 4;
        const std::uint32_t oldCapacity =
            CSF_AT(std::uint32_t, oldAllocation, 0);
        auto* entry = oldStorage + 36u * oldCapacity;

        if (static_cast<std::int32_t>(oldCapacity - 1u) >= 0)
        {
            std::uint32_t remaining = oldCapacity;
            do
            {
                entry -= 0x24;
                --remaining;
                CSF_AT(void**, entry, 0x10) = CSF_VTBL(0x0082D410);
                CSF_AT(void**, entry, 0) = CSF_VTBL(0x0082D410);
            } while (remaining);
        }

        result = reinterpret_cast<std::uint32_t*>(
            CoreMemory_Free(oldAllocation));
        CSF_AT(void*, self, 0) = nullptr;
    }

    CSF_AT(void*, self, 0) = newStorage;
    return result;
}

HOOK(0x00448C60, EntryTable_GrowStorage, "auto");
