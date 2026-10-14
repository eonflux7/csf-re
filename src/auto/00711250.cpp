#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

// Virtual slot 0x34 / 4 = 13 on the inner object's vtable (__stdcall, 2 args).
// The original leaves the inner vtable in ecx and a2 in edx (`mov ecx, [eax]`; `mov edx, [ebp+8]`) and pushes
// the object and a2 on the stack. The __fastcall pointer below passes those same registers and stack arguments.
static int __fastcall C_00863cd4_ForwardToInnerSlot13(C_00863cd4* self, void* /*edx*/, int a2)
{
    void* innerObject = reinterpret_cast<void*>(self->field_0004);
    void* innerVtbl = CSF_AT(void*, innerObject, 0);
    auto innerMethod = reinterpret_cast<int(__fastcall*)(void*, int, void*, int)>(CSF_AT(void**, innerObject, 0)[13]);
    return innerMethod(innerVtbl, a2, innerObject, a2);
}

HOOK(0x711250, C_00863cd4_ForwardToInnerSlot13, "auto");
