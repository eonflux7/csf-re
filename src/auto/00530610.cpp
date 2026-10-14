#include <cstdint>
#include "hook.h"
#include "types/auto_classes.h"

// 0x00530610
// If a sub-object pointer is stored at +0x1EC, invoke its first virtual method with
// argument 1 and clear the stored pointer. Returns the callee's result (undefined
// when no sub-object is present).
static int __fastcall CMenuMpServerInfo_ReleaseSubObject(CMenuMpServerInfo* self, void* /*edx*/)
{
    int result;
    void** sub = reinterpret_cast<void**>(static_cast<uintptr_t>(self->field_01ec));
    if ( sub )
    {
        // vtable slot 0, __thiscall with one stack argument (popped by the callee)
        // mov eax,[ecx] (vtable); call [eax] (slot 0): two loads from the object
        void** vtbl = reinterpret_cast<void**>(sub[0]);
        result = reinterpret_cast<int(__fastcall*)(void*, void*, int)>(vtbl[0])(sub, nullptr, 1);
        self->field_01ec = 0;
    }
    return result;
}

HOOK(0x530610, CMenuMpServerInfo_ReleaseSubObject, "auto");
