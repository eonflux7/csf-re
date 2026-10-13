#include <cstdint>
#include <cstddef>
#include <cstring>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"

CSF_FUNC(int, __cdecl, CoreMemory_AllocUntagged, 0x006910F0, (std::size_t size));
CSF_FUNC(int, __cdecl, CoreMemory_Free, 0x00691110, (void* block));

static int __fastcall RecordArray_Assign(void* self, void* /*edx*/, void* source)
{
    if (CSF_AT(std::uint32_t, source, 0x08) >
        CSF_AT(std::uint32_t, self, 0x04))
    {
        const std::uint32_t capacity = CSF_AT(std::uint32_t, source, 0x04);
        auto* allocation = reinterpret_cast<std::uint8_t*>(
            CoreMemory_AllocUntagged(20 * capacity + 4));
        std::uint8_t* newRecords = nullptr;

        if (allocation)
        {
            CSF_AT(std::uint32_t, allocation, 0x00) = capacity;
            newRecords = allocation + 4;

            if (static_cast<std::int32_t>(capacity - 1) >= 0)
            {
                auto* record = newRecords;
                std::uint32_t remaining = capacity;
                do
                {
                    CSF_AT(void**, record, 0x04) = CSF_VTBL(0x0082D448);
                    CSF_AT(std::int32_t, record, 0x08) = -1;
                    CSF_AT(std::int32_t, record, 0x0C) = -1;
                    record += 20;
                } while (--remaining);
            }
        }

        auto* oldRecords = CSF_AT(std::uint8_t*, self, 0x00);
        CSF_AT(std::uint32_t, self, 0x04) =
            CSF_AT(std::uint32_t, source, 0x04);

        if (oldRecords)
        {
            auto* oldAllocation = oldRecords - 4;
            const std::uint32_t count =
                CSF_AT(std::uint32_t, oldAllocation, 0x00);
            auto* recordEnd = oldRecords + 20 * count;

            if (static_cast<std::int32_t>(count - 1) >= 0)
            {
                std::uint32_t remaining = count;
                do
                {
                    recordEnd -= 20;
                    CSF_AT(void**, recordEnd, 0x04) = CSF_VTBL(0x0082D410);
                } while (--remaining);
            }

            CoreMemory_Free(oldAllocation);
            CSF_AT(std::uint8_t*, self, 0x00) = nullptr;
        }

        CSF_AT(std::uint8_t*, self, 0x00) = newRecords;
    }

    const std::uint32_t count = CSF_AT(std::uint32_t, source, 0x08);
    if (count)
    {
        const void* sourceRecords = CSF_AT(void*, source, 0x00);
        void* destinationRecords = CSF_AT(void*, self, 0x00);
        std::memcpy(destinationRecords, sourceRecords, 20 * count);
    }

    CSF_AT(std::uint32_t, self, 0x08) =
        CSF_AT(std::uint32_t, source, 0x08);
    CSF_AT(std::uint32_t, self, 0x0C) =
        CSF_AT(std::uint32_t, source, 0x0C);

    return static_cast<int>(reinterpret_cast<std::intptr_t>(self));
}

HOOK(0x004016B0, RecordArray_Assign, "auto");
