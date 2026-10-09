#include <cstdint>
#include "types/auto_classes.h"
#include "hook.h"

// 0x4E8C90 (ret 0):
//   eax = this->m_pGlobalSceneState;         // dword at +0x1F8C (2019 dwords)
//   if (!eax) return 0;                      // returns the null pointer value
//   ecx = eax; eax = *ecx;                   // state's vtable
//   jmp [eax + 0x1C];                        // tail-call state virtual slot 7, forwarding
//                                            // the caller's frame and returning its result
//
// The callee is reached only through a vtable slot (signature unknown), so the tail jump is
// reproduced verbatim as a naked thunk: ecx/stack/return-address pass through untouched and the
// callee's eax becomes our return value.
static __declspec(naked) void SceneLoader_ForwardPrepareToGlobalState()
{
    __asm {
        mov eax, dword ptr [ecx + 0x1F8C]
        test eax, eax
        je  no_state
        mov ecx, eax
        mov eax, dword ptr [ecx]
        jmp dword ptr [eax + 0x1C]
    no_state:
        ret
    }
}

HOOK(0x4E8C90, SceneLoader_ForwardPrepareToGlobalState, "auto");
