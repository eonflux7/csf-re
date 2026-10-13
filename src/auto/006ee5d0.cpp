#include <cstdint>
#include <cstddef>
#include <cstring>
#include "globals.h"
#include "hook.h"

CSF_FUNC(int, __cdecl, CoreMemory_AllocUntagged, 0x006910F0, (std::size_t size));
CSF_FUNC(int, __cdecl, CoreMemory_Free, 0x00691110, (void* block));

static int __fastcall GfxPTankCreate_GrowStorage(
    void* self, void* /*edx*/, int additionalCapacity)
{
    const std::uint32_t newCapacity =
        CSF_AT(std::uint32_t, self, 0x04) +
        static_cast<std::uint32_t>(additionalCapacity);

    const int allocationResult = CoreMemory_AllocUntagged(20u * newCapacity);
    void* newStorage =
        reinterpret_cast<void*>(static_cast<std::uintptr_t>(allocationResult));

    if (newStorage &&
        static_cast<std::int32_t>(newCapacity - 1u) >= 0)
    {
        auto* entry = static_cast<std::uint8_t*>(newStorage);
        std::uint32_t remaining = newCapacity;
        do
        {
            CSF_AT(std::uint32_t, entry, 0x00) = 0;
            CSF_AT(std::uint32_t, entry, 0x04) = 0;
            CSF_AT(std::uint32_t, entry, 0x08) = 0;
            CSF_AT(std::uint32_t, entry, 0x0C) = 0;
            entry += 20;
        }
        while (--remaining);
    }

    const std::uint32_t usedCount = CSF_AT(std::uint32_t, self, 0x08);
    if (usedCount > 0)
    {
        std::memcpy(newStorage, CSF_AT(void*, self, 0x00),
                    20u * usedCount);
    }

    const std::uint32_t updatedCapacity =
        CSF_AT(std::uint32_t, self, 0x04) +
        static_cast<std::uint32_t>(additionalCapacity);
    void* oldStorage = CSF_AT(void*, self, 0x00);
    CSF_AT(std::uint32_t, self, 0x04) = updatedCapacity;

    int result = allocationResult;
    if (oldStorage)
    {
        result = CoreMemory_Free(oldStorage);
        CSF_AT(void*, self, 0x00) = nullptr;
    }

    CSF_AT(void*, self, 0x00) = newStorage;
    return result;
}

HOOK(0x006EE5D0, GfxPTankCreate_GrowStorage, "auto");
