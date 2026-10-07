#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

// 0x006F04C0: tail-forwards to virtual slot 7 (vtable offset 0x1C) of the object
// stored in field_0020, passing that object as `this` and no further arguments.
static int __fastcall C_008622e8_InvokeInnerVirtualSlot7(C_008622e8* self, void* /*edx*/)
{
    void* inner = reinterpret_cast<void*>(self->field_0020);
    void** inner_vtable = reinterpret_cast<void**>(CSF_AT(void*, inner, 0));
    int (__fastcall * slot7)(void*, void*) =
        reinterpret_cast<int (__fastcall*)(void*, void*)>(inner_vtable[7]);
    return slot7(inner, nullptr);
}

HOOK(0x6F04C0, C_008622e8_InvokeInnerVirtualSlot7, "auto");
