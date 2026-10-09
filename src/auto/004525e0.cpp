#include <cstdint>
#include "globals.h"
#include <cstddef>
#include <cstring>
#include "hook.h"
#include "core/memory.h"

CSF_FUNC(void*, __cdecl, Mem_Alloc, 0x006910F0, (std::size_t size));
CSF_FUNC(int, __cdecl, Mem_Free, 0x00691110, (void* block));

static void* __fastcall AssignRecordArray(void* self, void* /*edx*/, const void* source)
{
    if (CSF_AT(std::uint32_t, source, 0x08) >
        CSF_AT(std::uint32_t, self, 0x04))
    {
        const std::uint32_t capacity = CSF_AT(std::uint32_t, source, 0x04);
        void* allocation = Mem_Alloc(12u * capacity + 4u);
        void* newRecords = nullptr;

        if (allocation)
        {
            CSF_AT(std::uint32_t, allocation, 0x00) = capacity;
            newRecords = static_cast<std::uint8_t*>(allocation) + 4;

            if (static_cast<std::int32_t>(capacity - 1u) >= 0)
            {
                auto* record = static_cast<std::uint8_t*>(newRecords);
                std::uint32_t remaining = capacity;
                do
                {
                    CSF_AT(void**, record, 0x00) = CSF_VTBL(0x0082D448);
                    CSF_AT(std::int32_t, record, 0x04) = -1;
                    CSF_AT(std::int32_t, record, 0x08) = -1;
                    record += 12;
                } while (--remaining);
            }
        }

        void* oldRecords = CSF_AT(void*, self, 0x00);
        CSF_AT(std::uint32_t, self, 0x04) =
            CSF_AT(std::uint32_t, source, 0x04);

        if (oldRecords)
        {
            if (CSF_AT(std::uint32_t, oldRecords, -4))
            {
                ((void(__fastcall*)(void*, void*, int))
                    CSF_AT(void**, oldRecords, 0x00)[0])
                    (oldRecords, nullptr, 3);
            }
            else
            {
                Mem_Free(static_cast<std::uint8_t*>(oldRecords) - 4);
            }
            CSF_AT(void*, self, 0x00) = nullptr;
        }
        CSF_AT(void*, self, 0x00) = newRecords;
    }

    const std::uint32_t count = CSF_AT(std::uint32_t, source, 0x08);
    if (count)
    {
        std::memcpy(CSF_AT(void*, self, 0x00),
                    CSF_AT(void*, source, 0x00),
                    12u * count);
    }

    CSF_AT(std::uint32_t, self, 0x08) =
        CSF_AT(std::uint32_t, source, 0x08);
    CSF_AT(std::uint32_t, self, 0x0C) =
        CSF_AT(std::uint32_t, source, 0x0C);
    return self;
}

HOOK(0x004525E0, AssignRecordArray, "auto");
