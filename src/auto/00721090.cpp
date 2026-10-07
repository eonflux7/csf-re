#include <cstdint>
#include "hook.h"

// 0x00721090: pure tail-jump thunk. The indirect jmp forwards the caller's stack arguments and registers untouched
// to a target whose signature is unknown, so it stays a naked jmp (a call would drop/shift stack arguments).
static __declspec(naked) void CGuiEdit_CallEmbeddedObjectVtbl64()
{
    __asm {
        mov ecx, dword ptr [ecx + 0x134]
        mov eax, dword ptr [ecx]
        jmp dword ptr [eax + 0x64]
    }
}

HOOK(0x721090, CGuiEdit_CallEmbeddedObjectVtbl64, "auto");
