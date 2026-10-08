#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"

CSF_FUNC(int, __cdecl, FreeDebugStorage, 0x00690E60, (void* block));
// 0x6C8D30 is invoked with ecx = this (mov ecx, esi before the call), so bind
// it as a __thiscall-style callee (self in ecx, two stack args, ret 8).
CSF_FUNC(void*, __fastcall, ReleaseVectorStorage, 0x006C8D30,
         (void* self, void* edx, void* block, int count));
CSF_FUNC(int, __cdecl, CopyWordsAndAdvance, 0x0071A3E0,
         (void* source, int sourceEnd, void* destination));
CSF_FUNC(int, __fastcall, ForwardToDispatchDeferred, 0x00730980,
         (void* self, void* edx, int first, int last, int destination));
CSF_FUNC(char, __fastcall, AllocateVectorStorage, 0x00730B40,
         (void* self, void* edx, int count));
CSF_FUNC(void*, __cdecl, CopyMemory, 0x007AEE60,
         (void* destination, const void* source, std::size_t bytes));

static uint32_t GetWordDistance(uint32_t end, uint32_t begin)
{
    return static_cast<uint32_t>(static_cast<int32_t>(end - begin) >> 2);
}

static void* __fastcall AssignDwordVector(void* self, void* /*edx*/,
                                          void* source)
{
    if (self == source)
        return self;

    // Source vector: [source+4] = begin, [source+8] = end.
    const uint32_t sourceBegin = CSF_AT(uint32_t, source, 0x04);
    uint32_t sourceEnd = 0;
    uint32_t sourceCount = 0;
    if (sourceBegin != 0) {
        sourceEnd = CSF_AT(uint32_t, source, 0x08);
        sourceCount = GetWordDistance(sourceEnd, sourceBegin);
    }

    if (sourceCount == 0) {
        const uint32_t storage = CSF_AT(uint32_t, self, 0x04);
        if (storage != 0)
            FreeDebugStorage(reinterpret_cast<void*>(storage));

        CSF_AT(uint32_t, self, 0x04) = 0;
        CSF_AT(uint32_t, self, 0x08) = 0;
        CSF_AT(uint32_t, self, 0x0C) = 0;
        return self;
    }

    const uint32_t selfBegin = CSF_AT(uint32_t, self, 0x04);
    const uint32_t selfCount = selfBegin != 0
        ? GetWordDistance(CSF_AT(uint32_t, self, 0x08), selfBegin)
        : 0;

    if (sourceCount <= selfCount) {
        // Enough room already: plain memcpy of the whole source range.
        const uint32_t copyBegin = CSF_AT(uint32_t, source, 0x04);
        const uint32_t copyBytes = GetWordDistance(sourceEnd, copyBegin) << 2;
        CopyMemory(reinterpret_cast<void*>(selfBegin),
                   reinterpret_cast<const void*>(copyBegin),
                   static_cast<std::size_t>(copyBytes));

        const uint32_t updatedSourceBegin = CSF_AT(uint32_t, source, 0x04);
        uint32_t newEnd;
        if (updatedSourceBegin != 0) {
            const uint32_t updatedSourceEnd =
                CSF_AT(uint32_t, source, 0x08);
            const uint32_t updatedCount =
                GetWordDistance(updatedSourceEnd, updatedSourceBegin);
            const uint32_t updatedSelfBegin = CSF_AT(uint32_t, self, 0x04);
            newEnd = updatedSelfBegin + (updatedCount << 2);
        } else {
            newEnd = CSF_AT(uint32_t, self, 0x04);
        }
        CSF_AT(uint32_t, self, 0x08) = newEnd;
        return self;
    }

    const uint32_t capacity = selfBegin != 0
        ? GetWordDistance(CSF_AT(uint32_t, self, 0x0C), selfBegin)
        : 0;

    if (sourceCount <= capacity) {
        // Have capacity: move the existing elements to the tail, then append.
        const uint32_t existingCount = selfBegin != 0
            ? GetWordDistance(CSF_AT(uint32_t, self, 0x08), selfBegin)
            : 0;
        const uint32_t splitBegin = CSF_AT(uint32_t, source, 0x04);
        const uint32_t split = splitBegin + (existingCount << 2);

        CopyWordsAndAdvance(reinterpret_cast<void*>(splitBegin),
                            static_cast<int>(split),
                            reinterpret_cast<void*>(selfBegin));

        const uint32_t selfEnd = CSF_AT(uint32_t, self, 0x08);
        const uint32_t sourceEndReloaded = CSF_AT(uint32_t, source, 0x08);
        const int newEnd = ForwardToDispatchDeferred(
            self, nullptr, static_cast<int>(split),
            static_cast<int>(sourceEndReloaded), static_cast<int>(selfEnd));
        CSF_AT(uint32_t, self, 0x08) = static_cast<uint32_t>(newEnd);
        return self;
    }

    // Capacity too small: release the old storage, allocate, then copy.
    if (selfBegin != 0) {
        const uint32_t capacityCount =
            GetWordDistance(CSF_AT(uint32_t, self, 0x0C), selfBegin);
        ReleaseVectorStorage(self, nullptr, reinterpret_cast<void*>(selfBegin),
                             static_cast<int>(capacityCount));
    }

    const uint32_t allocSourceBegin = CSF_AT(uint32_t, source, 0x04);
    const uint32_t allocCount = allocSourceBegin != 0
        ? GetWordDistance(CSF_AT(uint32_t, source, 0x08), allocSourceBegin)
        : 0;

    if (AllocateVectorStorage(self, nullptr, static_cast<int>(allocCount))) {
        const uint32_t newSelfBegin = CSF_AT(uint32_t, self, 0x04);
        const uint32_t sourceEndReloaded = CSF_AT(uint32_t, source, 0x08);
        const uint32_t sourceBeginReloaded = CSF_AT(uint32_t, source, 0x04);
        const int newEnd = ForwardToDispatchDeferred(
            self, nullptr, static_cast<int>(sourceBeginReloaded),
            static_cast<int>(sourceEndReloaded), static_cast<int>(newSelfBegin));
        CSF_AT(uint32_t, self, 0x08) = static_cast<uint32_t>(newEnd);
    }

    return self;
}

HOOK(0x00730EA0, AssignDwordVector, "auto");
