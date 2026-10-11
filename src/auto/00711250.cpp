#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

// Virtual slot 0x34 / 4 = 13 on the inner object's vtable (__stdcall, 2 args).
static int __fastcall C_00863cd4_ForwardToInnerSlot13(C_00863cd4* self, void* /*edx*/, int a2)
{
    void* innerObject = reinterpret_cast<void*>(self->field_0004);
    auto innerMethod = reinterpret_cast<int(__stdcall*)(void*, int)>(CSF_AT(void**, innerObject, 0)[13]);
    return innerMethod(innerObject, a2);
}

HOOK(0x711250, C_00863cd4_ForwardToInnerSlot13, "auto");
