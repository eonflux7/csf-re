#include <cstdint>
#include <cstddef>
#include <cstring>
#include "globals.h"
#include "hook.h"

// Growable array of 20-byte records:
//   +0x00 data pointer, +0x04 capacity, +0x08 count, +0x0C grow step.

CSF_FUNC(void*, __cdecl, Mem_Alloc, 0x006910F0, (std::size_t size));
CSF_FUNC(int, __cdecl, Mem_Free, 0x00691110, (void* block));
CSF_FUNC(void*, __cdecl, _memcpy, 0x007AEE60, (void* destination, const void* source, std::size_t byteCount));

static int __fastcall DynArray20_InsertAt(void* self, void* /*edx*/, int index, const uint32_t* item)
{
    uint32_t count = CSF_AT(uint32_t, self, 0x8);
    uint32_t capacity = CSF_AT(uint32_t, self, 0x4);

    if (count < capacity)
    {
        // Enough room: shift the tail up by one record (overlapping copy, as in the original).
        uint8_t* insertPos = CSF_AT(uint8_t*, self, 0x0) + 20 * index;
        _memcpy(insertPos + 20, insertPos, 20 * (count - index));

        uint32_t* slot = reinterpret_cast<uint32_t*>(CSF_AT(uint8_t*, self, 0x0) + 20 * index);
        slot[0] = item[0];
        slot[1] = item[1];
        slot[2] = item[2];
        slot[3] = item[3];
        slot[4] = item[4];

        int newCount = CSF_AT(int, self, 0x8) + 1;
        CSF_AT(int, self, 0x8) = newCount;
        return newCount;
    }

    // Grow: allocate a bigger buffer and default-construct its records.
    uint32_t newCapacity = capacity + CSF_AT(uint32_t, self, 0xC);
    uint32_t* allocated = static_cast<uint32_t*>(Mem_Alloc(20 * newCapacity));
    uint8_t* newData;
    if (allocated != nullptr)
    {
        if (static_cast<int>(newCapacity - 1) >= 0)
        {
            uint32_t* record = allocated;
            uint32_t remaining = newCapacity;
            do
            {
                record[0] = 0;
                record[1] = 0;
                record[2] = 0;
                record += 5;
                --remaining;
            } while (remaining != 0);
        }
        newData = reinterpret_cast<uint8_t*>(allocated);
    }
    else
    {
        newData = nullptr;
    }

    // Records before the insertion point.
    std::memcpy(newData, CSF_AT(const uint8_t*, self, 0x0), static_cast<uint32_t>(20 * index));

    // The inserted record.
    uint32_t* slot = reinterpret_cast<uint32_t*>(newData + 20 * index);
    slot[0] = item[0];
    slot[1] = item[1];
    slot[2] = item[2];
    slot[3] = item[3];
    slot[4] = item[4];

    // Records after the insertion point.
    std::memcpy(reinterpret_cast<uint8_t*>(slot) + 20,
                CSF_AT(const uint8_t*, self, 0x0) + 20 * index,
                20 * (CSF_AT(uint32_t, self, 0x8) - static_cast<uint32_t>(index)));

    void* oldData = CSF_AT(void*, self, 0x0);
    CSF_AT(uint32_t, self, 0x4) = CSF_AT(uint32_t, self, 0x4) + CSF_AT(uint32_t, self, 0xC);
    if (oldData != nullptr)
    {
        Mem_Free(oldData);
        CSF_AT(void*, self, 0x0) = nullptr;
    }

    int newCount = CSF_AT(int, self, 0x8) + 1;
    CSF_AT(uint8_t*, self, 0x0) = newData;
    CSF_AT(int, self, 0x8) = newCount;
    return newCount;
}

HOOK(0x00425B90, DynArray20_InsertAt, "auto");
