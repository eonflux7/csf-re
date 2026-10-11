#include <cstdint>
#include <cstddef>
#include <cstring>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_FUNC(void*, __cdecl, Mem_Alloc, 0x006910F0, (std::size_t size));
CSF_FUNC(int, __cdecl, Mem_Free, 0x00691110, (void* block));

static unsigned int __fastcall S_00413570_GrowEventRecords(
    S_00413570* self, void* /*edx*/, int additionalCapacity)
{
    const std::uint32_t capacity = self->field_0004 + additionalCapacity;
    auto* allocation =
        static_cast<std::uint8_t*>(Mem_Alloc(12 * capacity + 4));
    std::uint8_t* records = nullptr;

    if (allocation)
    {
        CSF_AT(std::uint32_t, allocation, 0) = capacity;
        records = allocation + 4;

        if (static_cast<std::int32_t>(capacity - 1) >= 0)
        {
            auto* record = records;
            std::uint32_t remaining = capacity;
            do
            {
                CSF_AT(void**, record, 0) = CSF_VTBL(0x0082D448);
                CSF_AT(std::int32_t, record, 4) = -1;
                CSF_AT(std::int32_t, record, 8) = -1;
                record += 12;
                --remaining;
            } while (remaining);
        }
    }

    unsigned int result = self->field_0008;
    if (result)
    {
        result *= 12;
        std::memcpy(
            records, reinterpret_cast<const void*>(self->field_0000), result);
    }

    const std::uint32_t updatedCapacity =
        self->field_0004 + additionalCapacity;
    auto* oldRecords = reinterpret_cast<std::uint8_t*>(self->field_0000);
    self->field_0004 = updatedCapacity;

    if (oldRecords)
    {
        if (CSF_AT(std::uint32_t, oldRecords - 4, 0))
        {
            reinterpret_cast<void(__fastcall*)(void*, void*, int)>(
                CSF_AT(void**, oldRecords, 0)[0])(oldRecords, nullptr, 3);
        }
        else
        {
            Mem_Free(oldRecords - 4);
        }

        self->field_0000 = 0;
        self->field_0000 = reinterpret_cast<std::uintptr_t>(records);
        return reinterpret_cast<std::uintptr_t>(records);
    }

    self->field_0000 = reinterpret_cast<std::uintptr_t>(records);
    return result;
}

HOOK(0x00413570, S_00413570_GrowEventRecords, "auto");
