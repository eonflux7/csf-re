#include <cstdint>
#include <cstddef>
#include <cstring>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_FUNC(void*, __cdecl, Mem_Alloc, 0x006910F0, (std::size_t size));

static std::uint32_t* __fastcall CopyConstructRecordArray(
    std::uint32_t* self, void* /*edx*/, const std::uint32_t* source)
{
    const std::uint32_t capacity = CSF_AT(std::uint32_t, source, 0x4);

    if (capacity != 0)
    {
        void* allocation = Mem_Alloc(12 * capacity + 4);
        std::uint8_t* records = nullptr;

        if (allocation)
        {
            records = static_cast<std::uint8_t*>(allocation) + 4;
            CSF_AT(std::uint32_t, allocation, 0x0) = capacity;

            if (static_cast<std::int32_t>(capacity - 1) >= 0)
            {
                std::uint8_t* record = records;
                std::uint32_t remaining = capacity;
                do
                {
                    CSF_AT(void**, record, 0x0) = CSF_VTBL(0x0082D448);
                    CSF_AT(std::int32_t, record, 0x4) = -1;
                    CSF_AT(std::int32_t, record, 0x8) = -1;
                    record += 12;
                } while (--remaining);
            }
        }

        CSF_AT(void*, self, 0x0) = records;
        const std::uint32_t copyBytes =
            12 * CSF_AT(std::uint32_t, source, 0x8);
        const void* sourceRecords = CSF_AT(void*, source, 0x0);
        std::memcpy(records, sourceRecords, copyBytes);
    }
    else
    {
        CSF_AT(std::uint32_t, self, 0x0) = 0;
    }

    CSF_AT(std::uint32_t, self, 0x8) = CSF_AT(std::uint32_t, source, 0x8);
    CSF_AT(std::uint32_t, self, 0x4) = CSF_AT(std::uint32_t, source, 0x4);
    CSF_AT(std::uint32_t, self, 0xC) = CSF_AT(std::uint32_t, source, 0xC);
    return self;
}

HOOK(0x00453260, CopyConstructRecordArray, "auto");
