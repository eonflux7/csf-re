#include <cstdint>
#include "hook.h"
#include "types/auto_classes.h"

// 0x005355C0
// If a sub-object is stored at +0x18, invoke its first virtual method with argument 1
// and clear the stored pointer. Returns the callee's result (undefined when no sub-object
// is present, matching the original which returns whatever eax held).
static int __fastcall CMenuMpGameMode_ReleaseSubObject(CMenuMpGameMode* self, void* /*edx*/)
{
    int result = 0;  // the original leaves eax undefined when there is nothing to release
    void** sub = reinterpret_cast<void**>(static_cast<uintptr_t>(self->field_0018));
    if ( sub )
    {
        // slot 0 of its vtable (the deleting destructor), __thiscall with one stack argument (popped by the callee)
        result = reinterpret_cast<int(__fastcall*)(void*, void*, int)>((*reinterpret_cast<void***>(sub))[0])(sub, nullptr, 1);
        self->field_0018 = 0;
    }
    return result;
}

HOOK(0x5355C0, CMenuMpGameMode_ReleaseSubObject, "auto");

