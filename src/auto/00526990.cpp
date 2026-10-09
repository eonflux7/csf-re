#include <cstdint>
#include <cstddef>
#include "hook.h"

// Sets the flag byte at +0x3A to 1, then tail-jumps to vtable slot 0x60 (slot 24)
// of the child object at +0x60. If the child is null, returns without calling anything.
// Naked thunk: the caller's stack args and registers are forwarded unchanged.
static __declspec(naked) void __fastcall C_008411a0_MarkFlagAndForwardToChild()
{
    __asm {
        mov byte ptr [ecx + 0x3a], 1
        mov ecx, dword ptr [ecx + 0x60]
        test ecx, ecx
        je done
        mov eax, dword ptr [ecx]
        jmp dword ptr [eax + 0x60]
    done:
        ret
    }
}

HOOK(0x00526990, C_008411a0_MarkFlagAndForwardToChild, "auto");
