#include <cstdint>
#include <cstddef>
#include <cstring>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"

CSF_FUNC(void*, __cdecl, Mem_Alloc, 0x006910F0, (std::size_t size));
CSF_FUNC(int, __cdecl, Mem_Free, 0x00691110, (void* block));
CSF_FUNC(void*, __cdecl, _memcpy, 0x007AEE60, (void* destination, const void* source, std::size_t byteCount));

static int __fastcall GfxCollision_InsertRecord(
    void* self, void* /*edx*/, int index, const void* record)
{
    const std::uint32_t count = CSF_AT(std::uint32_t, self, 0x08);
    const std::uint32_t capacity = CSF_AT(std::uint32_t, self, 0x04);

    if (count < capacity)
    {
        const std::uint32_t offset = 104u * static_cast<std::uint32_t>(index);
        auto* data = CSF_AT(std::uint8_t*, self, 0x00);

        _memcpy(data + offset + 104, data + offset,
                104u * (count - static_cast<std::uint32_t>(index)));

        data = CSF_AT(std::uint8_t*, self, 0x00);
        std::memcpy(data + offset, record, 0x68u);

        const std::uint32_t newCount =
            CSF_AT(std::uint32_t, self, 0x08) + 1;
        CSF_AT(std::uint32_t, self, 0x08) = newCount;
        return static_cast<int>(newCount);
    }

    const std::uint32_t newCapacity =
        capacity + CSF_AT(std::uint32_t, self, 0x0C);
    auto* newData =
        static_cast<std::uint8_t*>(Mem_Alloc(104u * newCapacity));

    if (newData && static_cast<std::int32_t>(newCapacity - 1) >= 0)
    {
        auto* entry = newData;
        std::uint32_t remaining = newCapacity;
        do
        {
            CSF_AT(std::uint32_t, entry, 0x04) = 0;
            CSF_AT(std::uint32_t, entry, 0x08) = 0;
            CSF_AT(std::uint32_t, entry, 0x0C) = 0;
            CSF_AT(std::uint32_t, entry, 0x10) = 0;
            CSF_AT(std::uint32_t, entry, 0x14) = 0;
            CSF_AT(std::uint32_t, entry, 0x18) = 0;
            CSF_AT(std::uint32_t, entry, 0x5C) = 0;
            CSF_AT(std::uint8_t, entry, 0x28) = 0;
            CSF_AT(std::int32_t, entry, 0x2C) = -1;
            CSF_AT(std::int8_t, entry, 0x30) = -1;
            CSF_AT(std::uint32_t, entry, 0x60) = 0;
            CSF_AT(std::uint32_t, entry, 0x64) = 10;
            CSF_AT(std::uint32_t, entry, 0x24) = 0;
            CSF_AT(std::int32_t, entry, 0x1C) = -1;
            CSF_AT(std::int8_t, entry, 0x20) = -1;
            entry += 104;
        } while (--remaining);
    }

    const std::uint32_t offset = 104u * static_cast<std::uint32_t>(index);
    std::memcpy(newData, CSF_AT(const void*, self, 0x00), offset);
    std::memcpy(newData + offset, record, 0x68u);
    std::memcpy(
        newData + offset + 104,
        CSF_AT(const std::uint8_t*, self, 0x00) + offset,
        104u * (CSF_AT(std::uint32_t, self, 0x08) -
                static_cast<std::uint32_t>(index)));

    void* oldData = CSF_AT(void*, self, 0x00);
    CSF_AT(std::uint32_t, self, 0x04) =
        CSF_AT(std::uint32_t, self, 0x04) +
        CSF_AT(std::uint32_t, self, 0x0C);

    if (oldData)
    {
        Mem_Free(oldData);
        CSF_AT(void*, self, 0x00) = nullptr;
    }

    const std::uint32_t newCount =
        CSF_AT(std::uint32_t, self, 0x08) + 1;
    CSF_AT(std::uint8_t*, self, 0x00) = newData;
    CSF_AT(std::uint32_t, self, 0x08) = newCount;
    return static_cast<int>(newCount);
}

HOOK(0x006EC7C0, GfxCollision_InsertRecord, "auto");
