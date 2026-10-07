#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"

// Thunk: forwards the call to the object referenced at this+4, invoking its
// virtual method at vtable offset 0x78 (slot 30) with that object as `this`.
// The original performs a tail `jmp [eax+0x78]`; here it is an ordinary
// __fastcall call so no stack bytes need to be popped.
static int __fastcall CallInnerVirtualSlot78(void* self, void* /*edx*/)
{
    void* inner = CSF_AT(void*, self, 0x4);
    void** inner_vtbl = CSF_AT(void**, inner, 0x0);
    return ((int(__fastcall*)(void*, void*))inner_vtbl[0x78 / 4])(inner, nullptr);
}

HOOK(0x6CA500, CallInnerVirtualSlot78, "auto");
