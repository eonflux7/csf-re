// Commandos: Strike Force (Pyro Studios) — hook implementation
// Target: 0x00480210 (__thiscall, ret 0)
//
// The original is a tiny forwarding thunk: it loads the object's vtable and
// invokes the virtual method at byte offset 0x11c (slot 71), passing the
// constant 12 as the single argument, then returns whatever that method
// returns.

#include <cstdint>
#include <cstddef>
#include "hook.h"
#include "types/auto_classes.h"

// Forward the constant 12 to virtual slot 0x11c of the object.
static int __fastcall C_00833228_InvokeVirtualSlot71With12(C_00833228* self, void* /*edx*/)
{
    // mov eax, dword ptr [ecx]      ; eax = self->vtbl
    // push 0xc                      ; arg = 12
    // call dword ptr [eax + 0x11c]  ; virtual call, callee pops the arg (ret 4)
    // ret                           ; return value propagated
    return reinterpret_cast<int(__fastcall*)(void*, void*, int)>(
               self->vtbl[0x11c / 4])(self, nullptr, 12);
}

HOOK(0x00480210, C_00833228_InvokeVirtualSlot71With12, "auto");
