#include <cstdint>
#include <cstddef>
#include <cstring>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"

CSF_FUNC(void*, __cdecl, Mem_Alloc, 0x006910F0, (std::size_t size));
CSF_FUNC(int, __cdecl, Mem_Free, 0x00691110, (void* block));

static const void* __fastcall GfxSkin_GrowEntryStorage(
    void* self, void* /*edx*/, int additionalCapacity)
{
    const std::uint32_t newCapacity =
        CSF_AT(std::uint32_t, self, 0x4) +
        static_cast<std::uint32_t>(additionalCapacity);

    auto* newStorage = static_cast<std::uint8_t*>(
        Mem_Alloc(12 * newCapacity));

    if (newStorage &&
        static_cast<std::int32_t>(newCapacity - 1) >= 0)
    {
        auto* entry = newStorage;
        std::uint32_t remaining = newCapacity;
        do
        {
            CSF_AT(std::uint32_t, entry, 0x0) = 0;
            CSF_AT(std::uint32_t, entry, 0x4) = 0;
            CSF_AT(std::uint8_t, entry, 0x8) = 0;
            entry += 12;
        } while (--remaining);
    }

    const std::uint32_t entryCount = CSF_AT(std::uint32_t, self, 0x8);
    if (entryCount > 0)
    {
        std::memcpy(newStorage, CSF_AT(void*, self, 0x0),
                    12 * entryCount);
    }

    const std::uint32_t currentCapacity =
        CSF_AT(std::uint32_t, self, 0x4);
    void* oldStorage = CSF_AT(void*, self, 0x0);
    const void* result = oldStorage;

    CSF_AT(std::uint32_t, self, 0x4) =
        currentCapacity + static_cast<std::uint32_t>(additionalCapacity);

    if (oldStorage)
    {
        result = reinterpret_cast<const void*>(
            static_cast<std::uintptr_t>(Mem_Free(oldStorage)));
        CSF_AT(void*, self, 0x0) = nullptr;
    }

    CSF_AT(void*, self, 0x0) = newStorage;
    return result;
}

HOOK(0x006C7DD0, GfxSkin_GrowEntryStorage, "auto");
