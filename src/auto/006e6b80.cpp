#include <cstdint>
#include <cstddef>
#include <cstring>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"

CSF_FUNC(int, __cdecl, CoreMemory_AllocUntagged, 0x006910F0, (std::size_t size));
CSF_FUNC(int, __cdecl, CoreMemory_Free, 0x00691110, (void* block));

static int __fastcall GrowEntryBuffer(void* self, void* /*edx*/, int additionalCapacity)
{
    const std::uint32_t newCapacity =
        CSF_AT(std::uint32_t, self, 0x4) +
        static_cast<std::uint32_t>(additionalCapacity);

    void* replacement = reinterpret_cast<void*>(
        CoreMemory_AllocUntagged(16 * newCapacity));
    int result = 0;

    if (replacement &&
        static_cast<std::int32_t>(newCapacity - 1) >= 0)
    {
        auto* entry = static_cast<std::uint32_t*>(replacement);
        std::uint32_t remaining = newCapacity;
        do
        {
            entry[0] = 0;
            entry[1] = static_cast<std::uint32_t>(-1);
            entry[2] = static_cast<std::uint32_t>(-1);
            entry[3] = static_cast<std::uint32_t>(-1);
            entry += 4;
        } while (--remaining);
    }

    const std::uint32_t count = CSF_AT(std::uint32_t, self, 0x8);
    if (count > 0)
    {
        std::memcpy(replacement, CSF_AT(void*, self, 0x0), 16 * count);
    }

    const std::uint32_t updatedCapacity =
        CSF_AT(std::uint32_t, self, 0x4) +
        static_cast<std::uint32_t>(additionalCapacity);
    void* oldBuffer = CSF_AT(void*, self, 0x0);
    CSF_AT(std::uint32_t, self, 0x4) = updatedCapacity;

    if (oldBuffer)
    {
        result = CoreMemory_Free(oldBuffer);
        CSF_AT(void*, self, 0x0) = nullptr;
    }

    CSF_AT(void*, self, 0x0) = replacement;
    return result;
}

HOOK(0x006E6B80, GrowEntryBuffer, "auto");
