#include <cstdint>
#include "hook.h"
#include "types/auto_classes.h"

// 0x00544860
// If a sub-object pointer is stored at +0x44, invoke its first virtual method with
// argument 1 and clear the stored pointer. Returns the callee's result
// (undefined when no sub-object was stored).
static int __fastcall CMenuSaveGame_ReleaseSubObject(CMenuSaveGame* self, void* /*edx*/)
{
    int result;
    void** sub = reinterpret_cast<void**>(static_cast<uintptr_t>(self->field_0044));
    if ( sub )
    {
        // the object's first dword is its vtable; slot 0 of that vtable is the target (mov eax, [ecx]; call [eax]).
        // __thiscall with one stack argument (popped by the callee)
        void** vtable = reinterpret_cast<void**>(sub[0]);
        result = reinterpret_cast<int(__fastcall*)(void*, void*, int)>(vtable[0])(sub, nullptr, 1);
        self->field_0044 = 0;
    }
    return result;
}

HOOK(0x544860, CMenuSaveGame_ReleaseSubObject, "auto");
