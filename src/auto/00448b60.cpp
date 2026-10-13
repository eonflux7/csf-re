#include <cstdint>
#include <cstddef>
#include <cstring>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_FUNC(int, __cdecl, CoreMemory_AllocUntagged, 0x006910F0, (std::size_t size));
CSF_FUNC(int, __cdecl, CoreMemory_Free, 0x00691110, (void* block));

static std::uint32_t* __fastcall CopyRecordArray(
    std::uint32_t* self, void* /*edx*/, const std::uint32_t* source)
{
    if (CSF_AT(std::uint32_t, source, 0x08) >
        CSF_AT(std::uint32_t, self, 0x04))
    {
        const std::uint32_t capacity = CSF_AT(std::uint32_t, source, 0x04);
        auto* allocation = reinterpret_cast<std::uint8_t*>(
            CoreMemory_AllocUntagged(36u * capacity + 4u));
        std::uint8_t* newRecords = nullptr;

        if (allocation)
        {
            newRecords = allocation + 4;
            CSF_AT(std::uint32_t, allocation, 0) = capacity;

            if (static_cast<std::int32_t>(capacity - 1u) >= 0)
            {
                auto* record = newRecords;
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
                    record += 0x24;
                } while (--remaining);
            }
        }

        CSF_AT(std::uint32_t, self, 0x04) =
            CSF_AT(std::uint32_t, source, 0x04);

        auto* oldRecords = CSF_AT(std::uint8_t*, self, 0x00);
        if (oldRecords)
        {
            auto* oldAllocation = oldRecords - 4;
            const std::uint32_t oldCapacity =
                CSF_AT(std::uint32_t, oldAllocation, 0);
            auto* record = oldRecords + 36u * oldCapacity;

            if (static_cast<std::int32_t>(oldCapacity - 1u) >= 0)
            {
                std::uint32_t remaining = oldCapacity;
                do
                {
                    record -= 0x24;
                    CSF_AT(void**, record, 0x10) = CSF_VTBL(0x0082D410);
                    CSF_AT(void**, record, 0x00) = CSF_VTBL(0x0082D410);
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
        const auto* sourceRecords = CSF_AT(const std::uint8_t*, source, 0x00);
        auto* destinationRecords = CSF_AT(std::uint8_t*, self, 0x00);
        const std::uint32_t bytes = 36u * count;

        // Preserve the original forward, dword-at-a-time copy.
        for (std::uint32_t words = bytes >> 2; words; --words)
        {
            CSF_AT(std::uint32_t, destinationRecords, 0) =
                CSF_AT(std::uint32_t, sourceRecords, 0);
            sourceRecords += 4;
            destinationRecords += 4;
        }
    }

    CSF_AT(std::uint32_t, self, 0x08) = CSF_AT(std::uint32_t, source, 0x08);
    CSF_AT(std::uint32_t, self, 0x0C) = CSF_AT(std::uint32_t, source, 0x0C);
    return self;
}

HOOK(0x00448B60, CopyRecordArray, "auto");
