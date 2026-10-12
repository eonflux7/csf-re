#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

static CSF_FUNC(int, __fastcall, ReleaseGameDataRecordLinks, 0x0051AA70, (S_0051aa70* self, void*));
static CSF_FUNC(void*, __stdcall, ReleaseVectorStorage, 0x006C8D30, (void* block, int count));
static CSF_FUNC(char, __fastcall, WaypointGraph_AllocateStorage, 0x0075D970, (void* self, void* edx, int a2));
static CSF_FUNC(int, __cdecl, AnimationDefs_CopyAssignRange, 0x007726C0, (void* a1, void* a2, int a3));
static CSF_FUNC(int, __cdecl, AnimationDefs_ConstructAdditionalRange, 0x007727A0, (void* a1, void* a2, int a3, int a4, int a5));
static CSF_FUNC(int, __cdecl, AnimationDefs_CopyExistingRange, 0x00772830, (void* a1, void* a2, int a3));
static CSF_FUNC(int, __fastcall, AnimationDefs_CopyConstructRange, 0x00772990, (void* self, void* edx, void* a1, void* a2, int a3));
static CSF_FUNC(int, __fastcall, WaypointGraph_GetSize, 0x00774C10, (void* self, void* edx));
static CSF_FUNC(int, __cdecl, UpdateGameDataRecordLinks, 0x007754B0, (int a1, int a2, int a3, int a4));
static CSF_FUNC(int, __fastcall, AnimationDefs_DestroyRange, 0x00776010, (void* self, void* edx, int a1, int a2));

[[maybe_unused]] static const auto g_PacketReleaseLinksVtable = CSF_VTBL(0x0051AA70);

static int __fastcall S_0051aa70_AssignAnimationRecords(
    S_0051aa70* self, void* /*edx*/, S_0051aa70* source)
{
    if (self == source)
        return reinterpret_cast<int>(self);

    const uint32_t sourceBegin = source->field_0004;
    const uint32_t sourceCount = sourceBegin
        ? static_cast<int32_t>(source->field_0008 - sourceBegin) / 44
        : 0;

    if (!sourceCount)
    {
        ReleaseGameDataRecordLinks(self, nullptr);
        return reinterpret_cast<int>(self);
    }

    const uint32_t destinationBegin = self->field_0004;
    const uint32_t destinationCount = destinationBegin
        ? static_cast<int32_t>(self->field_0008 - destinationBegin) / 44
        : 0;

    if (sourceCount <= destinationCount)
    {
        const int copiedEnd = AnimationDefs_CopyAssignRange(
            reinterpret_cast<void*>(source->field_0004),
            reinterpret_cast<void*>(source->field_0008),
            static_cast<int>(destinationBegin));

        UpdateGameDataRecordLinks(
            copiedEnd,
            static_cast<int>(self->field_0008),
            reinterpret_cast<int>(self),
            reinterpret_cast<int>(source));

        const uint32_t currentSourceBegin = source->field_0004;
        if (currentSourceBegin)
        {
            const int count =
                static_cast<int32_t>(source->field_0008 - currentSourceBegin) / 44;
            self->field_0008 =
                self->field_0004 + 44 * static_cast<uint32_t>(count);
        }
        else
        {
            self->field_0008 = self->field_0004;
        }
        return reinterpret_cast<int>(self);
    }

    const uint32_t capacity = destinationBegin
        ? static_cast<int32_t>(self->field_000c - destinationBegin) / 44
        : 0;

    if (sourceCount <= capacity)
    {
        const int currentSize = WaypointGraph_GetSize(self, nullptr);
        const uint32_t currentSourceBegin = source->field_0004;
        void* split = reinterpret_cast<void*>(
            currentSourceBegin + 44 * static_cast<uint32_t>(currentSize));

        AnimationDefs_CopyExistingRange(
            reinterpret_cast<void*>(currentSourceBegin),
            split,
            static_cast<int>(destinationBegin));

        self->field_0008 = static_cast<uint32_t>(
            AnimationDefs_ConstructAdditionalRange(
                split,
                reinterpret_cast<void*>(source->field_0008),
                static_cast<int>(self->field_0008),
                reinterpret_cast<int>(self),
                reinterpret_cast<int>(source)));

        return reinterpret_cast<int>(self);
    }

    if (destinationBegin)
    {
        AnimationDefs_DestroyRange(
            self, nullptr,
            static_cast<int>(destinationBegin),
            static_cast<int>(self->field_0008));

        const uint32_t block = self->field_0004;
        const int allocatedCount =
            static_cast<int32_t>(self->field_000c - block) / 44;
        ReleaseVectorStorage(reinterpret_cast<void*>(block), allocatedCount);
    }

    const int requiredCount = WaypointGraph_GetSize(source, nullptr);
    if (WaypointGraph_AllocateStorage(self, nullptr, requiredCount))
    {
        self->field_0008 = static_cast<uint32_t>(
            AnimationDefs_CopyConstructRange(
                self, nullptr,
                reinterpret_cast<void*>(source->field_0004),
                reinterpret_cast<void*>(source->field_0008),
                static_cast<int>(self->field_0004)));
    }

    return reinterpret_cast<int>(self);
}

HOOK(0x00772E00, S_0051aa70_AssignAnimationRecords, "auto");
