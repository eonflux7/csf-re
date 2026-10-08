#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"

CSF_FUNC(std::uint32_t, __fastcall, ClearRecordStorage, 0x724420,
         (void* self, void* edx));
CSF_FUNC(char, __fastcall, AllocateRecordStorage, 0x410BD0,
         (void* self, void* edx, std::uint32_t count));
CSF_FUNC(void*, __stdcall, ReleaseRecordStorage, 0x6C8D30,
         (void* storage, std::uint32_t capacity));

// Preserve the additional context arguments pushed by the original caller.
CSF_FUNC(std::uint32_t, __cdecl, Copy16ByteElementRange, 0x723C10,
         (std::uint32_t first, std::uint32_t last, std::uint32_t destination, ...));
CSF_FUNC(std::uint32_t, __cdecl, ResetWideStringRecordRange, 0x7241E0,
         (std::uint32_t first, std::uint32_t last, ...));
CSF_FUNC(std::uint32_t, __cdecl, AssignRecordRange, 0x723FB0,
         (std::uint32_t first, std::uint32_t last, std::uint32_t destination));
CSF_FUNC(std::uint32_t, __cdecl, CopyConstructRecordRange, 0x724090,
         (std::uint32_t first, std::uint32_t last, std::uint32_t destination, ...));

CSF_FUNC(std::uint32_t, __fastcall, DestroyRecordRangeWithContext, 0x7243D0,
         (void* self, void* edx, std::uint32_t first, std::uint32_t last));
CSF_FUNC(std::uint32_t, __fastcall, ConstructRecordRangeWithContext, 0x7243B0,
         (void* self, void* edx, std::uint32_t first,
          std::uint32_t last, std::uint32_t destination));

static std::uint32_t GetRecordCount(std::uint32_t first, std::uint32_t last)
{
    return static_cast<std::uint32_t>(
        static_cast<std::int32_t>(last - first) >> 4);
}

static int __fastcall AssignWideStringRecordStorage(
    void* self, void* /*edx*/, void* source)
{
    if (self == source)
        return static_cast<int>(reinterpret_cast<std::uintptr_t>(self));

    const std::uint32_t sourceBegin = CSF_AT(std::uint32_t, source, 0x4);
    std::uint32_t sourceCount = 0;
    if (sourceBegin != 0)
        sourceCount = GetRecordCount(
            sourceBegin, CSF_AT(std::uint32_t, source, 0x8));

    if (sourceCount == 0)
    {
        ClearRecordStorage(self, nullptr);
        return static_cast<int>(reinterpret_cast<std::uintptr_t>(self));
    }

    const std::uint32_t destinationBegin = CSF_AT(std::uint32_t, self, 0x4);
    const std::uint32_t destinationCount = destinationBegin != 0
        ? GetRecordCount(destinationBegin, CSF_AT(std::uint32_t, self, 0x8))
        : 0;

    if (sourceCount <= destinationCount)
    {
        const std::uint32_t copiedEnd = Copy16ByteElementRange(
            sourceBegin, CSF_AT(std::uint32_t, source, 0x8),
            destinationBegin, source);

        ResetWideStringRecordRange(
            copiedEnd, CSF_AT(std::uint32_t, self, 0x8), self, source);

        const std::uint32_t currentSourceBegin =
            CSF_AT(std::uint32_t, source, 0x4);
        std::uint32_t newEnd;
        if (currentSourceBegin == 0)
        {
            newEnd = CSF_AT(std::uint32_t, self, 0x4);
        }
        else
        {
            const std::uint32_t currentSourceEnd =
                CSF_AT(std::uint32_t, source, 0x8);
            const std::uint32_t currentDestinationBegin =
                CSF_AT(std::uint32_t, self, 0x4);
            newEnd = currentDestinationBegin
                   + (GetRecordCount(currentSourceBegin, currentSourceEnd) << 4);
        }
        CSF_AT(std::uint32_t, self, 0x8) = newEnd;
        return static_cast<int>(reinterpret_cast<std::uintptr_t>(self));
    }

    const std::uint32_t capacity = destinationBegin != 0
        ? GetRecordCount(destinationBegin, CSF_AT(std::uint32_t, self, 0xC))
        : 0;

    if (sourceCount <= capacity)
    {
        const std::uint32_t existingCount = destinationBegin != 0
            ? GetRecordCount(destinationBegin, CSF_AT(std::uint32_t, self, 0x8))
            : 0;
        const std::uint32_t currentSourceBegin =
            CSF_AT(std::uint32_t, source, 0x4);
        const std::uint32_t split = currentSourceBegin + (existingCount << 4);

        AssignRecordRange(currentSourceBegin, split, destinationBegin);

        const std::uint32_t currentDestinationEnd =
            CSF_AT(std::uint32_t, self, 0x8);
        const std::uint32_t currentSourceEnd =
            CSF_AT(std::uint32_t, source, 0x8);
        const std::uint32_t newEnd = CopyConstructRecordRange(
            split, currentSourceEnd, currentDestinationEnd, self, source);
        CSF_AT(std::uint32_t, self, 0x8) = newEnd;
    }
    else
    {
        if (destinationBegin != 0)
        {
            DestroyRecordRangeWithContext(
                self, nullptr, destinationBegin,
                CSF_AT(std::uint32_t, self, 0x8));

            const std::uint32_t currentDestinationBegin =
                CSF_AT(std::uint32_t, self, 0x4);
            const std::uint32_t storageEnd =
                CSF_AT(std::uint32_t, self, 0xC);
            ReleaseRecordStorage(
                reinterpret_cast<void*>(currentDestinationBegin),
                GetRecordCount(currentDestinationBegin, storageEnd));
        }

        const std::uint32_t currentSourceBegin =
            CSF_AT(std::uint32_t, source, 0x4);
        const std::uint32_t requiredCount = currentSourceBegin != 0
            ? GetRecordCount(currentSourceBegin, CSF_AT(std::uint32_t, source, 0x8))
            : 0;

        if (AllocateRecordStorage(self, nullptr, requiredCount) != 0)
        {
            const std::uint32_t newDestinationBegin =
                CSF_AT(std::uint32_t, self, 0x4);
            const std::uint32_t currentSourceEnd =
                CSF_AT(std::uint32_t, source, 0x8);
            const std::uint32_t newSourceBegin =
                CSF_AT(std::uint32_t, source, 0x4);
            const std::uint32_t newEnd = ConstructRecordRangeWithContext(
                self, nullptr, newSourceBegin, currentSourceEnd,
                newDestinationBegin);
            CSF_AT(std::uint32_t, self, 0x8) = newEnd;
        }
    }

    return static_cast<int>(reinterpret_cast<std::uintptr_t>(self));
}

HOOK(0x7248C0, AssignWideStringRecordStorage, "auto");
