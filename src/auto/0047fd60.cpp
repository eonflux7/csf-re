#include <cstddef>
#include <cstdint>
#include "hook.h"
#include "types/auto_classes.h"

// Forwards the call to the vtable slot at +0x1E0 when the weapon FSM state
// (field_0010) equals 9. Otherwise returns with the caller's stack args popped.
// The original's push/pop ebp pair is a net no-op on the stack and ebp, so it is omitted here.
static __declspec(naked) void CCharWeaponFsm_ForwardToVSlot1E0WhenState9() {
    __asm {
        cmp dword ptr [ecx + 0x10], 9
        jne not_nine
        mov eax, dword ptr [ecx]
        jmp dword ptr [eax + 0x1e0]
    not_nine:
        ret 4
    }
}

HOOK(0x0047FD60, CCharWeaponFsm_ForwardToVSlot1E0WhenState9, "auto");
