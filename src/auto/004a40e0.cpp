#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_FUNC(int, __fastcall, GetTwelveByteVectorSize, 0x004A31E0, (void* self, void* edx));
CSF_FUNC(uint32_t*, __cdecl, CopyConstructTwelveByteTail, 0x004A3230,
         (uint32_t* a1, uint32_t* a2, uint32_t* a3, int a4, int a5));
CSF_FUNC(uint32_t*, __fastcall, S_004a31e0_CopyConstructTwelveByteRange, 0x004A3530,
         (void* self, void* edx, uint32_t* a1, uint32_t* a2, uint32_t* a3));
CSF_FUNC(std::uint32_t*, __cdecl, CopyAssignTwelveByteRange, 0x004EF1F0,
         (std::uint32_t* first, std::uint32_t* last, std::uint32_t* destination));
CSF_FUNC(std::uint32_t*, __cdecl, CopyAssignTwelveBytePrefix, 0x004EF420,
         (std::uint32_t* first, std::uint32_t* last, std::uint32_t* destination));
CSF_FUNC(int, __cdecl, Mem_FreeDebug, 0x00690E60, (void* block));
CSF_FUNC(void*, __stdcall, ReleaseVectorStorage, 0x006C8D30, (void* block, int count));
CSF_FUNC(char, __fastcall, AllocateTwelveByteVectorStorage, 0x00715920,
         (void* self, void* edx, int count));

// The packet's vtable annotations at 0x004A31E0 identify direct
// GetTwelveByteVectorSize calls, not vtable accesses.

static std::uint32_t CountTwelveByteElements(
    std::uint32_t end, std::uint32_t begin)
{
    return static_cast<std::uint32_t>(
        static_cast<std::int32_t>(end - begin) / 12);
}

static int __fastcall S_004a31e0_AssignTwelveByteVector(
    S_004a31e0* self, void* /*edx*/, S_004a31e0* source)
{
    if (self == source)
        return reinterpret_cast<int>(self);

    const std::uint32_t sourceBegin = source->field_0004;
    const std::uint32_t sourceCount = sourceBegin != 0
        ? CountTwelveByteElements(source->field_0008, sourceBegin)
        : 0;

    if (sourceCount == 0)
    {
        if (self->field_0004 != 0)
            Mem_FreeDebug(reinterpret_cast<void*>(self->field_0004));

        self->field_0004 = 0;
        self->field_0008 = 0;
        self->field_000c = 0;
        return reinterpret_cast<int>(self);
    }

    const std::uint32_t destinationBegin = self->field_0004;
    const std::uint32_t destinationCount = destinationBegin != 0
        ? CountTwelveByteElements(self->field_0008, destinationBegin)
        : 0;

    if (sourceCount <= destinationCount)
    {
        CopyAssignTwelveByteRange(
            reinterpret_cast<std::uint32_t*>(sourceBegin),
            reinterpret_cast<std::uint32_t*>(source->field_0008),
            reinterpret_cast<std::uint32_t*>(destinationBegin));

        const std::uint32_t currentSourceBegin = source->field_0004;
        const std::uint32_t currentSourceCount = currentSourceBegin != 0
            ? CountTwelveByteElements(source->field_0008, currentSourceBegin)
            : 0;
        self->field_0008 = self->field_0004 + 12u * currentSourceCount;
        return reinterpret_cast<int>(self);
    }

    const std::uint32_t capacity = destinationBegin != 0
        ? CountTwelveByteElements(self->field_000c, destinationBegin)
        : 0;

    if (sourceCount <= capacity)
    {
        const int existingCount = GetTwelveByteVectorSize(self, nullptr);
        const std::uint32_t split =
            sourceBegin + 12u * static_cast<std::uint32_t>(existingCount);

        CopyAssignTwelveBytePrefix(
            reinterpret_cast<std::uint32_t*>(source->field_0004),
            reinterpret_cast<std::uint32_t*>(split),
            reinterpret_cast<std::uint32_t*>(destinationBegin));

        std::uint32_t* constructedEnd = CopyConstructTwelveByteTail(
            reinterpret_cast<std::uint32_t*>(split),
            reinterpret_cast<std::uint32_t*>(source->field_0008),
            reinterpret_cast<std::uint32_t*>(self->field_0008),
            reinterpret_cast<int>(self),
            reinterpret_cast<int>(source));
        self->field_0008 = reinterpret_cast<std::uint32_t>(constructedEnd);
        return reinterpret_cast<int>(self);
    }

    if (destinationBegin != 0)
    {
        const std::uint32_t currentCapacity =
            CountTwelveByteElements(self->field_000c, destinationBegin);
        ReleaseVectorStorage(
            reinterpret_cast<void*>(destinationBegin),
            static_cast<std::int32_t>(currentCapacity));
    }

    const int requiredCount = GetTwelveByteVectorSize(source, nullptr);
    if (AllocateTwelveByteVectorStorage(self, nullptr, requiredCount) != 0)
    {
        const std::uint32_t newDestinationBegin = self->field_0004;
        const std::uint32_t sourceEnd = source->field_0008;
        const std::uint32_t currentSourceBegin = source->field_0004;
        std::uint32_t* constructedEnd =
            S_004a31e0_CopyConstructTwelveByteRange(
                self, nullptr,
                reinterpret_cast<std::uint32_t*>(currentSourceBegin),
                reinterpret_cast<std::uint32_t*>(sourceEnd),
                reinterpret_cast<std::uint32_t*>(newDestinationBegin));
        self->field_0008 = reinterpret_cast<std::uint32_t>(constructedEnd);
    }

    return reinterpret_cast<int>(self);
}

HOOK(0x004A40E0, S_004a31e0_AssignTwelveByteVector, "auto");
