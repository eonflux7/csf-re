#include <cstdint>
#include "types/auto_classes.h"
#include "hook.h"

// 0x724D00 (ret 0):
//   eax = this->m_pText;             // the TEXTO text object (+0x134)
//   if (!eax) return 0;              // returns the null pointer value
//   ecx = eax; eax = *ecx;           // text object's vtable
//   jmp [eax + 0x34];                // tail-call text object virtual slot 13,
//                                    // forwarding the caller's frame and
//                                    // returning the callee's eax unchanged
//
// The callee is reached only through a vtable slot (signature unknown), so the
// tail jump is reproduced verbatim as a naked thunk: ecx/stack/return-address
// pass through untouched and the callee's eax becomes our return value.
static __declspec(naked) void CGuiStatic_InvokeTextVirtual()
{
    __asm {
        mov eax, dword ptr [ecx + 0x134]
        test eax, eax
        je  no_text
        mov ecx, eax
        mov eax, dword ptr [ecx]
        jmp dword ptr [eax + 0x34]
    no_text:
        ret
    }
}

HOOK(0x724D00, CGuiStatic_InvokeTextVirtual, "auto");
