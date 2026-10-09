#include <cstdint>
#include "hook.h"

// Target: 0x00526A80 (22 bytes)
// __thiscall int sub_526A80(C_008411a0* this, int a2)  -> ret 4
//
// Semantics (from disassembly):
//   1. eax = this->field_0060 (offset 0x60)
//   2. if (eax == 0) -> return 0 (eax already holds the null value)
//   3. otherwise     -> tail-jump into child->vtbl[0x30 / 4 = 12]
//                       with ecx = child and the single stack argument a2.
//                       The callee pops the 4 argument bytes.
//
// The original's push ebp / mov ebp,esp / pop ebp frame is balanced and
// restored before the jump, so the thunk omits it and forwards the caller's
// stack untouched. The indirect jump target has an unknown signature, so the
// tail jump is a naked thunk.

static __declspec(naked) void C_008411a0_ForwardToChildSlot30()
{
    __asm
    {
        mov eax, dword ptr [ecx + 0x60]   // eax = this->field_0060
        test eax, eax
        je  no_child
        mov ecx, eax                      // ecx = child (this for the virtual)
        mov eax, dword ptr [ecx]          // eax = child->vtbl
        jmp dword ptr [eax + 0x30]        // tail-jump to vtbl slot 12 (ret 4)
    no_child:
        ret 4                             // return 0, pop the stack argument
    }
}

HOOK(0x526A80, C_008411a0_ForwardToChildSlot30, "auto");
