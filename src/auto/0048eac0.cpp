#include <cstdint>
#include <cstddef>
#include <cstring>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_FUNC(void*, __cdecl, Mem_Alloc, 0x006910F0, (std::size_t size));
CSF_FUNC(int, __cdecl, Mem_Free, 0x00691110, (void* block));

static int __fastcall GrowEntryCapacity(
    void* self, void* /*edx*/, int additionalCapacity)
{
    const std::uint32_t newCapacity =
        CSF_AT(std::uint32_t, self, 0x4) +
        static_cast<std::uint32_t>(additionalCapacity);
    void* replacement = Mem_Alloc(
        static_cast<std::uint32_t>(12 * newCapacity));

    if (replacement &&
        static_cast<std::int32_t>(newCapacity - 1) >= 0)
    {
        auto* entry = static_cast<std::uint8_t*>(replacement);
        std::uint32_t remaining = newCapacity;
        do
        {
            CSF_AT(std::uint32_t, entry, 0x0) = 0;
            CSF_AT(std::uint32_t, entry, 0x4) = 0;
            CSF_AT(std::int32_t, entry, 0x8) = -1;
            entry += 12;
        }
        while (--remaining);
    }

    const std::uint32_t count = CSF_AT(std::uint32_t, self, 0x8);
    if (count)
    {
        std::memcpy(
            replacement,
            CSF_AT(void*, self, 0x0),
            static_cast<std::uint32_t>(12 * count));
    }

    void* oldBuffer = CSF_AT(void*, self, 0x0);
    CSF_AT(std::uint32_t, self, 0x4) +=
        static_cast<std::uint32_t>(additionalCapacity);

    int result = 0;
    if (oldBuffer)
    {
        result = Mem_Free(oldBuffer);
        CSF_AT(void*, self, 0x0) = nullptr;
    }

    CSF_AT(void*, self, 0x0) = replacement;
    return result;
}

HOOK(0x0048EAC0, GrowEntryCapacity, "auto");
