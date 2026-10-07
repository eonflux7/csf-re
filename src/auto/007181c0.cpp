#include "hook.h"

// Returns the control's parent pointer stored at +0xA4. When it is null, tail-jumps
// through vtable slot +0xD0 (0xD0 / 4 == 52), forwarding the caller's argument and
// registers untouched to that unknown __thiscall virtual (which sees the same stack
// layout as a direct call, since our own ret-address is still on top of the stack).
static __declspec(naked) int CGuiControl_GetParent()
{
    __asm
    {
        mov eax, [ecx + 0xa4]
        test eax, eax
        jne returnParent
        mov eax, [ecx]
        jmp dword ptr [eax + 0xd0]
    returnParent:
        ret 4
    }
}

HOOK(0x7181C0, CGuiControl_GetParent, "auto");
