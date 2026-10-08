#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"

CSF_FUNC(int, __cdecl, FreeDebugStorage, 0x00690E60, (void* block));
CSF_FUNC(void*, __stdcall, ReleaseVectorStorage, 0x006C8D30,
         (void* block, int count));
CSF_FUNC(int, __cdecl, CopyWordsAndAdvance, 0x0071A3E0,
         (void* source, int sourceEnd, void* destination));
CSF_FUNC(int, __fastcall, CopyWordRangeAndReturnEnd, 0x00553FA0,
         (void* self, void* edx, int begin, int end, int destination));
CSF_FUNC(char, __fastcall, AllocateVectorStorage, 0x00730B40,
         (void* self, void* edx, int count));
CSF_FUNC(void*, __cdecl, CopyMemory, 0x007AEE60,
         (void* destination, const void* source, std::size_t bytes));

static uint32_t GetWordDistance(uint32_t end, uint32_t begin)
{
    return static_cast<uint32_t>(static_cast<int32_t>(end - begin) >> 2);
}

// Assigns the pointer-vector held in `source` to the vector held in `self`
// (begin/end/capacity at offsets +4/+8/+0xC).
static void* __fastcall AssignPointerVector(void* self, void* /*edx*/,
                                            void* source)
{
    if (self == source)
        return self;

    const uint32_t initialSourceBegin = CSF_AT(uint32_t, source, 0x04);
    uint32_t initialSourceEnd = 0;
    uint32_t sourceCount = 0;
    if (initialSourceBegin != 0) {
        initialSourceEnd = CSF_AT(uint32_t, source, 0x08);
        sourceCount = GetWordDistance(initialSourceEnd, initialSourceBegin);
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

    const uint32_t destinationBegin = CSF_AT(uint32_t, self, 0x04);
    const uint32_t destinationCount = destinationBegin != 0
        ? GetWordDistance(CSF_AT(uint32_t, self, 0x08), destinationBegin)
        : 0;

    if (sourceCount <= destinationCount) {
        const uint32_t copyBegin = CSF_AT(uint32_t, source, 0x04);
        const uint32_t copyBytes =
            GetWordDistance(initialSourceEnd, copyBegin) << 2;
        CopyMemory(reinterpret_cast<void*>(destinationBegin),
                   reinterpret_cast<const void*>(copyBegin), copyBytes);

        const uint32_t updatedSourceBegin = CSF_AT(uint32_t, source, 0x04);
        uint32_t updatedCount = 0;
        if (updatedSourceBegin != 0) {
            updatedCount = GetWordDistance(
                CSF_AT(uint32_t, source, 0x08), updatedSourceBegin);
        }
        const uint32_t updatedDestinationBegin = CSF_AT(uint32_t, self, 0x04);
        CSF_AT(uint32_t, self, 0x08) =
            updatedDestinationBegin + (updatedCount << 2);
        return self;
    }

    const uint32_t capacity = destinationBegin != 0
        ? GetWordDistance(CSF_AT(uint32_t, self, 0x0C), destinationBegin)
        : 0;

    if (sourceCount <= capacity) {
        const uint32_t existingCount = destinationBegin != 0
            ? GetWordDistance(CSF_AT(uint32_t, self, 0x08), destinationBegin)
            : 0;
        const uint32_t copyBegin = CSF_AT(uint32_t, source, 0x04);
        const uint32_t split = copyBegin + (existingCount << 2);

        CopyWordsAndAdvance(reinterpret_cast<void*>(copyBegin),
                            static_cast<int>(split),
                            reinterpret_cast<void*>(destinationBegin));

        const uint32_t destinationEnd = CSF_AT(uint32_t, self, 0x08);
        const uint32_t sourceEnd = CSF_AT(uint32_t, source, 0x08);
        const int newEnd = CopyWordRangeAndReturnEnd(
            self, nullptr, static_cast<int>(split),
            static_cast<int>(sourceEnd), static_cast<int>(destinationEnd));
        CSF_AT(uint32_t, self, 0x08) = static_cast<uint32_t>(newEnd);
        return self;
    }

    if (destinationBegin != 0) {
        const uint32_t releaseCount =
            GetWordDistance(CSF_AT(uint32_t, self, 0x0C), destinationBegin);
        ReleaseVectorStorage(reinterpret_cast<void*>(destinationBegin),
                             static_cast<int>(releaseCount));
    }

    const uint32_t allocationSourceBegin = CSF_AT(uint32_t, source, 0x04);
    const uint32_t allocationCount = allocationSourceBegin != 0
        ? GetWordDistance(CSF_AT(uint32_t, source, 0x08), allocationSourceBegin)
        : 0;

    if (AllocateVectorStorage(self, nullptr, static_cast<int>(allocationCount))) {
        const uint32_t newStorage = CSF_AT(uint32_t, self, 0x04);
        const uint32_t sourceEnd = CSF_AT(uint32_t, source, 0x08);
        const uint32_t sourceBegin = CSF_AT(uint32_t, source, 0x04);
        const int newEnd = CopyWordRangeAndReturnEnd(
            self, nullptr, static_cast<int>(sourceBegin),
            static_cast<int>(sourceEnd), static_cast<int>(newStorage));
        CSF_AT(uint32_t, self, 0x08) = static_cast<uint32_t>(newEnd);
    }

    return self;
}

HOOK(0x00715E70, AssignPointerVector, "auto");
