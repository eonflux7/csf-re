#include <cstdint>
#include "hook.h"

// Target: 0x00720DF0 (25 bytes)
// __thiscall int sub_720DF0(void* self, int a2)   -> ret 4
//
// Semantics (from disassembly):
//   1. eax = self->field_0134 (offset 0x134)
//   2. if (eax == 0)  -> return 0 (the null pointer value), popping the
//                        single stack argument.
//   3. otherwise      -> tail-jump into obj->vtbl[16] (offset 0x40),
//                        with ecx = obj and the single stack argument a2.
//                        The callee pops the 4 argument bytes.
//
// The call target lives in an indirect vtable slot, so the tail jump is
// reproduced as a naked thunk matching the original's observable behaviour
// (the original's push ebp / mov ebp, esp / pop ebp pair is a no-op frame
// that cancels out before the jump and is omitted here).

static __declspec(naked) void C_00720DF0_ForwardCallToChildVtblSlot16()
{
    __asm
    {
        mov eax, dword ptr [ecx + 0x134]  // obj = self->field_0134
        test eax, eax
        je  no_target
        mov ecx, eax                      // ecx = obj (this for the virtual)
        mov eax, dword ptr [ecx]          // eax = obj->vtbl
        jmp dword ptr [eax + 0x40]        // tail-jump to vtbl slot 16 (ret 4)
    no_target:
        ret 4                             // return 0, pop the stack argument
    }
}

HOOK(0x720DF0, C_00720DF0_ForwardCallToChildVtblSlot16, "auto");
