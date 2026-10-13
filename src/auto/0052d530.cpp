#include <cstdint>
#include "hook.h"
#include "types/auto_classes.h"

// 0x0052D530
// If a sub-object is stored at +0x20, invoke its first virtual method with argument 1
// and clear the stored pointer. Returns the callee's result (undefined when no sub-object).
static int __fastcall CMenuProgressBar_ReleaseSubObject(CMenuProgressBar* self, void* /*edx*/)
{
    int result;
    void** sub = reinterpret_cast<void**>(static_cast<uintptr_t>(self->field_0020));
    if ( sub )
    {
        // vtable slot 0, __thiscall with one stack argument (popped by the callee)
        result = reinterpret_cast<int(__fastcall*)(void*, void*, int)>((*reinterpret_cast<void***>(sub))[0])(sub, nullptr, 1);
        self->field_0020 = 0;
    }
    return result;
}

HOOK(0x52D530, CMenuProgressBar_ReleaseSubObject, "auto");
