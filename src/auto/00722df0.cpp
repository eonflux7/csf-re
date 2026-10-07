#include "hook.h"

// Forwards to virtual slot 0x28 (index 10) of the object stored at this+0x218.
// If that pointer is null, returns 0.
static __declspec(naked) void ForwardToStoredObjectVirtualSlot40()
{
    __asm
    {
        mov eax, dword ptr [ecx + 0x218]
        test eax, eax
        jne has_target
        mov eax, 0
        ret
    has_target:
        mov ecx, eax
        mov eax, dword ptr [ecx]
        jmp dword ptr [eax + 0x28]
    }
}

HOOK(0x722DF0, ForwardToStoredObjectVirtualSlot40, "auto");
