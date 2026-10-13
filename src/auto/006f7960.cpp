#include <cstdint>
#include <cstddef>
#include <cstring>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_FUNC(void*, __fastcall, S_0041e5f0_GrowArrayCapacity, 0x0041E5F0, (S_0041e5f0* self, void* , int additionalCapacity));
CSF_FUNC(int, __cdecl, CoreMemory_AllocUntagged, 0x006910F0, (std::size_t size));
CSF_FUNC(int, __cdecl, GfxHAnim_CreateHierarchyFromSourceObject, 0x006E80F0, (uint32_t* self));
CSF_FUNC(int*, __cdecl, HAnim_BuildParentIndexTable, 0x006E8340, (void* hierarchy));
CSF_FUNC(int, __cdecl, HAnimForwardToRwEngineSlot67, 0x006E83D0, (int a1));

static int __fastcall S_0041e5f0_RegisterHierarchy(
    S_0041e5f0* self, void* /*edx*/, uint32_t* source)
{
    int* parentIndices = HAnim_BuildParentIndexTable(source);
    const uint32_t nodeCount = source[1];

    for (uint32_t index = 0; index < self->field_0008; ++index)
    {
        void* entry = reinterpret_cast<void**>(self->field_0000)[index];
        if (CSF_AT(uint32_t, entry, 0x04) == nodeCount &&
            std::memcmp(parentIndices,
                        reinterpret_cast<const void*>(
                            CSF_AT(uint32_t, entry, 0x08)),
                        4u * nodeCount) == 0)
        {
            entry = reinterpret_cast<void**>(self->field_0000)[index];
            ++CSF_AT(uint32_t, entry, 0x1C);
            HAnimForwardToRwEngineSlot67(
                reinterpret_cast<int>(parentIndices));
            return static_cast<int>(index);
        }
    }

    void* entry = reinterpret_cast<void*>(CoreMemory_AllocUntagged(32));
    if (entry)
    {
        CSF_AT(uint32_t, entry, 0x0C) = 0;
        CSF_AT(uint32_t, entry, 0x10) = 0;
        CSF_AT(uint32_t, entry, 0x14) = 0;
        CSF_AT(uint32_t, entry, 0x18) = 10;
    }

    CSF_AT(uint32_t, entry, 0x04) = nodeCount;
    CSF_AT(uint32_t, entry, 0x08) =
        reinterpret_cast<uint32_t>(parentIndices);
    CSF_AT(uint32_t, entry, 0x1C) = 1;
    CSF_AT(uint32_t, entry, 0x00) =
        GfxHAnim_CreateHierarchyFromSourceObject(source);

    if (self->field_0008 >= self->field_0004)
    {
        // The packet also identifies the direct callee as a vtable reference.
        (void)CSF_VTBL(0x0041E5F0);
        S_0041e5f0_GrowArrayCapacity(
            self, nullptr, static_cast<int>(self->field_000c));
    }

    const uint32_t index = self->field_0008;
    void** slot = reinterpret_cast<void**>(self->field_0000) + index;
    self->field_0008 = index + 1;
    *slot = entry;
    return static_cast<int>(self->field_0008 - 1);
}

HOOK(0x006F7960, S_0041e5f0_RegisterHierarchy, "auto");
