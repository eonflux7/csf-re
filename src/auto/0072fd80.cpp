#include <cstdint>
#include "types/auto_classes.h"
#include "hook.h"

// 0x72FD80 (ret 0):
//   eax = this->m_pText;                     // SWITCH's TEXTO quad, +0x148
//   if (!eax) return 0;                      // returns the null pointer value
//   ecx = eax; eax = *ecx;                   // text object's vtable
//   jmp [eax + 0x1c];                        // tail-call text virtual slot 7, forwarding
//                                            // the caller's frame and returning its result
//
// The callee is reached only through a vtable slot (signature unknown), so the tail jump is
// reproduced verbatim as a naked thunk: ecx/stack/return-address pass through untouched and the
// callee's eax becomes our return value.
static __declspec(naked) void CGuiSwitch_DispatchTextVirtualSlot7()
{
    __asm {
        mov eax, dword ptr [ecx + 0x148]
        test eax, eax
        je  no_text
        mov ecx, eax
        mov eax, dword ptr [ecx]
        jmp dword ptr [eax + 0x1c]
    no_text:
        ret
    }
}

HOOK(0x72FD80, CGuiSwitch_DispatchTextVirtualSlot7, "auto");
