#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_FUNC(void*, __stdcall, ReleaseVectorStorage, 0x006C8D30,
         (void* block, int count));
CSF_FUNC(int, __fastcall, GetRecordCount, 0x006D4160,
         (void* self, void* edx));
CSF_FUNC(uint32_t*, __cdecl, GameDataDefs_ConstructRecordTail, 0x00775000,
         (uint32_t* a1, uint32_t* a2, uint32_t* a3, int a4, int a5));
CSF_FUNC(int, __cdecl, ReleaseNativeHandle, 0x00775360,
         (int a1, int a2));
CSF_FUNC(uint32_t*, __cdecl, GameDataDefs_AssignRecordRange, 0x00775600,
         (uint32_t* a1, uint32_t* a2, uint32_t* a3, int a4));
CSF_FUNC(char, __fastcall, AllocateRecordStorage_775690, 0x00775690,
         (void* self, void* edx, int count));
CSF_FUNC(uint32_t*, __fastcall, GameDataDefs_ConstructRecordRange, 0x00775810,
         (void* self, void* edx, uint32_t* a1, uint32_t* a2, uint32_t* a3));
CSF_FUNC(uint32_t*, __cdecl, GameDataDefs_AssignRecordPrefix, 0x00775910,
         (uint32_t* a1, uint32_t* a2, uint32_t* a3));
CSF_FUNC(int, __fastcall, GameDataDefs_DestroyRecordRange, 0x007759D0,
         (void* self, void* edx, int a1, int a2));
CSF_FUNC(char, __fastcall, GameDataDefs_ClearRecords, 0x00776840,
         (void* self, void* edx));

static uint32_t GameDataDefs_CountRecordsBetween(uint32_t first, uint32_t last)
{
    return static_cast<uint32_t>(static_cast<int32_t>(last - first) / 60);
}

static int __fastcall S_006d4160_AssignRecords(
    S_006d4160* self, void* /*edx*/, int sourceAddress)
{
    auto* source = reinterpret_cast<S_006d4160*>(sourceAddress);
    const int result = static_cast<int>(reinterpret_cast<uintptr_t>(self));

    if (self == source)
        return result;

    const uint32_t sourceBegin = source->field_0004;
    const uint32_t sourceCount = sourceBegin
        ? GameDataDefs_CountRecordsBetween(sourceBegin, source->field_0008)
        : 0;

    if (sourceCount == 0) {
        GameDataDefs_ClearRecords(self, nullptr);
        return result;
    }

    const uint32_t destinationBegin = self->field_0004;
    const uint32_t destinationCount = destinationBegin
        ? GameDataDefs_CountRecordsBetween(destinationBegin, self->field_0008)
        : 0;

    if (sourceCount <= destinationCount) {
        auto* sourceEnd = reinterpret_cast<uint32_t*>(source->field_0008);
        auto* sourceFirst = reinterpret_cast<uint32_t*>(source->field_0004);
        uint32_t* copiedEnd = GameDataDefs_AssignRecordRange(
            sourceFirst, sourceEnd,
            reinterpret_cast<uint32_t*>(destinationBegin), sourceAddress);

        ReleaseNativeHandle(
            static_cast<int>(reinterpret_cast<uintptr_t>(copiedEnd)),
            static_cast<int>(self->field_0008));

        const uint32_t currentSourceBegin = source->field_0004;
        const uint32_t currentSourceCount = currentSourceBegin
            ? GameDataDefs_CountRecordsBetween(
                currentSourceBegin, source->field_0008)
            : 0;
        self->field_0008 = self->field_0004 + 60 * currentSourceCount;
        return result;
    }

    const uint32_t capacity = destinationBegin
        ? GameDataDefs_CountRecordsBetween(destinationBegin, self->field_000c)
        : 0;

    if (sourceCount <= capacity) {
        const int existingCount = GetRecordCount(self, nullptr);
        const uint32_t currentSourceBegin = source->field_0004;
        auto* split = reinterpret_cast<uint32_t*>(
            currentSourceBegin + 60 * static_cast<uint32_t>(existingCount));

        GameDataDefs_AssignRecordPrefix(
            reinterpret_cast<uint32_t*>(currentSourceBegin), split,
            reinterpret_cast<uint32_t*>(destinationBegin));

        auto* sourceEnd = reinterpret_cast<uint32_t*>(source->field_0008);
        auto* destinationEnd = reinterpret_cast<uint32_t*>(self->field_0008);
        self->field_0008 = static_cast<uint32_t>(
            reinterpret_cast<uintptr_t>(GameDataDefs_ConstructRecordTail(
                split, sourceEnd, destinationEnd, result, sourceAddress)));
        return result;
    }

    if (destinationBegin) {
        GameDataDefs_DestroyRecordRange(
            self, nullptr, static_cast<int>(destinationBegin),
            static_cast<int>(self->field_0008));

        const uint32_t storage = self->field_0004;
        const uint32_t storageEnd = self->field_000c;
        ReleaseVectorStorage(
            reinterpret_cast<void*>(storage),
            static_cast<int>(GameDataDefs_CountRecordsBetween(
                storage, storageEnd)));
    }

    const int replacementCount = GetRecordCount(source, nullptr);
    if (AllocateRecordStorage_775690(self, nullptr, replacementCount)) {
        auto* destination = reinterpret_cast<uint32_t*>(self->field_0004);
        auto* sourceEnd = reinterpret_cast<uint32_t*>(source->field_0008);
        auto* sourceFirst = reinterpret_cast<uint32_t*>(source->field_0004);
        self->field_0008 = static_cast<uint32_t>(
            reinterpret_cast<uintptr_t>(GameDataDefs_ConstructRecordRange(
                self, nullptr, sourceFirst, sourceEnd, destination)));
    }

    return result;
}

HOOK(0x00776DB0, S_006d4160_AssignRecords, "auto");
