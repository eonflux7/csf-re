#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

// 0x004F50D0  (12 bytes, __thiscall, no stack args)
//
//   mov ecx, [ecx+4]      ; ecx = pointer stored at this+4
//   mov eax, [ecx]        ; eax = its vtable
//   jmp dword ptr [eax+0x50]
//
// A tail-jump forwarding thunk: it fetches the inner object referenced at
// offset +4 of *this and forwards the call (with that inner object as the
// __thiscall `this`) to the virtual method in vtable slot 0x50/4 = 20.
// The virtual method's return value is returned unchanged.
static int __fastcall InvokeInnerVirtualAt0x50(void* self, void* /*edx*/)
{
    void* inner = CSF_AT(void*, self, 0x4);
    void** innerVtbl = *reinterpret_cast<void***>(inner);
    return ((int(__fastcall*)(void*, void*))innerVtbl[0x50 / 4])(inner, nullptr);
}

HOOK(0x4F50D0, InvokeInnerVirtualAt0x50, "auto");
