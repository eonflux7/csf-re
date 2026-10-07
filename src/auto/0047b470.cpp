#include <cstdint>
#include "hook.h"

// Original (14 bytes):
//   mov eax, [ecx+8]
//   mov ecx, [eax+0x14]
//   mov ecx, [ecx+0x44]
//   mov edx, [ecx]
//   jmp dword ptr [edx+0x70]
//
// Walks the pointer chain self->[0x08] -> [0x14] -> [0x44] and tail-jumps into
// the virtual method at vtable slot 0x70 of the final object, forwarding the
// caller's stack arguments and registers (notably eax's upper bytes and the
// untouched stack) exactly as the original does. The callee's prototype is
// unknown, so this stays a naked indirect-jmp thunk with no stack frame.
static __declspec(naked) void ForwardChainToVfunc70()
{
    __asm
    {
        mov eax, dword ptr [ecx + 8]
        mov ecx, dword ptr [eax + 0x14]
        mov ecx, dword ptr [ecx + 0x44]
        mov edx, dword ptr [ecx]
        jmp dword ptr [edx + 0x70]
    }
}

HOOK(0x47B470, ForwardChainToVfunc70, "auto");
