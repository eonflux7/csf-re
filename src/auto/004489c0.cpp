#include <cstdint>
#include "globals.h"
#include <cstddef>
#include <cstring>
#include "hook.h"
#include "core/memory.h"

CSF_FUNC(void*, __cdecl, Mem_Alloc, 0x006910F0, (std::size_t size));

static std::uint32_t* __fastcall CopyRecordArray(
    std::uint32_t* self, void* /*edx*/, std::uint32_t* source)
{
    const std::uint32_t capacity = CSF_AT(std::uint32_t, source, 0x04);

    if (capacity != 0)
    {
        auto* allocation = static_cast<std::uint8_t*>(
            Mem_Alloc(36u * capacity + 4u));
        std::uint8_t* records = nullptr;

        if (allocation)
        {
            records = allocation + 4;
            CSF_AT(std::uint32_t, allocation, 0x00) = capacity;

            if (static_cast<std::int32_t>(capacity - 1u) >= 0)
            {
                auto* record = records;
                std::uint32_t remaining = capacity;
                do
                {
                    CSF_AT(void**, record, 0x00) = CSF_VTBL(0x0082D448);
                    CSF_AT(std::int32_t, record, 0x04) = -1;
                    CSF_AT(std::int32_t, record, 0x08) = -1;
                    CSF_AT(void**, record, 0x10) = CSF_VTBL(0x0082D448);
                    CSF_AT(std::int32_t, record, 0x14) = -1;
                    CSF_AT(std::int32_t, record, 0x18) = -1;
                    CSF_AT(std::uint32_t, record, 0x0C) = 0;
                    record += 36;
                } while (--remaining != 0);
            }
        }

        CSF_AT(std::uint8_t*, self, 0x00) = records;
        const std::uint32_t copyBytes =
            36u * CSF_AT(std::uint32_t, source, 0x08);
        const void* sourceRecords = CSF_AT(void*, source, 0x00);
        if (copyBytes != 0)
            std::memcpy(records, sourceRecords, copyBytes);
    }
    else
    {
        CSF_AT(std::uint32_t, self, 0x00) = 0;
    }

    CSF_AT(std::uint32_t, self, 0x08) =
        CSF_AT(std::uint32_t, source, 0x08);
    CSF_AT(std::uint32_t, self, 0x04) =
        CSF_AT(std::uint32_t, source, 0x04);
    CSF_AT(std::uint32_t, self, 0x0C) =
        CSF_AT(std::uint32_t, source, 0x0C);
    return self;
}

HOOK(0x004489C0, CopyRecordArray, "auto");
