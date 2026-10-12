#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_FUNC(char, __fastcall, AllocateRecordStorage, 0x00410B80,
         (void* self, void* edx, int a2));
CSF_FUNC(int, __cdecl, MissionScript_DestroyElementRange, 0x00469760,
         (int* first, int* last));
CSF_FUNC(int, __fastcall, MissionScript_ForwardElementRangeDestroy, 0x00469FB0,
         (void* owner, void*, int* begin, int* end));
CSF_FUNC(int, __fastcall, MissionScript_GetEntryCount_46BEA0, 0x0046BEA0,
         (S_0046bea0* self, void*));
CSF_FUNC(int, __cdecl, MissionScript_CopyRecordRange_46F990, 0x0046F990,
         (void* source, void* sourceEnd, void* destination, void*, int));
CSF_FUNC(int, __fastcall, MissionScript_ForwardToParseEntryHelper_470300, 0x00470300,
         (void* self, void*, void* a1, void* a2, int a3));
CSF_FUNC(int*, __fastcall, MenuManager_ClearElementArray, 0x0050C230,
         (int* self, void*));
CSF_FUNC(void*, __cdecl, CopyAssignRecordRange, 0x00519190,
         (void* a1, void* a2, void* a3, int a4));
CSF_FUNC(void*, __cdecl, CopyAssignRecordPrefix, 0x005191D0,
         (void* a1, void* a2, void* a3));
CSF_FUNC(void*, __stdcall, ReleaseVectorStorage, 0x006C8D30,
         (void* block, int count));

static std::uint32_t CountRecordsBetween(std::uint32_t first,
                                        std::uint32_t last)
{
    return static_cast<std::uint32_t>(
        static_cast<std::int32_t>(last - first) / 28);
}

static int __fastcall S_0046bea0_AssignRecords(
    S_0046bea0* self, void* /*edx*/, int sourceAddress)
{
    auto* source = reinterpret_cast<S_0046bea0*>(sourceAddress);
    const int result = static_cast<int>(
        reinterpret_cast<std::uintptr_t>(self));

    if (self == source)
        return result;

    const std::uint32_t sourceBegin = source->field_0004;
    const std::uint32_t sourceCount = sourceBegin
        ? CountRecordsBetween(sourceBegin, source->field_0008)
        : 0;

    if (sourceCount == 0) {
        MenuManager_ClearElementArray(reinterpret_cast<int*>(self), nullptr);
        return result;
    }

    const std::uint32_t destinationBegin = self->field_0004;
    const std::uint32_t destinationCount = destinationBegin
        ? CountRecordsBetween(destinationBegin, self->field_0008)
        : 0;

    if (sourceCount <= destinationCount) {
        void* sourceEnd = reinterpret_cast<void*>(source->field_0008);
        void* sourceFirst = reinterpret_cast<void*>(source->field_0004);
        void* copiedEnd = CopyAssignRecordRange(
            sourceFirst, sourceEnd,
            reinterpret_cast<void*>(destinationBegin), sourceAddress);

        MissionScript_DestroyElementRange(
            static_cast<int*>(copiedEnd),
            reinterpret_cast<int*>(self->field_0008));

        const std::uint32_t currentSourceBegin = source->field_0004;
        const std::uint32_t currentSourceCount = currentSourceBegin
            ? CountRecordsBetween(currentSourceBegin, source->field_0008)
            : 0;
        self->field_0008 = self->field_0004 + 28 * currentSourceCount;
        return result;
    }

    const std::uint32_t capacity = destinationBegin
        ? CountRecordsBetween(destinationBegin, self->field_000c)
        : 0;

    if (sourceCount <= capacity) {
        const int existingCount =
            MissionScript_GetEntryCount_46BEA0(self, nullptr);
        const std::uint32_t currentSourceBegin = source->field_0004;
        void* split = reinterpret_cast<void*>(
            currentSourceBegin + 28 * static_cast<std::uint32_t>(existingCount));

        CopyAssignRecordPrefix(
            reinterpret_cast<void*>(currentSourceBegin), split,
            reinterpret_cast<void*>(destinationBegin));

        void* sourceEnd = reinterpret_cast<void*>(source->field_0008);
        void* destinationEnd = reinterpret_cast<void*>(self->field_0008);
        self->field_0008 = static_cast<std::uint32_t>(
            MissionScript_CopyRecordRange_46F990(
                split, sourceEnd, destinationEnd, self, sourceAddress));
        return result;
    }

    if (destinationBegin) {
        MissionScript_ForwardElementRangeDestroy(
            self, nullptr, reinterpret_cast<int*>(destinationBegin),
            reinterpret_cast<int*>(self->field_0008));

        const std::uint32_t storage = self->field_0004;
        const std::uint32_t storageEnd = self->field_000c;
        ReleaseVectorStorage(
            reinterpret_cast<void*>(storage),
            static_cast<int>(CountRecordsBetween(storage, storageEnd)));
    }

    const int replacementCount =
        MissionScript_GetEntryCount_46BEA0(source, nullptr);
    if (AllocateRecordStorage(self, nullptr, replacementCount)) {
        const int destination = static_cast<int>(self->field_0004);
        void* sourceEnd = reinterpret_cast<void*>(source->field_0008);
        void* sourceFirst = reinterpret_cast<void*>(source->field_0004);
        self->field_0008 = static_cast<std::uint32_t>(
            MissionScript_ForwardToParseEntryHelper_470300(
                self, nullptr, sourceFirst, sourceEnd, destination));
    }

    return result;
}

HOOK(0x006D9890, S_0046bea0_AssignRecords, "auto");
