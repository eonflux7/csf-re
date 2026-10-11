#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_FUNC(uint32_t*, __cdecl, CopyPointerRange, 0x0047EBA0, (uint32_t* a1, uint32_t* a2, uint32_t* a3, int a4, int a5));
CSF_FUNC(int, __fastcall, DestroyPointerStorage, 0x00558790, (int* self, void* edx));
CSF_FUNC(void*, __stdcall, ReleaseVectorStorage, 0x006C8D30, (void* block, int count));
CSF_FUNC(char, __fastcall, AllocateVectorStorage, 0x00730B40, (void* self, void* edx, int count));
CSF_FUNC(void*, __cdecl, _memcpy, 0x007AEE60, (void* destination, const void* source, std::size_t byteCount));

static int* __fastcall S_0047ea40_AssignPointerStorage(
    S_0047ea40* self, void* /*edx*/, S_0047ea40* source)
{
    if (self == source)
        return reinterpret_cast<int*>(self);

    uint32_t sourceBegin = source->field_0004;
    uint32_t sourceEnd = 0;
    uint32_t sourceCount = 0;
    if (sourceBegin) {
        sourceEnd = source->field_0008;
        sourceCount = static_cast<uint32_t>(
            static_cast<int32_t>(sourceEnd - sourceBegin) >> 2);
    }

    if (!sourceCount) {
        DestroyPointerStorage(reinterpret_cast<int*>(self), nullptr);
        return reinterpret_cast<int*>(self);
    }

    const uint32_t destinationBegin = self->field_0004;
    const uint32_t destinationCount = destinationBegin
        ? static_cast<uint32_t>(
              static_cast<int32_t>(self->field_0008 - destinationBegin) >> 2)
        : 0;

    if (sourceCount <= destinationCount) {
        sourceBegin = source->field_0004;
        const uint32_t byteCount = static_cast<uint32_t>(
            static_cast<int32_t>(sourceEnd - sourceBegin) >> 2) << 2;
        _memcpy(reinterpret_cast<void*>(destinationBegin),
                reinterpret_cast<const void*>(sourceBegin), byteCount);

        sourceBegin = source->field_0004;
        const uint32_t count = sourceBegin
            ? static_cast<uint32_t>(
                  static_cast<int32_t>(source->field_0008 - sourceBegin) >> 2)
            : 0;
        self->field_0008 = self->field_0004 + count * 4;
        return reinterpret_cast<int*>(self);
    }

    const uint32_t capacity = destinationBegin
        ? static_cast<uint32_t>(
              static_cast<int32_t>(self->field_000c - destinationBegin) >> 2)
        : 0;

    // The original clears only the low byte of its source argument.
    const int copyTag = static_cast<int>(
        reinterpret_cast<uint32_t>(source) & 0xFFFFFF00u);

    if (sourceCount > capacity) {
        if (destinationBegin) {
            ReleaseVectorStorage(
                reinterpret_cast<void*>(destinationBegin),
                static_cast<int32_t>(self->field_000c - destinationBegin) >> 2);
        }

        sourceBegin = source->field_0004;
        const int count = sourceBegin
            ? (static_cast<int32_t>(source->field_0008 - sourceBegin) >> 2)
            : 0;

        if (AllocateVectorStorage(self, nullptr, count)) {
            auto* destination = reinterpret_cast<uint32_t*>(self->field_0004);
            auto* end = reinterpret_cast<uint32_t*>(source->field_0008);
            auto* begin = reinterpret_cast<uint32_t*>(source->field_0004);
            self->field_0008 = reinterpret_cast<uint32_t>(
                CopyPointerRange(begin, end, destination,
                                 reinterpret_cast<int>(self), copyTag));
        }
        return reinterpret_cast<int*>(self);
    }

    const int existingCount = destinationBegin
        ? (static_cast<int32_t>(self->field_0008 - destinationBegin) >> 2)
        : 0;
    sourceBegin = source->field_0004;
    const uint32_t remainingBegin =
        sourceBegin + static_cast<uint32_t>(existingCount) * 4;
    const uint32_t byteCount = static_cast<uint32_t>(
        static_cast<int32_t>(remainingBegin - sourceBegin) >> 2) << 2;

    _memcpy(reinterpret_cast<void*>(destinationBegin),
            reinterpret_cast<const void*>(sourceBegin), byteCount);

    auto* destinationEnd = reinterpret_cast<uint32_t*>(self->field_0008);
    auto* remainingEnd = reinterpret_cast<uint32_t*>(source->field_0008);
    self->field_0008 = reinterpret_cast<uint32_t>(
        CopyPointerRange(reinterpret_cast<uint32_t*>(remainingBegin),
                         remainingEnd, destinationEnd,
                         reinterpret_cast<int>(self), copyTag));
    return reinterpret_cast<int*>(self);
}

HOOK(0x0047EA40, S_0047ea40_AssignPointerStorage, "auto");
