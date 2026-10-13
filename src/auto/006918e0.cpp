#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_FUNC(void*, __cdecl, Mem_Alloc, 0x006910F0, (std::size_t size));
CSF_FUNC(int, __cdecl, Mem_Free, 0x00691110, (void* block));
CSF_FUNC(void*, __cdecl, _memcpy, 0x007AEE60, (void* destination, const void* source, std::size_t byteCount));

static int __fastcall CoreMemory_InsertRecord(
    void* self, void* /*edx*/, int index, const std::uint32_t* record)
{
    const std::uint32_t count = CSF_AT(std::uint32_t, self, 0x08);
    const std::uint32_t capacity = CSF_AT(std::uint32_t, self, 0x04);
    const std::uint32_t position = static_cast<std::uint32_t>(index);
    const std::uint32_t byteOffset = 16 * position;

    if (count < capacity)
    {
        auto* source = CSF_AT(std::uint8_t*, self, 0x00) + byteOffset;
        _memcpy(source + 16, source, 16 * (count - position));

        auto* destination = reinterpret_cast<std::uint32_t*>(
            CSF_AT(std::uint8_t*, self, 0x00) + byteOffset);
        destination[0] = record[0];
        destination[1] = record[1];
        destination[2] = record[2];
        destination[3] = record[3];

        const std::uint32_t newCount =
            CSF_AT(std::uint32_t, self, 0x08) + 1;
        CSF_AT(std::uint32_t, self, 0x08) = newCount;
        return static_cast<int>(newCount);
    }

    const std::uint32_t newCapacity =
        capacity + CSF_AT(std::uint32_t, self, 0x0C);
    auto* newBuffer = static_cast<std::uint8_t*>(
        Mem_Alloc(16 * newCapacity));

    if (newBuffer &&
        static_cast<std::int32_t>(newCapacity - 1) >= 0)
    {
        auto* entry = newBuffer;
        std::uint32_t remaining = newCapacity;
        do
        {
            CSF_AT(std::uint32_t, entry, 0x00) = 0;
            CSF_AT(std::uint32_t, entry, 0x04) = 0;
            CSF_AT(std::uint8_t, entry, 0x08) = 1;
            CSF_AT(std::uint8_t, entry, 0x09) = 0;
            CSF_AT(std::uint32_t, entry, 0x0C) = 0;
            entry += 16;
        } while (--remaining);
    }

    // These copies were inline forward copies, not calls to _memcpy.
    auto* prefixSource = CSF_AT(std::uint32_t*, self, 0x00);
    auto* prefixDestination = reinterpret_cast<std::uint32_t*>(newBuffer);
    for (std::uint32_t i = 0; i < (byteOffset >> 2); ++i)
        prefixDestination[i] = prefixSource[i];

    auto* inserted = reinterpret_cast<std::uint32_t*>(
        newBuffer + byteOffset);
    inserted[0] = record[0];
    inserted[1] = record[1];
    inserted[2] = record[2];
    inserted[3] = record[3];

    const std::uint32_t suffixBytes =
        16 * (CSF_AT(std::uint32_t, self, 0x08) - position);
    auto* suffixSource = reinterpret_cast<std::uint32_t*>(
        CSF_AT(std::uint8_t*, self, 0x00) + byteOffset);
    auto* suffixDestination = inserted + 4;
    for (std::uint32_t i = 0; i < (suffixBytes >> 2); ++i)
        suffixDestination[i] = suffixSource[i];

    const std::uint32_t updatedCapacity =
        CSF_AT(std::uint32_t, self, 0x04) +
        CSF_AT(std::uint32_t, self, 0x0C);
    void* oldBuffer = CSF_AT(void*, self, 0x00);
    CSF_AT(std::uint32_t, self, 0x04) = updatedCapacity;

    if (oldBuffer)
    {
        Mem_Free(oldBuffer);
        CSF_AT(void*, self, 0x00) = nullptr;
    }

    const std::uint32_t newCount =
        CSF_AT(std::uint32_t, self, 0x08) + 1;
    CSF_AT(std::uint8_t*, self, 0x00) = newBuffer;
    CSF_AT(std::uint32_t, self, 0x08) = newCount;
    return static_cast<int>(newCount);
}

HOOK(0x006918E0, CoreMemory_InsertRecord, "auto");
