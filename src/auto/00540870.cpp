#include <cstdint>
#include "hook.h"
#include "types/auto_classes.h"

// 0x00540870
// If a sub-object is stored at +0x30, invoke its first virtual method with argument 1
// and clear the stored pointer. Returns the callee's result (undefined when no sub-object).
static int __fastcall CMenuMpScoreBar_ReleaseSubObject(CMenuMpScoreBar* self, void* /*edx*/)
{
    int result = 0;  // the original leaves eax undefined when there is nothing to release
    void** sub = reinterpret_cast<void**>(static_cast<uintptr_t>(self->field_0030));
    if ( sub )
    {
        // slot 0 of its vtable (the deleting destructor), __thiscall with one stack argument (popped by the callee)
        result = reinterpret_cast<int(__fastcall*)(void*, void*, int)>((*reinterpret_cast<void***>(sub))[0])(sub, nullptr, 1);
        self->field_0030 = 0;
    }
    return result;
}

HOOK(0x540870, CMenuMpScoreBar_ReleaseSubObject, "auto");

