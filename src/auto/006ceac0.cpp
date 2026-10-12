#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

static CSF_FUNC(int, __fastcall, GfxCollisionShapes_GetSize, 0x0044BB60, (void* self, void* edx));
static CSF_FUNC(void*, __stdcall, ReleaseVectorStorage, 0x006C8D30, (void* block, int count));
static CSF_FUNC(int, __cdecl, GfxCollisionShapes_DestroyTrailingRange, 0x006CE270, (int a1, int a2));
static CSF_FUNC(uint32_t*, __cdecl, GfxCollisionShapes_ConstructAdditionalRange, 0x006CE310, (uint32_t* a1, uint32_t* a2, uint32_t* a3, int a4, int a5));
static CSF_FUNC(uint32_t*, __cdecl, GfxCollisionShapes_CopyAssignRange, 0x006CE480, (uint32_t* a1, uint32_t* a2, uint32_t* a3, int a4));
static CSF_FUNC(char, __fastcall, GfxCollisionShapes_AllocateStorage, 0x006CE590, (void* self, void* edx, int a2));
static CSF_FUNC(int, __fastcall, GfxCollisionShapes_DestroyRange, 0x006CE5E0, (void* self, void* edx, int a1, int a2));
static CSF_FUNC(uint32_t*, __cdecl, GfxCollisionShapes_CopyExistingRange, 0x006CE600, (uint32_t* a1, uint32_t* a2, uint32_t* a3));
static CSF_FUNC(uint32_t*, __fastcall, GfxCollisionShapes_CopyConstructRange, 0x006CE620, (void* self, void* edx, uint32_t* a1, uint32_t* a2, uint32_t* a3));
static CSF_FUNC(char, __fastcall, GfxCollisionShapes_Clear, 0x006CE690, (void* self, void* edx));

[[maybe_unused]] static const auto g_PacketSizeGetterVtable = CSF_VTBL(0x0044BB60);

static int __fastcall S_0044bb60_Assign(
    S_0044bb60* self, void* /*edx*/, S_0044bb60* source)
{
    if (self == source)
        return reinterpret_cast<int>(self);

    const uint32_t sourceBegin = source->field_0004;
    uint32_t sourceEnd = 0;
    uint32_t sourceCount = 0;
    if (sourceBegin)
    {
        sourceEnd = source->field_0008;
        sourceCount = static_cast<int32_t>(sourceEnd - sourceBegin) / 100;
    }

    if (!sourceCount)
    {
        GfxCollisionShapes_Clear(self, nullptr);
        return reinterpret_cast<int>(self);
    }

    const uint32_t destinationBegin = self->field_0004;
    const uint32_t destinationCount = destinationBegin
        ? static_cast<int32_t>(self->field_0008 - destinationBegin) / 100
        : 0;

    if (sourceCount <= destinationCount)
    {
        uint32_t* copiedEnd = GfxCollisionShapes_CopyAssignRange(
            reinterpret_cast<uint32_t*>(source->field_0004),
            reinterpret_cast<uint32_t*>(sourceEnd),
            reinterpret_cast<uint32_t*>(destinationBegin),
            reinterpret_cast<int>(source));

        GfxCollisionShapes_DestroyTrailingRange(
            reinterpret_cast<int>(copiedEnd),
            static_cast<int>(self->field_0008));

        const uint32_t currentSourceBegin = source->field_0004;
        if (currentSourceBegin)
        {
            const int count =
                static_cast<int32_t>(source->field_0008 - currentSourceBegin) / 100;
            self->field_0008 = self->field_0004 + 100 * static_cast<uint32_t>(count);
        }
        else
        {
            self->field_0008 = self->field_0004;
        }
        return reinterpret_cast<int>(self);
    }

    const uint32_t capacity = destinationBegin
        ? static_cast<int32_t>(self->field_000c - destinationBegin) / 100
        : 0;

    if (sourceCount <= capacity)
    {
        const int currentSize = GfxCollisionShapes_GetSize(self, nullptr);
        const uint32_t currentSourceBegin = source->field_0004;
        uint32_t* split = reinterpret_cast<uint32_t*>(
            currentSourceBegin + 100 * static_cast<uint32_t>(currentSize));

        GfxCollisionShapes_CopyExistingRange(
            reinterpret_cast<uint32_t*>(currentSourceBegin),
            split,
            reinterpret_cast<uint32_t*>(destinationBegin));

        self->field_0008 = reinterpret_cast<uint32_t>(
            GfxCollisionShapes_ConstructAdditionalRange(
                split,
                reinterpret_cast<uint32_t*>(source->field_0008),
                reinterpret_cast<uint32_t*>(self->field_0008),
                reinterpret_cast<int>(self),
                reinterpret_cast<int>(source)));
        return reinterpret_cast<int>(self);
    }

    if (destinationBegin)
    {
        GfxCollisionShapes_DestroyRange(
            self, nullptr,
            static_cast<int>(destinationBegin),
            static_cast<int>(self->field_0008));

        const uint32_t block = self->field_0004;
        const int allocatedCount =
            static_cast<int32_t>(self->field_000c - block) / 100;
        ReleaseVectorStorage(reinterpret_cast<void*>(block), allocatedCount);
    }

    const int requiredCount = GfxCollisionShapes_GetSize(source, nullptr);
    if (GfxCollisionShapes_AllocateStorage(self, nullptr, requiredCount))
    {
        self->field_0008 = reinterpret_cast<uint32_t>(
            GfxCollisionShapes_CopyConstructRange(
                self, nullptr,
                reinterpret_cast<uint32_t*>(source->field_0004),
                reinterpret_cast<uint32_t*>(source->field_0008),
                reinterpret_cast<uint32_t*>(self->field_0004)));
    }

    return reinterpret_cast<int>(self);
}

HOOK(0x006CEAC0, S_0044bb60_Assign, "auto");
