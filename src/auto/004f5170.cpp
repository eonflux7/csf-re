#include <cstdint>
#include "hook.h"

// 0x004F5170: tail-jumps to slot 8 (vtable +0x20) of the object stored at this+4, with that object as `this`.
// The jmp forwards the caller's stack arguments untouched (the slot's signature is unknown): must stay a jmp.
static __declspec(naked) void InvokeMemberVirtualSlot8()
{
    __asm {
        mov ecx, dword ptr [ecx + 4]
        mov eax, dword ptr [ecx]
        jmp dword ptr [eax + 0x20]
    }
}

HOOK(0x4F5170, InvokeMemberVirtualSlot8, "auto");
