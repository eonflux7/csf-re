#include <cstdint>
#include "hook.h"

// The original function tail-jumps through the vtable of the object stored at this+0xa4:
//   mov ecx, [ecx+0xa4]
//   mov eax, [ecx]
//   jmp dword ptr [eax+0x90]
// The indirect target's signature is unknown, so this must stay a naked thunk that forwards
// ecx and all caller stack arguments untouched.
static __declspec(naked) void C_008442a0_InvokeInnerVirtual90()
{
    __asm
    {
        mov ecx, dword ptr [ecx + 0xa4]
        mov eax, dword ptr [ecx]
        jmp dword ptr [eax + 0x90]
    }
}

HOOK(0x0054C130, C_008442a0_InvokeInnerVirtual90, "auto");
