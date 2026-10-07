#include "hook.h"

// CGuiStatic: ESTATICO (slot +0x30 -> 0x7202A0); ctor 0x720E10.
// Reads the text object at +0x134; if present, tail-jumps into its virtual
// method at vtable slot +0xC4 (0x31). Returns 0 when there is no text object.
static __declspec(naked) void CGuiStatic_ForwardToTextSlotC4()
{
    __asm
    {
        mov eax, dword ptr [ecx + 0x134]
        test eax, eax
        je no_text
        mov ecx, eax
        mov eax, dword ptr [ecx]
        jmp dword ptr [eax + 0xc4]
    no_text:
        mov eax, 0
        ret
    }
}

HOOK(0x720CF0, CGuiStatic_ForwardToTextSlotC4, "auto");
