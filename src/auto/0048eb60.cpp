#include <cstdint>
#include "globals.h"
#include <cstddef>
#include <cstring>
#include "hook.h"
#include "core/memory.h"

CSF_FUNC(void*, __cdecl, Mem_Alloc, 0x006910F0, (std::size_t size));
CSF_FUNC(int, __cdecl, Mem_Free, 0x00691110, (void* block));

static int __fastcall GrowEntryCapacity(void* self, void* /*edx*/, int additionalCapacity)
{
    const std::uint32_t capacity =
        CSF_AT(std::uint32_t, self, 0x4) +
        static_cast<std::uint32_t>(additionalCapacity);

    void* replacement = Mem_Alloc(12 * capacity);
    if (replacement && static_cast<std::int32_t>(capacity - 1) >= 0)
    {
        auto* entry = static_cast<std::uint8_t*>(replacement);
        for (std::uint32_t i = 0; i < capacity; ++i, entry += 12)
        {
            CSF_AT(std::int32_t, entry, 0x0) = -1;
            CSF_AT(std::uint32_t, entry, 0x4) = 0;
            CSF_AT(std::uint32_t, entry, 0x8) = 0;
        }
    }

    int result = 0;
    const std::uint32_t count = CSF_AT(std::uint32_t, self, 0x8);
    if (count > 0)
        std::memcpy(replacement, CSF_AT(void*, self, 0x0), 12 * count);

    const std::uint32_t updatedCapacity =
        CSF_AT(std::uint32_t, self, 0x4) +
        static_cast<std::uint32_t>(additionalCapacity);
    void* previous = CSF_AT(void*, self, 0x0);
    CSF_AT(std::uint32_t, self, 0x4) = updatedCapacity;

    if (previous)
    {
        result = Mem_Free(previous);
        CSF_AT(void*, self, 0x0) = nullptr;
    }

    CSF_AT(void*, self, 0x0) = replacement;
    return result;
}

HOOK(0x0048EB60, GrowEntryCapacity, "auto");
