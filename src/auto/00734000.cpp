#include <cstdint>
#include "globals.h"
#include <cstddef>
#include <cstring>
#include "hook.h"
#include "core/memory.h"

CSF_FUNC(void*, __cdecl, Mem_Alloc, 0x006910F0, (std::size_t size));
CSF_FUNC(int, __cdecl, Mem_Free, 0x00691110, (void* block));

static const void* __fastcall ExpandRecordBuffer(void* self, void* /*edx*/, int additionalCapacity)
{
    const std::uint32_t newCapacity =
        CSF_AT(std::uint32_t, self, 0x04) +
        static_cast<std::uint32_t>(additionalCapacity);

    void* newBuffer = Mem_Alloc(48u * newCapacity);
    if (newBuffer &&
        static_cast<std::int32_t>(newCapacity - 1u) >= 0)
    {
        auto* record = static_cast<std::uint8_t*>(newBuffer);
        std::uint32_t remaining = newCapacity;
        do
        {
            CSF_AT(std::uint32_t, record, 0x08) = 0;
            CSF_AT(std::uint32_t, record, 0x0C) = 0;
            CSF_AT(std::int32_t, record, 0x04) = -1;
            CSF_AT(std::uint32_t, record, 0x14) = 0;
            CSF_AT(std::uint32_t, record, 0x18) = 0;
            CSF_AT(std::uint32_t, record, 0x1C) = 0;
            CSF_AT(std::uint32_t, record, 0x20) = 0;
            CSF_AT(std::uint32_t, record, 0x24) = 0;
            CSF_AT(float, record, 0x28) = 1.0f;
            CSF_AT(std::uint32_t, record, 0x10) = 0;
            CSF_AT(std::int32_t, record, 0x00) = -1;
            record += 48;
        } while (--remaining);
    }

    const std::uint32_t usedCount = CSF_AT(std::uint32_t, self, 0x08);
    if (usedCount > 0)
    {
        std::memcpy(newBuffer, CSF_AT(void*, self, 0x00), 48u * usedCount);
    }

    const std::uint32_t updatedCapacity =
        CSF_AT(std::uint32_t, self, 0x04) +
        static_cast<std::uint32_t>(additionalCapacity);
    void* oldBuffer = CSF_AT(void*, self, 0x00);
    CSF_AT(std::uint32_t, self, 0x04) = updatedCapacity;

    const void* result = oldBuffer;
    if (oldBuffer)
    {
        result = reinterpret_cast<const void*>(
            static_cast<std::uintptr_t>(Mem_Free(oldBuffer)));
        CSF_AT(void*, self, 0x00) = nullptr;
    }

    CSF_AT(void*, self, 0x00) = newBuffer;
    return result;
}

HOOK(0x00734000, ExpandRecordBuffer, "auto");
