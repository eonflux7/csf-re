#include <cstdint>
#include "hook.h"
#include "types/auto_classes.h"

// 0x00534B70
// If a sub-object pointer is stored at +0x38, invoke its first virtual method with argument 1
// and clear the stored pointer. Returns the callee's result (undefined when no sub-object).
static int __fastcall CMenuMpNetworkType_ReleaseSubObject(CMenuMpNetworkType* self, void* /*edx*/)
{
    int result;
    void** sub = reinterpret_cast<void**>(static_cast<uintptr_t>(self->field_0038));
    if ( sub )
    {
        // vtable slot 0, __thiscall with one stack argument (popped by the callee)
        result = reinterpret_cast<int(__fastcall*)(void*, void*, int)>(sub[0])(sub, nullptr, 1);
        self->field_0038 = 0;
    }
    return result;
}

HOOK(0x534B70, CMenuMpNetworkType_ReleaseSubObject, "auto");
