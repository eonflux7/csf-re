#include <cstdint>
#include "hook.h"
#include "core/memory.h"
#include "globals.h"

CSF_FUNC(void*, __stdcall, ReleaseVectorStorage, 0x006C8D30,
         (void* storage, int count));
CSF_FUNC(int, __fastcall, GetRecordCount, 0x006D4160,
         (void* self, void* edx));
CSF_FUNC(int, __cdecl, ResetWideStringsInRecordRange, 0x007233D0,
         (std::uint32_t* first, std::uint32_t* last));
CSF_FUNC(int, __fastcall, ForwardArgumentsToProcessor_723560, 0x00723560,
         (void* self, void* edx, std::uint32_t* first, std::uint32_t* last));
CSF_FUNC(std::uint32_t*, __cdecl, CopyStringRecordsForward, 0x00723580,
         (std::uint32_t* first, std::uint32_t* last, std::uint32_t* destination));
CSF_FUNC(std::uint32_t*, __cdecl, CopyConstructRecordRange_7236C0, 0x007236C0,
         (std::uint32_t* first, std::uint32_t* last, std::uint32_t* destination));
CSF_FUNC(int, __fastcall, ClearWideStringRecordStorage, 0x00723750,
         (void* self, void* edx));
CSF_FUNC(std::uint32_t*, __cdecl, ForwardTo723580, 0x007237A0,
         (std::uint32_t* first, std::uint32_t* last, std::uint32_t* destination));
CSF_FUNC(std::uint32_t*, __fastcall, ForwardTo7236C0, 0x00723930,
         (void* self, void* edx, std::uint32_t* first, std::uint32_t* last, std::uint32_t* destination));
CSF_FUNC(char, __fastcall, AllocateRecordStorage_775690, 0x00775690,
         (void* self, void* edx, int count));

static std::uint32_t CountRecordsBetween(std::uint32_t first, std::uint32_t last)
{
    return static_cast<std::uint32_t>(
        static_cast<std::int32_t>(last - first) / 60);
}

static std::uint32_t* RecordPointer(std::uint32_t address)
{
    return reinterpret_cast<std::uint32_t*>(address);
}

static int __fastcall AssignWideStringRecords(void* self, void* /*edx*/, void* source)
{
    const int result = static_cast<int>(reinterpret_cast<std::uintptr_t>(self));
    if (self == source)
        return result;

    const std::uint32_t sourceBegin = CSF_AT(std::uint32_t, source, 0x04);
    const std::uint32_t sourceCount = sourceBegin
        ? CountRecordsBetween(sourceBegin, CSF_AT(std::uint32_t, source, 0x08))
        : 0;

    if (sourceCount == 0) {
        ClearWideStringRecordStorage(self, nullptr);
        return result;
    }

    const std::uint32_t destinationBegin = CSF_AT(std::uint32_t, self, 0x04);
    const std::uint32_t destinationCount = destinationBegin
        ? CountRecordsBetween(destinationBegin, CSF_AT(std::uint32_t, self, 0x08))
        : 0;

    if (sourceCount <= destinationCount) {
        auto* sourceEnd = RecordPointer(CSF_AT(std::uint32_t, source, 0x08));
        auto* sourceFirst = RecordPointer(CSF_AT(std::uint32_t, source, 0x04));
        auto* copiedEnd = CopyStringRecordsForward(
            sourceFirst, sourceEnd, RecordPointer(destinationBegin));

        auto* destinationEnd = RecordPointer(CSF_AT(std::uint32_t, self, 0x08));
        ResetWideStringsInRecordRange(copiedEnd, destinationEnd);

        const std::uint32_t currentSourceBegin = CSF_AT(std::uint32_t, source, 0x04);
        const std::uint32_t currentSourceCount = currentSourceBegin
            ? CountRecordsBetween(currentSourceBegin,
                                  CSF_AT(std::uint32_t, source, 0x08))
            : 0;
        CSF_AT(std::uint32_t, self, 0x08) =
            CSF_AT(std::uint32_t, self, 0x04) + currentSourceCount * 60u;
        return result;
    }

    const std::uint32_t capacity = destinationBegin
        ? CountRecordsBetween(destinationBegin, CSF_AT(std::uint32_t, self, 0x0C))
        : 0;

    if (sourceCount <= capacity) {
        const int existingCount = GetRecordCount(self, nullptr);
        const std::uint32_t currentSourceBegin = CSF_AT(std::uint32_t, source, 0x04);
        auto* split = RecordPointer(
            currentSourceBegin + static_cast<std::uint32_t>(existingCount) * 60u);

        ForwardTo723580(
            RecordPointer(currentSourceBegin), split, RecordPointer(destinationBegin));

        auto* sourceEnd = RecordPointer(CSF_AT(std::uint32_t, source, 0x08));
        auto* destinationEnd = RecordPointer(CSF_AT(std::uint32_t, self, 0x08));
        auto* constructedEnd = CopyConstructRecordRange_7236C0(split, sourceEnd, destinationEnd);
        CSF_AT(std::uint32_t, self, 0x08) =
            static_cast<std::uint32_t>(reinterpret_cast<std::uintptr_t>(constructedEnd));
        return result;
    }

    if (destinationBegin) {
        auto* destinationEnd = RecordPointer(CSF_AT(std::uint32_t, self, 0x08));
        ForwardArgumentsToProcessor_723560(self, nullptr, RecordPointer(destinationBegin), destinationEnd);  // ecx = this

        const std::uint32_t storage = CSF_AT(std::uint32_t, self, 0x04);
        const std::uint32_t storageEnd = CSF_AT(std::uint32_t, self, 0x0C);
        ReleaseVectorStorage(
            reinterpret_cast<void*>(storage),
            static_cast<int>(CountRecordsBetween(storage, storageEnd)));
    }

    const int replacementCount = GetRecordCount(source, nullptr);
    if (AllocateRecordStorage_775690(self, nullptr, replacementCount)) {
        auto* destination = RecordPointer(CSF_AT(std::uint32_t, self, 0x04));
        auto* sourceEnd = RecordPointer(CSF_AT(std::uint32_t, source, 0x08));
        auto* sourceFirst = RecordPointer(CSF_AT(std::uint32_t, source, 0x04));
        auto* constructedEnd =
            ForwardTo7236C0(self, nullptr, sourceFirst, sourceEnd, destination);  // ecx = this
        CSF_AT(std::uint32_t, self, 0x08) =
            static_cast<std::uint32_t>(reinterpret_cast<std::uintptr_t>(constructedEnd));
    }

    return result;
}

HOOK(0x00723950, AssignWideStringRecords, "auto");
