#include <cstdint>
#include <cstddef>
#include <cstring>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_FUNC(void*, __cdecl, Mem_Alloc, 0x006910F0, (std::size_t size));
CSF_FUNC(int, __cdecl, Mem_Free, 0x00691110, (void* block));

static std::uint32_t* __fastcall GrowRecordArrayCapacity(
    void* self, void* /*edx*/, int additionalCapacity)
{
    const std::uint32_t newCapacity =
        CSF_AT(std::uint32_t, self, 0x04) +
        static_cast<std::uint32_t>(additionalCapacity);

    auto* allocation = static_cast<std::uint8_t*>(
        Mem_Alloc(40u * newCapacity + 4u));
    std::uint8_t* newRecords = nullptr;

    if (allocation)
    {
        newRecords = allocation + 4;
        CSF_AT(std::uint32_t, allocation, 0x00) = newCapacity;

        if (static_cast<std::int32_t>(newCapacity - 1u) >= 0)
        {
            auto* record = newRecords;
            std::uint32_t remaining = newCapacity;
            do
            {
                CSF_AT(void**, record, 0x04) = CSF_VTBL(0x0082D448);
                CSF_AT(std::int32_t, record, 0x08) = -1;
                CSF_AT(std::int32_t, record, 0x0C) = -1;
                CSF_AT(std::uint32_t, record, 0x10) = 0;
                CSF_AT(std::uint32_t, record, 0x14) = 0;
                CSF_AT(std::uint32_t, record, 0x18) = 0;
                CSF_AT(std::uint32_t, record, 0x1C) = 0;
                CSF_AT(std::uint32_t, record, 0x20) = 0;
                CSF_AT(std::uint8_t, record, 0x00) = 0;
                CSF_AT(std::uint32_t, record, 0x24) = 0;
                record += 40;
            } while (--remaining);
        }
    }

    const std::uint32_t usedCount = CSF_AT(std::uint32_t, self, 0x08);
    if (usedCount > 0)
    {
        std::memcpy(newRecords, CSF_AT(void*, self, 0x00), 40u * usedCount);
    }

    const std::uint32_t updatedCapacity =
        CSF_AT(std::uint32_t, self, 0x04) +
        static_cast<std::uint32_t>(additionalCapacity);
    auto* oldRecords = CSF_AT(std::uint8_t*, self, 0x00);
    CSF_AT(std::uint32_t, self, 0x04) = updatedCapacity;

    if (!oldRecords)
    {
        CSF_AT(void*, self, 0x00) = newRecords;
        return reinterpret_cast<std::uint32_t*>(newRecords);
    }

    auto* oldAllocation = oldRecords - 4;
    const std::uint32_t oldCapacity =
        CSF_AT(std::uint32_t, oldAllocation, 0x00);
    if (static_cast<std::int32_t>(oldCapacity - 1u) >= 0)
    {
        auto* recordEnd = oldRecords + 40u * oldCapacity;
        std::uint32_t remaining = oldCapacity;
        do
        {
            recordEnd -= 40;
            CSF_AT(void**, recordEnd, 0x04) = CSF_VTBL(0x0082D410);
        } while (--remaining);
    }

    const int freeResult = Mem_Free(oldAllocation);
    CSF_AT(void*, self, 0x00) = nullptr;
    CSF_AT(void*, self, 0x00) = newRecords;
    return reinterpret_cast<std::uint32_t*>(
        static_cast<std::uintptr_t>(freeResult));
}

HOOK(0x00443E00, GrowRecordArrayCapacity, "auto");
