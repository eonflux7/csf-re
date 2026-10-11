#include <cstdint>
#include "globals.h"
#include <cstddef>
#include <cstring>
#include "hook.h"
#include "core/memory.h"

CSF_FUNC(void*, __cdecl, Mem_Alloc, 0x006910F0, (std::size_t size));
CSF_FUNC(int, __cdecl, Mem_Free, 0x00691110, (void* block));

static int __fastcall RecordArray_GrowCapacity(void* self, void* /*edx*/, int additionalCapacity)
{
    const std::uint32_t newCapacity =
        CSF_AT(std::uint32_t, self, 0x04) +
        static_cast<std::uint32_t>(additionalCapacity);

    void* newStorage = Mem_Alloc(
        static_cast<std::size_t>(28u * newCapacity));

    if (newStorage &&
        static_cast<std::int32_t>(newCapacity - 1u) >= 0)
    {
        auto* record = static_cast<std::uint8_t*>(newStorage);
        std::uint32_t remaining = newCapacity;
        do
        {
            CSF_AT(std::uint32_t, record, 0x04) = 0;
            CSF_AT(std::uint32_t, record, 0x08) = 0;
            CSF_AT(std::uint32_t, record, 0x0C) = 0;
            CSF_AT(std::uint32_t, record, 0x10) = 0;
            CSF_AT(std::uint32_t, record, 0x14) = 0;
            CSF_AT(float, record, 0x18) = 1.0f;
            CSF_AT(std::uint32_t, record, 0x00) = 0;
            record += 28;
        }
        while (--remaining);
    }

    const std::uint32_t usedCount = CSF_AT(std::uint32_t, self, 0x08);
    if (usedCount > 0)
    {
        std::memcpy(newStorage, CSF_AT(void*, self, 0x00),
                    static_cast<std::size_t>(28u * usedCount));
    }

    const std::uint32_t updatedCapacity =
        CSF_AT(std::uint32_t, self, 0x04) +
        static_cast<std::uint32_t>(additionalCapacity);
    void* oldStorage = CSF_AT(void*, self, 0x00);
    CSF_AT(std::uint32_t, self, 0x04) = updatedCapacity;

    int result;
    if (oldStorage)
    {
        result = Mem_Free(oldStorage);
        CSF_AT(void*, self, 0x00) = nullptr;
    }
    else
    {
        result = static_cast<int>(
            reinterpret_cast<std::uintptr_t>(newStorage));
    }

    CSF_AT(void*, self, 0x00) = newStorage;
    return result;
}

HOOK(0x007340D0, RecordArray_GrowCapacity, "auto");
