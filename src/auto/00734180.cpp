#include <cstdint>
#include <cstddef>
#include <cstring>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"

CSF_FUNC(void*, __cdecl, Mem_Alloc, 0x006910F0, (std::size_t size));
CSF_FUNC(int, __cdecl, Mem_Free, 0x00691110, (void* block));

static int __fastcall GrowEntryArray(void* self, void* /*edx*/, int additionalCapacity)
{
    const std::uint32_t newCapacity =
        CSF_AT(std::uint32_t, self, 0x4) +
        static_cast<std::uint32_t>(additionalCapacity);

    void* newStorage = Mem_Alloc(12u * newCapacity);
    if (newStorage &&
        static_cast<std::int32_t>(newCapacity - 1u) >= 0)
    {
        auto* entry = static_cast<std::uint8_t*>(newStorage);
        std::uint32_t remaining = newCapacity;
        do
        {
            CSF_AT(std::uint32_t, entry, 0x4) = 0;
            CSF_AT(std::uint32_t, entry, 0x8) = 0;
            CSF_AT(std::int32_t, entry, 0x0) = -1;
            entry += 12;
        } while (--remaining);
    }

    const std::uint32_t count = CSF_AT(std::uint32_t, self, 0x8);
    if (count > 0)
    {
        std::memcpy(newStorage, CSF_AT(void*, self, 0x0), 12u * count);
    }

    const std::uint32_t updatedCapacity =
        CSF_AT(std::uint32_t, self, 0x4) +
        static_cast<std::uint32_t>(additionalCapacity);
    void* oldStorage = CSF_AT(void*, self, 0x0);
    CSF_AT(std::uint32_t, self, 0x4) = updatedCapacity;

    int result = 0;
    if (oldStorage)
    {
        result = Mem_Free(oldStorage);
        CSF_AT(void*, self, 0x0) = nullptr;
    }
    CSF_AT(void*, self, 0x0) = newStorage;
    return result;
}

HOOK(0x00734180, GrowEntryArray, "auto");
