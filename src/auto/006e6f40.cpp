#include <cstdint>
#include "globals.h"
#include <cstddef>
#include <cstring>
#include "hook.h"
#include "core/memory.h"

CSF_FUNC(int, __cdecl, CoreMemory_AllocUntagged, 0x006910F0, (std::size_t size));
CSF_FUNC(int, __cdecl, CoreMemory_Free, 0x00691110, (void* block));

static const void* __fastcall GrowEntryBuffer(
    void* self, void* /*edx*/, int additionalCapacity)
{
    const std::uint32_t capacity =
        CSF_AT(std::uint32_t, self, 0x04) +
        static_cast<std::uint32_t>(additionalCapacity);

    void* newBuffer = reinterpret_cast<void*>(
        CoreMemory_AllocUntagged(40 * capacity));

    if (newBuffer &&
        static_cast<std::int32_t>(capacity - 1) >= 0)
    {
        auto* entry = static_cast<std::uint8_t*>(newBuffer);
        std::uint32_t remaining = capacity;
        do
        {
            CSF_AT(std::uint32_t, entry, 0x00) = 0;
            CSF_AT(std::uint32_t, entry, 0x04) = 0;
            CSF_AT(std::uint32_t, entry, 0x08) = 0;
            CSF_AT(std::uint32_t, entry, 0x0C) = 0;
            CSF_AT(std::uint32_t, entry, 0x10) = 0;
            CSF_AT(std::uint32_t, entry, 0x14) = 0;
            CSF_AT(std::uint32_t, entry, 0x18) = 0;
            CSF_AT(std::int32_t, entry, 0x1C) = -1;
            CSF_AT(std::int32_t, entry, 0x20) = -1;
            CSF_AT(float, entry, 0x24) = -1.0f;
            entry += 40;
        } while (--remaining);
    }

    const std::uint32_t count = CSF_AT(std::uint32_t, self, 0x08);
    if (count > 0)
    {
        std::memcpy(
            newBuffer, CSF_AT(void*, self, 0x00), 40 * count);
    }

    const std::uint32_t updatedCapacity =
        CSF_AT(std::uint32_t, self, 0x04) +
        static_cast<std::uint32_t>(additionalCapacity);
    void* oldBuffer = CSF_AT(void*, self, 0x00);
    const void* result = oldBuffer;
    CSF_AT(std::uint32_t, self, 0x04) = updatedCapacity;

    if (oldBuffer)
    {
        result = reinterpret_cast<const void*>(CoreMemory_Free(oldBuffer));
        CSF_AT(void*, self, 0x00) = nullptr;
    }

    CSF_AT(void*, self, 0x00) = newBuffer;
    return result;
}

HOOK(0x006E6F40, GrowEntryBuffer, "auto");
