#include <cstdint>
#include "hook.h"

// Target: 0x00728480 (22 bytes)
// __thiscall int sub_728480(C_008657e0* self, int a2)   -> ret 4
//
// Semantics (from disassembly):
//   1. eax = self->field_0030 (offset 0x30)
//   2. if (eax == 0)  -> return 0 (the null pointer value)
//   3. otherwise      -> tail-jump into obj->vtbl[11] (offset 0x2c),
//                        with ecx = obj and the single stack argument a2.
//                        The callee pops the 4 argument bytes.
//
// The call target lives in an indirect vtable slot, so the tail jump is
// reproduced as a naked thunk that matches the original instructions.

static __declspec(naked) void C_008657e0_ForwardCallToVtblSlot11()
{
    __asm
    {
        mov eax, dword ptr [ecx + 0x30]   // obj = self->field_0030
        test eax, eax
        je  no_target
        mov ecx, eax                      // ecx = obj (this for the virtual)
        mov eax, dword ptr [ecx]          // eax = obj->vtbl
        jmp dword ptr [eax + 0x2c]        // tail-jump to vtbl slot 11 (ret 4)
    no_target:
        ret 4                             // return 0, pop the stack argument
    }
}

HOOK(0x728480, C_008657e0_ForwardCallToVtblSlot11, "auto");
