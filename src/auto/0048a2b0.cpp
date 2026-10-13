#include <cstdint>
#include "globals.h"
#include <cstddef>
#include <cstring>
#include "hook.h"
#include "core/memory.h"

CSF_FUNC(int, __cdecl, CoreMemory_AllocUntagged, 0x006910F0, (std::size_t size));
CSF_FUNC(int, __cdecl, CoreMemory_Free, 0x00691110, (void* block));

static char* __fastcall GrowActionRecords(void* self, void* /*edx*/, int additionalCapacity)
{
    const std::uint32_t capacity =
        CSF_AT(std::uint32_t, self, 0x04) +
        static_cast<std::uint32_t>(additionalCapacity);

    char* allocation = reinterpret_cast<char*>(
        static_cast<std::uintptr_t>(
            CoreMemory_AllocUntagged(96u * capacity + 4u)));
    char* newRecords = nullptr;

    if (allocation)
    {
        newRecords = allocation + 4;
        CSF_AT(std::uint32_t, newRecords, -4) = capacity;

        if (static_cast<std::int32_t>(capacity - 1u) >= 0)
        {
            char* record = newRecords;
            std::uint32_t remaining = capacity;
            do
            {
                CSF_AT(std::uint32_t, record, 0x08) = 0;
                CSF_AT(std::uint32_t, record, 0x0C) = 0;
                CSF_AT(void**, record, 0x10) = CSF_VTBL(0x0082D448);
                CSF_AT(std::int32_t, record, 0x14) = -1;
                CSF_AT(std::int32_t, record, 0x18) = -1;
                CSF_AT(std::uint32_t, record, 0x1C) = 0;
                CSF_AT(std::uint32_t, record, 0x20) = 0;
                CSF_AT(std::uint32_t, record, 0x24) = 0;
                CSF_AT(std::uint8_t, record, 0x28) = 1;
                CSF_AT(std::uint8_t, record, 0x29) = 0;
                CSF_AT(std::uint8_t, record, 0x2A) = 1;
                CSF_AT(void**, record, 0x2C) = CSF_VTBL(0x0082D448);
                CSF_AT(void**, record, 0x3C) = CSF_VTBL(0x0082D448);
                CSF_AT(std::int32_t, record, 0x40) = -1;
                CSF_AT(std::int32_t, record, 0x44) = -1;
                CSF_AT(std::uint32_t, record, 0x48) = 0;
                CSF_AT(std::uint32_t, record, 0x4C) = 0;
                CSF_AT(std::uint32_t, record, 0x50) = 0;
                CSF_AT(std::uint32_t, record, 0x38) = 0;
                CSF_AT(std::int32_t, record, 0x54) = -1;
                CSF_AT(std::uint8_t, record, 0x58) = 1;
                CSF_AT(std::uint8_t, record, 0x59) = 0;
                CSF_AT(std::uint8_t, record, 0x5A) = 0;
                CSF_AT(std::uint8_t, record, 0x5B) = 0;
                CSF_AT(std::uint8_t, record, 0x5C) = 1;
                CSF_AT(std::uint8_t, record, 0x5D) = 0;
                CSF_AT(std::int32_t, record, 0x30) = -1;
                CSF_AT(std::int32_t, record, 0x34) = -1;
                CSF_AT(std::uint32_t, record, 0x00) = 0;
                CSF_AT(std::uint32_t, record, 0x04) = 1;

                record += 96;
            } while (--remaining);
        }
    }

    const std::uint32_t usedCount = CSF_AT(std::uint32_t, self, 0x08);
    if (usedCount > 0)
    {
        std::memcpy(newRecords, CSF_AT(char*, self, 0x00), 96u * usedCount);
    }

    CSF_AT(std::uint32_t, self, 0x04) +=
        static_cast<std::uint32_t>(additionalCapacity);

    char* oldRecords = CSF_AT(char*, self, 0x00);
    if (oldRecords)
    {
        const std::uint32_t oldCapacity =
            CSF_AT(std::uint32_t, oldRecords, -4);

        if (static_cast<std::int32_t>(oldCapacity - 1u) >= 0)
        {
            char* record = oldRecords + 96u * oldCapacity;
            std::uint32_t remaining = oldCapacity;
            do
            {
                record -= 96;
                CSF_AT(void**, record, 0x3C) = CSF_VTBL(0x0082D410);
                CSF_AT(void**, record, 0x2C) = CSF_VTBL(0x0082D410);
                CSF_AT(void**, record, 0x10) = CSF_VTBL(0x0082D410);
            } while (--remaining);
        }

        char* result = reinterpret_cast<char*>(
            static_cast<std::uintptr_t>(CoreMemory_Free(oldRecords - 4)));
        CSF_AT(char*, self, 0x00) = nullptr;
        CSF_AT(char*, self, 0x00) = newRecords;
        return result;
    }

    CSF_AT(char*, self, 0x00) = newRecords;
    return newRecords;
}

HOOK(0x0048A2B0, GrowActionRecords, "auto");
