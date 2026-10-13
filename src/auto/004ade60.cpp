#include <cstdint>
#include "globals.h"
#include <cstddef>
#include <cstring>
#include "hook.h"
#include "core/memory.h"

CSF_FUNC(int, __cdecl, CoreMemory_AllocUntagged, 0x006910F0, (std::size_t size));
CSF_FUNC(int, __cdecl, CoreMemory_Free, 0x00691110, (void* block));

static int __fastcall CoverAttackTasks_GrowRecordArray(
    void* self, void* /*edx*/, int additionalCapacity)
{
    const std::uint32_t newCapacity =
        CSF_AT(std::uint32_t, self, 0x4) +
        static_cast<std::uint32_t>(additionalCapacity);

    auto* newRecords = reinterpret_cast<std::uint8_t*>(
        CoreMemory_AllocUntagged(28 * newCapacity));

    if (newRecords &&
        static_cast<std::int32_t>(newCapacity - 1) >= 0)
    {
        auto* record = newRecords;
        std::uint32_t remaining = newCapacity;
        do
        {
            CSF_AT(std::uint32_t, record, 0x00) = 0;
            CSF_AT(std::uint32_t, record, 0x04) = 0;
            CSF_AT(std::uint32_t, record, 0x08) = 0;
            CSF_AT(std::uint32_t, record, 0x0C) = 0;
            CSF_AT(std::uint32_t, record, 0x10) = 0;
            CSF_AT(std::uint32_t, record, 0x14) = 0;
            CSF_AT(std::uint32_t, record, 0x18) = 2139095039;
            record += 28;
        } while (--remaining);
    }

    const std::uint32_t recordCount = CSF_AT(std::uint32_t, self, 0x8);
    if (recordCount > 0)
    {
        std::memcpy(newRecords, CSF_AT(void*, self, 0x0),
                    28 * recordCount);
    }

    const std::uint32_t updatedCapacity =
        CSF_AT(std::uint32_t, self, 0x4) +
        static_cast<std::uint32_t>(additionalCapacity);
    void* oldRecords = CSF_AT(void*, self, 0x0);
    CSF_AT(std::uint32_t, self, 0x4) = updatedCapacity;

    int result;
    if (oldRecords)
    {
        result = CoreMemory_Free(oldRecords);
        CSF_AT(void*, self, 0x0) = nullptr;
    }
    else
    {
        result = reinterpret_cast<int>(newRecords);
    }

    CSF_AT(void*, self, 0x0) = newRecords;
    return result;
}

HOOK(0x004ADE60, CoverAttackTasks_GrowRecordArray, "auto");
