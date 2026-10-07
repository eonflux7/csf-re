#include <cstdint>
#include "types/auto_classes.h"
#include "hook.h"

// 0x724CE0 (ret 0):
//   eax = this->m_pText;                     // +0x134, the text object pointer
//   if (!eax) return 0;                      // returns the null pointer value
//   ecx = eax; eax = *ecx;                   // text object's vtable
//   jmp [eax + 0x1C];                        // tail-call text virtual slot 7,
//                                            // forwarding the caller's frame and returning its result
//
// The callee is reached only through a vtable slot (signature unknown), so the tail jump is
// reproduced verbatim as a naked thunk: ecx/stack/return-address pass through untouched and the
// callee's eax becomes our return value.
static __declspec(naked) void CGuiStatic_InvokeTextVirtual()
{
    __asm {
        mov eax, dword ptr [ecx + 0x134]
        test eax, eax
        je  no_text
        mov ecx, eax
        mov eax, dword ptr [ecx]
        jmp dword ptr [eax + 0x1c]
    no_text:
        ret
    }
}

HOOK(0x724CE0, CGuiStatic_InvokeTextVirtual, "auto");
