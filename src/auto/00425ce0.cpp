#include <cstdint>
#include <cstddef>
#include <cstring>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"

CSF_FUNC(void*, __cdecl, Mem_Alloc, 0x006910F0, (std::size_t size));
CSF_FUNC(int, __cdecl, Mem_Free, 0x00691110, (void* block));

static const void* __fastcall Buffer_GrowCapacity(
    const void** self, void* /*edx*/, int additionalCapacity)
{
    const std::uint32_t newCapacity =
        CSF_AT(std::uint32_t, self, 0x04) +
        static_cast<std::uint32_t>(additionalCapacity);

    auto* replacement = static_cast<std::uint8_t*>(
        Mem_Alloc(36u * newCapacity));

    if (replacement &&
        static_cast<std::int32_t>(newCapacity - 1u) >= 0)
    {
        auto* entry = replacement;
        std::uint32_t remaining = newCapacity;
        do
        {
            CSF_AT(std::uint32_t, entry, 0x08) = 0;
            CSF_AT(std::uint32_t, entry, 0x0C) = 0;
            CSF_AT(std::uint32_t, entry, 0x10) = 0;
            CSF_AT(std::uint32_t, entry, 0x14) = 0;
            CSF_AT(std::uint32_t, entry, 0x00) = 1;
            CSF_AT(std::uint32_t, entry, 0x04) = 0;
            CSF_AT(std::uint32_t, entry, 0x18) = 0;
            CSF_AT(std::uint32_t, entry, 0x1C) = 0;
            CSF_AT(std::uint8_t, entry, 0x20) = 0;
            entry += 36;
        } while (--remaining);
    }

    const std::uint32_t usedCount = CSF_AT(std::uint32_t, self, 0x08);
    if (usedCount != 0)
    {
        std::memcpy(replacement, CSF_AT(const void*, self, 0x00),
                    36u * usedCount);
    }

    const void* oldBuffer = CSF_AT(const void*, self, 0x00);
    CSF_AT(std::uint32_t, self, 0x04) =
        CSF_AT(std::uint32_t, self, 0x04) +
        static_cast<std::uint32_t>(additionalCapacity);

    const void* result = oldBuffer;
    if (oldBuffer)
    {
        result = reinterpret_cast<const void*>(
            static_cast<std::uintptr_t>(
                Mem_Free(const_cast<void*>(oldBuffer))));
        CSF_AT(const void*, self, 0x00) = nullptr;
    }

    CSF_AT(const void*, self, 0x00) = replacement;
    return result;
}

HOOK(0x00425CE0, Buffer_GrowCapacity, "auto");
