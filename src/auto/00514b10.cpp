#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_FUNC(char, __fastcall, AllocateRecordStorage_410BD0, 0x00410BD0, (void* self, void* edx, std::uint32_t count));
CSF_FUNC(uint32_t*, __cdecl, ProfilesSavegames_CopyRecords, 0x00514910, (uint32_t* a1, uint32_t* a2, uint32_t* a3));
CSF_FUNC(int, __cdecl, ProfilesSavegames_ConstructRecordTail, 0x00514930, (int a1, int a2, int a3, int a4, int a5));
CSF_FUNC(int, __fastcall, S_00514aa0_ConstructRecordRange, 0x00514AA0, (void* self, void* edx, int a1, int a2, int a3));
CSF_FUNC(int, __cdecl, Mem_FreeDebug, 0x00690E60, (void* block));
CSF_FUNC(void*, __stdcall, ReleaseVectorStorage, 0x006C8D30, (void* block, int count));
CSF_FUNC(uint32_t*, __cdecl, ProfilesSavegames_AssignRecordRange, 0x006D9E00, (uint32_t* a1, uint32_t* a2, uint32_t* a3, int a4));

// The packet also classifies the construction callee's address as a vtable.
[[maybe_unused]] static const auto kReportedRecordVtable = CSF_VTBL(0x00514AA0);

static int __fastcall S_00514aa0_AssignRecords(
    S_00514aa0* self, void* /*edx*/, int sourceAddress)
{
    auto* source = reinterpret_cast<S_00514aa0*>(sourceAddress);
    if (self == source)
        return reinterpret_cast<int>(self);

    const std::uint32_t sourceBegin = source->field_0004;
    const std::uint32_t sourceCount = sourceBegin
        ? static_cast<std::uint32_t>(
            static_cast<std::int32_t>(source->field_0008 - sourceBegin) >> 4)
        : 0;

    if (sourceCount == 0)
    {
        if (self->field_0004)
            Mem_FreeDebug(reinterpret_cast<void*>(self->field_0004));

        self->field_0004 = 0;
        self->field_0008 = 0;
        self->field_000c = 0;
        return reinterpret_cast<int>(self);
    }

    const std::uint32_t destinationBegin = self->field_0004;
    const std::uint32_t destinationCount = destinationBegin
        ? static_cast<std::uint32_t>(
            static_cast<std::int32_t>(self->field_0008 - destinationBegin) >> 4)
        : 0;

    if (sourceCount <= destinationCount)
    {
        ProfilesSavegames_AssignRecordRange(
            reinterpret_cast<uint32_t*>(sourceBegin),
            reinterpret_cast<uint32_t*>(source->field_0008),
            reinterpret_cast<uint32_t*>(destinationBegin),
            sourceAddress);

        const std::uint32_t updatedSourceBegin = source->field_0004;
        const std::uint32_t updatedSourceCount = updatedSourceBegin
            ? static_cast<std::uint32_t>(
                static_cast<std::int32_t>(
                    source->field_0008 - updatedSourceBegin) >> 4)
            : 0;
        self->field_0008 = self->field_0004 + 16 * updatedSourceCount;
        return reinterpret_cast<int>(self);
    }

    const std::uint32_t destinationCapacity = destinationBegin
        ? static_cast<std::uint32_t>(
            static_cast<std::int32_t>(self->field_000c - destinationBegin) >> 4)
        : 0;

    if (sourceCount > destinationCapacity)
    {
        if (destinationBegin)
        {
            ReleaseVectorStorage(
                reinterpret_cast<void*>(destinationBegin),
                static_cast<std::int32_t>(
                    self->field_000c - destinationBegin) >> 4);
        }

        const std::uint32_t updatedSourceBegin = source->field_0004;
        const std::uint32_t allocationCount = updatedSourceBegin
            ? static_cast<std::uint32_t>(
                static_cast<std::int32_t>(
                    source->field_0008 - updatedSourceBegin) >> 4)
            : 0;

        if (AllocateRecordStorage_410BD0(self, nullptr, allocationCount))
        {
            self->field_0008 = S_00514aa0_ConstructRecordRange(
                self, nullptr,
                static_cast<int>(source->field_0004),
                static_cast<int>(source->field_0008),
                static_cast<int>(self->field_0004));
        }
        return reinterpret_cast<int>(self);
    }

    const std::int32_t existingCount = destinationBegin
        ? (static_cast<std::int32_t>(
            self->field_0008 - destinationBegin) >> 4)
        : 0;
    const std::uint32_t sourceTail =
        source->field_0004 + 16 * static_cast<std::uint32_t>(existingCount);

    ProfilesSavegames_CopyRecords(
        reinterpret_cast<uint32_t*>(source->field_0004),
        reinterpret_cast<uint32_t*>(sourceTail),
        reinterpret_cast<uint32_t*>(destinationBegin));

    self->field_0008 = ProfilesSavegames_ConstructRecordTail(
        static_cast<int>(sourceTail),
        static_cast<int>(source->field_0008),
        static_cast<int>(self->field_0008),
        reinterpret_cast<int>(self),
        sourceAddress);

    return reinterpret_cast<int>(self);
}

HOOK(0x00514B10, S_00514aa0_AssignRecords, "auto");
