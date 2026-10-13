#include <cstdint>
#include <cstddef>
#include <cstring>
#include "globals.h"
#include "hook.h"

CSF_FUNC(void*, __cdecl, Mem_Alloc, 0x006910F0, (std::size_t size));
CSF_FUNC(int, __cdecl, Mem_Free, 0x00691110, (void* block));

static int __fastcall SceneLightsObjects_GrowRecordBuffer(
    void* self, void* /*edx*/, int additionalCapacity)
{
    const std::uint32_t newCapacity =
        CSF_AT(std::uint32_t, self, 0x04) +
        static_cast<std::uint32_t>(additionalCapacity);

    void* newBuffer = Mem_Alloc(36 * newCapacity);
    if (newBuffer &&
        static_cast<std::int32_t>(newCapacity - 1) >= 0)
    {
        auto* record = static_cast<std::uint8_t*>(newBuffer);
        std::uint32_t remaining = newCapacity;
        do
        {
            CSF_AT(std::uint32_t, record, 0x00) = 0;
            CSF_AT(std::uint32_t, record, 0x04) = 0;
            CSF_AT(std::uint32_t, record, 0x08) = 0;
            CSF_AT(float, record, 0x0C) = 1.0f;
            CSF_AT(std::uint32_t, record, 0x10) = 0;
            CSF_AT(std::uint32_t, record, 0x14) = 0;
            CSF_AT(std::uint32_t, record, 0x18) = 0;
            record += 0x24;
        } while (--remaining);
    }

    const std::uint32_t usedCount = CSF_AT(std::uint32_t, self, 0x08);
    if (usedCount > 0)
    {
        std::memcpy(newBuffer, CSF_AT(void*, self, 0x00),
                    36 * usedCount);
    }

    void* oldBuffer = CSF_AT(void*, self, 0x00);
    CSF_AT(std::uint32_t, self, 0x04) +=
        static_cast<std::uint32_t>(additionalCapacity);

    int result;
    if (oldBuffer)
    {
        result = Mem_Free(oldBuffer);
        CSF_AT(void*, self, 0x00) = nullptr;
    }
    else
    {
        result = static_cast<int>(
            reinterpret_cast<std::uintptr_t>(newBuffer));
    }

    CSF_AT(void*, self, 0x00) = newBuffer;
    return result;
}

HOOK(0x004F0510, SceneLightsObjects_GrowRecordBuffer, "auto");
