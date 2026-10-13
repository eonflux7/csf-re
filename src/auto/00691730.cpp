#include <cstdint>
#include "globals.h"
#include <cstddef>
#include <cstring>
#include "hook.h"
#include "core/memory.h"

CSF_FUNC(void*, __cdecl, Mem_Alloc, 0x006910F0, (std::size_t size));
CSF_FUNC(int, __cdecl, Mem_Free, 0x00691110, (void* block));

static int __fastcall CoreMemory_GrowReferenceStorage(
    void* self, void* /*edx*/, int additionalCapacity)
{
    const std::uint32_t increment =
        static_cast<std::uint32_t>(additionalCapacity);
    const std::uint32_t newCapacity =
        CSF_AT(std::uint32_t, self, 0x4) + increment;

    void* newStorage = Mem_Alloc(newCapacity << 4);
    if (newStorage &&
        static_cast<std::int32_t>(newCapacity - 1) >= 0)
    {
        auto* entry = static_cast<std::uint8_t*>(newStorage);
        std::uint32_t remaining = newCapacity;
        do
        {
            CSF_AT(std::uint32_t, entry, 0x0) = 0;
            CSF_AT(std::uint32_t, entry, 0x4) = 0;
            CSF_AT(std::uint8_t, entry, 0x8) = 1;
            CSF_AT(std::uint8_t, entry, 0x9) = 0;
            CSF_AT(std::uint32_t, entry, 0xC) = 0;
            entry += 16;
        } while (--remaining);
    }

    const std::uint32_t usedCount = CSF_AT(std::uint32_t, self, 0x8);
    if (usedCount > 0)
    {
        std::memcpy(newStorage, CSF_AT(void*, self, 0x0), usedCount << 4);
    }

    const std::uint32_t updatedCapacity =
        CSF_AT(std::uint32_t, self, 0x4) + increment;
    void* oldStorage = CSF_AT(void*, self, 0x0);
    CSF_AT(std::uint32_t, self, 0x4) = updatedCapacity;

    int result;
    if (oldStorage)
    {
        result = Mem_Free(oldStorage);
        CSF_AT(void*, self, 0x0) = nullptr;
    }
    else
    {
        result = static_cast<int>(
            reinterpret_cast<std::uintptr_t>(newStorage));
    }

    CSF_AT(void*, self, 0x0) = newStorage;
    return result;
}

HOOK(0x00691730, CoreMemory_GrowReferenceStorage, "auto");
