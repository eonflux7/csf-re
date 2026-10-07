#include <cstdint>
#include "types/auto_classes.h"
#include "hook.h"

// 0x72FDE0 (ret 0):
//   eax = this->m_pText;                     // the switch's text sub-object (+0x148)
//   if (!eax) return 0;                      // returns the null pointer value (eax == 0)
//   ecx = eax; eax = *ecx;                   // text object's vtable
//   jmp [eax + 0x20];                        // tail-call text virtual slot 8, forwarding
//                                            // the caller's frame and returning its result
//
// The callee is reached only through a vtable slot (signature unknown), so the tail jump is
// reproduced verbatim as a naked thunk: ecx/stack/return-address pass through untouched and the
// callee's eax becomes our return value.
static __declspec(naked) void CGuiSwitch_DispatchTextVirtualCall()
{
    __asm {
        mov eax, dword ptr [ecx + 0x148]
        test eax, eax
        je  no_text
        mov ecx, eax
        mov eax, dword ptr [ecx]
        jmp dword ptr [eax + 0x20]
    no_text:
        ret
    }
}

HOOK(0x72FDE0, CGuiSwitch_DispatchTextVirtualCall, "auto");
