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
        CSF_AT(std::uint32_t, self, 0x04) +
        static_cast<std::uint32_t>(additionalCapacity);

    void* newBuffer = Mem_Alloc(12u * newCapacity);
    if (newBuffer &&
        static_cast<std::int32_t>(newCapacity - 1u) >= 0)
    {
        auto* entry = static_cast<std::uint8_t*>(newBuffer);
        std::uint32_t remaining = newCapacity;
        do
        {
            CSF_AT(std::uint32_t, entry, 0x04) = 0;
            CSF_AT(std::uint32_t, entry, 0x08) = 0;
            entry += 12;
        } while (--remaining);
    }

    const std::uint32_t count = CSF_AT(std::uint32_t, self, 0x08);
    if (count > 0)
        std::memcpy(newBuffer, CSF_AT(void*, self, 0x00), 12u * count);

    void* oldBuffer = CSF_AT(void*, self, 0x00);
    const void* result = oldBuffer;
    CSF_AT(std::uint32_t, self, 0x04) +=
        static_cast<std::uint32_t>(additionalCapacity);

    if (oldBuffer)
    {
        result = reinterpret_cast<const void*>(
            static_cast<std::uintptr_t>(
                static_cast<std::uint32_t>(Mem_Free(oldBuffer))));
        CSF_AT(void*, self, 0x00) = nullptr;
    }

    CSF_AT(void*, self, 0x00) = newBuffer;
    return result;
}

HOOK(0x006C49B0, GfxSkin_GrowEntryStorage, "auto");
