#include <cstdint>
#include "globals.h"
#include <cstddef>
#include <cstring>
#include "hook.h"
#include "core/memory.h"

CSF_FUNC(void*, __cdecl, Mem_Alloc, 0x006910F0, (std::size_t size));
CSF_FUNC(int, __cdecl, Mem_Free, 0x00691110, (void* block));

static const void* __fastcall GrowRecordBuffer(void* self, void* /*edx*/, int additionalCapacity)
{
    const std::uint32_t newCapacity =
        CSF_AT(std::uint32_t, self, 0x04) +
        static_cast<std::uint32_t>(additionalCapacity);

    void* newBuffer = Mem_Alloc(static_cast<std::size_t>(16u * newCapacity));
    if (newBuffer && static_cast<std::int32_t>(newCapacity - 1u) >= 0)
    {
        auto* record = static_cast<std::uint8_t*>(newBuffer);
        std::uint32_t remaining = newCapacity;
        do
        {
            CSF_AT(std::uint32_t, record, 0x04) = 0;
            CSF_AT(std::uint32_t, record, 0x08) = 0;
            CSF_AT(std::uint32_t, record, 0x0C) = 0;
            record += 16;
            --remaining;
        } while (remaining);
    }

    const std::uint32_t count = CSF_AT(std::uint32_t, self, 0x08);
    if (count > 0)
    {
        std::memcpy(newBuffer, CSF_AT(void*, self, 0x00),
                    static_cast<std::size_t>(16u * count));
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

HOOK(0x00443A70, GrowRecordBuffer, "auto");
