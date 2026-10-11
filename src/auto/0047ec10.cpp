#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_FUNC(int, __cdecl, CopyConstructTwelveByteElements, 0x0044AD70, (int a1, int a2, int a3, int a4, int a5));
CSF_FUNC(int, __fastcall, DestroyPointerStorage, 0x00558790, (int* self, void* edx));
CSF_FUNC(void*, __stdcall, ReleaseVectorStorage, 0x006C8D30, (void* block, int count));
CSF_FUNC(char, __fastcall, AllocateTwelveByteVectorStorage, 0x00715920, (void* self, void* edx, int count));

static int* __fastcall S_0047ea40_AssignTwelveByteVector(
    S_0047ea40* self, void* /*edx*/, int sourceAddress)
{
    auto* source = reinterpret_cast<S_0047ea40*>(sourceAddress);
    if (self == source)
        return reinterpret_cast<int*>(self);

    const uint32_t sourceBegin = source->field_0004;
    uint32_t sourceEnd = 0;
    uint32_t sourceCount = 0;
    if (sourceBegin)
    {
        sourceEnd = source->field_0008;
        sourceCount = static_cast<int32_t>(sourceEnd - sourceBegin) / 12;
    }

    if (!sourceCount)
    {
        DestroyPointerStorage(reinterpret_cast<int*>(self), nullptr);
        return reinterpret_cast<int*>(self);
    }

    uint32_t destination = self->field_0004;
    const uint32_t destinationCount = destination
        ? static_cast<int32_t>(self->field_0008 - destination) / 12
        : 0;

    if (sourceCount > destinationCount)
    {
        const uint32_t capacity = destination
            ? static_cast<int32_t>(self->field_000c - destination) / 12
            : 0;

        uint32_t newEnd;
        if (sourceCount > capacity)
        {
            if (destination)
            {
                ReleaseVectorStorage(
                    reinterpret_cast<void*>(destination),
                    static_cast<int32_t>(self->field_000c - destination) / 12);
            }

            int count = static_cast<int>(source->field_0004);
            if (count)
                count = static_cast<int32_t>(source->field_0008 - count) / 12;

            if (!AllocateTwelveByteVectorStorage(self, nullptr, count))
                return reinterpret_cast<int*>(self);

            newEnd = CopyConstructTwelveByteElements(
                static_cast<int>(source->field_0004),
                static_cast<int>(source->field_0008),
                static_cast<int>(self->field_0004),
                reinterpret_cast<int>(self),
                sourceAddress);
        }
        else
        {
            const int existingCount = destination
                ? static_cast<int32_t>(self->field_0008 - destination) / 12
                : 0;
            const uint32_t split = source->field_0004 + 12 * existingCount;
            uint32_t input = source->field_0004;
            uint32_t outputPosition = self->field_0004;

            while (input != split)
            {
                CSF_AT(uint32_t, reinterpret_cast<void*>(destination), 0x00) =
                    CSF_AT(uint32_t, reinterpret_cast<void*>(input), 0x00);
                CSF_AT(uint32_t, reinterpret_cast<void*>(destination), 0x04) =
                    CSF_AT(uint32_t, reinterpret_cast<void*>(input), 0x04);
                CSF_AT(uint32_t, reinterpret_cast<void*>(destination), 0x08) =
                    CSF_AT(uint32_t, reinterpret_cast<void*>(input), 0x08);
                destination = outputPosition + 12;
                input += 12;
                outputPosition += 12;
            }

            newEnd = CopyConstructTwelveByteElements(
                static_cast<int>(split),
                static_cast<int>(source->field_0008),
                static_cast<int>(self->field_0008),
                reinterpret_cast<int>(self),
                sourceAddress);
        }

        self->field_0008 = newEnd;
        return reinterpret_cast<int*>(self);
    }

    uint32_t input = source->field_0004;
    uint32_t outputPosition = self->field_0004;
    while (input != sourceEnd)
    {
        CSF_AT(uint32_t, reinterpret_cast<void*>(destination), 0x00) =
            CSF_AT(uint32_t, reinterpret_cast<void*>(input), 0x00);
        CSF_AT(uint32_t, reinterpret_cast<void*>(destination), 0x04) =
            CSF_AT(uint32_t, reinterpret_cast<void*>(input), 0x04);
        CSF_AT(uint32_t, reinterpret_cast<void*>(destination), 0x08) =
            CSF_AT(uint32_t, reinterpret_cast<void*>(input), 0x08);
        destination = outputPosition + 12;
        input += 12;
        outputPosition += 12;
    }

    const uint32_t currentSourceBegin = source->field_0004;
    uint32_t newEnd;
    if (currentSourceBegin)
    {
        newEnd = self->field_0004 +
            12 * (static_cast<int32_t>(source->field_0008 - currentSourceBegin) / 12);
    }
    else
    {
        newEnd = self->field_0004;
    }
    self->field_0008 = newEnd;
    return reinterpret_cast<int*>(self);
}

HOOK(0x0047EC10, S_0047ea40_AssignTwelveByteVector, "auto");
