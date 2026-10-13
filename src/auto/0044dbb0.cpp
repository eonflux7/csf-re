#include <cstdint>
#include <cstddef>
#include <cstring>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"

CSF_FUNC(void*, __cdecl, Mem_Alloc, 0x006910F0, (std::size_t size));
CSF_FUNC(int, __cdecl, Mem_Free, 0x00691110, (void* block));
CSF_FUNC(void*, __cdecl, _memcpy, 0x007AEE60, (void* destination, const void* source, std::size_t byteCount));

static int __fastcall DynamicArray_InsertRecord(
    void* self, void* /*edx*/, int index, std::uint32_t* record)
{
    const std::uint32_t count = CSF_AT(std::uint32_t, self, 0x08);
    const std::uint32_t capacity = CSF_AT(std::uint32_t, self, 0x04);
    const std::uint32_t position = static_cast<std::uint32_t>(index);
    const std::uint32_t offset = 12u * position;

    if (count < capacity)
    {
        std::uint8_t* source = CSF_AT(std::uint8_t*, self, 0x00) + offset;
        _memcpy(source + 12, source, 12u * (count - position));

        std::uint8_t* destination =
            CSF_AT(std::uint8_t*, self, 0x00) + offset;
        CSF_AT(std::uint32_t, destination, 0x00) = record[0];
        CSF_AT(std::uint32_t, destination, 0x04) = record[1];
        CSF_AT(std::uint32_t, destination, 0x08) = record[2];

        const std::uint32_t newCount =
            CSF_AT(std::uint32_t, self, 0x08) + 1u;
        CSF_AT(std::uint32_t, self, 0x08) = newCount;
        return static_cast<int>(newCount);
    }

    const std::uint32_t allocationCapacity =
        capacity + CSF_AT(std::uint32_t, self, 0x0C);
    auto* replacement = static_cast<std::uint8_t*>(
        Mem_Alloc(12u * allocationCapacity));

    if (replacement &&
        static_cast<std::int32_t>(allocationCapacity - 1u) >= 0)
    {
        std::uint8_t* entry = replacement;
        std::uint32_t remaining = allocationCapacity;
        do
        {
            CSF_AT(std::uint32_t, entry, 0x00) = 0;
            CSF_AT(std::uint8_t, entry, 0x08) = 0;
            CSF_AT(std::uint32_t, entry, 0x04) = 0;
            entry += 12;
        } while (--remaining);
    }

    std::memcpy(replacement, CSF_AT(std::uint8_t*, self, 0x00), offset);

    std::uint8_t* destination = replacement + offset;
    CSF_AT(std::uint32_t, destination, 0x00) = record[0];
    CSF_AT(std::uint32_t, destination, 0x04) = record[1];
    CSF_AT(std::uint32_t, destination, 0x08) = record[2];

    std::memcpy(
        destination + 12,
        CSF_AT(std::uint8_t*, self, 0x00) + offset,
        12u * (CSF_AT(std::uint32_t, self, 0x08) - position));

    const std::uint32_t newCapacity =
        CSF_AT(std::uint32_t, self, 0x04) +
        CSF_AT(std::uint32_t, self, 0x0C);
    void* oldBuffer = CSF_AT(std::uint8_t*, self, 0x00);
    CSF_AT(std::uint32_t, self, 0x04) = newCapacity;

    if (oldBuffer)
    {
        Mem_Free(oldBuffer);
        CSF_AT(std::uint8_t*, self, 0x00) = nullptr;
    }

    const std::uint32_t newCount =
        CSF_AT(std::uint32_t, self, 0x08) + 1u;
    CSF_AT(std::uint8_t*, self, 0x00) = replacement;
    CSF_AT(std::uint32_t, self, 0x08) = newCount;
    return static_cast<int>(newCount);
}

HOOK(0x0044DBB0, DynamicArray_InsertRecord, "auto");
