#include <cstdint>
#include <cstddef>
#include <cstring>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"

CSF_FUNC(int, __cdecl, CoreMemory_AllocUntagged, 0x006910F0, (std::size_t size));
CSF_FUNC(int, __cdecl, CoreMemory_Free, 0x00691110, (void* block));

static char* __fastcall GfxCollision_GrowRecords(
    const void** self, void* /*edx*/, int additionalCapacity)
{
    const std::uint32_t newCapacity =
        CSF_AT(std::uint32_t, self, 0x04) +
        static_cast<std::uint32_t>(additionalCapacity);

    char* newRecords = reinterpret_cast<char*>(
        CoreMemory_AllocUntagged(std::uint32_t(104 * newCapacity)));

    if (newRecords &&
        static_cast<std::int32_t>(newCapacity - 1) >= 0)
    {
        char* record = newRecords;
        std::uint32_t remaining = newCapacity;
        do
        {
            CSF_AT(std::uint32_t, record, 0x04) = 0;
            CSF_AT(std::uint32_t, record, 0x08) = 0;
            CSF_AT(std::uint32_t, record, 0x0C) = 0;
            CSF_AT(std::uint32_t, record, 0x10) = 0;
            CSF_AT(std::uint32_t, record, 0x14) = 0;
            CSF_AT(std::uint32_t, record, 0x18) = 0;
            CSF_AT(std::uint32_t, record, 0x5C) = 0;
            CSF_AT(std::uint8_t, record, 0x28) = 0;
            CSF_AT(std::int32_t, record, 0x2C) = -1;
            CSF_AT(std::int8_t, record, 0x30) = -1;
            CSF_AT(std::uint32_t, record, 0x60) = 0;
            CSF_AT(std::int32_t, record, 0x64) = 10;
            CSF_AT(std::uint32_t, record, 0x24) = 0;
            CSF_AT(std::int32_t, record, 0x1C) = -1;
            CSF_AT(std::int8_t, record, 0x20) = -1;
            record += 104;
        } while (--remaining);
    }

    const std::uint32_t usedCount = CSF_AT(std::uint32_t, self, 0x08);
    if (usedCount != 0)
    {
        std::memcpy(newRecords, CSF_AT(void*, self, 0x00),
                    std::uint32_t(104 * usedCount));
    }

    const std::uint32_t updatedCapacity =
        CSF_AT(std::uint32_t, self, 0x04) +
        static_cast<std::uint32_t>(additionalCapacity);
    void* oldRecords = CSF_AT(void*, self, 0x00);
    CSF_AT(std::uint32_t, self, 0x04) = updatedCapacity;

    if (oldRecords)
    {
        char* result = reinterpret_cast<char*>(CoreMemory_Free(oldRecords));
        CSF_AT(void*, self, 0x00) = nullptr;
        CSF_AT(char*, self, 0x00) = newRecords;
        return result;
    }

    CSF_AT(char*, self, 0x00) = newRecords;
    return newRecords;
}

HOOK(0x006EC910, GfxCollision_GrowRecords, "auto");
