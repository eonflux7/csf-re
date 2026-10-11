#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

static CSF_FUNC(void*, __stdcall, ReleaseVectorStorage, 0x006C8D30, (void* block, int count));
static CSF_FUNC(uint32_t*, __cdecl, WaypointGraph_CopyAssignRange, 0x0075C350, (uint32_t* a1, uint32_t* a2, uint32_t* a3, int a4));
static CSF_FUNC(uint32_t*, __cdecl, WaypointGraph_CopyExistingRange, 0x0075C440, (uint32_t* a1, uint32_t* a2, uint32_t* a3));
static CSF_FUNC(uint32_t*, __cdecl, WaypointGraph_ConstructAdditionalRange, 0x0075C490, (uint32_t* a1, uint32_t* a2, uint32_t* a3, int a4, int a5));
static CSF_FUNC(int, __cdecl, WaypointGraph_DestroyTrailingRange, 0x0075D0E0, (int a1, int a2, int a3, int a4));
static CSF_FUNC(uint32_t*, __fastcall, WaypointGraph_CopyConstructRange, 0x0075D370, (void* self, void* edx, uint32_t* a1, uint32_t* a2, uint32_t* a3));
static CSF_FUNC(int, __fastcall, WaypointGraph_DestroyRange, 0x0075D390, (void* self, void* edx, int a1, int a2));
static CSF_FUNC(char, __fastcall, WaypointGraph_Clear, 0x0075D3B0, (void* self, void* edx));
static CSF_FUNC(char, __fastcall, WaypointGraph_AllocateStorage, 0x0075D970, (void* self, void* edx, int a2));
static CSF_FUNC(int, __fastcall, WaypointGraph_GetSize, 0x00774C10, (void* self, void* edx));

static int __fastcall S_0051aa70_Assign(
    S_0051aa70* self, void* /*edx*/, S_0051aa70* source)
{
    if (self == source)
        return reinterpret_cast<int>(self);

    const uint32_t sourceBegin = source->field_0004;
    uint32_t sourceEnd = 0;
    uint32_t sourceCount = 0;
    if (sourceBegin)
    {
        sourceEnd = source->field_0008;
        sourceCount = static_cast<int32_t>(sourceEnd - sourceBegin) / 44;
    }

    if (!sourceCount)
    {
        WaypointGraph_Clear(self, nullptr);
        return reinterpret_cast<int>(self);
    }

    const uint32_t destinationBegin = self->field_0004;
    const uint32_t destinationCount = destinationBegin
        ? static_cast<int32_t>(self->field_0008 - destinationBegin) / 44
        : 0;

    if (sourceCount <= destinationCount)
    {
        uint32_t* copiedEnd = WaypointGraph_CopyAssignRange(
            reinterpret_cast<uint32_t*>(source->field_0004),
            reinterpret_cast<uint32_t*>(sourceEnd),
            reinterpret_cast<uint32_t*>(destinationBegin),
            reinterpret_cast<int>(source));

        WaypointGraph_DestroyTrailingRange(
            reinterpret_cast<int>(copiedEnd),
            static_cast<int>(self->field_0008),
            reinterpret_cast<int>(self),
            reinterpret_cast<int>(source));

        const uint32_t currentSourceBegin = source->field_0004;
        if (currentSourceBegin)
        {
            const int count =
                static_cast<int32_t>(source->field_0008 - currentSourceBegin) / 44;
            self->field_0008 = self->field_0004 + 44 * static_cast<uint32_t>(count);
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
        uint32_t* split = reinterpret_cast<uint32_t*>(
            currentSourceBegin + 44 * static_cast<uint32_t>(currentSize));

        WaypointGraph_CopyExistingRange(
            reinterpret_cast<uint32_t*>(currentSourceBegin),
            split,
            reinterpret_cast<uint32_t*>(destinationBegin));

        self->field_0008 = reinterpret_cast<uint32_t>(
            WaypointGraph_ConstructAdditionalRange(
                split,
                reinterpret_cast<uint32_t*>(source->field_0008),
                reinterpret_cast<uint32_t*>(self->field_0008),
                reinterpret_cast<int>(self),
                reinterpret_cast<int>(source)));
        return reinterpret_cast<int>(self);
    }

    if (destinationBegin)
    {
        WaypointGraph_DestroyRange(
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
        self->field_0008 = reinterpret_cast<uint32_t>(
            WaypointGraph_CopyConstructRange(
                self, nullptr,
                reinterpret_cast<uint32_t*>(source->field_0004),
                reinterpret_cast<uint32_t*>(source->field_0008),
                reinterpret_cast<uint32_t*>(self->field_0004)));
    }

    return reinterpret_cast<int>(self);
}

HOOK(0x0075DAA0, S_0051aa70_Assign, "auto");
