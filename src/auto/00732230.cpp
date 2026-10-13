#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

CSF_FUNC(char, __fastcall, AllocateRecordStorage_410BD0, 0x00410BD0, (void* self, void* edx, std::uint32_t count));
CSF_FUNC(int, __fastcall, S_0047ea40_DestroyPointerStorage, 0x00558790, (S_0047ea40* self, void*));
CSF_FUNC(void*, __stdcall, ReleaseVectorStorage, 0x006C8D30, (void* block, int));
CSF_FUNC(uint32_t*, __cdecl, CopyRecordsToStorage, 0x007323D0, (uint32_t* a1, uint32_t* a2, uint32_t* a3, int a4, int a5));

static int* __fastcall S_0047ea40_AssignRecords(
    S_0047ea40* self, void* /*edx*/, int sourceAddress)
{
    auto* source = reinterpret_cast<S_0047ea40*>(sourceAddress);
    if (self == source)
        return reinterpret_cast<int*>(self);

    const uint32_t sourceBeginAddress = source->field_0004;
    uint32_t sourceEndAddress = 0;
    uint32_t sourceCount = 0;
    if (sourceBeginAddress)
    {
        sourceEndAddress = source->field_0008;
        sourceCount = static_cast<int32_t>(
            sourceEndAddress - sourceBeginAddress) >> 4;
    }

    if (sourceCount == 0)
    {
        S_0047ea40_DestroyPointerStorage(self, nullptr);
        return reinterpret_cast<int*>(self);
    }

    auto* destination = reinterpret_cast<uint32_t*>(self->field_0004);
    const uint32_t destinationCount = destination
        ? static_cast<int32_t>(
              self->field_0008 - reinterpret_cast<uint32_t>(destination)) >> 4
        : 0;

    if (sourceCount > destinationCount)
    {
        const uint32_t capacity = destination
            ? static_cast<int32_t>(
                  self->field_000c - reinterpret_cast<uint32_t>(destination)) >> 4
            : 0;

        uint32_t* copiedEnd;
        if (sourceCount > capacity)
        {
            if (destination)
            {
                ReleaseVectorStorage(
                    destination,
                    static_cast<int32_t>(
                        self->field_000c -
                        reinterpret_cast<uint32_t>(destination)) >> 4);
            }

            const uint32_t currentSourceBegin = source->field_0004;
            const uint32_t count = currentSourceBegin
                ? static_cast<int32_t>(
                      source->field_0008 - currentSourceBegin) >> 4
                : 0;

            if (!AllocateRecordStorage_410BD0(self, nullptr, count))
                return reinterpret_cast<int*>(self);

            copiedEnd = CopyRecordsToStorage(
                reinterpret_cast<uint32_t*>(source->field_0004),
                reinterpret_cast<uint32_t*>(source->field_0008),
                reinterpret_cast<uint32_t*>(self->field_0004),
                reinterpret_cast<int>(self), sourceAddress);
        }
        else
        {
            const int existingCount = destination
                ? static_cast<int32_t>(
                      self->field_0008 -
                      reinterpret_cast<uint32_t>(destination)) >> 4
                : 0;

            auto* input = reinterpret_cast<uint32_t*>(source->field_0004);
            auto* prefixEnd = input + 4 * existingCount;
            uint32_t destinationAddress = self->field_0004;

            while (input != prefixEnd)
            {
                destination[0] = input[0];
                destination[1] = input[1];
                destination[2] = input[2];
                destination[3] = input[3];
                destination = reinterpret_cast<uint32_t*>(destinationAddress + 16);
                input += 4;
                destinationAddress += 16;
            }

            copiedEnd = CopyRecordsToStorage(
                prefixEnd,
                reinterpret_cast<uint32_t*>(source->field_0008),
                reinterpret_cast<uint32_t*>(self->field_0008),
                reinterpret_cast<int>(self), sourceAddress);
        }

        self->field_0008 = reinterpret_cast<uint32_t>(copiedEnd);
        return reinterpret_cast<int*>(self);
    }

    auto* input = reinterpret_cast<uint32_t*>(source->field_0004);
    if (sourceBeginAddress != sourceEndAddress)
    {
        do
        {
            destination[0] = input[0];
            destination[1] = input[1];
            destination[2] = input[2];
            destination[3] = input[3];
            input += 4;
            destination += 4;
        }
        while (reinterpret_cast<uint32_t>(input) != sourceEndAddress);
    }

    const uint32_t currentSourceBegin = source->field_0004;
    if (currentSourceBegin)
    {
        self->field_0008 = self->field_0004 + 16u *
            static_cast<uint32_t>(
                static_cast<int32_t>(
                    source->field_0008 - currentSourceBegin) >> 4);
    }
    else
    {
        self->field_0008 = self->field_0004;
    }

    return reinterpret_cast<int*>(self);
}

HOOK(0x00732230, S_0047ea40_AssignRecords, "auto");
