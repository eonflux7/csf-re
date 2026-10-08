#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"

CSF_FUNC(std::uint32_t, __fastcall, ClearRecordStorage, 0x6B59F0,
         (void* self, void* edx));
CSF_FUNC(char, __fastcall, AllocateRecordStorage, 0x410BD0,
         (void* self, void* edx, std::uint32_t count));
CSF_FUNC(std::uint32_t, __fastcall, DestroyRecordRange, 0x50FBB0,
         (void* self, void* edx, std::uint32_t first, std::uint32_t last));
CSF_FUNC(void*, __fastcall, ReleaseRecordStorage, 0x6C8D30,
         (void* self, void* edx, void* storage, std::uint32_t capacity));

// These helpers are pushed extra context arguments by the original callers.
CSF_FUNC(std::uint32_t, __cdecl, ResetRecordRange, 0x6DEC40,
         (std::uint32_t first, std::uint32_t last, ...));
CSF_FUNC(std::uint32_t, __cdecl, Copy16ByteElementRange, 0x7165B0,
         (std::uint32_t first, std::uint32_t last,
          std::uint32_t destination, ...));
CSF_FUNC(std::uint32_t, __cdecl, CopyConstructRecordRange, 0x716080,
         (std::uint32_t first, std::uint32_t last,
          std::uint32_t destination, ...));
CSF_FUNC(std::uint32_t, __cdecl, ForwardTo7165B0, 0x716B20,
         (std::uint32_t first, std::uint32_t last,
          std::uint32_t destination));
CSF_FUNC(std::uint32_t, __fastcall, ForwardTo716080, 0x716B40,
         (void* self, void* edx, std::uint32_t first, std::uint32_t last,
          std::uint32_t destination));

static std::uint32_t GetRecordCount(std::uint32_t first, std::uint32_t last)
{
    return static_cast<std::uint32_t>(
        static_cast<std::int32_t>(last - first) >> 4);
}

static int __fastcall AssignRecordStorage(
    void* self, void* /*edx*/, void* source)
{
    if (self == source)
        return static_cast<int>(reinterpret_cast<std::uintptr_t>(self));

    const std::uint32_t sourceBegin = CSF_AT(std::uint32_t, source, 0x4);
    std::uint32_t sourceCount = 0;
    if (sourceBegin != 0)
        sourceCount = GetRecordCount(sourceBegin,
                                     CSF_AT(std::uint32_t, source, 0x8));

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

        ResetRecordRange(copiedEnd, CSF_AT(std::uint32_t, self, 0x8),
                         self, source);

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
            ? GetRecordCount(destinationBegin,
                             CSF_AT(std::uint32_t, self, 0x8))
            : 0;
        const std::uint32_t currentSourceBegin =
            CSF_AT(std::uint32_t, source, 0x4);
        const std::uint32_t split =
            currentSourceBegin + (existingCount << 4);

        ForwardTo7165B0(currentSourceBegin, split, destinationBegin);

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
            DestroyRecordRange(self, nullptr, destinationBegin,
                               CSF_AT(std::uint32_t, self, 0x8));

            const std::uint32_t currentDestinationBegin =
                CSF_AT(std::uint32_t, self, 0x4);
            const std::uint32_t storageEnd =
                CSF_AT(std::uint32_t, self, 0xC);
            ReleaseRecordStorage(
                self, nullptr,
                reinterpret_cast<void*>(currentDestinationBegin),
                GetRecordCount(currentDestinationBegin, storageEnd));
        }

        const std::uint32_t currentSourceBegin =
            CSF_AT(std::uint32_t, source, 0x4);
        const std::uint32_t requiredCount = currentSourceBegin != 0
            ? GetRecordCount(currentSourceBegin,
                             CSF_AT(std::uint32_t, source, 0x8))
            : 0;

        if (AllocateRecordStorage(self, nullptr, requiredCount) != 0)
        {
            const std::uint32_t newDestinationBegin =
                CSF_AT(std::uint32_t, self, 0x4);
            const std::uint32_t currentSourceEnd =
                CSF_AT(std::uint32_t, source, 0x8);
            const std::uint32_t newSourceBegin =
                CSF_AT(std::uint32_t, source, 0x4);
            const std::uint32_t newEnd = ForwardTo716080(
                self, nullptr, newSourceBegin, currentSourceEnd,
                newDestinationBegin);
            CSF_AT(std::uint32_t, self, 0x8) = newEnd;
        }
    }

    return static_cast<int>(reinterpret_cast<std::uintptr_t>(self));
}

HOOK(0x7172A0, AssignRecordStorage, "auto");
