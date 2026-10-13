#include <cstdint>
#include <cstddef>
#include <cstring>
#include "globals.h"
#include "hook.h"

CSF_FUNC(int, __cdecl, CoreMemory_AllocUntagged, 0x006910F0, (std::size_t size));
CSF_FUNC(int, __cdecl, CoreMemory_Free, 0x00691110, (void* block));

static const void* __fastcall GameSceneAccess_GrowRecordArray(
    void* self, void* /*edx*/, int additionalCapacity)
{
    const std::uint32_t newCapacity =
        CSF_AT(std::uint32_t, self, 0x04) +
        static_cast<std::uint32_t>(additionalCapacity);

    void* newRecords = reinterpret_cast<void*>(
        static_cast<std::uintptr_t>(
            CoreMemory_AllocUntagged(48 * newCapacity)));

    if (newRecords &&
        static_cast<std::int32_t>(newCapacity - 1) >= 0)
    {
        auto* record = static_cast<std::uint8_t*>(newRecords);
        std::uint32_t remaining = newCapacity;
        do
        {
            std::memset(record, 0, 48);
            record += 48;
        } while (--remaining);
    }

    const std::uint32_t recordCount =
        CSF_AT(std::uint32_t, self, 0x08);
    if (recordCount > 0)
    {
        std::memcpy(
            newRecords,
            CSF_AT(void*, self, 0x00),
            48 * recordCount);
    }

    const std::uint32_t updatedCapacity =
        CSF_AT(std::uint32_t, self, 0x04) +
        static_cast<std::uint32_t>(additionalCapacity);
    void* oldRecords = CSF_AT(void*, self, 0x00);
    CSF_AT(std::uint32_t, self, 0x04) = updatedCapacity;

    const void* result = oldRecords;
    if (oldRecords)
    {
        result = reinterpret_cast<const void*>(
            static_cast<std::uintptr_t>(CoreMemory_Free(oldRecords)));
        CSF_AT(void*, self, 0x00) = nullptr;
    }

    CSF_AT(void*, self, 0x00) = newRecords;
    return result;
}

HOOK(0x0044F990, GameSceneAccess_GrowRecordArray, "auto");
