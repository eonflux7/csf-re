#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

CSF_FUNC(void*, __fastcall, S_0041e5f0_GrowArrayCapacity, 0x0041E5F0,
         (S_0041e5f0* self, void* /*edx*/, int additionalCapacity));

// Appends boneIndex to the growable int array referenced by *listRef and returns boneIndex.
static int __cdecl GfxBones_AppendBoneIndex(int boneIndex, S_0041e5f0** listRef)
{
    S_0041e5f0* list = *listRef;

    if (list->field_0008 >= list->field_0004)
    {
        (void)CSF_VTBL(0x0041E5F0);
        S_0041e5f0_GrowArrayCapacity(list, nullptr, static_cast<int>(list->field_000c));
    }

    uint32_t count = list->field_0008;
    int* items = reinterpret_cast<int*>(list->field_0000);
    items[count] = boneIndex;
    list->field_0008 = count + 1;

    return boneIndex;
}

HOOK(0x006E7EF0, GfxBones_AppendBoneIndex, "auto");
