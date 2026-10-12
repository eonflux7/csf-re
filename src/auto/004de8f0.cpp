#include <cstdint>
#include <cstddef>
#include <cstring>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"

CSF_FUNC(void*, __cdecl, Mem_Alloc, 0x006910F0, (std::size_t size));
CSF_FUNC(int, __cdecl, Mem_Free, 0x00691110, (void* block));

static const void* __fastcall GrowRecordBuffer(void* self, void* /*edx*/, int additionalCount)
{
    const std::uint32_t newCapacity =
        CSF_AT(std::uint32_t, self, 0x04) +
        static_cast<std::uint32_t>(additionalCount);

    auto* newBuffer = static_cast<std::uint8_t*>(Mem_Alloc(newCapacity << 6));

    if (newBuffer &&
        static_cast<std::int32_t>(newCapacity - 1) >= 0)
    {
        auto* recordFields = newBuffer + 0x24;
        std::uint32_t remaining = newCapacity;
        do
        {
            CSF_AT(float, recordFields, -8) = 0.0f;
            CSF_AT(float, recordFields, -4) = 0.0f;
            CSF_AT(float, recordFields, 0) = 0.0f;
            CSF_AT(float, recordFields, -0x14) = 0.0f;
            CSF_AT(float, recordFields, -0x10) = 0.0f;
            CSF_AT(float, recordFields, -0x0C) = 0.0f;
            CSF_AT(float, recordFields, -8) = 1.0f;
            CSF_AT(float, recordFields, -4) = 0.0f;
            CSF_AT(float, recordFields, 0) = 0.0f;
            CSF_AT(std::uint8_t, recordFields, 0x14) = 1;
            recordFields += 0x40;
        } while (--remaining);
    }

    const std::uint32_t usedCount = CSF_AT(std::uint32_t, self, 0x08);
    if (usedCount > 0)
    {
        std::memcpy(newBuffer, CSF_AT(void*, self, 0x00), usedCount << 6);
    }

    const std::uint32_t capacity = CSF_AT(std::uint32_t, self, 0x04);
    void* oldBuffer = CSF_AT(void*, self, 0x00);
    CSF_AT(std::uint32_t, self, 0x04) =
        capacity + static_cast<std::uint32_t>(additionalCount);

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

HOOK(0x004DE8F0, GrowRecordBuffer, "auto");
