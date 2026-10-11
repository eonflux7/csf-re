#include <cstdint>
#include <cstddef>
#include <cstring>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"

CSF_FUNC(void*, __cdecl, Mem_Alloc, 0x006910F0, (std::size_t size));
CSF_FUNC(int, __cdecl, Mem_Free, 0x00691110, (void* block));
CSF_FUNC(void*, __cdecl, _memcpy, 0x007AEE60, (void* destination, const void* source, std::size_t byteCount));

static int __fastcall GameConfig_InsertRecord(
    void* self, void* /*edx*/, int index, std::uint32_t* record)
{
    const std::uint32_t count = CSF_AT(std::uint32_t, self, 0x08);
    const std::uint32_t capacity = CSF_AT(std::uint32_t, self, 0x04);
    const std::uint32_t byteOffset = 12 * static_cast<std::uint32_t>(index);

    if (count < capacity)
    {
        auto* source = CSF_AT(std::uint8_t*, self, 0x00) + byteOffset;
        _memcpy(source + 12, source,
                12 * (count - static_cast<std::uint32_t>(index)));

        auto* destination = reinterpret_cast<std::uint32_t*>(
            CSF_AT(std::uint8_t*, self, 0x00) + byteOffset);
        destination[0] = record[0];
        destination[1] = record[1];
        destination[2] = record[2];

        const std::uint32_t result = CSF_AT(std::uint32_t, self, 0x08) + 1;
        CSF_AT(std::uint32_t, self, 0x08) = result;
        return static_cast<int>(result);
    }

    const std::uint32_t newCapacity =
        capacity + CSF_AT(std::uint32_t, self, 0x0C);
    auto* newBuffer = static_cast<std::uint8_t*>(Mem_Alloc(12 * newCapacity));

    if (newBuffer && static_cast<std::int32_t>(newCapacity - 1) >= 0)
    {
        auto* entry = reinterpret_cast<std::uint32_t*>(newBuffer);
        std::uint32_t remaining = newCapacity;
        do
        {
            entry[0] = 0;
            entry[1] = 0;
            entry[2] = 0;
            entry += 3;
        } while (--remaining);
    }

    std::memcpy(newBuffer, CSF_AT(void*, self, 0x00), byteOffset);

    auto* insertion = newBuffer + byteOffset;
    auto* destination = reinterpret_cast<std::uint32_t*>(insertion);
    destination[0] = record[0];
    destination[1] = record[1];
    destination[2] = record[2];

    const std::uint32_t tailBytes =
        12 * (CSF_AT(std::uint32_t, self, 0x08) -
              static_cast<std::uint32_t>(index));
    std::memcpy(insertion + 12,
                CSF_AT(std::uint8_t*, self, 0x00) + byteOffset,
                tailBytes);

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

    const std::uint32_t result = CSF_AT(std::uint32_t, self, 0x08) + 1;
    CSF_AT(void*, self, 0x00) = newBuffer;
    CSF_AT(std::uint32_t, self, 0x08) = result;
    return static_cast<int>(result);
}

HOOK(0x0075A1D0, GameConfig_InsertRecord, "auto");
