#include <cstdint>
#include <cstddef>
#include <cstring>
#include "globals.h"
#include "hook.h"

CSF_FUNC(void*, __cdecl, Mem_Alloc, 0x006910F0, (std::size_t size));
CSF_FUNC(int, __cdecl, Mem_Free, 0x00691110, (void* block));

static const void* __fastcall SceneObjects_GrowStorage(
    void* self, void* /*edx*/, int additionalCapacity)
{
    const std::uint32_t allocationCount =
        CSF_AT(std::uint32_t, self, 0x04) +
        static_cast<std::uint32_t>(additionalCapacity);

    auto* newStorage = static_cast<std::uint8_t*>(
        Mem_Alloc(12u * allocationCount));

    if (newStorage &&
        static_cast<std::int32_t>(allocationCount - 1u) >= 0)
    {
        auto* entry = newStorage;
        std::uint32_t remaining = allocationCount;
        do
        {
            CSF_AT(std::uint32_t, entry, 0x00) = 0;
            CSF_AT(std::uint8_t, entry, 0x08) = 0;
            CSF_AT(std::uint32_t, entry, 0x04) = 0;
            entry += 12;
        } while (--remaining);
    }

    const std::uint32_t usedCount = CSF_AT(std::uint32_t, self, 0x08);
    if (usedCount > 0)
    {
        std::memcpy(
            newStorage, CSF_AT(void*, self, 0x00), 12u * usedCount);
    }

    const std::uint32_t updatedCapacity =
        CSF_AT(std::uint32_t, self, 0x04) +
        static_cast<std::uint32_t>(additionalCapacity);
    void* oldStorage = CSF_AT(void*, self, 0x00);
    const void* result = oldStorage;

    CSF_AT(std::uint32_t, self, 0x04) = updatedCapacity;
    if (oldStorage)
    {
        result = reinterpret_cast<const void*>(
            static_cast<std::uintptr_t>(Mem_Free(oldStorage)));
        CSF_AT(void*, self, 0x00) = nullptr;
    }

    CSF_AT(void*, self, 0x00) = newStorage;
    return result;
}

HOOK(0x0044DD40, SceneObjects_GrowStorage, "auto");
