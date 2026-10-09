#include <cstdint>
#include "hook.h"
#include "types/auto_classes.h"

// 0x00549020
// If a sub-object pointer is stored at +0x1C, invoke its first virtual method with
// argument 1 and clear the stored pointer. Returns the callee's result (undefined
// when there is no sub-object).
static int __fastcall CMenuOptions_ReleaseSubObject(CMenuOptions* self, void* /*edx*/)
{
    int result = 0;  // the original leaves eax undefined when there is nothing to release
    void** sub = reinterpret_cast<void**>(static_cast<uintptr_t>(self->field_001c));
    if ( sub )
    {
        // slot 0 of its vtable (the deleting destructor), __thiscall with one stack argument (popped by the callee)
        result = reinterpret_cast<int(__fastcall*)(void*, void*, int)>((*reinterpret_cast<void***>(sub))[0])(sub, nullptr, 1);
        self->field_001c = 0;
    }
    return result;
}

HOOK(0x549020, CMenuOptions_ReleaseSubObject, "auto");

