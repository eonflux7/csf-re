#include "hook.h"

// CCharActionFsm::GetCurrentState (0x004834F0), __thiscall, ret 0.
// Returns 0 when the state-machine pointer at +0x9C is null. Otherwise it
// tail-jumps to vtbl[0x4C / 4] of that object, forwarding the caller's
// stack arguments and registers unchanged, so the target's signature is unknown.
static __declspec(naked) void CharActionFsm_GetCurrentState() {
    __asm {
        mov eax, dword ptr [ecx + 0x9c]
        test eax, eax
        jne has_state
        mov eax, 0
        ret
    has_state:
        mov ecx, dword ptr [ecx + 0x9c]
        mov eax, dword ptr [ecx]
        jmp dword ptr [eax + 0x4c]
    }
}

HOOK(0x004834F0, CharActionFsm_GetCurrentState, "auto");
