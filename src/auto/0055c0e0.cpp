#include <cstdint>
#include "hook.h"
#include "types/auto_classes.h"

// 0x0055C0E0
// If a sub-object pointer is stored at +0x284, invoke its first virtual method
// with argument 1 (callee pops the argument) and then clear the stored pointer.
// Returns the callee's result; undefined when no sub-object is present.
static int __fastcall CMenuAccountEdit_ReleaseSubObject(CMenuAccountEdit* self, void* /*edx*/)
{
    int result;
    void** sub = reinterpret_cast<void**>(static_cast<uintptr_t>(self->field_0284));
    if ( sub )
    {
        // vtable slot 0, __thiscall with one stack argument (popped by the callee)
        result = reinterpret_cast<int(__fastcall*)(void*, void*, int)>(sub[0])(sub, nullptr, 1);
        self->field_0284 = 0;
    }
    return result;
}

HOOK(0x55C0E0, CMenuAccountEdit_ReleaseSubObject, "auto");
