#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"

CSF_FUNC(int, __fastcall, GetTwelveByteVectorSize, 0x004A31E0,
         (void* self, void* edx));
CSF_FUNC(std::uint32_t*, __cdecl, CopyAssignTwelveByteRange, 0x004EF1F0,
         (std::uint32_t* first, std::uint32_t* last, std::uint32_t* destination));
CSF_FUNC(std::uint32_t*, __cdecl, CopyAssignTwelveBytePrefix, 0x004EF420,
         (std::uint32_t* first, std::uint32_t* last, std::uint32_t* destination));
CSF_FUNC(int, __cdecl, Mem_FreeDebug, 0x00690E60, (void* block));
CSF_FUNC(void*, __stdcall, ReleaseVectorStorage, 0x006C8D30,
         (void* block, int capacity));
CSF_FUNC(std::uint32_t, __cdecl, CopyConstructTwelveByteRange, 0x00714410,
         (std::uint32_t first, std::uint32_t last,
          std::uint32_t destination, void* constructionContext));
CSF_FUNC(std::uint32_t*, __fastcall, ForwardTo714410, 0x00714A40,
         (void* self, void* edx, std::uint32_t* first, std::uint32_t* last, std::uint32_t* destination));
CSF_FUNC(char, __fastcall, AllocateTwelveByteVectorStorage, 0x00715920,
         (void* self, void* edx, int count));

static std::uint32_t CountTwelveByteElements(
    std::uint32_t end, std::uint32_t begin)
{
    // The subtraction wraps at 32 bits, but the division is signed.
    return static_cast<std::uint32_t>(
        static_cast<std::int32_t>(end - begin) / 12);
}

static void* __fastcall AssignTwelveByteVector(
    void* self, void* /*edx*/, void* source)
{
    if (self == source)
        return self;

    const std::uint32_t sourceBegin = CSF_AT(std::uint32_t, source, 0x04);
    std::uint32_t sourceCount = 0;
    if (sourceBegin != 0)
    {
        sourceCount = CountTwelveByteElements(
            CSF_AT(std::uint32_t, source, 0x08), sourceBegin);
    }

    if (sourceCount == 0)
    {
        const std::uint32_t storage = CSF_AT(std::uint32_t, self, 0x04);
        if (storage != 0)
            Mem_FreeDebug(reinterpret_cast<void*>(storage));

        CSF_AT(std::uint32_t, self, 0x04) = 0;
        CSF_AT(std::uint32_t, self, 0x08) = 0;
        CSF_AT(std::uint32_t, self, 0x0C) = 0;
        return self;
    }

    const std::uint32_t destinationBegin = CSF_AT(std::uint32_t, self, 0x04);
    const std::uint32_t destinationCount = destinationBegin != 0
        ? CountTwelveByteElements(
              CSF_AT(std::uint32_t, self, 0x08), destinationBegin)
        : 0;

    if (sourceCount <= destinationCount)
    {
        const std::uint32_t sourceEnd = CSF_AT(std::uint32_t, source, 0x08);
        CopyAssignTwelveByteRange(
            reinterpret_cast<std::uint32_t*>(sourceBegin),
            reinterpret_cast<std::uint32_t*>(sourceEnd),
            reinterpret_cast<std::uint32_t*>(destinationBegin));

        const std::uint32_t currentSourceBegin =
            CSF_AT(std::uint32_t, source, 0x04);
        const std::uint32_t currentSourceCount = currentSourceBegin != 0
            ? CountTwelveByteElements(
                  CSF_AT(std::uint32_t, source, 0x08), currentSourceBegin)
            : 0;
        CSF_AT(std::uint32_t, self, 0x08) =
            CSF_AT(std::uint32_t, self, 0x04) + 12u * currentSourceCount;
        return self;
    }

    const std::uint32_t capacity = destinationBegin != 0
        ? CountTwelveByteElements(
              CSF_AT(std::uint32_t, self, 0x0C), destinationBegin)
        : 0;

    if (sourceCount <= capacity)
    {
        const int existingCount = GetTwelveByteVectorSize(self, nullptr);
        const std::uint32_t currentSourceBegin =
            CSF_AT(std::uint32_t, source, 0x04);
        const std::uint32_t split =
            sourceBegin + 12u * static_cast<std::uint32_t>(existingCount);

        CopyAssignTwelveBytePrefix(
            reinterpret_cast<std::uint32_t*>(currentSourceBegin),
            reinterpret_cast<std::uint32_t*>(split),
            reinterpret_cast<std::uint32_t*>(destinationBegin));

        const std::uint32_t sourceEnd = CSF_AT(std::uint32_t, source, 0x08);
        const std::uint32_t destinationEnd = CSF_AT(std::uint32_t, self, 0x08);
        const std::uint32_t constructedEnd = CopyConstructTwelveByteRange(
            split, sourceEnd, destinationEnd, self);
        CSF_AT(std::uint32_t, self, 0x08) = constructedEnd;
        return self;
    }

    if (destinationBegin != 0)
    {
        const std::uint32_t currentCapacity = CountTwelveByteElements(
            CSF_AT(std::uint32_t, self, 0x0C), destinationBegin);
        ReleaseVectorStorage(
            reinterpret_cast<void*>(destinationBegin),
            static_cast<std::int32_t>(currentCapacity));
    }

    const int requiredCount = GetTwelveByteVectorSize(source, nullptr);
    if (AllocateTwelveByteVectorStorage(self, nullptr, requiredCount) != 0)
    {
        const std::uint32_t newDestinationBegin =
            CSF_AT(std::uint32_t, self, 0x04);
        const std::uint32_t sourceEnd = CSF_AT(std::uint32_t, source, 0x08);
        const std::uint32_t currentSourceBegin =
            CSF_AT(std::uint32_t, source, 0x04);
        std::uint32_t* constructedEnd = ForwardTo714410(self, nullptr,  // ecx = this (mov ecx, esi)
            reinterpret_cast<std::uint32_t*>(currentSourceBegin),
            reinterpret_cast<std::uint32_t*>(sourceEnd),
            reinterpret_cast<std::uint32_t*>(newDestinationBegin));
        CSF_AT(std::uint32_t, self, 0x08) =
            reinterpret_cast<std::uint32_t>(constructedEnd);
    }

    return self;
}

HOOK(0x00715CD0, AssignTwelveByteVector, "auto");
