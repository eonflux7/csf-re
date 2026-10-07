#include <cstdint>
#include "types/auto_classes.h"
#include "hook.h"

// 0x718880 (ret 0):
//   eax = this->m_pEffects;                  // head of the effects linked list (+0x128)
//   if (!eax) return 0;                      // returns the null pointer value
//   ecx = eax; eax = *ecx;                   // effect's vtable
//   jmp [eax + 0x58];                        // tail-call effect virtual slot 22, forwarding
//                                            // the caller's frame and returning its result
//
// The callee is reached only through a vtable slot (signature unknown), so the tail jump is
// reproduced verbatim as a naked thunk: ecx/stack/return-address pass through untouched and the
// callee's eax becomes our return value.
static __declspec(naked) void CGuiControl_DispatchHeadEffect()
{
    __asm {
        mov eax, dword ptr [ecx + 0x128]
        test eax, eax
        je  no_effect
        mov ecx, eax
        mov eax, dword ptr [ecx]
        jmp dword ptr [eax + 0x58]
    no_effect:
        ret
    }
}

HOOK(0x718880, CGuiControl_DispatchHeadEffect, "auto");
