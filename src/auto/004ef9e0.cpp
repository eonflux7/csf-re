#include <cstdint>
#include <cstddef>
#include <cstring>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_FUNC(void*, __cdecl, Mem_Alloc, 0x006910F0, (std::size_t size));
CSF_FUNC(int, __cdecl, Mem_Free, 0x00691110, (void* block));

static char* __fastcall TextDatabase_GrowEntryCapacity(
    void* self, void* /*edx*/, int additionalCapacity)
{
    const uint32_t newCapacity =
        CSF_AT(uint32_t, self, 0x4) +
        static_cast<uint32_t>(additionalCapacity);

    auto* allocation = static_cast<uint8_t*>(
        Mem_Alloc(32u * newCapacity + 4u));

    char* newEntries = nullptr;
    if (allocation)
    {
        CSF_AT(uint32_t, allocation, 0x0) = newCapacity;
        newEntries = reinterpret_cast<char*>(allocation + 4);

        if (static_cast<int32_t>(newCapacity - 1u) >= 0)
        {
            uint32_t remaining = newCapacity;
            char* record = newEntries;
            do
            {
                auto* entry = reinterpret_cast<C_0082d448*>(record + 8);
                entry->vtbl = CSF_VTBL(0x0082D448);
                entry->field_0004 = static_cast<uint32_t>(-1);
                entry->field_0008 = static_cast<uint32_t>(-1);
                CSF_AT(int32_t, record, 0x0) = -1;
                CSF_AT(int32_t, record, 0x4) = -1;
                entry->field_000c = static_cast<uint32_t>(-1);
                *reinterpret_cast<uint8_t*>(&entry->field_0010) = 0;
                entry->field_0014 = 0;
                record += 32;
            }
            while (--remaining);
        }
    }

    char* result = newEntries;
    const uint32_t usedCount = CSF_AT(uint32_t, self, 0x8);
    if (usedCount != 0)
    {
        std::memcpy(newEntries, CSF_AT(char*, self, 0x0),
                    32u * usedCount);
    }

    CSF_AT(uint32_t, self, 0x4) =
        CSF_AT(uint32_t, self, 0x4) +
        static_cast<uint32_t>(additionalCapacity);

    char* oldEntries = CSF_AT(char*, self, 0x0);
    if (oldEntries)
    {
        uint8_t* oldAllocation =
            reinterpret_cast<uint8_t*>(oldEntries) - 4;
        const uint32_t oldCapacity =
            CSF_AT(uint32_t, oldAllocation, 0x0);

        if (static_cast<int32_t>(oldCapacity - 1u) >= 0)
        {
            char* end = oldEntries + 32u * oldCapacity + 8;
            uint32_t remaining = oldCapacity;
            do
            {
                end -= 32;
                --remaining;
                reinterpret_cast<C_0082d410*>(end)->vtbl =
                    CSF_VTBL(0x0082D410);
            }
            while (remaining);
        }

        result = reinterpret_cast<char*>(
            static_cast<uintptr_t>(Mem_Free(oldAllocation)));
        CSF_AT(char*, self, 0x0) = nullptr;
        CSF_AT(char*, self, 0x0) = newEntries;
    }
    else
    {
        CSF_AT(char*, self, 0x0) = result;
    }

    return result;
}

HOOK(0x004EF9E0, TextDatabase_GrowEntryCapacity, "auto");
