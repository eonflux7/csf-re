#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_FUNC(int, __cdecl, CoreMemory_AllocUntagged, 0x006910F0, (std::size_t size));
CSF_FUNC(int, __fastcall, C_00831678_RegisterChild, 0x00474670, (void* self, void* edx, int a2));

static int __fastcall C_00831678_CreateAndRegisterChild(C_00831678* self, void* /*edx*/)
{
    int allocated = CoreMemory_AllocUntagged(88);
    if (allocated != 0)
    {
        C_00831678* obj = reinterpret_cast<C_00831678*>(static_cast<uintptr_t>(allocated));

        obj->field_000c = 0xFFFFFFFFu;
        obj->field_0010 = 0xFFFFFFFFu;
        obj->field_0008 = static_cast<uint32_t>(reinterpret_cast<uintptr_t>(CSF_VTBL(0x0082D448)));
        obj->field_0014 = static_cast<uint32_t>(reinterpret_cast<uintptr_t>(CSF_VTBL(0x0082D448)));
        obj->field_0018 = 0xFFFFFFFFu;
        obj->field_001c = 0xFFFFFFFFu;
        obj->field_0020 = 0;
        obj->field_0024 = 0;
        obj->field_002c = 0;
        obj->field_0028 = 0;
        obj->field_0030 = 0;
        obj->field_0004 = 0xFFFFFFFFu;
        obj->vtbl = CSF_VTBL(0x00831678);
        obj->field_0034 = 0;
        obj->field_0038 = 0;
        obj->field_003c = 0;
        obj->field_0040 = 0;
        obj->field_0044 = 0;
        obj->field_0048 = 0;

        C_00831678_RegisterChild(self, nullptr, allocated);
        return allocated;
    }

    C_00831678_RegisterChild(self, nullptr, 0);
    return 0;
}

HOOK(0x474AB0, C_00831678_CreateAndRegisterChild, "auto");
