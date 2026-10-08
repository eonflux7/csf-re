#include <cstdint>
#include "hook.h"
#include "types/auto_classes.h"

// 0x00541920
// If a sub-object is stored at +0x84, invoke its first virtual method with argument 1
// and then clear the stored pointer. Returns the callee's result.
static int __fastcall CMenuMpStats_ReleaseSubObject(CMenuMpStats* self, void* /*edx*/)
{
    int result;
    void** sub = reinterpret_cast<void**>(static_cast<uintptr_t>(self->field_0084));
    if ( sub )
    {
        // vtable slot 0, __thiscall with one stack argument (popped by the callee)
        result = reinterpret_cast<int(__fastcall*)(void*, void*, int)>(sub[0])(sub, nullptr, 1);
        self->field_0084 = 0;
    }
    return result;
}

HOOK(0x541920, CMenuMpStats_ReleaseSubObject, "auto");
