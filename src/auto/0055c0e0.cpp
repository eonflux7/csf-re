#include <cstdint>
#include "hook.h"
#include "types/auto_classes.h"

// 0x0055C0E0
// If a sub-object pointer is stored at +0x284, invoke its first virtual method
// with argument 1 (callee pops the argument) and then clear the stored pointer.
// Returns the callee's result; undefined when no sub-object is present.
static int __fastcall CMenuAccountEdit_ReleaseSubObject(CMenuAccountEdit* self, void* /*edx*/)
{
    int result = 0;  // the original leaves eax undefined when there is nothing to release
    void** sub = reinterpret_cast<void**>(static_cast<uintptr_t>(self->field_0284));
    if ( sub )
    {
        // slot 0 of its vtable (the deleting destructor), __thiscall with one stack argument (popped by the callee)
        result = reinterpret_cast<int(__fastcall*)(void*, void*, int)>((*reinterpret_cast<void***>(sub))[0])(sub, nullptr, 1);
        self->field_0284 = 0;
    }
    return result;
}

HOOK(0x55C0E0, CMenuAccountEdit_ReleaseSubObject, "auto");

