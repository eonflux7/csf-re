#include "hook.h"

// Forwards the call to the object stored at +0x60 through its vtable slot 0x44 (17).
// If the stored pointer is null, returns 0. Stack args: none (thiscall, ret 0).
static __declspec(naked) void C_008411a0_ForwardToAttachedObject() {
    __asm {
        mov edx, dword ptr [ecx + 0x60]
        mov al, 0
        test edx, edx
        je short NoAttachedObject
        mov ecx, edx
        mov eax, dword ptr [ecx]
        jmp dword ptr [eax + 0x44]
    NoAttachedObject:
        ret
    }
}

HOOK(0x00526B80, C_008411a0_ForwardToAttachedObject, "auto");
